//$Id$

#ifndef QOSEVENT_H
#define QOSEVENT_H

#define MY_DEFPORT (5001)

// Application specific define - there is NO specific invalid send priority.
// This value is simply used to indicate whether or not to even set priority.
#define INVALID_SEND_PRIORITY 255

// application specific define - there is no "default" QOS template in GQoS,
// only for this particular application does "default" have meaning.
#define QOS_DEFAULTNAME "default"       

// Use an arbitrary multicast group if none is specified on the command line
#define DEFAULT_MULTICASTGROUP        "234.5.6.7"

typedef struct _SP_OPTIONS
{
  int iProtocol;             // protocol to use
  int bQos;                  // enable QOS
  int bMulticast;            // enable multicasting
} SP_OPTIONS;

typedef enum 
{
  QOS_IOCTL_SET_DURING,       // do not use ioctl to set Qos
  QOS_IOCTL_SET_BEFORE,       // set qos with ioctl before connection/accept
  QOS_IOCTL_SET_AFTER,        // set qos with ioctl after connection/accept
  QOS_IOCTL_SET_QOS           // set qos in receiver during FD_QOS, note that 
                                //  qos must be set prior to wsaeventselect in 
                                //  order to ever receive fd_qos
} QOS_IOCTL_SET;


typedef struct _QOS_OPTIONS
{
  int    bReceiver;          // act as receiver if TRUE
  int    bWaitToSend;        // sender must wait for RESV before sending
  int    bConfirmResv;       // have receiver request RESV confirmation
  int    bQueryBufferSize;   // have WSAioctl return size of buffer needed 
                                //      when using SIO_GET_QOS (behaviour not
                                //      avail on Win98).
  int    bMustSetQosInAccept;// Will be set for Win98 only.  Win98 currently
                                //      needs valid qos structure to be set in
                                //      wsaaccept condition func in lpSQos is valid
  int    bDisableSignalling; // If TRUE do not emit RSVP signalling, just use 
                                //      traffic control
  int    bAlternateQos;      // Alternate between enabling and disabling QOS on 
                                //      a socket every N sends
  unsigned int   SendPriority;       // create QOS_OBJECT_PRIORITY structure, and
                                //  set valid priority 0-7
  int    bSetDestaddr;       // TRUE if unconnected UDP and QOS is used (not multicast)
  int    bProviderSpecific;  // TRUE if a provider-specific object must be set
  int    bFineGrainErrorAvail;   // TRUE if Win2000beta3 availability of fine grain errors on SIO_SET_QOS
  int    bQosabilityIoctls;   // TRUE if Win2000beta3 qosablity ioctls 
  int    bFixTemplate;       // TRUE if Win98 to divide ToeknRate and PeakBandwidth
                                //   by 8 when using WSAGetQOSByName 
  QOS_IOCTL_SET  qosIoctlSet;  // when to set QOS
  char    szTemplate[64];     // qos template
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

ACE_Flow_Spec default_notraffic (
								 QOS_NOT_SPECIFIED,
								 QOS_NOT_SPECIFIED,
								 QOS_NOT_SPECIFIED,
								 QOS_NOT_SPECIFIED,
								 QOS_NOT_SPECIFIED,
								 SERVICETYPE_NOTRAFFIC,
								 QOS_NOT_SPECIFIED,
								 QOS_NOT_SPECIFIED,
								 25,
								 1);
    

ACE_Flow_Spec default_g711 (
							9200,
							708,
							18400,
							0,
							0,
							SERVICETYPE_CONTROLLEDLOAD,
							368,
							368,
							25,
							1);

/*

FLOWSPEC default_g711 =
    {
    8500,
    680,
    17000,
    QOS_NOT_SPECIFIED,
    QOS_NOT_SPECIFIED,
    SERVICETYPE_GUARANTEED,
    340,
    340
    };


FLOWSPEC default_g711 =
    {
    68000,
    6800,
    136000,
    QOS_NOT_SPECIFIED,
    QOS_NOT_SPECIFIED,
    SERVICETYPE_GUARANTEED,
    340,
    340
    };
*/

SOCKADDR_IN g_destaddr;

#else

extern FLOWSPEC default_notraffic;
extern FLOWSPEC default_g711;
extern SOCKADDR_IN g_destaddr;
    
#endif

extern int FindServiceProvider(
                                int iProtocol,
                                int bQos,
                                int bMulticast,
                                ACE_Protocol_Info *pProtocolInfo);

#endif
