#ifndef CALCURATOR_H
#define CALCURATOR_H

#include <math.h>

#ifndef __weak
#define __weak __attribute__((weak))
#endif

#define PI (float)(M_PI)

#define abs(X) ((X) < 0 ? -(X) : (X))
#define normalize(MIN,X,MAX) ((X) > (MAX) ? (MAX) : (X) < (MIN) ? (MIN) : (X))

void cos_sin(float theta,float *cos,float *sin);

void park_transform(float voltage_alpha,float voltage_beta,
        float *voltage_d,float *voltage_q,float sint,float cost);

void inv_park_transform(float voltage_d,float voltage_q,
		float *voltage_alpha,float *voltage_beta,float sint,float cost);

void clarke_transform(float voltage_u,float voltage_v,float voltage_w,
		float *voltage_alpha,float *voltage_beta);

void inv_clarke_transform(float voltage_alpha,float voltage_beta,
		float *voltage_u,float *voltage_v,float *voltage_w);

#endif // CALCURATOR_H