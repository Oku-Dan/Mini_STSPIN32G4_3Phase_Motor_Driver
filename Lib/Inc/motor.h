#ifndef MOTOR_H
#define MOTOR_H

#include "typedef.h"

void MotorInit(Motor_HandleTypeDef *handle);
Voltage_TypeDef MotorUpdateController(Motor_HandleTypeDef *handle, float mech_angle, float mech_speed, float current_u, float current_v, float current_w);

void MotorSetForcedSpeed(Motor_HandleTypeDef *handle, float speed, float voltage);
void MotorSetForcedAngle(Motor_HandleTypeDef *handle, float angle, float voltage);
void MotorSetVoltage(Motor_HandleTypeDef *handle, float voltage_d, float voltage_q);
void MotorSetTargetSpeed(Motor_HandleTypeDef *handle, float speed);
void MotorSetTargetAngle(Motor_HandleTypeDef *handle, float angle);
void MotorSetStop(Motor_HandleTypeDef *handle);

#endif // MOTOR_H