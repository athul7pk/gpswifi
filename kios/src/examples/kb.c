/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	* File Name : kb.c

	* Creation Date : 02-12-2016

	* Last Modified : Monday 05 December 2016 06:53:53 PM IST

	* Created By :  Athul P.K

	_._._._._._._._._._._._._._._._._._._._._.*/
#include "includes.h"
#define UART_BUFFER_SIZE (256)
char g_KBBuf[4];
char Num[25];
int KB_Fdes,i;
pthread_t KB_thread;
extern char g_Pass[];
extern char Keypress;
extern char lat[];
extern char lon[];
void init_Keypad(){
								struct termios oldtio;
								KB_Fdes = open(KBMODEMDEVICE, O_RDWR | O_NOCTTY);
								if (KB_Fdes < 0) {
																perror("KB::COM:");
																return ;
								}
								tcgetattr(KB_Fdes, &oldtio); // save current serial port settings /
								oldtio.c_iflag = IGNPAR;
								oldtio.c_oflag = 0;
								oldtio.c_lflag = 0;
								oldtio.c_cc[VMIN] = 1;
								oldtio.c_cflag = CS8 | CREAD | CLOCAL;
								tcflush(KB_Fdes, TCIFLUSH);
								cfsetispeed( &oldtio,B9600);
								cfsetospeed( &oldtio,B9600);
								if( tcsetattr(KB_Fdes, TCSANOW, &oldtio) < 0 ) // KG - Existing settings are modified. If failed???
																perror("tcset");
								pthread_create(&KB_thread,NULL,KB_Task,NULL);
}
void* KB_Task(void *s){
								int pos=0,initial=0;
								while(1){
																read(KB_Fdes, g_KBBuf,1 );
																//		g_KBBuf[0]=getchar();
																if(((g_KBBuf[0]>='0')&&(g_KBBuf[0]<='9'))||((g_KBBuf[0]>='A')&&(g_KBBuf[0]<='D'))||(g_KBBuf[0]=='*')||(g_KBBuf[0]=='#')){
																								Keypress=1;
																								if(g_KBBuf[0]=='*'){
																																pos=0;
																																sendCommand(1);
																																sendCommand(0x80);
																																lcd_putc('*');

																																while(g_KBBuf[0]!='C'){
																																								//scanf(" %c",g_KBBuf);
																																								read(KB_Fdes, g_KBBuf,1 );
																																								if(g_KBBuf[0]=='C'){
																																																Keypress=0;
																																									break;							
																																								};

																																								if(((g_KBBuf[0]>='0')&&(g_KBBuf[0]<='9'))||((g_KBBuf[0]>='A')&&(g_KBBuf[0]<='D'))||(g_KBBuf[0]=='*')||(g_KBBuf[0]=='#')){
																																																printf("Ok\r\n");
																																								}else{
																																																initial=0;
																																																continue;
																																								}
																																								if((g_KBBuf[0]=='#'))	{

																																																Num[pos]=0;
																																																if(!strcmp(Num,g_Pass)){
																																																								printf("Entered Configuration mode\r\n");
																																																								sendCommand(0x80);
																																																								lcd_puts("Config Mode       ");
																																																								sendCommand(0xC0);
																																																								lcd_puts("IP 192.168.100.1 ");
																																																								system("cp /usr/bin/AppData/kios/wireless  /etc/config/");
																																																								sleep(2);
																																																								sendCommand(0x80);
																																																								lcd_puts("Please Wait !!!  ");
																																																								system("reboot");
																																																								sleep(1);

																																																}

																																								}
																																								Num[pos++]=g_KBBuf[0];
																																								if(pos>13){pos=0;};
																																								lcd_putc(g_KBBuf[0]);
																																								initial=0;
																																}

																								}
																								else if(g_KBBuf[0]=='D'){
																																initial=0;

																																Keypress=0;
																																if(pos==10){

																																								printf("Dialing server %s \r\n",Num);
																																								sendCommand(1);
																																								sendCommand(0x80);
																																								lcd_puts("Registering...");
																																								SendDataToTheServer(Num);
																																								pos=0;
																																								memset(Num,0,13);
																																								sendCommand(0x80);
																																								lcd_puts("     Welcome      ");
																																								sendCommand(0xc0);
																																								lcd_puts("Enter MobNumber ");

																																}
																																else{
																																								printf("Invalid Number\r\n");
																																								goto Invalid;


																																}


																								}
																								else if(g_KBBuf[0]=='C'){
																																printf("Clear\r\n");
																																initial=0;

																																Keypress=0;
																																printf("Enter MobNumber\r\n");
																																sendCommand(0x80);
																																lcd_puts("     Welcome      ");
																																sendCommand(0xc0);
																																lcd_puts("Enter MobNumber ");
																																pos=0;
																																memset(Num,0,13);

																								}
																								else if(g_KBBuf[0]=='B'){

																																sendCommand(1);
																																sendCommand(0x80);
																																lcd_puts(lat);
																																sendCommand(0xc0);
																																lcd_puts(lon);
																									

																								}
																								else if(g_KBBuf[0]=='A'){

																								}


																								else  {

																																Num[pos++]=g_KBBuf[0];
																																printf("Num%s\r\n",Num);
																																if(initial==0){
																																								initial=1;
																																								sendCommand(1);
																																								sendCommand(0x80);
																																}
																																lcd_putc(g_KBBuf[0]);

																								}
																								if(pos>11){
																																initial=0;
Invalid:																								sendCommand(1);
																																printf("Invalid number");
																																sendCommand(0x80);
																																lcd_puts("Invalid Number");
																																sleep(1);
																																pos=0;
																																memset(Num,0,13);
																																sendCommand(0x80);
																																lcd_puts("     Welcome      ");
																																sendCommand(0xc0);
																																lcd_puts("Enter MobNumber ");





																								}

																}

								}

}
#if 0
void main(){
								init_Keypad();
								while(1)	{
								}
}
#endif
