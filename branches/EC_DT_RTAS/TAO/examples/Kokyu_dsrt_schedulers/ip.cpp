//$Id$
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "ace/Log_Msg.h"

int get_ip_addr()
{
    char ac[80];
    char *valueptr;
    const char searchchar='.';
    if (gethostname(ac, sizeof(ac)) == -1) {
        ACE_ERROR((LM_ERROR,"Can not get host name\n"));;
        return -1;
    }

    struct hostent *phe = gethostbyname(ac);
    if (phe == 0) {
        ACE_ERROR((LM_ERROR,"Can not find the host by its name\n"));
        return -1;
    }

    struct in_addr addr;
    memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
    valueptr = strrchr(inet_ntoa(addr), searchchar);
    int lipis = atoi(valueptr+1);
#ifdef KOKYU_DSRT_LOGGING
    ACE_DEBUG((LM_DEBUG,"last ip is %d\n",lipis));
#endif    
    return lipis;
}

