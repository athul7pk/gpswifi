/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : lcd.c

* Creation Date : 02-12-2016

* Last Modified : Monday 05 December 2016 12:05:46 PM IST

* Created By :  Athul P.K

_._._._._._._._._._._._._._._._._._._._._.*/
#include "includes.h"
void DATA(char data){
		char PioCmd[70];
		sprintf(PioCmd,"echo %d > /sys/class/gpio/gpio14/value",((data&0x80)>>7));
	//	printf("%s\r\n",PioCmd);
		system(PioCmd);
		sprintf(PioCmd,"echo %d > /sys/class/gpio/gpio15/value",((data&0x40)>>6));

	//	printf("%s\r\n",PioCmd);
		system(PioCmd);
		sprintf(PioCmd,"echo %d > /sys/class/gpio/gpio16/value",((data&0x20)>>5));

	//	printf("%s\r\n",PioCmd);
		system(PioCmd);
		sprintf(PioCmd,"echo %d > /sys/class/gpio/gpio17/value",((data&0x10)>>4));

	//	printf("%s\r\n",PioCmd);
		system(PioCmd);

}
void lcd_init(){

		system("echo 14 > /sys/class/gpio/export");
		system("echo \"out\" > /sys/class/gpio/gpio14/direction");
		system("echo 15 > /sys/class/gpio/export");
		system("echo \"out\" > /sys/class/gpio/gpio15/direction");
		system("echo 16 > /sys/class/gpio/export");
		system("echo \"out\" > /sys/class/gpio/gpio16/direction");
		system("echo 17 > /sys/class/gpio/export");
		system("echo \"out\" > /sys/class/gpio/gpio17/direction");
		system("echo 1 > /sys/class/gpio/export");
		system("echo \"out\" > /sys/class/gpio/gpio1/direction");
		system("echo 2 > /sys/class/gpio/export");
		system("echo \"out\" > /sys/class/gpio/gpio2/direction");
		sendCommand(0x33);
		sendCommand(0x32);
		sendCommand(0x28);
		sendCommand(0x0c);
		sendCommand(0x01);
		sendCommand(0x06);
		sendCommand(0x80);
}
void sendCommand(char x){
		RS0;
		EN1;
		DATA(x);
	//	delay_ms(.1);
		EN0;
		RS0;
		EN1;
		DATA(x<<4);
	//	delay_ms(.1);
		EN0;
}


void lcd_putc(char x){
		
		RS1;
		EN1;
		DATA(x);
//		delay_ms(.1);
		EN0;
		RS1;
		EN1;
		DATA(x<<4);
//		delay_ms(.1);
		EN0;

}


void lcd_puts( char* s ){
	//	printf("\r\n");
		while(*s){
				putchar(*s);
				lcd_putc(*s++);
		}

}


