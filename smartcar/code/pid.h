#ifndef MOTOR_H
#define MOTOR_H

#include "zf_common_headfile.h"

#define Motor_MAX 5000
#define Motor_MIN -5000

#define LMOTOR_DIR P00_2
#define LMOTOR_PWM TCPWM_CH13_P00_3

#define RMOTOR_DIR P18_6
#define RMOTOR_PWM TCPWM_CH50_P18_7

#define ENCODER1                     	(TC_CH09_ENCODER)                            
#define ENCODER1_QUADDEC_A             	(TC_CH09_ENCODER_CH1_P05_0)                                
#define ENCODER1_QUADDEC_B            	(TC_CH09_ENCODER_CH2_P05_1)                                 
                                                                                
#define ENCODER2                     	(TC_CH07_ENCODER)                     
#define ENCODER2_QUADDEC_A            	(TC_CH07_ENCODER_CH1_P02_0)                        
#define ENCODER2_QUADDEC_B            	(TC_CH07_ENCODER_CH2_P02_1) 

void Motor_Init();
void START_CAR();
void Motor_Forward(int lv,int rv);
void Get_Speed(void);
void LMotor_PI(int16 target,int16 eyaw);
void RMotor_PI(int16 target,int16 eyaw);

void Speed_Judge();
#endif
