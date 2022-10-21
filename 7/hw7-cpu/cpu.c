#include "stm32f4xx.h"
#include <math.h>
#include <stdio.h>
void delayMs(int n); 
void uart_init();
void send(char c);
void sendstring (char *string);
char get(void);
void LED_blink();
void TIM2Config (void)
{
	/************** STEPS TO FOLLOW *****************
	1. Enable Timer clock
	2. Set the prescalar and the ARR
	3. Enable the Timer, and wait for the update Flag to set
	************************************************/

// 1. Enable Timer clock
	RCC->APB1ENR |= (1<<0);  // Enable the timer2 clock
	
// 2. Set the prescalar and the ARR
	TIM2->PSC = 15;  // 16MHz/16 = 1 MHz ~~ 1 uS delay
	TIM2->ARR = 0xffff;  // MAX ARR value
	
// 3. Enable the Timer, and wait for the update Flag to set
	TIM2->CR1 |= (1<<0); // Enable the Counter
	while (!(TIM2->SR & (1<<0)));  // UIF: Update interrupt flag..  This bit is set by hardware when the registers are updated
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

void Delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		Delay_us (1000); // delay of 1 ms
	}
}
int main(){
uart_init();
TIM2Config();

int i=0;
int g=0;
char	t='+';
char message[6];
message[0]='E';
message[1]='E';
message[2]='E';
message[3]='E';
message[4]='E';
message[5]='E';
get();
int a=0;
int b=0;
int c=0;
int j=0;
int x=0;
while(1){
//sendstring("smrh");
t=get();	
if (t=='@'){main();}
LED_blink();
if (t!='#' & t!='*'){
g=t;
g-=48;
if (j==0)a=a*10+(g);
if (j==1)b=b*10+(g);
if (j==2)c=c*10+(g);
if (j==3)x=x*10+(g);
message[i]=t;
i++;
}
else if (t=='*'){
message[0]='E';
message[1]='E';
message[2]='E';
message[3]='E';
message[4]='E';
message[5]='E';	
i=0;
if (j==3) x=0;
j--;
if (j<0) {
j=0;
a=0;
b=0;
c=0;
x=0;
}
if (j==0) a=0;
if (j==1) b=0;
if (j==2) c=0;

}
else if (t=='#'){
for (i = 0; i <6 ; i++) {
		if(message[i]==('E')) break;
		send(message[i]);
		}

j++;
send('q');
if (j==4) break;
i=0;
message[0]='E';
message[1]='E';
message[2]='E';
message[3]='E';
message[4]='E';
message[5]='E';	
}
}
int javab=0;
int temp=0;
int count=0;
javab = (a*x) + (2*b*x) + (3*c*x);
temp=javab;
while (temp!=0){
temp/=10;
count++;
}
Delay_ms(300);
if (count==0) send('0');
while (count!=0){
count--;
send(((javab/(int) pow(10,count)%10)+48));
}

j=3;
x=0;
send('q');
while(1){
t=get();
if (t=='@'){main();}
else{
LED_blink();
if (t!='#' & t!='*'){
g=t;
g-=48;
if (j==3)x=x*10+(g);
}
else if (t=='*'){
x=0;
}
else if (t=='#'){
 javab=0;
 temp=0;
 count=0;
javab = (a*x) + (2*b*x) + (3*c*x);
temp=javab;
while (temp!=0){
temp/=10;
count++;
}
Delay_ms(300);
if (count==0) send('0');
while (count!=0){
count--;
send(((javab/(int) pow(10,count)%10)+48));
}
send('q');
j=3;
x=0;
Delay_ms(100);
send('q');
}
}
}
}

void LED_blink() {
	GPIOA->BSRR = 0x00000020;   /* turn on LED */
	Delay_ms(100);
	GPIOA->BSRR = 0x00200000;   /* turn off LED */
        
}
void uart_init(){

RCC->AHB1ENR |= 1<<0   ;
RCC->AHB1ENR |= 1<<1   ;	
RCC->APB1ENR |= 0X20000 ;
GPIOA->MODER |=  0x00000400;
GPIOA->BSRR=0x00200000;
GPIOA->AFR[0] &=~(0XFF00) ;
GPIOA->AFR[0] |=(0X7700) ;	
GPIOA->MODER &= ~(0X00F0) ;
GPIOA->MODER |= (0X00A0) ;	
USART2->CR1 = 0x00;
USART2->BRR |=0X683;
USART2->CR1 |=(0X000C);
USART2->CR1 |=0X2000;
GPIOA->ODR=0;
//USART2->CR3 |= 1<<8 | 1<<9;
}
void send(char c)
{
while(!(USART2->SR & (1<<7|1<<4)));
USART2->DR=(c);
while (!(USART2->SR & (1<<6)));
}
void sendstring (char *string)
{
	while (*string){ send (*string++);
	delayMs(100);
	}
}
char get(void){
while(!(USART2->SR & 1<<5));
return  USART2->DR;
}



/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n) {
		int j;
    int i;
    for (; n>0; n--)
        for (i = 0; i < 3195; i++) {}
}
