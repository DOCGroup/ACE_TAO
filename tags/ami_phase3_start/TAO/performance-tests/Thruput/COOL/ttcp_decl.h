// $Id$

/*
 *    T T C P Header File
 *
 */

/* This file includes all the declarations and prototypes */

#ifndef _TTCP_DECL_H_
#define _TTCP_DECL_H_

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Profile_Timer.h"

/* File to be included if Quantify is to be used */
#if defined (ACE_HAS_QUANTIFY)
# include <quantify.h>
#endif /* ACE_HAS_QUANTIFY */


/* define the data types to be sent */
#define SEND_SHORT ((CORBA::ULong)(1))
#define SEND_LONG ((CORBA::ULong)(2))
#define SEND_CHAR ((CORBA::ULong)(3))
#define SEND_OCTET ((CORBA::ULong)(4))
#define SEND_DOUBLE ((CORBA::ULong)(5))
#define SEND_STRUCT ((CORBA::ULong)(6))
#define SEND_COMPOSITE ((CORBA::ULong)(7))

/**************
// Prototypes
***************/
int err (char *s);
void mes (CORBA::Char *s);
CORBA::String outfmt (CORBA::Double b);
void prep_timer (void);
CORBA::Double read_timer (char *str, CORBA::Long len);
void prusage (register struct rusage *r0, struct rusage *r1, struct timeval *e, struct timeval *b, char *outp);
void tvadd (struct timeval *tsum, struct timeval *t0, struct timeval *t1);
void tvsub (struct timeval *tdiff, struct timeval *t1, struct timeval *t0);
void psecs (CORBA::Long l, register char *cp);
void delay (CORBA::Long us);
void FillPattern (register char *cp, register CORBA::Long bufLen, CORBA::ULong dt);
void PrintStats (void);

/* Global variables defined here as extern */
extern ACE_Svc_Export CORBA::Long trans;        // whether we are transmitter
                                                // or receiver
extern ACE_Svc_Export CORBA::Long buflen;               /* length of buffer */
extern ACE_Svc_Export char *buf;                /* ptr to dynamic buffer */
extern ACE_Svc_Export CORBA::Long nbuf;         /* number of buffers to send in sinkmode */
extern ACE_Svc_Export CORBA::Short port;                /* TCP port number */
extern ACE_Svc_Export char *host;              /* ptr to name of host */
extern ACE_Svc_Export CORBA::Long verbose;                      /* 0=print basic info, 1=prCORBA::Long cpu rate, proc
                                 * resource usage. */
extern ACE_Svc_Export CORBA::Char fmt;          /* output format:k=kilobits,K=kilobytes,
                                 *  m = megabits, M = megabytes,
                                 *  g = gigabits, G = gigabytes */
extern ACE_Svc_Export char *title;

extern ACE_Svc_Export char stats[128];
extern ACE_Svc_Export CORBA::ULong srcDataSize; /* Total amount of source data */
extern ACE_Svc_Export CORBA::ULong nbytes;              /* bytes on net */
extern ACE_Svc_Export CORBA::ULong numCalls;            /* # of I/O system calls */
extern ACE_Svc_Export CORBA::Double cput, realt;                /* user, real time (seconds) */
extern ACE_Svc_Export CORBA::ULong      dt;

/* declare struct variables for various message types */
extern ACE_Svc_Export ttcp_sequence::ShortSeq           *sseq;
extern ACE_Svc_Export ttcp_sequence::LongSeq            *lseq;
extern ACE_Svc_Export ttcp_sequence::OctetSeq           *oseq;
extern ACE_Svc_Export ttcp_sequence::DoubleSeq          *dseq;
extern ACE_Svc_Export ttcp_sequence::CharSeq            *cseq;
extern ACE_Svc_Export ttcp_sequence::StructSeq          *Sseq;

#endif
