/* -*- C++ -*- */
//$Id$
// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    QosEvent.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef QOSEVENT_H
#define QOSEVENT_H

// @@ Vishal, we should probably most this stuff to OS.h at some point
// so that it'll be available to all applications that use the ACE QoS
// wrappers.

// I agree. However, if the user chooses to use other options than used
// in this test application she might have to declare other such stuff.
// I would suggest keeping these defines to the test program until we find
// a more portable way of handling this. This would mean having these defines 
// in the user applications which is not neat but it will avoid cluttering 
// OS.h with incomplete QoS related defines.
  
#if !(defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))

#define XP1_QOS_SUPPORTED        0x00002000
#define XP1_SUPPORT_MULTIPOINT   0x00000400

#define TRUE  1
#define FALSE 0

#define WSABASEERR              10000
#define WSAENOBUFS              (WSABASEERR+55)

#define FROM_PROTOCOL_INFO (-1)

#define WSA_FLAG_OVERLAPPED           0x01
#define WSA_FLAG_MULTIPOINT_C_ROOT    0x02
#define WSA_FLAG_MULTIPOINT_C_LEAF    0x04
#define WSA_FLAG_MULTIPOINT_D_ROOT    0x08
#define WSA_FLAG_MULTIPOINT_D_LEAF    0x10

#define QOS_NOT_SPECIFIED     0xFFFFFFFF
#define SERVICETYPE_NOTRAFFIC 0x00000000  // No data in this direction.
#define SERVICETYPE_CONTROLLEDLOAD 0x00000002  // Controlled Load. 
#define SERVICETYPE_GUARANTEED 0x00000003  // Guaranteed.

#define JL_SENDER_ONLY    0x01
#define JL_BOTH           0x04

#define SIO_GET_QOS              (0x40000000 | 0x08000000 | 7)
#define SIO_MULTIPOINT_LOOPBACK  (0x08000000 | 9)
#define SIO_MULTICAST_SCOPE      (0x08000000 | 10)
#define SIO_SET_QOS			     (0x08000000 | 11)

#endif /* ACE_HAS_WINSOCK2 */


#define MY_DEFPORT (5001)

// Application-specific define - there is NO specific invalid send
// priority.  This value is simply used to indicate whether or not to
// even set priority.
#define INVALID_SEND_PRIORITY 255

// Application-specific define - there is no "default" QOS template in
// GQoS, only for this particular application does "default" have
// meaning.
#define QOS_DEFAULTNAME "default"       

// Use an arbitrary multicast group if none is specified on the
// command line
#define DEFAULT_MULTICASTGROUP "234.5.6.7"

typedef struct _SP_OPTIONS
{
  int iProtocol;             
  // protocol to use

  int bQos;                  
  // enable QOS

  int bMulticast;            
  // enable multicasting
} SP_OPTIONS;

typedef enum 
{
  QOS_IOCTL_SET_DURING,       
  // do not use ioctl to set Qos

  QOS_IOCTL_SET_BEFORE,       
  // set qos with ioctl before connection/accept

  QOS_IOCTL_SET_AFTER,        
  // set qos with ioctl after connection/accept

  QOS_IOCTL_SET_QOS           
  // set qos in receiver during FD_QOS, note that qos must be set
  // prior to wsaeventselect in order to ever receive fd_qos
} QOS_IOCTL_SET;


typedef struct _QOS_OPTIONS
{
  int    bReceiver;          
  // act as receiver if TRUE

  int    bWaitToSend;        
  // sender must wait for RESV before sending

  int    bConfirmResv;       
  // have receiver request RESV confirmation

  int    bQueryBufferSize;   
  // have WSAioctl return size of buffer needed when using SIO_GET_QOS
  // (behaviour not avail on Win98).

  int    bMustSetQosInAccept;
  // Will be set for Win98 only.  Win98 currently needs valid qos
  // structure to be set in wsaaccept condition func in lpSQos is
  // valid

  int    bDisableSignalling; 
  // If TRUE do not emit RSVP signalling, just use traffic control

  int    bAlternateQos;      
  // Alternate between enabling and disabling QOS on a socket every N
  // sends

  unsigned int   SendPriority;       
  // create QOS_OBJECT_PRIORITY structure, and set valid priority 0-7

  int    bSetDestaddr;       
  // TRUE if unconnected UDP and QOS is used (not multicast)

  int    bProviderSpecific;  
  // TRUE if a provider-specific object must be set

  int    bFineGrainErrorAvail;   
  // TRUE if Win2000beta3 availability of fine grain errors on SIO_SET_QOS

  int    bQosabilityIoctls;   
  // TRUE if Win2000beta3 qosablity ioctls 

  int    bFixTemplate;       
  // TRUE if Win98 to divide ToeknRate and PeakBandwidth by 8 when
  // using WSAGetQOSByName

  QOS_IOCTL_SET  qosIoctlSet;  
  // when to set QOS

  char    szTemplate[64];     
  // qos template
} QOS_OPTIONS;

extern ACE_Flow_Spec default_notraffic;
extern ACE_Flow_Spec default_g711;

typedef struct _OPTIONS
{
  char            szHostname[64];
  unsigned short  port;
  int             nRepeat;
  char            fillchar;
  int             nBufSize;
  char *          buf;
  u_long          dwSleep;
  u_long          dwTotalClients;
  QOS_OPTIONS     qosOptions;
  SP_OPTIONS      spOptions;
} OPTIONS;

#ifdef QOSEVENT_MAIN

OPTIONS default_options = 
{
  "localhost",
  MY_DEFPORT,
  1,
  'c',
  4096,
  NULL,
  0,
  0,
  {TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, INVALID_SEND_PRIORITY, 
   TRUE, TRUE, TRUE, TRUE, FALSE, QOS_IOCTL_SET_BEFORE, QOS_DEFAULTNAME},
  {IPPROTO_TCP, FALSE, FALSE}
};

ACE_Flow_Spec default_notraffic (QOS_NOT_SPECIFIED,
                                 QOS_NOT_SPECIFIED,
                                 QOS_NOT_SPECIFIED,
                                 QOS_NOT_SPECIFIED,
                                 QOS_NOT_SPECIFIED,
                                 SERVICETYPE_NOTRAFFIC,
                                 QOS_NOT_SPECIFIED,
                                 QOS_NOT_SPECIFIED,
                                 25,
                                 1);
    
ACE_Flow_Spec default_g711 (9200,
                            708,
                            18400,
                            0,
                            0,
                            SERVICETYPE_CONTROLLEDLOAD,
                            368,
                            368,
                            25,
                            1);


#else

extern FLOWSPEC default_notraffic;
extern FLOWSPEC default_g711;
extern SOCKADDR_IN g_destaddr;
    
#endif /* QOSEVENT_MAIN */

extern int FindServiceProvider (int iProtocol,
                                int bQos,
                                int bMulticast,
                                ACE_Protocol_Info *pProtocolInfo);

#endif /* QOSEVENT_H */
