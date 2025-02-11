#include "zf_common_headfile.h"

int main(void)
{

    encoder_dir_init(ENCODER1, ENCODER1_QUADDEC_A, ENCODER1_QUADDEC_B);       
    encoder_dir_init(ENCODER2, ENCODER2_QUADDEC_A, ENCODER2_QUADDEC_B);       
    gpio_init(LMOTOR_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            
    gpio_init(RMOTOR_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                           
    pwm_init(RMOTOR_PWM, 17000, 0);  
    pwm_init(LMOTOR_PWM, 17000, 0);  
    
                                                     
    clock_init(SYSTEM_CLOCK_160M); 	                                            // ʱ�����ü�ϵͳ��ʼ��<��ر���>
    debug_info_init();                                                          // ���Դ�����Ϣ��ʼ��
    
    tft180_set_dir(TFT180_CROSSWISE);                                           // ��Ҫ�Ⱥ��� ��Ȼ��ʾ����
    tft180_init();
    tft180_show_string(0, 0, "mt9v03x init.");

    while(1)
    {
        if(mt9v03x_init())
            tft180_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // ���Ʊ�ʾ�쳣
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