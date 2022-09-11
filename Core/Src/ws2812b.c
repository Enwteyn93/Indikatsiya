//#################################################################################################
#include "ws2812b.h"
//#################################################################################################
extern TIM_HandleTypeDef htim1;
//#################################################################################################
uint16_t BUF_DMA [ARRAY_LEN] = {0};
uint8_t rgb_temp[16][3];
uint16_t DMA_BUF_TEMP[24];
uint8_t frame = 0;
uint8_t line_number = 0;
uint8_t command = 0;
//#################################################################################################
uint8_t X_template[256] = {
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,
	0,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	1,	0,	0,
	0,	0,	0,	1,	1,	1,	0,	0,	0,	0,	1,	1,	1,	0,	0,	0,
	0,	0,	0,	0,	1,	1,	1,	0,	0,	1,	1,	1,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,	0,	0,	0,	0,	0, 	0,
	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,	0,	0,	0,	0,	0, 	0,
	0,	0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	0,	0, 	0,
	0,	0,	0,	0,	1,	1,	1,	0,	0,	1,	1,	1,	0,	0,	0, 	0,
	0,	0,	0,	1,	1,	1,	0,	0,	0,	0,	1,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	1,	0, 	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0, 	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	0
};
//#################################################################################################
uint8_t G_template[256] = {
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	1,	1,	1,	0,	0,	1,	1,	1,	0,	0,	0,	0,
	0,	0,	0,	1,	1,	1,	0,	0,	0,	0,	1,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	1,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	1,	1,	0,	0,	0,	1,	1,	0, 	0,
	0,	0,	1,	0,	0,	0,	1,	1,	1,	1,	0,	0,	0,	1,	0, 	0,
	0,	0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	0,	0, 	0,
	0,	0,	0,	0,	1,	1,	1,	0,	0,	1,	1,	1,	0,	0,	0, 	0,
	0,	0,	0,	1,	1,	1,	0,	0,	0,	0,	1,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	1,	0, 	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0, 	0,
	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0, 	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	0
};
//#################################################################################################
uint8_t E1_template[256] = {
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	0,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	1,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	1,	0,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	0,	0,	0,	0,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	0,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	0,	1,	1,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0
};
//#################################################################################################
uint8_t E2_template[256] = {
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	1,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	1,	0,	0,	0,	1,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	1,	0,	0,	0,	1,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	0,	0,	0,	0,	0,	1,	0,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	1,	0,	0,	0,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0
};
//#################################################################################################
uint8_t E3_template[256] = {
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	1,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	1,	0,	0,	0,	1,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	0,	0,	0,	0,	1,	1,	0,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,
	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	1,	0,	0,	0,	1,	0,	0,
	0,	0,	1,	1,	1,	1,	1,	1,	0,	0,	1,	1,	1,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0
};
//#################################################################################################
void ws2812_init(void)
{
  int i;
  for(i=DELAY_LEN;i<ARRAY_LEN;i++) BUF_DMA[i] = LOW;
}
//#################################################################################################
void ws2812_pixel_rgb_to_buf_dma(uint8_t Rpixel , uint8_t Gpixel, uint8_t Bpixel, uint16_t posX)
{
  volatile uint16_t i;
  for(i=0;i<8;i++)
  {
    if (BitIsSet(Rpixel,(7-i)) == 1)
    {
      BUF_DMA[DELAY_LEN+posX*24+i+8] = HIGH;
    }else
    {
      BUF_DMA[DELAY_LEN+posX*24+i+8] = LOW;
    }
    if (BitIsSet(Gpixel,(7-i)) == 1)
    {
      BUF_DMA[DELAY_LEN+posX*24+i+0] = HIGH;
    }else
    {
      BUF_DMA[DELAY_LEN+posX*24+i+0] = LOW;
    }
    if (BitIsSet(Bpixel,(7-i)) == 1)
    {
      BUF_DMA[DELAY_LEN+posX*24+i+16] = HIGH;
    }else
    {
      BUF_DMA[DELAY_LEN+posX*24+i+16] = LOW;
    }
  }
}
//#################################################################################################
void ws2812_prepareValue(uint8_t r00, uint8_t g00, uint8_t b00,
    uint8_t r01, uint8_t g01, uint8_t b01,
    uint8_t r02, uint8_t g02, uint8_t b02,
    uint8_t r03, uint8_t g03, uint8_t b03,
    uint8_t r04, uint8_t g04, uint8_t b04,
    uint8_t r05, uint8_t g05, uint8_t b05,
    uint8_t r06, uint8_t g06, uint8_t b06,
    uint8_t r07, uint8_t g07, uint8_t b07,
    uint8_t r08, uint8_t g08, uint8_t b08,
    uint8_t r09, uint8_t g09, uint8_t b09,
    uint8_t r10, uint8_t g10, uint8_t b10,
    uint8_t r11, uint8_t g11, uint8_t b11,
    uint8_t r12, uint8_t g12, uint8_t b12,
    uint8_t r13, uint8_t g13, uint8_t b13,
    uint8_t r14, uint8_t g14, uint8_t b14,
		uint8_t r15, uint8_t g15, uint8_t b15
		)
  {
  rgb_temp[0][0]=r00; rgb_temp[0][1]=g00; rgb_temp[0][2]=b00;
  rgb_temp[1][0]=r01; rgb_temp[1][1]=g01; rgb_temp[1][2]=b01;
  rgb_temp[2][0]=r02; rgb_temp[2][1]=g02; rgb_temp[2][2]=b02;
  rgb_temp[3][0]=r03; rgb_temp[3][1]=g03; rgb_temp[3][2]=b03;
  rgb_temp[4][0]=r04; rgb_temp[4][1]=g04; rgb_temp[4][2]=b04;
  rgb_temp[5][0]=r05; rgb_temp[5][1]=g05; rgb_temp[5][2]=b05;
  rgb_temp[6][0]=r06; rgb_temp[6][1]=g06; rgb_temp[6][2]=b06;
  rgb_temp[7][0]=r07; rgb_temp[7][1]=g07; rgb_temp[7][2]=b07;
  rgb_temp[8][0]=r08; rgb_temp[8][1]=g08; rgb_temp[8][2]=b08;
  rgb_temp[9][0]=r09; rgb_temp[9][1]=g09; rgb_temp[9][2]=b09;
  rgb_temp[10][0]=r10;rgb_temp[10][1]=g10;rgb_temp[10][2]=b10;
  rgb_temp[11][0]=r11;rgb_temp[11][1]=g11;rgb_temp[11][2]=b11;
  rgb_temp[12][0]=r12;rgb_temp[12][1]=g12;rgb_temp[12][2]=b12;
  rgb_temp[13][0]=r13;rgb_temp[13][1]=g13;rgb_temp[13][2]=b13;
  rgb_temp[14][0]=r14;rgb_temp[14][1]=g14;rgb_temp[14][2]=b14;
	rgb_temp[15][0]=r15;rgb_temp[15][1]=g15;rgb_temp[15][2]=b15;	
}
//#################################################################################################
void ws2812_setValue(void)
{
  uint8_t n=0;
  for(n=0;n<16;n++)
  {
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[0][0], rgb_temp[0][1], rgb_temp[0][2], n*16);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[1][0], rgb_temp[1][1], rgb_temp[1][2], n*16+1);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[2][0], rgb_temp[2][1], rgb_temp[2][2], n*16+2);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[3][0], rgb_temp[3][1], rgb_temp[3][2], n*16+3);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[4][0], rgb_temp[4][1], rgb_temp[4][2], n*16+4);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[5][0], rgb_temp[5][1], rgb_temp[5][2], n*16+5);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[6][0], rgb_temp[6][1], rgb_temp[6][2], n*16+6);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[7][0], rgb_temp[7][1], rgb_temp[7][2], n*16+7);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[8][0], rgb_temp[8][1], rgb_temp[8][2], n*16+8);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[9][0], rgb_temp[9][1], rgb_temp[9][2], n*16+9);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[10][0],rgb_temp[10][1],rgb_temp[10][2],n*16+10);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[11][0],rgb_temp[11][1],rgb_temp[11][2],n*16+11);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[12][0],rgb_temp[12][1],rgb_temp[12][2],n*16+12);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[13][0],rgb_temp[13][1],rgb_temp[13][2],n*16+13);
    ws2812_pixel_rgb_to_buf_dma( rgb_temp[14][0],rgb_temp[14][1],rgb_temp[14][2],n*16+14);
		ws2812_pixel_rgb_to_buf_dma( rgb_temp[15][0],rgb_temp[15][1],rgb_temp[15][2],n*16+15);
	}
}
//#################################################################################################
void ws2812_light(void)
{
  HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_1,(uint32_t*)&BUF_DMA, ARRAY_LEN);
}
//#################################################################################################
void ws2812_clear(void)
{
	ws2812_prepareValue(0,   0,   0,
                      0,   0,   0,
                      0,   0,   0,
											0, 	 0,   0,
                      0, 	 0, 	0,
											0,   0, 	0,
											0,   0,   0,
											0,   0,   0,
											0,   0,   0,
                      0,   0,   0,
                      0,   0,   0,
                      0,   0,   0,
											0,   0,   0,
											0,   0, 	0,
											0,   0,   0,
											0,   0,   0);
	ws2812_setValue();
	ws2812_light();
}
//#################################################################################################
void ws2812_set_all(uint8_t red, uint8_t green, uint8_t blue)
{
	ws2812_prepareValue(red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
											red,	green,	blue,
											red,	green,	blue,
											red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
											red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
											red,	green,	blue
                      );
	ws2812_setValue();
	ws2812_light();
}
//#################################################################################################
void ws2812_use_template(uint8_t teplate_number, uint8_t red, uint8_t green, uint8_t blue)
{
	if(teplate_number == 5){
		for(int i = 0; i < 16; i++){
			for(int j = 0; j < 16; j++){
				if(E3_template[i*16+j] == 1)
				{
						ws2812_XY(i, j, red, green, blue);
				}
			}
		}
		ws2812_light();
	}
	else if(teplate_number == 4){
		for(int i = 0; i < 16; i++){
			for(int j = 0; j < 16; j++){
				if(E2_template[i*16+j] == 1)
				{
						ws2812_XY(i, j, red, green, blue);
				}
			}
		}
		ws2812_light();
	}
	else if(teplate_number == 3){
		for(int i = 0; i < 16; i++){
			for(int j = 0; j < 16; j++){
				if(E1_template[i*16+j] == 1)
				{
						ws2812_XY(i, j, red, green, blue);
				}
			}
		}
		ws2812_light();
	}
	else if(teplate_number == 2){
		for(int i = 0; i < 256; i++)
		{
			if(G_template[i] == 1)
			{
					ws2812_pixel_rgb_to_buf_dma(red, green, blue, i);
			}
		}
		ws2812_light();
	}
	else if(teplate_number == 1){
		for(int i = 0; i < 256; i++)
		{
			if(X_template[i] == 1)
			{
					ws2812_pixel_rgb_to_buf_dma(red, green, blue, i);
			}
		}
		ws2812_light();
	}
	else if(teplate_number == 0){
		ws2812_clear();
	}
	
}
//#################################################################################################
void ws2812_set_line(uint8_t red, uint8_t green, uint8_t blue,  uint8_t line_num){
	ws2812_prepareValue(red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
											red,	green,	blue,
											red,	green,	blue,
											red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
											red,	green,	blue,
                      red,	green,	blue,
                      red,	green,	blue,
											red,	green,	blue);
	ws2812_pixel_rgb_to_buf_dma( rgb_temp[0][0], rgb_temp[0][1], rgb_temp[0][2], line_num*16);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[1][0], rgb_temp[1][1], rgb_temp[1][2], line_num*16+1);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[2][0], rgb_temp[2][1], rgb_temp[2][2], line_num*16+2);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[3][0], rgb_temp[3][1], rgb_temp[3][2], line_num*16+3);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[4][0], rgb_temp[4][1], rgb_temp[4][2], line_num*16+4);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[5][0], rgb_temp[5][1], rgb_temp[5][2], line_num*16+5);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[6][0], rgb_temp[6][1], rgb_temp[6][2], line_num*16+6);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[7][0], rgb_temp[7][1], rgb_temp[7][2], line_num*16+7);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[8][0], rgb_temp[8][1], rgb_temp[8][2], line_num*16+8);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[9][0], rgb_temp[9][1], rgb_temp[9][2], line_num*16+9);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[10][0],rgb_temp[10][1],rgb_temp[10][2],line_num*16+10);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[11][0],rgb_temp[11][1],rgb_temp[11][2],line_num*16+11);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[12][0],rgb_temp[12][1],rgb_temp[12][2],line_num*16+12);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[13][0],rgb_temp[13][1],rgb_temp[13][2],line_num*16+13);
  ws2812_pixel_rgb_to_buf_dma( rgb_temp[14][0],rgb_temp[14][1],rgb_temp[14][2],line_num*16+14);
	ws2812_pixel_rgb_to_buf_dma( rgb_temp[15][0],rgb_temp[15][1],rgb_temp[15][2],line_num*16+15);
	ws2812_light();	
}
//#################################################################################################
void ws2812_set_command(uint8_t com){
	command = com;
}
//#################################################################################################
void ws2812_update_frame(void){
	switch(command){
		case 0:
			if(frame == 1){
				frame = 0;
				ws2812_clear();
			}
			else{
				frame = 1;
				ws2812_clear();
			}
			break;
		case 1:
			if(frame == 1){
				frame = 0;
				ws2812_use_template(1, 128, 0, 0);
			}
			else{
				frame = 1;
				ws2812_clear();
			}
			break;
		case 2:
			if(frame == 1){
				frame = 0;
				ws2812_use_template(2, 0, 128, 0);
			}
			else{
				frame = 1;
				ws2812_clear();
			}
			break;
		case 3:
			if(frame == 1){
				frame = 0;
				ws2812_use_template(3, 80, 80, 0);
			}
			else{
				frame = 1;
				ws2812_clear();
			}
			break;
		case 4:
			if(frame == 1){
				frame = 0;
				ws2812_use_template(4, 80, 80, 0);
			}
			else{
				frame = 1;
				ws2812_clear();
			}
			break;
		case 5:
			if(frame == 1){
				frame = 0;
				ws2812_use_template(5, 80, 80, 0);
			}
			else{
				frame = 1;
				ws2812_clear();
			}
			break;
		default:
				break;
	}
}
//#################################################################################################
void ws2812_XY(uint8_t Xpic, uint8_t Ypic, uint8_t Rpixel , uint8_t Gpixel, uint8_t Bpixel){
	if(Xpic % 2){
		ws2812_pixel_rgb_to_buf_dma(Rpixel, Gpixel, Bpixel, (Xpic * 16) + Ypic);
	}
	else{
		ws2812_pixel_rgb_to_buf_dma(Rpixel, Gpixel, Bpixel, (Xpic * 16) + (15 - Ypic));
	}
}
//#################################################################################################


