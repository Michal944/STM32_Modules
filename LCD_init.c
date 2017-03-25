#include "stm32f10x.h"

#define P0 0x00000001
#define P1 0x00000002
#define PC2 0x00000004
#define PC3 0x00000008

#define RS P0
#define E P1
#define PC0 P0
#define PC1 P1
#define rs(x) (x) ? (GPIOA->ODR |= RS) : (GPIOA->ODR &= ~RS)
#define e(x) (x) ? (GPIOA->ODR |= E) : (GPIOA->ODR &= ~E)
#define PORTA 0x00000004
#define PORTC 0x00000010
#define PORTB 0x00000008

volatile void delay( volatile uint16_t time)
{
	while(time)
		time--;
}

void LCD_set(char c)
{
	e(1);
	c&=0x0f;
	GPIOC->ODR |= c;
	delay(10);
	e(0);
	delay(10);
	GPIOC->ODR &= ~c;
	delay(10);
}

void set_4bit(char c)
{
	delay(8100);
	delay(10);
	LCD_set(c>>4);
	delay(10);
	LCD_set(c);
	delay(10);
}

void lcd_init()
{
	delay(3000);
	rs(0);
	LCD_set(0b0011);
	delay(4000);
	LCD_set(0b0011);
	delay(100);
	LCD_set(0b0011);
	delay(100);
	LCD_set(0b0010);

	set_4bit(0b00101000); //FUNCTION_SET
	set_4bit(0b00010100); //DISPLAY_CURSOR_SIFT
	set_4bit(0b00001111); //DISPLA_ON/OFF
	set_4bit(0b00000110); //Entry_mode_set
	set_4bit(0b00000001); //Clear_Display
}


int main()
{
	RCC->APB2ENR = PORTA | PORTC | PORTB;
	GPIOC->CRL = 0x00000003 | 3<<4 | 3<<8 | 3<<12;
	GPIOA->CRL = 0x00000003 | 3<<4;
	GPIOB->CRL = 0x00000008 | 8<<4 | 3<<8;
	GPIOB->ODR = 0x00000004;
	lcd_init();

	rs(1);
	set_4bit('H');
	set_4bit('E');
	set_4bit('L');
	set_4bit('L');
	set_4bit('O');
	set_4bit(' ');
	set_4bit('W');
	set_4bit('O');
	set_4bit('R');
	set_4bit('L');
	set_4bit('D');

	while(1)
	{
		if(GPIOB->IDR & 0x00000001)
			set_4bit(0b00000001);
		else if(GPIOB->IDR & 0x00000002)
		{
			set_4bit('O');
			set_4bit('K');
		}
	}
}
