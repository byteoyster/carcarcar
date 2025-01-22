#include "zf_common_headfile.h"

int main(void)
{
    clock_init(SYSTEM_CLOCK_160M); 	// ʱ�����ü�ϵͳ��ʼ��<��ر���>
    debug_info_init();              // ���Դ�����Ϣ��ʼ��
    
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
            Gate = GetOSTU(mt9v03x_image);
            tft180_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, 160, 128, Gate);
            mt9v03x_finish_flag = 0;
        }
    }
}