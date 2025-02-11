#include "zf_common_headfile.h"

int main(void)
{

    encoder_dir_init(ENCODER1, ENCODER1_QUADDEC_A, ENCODER1_QUADDEC_B);       
    encoder_dir_init(ENCODER2, ENCODER2_QUADDEC_A, ENCODER2_QUADDEC_B);       
    gpio_init(LMOTOR_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            
    gpio_init(RMOTOR_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                           
    pwm_init(RMOTOR_PWM, 17000, 0);  
    pwm_init(LMOTOR_PWM, 17000, 0);  
    
                                                     
    clock_init(SYSTEM_CLOCK_160M); 	                                            // 时钟配置及系统初始化<务必保留>
    debug_info_init();                                                          // 调试串口信息初始化
    
    tft180_set_dir(TFT180_CROSSWISE);                                           // 需要先横屏 不然显示不下
    tft180_init();
    tft180_show_string(0, 0, "mt9v03x init.");

    while(1)
    {
        if(mt9v03x_init())
            tft180_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // 闪灯表示异常
    }

    tft180_show_string(0, 16, "init success.");

    while(true)
    {
        if(mt9v03x_finish_flag)
        {       
            image_process();
            mt9v03x_finish_flag = 0;
        }
    }
}