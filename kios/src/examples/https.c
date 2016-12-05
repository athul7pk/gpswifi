/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : httpss.c

* Creation Date : 05-12-2016

* Last Modified : Monday 05 December 2016 04:08:46 PM IST

* Created By :  Athul P.K

_._._._._._._._._._._._._._._._._._._._._.*/
#include "includes.h"
struct sockaddr_in cloudaddr;
struct in_addr addrKA;
SSL_CTX *g_ctx=NULL;
SSL *g_ssl=NULL;
BIO *g_sbio=NULL;//,*g_bio=NULL;
int g_Tsock=0;
extern char HTTPS_IP[];
int cloud_inf_init(int type, char *server_ip, struct sockaddr_in *serveraddr,int k);
int SecureLayerConnect(char*host,int port);
void SecLayerInit();
int httpsGet(void *conn, char *Req, char *Resp, int recvsz);
void Reconnect();
/*********************************************************************************
* NAME :         int httpsGet(void *conn, char *Req, char *Resp, int recvsz)
*
* DESCRIPTION :     Send the restfull API to server an return the response buffer over ssl
*
* INPUTS :
*       PARAMETERS:
*			void *conn
*			char *Req
*			char *Resp
*			int recvs
*			
*       GLOBALS :
*			None
* OUTPUTS :
*       PARAMETERS:
*			None			
*
*       GLOBALS :
*			None
*
*       RETURN :
*            Type:  int				+ve number on success
*							-1 if operation failed
*********************************************************************************/
void Reconnect(){
		printf("Reconnecting...\r\n");
system("date +%H:%M:%S:%N>> rc.log");
 SecureLayerConnect(HTTPS_IP,443);
}
int httpsGet(void *conn, char *Req, char *Resp, int recvsz)
{   
		int Offset,ret;
		struct timeval tv;
		fd_set readfds;
  printf("Req\r\n%s\r\n",Req);  
		if((-1==SecureLayerConnect(0,0))){
				return -1;
		}
		if(fcntl(g_Tsock,F_SETFL,fcntl(g_Tsock,F_GETFL,0)|O_NDELAY)){
				printf("Couldn't make socket nonblocking");
		}
		FD_ZERO(&readfds);
		FD_SET(g_Tsock,&readfds);
		if(SSL_ERROR_NONE!=SSL_get_error(g_ssl,SSL_write(g_ssl,Req,strlen(Req)))){
				printf("SSL write problem\r\n");
				return -1;
		}
		tv.tv_sec =5;
		tv.tv_usec = 0;
		memset(Resp,0,recvsz);
		if (select(g_Tsock+1,  &readfds,NULL, NULL,&tv) == -1){
				perror("select():");
				return  -1;
		}
		else if(FD_ISSET(g_Tsock,&readfds)){
				Offset=0;
				do {
ReadMore:
						Offset=SSL_read(g_ssl,Resp+Offset,recvsz);
						switch(SSL_get_error(g_ssl,Offset)){
								case SSL_ERROR_NONE:
		//								printf("SSL_ERR_Non\r\n");
										goto ReadMore;
										break;
								case SSL_ERROR_ZERO_RETURN:
				//						printf("o Return\r\n");
										break;
								case SSL_ERROR_WANT_READ:
						//				printf("ERR wnat Read\r\n");
										break;
								case SSL_ERROR_WANT_WRITE:
								//		printf("want write\r\n");
										break;
								default:
          Reconnect();
										return -1;
						}
				} while (SSL_pending(g_ssl));
		}
		else {
				perror("select:");
				printf("No data within TimeOutseconds.\n\r");
				return -1;
		}

 printf("Resp\r\n%s\r\n",Resp);  
		ret=strstr(Resp, "success") ? 1: -1;
		if (ret == -1){
				printf("Server error\r\n");
				ret = -2;
		}
			SSL_shutdown(g_ssl);
		//		SSL_free(g_ssl);
				close(g_Tsock);
		//
		return ret;
} 

void check_cert(ssl,host)
	SSL *ssl;
	char *host;
{
	X509 *peer;
	char peer_CN[256];
	if(SSL_get_verify_result(ssl)!=X509_V_OK)
		printf("Certificate doesn't verify");
	peer=SSL_get_peer_certificate(ssl);
	X509_NAME_get_text_by_NID
		(X509_get_subject_name(peer),
		 NID_commonName, peer_CN, 256);
	if(strcasecmp(peer_CN,host))
		printf	("Common name doesn't match host name");
}
		struct hostent *hp;
		struct sockaddr_in addr;
		struct in_addr *addr_list;
		int i;
		fd_set  sock_set;
		struct timeval tv;
		int ret=0;	


/*********************************************************************************
* NAME :         void SecLayerInit()
*
* DESCRIPTION :     initialise the secure layer 
*
* INPUTS :
*       PARAMETERS:
*			None
*			
*       GLOBALS :
*			None
* OUTPUTS :
*       PARAMETERS:
*			None			
*
*       GLOBALS :
*			None
*
*       RETURN :
*            		None
*
*********************************************************************************/		
void SecLayerInit(){
		hp=gethostbyname(HTTPS_IP);
		if(!hp){
				printf("Couldn't resolve host\r\n");
				sleep(1);
				return;
		}
		memset(&addr,0,sizeof(addr));
		addr.sin_addr=*(struct in_addr*)
				hp->h_addr_list[0];
		addr.sin_family=AF_INET;
		addr.sin_port=htons(443);
		addr_list = (struct in_addr *)*hp->h_addr_list;
		printf("Hostname resolved\r\n-->[ %s] \r\n",HTTPS_IP);
	
		SSL_library_init();
		ERR_load_BIO_strings();
		SSL_load_error_strings();
		OpenSSL_add_all_algorithms();
		/* Set up the SSL context */
		g_ctx = SSL_CTX_new(SSLv23_client_method());
		/* Load the trust store */
#if WC_ENABLE_CERT_CHECK
		if(! SSL_CTX_load_verify_locations(g_ctx, CAFile, NULL))
		{
				fprintf(stderr, "Error loading trust store\n");
				ERR_print_errors_fp(stderr);
				SSL_CTX_free(g_ctx);
				return;
		}
#endif


}

/*********************************************************************************
* NAME :          int SecureLayerConnect(char*host,int port)
*
* DESCRIPTION :     Connect over SSL to the given host over given port
*
* INPUTS :
*       PARAMETERS:
*			char*host
*			int port
*			
*       GLOBALS :
*			None
* OUTPUTS :
*       PARAMETERS:
*			None			
*
*       GLOBALS :
*			None
*
*       RETURN :
*            Type:  int				Socket descryptor on success
*							-1 if operation failed
*********************************************************************************/
int SecureLayerConnect(char*host,int port){
	if (g_Tsock)
		close(g_Tsock);
	
	if((g_Tsock=socket(AF_INET,SOCK_STREAM,	IPPROTO_TCP))<0){
				printf("Couldn't create socket\r\n");
				return -1;
		}
		i=1;
		if(0>setsockopt(g_Tsock, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)))
		{
				perror("SockReuseAddr:\r\n");
				return -1;
		}
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&sock_set);
		FD_SET(g_Tsock, &sock_set);
		ret = select (FD_SETSIZE, &sock_set, NULL, NULL, &tv);
		if(ret==-1){
				perror("SSL Select sock: connect:");
				return -1;
		}
		else if(ret==0){
				printf("Sock connect :Time out \r\n");
				return -1;
		}


		if(connect(g_Tsock,(struct sockaddr *)&addr,sizeof(addr))<0){
				printf("Couldn't connect socket\r\n");
				return -1;
		}

		g_ssl=SSL_new(g_ctx);
		g_sbio=BIO_new_socket(g_Tsock,BIO_NOCLOSE);
		SSL_set_bio(g_ssl,g_sbio,g_sbio);
		if(SSL_connect(g_ssl)<=0){
				printf("SSL connect error\r\n");
				return -1;
		}
		return g_Tsock;
}

/*********************************************************************************
* NAME :          int cloud_inf_init(int type, char *server_ip, struct sockaddr_in *serveraddr,sslKeys_t **keys)
*
* DESCRIPTION :     initialise the server details for communication over SSL.
*
* INPUTS :
*       PARAMETERS:
*			int type, char *server_ip
*			struct sockaddr_in *serveraddr
*			sslKeys_t **keys
*			
*       GLOBALS :
*			None
* OUTPUTS :
*       PARAMETERS:
*			None			
*
*       GLOBALS :
*			None
*
*       RETURN :
*            Type:  int				  0 on success
*							-1 if operation failed
*********************************************************************************/
int cloud_inf_init(int type, char *server_ip, struct sockaddr_in *serveraddr,
		int k) {
			if(-1==SecureLayerConnect(server_ip,443))
			{
				g_ssl=NULL;
				g_Tsock=0;
				printf("SSL connect Failed\r\n");
				return -1;
			}
			else{
				printf("SSL connect Success\r\n");
				return 0;
			}
	return 0;
}
#if 0
char Req[]="GET https://dheauto.com/dhericknew/user/create_request?cat=3&DevId=77777&mob_number=9387320500&latitude=9.97182510&longitude=76.31519740 \r\n\r\n\r\n\r\n";
char recvbuf[1024];
char HTTPS_IP[]="dheauto.com";
void main(){
			SecLayerInit();
								if (cloud_inf_init(0,HTTPS_IP, &cloudaddr,NULL) == -1)
								{
																printf("Cloud info init error\n");
																sleep (1);

								}
		if (httpsGet(0, Req, recvbuf, 1024) < 1) {
															return;
							}					
}
#endif
