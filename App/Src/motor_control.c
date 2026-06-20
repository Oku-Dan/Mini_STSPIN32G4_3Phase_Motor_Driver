#include "motor_control.h"
#include "main.h"
#include "math.h"

#define GET_OFFSET_REPETITION_NUM (1000)
#define GET_OFFSET_STABILIZING_TIME (10)
#define abs(X) ((X) < 0 ? -(X) : (X))
#define normalize(MIN,X,MAX) ((X) > (MAX) ? (MAX) : (X) < (MIN) ? (MIN) : (X))

__weak void cos_sin(float theta,float *cos,float *sin){
	*sin = sinf(theta);
	*cos = cosf(theta);
}

static void inv_clarke_transform(volatile float voltage_d,volatile float voltage_q,
		volatile float *voltage_alpha,volatile float *voltage_beta,float sint,float cost){
	*voltage_alpha = voltage_d * cost - voltage_q * sint;
	*voltage_beta  = voltage_d * sint + voltage_q * cost;
}

static void inv_park_transform(volatile float voltage_alpha,volatile float voltage_beta,
		volatile float *voltage_u,volatile float *voltage_v,volatile float *voltage_w){
	*voltage_u = 0.81649658f * voltage_alpha;
	*voltage_v = -0.40824829f * voltage_alpha + 0.70710678f * voltage_beta;
	*voltage_w = -0.40824829f * voltage_alpha - 0.70710678f * voltage_beta;
}

static float MC_Get_Electrical_Angle(Motor_HandleTypeDef *motor){
	float angle;
	angle = MC_Get_Mechanical_Angle(motor) * (float)motor->Init.pole_pares - motor->Init.angle_offset;
	while(angle >= PI)angle -= PI * 2.0f;
	while(angle < -PI)angle += PI * 2.0f;
	return angle;
}

static float MC_Get_Angle_Delta(float a,float b){
	if(a - b > PI){
		return a - b - 2 * PI;
	}else if(a - b < -PI){
		return a - b + 2 * PI;
	}
	return a - b;
}

void MC_Safe_State(Controller_HandleTypeDef* ctrl){
	ctrl->state = CONTROL_STATE_DISABLE;
	ctrl->voltage_d = 0;
	ctrl->voltage_q = 0;
}

void MC_Forced_Angle(Controller_HandleTypeDef* ctrl,float angle,float Vd,float Vq){
	ctrl->state = CONTROL_STATE_FORCED_COMMUTATION;
	ctrl->mech_speed_ref = 0;
	ctrl->elec_angle_ref = angle;
	ctrl->voltage_d = Vd;
	ctrl->voltage_q = Vq;
}

void MC_Forced_Speed(Controller_HandleTypeDef* ctrl,float speed,float Vd,float Vq){
	ctrl->state = CONTROL_STATE_FORCED_COMMUTATION;
	ctrl->mech_speed_ref = speed;
	ctrl->voltage_d = Vd;
	ctrl->voltage_q = Vq;
}

void MC_Voltage_Control(Controller_HandleTypeDef* ctrl,float Vd,float Vq){
	ctrl->state = CONTROL_STATE_VOLTAGE_CONTROL;
	ctrl->voltage_d = Vd;
	ctrl->voltage_q = Vq;
}

void MC_Current_Control(Controller_HandleTypeDef* ctrl,float cur_d,float cur_q){
	ctrl->state = CONTROL_STATE_CURRENT_CONTROL;
	ctrl->current_d_ref = cur_d;
	ctrl->current_q_ref = cur_q;
}

void MC_Speed_Control(Controller_HandleTypeDef* ctrl,float speed){
	ctrl->state = CONTROL_STATE_SPEED_CONTROL;
	ctrl->mech_speed_ref = speed;
}

void MC_Position_Control(Controller_HandleTypeDef* ctrl,float angle){
	ctrl->state = CONTROL_STATE_POSITION_CONTROL;
	ctrl->mech_angle_ref = angle;
}

MC_StatusTypeDef MC_Reset_Offset(Controller_HandleTypeDef* ctrl,float reset_speed,float reset_voltage){
	if(reset_voltage <= 0){
		return MC_ERROR;
	}

	ctrl->motor->Init.angle_offset = 0;
	ctrl->elec_angle_ref = 0;
	ctrl->motor->Init.parameter.connection = MOTOR_CONNECTION_UNKNOWN;
	float offset_sum = 0;
	float elec_angle = MC_Get_Electrical_Angle(ctrl->motor);
	float mech_speed_ave = 0;
	MC_Forced_Angle(ctrl,0,reset_voltage,0);
	HAL_Delay(GET_OFFSET_STABILIZING_TIME);

	for(uint16_t i = 0;i < GET_OFFSET_REPETITION_NUM;i++){
		MC_Forced_Angle(ctrl,reset_speed * ctrl->motor->Init.pole_pares * (GET_OFFSET_STABILIZING_TIME * 0.001) * i,reset_voltage,0);
		HAL_Delay(GET_OFFSET_STABILIZING_TIME);
		float new_elec_angle = MC_Get_Electrical_Angle(ctrl->motor);
		float mech_speed = MC_Get_Angle_Delta(new_elec_angle,elec_angle) / ctrl->motor->Init.pole_pares / (GET_OFFSET_STABILIZING_TIME * 0.001);
		elec_angle = new_elec_angle;
		mech_speed_ave += mech_speed / GET_OFFSET_REPETITION_NUM;
		offset_sum += MC_Get_Angle_Delta(elec_angle,ctrl->elec_angle_ref);
	}

	MC_Safe_State(ctrl);

	if(abs(mech_speed_ave - reset_speed) / reset_speed < 0.1f){
		ctrl->motor->Init.parameter.connection = MOTOR_CONNECTION_FORWARD;
	}else if(abs(mech_speed_ave + reset_speed) / reset_speed < 0.1f){
		ctrl->motor->Init.parameter.connection = MOTOR_CONNECTION_REVERSE;
	}else{
		ctrl->motor->Init.parameter.connection = MOTOR_CONNECTION_BREAK;
		return MC_ERROR;
	}

	ctrl->motor->Init.angle_offset = offset_sum / GET_OFFSET_REPETITION_NUM;
	return MC_OK;
}

static void MC_Motor_Calcurate_Current(Motor_HandleTypeDef *motor){
	motor->current_alpha = 0.81649658f * motor->cur_u - (motor->cur_v + motor->cur_w) * 0.40824829f;
	motor->current_beta  = 0.70710678f * (motor->cur_v - motor->cur_w);

	float cost,sint;
	cos_sin(motor->elec_angle,&cost,&sint);

	motor->current_d = motor->current_alpha * cost + motor->current_beta * sint;
	motor->current_q = motor->current_beta * cost - motor->current_alpha * sint;
}

static float prev_mech_angle;

void MC_Control_Update(Controller_HandleTypeDef* ctrl){
	ctrl->motor->elec_angle = MC_Get_Electrical_Angle(ctrl->motor);

	MC_Motor_Calcurate_Current(ctrl->motor);

	if(ctrl->state & CONTROL_STATE_FLAG_ENC_FDBK){
		ctrl->elec_angle_ref = ctrl->motor->elec_angle + ctrl->motor->mech_speed * ctrl->motor->Init.pole_pares * ctrl->delta * 1.5f;
	}else{
		ctrl->elec_angle_ref += ctrl->mech_speed_ref * ctrl->delta * ctrl->motor->Init.pole_pares;
	}

	while(ctrl->elec_angle_ref >= PI)ctrl->elec_angle_ref -= 2.0f * PI;
	while(ctrl->elec_angle_ref < -PI)ctrl->elec_angle_ref += 2.0f * PI;

	if(ctrl->state & CONTROL_STATE_FLAG_CALC_POS){

	}

	if(ctrl->state & CONTROL_STATE_FLAG_CALC_SPD){

	}

	if(ctrl->state & CONTROL_STATE_FLAG_CALC_CUR){

	}

	ctrl->voltage_d = normalize(-1.0f,ctrl->voltage_d,1.0f);
	ctrl->voltage_q = normalize(-1.0f,ctrl->voltage_q,1.0f);

	float cost,sint;
	cos_sin(ctrl->elec_angle_ref,&cost,&sint);
	inv_clarke_transform(ctrl->voltage_d,ctrl->voltage_q,&ctrl->voltage_alpha,&ctrl->voltage_beta,sint,cost);
	inv_park_transform(ctrl->voltage_alpha,ctrl->voltage_beta,&ctrl->voltage_u,&ctrl->voltage_v,&ctrl->voltage_w);

	ctrl->voltage_u = normalize(-1.0f,ctrl->voltage_u,1.0f);
	ctrl->voltage_v = normalize(-1.0f,ctrl->voltage_v,1.0f);
	ctrl->voltage_w = normalize(-1.0f,ctrl->voltage_w,1.0f);

	if(ctrl->state & CONTROL_STATE_FLAG_ARMED){
		ctrl->voltage_u = 0.5f + ctrl->voltage_u * 0.5f;
		ctrl->voltage_v = 0.5f + ctrl->voltage_v * 0.5f;
		ctrl->voltage_w = 0.5f + ctrl->voltage_w * 0.5f;
	}else{
		ctrl->voltage_u = 0;
		ctrl->voltage_v = 0;
		ctrl->voltage_w = 0;
	}

	ctrl->motor->mech_speed =  MC_Get_Angle_Delta(ctrl->motor->mech_angle,prev_mech_angle) / ctrl->delta;
	prev_mech_angle = ctrl->motor->mech_angle;
}

MC_StatusTypeDef MC_Motor_Init(Motor_HandleTypeDef* motor){
	if(motor->Init.pole_pares < 1){
		return MC_ERROR;
	}
	return MC_OK;
}

MC_StatusTypeDef MC_Controller_Init(Controller_HandleTypeDef* ctrl){
	if(ctrl->Init.freq < 1){
		return MC_ERROR;
	}

	ctrl->delta = 1.0f / (float)ctrl->Init.freq;
	ctrl->state = CONTROL_STATE_DISABLE;
	ctrl->motor->controller = ctrl;

	ctrl->elec_angle_ref = 0;
	ctrl->mech_speed_ref = 0;
	ctrl->mech_angle_ref = 0;
	ctrl->current_d_ref = 0;
	ctrl->current_q_ref = 0;
	ctrl->voltage_d = 0;
	ctrl->voltage_q = 0;
	ctrl->voltage_alpha = 0;
	ctrl->voltage_beta = 0;
	ctrl->voltage_u = 0;
	ctrl->voltage_v = 0;
	ctrl->voltage_w = 0;
	return MC_OK;
}