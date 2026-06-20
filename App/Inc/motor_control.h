#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "stdint.h"
#include "math.h"

#define PI (float)(M_PI)

#define CONTROL_STATE_FLAG_ARMED 	(1U << 0)
#define CONTROL_STATE_FLAG_ENC_FDBK	(1U << 1)
#define CONTROL_STATE_FLAG_CALC_CUR	(1U << 2)
#define CONTROL_STATE_FLAG_CALC_SPD	(1U << 3)
#define CONTROL_STATE_FLAG_CALC_POS	(1U << 4)

typedef enum{
	CONTROL_STATE_DISABLE 			 = 0x00U,
	CONTROL_STATE_FORCED_COMMUTATION = CONTROL_STATE_FLAG_ARMED,
	CONTROL_STATE_VOLTAGE_CONTROL    = CONTROL_STATE_FLAG_ARMED | CONTROL_STATE_FLAG_ENC_FDBK,
	CONTROL_STATE_CURRENT_CONTROL    = CONTROL_STATE_FLAG_ARMED | CONTROL_STATE_FLAG_ENC_FDBK | CONTROL_STATE_FLAG_CALC_CUR,
	CONTROL_STATE_SPEED_CONTROL    	 = CONTROL_STATE_FLAG_ARMED | CONTROL_STATE_FLAG_ENC_FDBK | CONTROL_STATE_FLAG_CALC_SPD,
	CONTROL_STATE_POSITION_CONTROL   = CONTROL_STATE_FLAG_ARMED | CONTROL_STATE_FLAG_ENC_FDBK | CONTROL_STATE_FLAG_CALC_SPD | CONTROL_STATE_FLAG_CALC_POS,
}Contoller_StateTypeDef;

typedef enum
{
  MC_OK,
  MC_ERROR,
  MC_BUSY,
  MC_TIMEOUT,
}MC_StatusTypeDef;

typedef enum{
	MOTOR_CONNECTION_UNKNOWN,
	MOTOR_CONNECTION_BREAK,
	MOTOR_CONNECTION_FORWARD,
	MOTOR_CONNECTION_REVERSE,
}Motor_ConnentionTypeDef;

typedef struct{
	float R;
	float L;
	float Ke;
	Motor_ConnentionTypeDef connection;
}Motor_CharactaeristicTypeDef;

typedef struct{
	float A0;
	float A1;
	float A2;
	float state[3];
	float Kp;
	float Ki;
	float Kd;
}Controller_PID_Instance; //aka arm_pid_instance_f32

typedef struct{
	uint8_t pole_pares;
	float angle_offset;
	Motor_CharactaeristicTypeDef parameter;
}Motor_InitTypeDef;

typedef struct _Controller_HandleTypeDef Controller_HandleTypeDef;

typedef struct{
	Motor_InitTypeDef Init;
	volatile float mech_angle;
	volatile float elec_angle;
	volatile float mech_speed;
	volatile float cur_u;
	volatile float cur_v;
	volatile float cur_w;
	volatile float current_alpha;
	volatile float current_beta;
	volatile float current_d;
	volatile float current_q;
	Controller_HandleTypeDef *controller;
}Motor_HandleTypeDef;

typedef struct{
	uint32_t freq;
	Controller_PID_Instance *pid_pos;
	Controller_PID_Instance *pid_spd;
	Controller_PID_Instance *pid_cur_d;
	Controller_PID_Instance *pid_cur_q;
}Controller_InitTypeDef;

struct _Controller_HandleTypeDef{
	float delta;
	Controller_InitTypeDef Init;
	Motor_HandleTypeDef *motor;
	volatile Contoller_StateTypeDef state;
	volatile float elec_angle_ref;
	volatile float mech_speed_ref;
	volatile float mech_angle_ref;
	volatile float current_d_ref;
	volatile float current_q_ref;
	volatile float voltage_d;
	volatile float voltage_q;
	volatile float voltage_alpha;
	volatile float voltage_beta;
	volatile float voltage_u;
	volatile float voltage_v;
	volatile float voltage_w;
};

void cos_sin(float,float*,float*);

void MC_Set_Safe_State(Controller_HandleTypeDef*);
void MC_Set_Forced_Speed(Controller_HandleTypeDef*,float,float,float);
void MC_Set_Forced_Angle(Controller_HandleTypeDef*,float,float,float);
void MC_Set_Voltage_Control(Controller_HandleTypeDef*,float,float);
void MC_Set_Current_control(Controller_HandleTypeDef*,float,float);
void MC_Set_Speed_Control(Controller_HandleTypeDef*,float);
void MC_Set_Position_Control(Controller_HandleTypeDef*,float);

MC_StatusTypeDef MC_Calibrate_Offset(Controller_HandleTypeDef*,float,float);
void MC_Control_Update(Controller_HandleTypeDef*);


MC_StatusTypeDef MC_Motor_Init(Motor_HandleTypeDef*);
MC_StatusTypeDef MC_Controller_Init(Controller_HandleTypeDef*);
float MC_Get_Mechanical_Angle(Motor_HandleTypeDef *motor);

#endif // MOTOR_CONTROL_H