/* $Id$ */

/* Copyright (c) 1995 Oregon Graduate Institute of Science and Technology
 * P.O.Box 91000-1000, Portland, OR 97291, USA;
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its 
 * documentation for any purpose is hereby granted without fee, provided that 
 * the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of O.G.I. not be used in advertising or 
 * publicity pertaining to distribution of the software without specific, 
 * written prior permission.  O.G.I. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * O.G.I. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL 
 * O.G.I. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF 
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Shanwei Cen
 *         Department of Computer Science and Engineering
 *         email: scen@cse.ogi.edu
 */

// #include <stdio.h>   
// #include <errno.h>   
// #include <stdlib.h>  
// #include <sys/types.h>
// #include <unistd.h>  
// #include <string.h>  
// #include <netdb.h>   
// #include <ctype.h>   
// #include <sys/select.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netinet/tcp.h>
// #include <arpa/inet.h>
// #include <sys/un.h>  
// #include <time.h>    
// #include <sys/time.h>
#include "include/common.h"
#include "fileio.h"
#include "routine.h"
#include "com.h"
#include "ace/OS.h"

#ifdef NATIVE_ATM
#include "atmcom.h"
#endif

ACE_RCSID(mpeg_shared, com, "$Id$")

#define CONN_INET	1
#define CONN_UNIX	2
#define CONN_ATM	3

#define SERVER_FDTABLE_SIZE	50
#define CLIENT_FDTABLE_SIZE	10

#define STATE_PENDING	1
#define STATE_CONTROL	2
#define STATE_DATA	3
#define STATE_SINGLE	4

static int inet_port;
static char * unix_port, * atm_port;

static struct FdTable
{
  int fd;   /* -1 -- free item, >=0 -- fd value */
  int type;
  unsigned stime;  /* setup time, in seconds */
  int state;
} * fdTable = NULL;
static int size = 0;

int ComInitClient(int pinet_port, char * punix_port, char * patm_port)
{
  int i;
  
  inet_port = pinet_port;
  unix_port = punix_port;
  atm_port = patm_port;
  if (size) {
    fprintf(stderr,
	    "Error ComInitClient: pid %d already done ComInitClient/Server\n",
	   ACE_OS::getpid ());
    return -1;
  }
  size = CLIENT_FDTABLE_SIZE;
  fdTable = (struct FdTable *)ACE_OS::malloc(size * sizeof(*fdTable));
  if (fdTable == NULL) {
    fprintf(stderr,
	    "Error ComInitClient: pid %d failed to allocated fdTable space:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    return -1;
  }
  for (i = 0; i < size; i ++) {
    fdTable[i].fd = -1;
  }
#ifdef NATIVE_ATM
  return ATMinitClient(atm_port);
#else
  return 0;
#endif
}

void ComCloseClient(void)
{
  int i;
  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd >= 0) ComCloseConn(fdTable[i].fd);
  }
#ifdef NATIVE_ATM
  ATMcloseClient();
#endif
  return;
}

int ComCloseFd(int fd)
{
  int i;
  if (!size) {
    fprintf(stderr,
	    "Error ComCloseFd: pid %d not done ComInitClient/Server yet.\n",
	   ACE_OS::getpid ());
    return -1;
  }
  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd == fd) break;
  }
  if (i < size) {
    fdTable[i].fd = -1;
    if (fdTable[i].type == CONN_ATM) {
#ifdef NATIVE_ATM
      return ATMcloseFd(fd);
#else
      fprintf(stderr,
	      "Error ComCloseFd: pid %d CONN_ATM not supported.\n",
	     ACE_OS::getpid ());
      return -1;
#endif
    }
    else {
      ACE_OS::close (fd);
    }
    return 0;
  }
  else {
    fprintf(stderr,
	    "Error ComCloseFd: pid %d can't find fd %d in table\n",
	   ACE_OS::getpid (), fd);
    return -1;
  }
}

int ComCloseConn(int fd)
{
  int i;
  if (!size) {
    fprintf(stderr,
	    "Error ComCloseConn: pid %d not done ComInitClient/Server yet.\n",
	   ACE_OS::getpid ());
    return -1;
  }
  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd == fd) break;
  }
  if (i < size) {
    fdTable[i].fd = -1;
    if (fdTable[i].type == CONN_ATM) {
#ifdef NATIVE_ATM
      return ATMcloseConn(fd);
#else
      fprintf(stderr,
	      "Error ComCloseConn: pid %d CONN_ATM not supported.\n",
	     ACE_OS::getpid ());
      return -1;
#endif
    }
    else {
      ACE_OS::close (fd);
    }
    return 0;
  }
  else {
    fprintf(stderr,
	    "Error ComCloseConn: pid %d can't find fd %d in table\n",
	   ACE_OS::getpid (), fd);
    return -1;
  }
}

int ComOpenConnPair(char * address, int *ctr_fd, int *data_fd, int *max_pkt_size)
{

  // This is right now called only by the audio client.
  // The protocol is slightly modified as after connecting we need to
  // inform that we are a audio client so that it can fork a process.
  int i;
  int cfd, dfd, csocktype, dsocktype;
  int conn_tag = !(*max_pkt_size);
  struct sockaddr_in addressIn;
  struct sockaddr_un addressUn;
  
  if (!size) {
    fprintf(stderr,
	    "Error ComOpenConnPair: pid %d not done ComInitClient/Server yet.\n",
	   ACE_OS::getpid ());
    return -1;
  }

  cfd = 0;
  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd == -1) cfd ++;
  }
  if (cfd < 2) {
    fprintf(stderr,
	    "Error ComOpenConnPair: pid %d no faTable entry for the pair",
	   ACE_OS::getpid ());
    return -1;
  }
  
  /* tell the type of connections to establish:
     If the server is on the same host, then UNIX socket is used */
  if (address[0] == 0) {
    csocktype = CONN_UNIX;
    dsocktype = CONN_UNIX;
  }
  else if (!strncasecmp(address, "atm:", 4)) {
    address += 4;
    csocktype = CONN_ATM;
    dsocktype = CONN_ATM;
  }
  else if (!strncasecmp(address, "datm:", 5)) {
    address += 5;
    csocktype = CONN_INET;
    dsocktype = CONN_ATM;
  }
  else {
    csocktype = CONN_INET;
    dsocktype = CONN_INET;
  }
  if (csocktype == CONN_INET || dsocktype == CONN_INET) {
    struct hostent *hp;
    char hostname[100];
    addressIn.sin_family = AF_INET;
    if (!strncasecmp(address, "inet:", 5)) {
      address += 5;
    }
    if (isascii(address[0]) && isdigit(address[0]))
      addressIn.sin_addr.s_addr = inet_addr(address);
    else {
      if ((hp = gethostbyname(address)) == NULL) { /* No such host! */
        fprintf(stderr,
		"Error ComOpenConnPair: pid %d host %s can't be found:",
		getpid(), address);
	perror("");
        return -1;
      }
      if (hp->h_addrtype != AF_INET) {
        fprintf(stderr,
		"Error ComOpenConnPair: pid %d host %s is not of INET type address.\n",
		getpid(), address);
        return -1;
      }
      ACE_OS::memcpy ((char *)&addressIn.sin_addr.s_addr, (char *)hp->h_addr,
	    sizeof(addressIn.sin_addr.s_addr));
      /*
      bcopy((char *)hp->h_addr, (char *)&addressIn.sin_addr.s_addr,
	    sizeof(addressIn.sin_addr.s_addr));
      */
    }
    if (get_hostname(hostname, 100)) {
      fprintf(stderr, "Error ComOpenConnPair: pid %d failed to get_hostname:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    if ((hp = gethostbyname(hostname)) == NULL) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to its own IP address:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    if (hp->h_addrtype != AF_INET) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d own address is not INET type",
	     ACE_OS::getpid ());
      return -1;
    }
    
    if (!memcmp((char *)hp->h_addr, (char *)&addressIn.sin_addr.s_addr,
	        sizeof(addressIn.sin_addr.s_addr))) {
      if (csocktype == CONN_INET) csocktype = CONN_UNIX;
      if (dsocktype == CONN_INET) dsocktype = CONN_UNIX;
    }
  }

  /* build appropriate type of Connections to Server */
  if (csocktype == CONN_UNIX && dsocktype == CONN_UNIX) {
    /* build UNIX connection to server */
    addressUn.sun_family = AF_UNIX;
    strncpy(addressUn.sun_path, unix_port,
	    sizeof(struct sockaddr_un) - sizeof(short));
    cfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (cfd == -1) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to open UNIX cfd:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    usleep(10000); /* this is for waiting for the uncaught signal mentioned
		      below */
    if (connect(cfd, (struct sockaddr *)&addressUn, sizeof(addressUn)) == -1) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to conn UNIX cfd to server:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      i = -3;  /* I don't understand why when connect() returns EINTR, the connection
		  is still built, and the other end can still get what written at
		  this end, 'i = -3' is a garbage to be written. */
      ACE_OS::write (cfd, &i, 4);
      ACE_OS::close (cfd);
      return -1;
    }
    if (time_write_int(cfd, -1) == -1 || time_read_int(cfd, &i) == -1) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to write -1 to UNIX cfd:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ACE_OS::close (cfd);
      return -1;
    }
    
    dfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (dfd == -1) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to open UNIX dfd:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ACE_OS::close (cfd);
      return -1;
    }
    usleep(10000);
    if (connect(dfd, (struct sockaddr *)&addressUn, sizeof(addressUn)) == -1) {
       fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to conn UNIX dfd to server:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      i = -3;
      ACE_OS::write (dfd, &i, 4);
      ACE_OS::close (cfd);
      ACE_OS::close (dfd);
      return -1;
    }
    if (time_write_int(dfd, i) == -1) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to write (cfd) to UNIX dfd:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ACE_OS::close (cfd);
      ACE_OS::close (dfd);
      return -1;
    }
    *max_pkt_size = 0;
  }
  else if (csocktype == CONN_INET && dsocktype == CONN_INET)  {
    /* build TCP connection to server */
    addressIn.sin_port = htons(inet_port);
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to open TCP cfd:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    usleep(10000);
    if (connect(cfd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
       fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to conn TCP cfd to server:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      i = -3;
      ACE_OS::write (cfd, &i, 4);
      ACE_OS::close (cfd);
      return -1;
    }
    if (conn_tag) { /* create a second connection with server */
      if (time_write_int(cfd, -1) == -1 || time_read_int(cfd, &i) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to write -1 to TCP cfd:",
		getpid());
	perror("");
	close(cfd);
	return -1;
      }
      dfd = socket(AF_INET, SOCK_STREAM, 0);
      if (dfd == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to open TCP dfd:",
		getpid());
	perror("");
	close(cfd);
	return -1;
      }
      usleep(10000);
      if (connect(dfd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
 	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to conn TCP dfd to server:",
		getpid());
	perror("");
	i = -3;
	write(dfd, &i, 4);
	close(cfd);
	close(dfd);
	return -1;
      }
      if (time_write_int(dfd, i) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to write (cfd) to TCP dfd:",
		getpid());
	perror("");
	close(cfd);
	close(dfd);
	return -1;
      }
      *max_pkt_size = 0;
    }
    else {  /* create a UDP socket for data */
      // Write a byte saying that we are a audio client.
      
      if (time_write_int(cfd,CmdINITaudio) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to write -2 to TCP cfd:",
		getpid());
	perror("");
	close(cfd);
	close(dfd);
	return -1;
      }

      dfd = socket(AF_INET, SOCK_DGRAM, 0);
      if (dfd == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to open UDP dfd:",
		getpid());
	perror("");
	close(cfd);
	return -1;
      }
      memset((char *)&addressIn, 0, sizeof(addressIn));

      i = sizeof(addressIn);
      if (getsockname(cfd, (struct sockaddr *)&addressIn, &i) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to getsocketname on TCP cfd:",
		getpid());
	perror("");
	close(dfd);
	close(cfd);
	return -1;
      }
      
      addressIn.sin_family = AF_INET;
      addressIn.sin_port = 0;
      if (bind(dfd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to bind UDP dfd:",
		getpid());
	perror("");
	close(cfd);
	close(dfd);
	return -1;
      }
      if (time_write_int(cfd, -2) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to write -2 to TCP cfd:",
		getpid());
	perror("");
	close(cfd);
	close(dfd);
	return -1;
      }
      
      i = sizeof(addressIn);
      if (getsockname(dfd, (struct sockaddr *)&addressIn, &i) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to getsocketname on UDP fd:",
		getpid());
	perror("");
	close(dfd);
	close(cfd);
	return -1;
      }
      if (time_write_bytes(cfd, (char *)&addressIn.sin_port, sizeof(short)) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to write (port) to TCP cfd:",
		getpid());
	perror("");
	close(dfd);
	close(cfd);
	return -1;
      }
      
      fprintf(stderr, "ComOpenConnPair local UDP socket: addr - %s,%d, port - %u.\n",
	      inet_ntoa(addressIn.sin_addr), addressIn.sin_addr.s_addr,ntohs(addressIn.sin_port));
      
      
      fprintf(stderr, "ComOpenConnPair UDP port %d (should be > 0)\n",
	      ntohs(addressIn.sin_port));
      
      
      i = sizeof(addressIn);
      if (getsockname(cfd, (struct sockaddr *)&addressIn, &i) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to getsocketname on TCP cfd:",
		getpid());
	perror("");
	close(dfd);
	close(cfd);
	return -1;
      }
      
      if (time_write_bytes(cfd, (char *)&addressIn.sin_addr.s_addr, sizeof(int)) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to write (IP addr) to TCP cfd:",
		getpid());
	perror("");
	close(dfd);
	close(cfd);
	return -1;
      }
      
      fprintf(stderr, "ComOpenConnPair TCP cfd socket: addr - %s, port - %u.\n",
	      inet_ntoa(addressIn.sin_addr), ntohs(addressIn.sin_port));
      
      if (time_read_bytes(cfd, (char *)&addressIn.sin_port, sizeof(short)) == -1 ||
	  time_read_bytes(cfd, (char *)&addressIn.sin_addr.s_addr, sizeof(int)) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to read (IP addr/port) from TCP cfd:",
		getpid());
	perror("");
	close(dfd);
	close(cfd);
	return -1;
      }
      usleep(10000);
      if (connect(dfd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed conn UDP dfd to peer:",
		getpid());
	perror("");
	close(cfd);
	close(dfd);
	return -1;
      }
      
      fprintf(stderr, "ComOpenConnPair UDP dfd connects to host-%s (port %u)\n",
  	      inet_ntoa(addressIn.sin_addr),
	      ntohs(addressIn.sin_port));
      
#ifndef LINUX
      /* adjust the dfd REVBUF size */
      {
	int len;
	len = INET_SOCKET_BUFFER_SIZE + 100;
	/*
	if (setsockopt(dfd, SOL_SOCKET, SO_SNDBUF, (char *)&len, sizeof(len)) == -1) {
	  fprintf(stderr,
		  "Warning ComOpenConnPair: pid %d failed to set UDP dfd-snd to size %d:",
		 ACE_OS::getpid (), len);
	 ACE_OS::perror ("");
	  ACE_OS::close (dfd);
	  ACE_OS::close (cfd);
	  return -1;
	}
	*/
	len = 65536;
	while (setsockopt(dfd, SOL_SOCKET, SO_RCVBUF, (char *)&len, sizeof(len)) == -1) {
	  len -= 1024;
	  if (len < 8192) break;
	  /*
	  fprintf(stderr,
		  "Warning ComOpenConnPair: pid %d failed to set UDP dfd-rcv to size %d:",
		 ACE_OS::getpid (), len);
	 ACE_OS::perror ("");
	  ACE_OS::close (dfd);
	  ACE_OS::close (cfd);
	  return -1;
	  */
	}
	if (getuid() == DEVELOPER_UID)
	fprintf(stderr,	"Set UDP dfd-rcv to %dB\n", len);
      }
#endif
      *max_pkt_size = - INET_SOCKET_BUFFER_SIZE;
                        /* UDP sockets on HP and SUN are discard mode */
    }
    if (setsockopt(cfd, IPPROTO_TCP, TCP_NODELAY,
		   (const char *)&i, sizeof(i)) == -1) {
      fprintf(stderr,
    "Error ComOpenConnPair: pid %d failed to setsockopt on TCP cfd with TCP_NODELAY:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ACE_OS::close (dfd);
      ACE_OS::close (cfd);
      return -1;
    }
  }
  else if (dsocktype == CONN_ATM) { /* CONN_ATM */
#ifdef NATIVE_ATM
    if (csocktype == CONN_INET) {
      /* build TCP connection to server */
      addressIn.sin_port = htons(inet_port);
      cfd = socket(AF_INET, SOCK_STREAM, 0);
      if (cfd == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to open TCP1 cfd:",
		getpid());
	perror("");
	return -1;
      }
      usleep(10000);
      if (connect(cfd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
	 fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to conn TCP1 cfd to server:",
		getpid());
	perror("");
	i = -3;
	write(cfd, &i, 4);
	close(cfd);
	return -1;
      }
    }
    else if (csocktype == CONN_UNIX) {
      /* build UNIX connection to server */
      addressUn.sun_family = AF_UNIX;
      strncpy(addressUn.sun_path, unix_port,
	      sizeof(struct sockaddr_un) - sizeof(short));
      cfd = socket(AF_UNIX, SOCK_STREAM, 0);
      if (cfd == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to open UNIX1 cfd:",
		getpid());
	perror("");
	return -1;
      }
      usleep(10000); /* this is for waiting for the uncaught signal mentioned
			below */
      if (connect(cfd, (struct sockaddr *)&addressUn, sizeof(addressUn)) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to conn UNIX1 cfd to server:",
		getpid());
	perror("");
	i = -3;  /* I don't understand why when select() returns EINTR, the connection
		    is still built, and the other end can still get what written at
		    this end, 'i = -3' is a garbage to be written. */
	write(cfd, &i, 4);
	close(cfd);
	return -1;
      }
    }
    else {  /* CONN_ATM */
      cfd = ATMopenConn(address, max_pkt_size);
      if (cfd == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to open ATM cfd conn:",
		getpid());
	perror("");
	return -1;
      }
      usleep(100000);  /* be nice to buggy ATM driver */
    }
    if (time_write_int(cfd, -1) == -1 || time_read_int(cfd, &i) == -1) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to write -1 to %s cfd:",
	     ACE_OS::getpid (),
	      (csocktype == CONN_UNIX) ? "UNIX1" :
	      (csocktype == CONN_INET) ? "INET1" : "ATM");
     ACE_OS::perror ("");
      if (csocktype == CONN_ATM) ATMcloseConn(cfd);
      else ACE_OS::close (cfd);
      return -1;
    }

    dfd = ATMopenConn(address, max_pkt_size);
    if (dfd == -1) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to open ATM dfd conn:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ATMcloseConn(cfd);
      return -1;
    }
    usleep(100000); /* be nice to buggy ATM driver */
    if (time_write_int(dfd, i) == -1) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to write (cfd) to ATM dfd:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ATMcloseConn(cfd);
      ATMcloseConn(dfd);
      return -1;
    }
#else
    fprintf(stderr,
	    "Error ComOpenConnPair: pid %d addresstype %d not supported\n",
	   ACE_OS::getpid (), dsocktype);
    return -1;
#endif
  }
  for (i = 0; i < size; i ++) {  /* guaranteed to succeed */
    if (fdTable[i].fd == -1) break;
  }
  if (i < size) {
    fdTable[i].fd = cfd;
    fdTable[i].type = csocktype;
    fdTable[i].stime = get_sec();
    fdTable[i].state = STATE_CONTROL;
  }
  else {
    fprintf(stderr, "<Weird>Error ComOpenConnPair: no entry for cfd %d\n", cfd);
    return -1;
  }
  for (; i < size; i ++) {
    if (fdTable[i].fd == -1) break;
  }
  if (i < size) {
    fdTable[i].fd = dfd;
    fdTable[i].type = dsocktype;
    fdTable[i].stime = get_sec();
    fdTable[i].state = STATE_DATA;
  }
  else {
    fprintf(stderr, "<Wierd>Error ComOpenConnPair: no entry for dfd %d\n", dfd);
    return -1;
  }
  *ctr_fd = cfd;
  *data_fd = dfd;
  ACE_DEBUG ((LM_DEBUG,"(%P|%t)Returning from ComOpenConnpair\n"));
  return 0;
}

/* address format: [ATM:|INET:]real_address | ""

   "" -- UNIX socket, reliable byte stream
   [INET:]real_address -- INET sockets, reliable control byte stream, and
                          *max_pkt_size == 0 : reliable data byte stream, or
			                != 0 : unreliable packet stream;
   ATM:real_address -- ATM sockets, both data and control are
                       unreliable packet streams;
   DATM:real_address -- INET/UNIX control reliable byte stream
                        and ATM unreliable byte stream;
			
   Communication between client and server:

   ctr_fd:   client --> server	    server --> client
             -2: conn-less INET	   Open udp sock, send addr in net byte order
	         <port, addr>                     <port(short), addr(long)>
	     -1: conn-orient	   Send value of fd, wait to be paired
	     cfd>=0: data sock     Paired with cfd and return.
   */

#if 0
int VideoComOpenConnPair (char * address, 
                          int *ctr_fd, 
                          int *data_fd, 
                          int *max_pkt_size)
{
  int i;
  int cfd, dfd, csocktype, dsocktype;
  int conn_tag = !(*max_pkt_size);
  struct sockaddr_in addressIn;
  struct sockaddr_un addressUn;
  
  ACE_DEBUG ((LM_DEBUG,
              "%s %d\n",
              __FILE__,__LINE__));

  if (!size) {
    fprintf(stderr,
	    "Error ComOpenConnPair: pid %d not done ComInitClient/Server yet.\n",
	   ACE_OS::getpid ());
    return -1;
  }

  cfd = 0;
  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd == -1) cfd ++;
  }
  if (cfd < 2) {
    fprintf(stderr,
	    "Error ComOpenConnPair: pid %d no faTable entry for the pair",
	   ACE_OS::getpid ());
    return -1;
  }
  
  /* tell the type of connections to establish:
     If the server is on the same host, then UNIX socket is used */
  if (address[0] == 0) {
    csocktype = CONN_UNIX;
    dsocktype = CONN_UNIX;
  }
  else if (!strncasecmp(address, "atm:", 4)) {
    address += 4;
    csocktype = CONN_ATM;
    dsocktype = CONN_ATM;
  }
  else if (!strncasecmp(address, "datm:", 5)) {
    address += 5;
    csocktype = CONN_INET;
    dsocktype = CONN_ATM;
  }
  else {
    csocktype = CONN_INET;
    dsocktype = CONN_INET;
  }
  if (csocktype == CONN_INET || dsocktype == CONN_INET) {
    struct hostent *hp;
    char hostname[100];
    addressIn.sin_family = AF_INET;
    if (!strncasecmp(address, "inet:", 5)) {
      address += 5;
    }
    if (isascii(address[0]) && isdigit(address[0]))
      addressIn.sin_addr.s_addr = inet_addr(address);
    else {
      if ((hp = gethostbyname(address)) == NULL) { /* No such host! */
        fprintf(stderr,
		"Error ComOpenConnPair: pid %d host %s can't be found:",
		getpid(), address);
	perror("");
        return -1;
      }
      if (hp->h_addrtype != AF_INET) {
        fprintf(stderr,
		"Error ComOpenConnPair: pid %d host %s is not of INET type address.\n",
		getpid(), address);
        return -1;
      }
      ACE_OS::memcpy ((char *)&addressIn.sin_addr.s_addr, (char *)hp->h_addr,
	    sizeof(addressIn.sin_addr.s_addr));
      /*
      bcopy((char *)hp->h_addr, (char *)&addressIn.sin_addr.s_addr,
	    sizeof(addressIn.sin_addr.s_addr));
      */
    }
    if (get_hostname(hostname, 100)) {
      fprintf(stderr, "Error ComOpenConnPair: pid %d failed to get_hostname:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    if ((hp = gethostbyname(hostname)) == NULL) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to its own IP address:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    if (hp->h_addrtype != AF_INET) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d own address is not INET type",
	     ACE_OS::getpid ());
      return -1;
    }
    
    if (!memcmp((char *)hp->h_addr, (char *)&addressIn.sin_addr.s_addr,
	        sizeof(addressIn.sin_addr.s_addr))) {
      if (csocktype == CONN_INET) csocktype = CONN_UNIX;
      if (dsocktype == CONN_INET) dsocktype = CONN_UNIX;
    }
  }

  /* build appropriate type of Connections to Server */
 
  if (csocktype == CONN_INET && dsocktype == CONN_INET)  {
    /* build TCP connection to server */
    addressIn.sin_port = htons(inet_port);
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1) {
      fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to open TCP cfd:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    usleep(10000);
    if (connect(cfd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
       fprintf(stderr,
	      "Error ComOpenConnPair: pid %d failed to conn TCP cfd to server:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      i = -3;
      ACE_OS::write (cfd, &i, 4);
      ACE_OS::close (cfd);
      return -1;
    }
//     if (conn_tag) { /* create a second connection with server */
//       if (time_write_int(cfd, -1) == -1 || time_read_int(cfd, &i) == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to write -1 to TCP cfd:",
// 		getpid());
// 	perror("");
// 	close(cfd);
// 	return -1;
//       }
      dfd = socket(AF_INET, SOCK_STREAM, 0);
      if (dfd == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to open TCP dfd:",
		getpid());
	perror("");
	close(cfd);
	return -1;
      }
      usleep(10000);
      if (connect(dfd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
 	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to conn TCP dfd to server:",
		getpid());
	perror("");
	i = -3;
	write(dfd, &i, 4);
	close(cfd);
	close(dfd);
	return -1;
      }
      if (time_write_int(dfd, i) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to write (cfd) to TCP dfd:",
		getpid());
	perror("");
	close(cfd);
	close(dfd);
	return -1;
      }
      *max_pkt_size = 0;
    }
    else {  /* create a UDP socket for data */
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) ::VideoComOpenConnPair: making UDP connection\n"));
      dfd = socket(AF_INET, SOCK_DGRAM, 0);
      if (dfd == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to open UDP dfd:",
		getpid());
	perror("");
	close(cfd);
	return -1;
      }
      memset((char *)&addressIn, 0, sizeof(addressIn));

      i = sizeof(addressIn);
      if (getsockname(cfd, (struct sockaddr *)&addressIn, &i) == -1) {
	fprintf(stderr,
		"Error ComOpenConnPair: pid %d failed to getsocketname on TCP cfd:",
		getpid());
	perror("");
	close(dfd);
	close(cfd);
	return -1;
      }
      
//       addressIn.sin_family = AF_INET;
//       addressIn.sin_port = 0;
//       if (bind(dfd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to bind UDP dfd:",
// 		getpid());
// 	perror("");
// 	close(cfd);
// 	close(dfd);
// 	return -1;
//       }
//       if (time_write_int(cfd, -2) == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to write -2 to TCP cfd:",
// 		getpid());
// 	perror("");
// 	close(cfd);
// 	close(dfd);
// 	return -1;
//       }
      
//       i = sizeof(addressIn);
//       if (getsockname(dfd, (struct sockaddr *)&addressIn, &i) == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to getsocketname on UDP fd:",
// 		getpid());
// 	perror("");
// 	close(dfd);
// 	close(cfd);
// 	return -1;
//       }
//       if (time_write_bytes(cfd, (char *)&addressIn.sin_port, sizeof(short)) == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to write (port) to TCP cfd:",
// 		getpid());
// 	perror("");
// 	close(dfd);
// 	close(cfd);
// 	return -1;
//       }
      
//       fprintf(stderr, "ComOpenConnPair local UDP socket: addr - %s, port - %u.\n",
// 	      inet_ntoa(addressIn.sin_addr), ntohs(addressIn.sin_port));
      
      
//       fprintf(stderr, "ComOpenConnPair UDP port %d (should be > 0)\n",
// 	      ntohs(addressIn.sin_port));
      
//       /*
//       i = sizeof(addressIn);
//       if (getsockname(cfd, (struct sockaddr *)&addressIn, &i) == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to getsocketname on TCP cfd:",
// 		getpid());
// 	perror("");
// 	close(dfd);
// 	close(cfd);
// 	return -1;
//       }
//       */
//       if (time_write_bytes(cfd, (char *)&addressIn.sin_addr.s_addr, sizeof(int)) == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to write (IP addr) to TCP cfd:",
// 		getpid());
// 	perror("");
// 	close(dfd);
// 	close(cfd);
// 	return -1;
//       }
      
//       fprintf(stderr, "ComOpenConnPair TCP cfd socket: addr - %s, port - %u.\n",
// 	      inet_ntoa(addressIn.sin_addr), ntohs(addressIn.sin_port));
      
//       if (time_read_bytes(cfd, (char *)&addressIn.sin_port, sizeof(short)) == -1 ||
// 	  time_read_bytes(cfd, (char *)&addressIn.sin_addr.s_addr, sizeof(int)) == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to read (IP addr/port) from TCP cfd:",
// 		getpid());
// 	perror("");
// 	close(dfd);
// 	close(cfd);
      // 	return -1;
      //       }
      //      usleep(10000);
      //            if (connect(dfd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
      //	fprintf(stderr,
      //		"Error ComOpenConnPair: pid %d failed conn UDP dfd to peer:",
      //		getpid());
      //	perror("");
      //	close(cfd);
      //	close(dfd);
      //	return -1;
      //      }
      
      fprintf(stderr, "ComOpenConnPair UDP dfd connects to host-%s (port %u)\n",
  	      inet_ntoa(addressIn.sin_addr),
	      ntohs(addressIn.sin_port));
      
#ifndef LINUX
      /* adjust the dfd REVBUF size */
      {
	int len;
	len = INET_SOCKET_BUFFER_SIZE + 100;
	/*
	if (setsockopt(dfd, SOL_SOCKET, SO_SNDBUF, (char *)&len, sizeof(len)) == -1) {
	  fprintf(stderr,
		  "Warning ComOpenConnPair: pid %d failed to set UDP dfd-snd to size %d:",
		 ACE_OS::getpid (), len);
	 ACE_OS::perror ("");
	  ACE_OS::close (dfd);
	  ACE_OS::close (cfd);
	  return -1;
	}
	*/
	len = 65536;
	while (setsockopt(dfd, SOL_SOCKET, SO_RCVBUF, (char *)&len, sizeof(len)) == -1) {
	  len -= 1024;
	  if (len < 8192) break;
	  /*
	  fprintf(stderr,
		  "Warning ComOpenConnPair: pid %d failed to set UDP dfd-rcv to size %d:",
		 ACE_OS::getpid (), len);
	 ACE_OS::perror ("");
	  ACE_OS::close (dfd);
	  ACE_OS::close (cfd);
	  return -1;
	  */
	}
	if (getuid() == DEVELOPER_UID)
	fprintf(stderr,	"Set UDP dfd-rcv to %dB\n", len);
      }
#endif
      *max_pkt_size = - INET_SOCKET_BUFFER_SIZE;
                        /* UDP sockets on HP and SUN are discard mode */
    }
    if (setsockopt(cfd, IPPROTO_TCP, TCP_NODELAY,
		   (const char *)&i, sizeof(i)) == -1) {
      fprintf(stderr,
    "Error ComOpenConnPair: pid %d failed to setsockopt on TCP cfd with TCP_NODELAY:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ACE_OS::close (dfd);
      ACE_OS::close (cfd);
      return -1;
    }
    //  }
//   else if (dsocktype == CONN_ATM) { /* CONN_ATM */
// #ifdef NATIVE_ATM
//     if (csocktype == CONN_INET) {
//       /* build TCP connection to server */
//       addressIn.sin_port = htons(inet_port);
//       cfd = socket(AF_INET, SOCK_STREAM, 0);
//       if (cfd == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to open TCP1 cfd:",
// 		getpid());
// 	perror("");
// 	return -1;
//       }
//       usleep(10000);
//       if (connect(cfd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
// 	 fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to conn TCP1 cfd to server:",
// 		getpid());
// 	perror("");
// 	i = -3;
// 	write(cfd, &i, 4);
// 	close(cfd);
// 	return -1;
//       }
//     }
//     else if (csocktype == CONN_UNIX) {
//       /* build UNIX connection to server */
//       addressUn.sun_family = AF_UNIX;
//       strncpy(addressUn.sun_path, unix_port,
// 	      sizeof(struct sockaddr_un) - sizeof(short));
//       cfd = socket(AF_UNIX, SOCK_STREAM, 0);
//       if (cfd == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to open UNIX1 cfd:",
// 		getpid());
// 	perror("");
// 	return -1;
//       }
//       usleep(10000); /* this is for waiting for the uncaught signal mentioned
// 			below */
//       if (connect(cfd, (struct sockaddr *)&addressUn, sizeof(addressUn)) == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to conn UNIX1 cfd to server:",
// 		getpid());
// 	perror("");
// 	i = -3;  /* I don't understand why when select() returns EINTR, the connection
// 		    is still built, and the other end can still get what written at
// 		    this end, 'i = -3' is a garbage to be written. */
// 	write(cfd, &i, 4);
// 	close(cfd);
// 	return -1;
//       }
//     }
//     else {  /* CONN_ATM */
//       cfd = ATMopenConn(address, max_pkt_size);
//       if (cfd == -1) {
// 	fprintf(stderr,
// 		"Error ComOpenConnPair: pid %d failed to open ATM cfd conn:",
// 		getpid());
// 	perror("");
// 	return -1;
//       }
//       usleep(100000);  /* be nice to buggy ATM driver */
//     }
//     if (time_write_int(cfd, -1) == -1 || time_read_int(cfd, &i) == -1) {
//       fprintf(stderr,
// 	      "Error ComOpenConnPair: pid %d failed to write -1 to %s cfd:",
// 	     ACE_OS::getpid (),
// 	      (csocktype == CONN_UNIX) ? "UNIX1" :
// 	      (csocktype == CONN_INET) ? "INET1" : "ATM");
//      ACE_OS::perror ("");
//       if (csocktype == CONN_ATM) ATMcloseConn(cfd);
//       else ACE_OS::close (cfd);
//       return -1;
//     }

//     dfd = ATMopenConn(address, max_pkt_size);
//     if (dfd == -1) {
//       fprintf(stderr,
// 	      "Error ComOpenConnPair: pid %d failed to open ATM dfd conn:",
// 	     ACE_OS::getpid ());
//      ACE_OS::perror ("");
//       ATMcloseConn(cfd);
//       return -1;
//     }
//     usleep(100000); /* be nice to buggy ATM driver */
//     if (time_write_int(dfd, i) == -1) {
//       fprintf(stderr,
// 	      "Error ComOpenConnPair: pid %d failed to write (cfd) to ATM dfd:",
// 	     ACE_OS::getpid ());
//      ACE_OS::perror ("");
//       ATMcloseConn(cfd);
//       ATMcloseConn(dfd);
//       return -1;
//     }
// #else
//     fprintf(stderr,
// 	    "Error ComOpenConnPair: pid %d addresstype %d not supported\n",
// 	   ACE_OS::getpid (), dsocktype);
//     return -1;
// #endif
//   }
  for (i = 0; i < size; i ++) {  /* guaranteed to succeed */
    if (fdTable[i].fd == -1) break;
  }
  if (i < size) {
    fdTable[i].fd = cfd;
    fdTable[i].type = csocktype;
    fdTable[i].stime = get_sec();
    fdTable[i].state = STATE_CONTROL;
  }
  else {
    fprintf(stderr, "<Weird>Error ComOpenConnPair: no entry for cfd %d\n", cfd);
    return -1;
  }
  for (; i < size; i ++) {
    if (fdTable[i].fd == -1) break;
  }
  if (i < size) {
    fdTable[i].fd = dfd;
    fdTable[i].type = dsocktype;
    fdTable[i].stime = get_sec();
    fdTable[i].state = STATE_DATA;
  }
  else {
    fprintf(stderr, "<Wierd>Error ComOpenConnPair: no entry for dfd %d\n", dfd);
    return -1;
  }
  *ctr_fd = cfd;
  *data_fd = dfd;
  return 0;
}
#endif

/* returns: -1 -- failed,  >=0 -- fd */
int ComOpenConn(char * address, int *max_pkt_size)
{
  int i;
  int fd, socktype;
  struct sockaddr_in addressIn;
  struct sockaddr_un addressUn;
  
  if (!size) {
    fprintf(stderr,
	    "Error ComOpenConn: pid %d not done ComInitClient/Server yet.\n",
	   ACE_OS::getpid ());
    return -1;
  }

  fd = 0;
  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd == -1) fd ++;
  }
  if (fd < 1) {
    fprintf(stderr,
	    "Error ComOpenConn: pid %d no faTable entry for the pair",
	   ACE_OS::getpid ());
    return -1;
  }
  
  /* tell the type of connections to establish:
     If the server is on the same host, then UNIX socket is used */
  if (address[0] == 0) {
    socktype = CONN_UNIX;
  }
  else if (!strncasecmp(address, "atm:", 4)) {
    address += 4;
    socktype = CONN_ATM;
  }
  else if (!strncasecmp(address, "datm:", 5)) {
    address += 5;
    socktype = CONN_ATM;
  }
  else {
    socktype = CONN_INET;
  }
  if (socktype == CONN_INET) {
    struct hostent *hp;
    char hostname[100];
    addressIn.sin_family = AF_INET;
    if (!strncasecmp(address, "inet:", 5)) {
      address += 5;
    }
    if (isascii(address[0]) && isdigit(address[0]))
      addressIn.sin_addr.s_addr = inet_addr(address);
    else {
      if ((hp = gethostbyname(address)) == NULL) { /* No such host! */
        fprintf(stderr,
		"Error ComOpenConn: pid %d host %s can't be found:",
		getpid(), address);
	perror("");
        return -1;
      }
      if (hp->h_addrtype != AF_INET) {
        fprintf(stderr,
		"Error ComOpenConn: pid %d host %s is not of INET type address.\n",
		getpid(), address);
        return -1;
      }
      ACE_OS::memcpy ((char *)&addressIn.sin_addr.s_addr, (char *)hp->h_addr,
	    sizeof(addressIn.sin_addr.s_addr));
      /*
      bcopy((char *)hp->h_addr, (char *)&addressIn.sin_addr.s_addr,
	    sizeof(addressIn.sin_addr.s_addr));
      */
    }
    if (get_hostname(hostname, 100)) {
      fprintf(stderr, "Error ComOpenConn: pid %d failed to get_hostname:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    if ((hp = gethostbyname(hostname)) == NULL) {
      fprintf(stderr,
	      "Error ComOpenConn: pid %d failed to its own IP address:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    if (hp->h_addrtype != AF_INET) {
      fprintf(stderr,
	      "Error ComOpenConn: pid %d own address is not INET type",
	     ACE_OS::getpid ());
      return -1;
    }
    
    if (!memcmp((char *)hp->h_addr, (char *)&addressIn.sin_addr.s_addr,
	        sizeof(addressIn.sin_addr.s_addr))) {
      if (socktype == CONN_INET) socktype = CONN_UNIX;
    }
  }

  /* build appropriate type of Connections to Server */
  if (socktype == CONN_UNIX) {
    /* build UNIX connection to server */
    addressUn.sun_family = AF_UNIX;
    strncpy(addressUn.sun_path, unix_port,
	    sizeof(struct sockaddr_un) - sizeof(short));
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
      fprintf(stderr,
	      "Error ComOpenConn: pid %d failed to open UNIX fd:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    if (connect(fd, (struct sockaddr *)&addressUn, sizeof(addressUn)) == -1) {
      fprintf(stderr,
	      "Error ComOpenConn: pid %d failed to conn UNIX fd to server:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ACE_OS::close (fd);
      return -1;
    }
    *max_pkt_size = 0;
  }
  else if (socktype == CONN_INET) {
    /* build TCP connection to server */
    addressIn.sin_port = htons(inet_port);
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
      fprintf(stderr,
	      "Error ComOpenConn: pid %d failed to open TCP fd:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
    if (connect(fd, (struct sockaddr *)&addressIn, sizeof(addressIn)) == -1) {
       fprintf(stderr,
	      "Error ComOpenConn: pid %d failed to conn TCP fd to server:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ACE_OS::close (fd);
      return -1;
    }
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
		   (const char *)&i, sizeof(i)) == -1) {
      fprintf(stderr,
    "Error ComOpenConn: pid %d failed to setsockopt on TCP fd with TCP_NODELAY:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ACE_OS::close (fd);
      return -1;
    }
    *max_pkt_size = 0;
  }
  else if (socktype == CONN_ATM) { /* CONN_ATM */
#ifdef NATIVE_ATM
    /* CONN_ATM */
    fd = ATMopenConn(address, max_pkt_size);
    if (fd == -1) {
      fprintf(stderr,
	      "Error ComOpenConn: pid %d failed to open ATM fd conn:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      return -1;
    }
#else
    fprintf(stderr,
	    "Error ComOpenConn: pid %d addresstype %d not supported\n",
	   ACE_OS::getpid (), socktype);
    return -1;
#endif
  }
  for (i = 0; i < size; i ++) {  /* guaranteed to succeed */
    if (fdTable[i].fd == -1) break;
  }
  if (i < size) {
    fdTable[i].fd = fd;
    fdTable[i].type = socktype;
    fdTable[i].stime = get_sec();
    fdTable[i].state = STATE_SINGLE;
    return fd;
  }
  else {
    fprintf(stderr, "<Weird>Error ComOpenConn: no entry for fd %d\n", fd);
    return -1;
  }
}


static int fd_unix, fd_inet, fd_atm;  /* for use in the server side */


int ComInitServer(int pinet_port, char * punix_port, char * patm_port)
{
  int i;
  
  struct sockaddr_in myaddr_in;
  struct sockaddr_un myaddr_un;

  inet_port = pinet_port;
  unix_port = punix_port;
  atm_port = patm_port;
  
  if (size) {
    fprintf(stderr,
	    "Error ComInitServer: pid %d already done ComInitServer/Server\n",
	   ACE_OS::getpid ());
    return -1;
  }
  size = CLIENT_FDTABLE_SIZE;
  // create and initialize the File Descriptor Table
  fdTable = (struct FdTable *)ACE_OS::malloc(size * sizeof(*fdTable));
  if (fdTable == NULL) {
    fprintf(stderr,
	    "Error ComInitServer: pid %d failed to allocated fdTable space:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    return -1;
  }
  for (i = 0; i < size; i ++) {
    fdTable[i].fd = -1;
  }
  // Set all the file descriptor for atm ,inet and unix to be -1
  fd_atm = fd_inet = fd_unix = -1;

  memset((char *)&myaddr_in, 0, sizeof(struct sockaddr_in));
  memset((char *)&myaddr_un, 0, sizeof(struct sockaddr_un));

  myaddr_in.sin_family = AF_INET;
  
  myaddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr_in.sin_port = htons((short)inet_port);

  /* setup INET listen socket */
  fd_inet = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_inet == -1) {
    fprintf(stderr,
	    "Error ComInitServer: pid %d failed to open fd_inet:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    goto open_fd_unix;
  }
  
  i = 1;
  if (setsockopt(fd_inet, SOL_SOCKET, SO_REUSEADDR,
		 (const char *)&i, sizeof(i)) == -1) {
    fprintf(stderr,
	    "Error ComInitServer: pid %d failed to setsockopt fd_inet with REUSEADDR:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    ACE_OS::close (fd_inet);
    fd_inet = -1;
    goto open_fd_unix;
  }
    
  if (bind(fd_inet, (struct sockaddr *)&myaddr_in, sizeof(struct sockaddr_in)) == -1) {
    fprintf(stderr,
	    "Error ComInitServer: pid %d failed to bind fd_inet:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    ACE_OS::close (fd_inet);
    fd_inet = -1;
    goto open_fd_unix;
  }
  if (listen(fd_inet, 2) == -1) {
    fprintf(stderr,
	    "Error ComInitServer: pid %d failed to listen on fd_inet:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    ACE_OS::close (fd_inet);
    fd_inet = -1;
    goto open_fd_unix;
  }

 open_fd_unix:
  
  myaddr_un.sun_family = AF_UNIX;
  strncpy(myaddr_un.sun_path, unix_port,
	  (sizeof(struct sockaddr_un) - sizeof(short)));
  fd_unix = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd_unix == -1) {
    fprintf(stderr,
	    "Error ComInitServer: pid %d failed to open fd_unix:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    goto open_fd_atm;
  }
  if ((bind(fd_unix, (struct sockaddr *)&myaddr_un,
	    sizeof(struct sockaddr_un))) == -1) {
    fprintf(stderr,
	    "Error ComInitServer: pid %d failed to bind fd_unix:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    ACE_OS::close (fd_unix);
    fd_unix = -1;
    goto open_fd_atm;
  }
  if ((listen(fd_unix, 2)) == -1) {
    fprintf(stderr,
	    "Error ComInitServer: pid %d failed to liston on fd_unix:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    ACE_OS::close (fd_unix);
    fd_unix = -1;
    goto open_fd_atm;
  }

 open_fd_atm:

#ifdef NATIVE_ATM
  fd_atm = ATMinitServer(atm_port);
#endif
  
  if (fd_inet >= 0 || fd_unix >= 0 || fd_atm >= 0) return 0;
  else return -1;
}

void ComCloseListen(void)
{
  if (fd_atm >= 0) {
#ifdef NATIVE_ATM
    /* fd_atm is not connected to a user_plane, ignore 
    ATMcloseFd(fd_atm);
    */
    ACE_OS::close (fd_atm);
    fd_atm = -1;
#else
    fprintf(stderr, "Error ComCloseListen: CONN_ATM not supported.\n");
#endif
  }
  if (fd_unix >= 0) {
    ACE_OS::close (fd_unix);
    fd_unix = -1;
  }
  if (fd_inet >= 0) {
    ACE_OS::close (fd_inet);
    fd_inet = -1;
  }
  return;
}

void ComCloseServer(void)
{
  int i;
  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd >= 0) ComCloseConn(fdTable[i].fd);
  }
  if (fd_atm >= 0) {
#ifdef NATIVE_ATM
    /* fd_atm not connected to a user-plane, igore it 
    ATMcloseConn(fd_atm);
    */
    ACE_OS::close (fd_atm);
    fd_atm = -1;
#else
    fprintf(stderr, "Error ComCloseServer: CONN_ATM not supported.\n");
#endif
  }
  if (fd_unix >= 0) {
    ACE_OS::close (fd_unix);
    fd_unix = -1;
    ACE_OS::unlink (unix_port);
  }
  if (fd_inet >= 0) {
    ACE_OS::close (fd_inet);
    fd_inet = -1;
  }
#ifdef NATIVE_ATM
  ATMcloseServer();
#endif
  return;
}

static int change_option(int fd)
{
  struct linger linger = {1, 1};
  int i = 1;
  if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
		 (const char *)&i, sizeof(i)) == -1) {
    fprintf(stderr,
	    "Error ComGetConnPair: pid %d failed to setsockopt on fd_inet:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    return -1;
  }
  /* data socket also NODELAY */
  if (setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char *)&linger,
		 sizeof(linger)) == -1) {
    fprintf(stderr,
	    "Error ComGetConnPair: pid %d failed to linger on fd_inet:",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    return -1;
  }
  return 0;
}

int ComGetConnPair(int *ctr_fd, int *data_fd, int *max_pkt_size)
{
  int i;
  int fd, fdType;
  int addrlen;
  struct sockaddr_in peeraddr_in;
  struct fd_set read_mask;
  struct timeval tval;
  int nfds;
  
  fd = 0;
  addrlen = get_sec();
  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd == -1) {
      fd ++;
    }
    else if (fdTable[i].type == STATE_PENDING &&
		     get_duration(fdTable[i].stime, addrlen) > 10) {
      fprintf(stderr,
	      "ComGetConnPair: pid %d closed fd %d (PENDING >= 10 sec)\n",
	     ACE_OS::getpid (), fdTable[i].fd);
      ComCloseConn(fdTable[i].fd);
      fdTable[i].fd = -1;
      fd ++;
    }
  }
  if (fd < 2) {
    fprintf(stderr,
	    "Error ComGetConnPair: pid %d no faTable entry for the pair",
	   ACE_OS::getpid ());
    return -1;
  }
  
  FD_ZERO(&read_mask);
  if (fd_inet >= 0) FD_SET(fd_inet, &read_mask);
  if (fd_unix >= 0) FD_SET(fd_unix, &read_mask);
  if (fd_atm  >= 0) FD_SET(fd_atm,  &read_mask);

  tval.tv_sec = 1;   /* wait at most 1 second */
  tval.tv_usec = 0;


  nfds = fd_inet;
  if (fd_unix > nfds) nfds = fd_unix;
  if (fd_atm > nfds) nfds = fd_atm;
  // nfds now holds the highest numbered open file descriptor
  nfds ++; // increment so that select can use o to nfds -1 no. of
           // file descriptors.
  errno = 0;
  // fd is here used as the result of the select call
#ifdef _HPUX_SOURCE
  if ((fd = select(nfds, (int *)&read_mask, NULL, NULL, &tval)) == -1)
#else
  if ((fd = select(nfds, &read_mask, NULL, NULL, &tval)) == -1)
#endif
  {
    
    if (errno == EINTR) return -1;
    
    fprintf(stderr,
	    "Error ComGetConnPair: pid %d failed on select():",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    return -1;
  }
  if (fd == 0) return -1; /* time expire */

  fd = -1;
  // fd will be reused inside this code as the accepted socket.
  if (fd == -1 && fd_unix >= 0 && FD_ISSET(fd_unix, &read_mask)) {
    struct sockaddr_un peeraddr_un;
    
    fprintf(stderr, "Server to accept a UNIX connection.\n");
    
    addrlen = sizeof(struct sockaddr_un);
    fd = accept(fd_unix, (struct sockaddr *)&peeraddr_un, &addrlen);
    if (fd == -1) {
      fprintf(stderr,
	      "Error ComGetConnPair: pid %d failed to accpet on fd_unix:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
    }
    else fdType = CONN_UNIX;
    *max_pkt_size = 0;
  }
  // fd here is the result of the  accept for unix if fd_unix is valid
  // Hence if fd_unix is present then this won't be called if the
  // accept for the fd_unix succeeds.
  if (fd == -1 && fd_inet >= 0 && FD_ISSET(fd_inet, &read_mask)) {
    
    fprintf(stderr, "Server to accept a INET connection.\n");
    
    addrlen = sizeof(struct sockaddr_in);
    fd = accept(fd_inet, (struct sockaddr *)&peeraddr_in, &addrlen);
    if (fd == -1) {
      fprintf(stderr,
	      "Error ComGetConnPair: pid %d failed to accpet on fd_inet:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
    }
    else fdType = CONN_INET;
    *max_pkt_size = 0;
  }
  if (fd == -1 && fd_atm >= 0 && FD_ISSET(fd_atm, &read_mask)) {
#ifdef NATIVE_ATM
    fd = ATMaccept(fd_atm, max_pkt_size);
    fdType = CONN_ATM;
#else
    fprintf(stderr,
	    "Error ComGetConnPair: pid %d CONN_ATM not supported.\n",
	   ACE_OS::getpid ());
#endif
  }
  // fd = -1 implies accept failed in any of the active connections
  // which came out of the select call.
  if (fd == -1) return -1;

  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd == -1) break;
  }
  // Enter the accepted fd in the file descriptor Table
  if (i < size) {
    fdTable[i].fd = fd;
    fdTable[i].type = fdType;
    fdTable[i].stime = get_sec();
  }
  else {
    fprintf(stderr, "Weird\n");
  }

  // read the port number for the UDP socket of the client
  if (time_read_int(fd, &nfds) == -1) {
    fprintf(stderr,
	    "Error ComGetConnPair: pid %d failed to read int from %s fd:",
	   ACE_OS::getpid (),
	    fdType == CONN_ATM ? "ATM" :
	    fdType == CONN_INET ? "INET" : "UNIX");
   ACE_OS::perror ("");
    ComCloseConn(fd);
    return -1;
  }
  fprintf (stderr,
           " Received int nfds = %d \n",
           nfds);
  // nfds holds the int read in time_read_int
  // if nfds is -1 then read int error.
  fprintf(stderr, "ComGetConnPair got %s fd = %d with value %d\n",
	  fdType == CONN_ATM ? "ATM" :
	  fdType == CONN_INET ? "INET" : "UNIX",
	  fd, nfds);

  if (nfds >= 0) { /* can be paired and return */
    // This is the success case where you got a control fd and a data fd.
    fdTable[i].state = STATE_DATA;
    for (i = 0; i < size; i ++) {
      if (fdTable[i].fd == nfds) break;
    }
    if (i < size && fdTable[i].state == STATE_PENDING) {
      fdTable[i].state = STATE_CONTROL;
    }
    else {
      if (i >= 0) fprintf(stderr, "<Weird> i %d\n", i);
      ComCloseConn(fd);
      return -1;
    }
    // ????
    *ctr_fd = fdTable[i].fd;
    *data_fd = fd;
    return 0;
  }
  else if (nfds == -1) { /* wait to be paired */
    fdTable[i].state = STATE_PENDING;
    // fd is the inet or unix accepted socket.
    // why is anyone writing the fd to the peer?
    // looks like he is indicating an error to the client!!
    // is this a hack ;-(
    if (time_write_int(fd, fd) == -1) {
      fprintf(stderr,
	      "Error ComGetConnPair: pid %d failed to write (fd) to %s fd:",
	     ACE_OS::getpid (),
	      fdType == CONN_ATM ? "ATM" :
	      fdType == CONN_INET ? "INET" : "UNIX");
     ACE_OS::perror ("");
      ComCloseConn(fd);
      return -1;
    }
    if (fdType == CONN_INET && change_option(fd) == -1) {
      ComCloseConn(fd);
    }
    return -1;
  }
  else if (nfds == -2) {  /* inet case only, UDP data sock is requested */
    int dfd;
    struct sockaddr_in * in = &(peeraddr_in);
    
    fdTable[i].state = STATE_CONTROL;

    if (fdType == CONN_INET && change_option(fd) == -1) {
      fprintf(stderr,
	      "Error ComGetConnPair: pid %d failed to change options of TCP cfd:",
	     ACE_OS::getpid ());
      ComCloseConn(fd);
      return -1;
    }
    
    dfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (dfd == -1) {
      fprintf(stderr,
	  "Error ComGetConnPair: pid %d failed to open dfd:",
	 ACE_OS::getpid ());
     ACE_OS::perror ("");
      ComCloseConn(fd);
      return -1;
    }
    memset((char *)in, 0, sizeof(* in));
    
    addrlen = sizeof(*in);
    if (getsockname(fd, (struct sockaddr *)in, &addrlen) == -1) {
      fprintf(stderr,
	  "Error ComGetConnPair: pid %d failed to getsockname of fd:",
	 ACE_OS::getpid ());
     ACE_OS::perror ("");
      ComCloseConn(fd);
      ACE_OS::close (dfd);
      return -1;
    }
    
    in->sin_family = AF_INET;
    in->sin_port = 0;
    /*
    in->sin_addr.s_addr = htonl(INADDR_ANY);
    */
    if (bind(dfd, (struct sockaddr *)in, sizeof(* in)) == -1) {
      fprintf(stderr,
	  "Error ComGetConnPair: pid %d failed to bind dfd:",
	 ACE_OS::getpid ());
     ACE_OS::perror ("");
      ComCloseConn(fd);
      ACE_OS::close (dfd);
      return -1;
    }
    addrlen = sizeof(*in);
    if (getsockname(dfd, (struct sockaddr *)in, &addrlen) == -1) {
      fprintf(stderr,
	  "Error ComGetConnPair: pid %d failed to getsockname of dfd:",
	 ACE_OS::getpid ());
     ACE_OS::perror ("");
      ComCloseConn(fd);
      ACE_OS::close (dfd);
      return -1;
    }
    if (time_write_bytes(fd, (char *)&in->sin_port, sizeof(short)) == -1) {
      fprintf(stderr,
	  "Error ComGetConnPair: pid %d failed to write (dfd.port) to fd:",
	 ACE_OS::getpid ());
     ACE_OS::perror ("");
      ComCloseConn(fd);
      ACE_OS::close (dfd);
      return -1;
    }
    /*
    addrlen = sizeof(*in);
    if (getsockname(fd, (struct sockaddr *)in, &addrlen) == -1) {
      fprintf(stderr,
	  "Error ComGetConnPair: pid %d failed to getsockname of fd:",
	 ACE_OS::getpid ());
     ACE_OS::perror ("");
      ComCloseConn(fd);
      ACE_OS::close (dfd);
      return -1;
    }
    */
    if (time_write_bytes(fd, (char *)&in->sin_addr.s_addr, sizeof(int)) == -1) {
      fprintf(stderr,
	  "Error ComGetConnPair: pid %d failed to write (dfd.IPaddr) to fd:",
	 ACE_OS::getpid ());
     ACE_OS::perror ("");
      ComCloseConn(fd);
      ACE_OS::close (dfd);
      return -1;
    }

    fprintf(stderr, "ComGetConnPair local UDP socket: addr - %s, port - %u.\n",
            inet_ntoa(in->sin_addr), ntohs(in->sin_port));

    if (time_read_bytes(fd, (char *)&in->sin_port, sizeof(short)) == -1 ||
	time_read_bytes(fd, (char *)&in->sin_addr.s_addr, sizeof(int)) == -1) {
      fprintf(stderr,
	  "Error ComGetConnPair: pid %d failed to read (port, IPaddr) from fd:",
	 ACE_OS::getpid ());
     ACE_OS::perror ("");
      ComCloseConn(fd);
      ACE_OS::close (dfd);
      return -1;
    }
    usleep(10000);
    if (connect(dfd, (struct sockaddr *)in, sizeof(*in)) == -1) {
      fprintf(stderr,
	      "Error ComGetConnPair: pid %d failed conn UDP dfd to peer:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
      ComCloseConn(fd);
      ACE_OS::close (dfd);
      return -1;
    }
#if 0
    {
      int len;
      len = 65536; /* INET_SOCKET_BUFFER_SIZE + 100; */
      /*
      if (setsockopt(dfd, SOL_SOCKET, SO_SNDBUF, (char *)&len, sizeof(len)) == -1) {
	fprintf(stderr,
		"Error ComGetConnPair: pid %d failed to set UDP dfd-snd to size %d:",
		getpid(), len);
	perror("");
	ComCloseConn(fd);
	close(dfd);
	return -1;
      }
      */
      len = 65536;
      while (setsockopt(dfd, SOL_SOCKET, SO_RCVBUF, (char *)&len, sizeof(len)) == -1) {
	len -= 1024;
	if (len < 8192) break;
	/*
	fprintf(stderr,
		"Warning ComGetConnPair: pid %d failed to set UDP dfd-rcv to size %d:",
		getpid(), len);
	perror("");
	ComCloseConn(fd);
	close(dfd);
	return -1;
	*/
      }
      if (getuid() == DEVELOPER_UID)
      fprintf(stderr,"Set UDP dfd-rcv to %dB\n", len);
    }
#endif
    // Most common return case where the server creates a UDP socket ,
    // writes the port and ip addr and also gets the client's UDP
    // endpoint .
    *max_pkt_size = - INET_SOCKET_BUFFER_SIZE;  /* UDP sockets on HP and SUN
						are known to be discard mode */

    fprintf(stderr, "ComGetConnPair UDP dfd connects to host-%s (port %u)\n",
	    inet_ntoa(in->sin_addr), ntohs(in->sin_port));

    for (i = 0; i < size; i ++) {
      if (fdTable[i].fd == -1) break;
    }
    if (i < size) {
      fdTable[i].fd = dfd;
      fdTable[i].type = CONN_INET;
      fdTable[i].stime = get_sec();
      fdTable[i].state = STATE_DATA;

      *ctr_fd = fd;
      *data_fd = dfd;
      return 0;
    }
    else {
      fprintf(stderr, "ComGetConnPair <Weird>: no space left\n");
      return -1;
    }
  }
  else {
    fprintf(stderr,
	    "Error ComGetConnPair: pid %d get invalid value %d from fd\n",
	   ACE_OS::getpid (), nfds);
    ComCloseConn(fd);
  }
  return -1;
}

/* returns: -1 -- failed,  >= 0 -- fd */
int ComGetConn(int *max_pkt_size)
{
  int i;
  int fd, fdType;
  int addrlen;
  struct fd_set read_mask;
  struct timeval tval;
  int nfds;
  
  fd = 0;
  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd == -1) {
      fd ++;
    }
  }
  if (fd < 1) {
    fprintf(stderr,
	    "Error ComGetConn: pid %d no faTable entry for the pair",
	   ACE_OS::getpid ());
    return -1;
  }
  
  FD_ZERO(&read_mask);
  if (fd_inet >= 0) FD_SET(fd_inet, &read_mask);
  if (fd_unix >= 0) FD_SET(fd_unix, &read_mask);
  if (fd_atm  >= 0) FD_SET(fd_atm,  &read_mask);

  tval.tv_sec = 1;   /* wait at most 1 second */
  tval.tv_usec = 0;

  nfds = fd_inet;
  if (fd_unix > nfds) nfds = fd_unix;
  if (fd_atm > nfds) nfds = fd_atm;
  nfds ++;
  errno = 0;
#ifdef _HPUX_SOURCE
  if ((fd = select(nfds, (int *)&read_mask, NULL, NULL, &tval)) == -1)
#else
  if ((fd = select(nfds, &read_mask, NULL, NULL, &tval)) == -1)
#endif
  {
    
    if (errno == EINTR) return -1;
    
    fprintf(stderr,
	    "Error ComGetConn: pid %d failed on select():",
	   ACE_OS::getpid ());
   ACE_OS::perror ("");
    return -1;
  }
  if (fd == 0) return -1; /* time expire */

  fd = -1;

  if (fd == -1 && fd_unix >= 0 && FD_ISSET(fd_unix, &read_mask)) {
    struct sockaddr_un peeraddr_un;

    fprintf(stderr, "Server to accept a UNIX connection.\n");

    addrlen = sizeof(struct sockaddr_un);
    fd = accept(fd_unix, (struct sockaddr *)&peeraddr_un, &addrlen);
    if (fd == -1) {
      fprintf(stderr,
	      "Error ComGetConn: pid %d failed to accpet on fd_unix:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
    }
    else fdType = CONN_UNIX;
    *max_pkt_size = 0;
  }
  if (fd == -1 && fd_inet >= 0 && FD_ISSET(fd_inet, &read_mask)) {
    struct sockaddr_in peeraddr_in;

    fprintf(stderr, "Server to accept a INET connection.\n");

    addrlen = sizeof(struct sockaddr_in);
    fd = accept(fd_inet, (struct sockaddr *)&peeraddr_in, &addrlen);
    if (fd == -1) {
      fprintf(stderr,
	      "Error ComGetConn: pid %d failed to accpet on fd_inet:",
	     ACE_OS::getpid ());
     ACE_OS::perror ("");
    }
    else fdType = CONN_INET;
    *max_pkt_size = 0;
  }
  if (fd == -1 && fd_atm >= 0 && FD_ISSET(fd_atm, &read_mask)) {
#ifdef NATIVE_ATM
    fd = ATMaccept(fd_atm, max_pkt_size);
    fdType = CONN_ATM;
#else
    fprintf(stderr,
	    "Error ComGetConn: pid %d CONN_ATM not supported.\n",
	   ACE_OS::getpid ());
#endif
  }
  if (fd == -1) return -1;

  for (i = 0; i < size; i ++) {
    if (fdTable[i].fd == -1) break;
  }
  if (i < size) {
    fdTable[i].fd = fd;
    fdTable[i].type = fdType;
    fdTable[i].stime = get_sec();
    fdTable[i].state = STATE_SINGLE;
    return fd;
  }
  else {
    fprintf(stderr, "Weird\n");
    ComCloseConn(fd);
    return -1;
  }
}
