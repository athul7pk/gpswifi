#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <termios.h>
#include <semaphore.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <openssl/ssl.h>
#ifndef ALLOW_OLD_VERSIONS
#if (OPENSSL_VERSION_NUMBER < 0x00905100L)
#error "Must use OpenSSL 0.9.6 or later"
#endif
#endif

#include "Include/lcd.h"
#include "Include/nvram.h"
#include "Include/gps.h"
#include "Include/kb.h"
#include "Include/kios.h"

