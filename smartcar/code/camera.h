#ifndef _CAMERA_H_
#define _CAMERA_H_


#include "zf_common_headfile.h"


//宏定义
#define image_h	120//图像高度
#define image_w	188//图像宽度

#define white_pixel	255
#define black_pixel	0

#define bin_jump_num	1//跳过的点数
#define border_max	image_w-2 //边界最大值
#define border_min	1	//边界最小值	
// extern uint8 original_image[image_h][image_w];
// extern uint8 bin_image[image_h][image_w];//图像数组

void image_process(void); //直接在中断或循环里调用此程序就可以循环执行了

#define MAP_X(x) ((x) * 160 / 188)
#define MAP_Y(y) ((y) * 128 / 120)

#endif 
