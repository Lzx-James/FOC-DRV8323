//
// Created by pc on 2024/1/23.
//

#include "foc_ctrl.h"
#include "AS5600.h"
#include "math.h"
#include "tim.h"
#include "stdio.h"
#include "pid.h"


foc_motor_t Motor_2208;

float _SIN(float theta) {
    return sin(theta);
}

float _COS(float theta) {
    return cos(theta);
}

#define SQRT_3 1.73205080757f

void get_Angle(foc_motor_t *__obj, float AbsoulteAngle) {
    __obj->machine_angle = AbsoulteAngle - __obj->Encoder_ZeroPoint;

    if (__obj->machine_angle < 0) {
        __obj->machine_angle += (2 * PI);
    }

    __obj->electron_angle = __obj->polar_pair * __obj->machine_angle;
}

// Feedback Part

void Current_Sample(foc_motor_t *__obj) {}

void Clark_Trans(foc_motor_t *__obj) {}

void Park_Trans(foc_motor_t *__obj) {}

// Output Part

//TODO 编写PID控制函数

void Inv_Park_Trans(foc_motor_t *__obj) {
//TODO 后续引入PID反馈
    float theta = __obj->electron_angle;
    __obj->inv_park_U_alpha = _COS(theta) * __obj->current_loop_Ud - _SIN(theta) * __obj->current_loop_Uq;
    __obj->inv_park_U_beta = _SIN(theta) * __obj->current_loop_Ud + _COS(theta) * __obj->current_loop_Uq;
}

void SVPWM_Trans(foc_motor_t *__obj) {
//    float Udc = sqrtf(powf(__obj->inv_park_U_alpha, 2) + powf(__obj->inv_park_U_beta, 2)); // 总电压向量模值，即输出电压大小
    float Udc = __obj->Udc; // 供电电压大小
    float Ual = __obj->inv_park_U_alpha, Ubt = __obj->inv_park_U_beta;
    float Ts = 1800.0f;// PWM周期长度
    float X, Y, Z; //扇区时间合成基准
    uint8_t A, B, C, N; // 扇区判断码
    float Ta, Tb, Tc, t1, t2; //扇区时间计算中间变量

    X = SQRT_3 * Ts * Ubt / Udc;
    Y = SQRT_3 * Ts * (SQRT_3 * Ual + Ubt) / Udc / 2;
    Z = SQRT_3 * Ts * (-SQRT_3 * Ual + Ubt) / Udc / 2;

    if (Ubt < -SQRT_3 * Ual) A = 1; else A = 0;
    if (Ubt < SQRT_3 * Ual) B = 1; else B = 0;
    if (Ubt > 0) C = 1; else C = 0;
    N = 4 * A + 2 * B + C;

//    315462 分别代表 1-6扇区
    if (N == 3) {
        t1 = -Z;
        t2 = X;
        Ta = 0.5f * (Ts - t1 - t2);
        Tb = Ta + t1;
        Tc = Tb + t2;
        __obj->svpwm_Ua = (uint16_t) (Ts - Ta);
        __obj->svpwm_Ub = (uint16_t) (Ts - Tb);
        __obj->svpwm_Uc = (uint16_t) (Ts - Tc);

    } else if (N == 1) {
        t1 = Z;
        t2 = Y;
        Ta = 0.5f * (Ts - t1 - t2);
        Tb = Ta + t1;
        Tc = Tb + t2;
        __obj->svpwm_Ua = (uint16_t) (Ts - Tb);
        __obj->svpwm_Ub = (uint16_t) (Ts - Ta);
        __obj->svpwm_Uc = (uint16_t) (Ts - Tc);

    } else if (N == 5) {
        t1 = X;
        t2 = -Y;
        Ta = 0.5f * (Ts - t1 - t2);
        Tb = Ta + t1;
        Tc = Tb + t2;
        __obj->svpwm_Ua = (uint16_t) (Ts - Tc);
        __obj->svpwm_Ub = (uint16_t) (Ts - Ta);
        __obj->svpwm_Uc = (uint16_t) (Ts - Tb);

    } else if (N == 4) {
        t1 = -X;
        t2 = Z;
        Ta = 0.5f * (Ts - t1 - t2);
        Tb = Ta + t1;
        Tc = Tb + t2;
        __obj->svpwm_Ua = (uint16_t) (Ts - Tc);
        __obj->svpwm_Ub = (uint16_t) (Ts - Tb);
        __obj->svpwm_Uc = (uint16_t) (Ts - Ta);

    } else if (N == 6) {
        t1 = -Y;
        t2 = -Z;
        Ta = 0.5f * (Ts - t1 - t2);
        Tb = Ta + t1;
        Tc = Tb + t2;
        __obj->svpwm_Ua = (uint16_t) (Ts - Tb);
        __obj->svpwm_Ub = (uint16_t) (Ts - Tc);
        __obj->svpwm_Uc = (uint16_t) (Ts - Ta);

    } else {//N == 2
        t1 = Y;
        t2 = -X;
        Ta = 0.5f * (Ts - t1 - t2);
        Tb = Ta + t1;
        Tc = Tb + t2;
        __obj->svpwm_Ua = (uint16_t) (Ts - Ta);
        __obj->svpwm_Ub = (uint16_t) (Ts - Tc);
        __obj->svpwm_Uc = (uint16_t) (Ts - Tb);

    }
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, __obj->svpwm_Ua);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, __obj->svpwm_Ub);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, __obj->svpwm_Uc);

}

//Functional Part

//此函数用来直接设定力矩
void setTorque(foc_motor_t *__obj, float Uq) {
    __obj->current_loop_Ud = 0;
    __obj->current_loop_Uq = Uq;
}

/* 角度边界处理 */
//#define BOUNDARY_PROCESS(var, ref, exp, max)  \
//    if ((ref) - (exp) >= (max / 2)) {         \
//        var = (ref)-max;                      \
//    } else if ((ref) - (exp) <= -(max / 2)) { \
//        var = (ref) + max;                    \
//    } else {                                  \
//        var = (ref);                          \
//    }
float boundary_process(float ref, float exp, float max) {
    if (ref - exp >= (max / 2)) {
        return ref - max;
    } else if (ref - exp <= -max / 2) {
        return ref + max;
    } else {
        return ref;
    }
}

pid_type_t position_loop;

void FOC_Init(foc_motor_t *__obj, float polar_pair, float Encoder_ZP, float Udc) {
    __obj->polar_pair = polar_pair;
    __obj->machine_angle = 0;
    __obj->electron_angle = 0;
    __obj->Encoder_ZeroPoint = Encoder_ZP;
    __obj->Udc = Udc;

    pid_init(&position_loop, PID_POSITION, 10, 0, 0, __obj->Udc, __obj->Udc * 0.8f);
}

void FOC_Run(foc_motor_t *__obj) {
    get_Angle(__obj, 2.0f * PI * as5600GetRawAngle() / 4096);
    static float T_count = 0;
    T_count += 0.07;
//    static float target_angle = 1.5*_SIN(T_count) * _SIN(0.05*T_count);//expf(-0.01*T_count);
    static float target_angle = 0;//expf(-0.01*T_count);


//    float angle_bias = boundary_process(__obj->machine_angle, 0, 2 * PI);
//    float torque = pid_calc(&position_loop,  angle_bias, target_angle);

    target_angle = (PI * 60 / 180);

//    float angle_bias = boundary_process(__obj->machine_angle, target_angle, 2 * PI);
//    float torque = pid_calc(&position_loop,  angle_bias, target_angle);

//    setTorque(__obj, torque);
    setTorque(__obj, 8);

//    printf("%.2f, %.2f, %.2f\n", 180 / PI * __obj->Encoder_ZeroPoint, 180 / PI * __obj->machine_angle, target_angle);
//    printf("%.2f, %.2f, %.2f\n", angle_bias, target_angle, torque);

    Inv_Park_Trans(__obj);
    SVPWM_Trans(__obj);
//    printf("%d,%d,%d\n", __obj->svpwm_Ua, __obj->svpwm_Ub, __obj->svpwm_Uc);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    static uint8_t a = 0;
    FOC_Run(&Motor_2208);
}