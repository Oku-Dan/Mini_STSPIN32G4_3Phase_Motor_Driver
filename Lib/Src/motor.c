#include "motor.h"
#include "calcurator.h"

static float get_elec_angle(Motor_HandleTypeDef *handle, float mech_angle)
{
    float res = (mech_angle * handle->object->pole_pares) + handle->object->angle_offset;
    while (res > 2.0f * PI) {
        res -= 2.0f * PI;
    }
    while (res < 0.0f) {
        res += 2.0f * PI;
    }
    return res;
}

static float get_elec_speed(Motor_HandleTypeDef *handle, float mech_speed)
{
    return mech_speed * handle->object->pole_pares;
}

void MotorInit(Motor_HandleTypeDef *handle)
{
    handle->controller->state = DISARMED;
    handle->controller->voltage_d = 0.0f;
    handle->controller->voltage_q = 0.0f;
    handle->controller->current_d_ref = 0.0f;
    handle->controller->current_q_ref = 0.0f;
    handle->controller->target_angle = 0.0f;
    handle->controller->target_speed = 0.0f;

    handle->mech_angle = 0.0f;
    handle->mech_speed = 0.0f;
    handle->current_u = 0.0f;
    handle->current_v = 0.0f;
    handle->current_w = 0.0f;
}

Voltage_TypeDef MotorUpdateController(Motor_HandleTypeDef *handle, float mech_angle, float mech_speed, 
    float current_u, float current_v, float current_w)
{
    float current_alpha, current_beta;
    float current_d, current_q;
    float sint, cost;
    float voltage_d, voltage_q, theta;
    float voltage_alpha, voltage_beta;
    float voltage_u, voltage_v, voltage_w;
    float elec_angle;
    float elec_speed;
    Voltage_TypeDef res;

    handle->mech_angle = mech_angle;
    handle->mech_speed = mech_speed;
    handle->current_u = current_u;
    handle->current_v = current_v;
    handle->current_w = current_w;

    elec_angle = get_elec_angle(handle, mech_angle);
    elec_speed = get_elec_speed(handle, mech_speed);
    
    cos_sin(elec_angle, &cost, &sint);
    clarke_transform(current_u, current_v, current_w, &current_alpha, &current_beta);
    park_transform(current_alpha, current_beta, &current_d, &current_q, sint, cost);

    switch (handle->controller->state) {
        case CURRENT_CONTROL:
            // Implement current control logic here
            break;
        case POSITION_CONTROL:
            // Implement position control logic here
            break;
        case SPEED_CONTROL:
            // Implement speed control logic here
            break;
        case VOLTAGE_CONTROL:
            theta = elec_angle;
            voltage_d = handle->controller->voltage_d;
            voltage_q = handle->controller->voltage_q;
            break;
        case FORCED_COMMUTATION:
            handle->controller->target_angle += handle->controller->target_speed * handle->controller->dt;
            while (handle->controller->target_angle > 2.0f * PI) {
                handle->controller->target_angle -= 2.0f * PI;
            }
            while (handle->controller->target_angle < 0.0f) {
                handle->controller->target_angle += 2.0f * PI;
            }
            theta = get_elec_angle(handle, handle->controller->target_angle);
            voltage_d = handle->controller->voltage_d;
            voltage_q = handle->controller->voltage_q;
            break;
        default:
            voltage_d = 0.0f;
            voltage_q = 0.0f;
            break;
    }

    // 非干渉化いれる

    cos_sin(theta, &cost, &sint);
    inv_park_transform(voltage_d, voltage_q, &voltage_alpha, &voltage_beta, sint, cost);
    inv_clarke_transform(voltage_alpha, voltage_beta, &voltage_u, &voltage_v, &voltage_w);

    res.u = voltage_u;
    res.v = voltage_v;
    res.w = voltage_w;

    return res;
}

void MotorSetForcedSpeed(Motor_HandleTypeDef *handle, float speed, float voltage)
{
    handle->controller->target_speed = speed;
    handle->controller->target_angle = handle->mech_angle;
    handle->controller->voltage_d = voltage;
    handle->controller->voltage_q = 0.0f;
    handle->controller->state = FORCED_COMMUTATION;
}

void MotorSetForcedAngle(Motor_HandleTypeDef *handle, float angle, float voltage)
{
    handle->controller->target_speed = 0.0f;
    handle->controller->target_angle = angle;
    handle->controller->voltage_d = voltage;
    handle->controller->voltage_q = 0.0f;
    handle->controller->state = FORCED_COMMUTATION;
}


void MotorSetVoltage(Motor_HandleTypeDef *handle, float voltage_d, float voltage_q)
{
    handle->controller->voltage_d = voltage_d;
    handle->controller->voltage_q = voltage_q;
    handle->controller->state = VOLTAGE_CONTROL;
}

void MotorSetTargetSpeed(Motor_HandleTypeDef *handle, float speed)
{
    handle->controller->target_speed = speed;
    handle->controller->state = SPEED_CONTROL;
}

void MotorSetTargetAngle(Motor_HandleTypeDef *handle, float angle)
{
    handle->controller->target_angle = angle;
    handle->controller->state = POSITION_CONTROL;
}

void MotorSetStop(Motor_HandleTypeDef *handle)
{
    handle->controller->state = DISARMED;
}