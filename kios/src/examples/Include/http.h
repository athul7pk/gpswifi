#ifndef __HTTP_H
#define __HTTP_

int connectServer(char *server_ip, sslKeys_t *keys, sslConn_t **conn);
int httpsGet(sslConn_t *conn, char *sendbuffer, char *recvbuffer, int recvsz) ;
int cloud_inf_init( char *server_ip, struct sockaddr_in *serveraddr,sslKeys_t **keys) ;

	
#endif
