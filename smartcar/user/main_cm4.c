#include "zf_common_headfile.h"

int main(void)
{
    clock_init(SYSTEM_CLOCK_160M); 	// 时钟配置及系统初始化<务必保留>
    debug_info_init();              // 调试串口信息初始化
    
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