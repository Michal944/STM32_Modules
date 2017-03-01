#include "stm32f10x.h"


#define PORTC 0x0010
#define PORTB 0x0008
#define AIO   0x0001
#define C_CRL 0b0011
#define B_CRL 0b1000

#define Fall_Rise  0x0001
#define Imr_value  0x0001
#define AIO_EX_B   0b0001

struct Struct_NVIC
{
	uint32_t PRIORITY_GROUP;
	uint32_t PRIORITY_S;
	int32_t  ON_IRQ;
};

struct Struct_EXTI
{
	uint8_t  NR_EXTICR;
	uint32_t AFIOEXTICR;
	uint32_t FTSR_EXTI;
	uint32_t RTSR_EXTI;
	uint32_t IMR_EXTI;
};

void EXTI0_IRQHandler()
{
	/* EXTI->PR  Pending register
	 *
	 *for example: pin0
	*/
	EXTI->PR = 0x0001;
	GPIOC->ODR = 0x0001;
}

void nvic_init(struct Struct_NVIC *set)
{
	/*SetPriorityGrouping(3-7) pre-emptation*
	for example: if 3 it's mean that xxxx 4bits x-pre-empt. or sub-.
	 * second situation: 4 xxxy y = subpriorities  8-pre and 2-sub
	 * You might have 16 priorities or subpriorities
	 SetPriority(VALUE1,VALUE2)
	 VALUE1 is located in CMSIS(stm32f10x.h)
	 VALUE2 is the prioritie of VALUE1
	 */
	NVIC_SetPriorityGrouping(set->PRIORITY_GROUP);
	NVIC_SetPriority(set->ON_IRQ, set->PRIORITY_S);
	NVIC_EnableIRQ(set->ON_IRQ);

}

void exti_init(struct Struct_EXTI *set)
{
	/*
	 * NR_EXTICR - external interrupt conf. register
	 * Array is able to take index between [0:3]
	 * AFIOEXTICR values: 0b0000(PA)...0b0110(PG) moving<<[1:3]
	 RTSR/FTSR - Rising/falling trigger register 32bit
	 Pin0 0x00000001 pin15 0x0000000F
	 IMR/EMR - Interrupt/event mask
	 */
	if((set->NR_EXTICR) <= 3)
		AFIO->EXTICR[set->NR_EXTICR] = set->AFIOEXTICR;

	if(set->FTSR_EXTI)
		EXTI->FTSR = set->FTSR_EXTI;

	if(set->RTSR_EXTI)
		EXTI->RTSR = set->RTSR_EXTI;

	EXTI->IMR  =  set->IMR_EXTI;
}


int main()
{

	struct Struct_NVIC nvic;
	struct Struct_EXTI exti;

	nvic.PRIORITY_GROUP	= (uint32_t)0x03;
	nvic.PRIORITY_S		= 0;
	nvic.ON_IRQ 		= EXTI0_IRQn;

	exti.NR_EXTICR 		= (uint8_t)0x00;
	exti.AFIOEXTICR		= AIO_EX_B;
	exti.RTSR_EXTI		= 0;
	exti.FTSR_EXTI		= Fall_Rise;
	exti.IMR_EXTI		= Imr_value;

	RCC->APB2ENR = PORTC|PORTB|AIO;
	GPIOC->CRL = C_CRL;
	GPIOB->CRL = B_CRL;
	GPIOB->ODR = 0x00001; //PULL_UP PB0

	exti_init(&exti);
	nvic_init(&nvic);

	while(1)
	{
		;
	}
}
