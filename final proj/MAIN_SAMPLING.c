#include "stm32f4xx.h"
void ADC_init();
void USART_init();
void send(int c);
char get(void);
void sendstring (char *string);
void TIM2CONFIG();
void TIM3CONFIG();
void TIM4CONFIG();
void Delay_us (uint16_t us);
void Delay_ms (uint16_t ms);
void PORTS_init(void);
void LCD_command(unsigned char command) ;
void LCD_data(unsigned char data);
void LCD_init(void);
void B_set(double b);
void TU_set(double t);
void V_SET(double v);
void LCD_SET();
int count=0;
uint8_t samples[128]={0};
void add(uint8_t);
uint16_t adc_conv(){
  ADC1->CR2 |= ADC_CR2_SWSTART; 
  while(!(ADC1->SR & 2)){}	
  return ADC1->DR;
}
uint8_t GLCD_Data[128];
volatile double TU=100;
volatile double A=1;
volatile double B=1;
int main(){
for(int i=0;i<128;i++){
	GLCD_Data[i]=0;
}
//FRQ();
TIM4CONFIG();
Delay_ms(200);
LCD_init();
USART_init();
ADC_init();
LCD_SET();
TIM2CONFIG();
TIM3CONFIG();
//send(0xff);
//send(50);
while(1){
GPIOB->ODR   = 0xE000;
if(!(GPIOB->IDR & 1<<8)){
	__disable_irq();
	A+=0.1;
  V_SET(A);
	send(0xfe);
	Delay_us(1);
	send((A*10));
	Delay_ms(20);
	__enable_irq();
}
if(!(GPIOB->IDR & 1<<9)){
	__enable_irq();
	A-=0.1;
	if(A<=0) A=0;
	V_SET(A);
	send(0xfe);
	Delay_us(1);
	send((A*10));
  Delay_ms(20);
	__disable_irq();
}
if(!(GPIOB->IDR & 1<<10) ){
	__enable_irq();
	B+=0.1;
	B_set(B);
	send(0xfd);
	Delay_us(1);
	send((B*10));
  Delay_ms(20);
	__disable_irq();
}	
GPIOB->ODR   = 0xD000;
if(!(GPIOB->IDR & 1<<8) ){
	__enable_irq();
	B-=0.1;
	if(B<=0) B=0;
	B_set(B);
	send(0xfd);
	Delay_us(1);
	send((B*10));
  Delay_ms(20);
	__disable_irq();
}
if(!(GPIOB->IDR & 1<<9)){
	TU+=5;
	TU_set(TU);
  Delay_ms(20);
}
if(!(GPIOB->IDR & 1<<10) ){
	TU-=5;
	TU_set(TU);
	send('e');
  Delay_ms(20);
}	
}
}
void FRQ(){
RCC->CR|=RCC_CR_HSEON;
while(!(RCC->CR & RCC_CR_HSERDY));
RCC->APB1ENR |=RCC_APB1ENR_PWREN;
PWR->CR |= PWR_CR_VOS;
}
void ADC_init(){
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;/*GPIOA CLOCK ENABLED*/
RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;/* ADC1 CLOCK ENABLED*/
GPIOA->MODER |= 3<<0;
GPIOA->MODER |= 3<<2;
//ADC->CCR|=1<<16; /* clk DIV4*/
//ADC1->CR1 |= (1<<25);// 8 BIT RESOULOUTION */
//ADC1->CR1 |= 1<<8 ;/*SCAN MODE ENABLE*/
//fffADC1->SQR1 |= 1<<20; /* 2 CHANNEL CONVERSION */
//ADC1->CR2 |= 0<<24 | 1<<28; /* EXTERNAL ENABLE AND ENABLE ON TIM1 CHNL 1   */
//ADC1->CR2 &= ~(1<<11); /*RIGHT ALIGNMENT*/
//ADC1->CR2 |= 1<<10; /*END OF CONVERSION ENABLE*/
//ADC1->SMPR2 |= 6<<0 | 6<<3; /* SYSCLK=32MHZ -> ADCCLK=16MHZ -> SAMPLINGFRQ=100KHZ -> SAMPLINGRATE=(1/100KHZ)/16MHZ=160-12=140 -> 144 CYCLES  */
ADC1->CR2 = 0; 	/* SW trigger */
//ADC1->CR2 |= 1<<10; /*END OF CONVERSION ENABLE*/	
ADC1->SQR1 |= 0;
ADC1->SQR3 |= 0 ;
//ADC1->SQR1 =	0;
ADC1->CR2 |= 1;                 /* enable ADC1 */
//ADC1->CR2|=1<<0;
}

void USART_init(){
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN   ;
RCC->APB1ENR |= RCC_APB1ENR_USART2EN ;
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
}
void send(int c)
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
while(!(USART2->SR & 1<<5));
return USART2->DR;
}
void TIM3CONFIG(){
__disable_irq();
RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
TIM3->PSC =15;
TIM3->ARR =0x4F0;
TIM3->CNT=0;
TIM3->DIER |=TIM_DIER_UIE;
TIM3->CR1 |= (1<<0); // Enable the Counter
NVIC_EnableIRQ(TIM3_IRQn);
__enable_irq();
}

void TIM2CONFIG(){
  __disable_irq();
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Enable the timer2 clock
	TIM2->PSC = 15;  // 16Hz/16 = 1 MHz ~~ 1 uS delay
	TIM2->ARR = 0xFF0;  //  ARR value
	TIM2->CNT=0;
	TIM2->DIER |=TIM_DIER_UIE;
  TIM2->CR1 |= (1<<0); // Enable the Counter
	NVIC_EnableIRQ(TIM2_IRQn);
	__enable_irq();
}
void add(uint8_t num){
samples[count]=num;
count++;
}
void TIM3_IRQHandler(void){
TIM3->SR = 0;
if (count==128){
send(0XFF);
Delay_us(1);
for (int i=0;i<128;i++){
send(GLCD_Data[i]);
Delay_us(1);
}
Delay_ms(100);
count=0;
}
}
void TIM2_IRQHandler(void){
GPIOA->MODER |= 0X55555000; 
//NVIC_ClearPendingIRQ(TIM2_IRQn);
TIM2->SR=0;
ADC1->CR2 |= ADC_CR2_SWSTART;/*START CONVERSION*/
while(!(ADC1->SR & 1<<1)){GPIOC->ODR &= ~(1<<15);}/*END OF CONVERSION */
volatile float RESULT;
if (count<128){
RESULT=ADC1->DR;
GPIOA->ODR |= ((int)RESULT) <<5;
GPIOC->ODR |= 1<<15;
RESULT/=4095;
GLCD_Data[count]=((int)((RESULT*64)+count))%63;
count++;
}

}

void TIM4CONFIG(){
	RCC->APB1ENR=RCC_APB1ENR_TIM4EN;
	TIM4->PSC=15;
	TIM4->ARR=0XFFFF;
	TIM4->CR1 |= (1<<0);
	while (!(TIM4->SR & (1<<0))); 
}
void Delay_us (uint16_t us)
{
	/************** STEPS TO FOLLOW *****************
	1. RESET the Counter
	2. Wait for the Counter to reach the entered value. As each count will take 1 us, 
		 the total waiting time will be the required us delay
	************************************************/
	TIM4->CNT = 0;
	while (TIM4->CNT < us);
}

void Delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		Delay_us (1000); // delay of 1 ms
	}
}
void PORTS_init(void) {
	  RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER  |= 0x55005555;               /* Initialize needed GPIOs and set ports mode appropriately  */	
		RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOCEN;
		GPIOC->MODER  |= 0x55555555; 
}

void LCD_command(unsigned char command) {
    GPIOB->ODR &= ~(1<<1)  ;                         /* RS = 0, R/W = 0 */
	  GPIOB->ODR &= ~(1<<0)  ;
		GPIOC->BSRR = 0X00FF0000;
	  GPIOC->ODR  = command;                           /* put command on data bus */
    Delay_us(0);
	  GPIOB->ODR |= (1<<2);                           /* pulse EN high */
    Delay_ms(0);
    GPIOB->ODR &= ~(1<<2);                             /* clear EN */

    if (command < 4)
        Delay_ms(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        Delay_us(50);         /* all others 40 us */
}

void LCD_data(unsigned char data) {
		//LCD_ready();
    GPIOB->ODR |= (1<<0)  ;                    /* RS = 1 */
    GPIOB->ODR &= ~(1<<1)  ;                    /* R/W = 0 */
		GPIOC->BSRR = 0X00FF0000;
		GPIOC->ODR = data  ;                 /* put data on data bus */
    GPIOB->ODR |= (1<<2);                   /* pulse EN high */
    Delay_ms(0);              /* Do not change this line! */
    GPIOB->ODR &= ~(1<<2);                   /* clear EN */
    Delay_ms(1);
}
void LCD_init(void) {
    PORTS_init();
    Delay_ms(10);            /* initialization sequence */
    LCD_command(0x30);
    Delay_ms(5);
    LCD_command(0x30);
    Delay_ms(1);
    LCD_command(0x30);
		Delay_ms(1);
    LCD_command(0x30);
	  Delay_ms(1);
    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}
void B_set(double b){
int intValue = (int)b;
double diffValue = b - (double)intValue;
int anotherIntValue = (int)(diffValue * 10);
LCD_command(0x80);
LCD_command(0x14);
LCD_command(0x14);	
if  (b==1){
LCD_data('1');	
LCD_data('.');
LCD_data('0');
LCD_data('v');
}
else if (b<1){
anotherIntValue = (int)(b * 10);
LCD_data('0');	
LCD_data('.');
LCD_data(anotherIntValue+48);
LCD_data('v');
}
else{
LCD_data(intValue+48);	
LCD_data('.');
LCD_data(anotherIntValue+48);
LCD_data('v');
}	
}
void TU_set(double t){
LCD_command(0xc0);
LCD_data('T');
LCD_data('U');	
LCD_data('=');
int Value = (int)t;
int count=0;
while(Value>0) {Value/=10;
count++;
}
Value=(int)t;
while(count>0){
LCD_data(((Value % (int) pow(10,count))/(int) pow(10,count-1))+48);
count--;
}
LCD_data('m');
LCD_data('s');
if(t<100) LCD_data(' ');
LCD_command(0x10);
}
void V_SET(double v){
int intValue = (int)v;
double diffValue = v - (double)intValue;
int anotherIntValue = (int)(diffValue * 10);
LCD_command(0x80);
LCD_command(0x14);
LCD_command(0x14);
LCD_command(0x14);
LCD_command(0x14);
LCD_command(0x14);
LCD_command(0x14);
LCD_command(0x14);
LCD_command(0x14);
LCD_command(0x14);
if  (v==1){
LCD_data('1');	
LCD_data('.');
LCD_data('0');
LCD_data('v');
}
else if (v<1){
int anotherIntValue = (int)(v * 10);
LCD_data('0');	
LCD_data('.');
LCD_data(anotherIntValue+48);
LCD_data('v');
}
else{	
LCD_data(intValue+48);	
LCD_data('.');
LCD_data(anotherIntValue+48);
LCD_data('v');	
}
}
void LCD_SET(){
LCD_command(0X1);
LCD_data('B');
LCD_data('=');
B_set(1.0);
LCD_command(0x14);
LCD_data('V');
LCD_data('=');
V_SET(1.0);
LCD_command(0xc0);
LCD_data('T');
LCD_data('U');	
LCD_data('=');
TU_set(100);
	
}