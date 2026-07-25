#include "calcurator.h"

const static float sqrt3 = 1.73205081f;//sqrtf(3.0f);
const static float inv_sqrt3 = 1.0f / sqrt3;

__weak void cos_sin(float theta,float *cos,float *sin){
    *sin = sinf(theta);
    *cos = cosf(theta);
}

__weak void park_transform(float current_alpha,float current_beta,
        float *current_d,float *current_q,float sint,float cost){
    *current_d = current_alpha * cost + current_beta * sint;
    *current_q = -current_alpha * sint + current_beta * cost;
}

__weak void inv_park_transform(float voltage_d,float voltage_q,
        float *voltage_alpha,float *voltage_beta,float sint,float cost){
    *voltage_alpha = voltage_d * cost - voltage_q * sint;
    *voltage_beta  = voltage_d * sint + voltage_q * cost;
}

__weak void clarke_transform(float current_u,float current_v,float current_w,
        float *current_alpha,float *current_beta){
    *current_alpha = current_u;
    *current_beta  = inv_sqrt3 * (current_u + 2 * current_v);
}

__weak void inv_clarke_transform(float voltage_alpha,float voltage_beta,
        float *voltage_u,float *voltage_v,float *voltage_w){
	*voltage_u = voltage_alpha;
	*voltage_v = 0.5f * (-voltage_alpha + sqrt3 * voltage_beta);
	*voltage_w = 0.5f * (-voltage_alpha - sqrt3 * voltage_beta);
}