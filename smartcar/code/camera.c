#include "zf_common_headfile.h"

int bin_image[image_h][image_w];//图像数组
int track_wide[image_h];//赛宽数组
int l_border[image_h];//左线数组
int r_border[image_h];//右线数组
int center_line[image_h];//中线数组
int qiguai;
int yuansu;//1为左，2为右
//寻找起始点，从左往右，从下到上遍历
//直线
int start_flag = 0;//最下方起始点标志位
int left_h_find = 0;//每一行的左边起始点标志位
int wide_num[image_h];//每一行的赛宽数量
int circle_wide_flag = 0;//环岛标志位，用于入环选取偏差

int hightest = 0;
int elemin = 0;
int make_flag = 0;

int my_abs(int value)
{
    if (value >= 0) return value;
    else return -value;
}
int my_min(int x, int y)
{
    if (x >= y)             return 1;
    else if (x < y)       return 0;
    else return 2;
}
int my_max(int x, int y)
{
    if (x >= y)             return 0;
    else if (x < y)       return 1;
    else return 2;
}

int16 limit_a_b(int16 x, int a, int b)
{
    if(x<a) x = a;
    if(x>b) x = b;
    return x;
}

/*变量声明*/
uint8 original_image[image_h][image_w];
uint8 image_thereshold;//图像分割阈值

void Get_image(uint8(*mt9v03x_image)[image_w])
{
#define use_num		1	//1就是不压缩，2就是压缩一倍	
	uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < image_h; i += use_num)          
    {
        for (j = 0; j <image_w; j += use_num)     
        {
            original_image[row][line] = mt9v03x_image[i][j];
			line++;
        }
        line = 0;
        row++;
    }
}

uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)//动态阈值
{
#define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};
	
	uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;
	
    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }
    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
   return Threshold;
}

void turn_to_bin(void)//图像二值化
{
  uint8 i,j;
 image_thereshold = otsuThreshold(original_image[0], image_w, image_h);
  for(i = 0;i<image_h;i++)
  {
      for(j = 0;j<image_w;j++)
      {
          if(original_image[i][j]>image_thereshold)bin_image[i][j] = white_pixel;
          else bin_image[i][j] = black_pixel;
      }
  }
}

void search_line()
{
    elemin = image_w / 2;
    circle_wide_flag = 0;
    for(int i = image_h - 4; i > 30; i-=2)//遍历列
    {
        left_h_find = 0;//初始清零
        wide_num[i] = 0;//初始化
        make_flag = 0;
        for (int j = 50; j < image_w - 50; j+=2)//遍历行
        {
            if (bin_image[i][j] == black_pixel && bin_image[i][j + 1] == white_pixel)//由黑到白的跳变点
            {
                l_border[i] = j;
                left_h_find = 1;//左边起始点已经找到
                if (start_flag == 0 && i < image_h - 10)start_flag = 1;//已经找到最下方起始点,并且已经合成了一部分中线
            }
            else if (bin_image[i][j] == white_pixel && bin_image[i][j + 1] == black_pixel)//由白到黑的跳变点
            {
                r_border[i] = j;
                //  if(left_h_find==0)l_border[i]=4;//给右边线进行赋值
                track_wide[i] = my_abs(r_border[i] - l_border[i]);//记录赛宽
                wide_num[i]++;//此行的赛宽数量加一
            }
            if (l_border[i] == 0 && r_border[i] == 0)//如果左右边线均未找到则强制赋值
            {
                l_border[i] = 1;
                r_border[i] = image_w - 1;
            }
            if (wide_num[i] != 0)hightest = i;//记录最高点
            if (wide_num[i] == 2 && start_flag == 0 && wide_num[i] != make_flag)//图像下方一定位置采用与elemin做参照
            {
                if (my_min(my_abs(center_line[i] - elemin), my_abs((l_border[i] + r_border[i]) / 2 - elemin)) == 0)//若一行出现两个赛宽则取偏差比较小的
                    center_line[i] = center_line[i];
                else center_line[i] = (l_border[i] + r_border[i]) / 2;
                   make_flag = wide_num[i];
            }
            else if (wide_num[i] == 2 && start_flag == 1 && circle_wide_flag != 3 && wide_num[i] != make_flag)//正常情况下与上一个点做参照
            {
                if (my_min(my_abs(center_line[i] - center_line[i + 100]), my_abs((l_border[i] + r_border[i]) / 2 - center_line[i + 100])) == 0)//若一行出现两个赛宽则取偏差比较小的
                    center_line[i] = center_line[i];
                else center_line[i] = (l_border[i] + r_border[i]) / 2;
                make_flag = wide_num[i];
            }
            // else if (wide_num[i] == 2 && circle_wide_flag==3&&wide_num[i] != make_flag)//入环
            // {
            //     if ((my_max(my_abs(center_line[i] - center_line[i + 50]), my_abs((l_border[i] + r_border[i]) / 2 - center_line[i + 50])) == 0&&yuansu==0)||yuansu==1)
            //     {
            //         yuansu = 1; center_line[i] = center_line[i];
            //     }
            //     if ((my_max(my_abs(center_line[i] - center_line[i + 50]), my_abs((l_border[i] + r_border[i]) / 2 - center_line[i + 50])) == 1&&yuansu==0)||yuansu == 2)
            //     {
            //         yuansu = 2; center_line[i] = (l_border[i] + r_border[i]) / 2;
            //     }
            //    /* printf("\n%d", i);*/
            //       make_flag = wide_num[i];
            // }
            else if (wide_num[i]!=make_flag)
            {
                center_line[i] = (l_border[i] + r_border[i]) >> 1;//左右边线合成中线
                make_flag = wide_num[i];
            }
            else if (j > image_w - 3 && wide_num[i] == 0)
            {
                center_line[i] = (l_border[i] + r_border[i]) >> 1;//左右边线合成中线
                make_flag = 1;
            }
            if (start_flag == 0)start_flag = 1;//已经找到最下方起始点
			tft180_draw_point(l_border[i], i, RGB565_RED);
			tft180_draw_point(r_border[i], i, RGB565_RED);
			tft180_draw_point(center_line[i], i, RGB565_RED);

         } 
        if (wide_num[i] == 2 && circle_wide_flag == 0)circle_wide_flag = 1;//疑似环岛//环岛特征为两个赛宽变成一个赛宽再往上又是俩个赛宽
        if (wide_num[i] == 1 && circle_wide_flag == 1)circle_wide_flag = 2;
        if (wide_num[i] == 2 && circle_wide_flag == 2) circle_wide_flag = 3; 
    }
}

void image_process(void)
{
int16 error = center_line[image_h / 2] - (image_w / 2);
// uint16 i;


/*这是离线调试用的*/
Get_image(mt9v03x_image);
turn_to_bin();

//显示图像 
// tft180_displayimage03x((const uint8 *)mt9v03x_image, 160, 128); 
tft180_show_gray_image(0,0,(const uint8*)mt9v03x_image,188,120,160,128,otsuThreshold((uint8*)mt9v03x_image,120,188));
// tft180_clear();

 search_line();

	static float integral_error = 0;
		integral_error += error;
		static float last_error = 0;
        float derivative = error - last_error;
        last_error = error;
        float adjust = 20 * error + 0.02 * integral_error + 0 * derivative;
        LMotor_PI(1600 + adjust, 0);
        RMotor_PI(1600 - adjust, 0);
		if (l_border[image_h / 2] == border_min)  // 左直角弯
		{
			system_delay_ms(650);
    		LMotor_PI(0, 0);  // 左轮倒转
    		RMotor_PI(1600, 0);   // 右轮加速
		}
		else if (r_border[image_h / 2] == border_max)  // 右直角弯
		{
    		LMotor_PI(1600, 0);   // 左轮加速
    		RMotor_PI(0, 0);  // 右轮倒转
		}
		


}



