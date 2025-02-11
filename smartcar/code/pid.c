#include "zf_common_headfile.h"

float Lp=32;  	float Rp=20;
float Li=13.5;  float Ri=13.5;
float Ld=5;    float Rd=5;
uint8_t current_speed_r,current_speed_l;

void LMotor_PI(int16 target,int16 eyaw)                                                         //左电机PID
{
    // static float lMotor_Out;
    // static float llast_err; 
    // static float llast_derivative; 
    // float err;
    // current_speed_l=encoder_get_count(ENCODER1);
    // err = target-abs(current_speed_l);

    // lMotor_Out+=Lp*(err-llast_err)+Li*err+Ld*((err-llast_err)-llast_derivative);
    // llast_err=err;
    // llast_derivative=err-llast_err;

    // if(lMotor_Out> Motor_MAX)    {lMotor_Out= Motor_MAX;}
    // if(lMotor_Out< Motor_MIN)    {lMotor_Out= Motor_MIN;}

    // if (lMotor_Out>=0)
    {gpio_set_level(LMOTOR_DIR, GPIO_HIGH);pwm_set_duty(LMOTOR_PWM, target);}
    // if (lMotor_Out<0) 
    //{gpio_set_level(RMOTOR_DIR, GPIO_LOW);pwm_set_duty(LMOTOR_PWM,-lMotor_Out);}
}

void RMotor_PI(int16 target,int16 eyaw)                                                         //右电机PID
{
    // static float rMotor_Out;
    // static float rlast_err; 
    // static float rlast_derivative; 
    // float err;
    // current_speed_r=encoder_get_count(ENCODER2);
    // err = target-abs(current_speed_r);//-eyaw;

    // rMotor_Out+=Rp*(err-rlast_err)+Ri*err+Rd*((err-rlast_err)-rlast_derivative);
    // rlast_err=err;
    // rlast_derivative=err-rlast_err;
    // // if (flag.start==1)      {rMotor_Out=0;}
    // if(rMotor_Out > Motor_MAX) { rMotor_Out = Motor_MAX; }
    // if(rMotor_Out < Motor_MIN) { rMotor_Out = Motor_MIN; }

    // if (rMotor_Out>=0)      
     {gpio_set_level(RMOTOR_DIR, GPIO_HIGH);pwm_set_duty(RMOTOR_PWM, target);}
    // if (rMotor_Out<0)      
     //{gpio_set_level(RMOTOR_DIR, GPIO_LOW);pwm_set_duty(RMOTOR_PWM, -rMotor_Out);}
}

// void Speed_Judge()                                                                   //速度判断
// {
//     static float addspeed_count=0;
//     if (在直线行驶)
//     {
//         addspeed_count+=((float)current_speed_l+(float)current_speed_r)/225;
//         if (addspeed_count>10)   {tarsp = stright_tarsp;} 
//         else  tarsp=normal_tarsp;
//     } 
//     else if (!flag.lcircle&&!flag.rcircle&&!flag.slope)
//     {
//         addspeed_count=0;
//         tarsp=normal_tarsp;                                
//     }
//     if (flag.lcircle||flag.rcircle)
//     {
//         tarsp=circle_tarsp;
//     }
//     if (flag.slope)
//     {
//         tarsp=slope_tarsp;
//     } 
//     if (runflag)
//     {
//         tarsp=0;
//     }
// }