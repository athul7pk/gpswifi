/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	* File Name : kios.c

	* Creation Date : 02-12-2016

	* Last Modified : Monday 05 December 2016 06:27:39 PM IST

	* Created By :  Athul P.K

	_._._._._._._._._._._._._._._._._._._._._.*/
#include "includes.h"
#define HTTP_PORT (80)
#define RCVBUFFLEN (1024)
char Keypress=0;
char Id[10];
char ServerIp[15];
char ServerPort[7];
char g_Pass[20];
char HTTPS_IP[20];
char recvbuf[RCVBUFFLEN];
char Sendbuf[RCVBUFFLEN];
struct sockaddr_in cloudaddr;
struct in_addr	addrKA;
static char Request[700] ;
extern char lat[];
extern char lon[];
extern char Location[70];
extern pthread_mutex_t LocLock;
int  main(){
								char * p_nvr;
								struct hostent *hostn = 0;
								p_nvr=nvram_get(7,"Id");
								if(p_nvr!=NULL){
																strcpy(Id,p_nvr);
								}
								else{
																printf("NVRAM corrupted \r\n");
																//reccover
								}
								p_nvr=nvram_get(7,"serverIp");
								if(p_nvr!=NULL){
																strcpy(ServerIp,p_nvr);
								}
								else{
																printf("NVRAM corrupted \r\n");
																//reccover
								}
								p_nvr=nvram_get(7,"serverPort");
								if(p_nvr!=NULL){
																strcpy(ServerPort,p_nvr);
								}
								else{
																printf("NVRAM corrupted \r\n");
																//reccover
								}
								p_nvr=nvram_get(7,"passWord");
								if(p_nvr!=NULL){
																strcpy(g_Pass,p_nvr);
								}
								else{
																printf("NVRAM corrupted \r\n");
																//reccover
								}

								printf("DevId=%s\r\n",Id);
								printf("Server @ %s:%s\r\n",ServerIp,ServerPort);
								printf("Welcome\r\n");
								lcd_init();
								sendCommand(0x80);
								lcd_puts("     Welcome          ");
								init_GPS();
								init_Keypad();
								sendCommand(0xc0);
								lcd_puts("     Loaded            ");
								sleep(1);
								while(!if_check()){
																printf("Waiting for Ip\r\n");
																if(Keypress==0){
																								sendCommand(0xc0);
																								lcd_puts("Waiting for IP      ");
																}
																sleep(7);

								}
								const char* host_name = nvram_get(7, "HostName");
								printf("Host:==> %s\r\n",host_name);
								hostn= gethostbyname(host_name);
								if (!hostn) 		  {											//host DNS resolving failure
																printf("hostname fetch :failed");
																sleep(3);
								} else {
																strcpy(HTTPS_IP, inet_ntoa( (struct in_addr) *((struct in_addr *) hostn->h_addr_list[0])));
																printf("%s is  on  %s \r\n",host_name,HTTPS_IP);

								}
								SecLayerInit();
								if (cloud_inf_init(0,HTTPS_IP, &cloudaddr,NULL) == -1)
								{
																printf("Cloud info init error\n");
																sleep (1);

								}
								printf("SSL OK\n");
								sendCommand(0xc0);
								lcd_puts("Server Connected ...  ");
								sendCommand(0x80);
								lcd_puts("     Welcome      ");
								sendCommand(0xc0);
								lcd_puts("Enter MobNumber ");

								while(1){
																//	KB_Task(NULL);
																usleep(1000*1000*17);
								}
								return 0;
}
float convertToMap(float rawLatLong,char  dir){
								static float latlong;
								latlong=((int)(rawLatLong / 100)) + ((rawLatLong - (((int)(rawLatLong / 100)) * 100)) / 60);
								if((dir=='S')||(dir=='W')){
																latlong=latlong*-1;
								}
								return latlong;
}
void SendDataToTheServer(char *Num){
								int retry=0;
								float latc,lonc;
								char dir;
								pthread_mutex_lock(&LocLock);
								pthread_mutex_unlock(&LocLock);
								dir=lat[10];
								lat[9]=0;
								latc=convertToMap(atof(lat),dir);
								dir=lon[10];
								lon[9]=0;
								lonc=convertToMap(atof(lon),dir);

								sprintf(Request,"GET https://dheauto.com/dhericknew/user/create_request?cat=3&DevId=%s&mob_number=%s&latitude=%f&longitude=%f\r\n\r\n\r\n\r\n",Id,Num,latc,lonc);

retr:		if (httpsGet(0, Request, recvbuf, RCVBUFFLEN) < 1) {
															sendCommand(0x80);
															lcd_puts("ERROR          ");		
															sendCommand(0xc0);
															lcd_puts("Server Error   ");	
															sleep(1);
															return;
							}	
							sendCommand(0x80);
							lcd_puts("  *    OK   *");		
							sendCommand(0x80);
							lcd_puts("     We will   ");		
							sendCommand(0xc0);
							lcd_puts("Get back to you");	
							sleep(1);
}


int if_check() {
								int fd;
								char ipadd[16];
								struct ifreq ifr;

								memset(&ifr, 0, sizeof(ifr));

								fd = socket(AF_INET, SOCK_DGRAM, 0);

								/* I want to get an IPv4 IP address */
								ifr.ifr_addr.sa_family = AF_INET;

								/* I want IP address attached to "ra0" */
								strncpy(ifr.ifr_name, "apcli0", IFNAMSIZ - 1);

								ioctl(fd, SIOCGIFADDR, &ifr);

								close(fd);

								/* display result */
								strcpy(ipadd, inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr));
								if (inet_aton(ipadd, &addrKA) == 0) {

																fprintf(stderr, "Invalid address\n");
																//exit(EXIT_FAILURE);
																addrKA.s_addr = 0;
								}


								if (strcmp(ipadd, "0.0.0.0") == 0)
																return 0;
								else
																return 1;
}
