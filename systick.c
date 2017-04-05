#include "stm32f10x.h"

#define PA 	 0x0004
#define PB 	 0x0008
#define PC 	 0x0010
#define PD   0x0020
#define AFIO 0x0001

#define pin0 0x0001
#define pin1 0x0002
#define pin2 0x0004
#define pin3 0x0008
#define pin4 0x0010

#define GPpp 	  0b0000
#define OutPut10  0b0001

#define My_SysTick_CLKSource_HCLK_Div8    ((uint32_t)0xFFFFFFFB)
#define My_SysTick_CLKSource_HCLK         ((uint32_t)0x00000004)

#define Clock_Frequency 		72000000
#define Clock_Frequency_Div8	 9000000

void RCC_Config()
{
	RCC->APB2ENR = PA;
}

void PIN_Config(uint16_t PIN)
{
	GPIOA->CRL = (GPpp | OutPut10)<<4*PIN;
}

volatile uint32_t flag;

void SysTick_Handler()
{
	if(flag)
	{
	 GPIOA->ODR = pin0;
	 flag=0;
	}
	else
	{
	 GPIOA->ODR &= ~pin0;
	 flag=1;
	}
}

int main(void)
{

 RCC_Config();
 PIN_Config(0);

 SysTick_Config(Clock_Frequency_Div8/2);
 SysTick->CTRL &= My_SysTick_CLKSource_HCLK_Div8;

 while (1) {

 }
}
