/*
 * LCD_GFX.c
 *
 * Created: 9/20/2021 6:54:25 PM
 *  Author: You
 */ 

#include "LCD_GFX.h"
#include "ST7735.h"
#include "ASCII_LUT.h"

/******************************************************************************
* Local Functions
******************************************************************************/



/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn			uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
* @brief		Convert RGB888 value to RGB565 16-bit color data
* @note
*****************************************************************************/
uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255));
}

/**************************************************************************//**
* @fn			void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
* @brief		Draw a single pixel of 16-bit rgb565 color to the x & y coordinate
* @note
*****************************************************************************/
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
	LCD_setAddr(x,y,x,y);
	SPI_ControllerTx_16bit(color);
}


/**************************************************************************//**
* @fn			void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
* @brief		Draw a character starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor){
	uint16_t row = character - 0x20;		//Determine row of ASCII table starting at space
	int i, j;
	if ((LCD_WIDTH-x>7)&&(LCD_HEIGHT-y>7)){
		for(i=0;i<5;i++){
			uint8_t pixels = ASCII[row][i]; //Go through the list of pixels
			for(j=0;j<8;j++){
				if ((pixels>>j)&1==1){
					LCD_drawPixel(x+i,y+j,fColor);
				}
				else {
					LCD_drawPixel(x+i,y+j,bColor);
				}
			}
		}
	}
}


/******************************************************************************
* LAB 4 TO DO. COMPLETE THE FUNCTIONS BELOW.
* You are free to create and add any additional files, libraries, and/or
*  helper function. All code must be authentically yours.
******************************************************************************/

/**************************************************************************//**
* @fn			void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
* @brief		Draw a colored circle of set radius at coordinates
* @note
*****************************************************************************/
void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
{
    int x = radius;
    int y = 0;
    int err = 0;
    
    LCD_setAddr(x0 - radius, y0, x0 + radius, y0);
    for(int i = 0; i <= 2 * radius; i++) {
        SPI_ControllerTx_16bit(color);
    }
    
    while (x >= y) {
        if(y > 0) {

            LCD_setAddr(x0 - x, y0 + y, x0 + x, y0 + y);
            for(int i = 0; i <= 2 * x; i++) {
                SPI_ControllerTx_16bit(color);
            }
            
            LCD_setAddr(x0 - x, y0 - y, x0 + x, y0 - y);
            for(int i = 0; i <= 2 * x; i++) {
                SPI_ControllerTx_16bit(color);
            }
            
            // ??????????
            LCD_setAddr(x0 - y, y0 + x, x0 + y, y0 + x);
            for(int i = 0; i <= 2 * y; i++) {
                SPI_ControllerTx_16bit(color);
            }
            
            LCD_setAddr(x0 - y, y0 - x, x0 + y, y0 - x);
            for(int i = 0; i <= 2 * y; i++) {
                SPI_ControllerTx_16bit(color);
            }
        }
        
        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}



/**************************************************************************//**
* @fn			void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
* @brief		Draw a line from and to a point with a color
* @note
*****************************************************************************/
void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
{
	short dx = abs(x1 - x0);
    short dy = abs(y1 - y0);
    short sx = x0 < x1 ? 1 : -1;
    short sy = y0 < y1 ? 1 : -1;
    short err = dx - dy;
    short err2;

    while (1) {
        LCD_drawPixel(x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }// Fill this out
}



/**************************************************************************//**
* @fn			void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw a colored block at coordinates
* @note
*****************************************************************************/
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
	if (x0 > x1) {
        uint8_t temp = x0;
        x0 = x1;
        x1 = temp;
    }
    if (y0 > y1) {
        uint8_t temp = y0;
        y0 = y1;
        y1 = temp;
    }
    
    uint16_t pixelCount = (x1 - x0 + 1) * (y1 - y0 + 1);
    
    LCD_setAddr(x0, y0, x1, y1);
    
    // ????????
    while(pixelCount--) {
        SPI_ControllerTx_16bit(color);
    }
}

/**************************************************************************//**
* @fn			void LCD_setScreen(uint16_t color)
* @brief		Draw the entire screen to a color
* @note
*****************************************************************************/
void LCD_setScreen(uint16_t color) 
{
	LCD_setAddr(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    
    
    for (uint16_t i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        SPI_ControllerTx_16bit(color);
    }  // Fill this out
}

/**************************************************************************//**
* @fn			void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
* @brief		Draw a string starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fColor, uint16_t bColor) {
    uint8_t currentX = x;
    uint8_t len = strlen(str);
    
    if (currentX + len * 6 > LCD_WIDTH) {
        len = (LCD_WIDTH - currentX) / 6;
    }

    LCD_drawBlock(x, y, x + len * 6 - 1, y + 7, bColor);
    

    while (*str && len--) {
        LCD_drawChar(currentX, y, *str++, fColor, bColor);
        currentX += 6;
    }
}

void LCD_Clear(uint16_t color) {
    
    LCD_setAddr(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    
    
    for (uint16_t i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        SPI_ControllerTx_16bit(color);
    }  
}

