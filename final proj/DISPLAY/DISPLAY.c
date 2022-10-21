#include "stm32f4xx.h"
int x_cursor,y_cursor,cs_cursor = 0;
void cls();
uint16_t GLCD_Data[128][64];
const unsigned short Terminal6x8[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char  
        0x00, 0x00, 0x06, 0x5F, 0x06, 0x00,  // Code for char !
        0x00, 0x07, 0x03, 0x00, 0x07, 0x03,  // Code for char "
        0x00, 0x24, 0x7E, 0x24, 0x7E, 0x24,  // Code for char #
        0x00, 0x24, 0x2B, 0x6A, 0x12, 0x00,  // Code for char $
        0x00, 0x63, 0x13, 0x08, 0x64, 0x63,  // Code for char %
        0x00, 0x36, 0x49, 0x56, 0x20, 0x50,  // Code for char &
        0x00, 0x00, 0x07, 0x03, 0x00, 0x00,  // Code for char '
        0x00, 0x00, 0x3E, 0x41, 0x00, 0x00,  // Code for char (
        0x00, 0x00, 0x41, 0x3E, 0x00, 0x00,  // Code for char )
        0x00, 0x08, 0x3E, 0x1C, 0x3E, 0x08,  // Code for char *
        0x00, 0x08, 0x08, 0x3E, 0x08, 0x08,  // Code for char +
        0x00, 0x00, 0xE0, 0x60, 0x00, 0x00,  // Code for char ,
        0x00, 0x08, 0x08, 0x08, 0x08, 0x08,  // Code for char -
        0x00, 0x00, 0x60, 0x60, 0x00, 0x00,  // Code for char .
        0x00, 0x20, 0x10, 0x08, 0x04, 0x02,  // Code for char /
        0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E,  // Code for char 0
        0x00, 0x00, 0x42, 0x7F, 0x40, 0x00,  // Code for char 1
        0x00, 0x62, 0x51, 0x49, 0x49, 0x46,  // Code for char 2
        0x00, 0x22, 0x49, 0x49, 0x49, 0x36,  // Code for char 3
        0x00, 0x18, 0x14, 0x12, 0x7F, 0x10,  // Code for char 4
        0x00, 0x2F, 0x49, 0x49, 0x49, 0x31,  // Code for char 5
        0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30,  // Code for char 6
        0x00, 0x01, 0x71, 0x09, 0x05, 0x03,  // Code for char 7
        0x00, 0x36, 0x49, 0x49, 0x49, 0x36,  // Code for char 8
        0x00, 0x06, 0x49, 0x49, 0x29, 0x1E,  // Code for char 9
        0x00, 0x00, 0x6C, 0x6C, 0x00, 0x00,  // Code for char :
        0x00, 0x00, 0xEC, 0x6C, 0x00, 0x00,  // Code for char ;
        0x00, 0x08, 0x14, 0x22, 0x41, 0x00,  // Code for char <
        0x00, 0x24, 0x24, 0x24, 0x24, 0x24,  // Code for char =
        0x00, 0x00, 0x41, 0x22, 0x14, 0x08,  // Code for char >
        0x00, 0x02, 0x01, 0x59, 0x09, 0x06,  // Code for char ?
        0x00, 0x3E, 0x41, 0x5D, 0x55, 0x1E,  // Code for char @
        0x00, 0x7E, 0x11, 0x11, 0x11, 0x7E,  // Code for char A
        0x00, 0x7F, 0x49, 0x49, 0x49, 0x36,  // Code for char B
        0x00, 0x3E, 0x41, 0x41, 0x41, 0x22,  // Code for char C
        0x00, 0x7F, 0x41, 0x41, 0x41, 0x3E,  // Code for char D
        0x00, 0x7F, 0x49, 0x49, 0x49, 0x41,  // Code for char E
        0x00, 0x7F, 0x09, 0x09, 0x09, 0x01,  // Code for char F
        0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A,  // Code for char G
        0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F,  // Code for char H
        0x00, 0x00, 0x41, 0x7F, 0x41, 0x00,  // Code for char I
        0x00, 0x30, 0x40, 0x40, 0x40, 0x3F,  // Code for char J
        0x00, 0x7F, 0x08, 0x14, 0x22, 0x41,  // Code for char K
        0x00, 0x7F, 0x40, 0x40, 0x40, 0x40,  // Code for char L
        0x00, 0x7F, 0x02, 0x04, 0x02, 0x7F,  // Code for char M
        0x00, 0x7F, 0x02, 0x04, 0x08, 0x7F,  // Code for char N
        0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E,  // Code for char O
        0x00, 0x7F, 0x09, 0x09, 0x09, 0x06,  // Code for char P
        0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E,  // Code for char Q
        0x00, 0x7F, 0x09, 0x09, 0x19, 0x66,  // Code for char R
        0x00, 0x26, 0x49, 0x49, 0x49, 0x32,  // Code for char S
        0x00, 0x01, 0x01, 0x7F, 0x01, 0x01,  // Code for char T
        0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F,  // Code for char U
        0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F,  // Code for char V
        0x00, 0x3F, 0x40, 0x3C, 0x40, 0x3F,  // Code for char W
        0x00, 0x63, 0x14, 0x08, 0x14, 0x63,  // Code for char X
        0x00, 0x07, 0x08, 0x70, 0x08, 0x07,  // Code for char Y
        0x00, 0x71, 0x49, 0x45, 0x43, 0x00,  // Code for char Z
        0x00, 0x00, 0x7F, 0x41, 0x41, 0x00,  // Code for char [
        0x00, 0x02, 0x04, 0x08, 0x10, 0x20,  // Code for char BackSlash
        0x00, 0x00, 0x41, 0x41, 0x7F, 0x00,  // Code for char ]
        0x00, 0x04, 0x02, 0x01, 0x02, 0x04,  // Code for char ^
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80,  // Code for char _
        0x00, 0x00, 0x03, 0x07, 0x00, 0x00,  // Code for char `
        0x00, 0x20, 0x54, 0x54, 0x54, 0x78,  // Code for char a
        0x00, 0x7F, 0x44, 0x44, 0x44, 0x38,  // Code for char b
        0x00, 0x38, 0x44, 0x44, 0x44, 0x28,  // Code for char c
        0x00, 0x38, 0x44, 0x44, 0x44, 0x7F,  // Code for char d
        0x00, 0x38, 0x54, 0x54, 0x54, 0x08,  // Code for char e
        0x00, 0x08, 0x7E, 0x09, 0x09, 0x00,  // Code for char f
        0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C,  // Code for char g
        0x00, 0x7F, 0x04, 0x04, 0x78, 0x00,  // Code for char h
        0x00, 0x00, 0x00, 0x7D, 0x40, 0x00,  // Code for char i
        0x00, 0x40, 0x80, 0x84, 0x7D, 0x00,  // Code for char j
        0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,  // Code for char k
        0x00, 0x00, 0x00, 0x7F, 0x40, 0x00,  // Code for char l
        0x00, 0x7C, 0x04, 0x18, 0x04, 0x78,  // Code for char m
        0x00, 0x7C, 0x04, 0x04, 0x78, 0x00,  // Code for char n
        0x00, 0x38, 0x44, 0x44, 0x44, 0x38,  // Code for char o
        0x00, 0xFC, 0x44, 0x44, 0x44, 0x38,  // Code for char p
        0x00, 0x38, 0x44, 0x44, 0x44, 0xFC,  // Code for char q
        0x00, 0x44, 0x78, 0x44, 0x04, 0x08,  // Code for char r
        0x00, 0x08, 0x54, 0x54, 0x54, 0x20,  // Code for char s
        0x00, 0x04, 0x3E, 0x44, 0x24, 0x00,  // Code for char t
        0x00, 0x3C, 0x40, 0x20, 0x7C, 0x00,  // Code for char u
        0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C,  // Code for char v
        0x00, 0x3C, 0x60, 0x30, 0x60, 0x3C,  // Code for char w
        0x00, 0x6C, 0x10, 0x10, 0x6C, 0x00,  // Code for char x
        0x00, 0x9C, 0xA0, 0x60, 0x3C, 0x00,  // Code for char y
        0x00, 0x64, 0x54, 0x54, 0x4C, 0x00,  // Code for char z
        0x00, 0x08, 0x3E, 0x41, 0x41, 0x00,  // Code for char {
        0x00, 0x00, 0x00, 0x77, 0x00, 0x00,  // Code for char |
        0x00, 0x00, 0x41, 0x41, 0x3E, 0x08,  // Code for char }
        0x00, 0x02, 0x01, 0x02, 0x01, 0x00,  // Code for char ~
        0x00, 0x3C, 0x26, 0x23, 0x26, 0x3C   // Code for char 
        };
void USART_init();
void PORTS_init();
void send(char c);
void sendstring (char *string);
int get(void);
void TIM2CONFIG();
void Delay_us (uint16_t us);
void Delay_ms (uint16_t ms);
void GLCD_Pageselect(int pge);
int get_x_pixel();
int get_y_pixel();
void GLCD_CMD(unsigned char cmd);
void GLCD_DTA(unsigned char data);
void SetY(int y);
void SetX(int x);
void set_x_address(uint16_t x,uint16_t cs);
void set_line_no(uint16_t y,uint16_t cs);
void GLCD_INIT();
void put_data(int data);
void glcd_put_char(char data);
void update();
void cls();
void clear_pixel(uint16_t x,uint16_t y);
void Set_pixel(uint16_t x,uint16_t y);
void glcd_puts(const char *data);				
int mode=0;
void reset(){
for (int i=0;i<128;i++){
for (int j=0;j<64;j++){
GLCD_Data[i][j]=0;
}
}
}
int main(){
PORTS_init();
TIM2CONFIG();
USART_init();
GLCD_INIT();
GPIOA->MODER|=0x55000000; 
volatile double TU=100;
volatile double A=1;
volatile double B=1;
char recieved;
SetX(0);
GLCD_Pageselect(0);
/*glcd_put_char('^');
for (int i=1;i<8;i++){
SetY(i);
SetX(0);
glcd_put_char('|');
}
SetY(7);
SetX(1);
for(int j=0;j<20;j++){
glcd_put_char('-');
}
glcd_put_char('>');*/
/*glcd_put_char('H');
glcd_put_char('E');
glcd_put_char('L');
glcd_put_char('L');
glcd_put_char('O');*/
update();
reset();
//uint8_t samples[128];
int temp=0;
while(1){
Delay_ms(10);
recieved=get();
switch (recieved){
	case 0xff:
	reset();
	for (int i=0;i<128;i++){
	temp=get();
	temp=(temp*A)+B;
	GLCD_Data[i][temp]=1;
	//cls();
	}
	break;
	case 0xfe:
		A=get();
	  A/=10;
	  cls();
	  break;
	case 0xfd:
		B=get();
	  B/=10;
	  cls();
	  break;
}
update();
}
}				
void USART_init(){
GPIOA->MODER |=  0x00000400;
GPIOA->BSRR=0x00200000;
GPIOA->AFR[0] &=~(0XFF00) ;
GPIOA->AFR[0] |=(0X7700) ;	
GPIOA->MODER &= ~(0X00F0) ;
GPIOA->MODER |= (0X00A0) ;	
USART2->CR1 = 0x00;
USART2->BRR |=0X683;/*SYSCLK=16MHZ , OVRSMPLING=16 , BAUDRATE=9600*/
USART2->CR1 |=(0X000C);
USART2->CR1 |=0X2000;
}
void PORTS_init(){
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN   ;
RCC->APB1ENR |= 0X20000 ; // USART2 
RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Enable the timer2 clock
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
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
int get(void){
while(!(USART2->SR & 1<<5));
return USART2->DR;
}
void TIM2CONFIG(){
	TIM2->PSC = 15;  // 16MHz/16 = 1 MHz ~~ 1 uS delay
	TIM2->ARR = 0XFFFF;  //  ARR value
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
void Delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		Delay_us (1000); // delay of 1 ms
	}
}
void GLCD_Pageselect(int pge){
switch(pge){
	case 0: 
		GPIOB->BSRR= 1<<20 | 1<<5; /*CS2=1,CS1=0*/
		cs_cursor = 0;
		break;
	case 1:
		GPIOB->BSRR= 1<<21 | 1<<4; /*CS2=0,CS1=1*/
	  cs_cursor = 1; 
		break;
	case 2:
		GPIOB->BSRR= 1<<5 | 1<<4; /*CS2=1,CS1=1*/
		break;
	case 3:
		GPIOB->BSRR= 1<<21 | 1<<20; /*CS2=0,CS1=0*/
		break;
	default:
		GPIOB->BSRR= 1<<21 | 1<<4; /*CS2=0,CS1=1*/
		break;		
}
}
int get_x_pixel(){
return (cs_cursor * 64) + x_cursor;
}

int get_y_pixel() {
	return y_cursor * 8;
} 
void GLCD_CMD(unsigned char cmd){
GPIOB->ODR &= ~(1<<0);// RS=0 
Delay_us(100);
GPIOB->ODR &=	~(1<<1) ; // RW=0
Delay_us(100);
GPIOB->ODR &= ~(1<<2) ;//E=0
Delay_us(100);
GLCD_Pageselect(0);
Delay_us(200);
//GPIOC->BSRR=0xFFFF0000;
GPIOB->BSRR = 0x4;
Delay_us(100);
GPIOC->ODR = cmd;
Delay_us(100);
GPIOB->BSRR=0x40000;
Delay_us(100);
GLCD_Pageselect(1);
Delay_us(200);
//GPIOC->BSRR=0xFFFF0000;
GPIOB->BSRR = 0x4;
Delay_us(100);
GPIOC->ODR = cmd;
Delay_us(100);
GPIOB->BSRR=0x40000;
Delay_us(100);
}
void GLCD_DTA(unsigned char data){
GPIOC->ODR = data;
GPIOB->BSRR = 1<<0 | 1<<17 | 1<<2 ; /* RS=1 , RW=0 , E=1*/
Delay_us(200);
GPIOB->BSRR =1<<18; /*E=0*/
}

char H[]={0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00};
void Glcd_DisplayChar(char *ptr_array)
{
    int i;
    for(i=0;i<6;i++) // 5x7 font, 5 chars + 1 blankspace
        GLCD_DTA(ptr_array[i]);
}
void SetY(int y){
GPIOC->ODR = (y & 0x07);
GPIOC->BSRR = 0x4000b8;
GPIOB->BSRR = 1<<16 | 1<<17 ;
GPIOB->BSRR = 1<<2;
Delay_us(100);
GPIOB->BSRR = 1<<18;
Delay_us(100);
y_cursor = y & 0X07;
 	
}
void SetX(int x){
GPIOC->ODR = (x & 0x3f);
GPIOC->BSRR = 0X800040;
GPIOB->BSRR = 1<<16 | 1<<17;
GPIOB->BSRR = 1<<2;
Delay_us(100);
GPIOB->BSRR = 1<<18;
Delay_us(100);
x_cursor = (x & 0X3F);
}
void set_x_address(uint16_t x,uint16_t cs) {
  SetX(x);
	GLCD_Pageselect(cs);
	SetY(y_cursor);
}
void set_line_no(uint16_t y,uint16_t cs) {
	GLCD_Pageselect(cs);
	SetY(y);
}

void GLCD_INIT(){
GPIOC->MODER=0X55555555;
GPIOB->MODER=0X55555555;
GPIOB->BSRR=0XFFFF0000;
Delay_us(300);
GPIOB->BSRR=1<<3;
Delay_us(300);
GLCD_CMD(0xC0);
GLCD_CMD(0x3F);
cls();

}
void put_data(int data){
GPIOC->ODR = (data & 0xff);
x_cursor+=1;
GPIOB->BSRR = 1<<0 | 1<<17;
GPIOB->BSRR = 1<<2;
Delay_us(100);
GPIOB->BSRR = 1<<18;
Delay_us(100);
if (x_cursor == 64){
	if (cs_cursor==1){
		GLCD_Pageselect(0);
		SetY((y_cursor+1)%8);
	}
	else {
		GLCD_Pageselect(1);
		SetY(y_cursor);
	}
   x_cursor = 0;
}
}
void glcd_put_char(char data) {
	int asci = (int) data;
	//Asci number of first chrachter of Terminal6x8 array is 32
	asci = asci - 32;
	//Each character is represented by six columns
	int down = asci * 6;
	for(int i = 0; i < 6; i++) {
		int res = Terminal6x8[down++];
		for(int j = 0; j < 8; j++) {
			int bit = (res >> j) & 1;
			GLCD_Data[get_x_pixel()][get_y_pixel() + j] = bit;
		}
		put_data(res);
	}

	
}
void update() {
	//Save current state
	int x_current,y_current,cs_current;
	x_current = x_cursor;
	y_current = y_cursor;
	cs_current = cs_cursor;
	//Set cursor to first line for updating GLCD
	SetX(0);
	GLCD_Pageselect(0);
	SetY(0);
	//Updating GLCD
	int result;
	for(int p = 0; p < 8; p++) {
		for(int z = 0; z < 2; z++) {
			for(int x = 0; x < 64; x++){
				result = 0;
				for(int y = 0; y < 8; y++){
					int bit = GLCD_Data[(z * 64) + x][ (p * 8) + y];
					bit = bit & 1;
					bit = bit << y;
					result = result | bit;
				}
				put_data(result);
			}
		}
	}
	//Set cursor to prev state
	SetX(x_current);
	GLCD_Pageselect(cs_current);
	SetY(y_current);
}
void cls() {
	for(int i = 0; i < 128; i++){
		for(int j = 0; j < 64; j++){
			GLCD_Data[i][j] = 0;
		}
	}
	
	update();
}
void clear_pixel(uint16_t x,uint16_t y) {
	GLCD_Data[x][y] = 0;
	update();
}
void Set_pixel(uint16_t x,uint16_t y) {
	GLCD_Data[x][y] = 1;
	update();
}
void glcd_puts(const char *data) {
		int i = 0;
  while (data[i] != '\0'){
		glcd_put_char(data[i]);
		i++;
  }
}
