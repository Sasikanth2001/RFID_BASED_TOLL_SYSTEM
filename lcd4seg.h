#include <LPC21xx.H>

// LCD Pin Definitions
#define LCD_D 0xF<<20
#define RS (1<<17)
#define RW (1<<18)
#define E  (1<<19)


// Function Declarations
void lcd_init(void);
void lcd_command(unsigned char cmd);
void lcd_data (unsigned char data);
void lcd_string(char *s);
void delay_milliseconds(unsigned int);

// LCD Functions
void lcd_init(void) {
    IODIR1 = LCD_D | RS | RW | E;
    IOCLR1 = RW;

    lcd_command(0x01);
    lcd_command(0x02);
    lcd_command(0x0C);
    lcd_command(0x28);
}

void lcd_command(unsigned char cmd) {
    IOCLR1 = LCD_D;
    IOSET1 = (cmd & 0xF0) << 16;
    IOCLR1 = RS;
    IOSET1 = E;
    delay_milliseconds(2);
    IOCLR1 = E;

    IOCLR1 = LCD_D;
    IOSET1 = (cmd & 0x0F) << 20;
    IOCLR1 = RS;
    IOSET1 = E;
    delay_milliseconds(2);
    IOCLR1 = E;
}

void lcd_data (unsigned char data) {
    IOCLR1 = LCD_D;
    IOSET1 = (data & 0xF0) << 16;
    IOSET1 = RS;
    IOSET1 = E;
    delay_milliseconds(2);
    IOCLR1 = E;

    IOCLR1 = LCD_D;
    IOSET1 = (data & 0x0F) << 20;
    IOSET1 = RS;
    IOSET1 = E;
    delay_milliseconds(2);
    IOCLR1 = E;
}

void lcd_string(char *s) {
    int count =0;
    while(*s) {
	count++;
        lcd_data(*s++);
		if(count == 16)
		lcd_command(0xc0);
		}
}
void delay_milliseconds(unsigned int second){
   T0PR = 15000-1;
   T0TCR = 0x01;
   while(T0TC < second);
   T0TCR = 0X03;
   T0TCR = 0X00;
   }
 /*

 */
