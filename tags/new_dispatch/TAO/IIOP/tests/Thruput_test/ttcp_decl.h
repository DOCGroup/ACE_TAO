
/*
 *    T T C P Header File
 *
 */

/* This file includes all the declarations and prototypes */

#ifndef _TTCP_DECL_H_
#define _TTCP_DECL_H_

#include "ttcpC.h"

/* #define BSD43 */
/* #define BSD42 */
/* #define BSD41a */


#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>		/* struct itimerval */
#include <limits.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream.h>
#include <sys/resource.h>

/* File to be included if Quantify is to be used */
#if defined (USE_QUANTIFY)
#include <quantify.h>
#endif


#if defined(SYSV)
#define bcopy(b1,b2,n)  memcpy(b2,b1,n)
#define bzero(b1,n)             memset(b1,0,n)
#include <sys/times.h>
#include <sys/param.h>
#endif

#if defined(SUNOS54)
struct rusage
  {
    struct timeval ru_utime, ru_stime;
  };
#define RUSAGE_SELF 0
#else
#include <sys/resource.h>
#endif

/* define the data types to be sent */
#define SEND_SHORT ((unsigned long)(1))
#define SEND_LONG ((unsigned long)(2))
#define SEND_CHAR ((unsigned long)(3))
#define SEND_OCTET ((unsigned long)(4))
#define SEND_DOUBLE ((unsigned long)(5))
#define SEND_STRUCT ((unsigned long)(6))
#define SEND_COMPOSITE ((unsigned long)(7))
 
/**************
// Prototypes
***************/
void err (char *s);
void mes (char *s);
char *outfmt (double b);
//static void getrusage (int ignored, register struct rusage *ru);
static void gettimeofday (struct timeval *tp, struct timezone *zp);
void prep_timer (void);
double read_timer (char *str, int len);
static void prusage (register struct rusage *r0, struct rusage *r1, struct timeval *e, struct timeval *b, char *outp);
static void tvadd (struct timeval *tsum, struct timeval *t0, struct timeval *t1);
static void tvsub (struct timeval *tdiff, struct timeval *t1, struct timeval *t0);
static void psecs (long l, register char *cp);
void delay (int us);
int mread (int fd, register char *bufp, unsigned n);
int Nread (int fd, void *buf, int count);
int Nwrite (int fd, void *buf, int count);
void FillPattern(register char *cp, register int bufLen, unsigned long dt);
void PrintStats();
int SetSocketBufferSize(int size, unsigned short isTrans);
int SetNoDelay();
void ProcessArguments(int argc, char **argv, unsigned short isTrans);

/* Global variables defined here as extern */
extern int fromlen;
extern int domain;           	/* Default is to use Internet domain sockets. */
extern char *domainname;	/* Rendezvous address for UNIX domain sockets. */
extern int fd;			/* fd of network socket */

extern int buflen;		/* length of buffer */
extern char *buf;		/* ptr to dynamic buffer */
extern int nbuf;		/* number of buffers to send in sinkmode */

extern int bufoffset;		/* align buffer to this */
extern int bufalign;		/* modulo this */

extern int udp;			/* 0 = tcp, !0 = udp */
extern int options;		/* socket options */
extern int one;			/* for 4.3 BSD style setsockopt() */
extern short port;		/* TCP port number */
extern char *host;              /* ptr to name of host */
extern int trans;		/* 0=receive, !0=transmit mode */
extern int sinkmode;		/* 0=normal I/O, !0=sink/source mode */
extern int verbose;			/* 0=print basic info, 1=print cpu rate, proc
				 * resource usage. */
extern int nodelay;		/* set TCP_NODELAY socket option */
extern int b_flag;		/* use mread() */
extern int sockbufsize;		/* socket buffer size to use */
extern int new_line;            /* This is a special flag */
extern int write_to_file;       /* indecates writing to file (default)*/
extern char fmt;		/* output format:k=kilobits,K=kilobytes,
				 *  m = megabits, M = megabytes, 
				 *  g = gigabits, G = gigabytes */
extern int touchdata;		/* access data after reading */

extern struct hostent *addr;
extern int errno;
extern int optind;
extern char *optarg;
extern char *title;

extern char stats[128];
extern unsigned long srcDataSize;	/* Total amount of source data */
extern unsigned long nbytes;		/* bytes on net */
extern unsigned long numCalls;		/* # of I/O system calls */
extern double cput, realt;		/* user, real time (seconds) */
extern unsigned long	dt;

extern unsigned short isDone;

/* declare struct variables for various message types */
extern ttcp_sequence::ShortSeq		*sseq;	
extern ttcp_sequence::LongSeq		*lseq;	
extern ttcp_sequence::OctetSeq		*oseq;	
extern ttcp_sequence::DoubleSeq	        *dseq;	
extern ttcp_sequence::CharSeq		*cseq;	
extern ttcp_sequence::StructSeq	        *Sseq;

#endif
