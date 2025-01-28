#ifndef _CAMERA_H
#define _CAMERA_H

#include "zf_common_headfile.h"

#define bin_jump_num	1//跳过的点数
#define border_max	MT9V03X_W-2 //边界最大值
#define border_min	1	//边界最小值	

// uint8 GetOSTU(uint8 tmImage[MT9V03X_H][MT9V03X_W]);
void Get_Pixle(void);

extern uint8 Gate;
extern uint8 original_image[MT9V03X_H][MT9V03X_W];
extern uint8 bin_image[MT9V03X_H][MT9V03X_W];//图像数组
extern void image_process(void); //直接在中断或循环里调用此程序就可以循环执行了

#endif