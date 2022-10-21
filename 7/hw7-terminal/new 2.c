#include "stm32f4xx.h"
#define RS 0x20     /* Pin mask for reg select (e.g. here is pin 5) */
#define RW 0x40     /* Pin mask for read/write (e.g. here is pin 6) */
#define EN 0x80     /* Pin mask for enable     (e.g. here is pin 7) */
void delayMs(int n); 
void LCD_nibble_write(char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_init(void);
void PORTS_init(void);
void uart_init();
void keypad_init();
void send(uint8_t c);
void LCD_ready();
int main(){
uart_init();
keypad_init();
    LCD_init();

 while(1) {   
        /* Write "HELLO" on LCD */
       
				LCD_data('H');
        LCD_data('E');
        LCD_data('L');
        LCD_data('L');
        LCD_data('O');
        delayMs(1000);

        /* clear LCD display */
        LCD_command(1);
        delayMs(1000);
}
		}

void uart_init(){
RCC->AHB1ENR |= 1<<0   ;
RCC->AHB1ENR |= 1<<1   ;	
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
void keypad_init(){
uint8_t reg=0;
uint8_t value=0;
GPIOB->MODER |= 0x55000000;
GPIOB->ODR   = 0xE000;
//while(reg<3){
if(GPIOB->IDR & 1<<8 ){
	value=value<<1;
	value|=1;
}
if(GPIOB->IDR & 1<<9){
	value=value<<1;
	value|=2;
}
if(GPIOB->IDR & 1<<10 ){
	value=value<<1;
	value|=3;
}	
GPIOB->ODR   = 0xD000;
if(GPIOB->IDR & 1<<8 ){
	value=value<<1;
	value|=4;
}
if(GPIOB->IDR & 1<<9){
	value=value<<1;
	value|=5;

}
if(GPIOB->IDR & 1<<10 ){
	value=value<<1;
	value|=6;
}	
GPIOB->ODR   = 0xB000;
if(GPIOB->IDR & 1<<8 ) {
	value=value<<1;
	value|=7;
}
if(GPIOB->IDR & 1<<9){
	value=value<<1;
	value|=8;
}
if(GPIOB->IDR & 1<<10 ){
	value=value<<1;
	value|=9;
}	
GPIOB->ODR   = 0x7000;
if(GPIOB->IDR & 1<<8 ){}
if(GPIOB->IDR & 1<<9){
	value=value<<1;
	value|=0;
}
if(GPIOB->IDR & 1<<10 ){
	value=0;
	reg+=1;
}
}
//}

void send(uint8_t c)
{
USART1->DR=c;
while(!(USART1->SR & 1<<6));
GPIOA->ODR = 0;
}
void LCD_init(void) {
    PORTS_init();	/* initialize the ports that connect to the LCD */

    /* LCD controller reset sequence */
    delayMs(20);
    LCD_nibble_write(0x30, 0);
    delayMs(5);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x30, 0);  /* use 4-bit data mode */
    delayMs(1);
    LCD_command(0x38);          /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);          /* move cursor right */
    LCD_command(0x01);          /* clear screen, move cursor to home */
    LCD_command(0x0F);          /* turn on display, cursor blinking */
}
void PORTS_init(void) {
    GPIOB->MODER |= 0x00005500;               /* Initialize needed GPIOs and set ports mode appropriately  */
		RCC->AHB1ENR |= 1<<2;
		GPIOC->MODER = 0x55555555;
}

void LCD_nibble_write(char data, unsigned char control) {
    /* populate data bits */
    GPIOC->BSRR = 0x00FF0000;   // clear D7-0
    GPIOC->BSRR = data ;    // set D7-0

    /* set R/S bit */
    if (control & RS)
        GPIOB->BSRR = 0x0020;
    else
        GPIOB->BSRR = 0x00200000;

    /* pulse E */
    delayMs(0);
    GPIOB->BSRR = 0X00000080;
    delayMs(0);
    GPIOB->BSRR = 0X00800000;
}

void LCD_command(unsigned char command) {
    LCD_nibble_write(command ,0 );    
    //LCD_nibble_write(command << 4, 0);      

    if (command < 4)
        delayMs(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(char data) {
    LCD_nibble_write(data , RS);      /* upper nibble first */
    //LCD_nibble_write(data << 4, RS);        /* then lower nibble */

    delayMs(1);
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}
void LCD_ready(){
char status;
GPIOC->MODER =0x55551555;
GPIOB->ODR |= 1<<6;
GPIOB->ODR |= (1<<7);
delayMs(0);
status=GPIOC->IDR;
GPIOB->ODR &= ~(1<<7);
delayMs(0);	
while(status  & 1<<7){
GPIOB->ODR |= (1<<7);
delayMs(0);
status=GPIOC->IDR;
GPIOB->ODR &= ~(1<<7);
delayMs(0);
}
GPIOC->MODER=0X55555555;
GPIOB->ODR &= ~(1<<6)  ;	
}