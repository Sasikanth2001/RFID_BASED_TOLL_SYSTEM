#include <LPC21xx.h>
#include <string.h>
#include <stdio.h>
#include "lcd4seg.h"
#include "uart.h"
#include "servomotor1.h"  // Custom header to control servo motor (via PWM)

#define SW1 14 // EINT0 – open gate
#define SW2 15 // EINT1 – close gate
#define SW3 16 // EINT2 – collection
#define IRQ_Sloten (1 << 5)

#define EINT0_fn (1 << 0)      // P0.16 (bits 1:0) -> EINT0
#define EINT1_fn (1 << 29)     // P0.14 (bits 29:28) -> EINT1
#define EINT2_fn (1 << 31)     // P0.15 (bits 31:30) -> EINT2

#define motor (1 << 17)    //p0.8

int total_collection = 0;
int flag = 0, flag1 = 0, flag2 = 0;

// Function Declarations
void extint0_isr(void) __irq;
void extint1_isr(void) __irq;
void extint2_isr(void) __irq;
void car_fun(void);
void bike_fun(void);
void bus_fun(void);
void collection(int);
void set_servo_angle_90(void);
void set_servo_angle_0(void);

int main() {
    char rx_byte[12];
    char RFID_CAR[] = "0600672F612F";
    char RFID_BUS[] = "060066FDC855";
    char RFID_BIKE[] = "060067882CC5";

    lcd_init();
    lcd_command(0x80);
    lcd_string("Vector Toll Based RFID System");
    UART0_CONFIG();
    PWM1_init();

    //IODIR0 |= motor;    // Set motor pin as output
   // IOSET0 |= motor;    // Turn off motor initially

    // Interrupt Pin Configuration
    PINSEL1 |= EINT0_fn;         // P0.16 -> EINT0
    PINSEL0 |= EINT1_fn;         // P0.14 -> EINT1
    PINSEL0 |= EINT2_fn;         // P0.15 -> EINT2

    VICIntSelect = 0x00000000;

    VICVectCntl0 = IRQ_Sloten | 14;   // EINT0
    VICVectAddr0 = (unsigned long)extint0_isr;

    VICVectCntl1 = IRQ_Sloten | 15;   // EINT1
    VICVectAddr1 = (unsigned long)extint1_isr;

    VICVectCntl2 = IRQ_Sloten | 16;   // EINT2
    VICVectAddr2 = (unsigned long)extint2_isr;

    EXTMODE = 0x07;      // Edge-sensitive
    EXTPOLAR = 0x00;     // Falling edge
    VICIntEnable = (1 << 14) | (1 << 15) | (1 << 16);

    while (1) {
        UART_RXSTRING(rx_byte);
        UART_TXSTRING(rx_byte);
        delay_milliseconds(1000);

        lcd_command(0x01);
        lcd_command(0x80);
        lcd_string("Received");
        delay_milliseconds(1000);

        if (strcmp(RFID_CAR, rx_byte) == 0) {
            car_fun();
        } else if (strcmp(RFID_BIKE, rx_byte) == 0) {
            bike_fun();
        } else if (strcmp(RFID_BUS, rx_byte) == 0) {
            bus_fun();
        } else {
            lcd_command(0x01);
            lcd_command(0x80);
            lcd_string("Invalid/Not Matching");
            delay_milliseconds(1000);
        }
		/*
        if (flag == 1) {
           // IOSET0 |= motor;
		   
        }  

        if (flag1 == 1) {
            //IOSET0 |= motor;
			delay_milliseconds(5000);
            lcd_command(0x01);
            lcd_command(0x80);
            lcd_string("CLOSING GATE");
            lcd_command(0xC2);
            lcd_string("0 degree");
			 delay_milliseconds(5000);
            set_servo_angle_0();
            delay_milliseconds(5000);
            flag1 = 0;
        }

        if (flag2 == 1) {
		    delay_milliseconds(5000);
            lcd_command(0x01);
            lcd_command(0x80);
            lcd_string("Total collection:");
			 delay_milliseconds(5000);
            collection(total_collection);
            delay_milliseconds(5000);
            flag2 = 0;
        }
		*/
    }
}

// ===== Vehicle Functions =====
void car_fun() {
    lcd_command(0x01);
    lcd_command(0x80);
    lcd_string("CAR : PAY 10 RS");
    delay_milliseconds(2000);
    lcd_command(0x01);
    lcd_string("WAIT FOR SW1...");
    total_collection += 10;
}

void bike_fun() {
    lcd_command(0x01);
    lcd_command(0x80);
    lcd_string("BIKE : PAY 5 RS");
    delay_milliseconds(2000);
    lcd_command(0x01);
    lcd_string("WAIT FOR SW1...");
    total_collection += 5;
}

void bus_fun() {
    lcd_command(0x01);
    lcd_command(0x80);
    lcd_string("BUS : PAY 20 RS");
    delay_milliseconds(2000);
    lcd_command(0x01);
    lcd_string("WAIT FOR SW1...");
    total_collection += 20;
}

// ===== ISR Functions =====
void extint0_isr(void) __irq {
    EXTINT = 0x01;
    //IOCLR0 |= motor;
	lcd_command(0x01);
    lcd_string("EINT0 Triggered");  // <-- Add this
    delay_milliseconds(1000);
     
	// delay_milliseconds(5000);
            lcd_command(0x01);
            lcd_command(0x80);
            lcd_string("OPENING GATE");
            lcd_command(0xC2);
            lcd_string("90 degree");
			 delay_milliseconds(5000);
            set_servo_angle_90();
            delay_milliseconds(5000);
            flag = 0;    

    VICVectAddr = 0;
}

void extint1_isr(void) __irq {
    EXTINT = 0x02;
   // IOCLR0 |= motor;
   lcd_command(0x01);
    lcd_string("EINT1 Triggered");  // <-- Add this
    //delay_milliseconds(1000);

            delay_milliseconds(5000);
            lcd_command(0x01);
            lcd_command(0x80);
            lcd_string("CLOSING GATE");
            lcd_command(0xC2);
            lcd_string("0 degree");
			 delay_milliseconds(5000);
            set_servo_angle_0();
            delay_milliseconds(5000);

    VICVectAddr = 0;
}

void extint2_isr(void) __irq {
    EXTINT = 0x04;
	lcd_command(0x01);
    lcd_string("EINT2 Triggered");  // <-- Add this
    //delay_milliseconds(1000);
    
            delay_milliseconds(5000);
            lcd_command(0x01);
            lcd_command(0x80);
            lcd_string("Total collection:");
			 delay_milliseconds(5000);
            collection(total_collection);
            delay_milliseconds(5000);

    VICVectAddr = 0;
}

// ===== Collection Display =====
void collection(int n) {
    unsigned char a[5];
    int i = 0;

    if (n == 0) {
        lcd_data('0');
    } else {
        if (n < 0) {
            lcd_data('-');
            n = -n;
        }
        while (n > 0) {
            a[i++] = n % 10;
            n = n / 10;
        }
        for (--i; i >= 0; i--) {
            lcd_data(a[i] + '0');
        }
    }
}

// ===== Servo Motor Control =====
void set_servo_angle_90() {
    int value = 1000;
    PWMMR4 = value;
    PWMLER = 0x10;  // Latch MR4
    delay_milliseconds(5000);
    lcd_command(0x01);
    lcd_string("WAIT FOR SW2...");
}

void set_servo_angle_0() {
    int value = 500;
    PWMMR4 = value;
    PWMLER = 0x10;
    delay_milliseconds(5000);
    lcd_command(0x01);
    lcd_string("Press SW3 to View Total");
}



/*
First, I check whether the ID matches or not.

    If it matches, I prompt the user to press SW1 to open the gate. This invokes EXTINT0 and activates the motor.

    At the end of the above function, I prompt the user to press SW2 to close the gate. This also invokes EXTINT1 and runs the motor.

    Finally, I prompt the user to press SW3 to invoke EXTINT1 and display the total collection.

I used UART communication for the RFID reader and tag.
I used a PWM pin (not part of any communication protocol) to control the motor as an output device.
For the switches, I implemented the interrupt concept to handle inputs.




learned from mistake
in debugging section
* first you will check each pin is invoked or not using --> pin connect block
* if you use interrupt check VIC block - interrupt is rised on or not. Not only interrupt uart or timer
* give correct pinsel 
* if you use output device like led, motor... use the IODIR0,IOSET0,IOCLR0
*/
