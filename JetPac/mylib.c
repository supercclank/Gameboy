#include "mylib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int col, int row, int height, int width, volatile unsigned short color)
{
	int r;
	for(r=0; r<height; r++)
	{
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)];
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;	
	}
}
	

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void fillScreen(volatile unsigned short color)
{
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240*160) | DMA_SOURCE_FIXED | DMA_ON;
}

void drawPicture(const unsigned short arr[])
{
	DMA[3].src = arr;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240*160) | DMA_ON;
}

void drawImage3(int r, int c, int width, int height, const u16* image)
{
	
	for (int i = 0; i < height; i++) {
		DMA[3].src = &image[OFFSET(i, 0, width)];
		DMA[3].dst = videoBuffer + OFFSET(c + i, r, 240);
		DMA[3].cnt = width | DMA_ON;
	}
}
