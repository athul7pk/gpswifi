#ifndef __KIOS_H
#define __KIOS_H
void SendDataToTheServer(char *s );
int	http_sock_init(void); 
int  initHttp(void);
int if_check(void);
int httpget(struct sockaddr_in serveraddr, char *sendbuffer, char *recvbuffer,int recvsz) ;
#endif
