#ifndef TYPDEF_H
#define TYPDEF_H

typedef enum{
	DISARMED,
	FORCED_COMMUTATION,
    VOLTAGE_CONTROL,
    CURRENT_CONTROL,
    SPEED_CONTROL,
    POSITION_CONTROL,
}Contoller_StateTypeDef;

typedef struct{
	float R;
	float Lq;
	float Ld;
	float Ke;
}Motor_CharactaeristicTypeDef;

typedef struct{
	unsigned char pole_pares;
	float angle_offset;
	const Motor_CharactaeristicTypeDef * const parameter;
}Motor_InitTypeDef;

typedef struct{
    const float dt;
    Contoller_StateTypeDef state;
    float target_angle;
    float target_speed;
    float current_d_ref;
    float current_q_ref;
    float voltage_d;
    float voltage_q;
}Motor_ControllerTypeDef;

typedef struct{
    float u;
    float v;
    float w;
}Voltage_TypeDef;

typedef struct{
	const Motor_InitTypeDef * const object;
    Motor_ControllerTypeDef * const controller;
	float mech_angle;
	float mech_speed;
    float current_u;
    float current_v;
    float current_w;
}Motor_HandleTypeDef;

#endif // TYPDEF_H