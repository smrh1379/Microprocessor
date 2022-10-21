#include "stm32f4xx.h"
void uart_init();
uint8_t get();
void send(uint8_t c);
void sendstring (char *string);
int main(){
char* data;

send('c');
//data=get();
while(1){}
}

void uart_init(){
RCC->AHB1ENR |= 1<<0   ;
RCC->AHB1ENR |= 1<<1   ;	
RCC->APB1ENR |= 0X20000 ;
GPIOA->AFR[0] &=~(0XFF00) ;
GPIOA->AFR[0] |=(0X7700) ;	
GPIOA->MODER &= ~(0X00F0) ;
GPIOA->MODER |= (0X00A0) ;	

USART2->BRR |=0X683;
USART2->CR1 |=(0X000C);
USART2->CR1 |=0X2000;
}

void send(uint8_t c)
{
while(!(USART2->SR & 1<<7));
USART2->DR=c;
//LCD_command(1);
//LCD_data('Y');
//delayMs(1000);
}
void sendstring (char *string)
{
	while (*string) send (*string++);
}
uint8_t get(void){
uint8_t temp;
while(!(USART2->SR & 1<<5));
temp = USART2->DR;
//LCD_data(temp);
//delayMs(1000);	
return temp;
}