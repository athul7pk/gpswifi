/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

 * File Name : gps.c

 * Creation Date : 01-11-2016

 * Last Modified : Monday 05 December 2016 06:29:31 PM IST

 * Created By :  Athul P.K

 _._._._._._._._._._._._._._._._._._._._._.*/
#include "includes.h"
#define UART_BUFFER_SIZE (256)

char g_GpsDataBuf[200];
int GPS_Fdes;
pthread_t GPS_Reader_thread;
pthread_mutex_t LocLock;
char Location[100];
char lat[20]="0959.84615,W";
char lon[20]="07618.11188,E"; 
void init_GPS(){
		struct termios oldtio;
		GPS_Fdes = open(GPSMODEMDEVICE, O_RDWR | O_NOCTTY);
		if (GPS_Fdes < 0) {
				perror("GPS::COM:");
				return ;
		}
		tcgetattr(GPS_Fdes, &oldtio); // save current serial port settings /
		oldtio.c_iflag = IGNPAR;
		oldtio.c_oflag = 0;
		oldtio.c_lflag = 0;
		oldtio.c_cc[VMIN] = 1;
		oldtio.c_cflag = CS8 | CREAD | CLOCAL;
		tcflush(GPS_Fdes, TCIFLUSH);
		cfsetispeed( &oldtio,GPSBAUDRATE);
		cfsetospeed( &oldtio,GPSBAUDRATE);
		if( tcsetattr(GPS_Fdes, TCSANOW, &oldtio) < 0 ) 
				perror("tcset");
		pthread_create(&GPS_Reader_thread,NULL,GPS_Reader_Task,NULL);
		pthread_mutex_init(&LocLock,NULL);

}

void* GPS_Reader_Task(void *s){
		static int res,loc;
		int nCommas=0;
		char GpsData[50];
		char *p_loc,i,j;
		while(1){
				while(g_GpsDataBuf[0]!='$')
				{
						res = read(GPS_Fdes, g_GpsDataBuf,1 );
				}

				memset(GpsData,0,50);
				memset(g_GpsDataBuf,sizeof(g_GpsDataBuf),0);
				loc=0;
				do{
						res = read(GPS_Fdes, g_GpsDataBuf+loc,1 );
						loc+=res;
				}while(loc<=4);
				if(!strncmp(g_GpsDataBuf,"GPRMC",5)){
						loc=0;
						res=0;
						nCommas=0;

#if 1
						do{
								loc=loc+res;
								res = read(GPS_Fdes, g_GpsDataBuf+loc,1 );
								if(g_GpsDataBuf[loc]==','){

										nCommas++;

								}

								if(nCommas==DATEPOS){
										g_GpsDataBuf[loc]=0;
										//sendToServer('0',g_GpsDataBuf);
										p_loc=strstr(g_GpsDataBuf,",A,");
										if(p_loc){
												pthread_mutex_lock(&LocLock);
												for(i=0;;i++){
														lat[i]=*(p_loc+3+i);
														if(*(p_loc+i+3+1)==','){
																lat[i+1]=(*(p_loc+i+3+1));
																lat[i+2]=(*(p_loc+i+3+2));
																lat[i+3]=0;
																break;
														}
												}
														j=0;

														while(1){
																lon[j]=(*(p_loc+3+i+4));
																if(*(p_loc+i+3+1+4)==','){
																		lon[j+1]=(*(p_loc+i+3+1+4));
																		lon[j+2]=(*(p_loc+i+3+2+4));
																		lon[j+3]=0;
																		break;
																}
																		j++;
																		i++;
														}

												//		printf("Lon=%s\r\n",lon);
													//	printf("Lat=%s\r\n",lat);
														strcpy(Location,g_GpsDataBuf+1);
														pthread_mutex_unlock(&LocLock);
												}
												loc=0;
												res=0;

										}

								}while(g_GpsDataBuf[loc]!='\n');

#endif

						}
				}

		}



