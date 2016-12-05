#ifndef __LCD_H_
#define __LCD_H_
#include<stdio.h>
#define RS0 system("echo 0 > /sys/class/gpio/gpio1/value")
#define RS1 system("echo 1 > /sys/class/gpio/gpio1/value")
#define EN0 system("echo 0 > /sys/class/gpio/gpio2/value")
#define EN1 system("echo 1 > /sys/class/gpio/gpio2/value")
#define delay_ms(x) usleep((x*1000));


void lcd_init();
void sendCommand(char );
void lcd_putc(char );
void lcd_puts(char* s );
void DATA(char data);
#endif
