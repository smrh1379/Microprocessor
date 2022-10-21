#include "stm32f4xx.h"
#define RS 0x20     /* Pin mask for reg select (e.g. here is pin 5) */
#define RW 0x40     /* Pin mask for read/write (e.g. here is pin 6) */
#define EN 0x80     /* Pin mask for enable     (e.g. here is pin 7) */
void delayMs(int n); 
void LCD_command(unsigned char command); 
void LCD_data(unsigned char data);
void LCD_init(void);
void PORTS_init(void);
void uart_init();
void keypad_init();
void send(char c);
void sendstring (char *string);
char get(void);
int mode=0;
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
TIM2Config();
uart_init();
LCD_init();
keypad_init();
char t;
int i;  
LCD_command(1);	
 while (1) {
	if (!(GPIOA->IDR & 1<<8)){
		send('@');
		mode=0;
		main();
}
	t=get();
	if (t!='q') LCD_data(t);
  else {
	mode=1;
	keypad_init();
	}
 }      
			
}	
void uart_init(){
RCC->AHB1ENR |= 1<<0   ;
RCC->AHB1ENR |= 1<<1   ;	
RCC->APB1ENR |= 0X20000 ;
GPIOA->AFR[0] &=~(0XFF00) ;
GPIOA->AFR[0] |=(0X7700) ;	
GPIOA->MODER &= ~(0X00FF) ;
GPIOA->MODER |= (0X00A0) ;	
//USART2->CR1 = 0x00;
USART2->BRR |=0X683;
USART2->CR1 |=(0X000C);
USART2->CR1 |=0X2000;
}

void keypad_init(){

char t;
int count[4];
count[0]=0;
count[1]=0;
count[2]=0;
count[3]=0;
int number=0;
int reg=0;
char value='0';
GPIOB->MODER |= 0x55000000;
if (mode==1) {
reg=3;
count[reg]=0;
}
while(reg<4){
if (!(GPIOA->IDR & 1<<8)){
		send('@');
		mode=0;
		main();
}
GPIOB->ODR   = 0xE000;
if(!(GPIOB->IDR & 1<<8)){
	value='1';
	send(value);
	count[reg]++;
	Delay_ms (150);
	
}
if(!(GPIOB->IDR & 1<<9)){
	value='2';
	send(value);
	count[reg]++;
	Delay_ms (150);	
}
if(!(GPIOB->IDR & 1<<10) ){
	value='3';
	send(value);
	count[reg]++;
	Delay_ms (150);	
}	
GPIOB->ODR   = 0xD000;
if(!(GPIOB->IDR & 1<<8) ){
	value='4';
	send(value);
	count[reg]++;
	Delay_ms (150);	
}
if(!(GPIOB->IDR & 1<<9)){
	value='5';
	send(value);
	count[reg]++;
	Delay_ms (150);	
}
if(!(GPIOB->IDR & 1<<10) ){
	value='6';
	send(value);
	count[reg]++;
	Delay_ms (150);	}	
GPIOB->ODR   = 0xB000;
if(!(GPIOB->IDR & 1<<8) ) {
	value='7';
	send(value);
	count[reg]++;
	Delay_ms (150);	}
if(!(GPIOB->IDR & 1<<9)){
	value='8';
	send(value);
	count[reg]++;
	Delay_ms (150);	}
if(!(GPIOB->IDR & 1<<10) ){
	value='9';
	send(value);
	count[reg]++;
	Delay_ms (150);	}	
GPIOB->ODR   = 0x7000;
if(!(GPIOB->IDR & 1<<8) ){
	value='*';
	send(value);
	if(mode==0){
	reg--;
	if (reg<0) reg=0;
	while ( count[reg]!=0){
	LCD_command(0x10);
	LCD_data(' ');
	LCD_command(0x10);
	count[reg]--;	
	}	
	Delay_ms (150);	
}
}
if(!(GPIOB->IDR & 1<<9)){
	value='0';
	send(value);
	count[reg]++;
	Delay_ms (150);	}
if(!(GPIOB->IDR & 1<<10) ){
	value='#';
	if (count[reg]!=0)
	{send(value);
	//LCD_command(1);
	//Delay_ms(100);
	if (mode==1) LCD_command(1);
	char received='=';
	while(received!='q'){
	received=get();  
	if(received!='q')LCD_data(received);
	}
	Delay_ms(100);
	reg+=1;
}
}
}
}

void send(char c)
{
while(!(USART2->SR & (1<<7|1<<4)));
USART2->DR=(c);
while (!(USART2->SR & (1<<6)));	
}
void sendstring (char *string)
{
	while (*string) {send (*string++);
	}
}
char get(void){
while(!(USART2->SR & 1<<5)){	
	if (!(GPIOA->IDR & 1<<8)){
		send('@');
		mode=0;
		main();
	}}
return USART2->DR;
}

void LCD_init(void) {
    PORTS_init();

    delayMs(20);            /* initialization sequence */
    LCD_command(0x30);
    delayMs(5);
    LCD_command(0x30);
    delayMs(1);
    LCD_command(0x30);
		delayMs(1);
    LCD_command(0x30);
	  delayMs(1);
    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

void PORTS_init(void) {
    GPIOB->MODER |= 0x00005500;               /* Initialize needed GPIOs and set ports mode appropriately  */
		RCC->AHB1ENR |= 1<<2;
		GPIOC->MODER = 0x55555555;
}

void LCD_command(unsigned char command) {
    GPIOB->ODR &= ~(1<<6)  ;                         /* RS = 0, R/W = 0 */
	  GPIOB->ODR &= ~(1<<5)  ;
		GPIOC->BSRR = 0X00FF0000;
	  GPIOC->ODR  = command;                           /* put command on data bus */
    delayMs(0);
	  GPIOB->ODR |= (1<<7);                           /* pulse EN high */
    delayMs(0);
    GPIOB->ODR &= ~(1<<7);                             /* clear EN */

    if (command < 4)
        delayMs(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(unsigned char data) {
		//LCD_ready();
    GPIOB->ODR |= (1<<5)  ;                    /* RS = 1 */
    GPIOB->ODR &= ~(1<<6)  ;                    /* R/W = 0 */
		GPIOC->BSRR = 0X00FF0000;
		GPIOC->ODR = data  ;                 /* put data on data bus */
    GPIOB->ODR |= (1<<7);                   /* pulse EN high */
    delayMs(0);              /* Do not change this line! */
    GPIOB->ODR &= ~(1<<7);                   /* clear EN */
    delayMs(1);
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n) {
		int j;
    int i;
    for (; n>0; n--)
        for (i = 0; i < 3195; i++) {}
}