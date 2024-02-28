//
// Created by pc on 2024/1/23.
//

#include "main.h"

#ifndef FOC_V1_2_FOC_CTRL_H
#define FOC_V1_2_FOC_CTRL_H

#define PI 3.14159265358979323846f

typedef struct __FOC_MOTOR_CONTROL{
    float polar_pair;
    float machine_angle;
    float electron_angle;
    float Encoder_ZeroPoint;
    float Udc;

//    output
    float current_loop_Ud;
    float current_loop_Uq;

    float inv_park_U_alpha;
    float inv_park_U_beta;

    uint16_t svpwm_Ua;
    uint16_t svpwm_Ub;
    uint16_t svpwm_Uc;

#if 0
//    input
    float Phase_Ia;
    float Phase_Ib;
    float Phase_Ic;

    float clark_I_alpha;
    float clark_I_beta;

    float park_Id;
    float park_Iq;
#endif

}foc_motor_t;

void setTorque(foc_motor_t *__obj, float Uq);

void FOC_Init(foc_motor_t *__obj, float polar_pair, float Encoder_ZP, float Udc);

void FOC_Run(foc_motor_t *__obj);

extern foc_motor_t Motor_2208;

#endif //FOC_V1_2_FOC_CTRL_H
