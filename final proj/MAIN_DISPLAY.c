#include "stm32f4xx.h"
void USART_init(){
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN   ;
RCC->AHB1ENR |= RCC_APB2ENR_ADC1EN   ;	
RCC->APB1ENR |= 0X20000 ;
GPIOA->MODER |=  0x00000400;
GPIOA->BSRR=0x00200000;
GPIOA->AFR[0] &=~(0XFF00) ;
GPIOA->AFR[0] |=(0X7700) ;	
GPIOA->MODER &= ~(0X00F0) ;
GPIOA->MODER |= (0X00A0) ;	
USART2->CR1 = 0x00;
USART2->BRR |=0XD05;/*SYSCLK=32MHZ , OVRSMPLING=16 , BAUDRATE=9600*/
USART2->CR1 |=(0X000C);
USART2->CR1 |=0X2000;
GPIOA->ODR=0;
}
void TIM2CONFIG(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Enable the timer2 clock
	TIM2->PSC = 31;  // 32MHz/32 = 1 MHz ~~ 1 uS delay
	TIM2->ARR = 9;  //  ARR value
  TIM2->CR1 |= (1<<0); // Enable the Counter
	while (!(TIM2->SR & (1<<0))); 
}
void Delay_us (uint16_t us)
{
	/************** STEPS TO FOLLOW *****************
	1. RESET the Counter
	2. Wait for the Counter to reach the entered value. As each count will take 1 us, 
		 the total waiting time will be the required us delay
	************************************************/
	TIM2->CNT = 0;
	while (TIM2->CNT < us);
}
void GLCD_INIT(){
RCC->AHB1ENR=RCC_AHB1ENR_GPIOBEN;
RCC->AHB1ENR=RCC_AHB1ENR_GPIOCEN;
GPIOC->MODER=0X55555555;
GPIOB->MODER=0X55555555;
GPIOB->ODR |= 0<<0 | 0<<1 | 1<<2 ; /* RS=0 , RW=0 , E=0*/
GPIOC->BSRR |= 0X3F;
GPIOB->BSRR |= 0X40000;
Delay_us(1);
}
