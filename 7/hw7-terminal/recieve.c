#include "stm32f4xx.h"
void uart_init();
uint8_t get();
void send(uint8_t c);
int main(){
uint8_t data;
data=get();
send(data);

}


void uart_init(){
RCC->AHB1ENR=1<<0;
RCC->AHB1ENR=1<<1;
RCC->APB2ENR |= 1<<4 ;
GPIOA->MODER |= (1<<0);
GPIOA->MODER |= (2<<4) ;
GPIOA->MODER |= (2<<6) ;	
GPIOA->AFR[0] |=(7<<8) ;
GPIOA->AFR[0] |=(7<<12);	
USART1->CR1   =0x00;
USART1->CR1 |=(1<<13);// USART ENABLE 
USART1->CR1 |=(0<<12);
USART1->BRR |=(16<<0) | (292<<4) ;
USART1->CR1 |=(1<<2) | (1<<3);
}

uint8_t get(){
uint8_t temp;
while(!(USART1->SR & 1<<5));
temp = USART1->DR;
return temp;
	
}
void send(uint8_t c){
USART1->DR = c;
while(!(USART1->SR & 1<<6));

}








