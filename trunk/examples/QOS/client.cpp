/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    client.cpp
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/OS.h"

#define QOSEVENT_MAIN
#include "QosEvent.h"
#include "Sender_QOS_Event_Handler.h"
    
static u_short SERVER_PORT = MY_DEFPORT;
static const char *const SERVER_HOST = DEFAULT_MULTICASTGROUP;
static const int MAX_ITERATIONS = 4;

static int ValidOptions (char *argv[],
                         int argc,
                         OPTIONS *pOptions);
static void PrintOptions (OPTIONS *pOptions);
static void Usage (char *szProgramname,
                   OPTIONS *pOptions);

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

// To open QOS sockets administrative access is required on the
// machine. Fill in default values for QoS structure.  The default
// values were simply choosen from existing QOS templates available
// via WSAGetQosByName.  Notice that ProviderSpecific settings are
// being allowed when picking the "default" template but not for
// "well-known" QOS templates.  Also notice that since data is only
// flowing from sender to receiver, different flowspecs are filled in
// depending upon whether this application is acting as a sender or
// receiver.

static int
fill_ace_qos_flowspec_default (ACE_QoS *pQos,
                               QOS_OPTIONS *pQosOptions)
{
  ACE_Flow_Spec ace_default_g711 (9200,
                                  708,
                                  18400,
                                  0,
                                  0,
                                  SERVICETYPE_GUARANTEED, 
                                  // SERVICETYPE_CONTROLLEDLOAD  
                                  // doesnt work here for a yet unknown
                                  // reason.
                                  368,
                                  368,
                                  25, // TTL. ACE specific. Not used on NT.
                                  1	// Priority. ACE specific. Not used on NT.
                                  );
	
  ACE_Flow_Spec ace_default_notraffic (QOS_NOT_SPECIFIED,
                                       QOS_NOT_SPECIFIED,
                                       QOS_NOT_SPECIFIED,
                                       QOS_NOT_SPECIFIED,
                                       QOS_NOT_SPECIFIED,
                                       SERVICETYPE_NOTRAFFIC,
                                       QOS_NOT_SPECIFIED,
                                       QOS_NOT_SPECIFIED,
                                       25,  // TTL. ACE specific. Not used on NT.
                                       1);  // Priority. ACE specific. Not used on NT.
  if (pQosOptions->bReceiver)
    {
      pQos->receiving_flowspec (ace_default_g711);
      pQos->sending_flowspec (ace_default_notraffic);        
    }
  else
    {
      pQos->sending_flowspec (ace_default_g711);     
      pQos->receiving_flowspec (ace_default_notraffic);
    }

  const iovec iov = {0, 0};
  pQos->provider_specific (iov);

  return 0;
}

int SetQos (QOS_OPTIONS *pQosOptions,
            int bSetQos,
            ACE_QoS *pQos) 
{
  int bError = FALSE;

  // fill in default values
  if (ACE_OS::strcasecmp (pQosOptions->szTemplate,
                          QOS_DEFAULTNAME) == 0)
    fill_ace_qos_flowspec_default (pQos,
                                   pQosOptions);
  else 
    ACE_DEBUG ((LM_DEBUG,
                "Run the program with -q:default option\n"));
  return !bError;
}

// Fill up the ACE_Flow_Spec with the default_g711 values as defined
// in the QoSEvent.h

int 
FillQoSTraffic (ACE_Flow_Spec &afc)
{
  afc = default_g711;
  return 0;
}

// Fill up the ACE_Flow_Spec with the default_notraffic values
// as defined in the QoSEvent.h

int
FillQoSNoTraffic (ACE_Flow_Spec &afc)
{
  afc = default_notraffic;
  return 0;
}

// This function fills up the ACE_QoS_Params with the supplied 
// iovec and ACE_QoS.

int
FillQoSParams (ACE_QoS_Params &qos_params,
               iovec* iov, 
               ACE_QoS* qos)
{
  qos_params.callee_data (iov);
  qos_params.caller_data (0);
  qos_params.socket_qos (qos);
  qos_params.group_socket_qos (0);
  qos_params.flags (JL_SENDER_ONLY);

  return 0;
}

int
main (int argc, char * argv[])
{  
  ACE_Protocol_Info protocol_info;

  OPTIONS options;

  if (!ValidOptions (argv,
                     argc,
                     &options))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in ValidOptions\n"),
                      -1);

  if (FindServiceProvider (options.spOptions.iProtocol,
                           options.spOptions.bQos,
                           options.spOptions.bMulticast,
                           &protocol_info) == FALSE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in FindServiceProvider\n"),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "Sender\n"));

  ACE_QoS qos;
         
  // By default the QOS is set as QOS_IOCTL_SET_BEFORE. 
  if (options.spOptions.bQos)
    {
      if (QOS_IOCTL_SET_BEFORE == options.qosOptions.qosIoctlSet)
        {
          if (SetQos (&options.qosOptions,
                      TRUE,
                      &qos))
            ACE_DEBUG ((LM_DEBUG,
                        "  QOS set before accept\n"));
        }
      else if (QOS_IOCTL_SET_QOS == options.qosOptions.qosIoctlSet)
        {
          options.qosOptions.bDisableSignalling = TRUE;
          if (SetQos (&options.qosOptions,
                      TRUE,
                      &qos))
            ACE_DEBUG ((LM_DEBUG,
                        "  QOS set qos before accept - will be "
                        "set again in FD_QOS\n"));
        }
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Use the -q:default option to enable the QOS\n"),
                      -1);

  // Opening a new Multicast Datagram. It is absolutely necessary that
  // the sender and the receiver subscribe to the same multicast
  // addresses to make sure the "multicast sessions" for the two are
  // the same. This is used to match the RESV<->PATH states.
  ACE_SOCK_Dgram_Mcast dgram_mcast;

  // The windows example code uses PF_INET for the address family.
  // Winsock.h defines PF_INET to be AF_INET. Is there really a
  // difference between the protocol families and the address families
  // ? The following code internally uses AF_INET as a default for the
  // underlying socket.
  ACE_INET_Addr mult_addr (options.port,
                           options.szHostname);

  // Fill the ACE_QoS_Params to be passed to the <ACE_OS::join_leaf>
  // through subscribe.
  ACE_QoS_Params qos_params;
  FillQoSParams (qos_params,
                 0,
                 &qos);

  // The following call opens the Dgram_Mcast and calls the
  // <ACE_OS::join_leaf> with the qos_params supplied here.
  if (dgram_mcast.subscribe (mult_addr,
                             qos_params,
                             1,
                             0,
                             AF_INET,
                             FROM_PROTOCOL_INFO,
                             &protocol_info,
                             0,
                             WSA_FLAG_OVERLAPPED 
                             | WSA_FLAG_MULTIPOINT_C_LEAF 
                             | WSA_FLAG_MULTIPOINT_D_LEAF) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in subscribe\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Dgram_Mcast subscribe succeeds \n"));

  int nIP_TTL = 25;
  char achInBuf [BUFSIZ];
  u_long dwBytes;

  if (ACE_OS::ioctl (dgram_mcast.get_handle (), // Socket.
                     SIO_MULTICAST_SCOPE, // IO control code.
                     &nIP_TTL, // In buffer.
                     sizeof (nIP_TTL), // Length of in buffer.
                     achInBuf, // Out buffer.
                     BUFSIZ, // Length of Out buffer.
                     &dwBytes, // bytes returned.
                     0, // Overlapped.
                     0) == -1) // Func.
    ACE_ERROR ((LM_ERROR,
                "Error in Multicast scope ACE_OS::ioctl() \n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "Setting TTL with Multicast scope ACE_OS::ioctl call succeeds \n"));

  int bFlag = FALSE;

  if (ACE_OS::ioctl (dgram_mcast.get_handle (), // Socket.
                     SIO_MULTIPOINT_LOOPBACK, // IO control code.
                     &bFlag, // In buffer.
                     sizeof (bFlag), // Length of in buffer.
                     achInBuf, // Out buffer.
                     BUFSIZ, // Length of Out buffer.
                     &dwBytes, // bytes returned.
                     0, // Overlapped.
                     0) == -1) // Func.
    ACE_ERROR ((LM_ERROR,
                "Error in Loopback ACE_OS::ioctl() \n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "Disable Loopback with ACE_OS::ioctl call succeeds \n"));

  // Fill up an ACE_QoS and pass it to the overloaded ACE_OS::ioctl ()
  // that uses the I/O control code as SIO_SET_QOS.
  ACE_QoS ace_qos;

  // Make sure the flowspec is set in the correct direction. Since
  // this is the sender, sending flowspec is set to g711 and receiving
  // flowspec is set to no traffic.
  ACE_Flow_Spec sending_flowspec;
  ACE_Flow_Spec receiving_flowspec;
  const iovec iov = {0, 0};

  FillQoSTraffic (sending_flowspec);
  FillQoSNoTraffic (receiving_flowspec);

  ace_qos.sending_flowspec (sending_flowspec);     
  ace_qos.receiving_flowspec (receiving_flowspec);
  ace_qos.provider_specific (iov); 
  
  // Set the QOS according to the supplied ACE_QoS. The I/O control
  // code used under the hood is SIO_SET_QOS.
  if (ACE_OS::ioctl (dgram_mcast.get_handle (), // Socket.
	                 SIO_SET_QOS,
                     ace_qos, // ACE_QoS.
					 &dwBytes) == -1) // bytes returned.
    ACE_ERROR ((LM_ERROR,
                "Error in Qos set ACE_OS::ioctl() %d\n",
                dwBytes));					   	
  else
    ACE_DEBUG ((LM_DEBUG,
                "Setting Default QOS with ACE_OS::ioctl succeeds \n"));

  // Instantiate a QOS Event Handler and pass the Dgram_Mcast into it.
  ACE_QOS_Event_Handler qos_event_handler (dgram_mcast);
	
  // Register the QOS Handler with the Reactor for the QOS_MASK.  Note
  // the receiver registers for both QOS_MASK as well as READ_MASK.

  if (ACE_Reactor::instance ()->register_handler 
      (&qos_event_handler,
       ACE_Event_Handler::QOS_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in registering QOS Handler\n"),
                      -1);

  // Start the event loop.
  ACE_DEBUG ((LM_DEBUG,
              "Running the Event Loop ... \n"));

  ACE_Reactor::instance ()->run_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) shutting down Sender\n"));
  return 0;
}

int 
FindServiceProvider(int iProtocol,
                    int bQos,
                    int bMulticast,
                    ACE_Protocol_Info   *pProtocolInfo)
{

  ACE_Protocol_Info *protocol_buffer = 0;

  u_long buffer_length = 0;
  u_long dwErr;
  int bProtocolFound = FALSE;

  // first have enum_protocols () tell us how big a buffer is needed.
  int ret_val = ACE_OS::enum_protocols (0,
                                        protocol_buffer, 
                                        &buffer_length);
  if (ret_val != -1)
    ACE_DEBUG ((LM_DEBUG,
                "enum_protocols () : should not have suceeded\n"));

  else if (WSAENOBUFS != (dwErr = ACE_OS::set_errno_to_wsa_last_error ()))
    // enum_protocols () failed for some reason not relating to buffer size 
    ACE_DEBUG ((LM_DEBUG,
                "enum_protocols () : failed for a reason other than "
                "inadequate buffer size : %d\n",
                ACE_OS::set_errno_to_wsa_last_error ()));
  else
    {
      if (buffer_length > 0)
        {
          void *ptr = 0;
          ACE_NEW_RETURN (ptr,
                          char [buffer_length],
                          0);
          protocol_buffer = (ACE_Protocol_Info *) ptr;
        }

      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Buffer length returned by enum_protocols () is"
                           "less than or equal to zero\n"),
                          -1);

      if (protocol_buffer)
        {
          // now we can call enum_protocols () again with the
          // expectation it will succeed because we have allocated a
          // big enough buffer.
          ret_val = ACE_OS::enum_protocols (0, 
                                            protocol_buffer, 
                                            &buffer_length);
          if (ret_val == -1)
            ACE_DEBUG ((LM_DEBUG,
                        "enum_protocols () : failed even after allocating"
                        " a big enough buffer : %d\n",
                        ACE_OS::set_errno_to_wsa_last_error ()));
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Size allocated for the Protocol Buffer = %d\n"
                          "\nList of transport protocols "
                          "returned by enum_protocols () : \n",
                          buffer_length));
			  
              // Loop thru protocols, looking for a matching service
              // provider.
              for (int i = 0; i < ret_val; i++)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "  sp <%s>\n",
                              protocol_buffer[i].szProtocol));

                  if (AF_INET == protocol_buffer[i].iAddressFamily 
                      && iProtocol == protocol_buffer[i].iProtocol)
                    {
                      // look for 
                      if (bQos && bMulticast)
                        {
                          if ((XP1_QOS_SUPPORTED == ((XP1_QOS_SUPPORTED
                                                      & protocol_buffer[i].dwServiceFlags1)))
                              && (XP1_SUPPORT_MULTIPOINT == (XP1_SUPPORT_MULTIPOINT &
                                                             protocol_buffer[i].dwServiceFlags1)))
                            {
                              *pProtocolInfo = protocol_buffer[i];
                              bProtocolFound = TRUE;
                              break;
                            }
                        }
                      else if (bQos)
                        {
                          if (XP1_QOS_SUPPORTED == (XP1_QOS_SUPPORTED
                                                    & protocol_buffer[i].dwServiceFlags1))
                            {
                              *pProtocolInfo = protocol_buffer[i];
                              bProtocolFound = TRUE;
                              break;
                            } 
                        }
                      else if (bMulticast)
                        {
                          if ((XP1_SUPPORT_MULTIPOINT == (XP1_SUPPORT_MULTIPOINT &   
                                                          protocol_buffer[i].dwServiceFlags1)) 
                              && (XP1_QOS_SUPPORTED != (XP1_QOS_SUPPORTED & 
                                                        protocol_buffer[i].dwServiceFlags1)))
                            {
                              *pProtocolInfo = protocol_buffer[i];
                              bProtocolFound = TRUE;
                              break;
                            }
                        }
                      else if ((XP1_QOS_SUPPORTED != (XP1_QOS_SUPPORTED 
                                                      & protocol_buffer[i].dwServiceFlags1)))
                        {
                          *pProtocolInfo = protocol_buffer[i];
                          bProtocolFound = TRUE;
                          break;
                        }
                    } 

                } // for loop.
          
            } // ACE_OS::enum_protocols ().
      
          ACE_OS::free (protocol_buffer);
      
        } // protocol_buffer

    } // ACE_OS::enum_protocols ().

  if (bProtocolFound)
    ACE_DEBUG ((LM_DEBUG,
                "\n  Using service provider <%s>\n\n", 
                pProtocolInfo->szProtocol));

  return bProtocolFound;

}

int 
ValidOptions (char *argv[],
              int argc,
              OPTIONS *pOptions)
{
  int bValidOptions = TRUE;
  *pOptions = default_options;

  for (int i = 1; i < argc; i++)
    {
      if (argv[i][0] == '-' || argv[i][0] == '/')
        {
          switch (ACE_OS::to_lower (argv[i][1])) 
            {
            case 'b' :
              if (ACE_OS::strlen (argv[i]) > 3)
                pOptions->nBufSize = 1024 * ACE_OS::atoi (&argv[i][3]);
              break;

            case 'c' :
              pOptions->qosOptions.bAlternateQos = TRUE;
              break;

            case 'd' :
              if (ACE_OS::strlen (argv[i]) > 3)
                pOptions->fillchar = argv[i][3];
              break;

            case 'e' :
              if (ACE_OS::strlen (argv[i]) > 3)
                pOptions->port = ACE_OS::atoi (&argv[i][3]);
              break;

            case 'i' :
              if (ACE_OS::strlen (argv[i]) > 3)
                {
                  if ('a' == argv[i][3] || 'A' == argv[i][3])
                    pOptions->qosOptions.qosIoctlSet = QOS_IOCTL_SET_AFTER;
                  else if ('b' == argv[i][3] || 'B' == argv[i][3])
                    pOptions->qosOptions.qosIoctlSet = QOS_IOCTL_SET_BEFORE;
                  else if ('d' == argv[i][3] || 'D' == argv[i][3])
                    pOptions->qosOptions.qosIoctlSet = QOS_IOCTL_SET_DURING;
                  else if ('q' == argv[i][3] || 'Q' == argv[i][3])
                    pOptions->qosOptions.qosIoctlSet = QOS_IOCTL_SET_QOS;
                  else
                    pOptions->qosOptions.qosIoctlSet = QOS_IOCTL_SET_BEFORE;
                }
              break;

            case 'l' :
              if (ACE_OS::strlen (argv[i]) > 3)
                pOptions->nRepeat = ACE_OS::atoi (&argv[i][3]);
              break;

            case 'm' :
              pOptions->spOptions.bMulticast = TRUE;
              if (ACE_OS::strlen (argv[i]) > 3)
                ACE_OS::strcpy (pOptions->szHostname,
                                &argv[i][3]);
              else
                ACE_OS::strcpy (pOptions->szHostname,
                                DEFAULT_MULTICASTGROUP);
              break;

            case 'n' : 
              pOptions->qosOptions.bReceiver = FALSE; 
              // multicast group overrides hostname on -n
              if (!pOptions->spOptions.bMulticast
                  && ACE_OS::strlen (argv[i]) > 3)
                ACE_OS::strcpy (pOptions->szHostname,
                                &argv[i][3]);
              break;

            case 'p' :
              if (ACE_OS::strlen (argv[i]) > 3)
                {
                  if ('u' == argv[i][3] || 'U' == argv[i][3])
                    pOptions->spOptions.iProtocol = IPPROTO_UDP;
                  else if ('t' == argv[i][3] || 'T' == argv[i][3])
                    pOptions->spOptions.iProtocol = IPPROTO_TCP;
                  else
                    pOptions->spOptions.iProtocol = IPPROTO_TCP;
                }
              break;

            case 'q' : 
              pOptions->spOptions.bQos = TRUE; 
              if (ACE_OS::strlen (argv[i]) > 3)
                ACE_OS::strcpy (pOptions->qosOptions.szTemplate,
                                &argv[i][3]);
              break;

            case 'r' :
              if (ACE_OS::strlen (argv[i]) > 3)
                {
                  if (ACE_OS::strcasecmp (argv[i],
                                          "-rsvp-confirm") == 0
                      || ACE_OS::strcasecmp (argv[i],
                                             "-rsvp_confirm") == 0)
                    {
                      pOptions->qosOptions.bConfirmResv = TRUE;
                      pOptions->qosOptions.bProviderSpecific = TRUE;
                    }
                  else if (ACE_OS::strcasecmp (argv[i],
                                               "-rsvp-wait") == 0
                           || ACE_OS::strcasecmp (argv[i],
                                                  "-rsvp_wait") == 0)
                    {
                      pOptions->qosOptions.bWaitToSend = TRUE;
                      pOptions->qosOptions.bProviderSpecific = TRUE;
                    }
                  else
                    ACE_DEBUG ((LM_DEBUG,
                                "Ignoring option <%s>\n", 
                                argv[i]));
                }
              break;

            case 's' :
              if (ACE_OS::strlen (argv[i]) > 3)
                pOptions->dwSleep =
                  ACE_OS::atoi (&argv[i][3]);
              break;

            case '?' :
              Usage (argv[0],
                     &default_options);
              return FALSE;
              break;

            default:
              ACE_DEBUG ((LM_DEBUG,
                          "  unknown options flag %s\n", 
                          argv[i]));
              Usage (argv[0],
                     &default_options);
              return FALSE;
              break;
            }
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "  unknown option %s\n", 
                      argv[i]));
          Usage (argv[0],
                 &default_options);
          return FALSE;
        }
    }


  if (pOptions->qosOptions.bReceiver)
    pOptions->fillchar = 0;

  if (pOptions->spOptions.bMulticast)
    {
      pOptions->spOptions.iProtocol = IPPROTO_UDP;
      pOptions->qosOptions.bSetDestaddr = FALSE;
    }

  if (IPPROTO_TCP == pOptions->spOptions.iProtocol)
    {
      pOptions->spOptions.bMulticast = FALSE;
      pOptions->qosOptions.bSetDestaddr = FALSE;
    }
  else if (!pOptions->spOptions.bMulticast)
    pOptions->qosOptions.bSetDestaddr = TRUE;

  if (pOptions->spOptions.bQos)
    {
      if (pOptions->qosOptions.bReceiver 
          && pOptions->qosOptions.bWaitToSend)
        pOptions->qosOptions.bWaitToSend = FALSE;  
      // not a valid receiver option

      if (!pOptions->qosOptions.bReceiver 
          && pOptions->qosOptions.bConfirmResv)
        pOptions->qosOptions.bWaitToSend = FALSE;  
      // not a valid sender option

      if (pOptions->qosOptions.bAlternateQos)
        // override repeat count to continuous mode
        pOptions->nRepeat = 0;					   

      if (IPPROTO_UDP == pOptions->spOptions.iProtocol
          && !pOptions->spOptions.bMulticast)
        // Using UDP, there WSAAccept will not be called, therefore
        // do not wait to set qos.
        pOptions->qosOptions.qosIoctlSet =
          QOS_IOCTL_SET_BEFORE;

      pOptions->qosOptions.bFineGrainErrorAvail = TRUE;
      pOptions->qosOptions.bQosabilityIoctls = TRUE;
      ACE_DEBUG ((LM_DEBUG,
                  "running on NT\n"));
    }

  if (pOptions->nBufSize > 0)
    ACE_NEW_RETURN (pOptions->buf,
                    char[pOptions->nBufSize],
                    0);
  else 
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Buffer size to be allocated is less than or"
                       "equal to zero\n"),
                      -1);

  if (pOptions->buf == 0)
    return FALSE;
  else
    {      
      ACE_OS::memset (pOptions->buf,
                      pOptions->fillchar,
                      pOptions->nBufSize);
      return TRUE;
    }
}

static 
void PrintOptions (OPTIONS *pOptions)
{
  ACE_DEBUG ((LM_DEBUG,
              "Options\n"
              "  Protocol %d\n"
              "  Port %d\n",
              pOptions->spOptions.iProtocol,
              pOptions->port));

  if (pOptions->qosOptions.bReceiver)
    ACE_DEBUG ((LM_DEBUG,
                "  Act as Receiver\n"));
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "  Act as sender and send to %s\n"
                  "  Sleep %d milliseconds between sends\n"
                  "  Fill buffer with <%c>\n", 
                  pOptions->szHostname,
                  pOptions->dwSleep,
                  pOptions->fillchar));

      if (pOptions->nRepeat)
        ACE_DEBUG ((LM_DEBUG,
                    "  Repeat sending the buffer %d times\n", 
                    pOptions->nRepeat));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "  Repeat sending the buffer continually\n"));

    }
  ACE_DEBUG ((LM_DEBUG,
              "  Bufsize %d (1K increments)\n"
              "  Multicast is %s\n", 
              pOptions->nBufSize,
              (pOptions->spOptions.bMulticast ? "Enabled" : "Disabled")));

  if (pOptions->spOptions.bQos)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "  Qos template %s\n"
                  "  Qos to be set ", 
                  pOptions->qosOptions.szTemplate));
      
      switch (pOptions->qosOptions.qosIoctlSet)
        {
        case QOS_IOCTL_SET_BEFORE: 
          ACE_DEBUG ((LM_DEBUG,
                      "Before accept/connect/joinleaf\n"));
          break;
        case QOS_IOCTL_SET_AFTER:  
          ACE_DEBUG ((LM_DEBUG,
                      "After accept/connect/joinleaf\n")); 
          break;
        case QOS_IOCTL_SET_DURING: 
          ACE_DEBUG ((LM_DEBUG,
                      "During accept/connect/joinleaf\n"));
          break;
        case QOS_IOCTL_SET_QOS: 
          if (pOptions->qosOptions.bReceiver)
            ACE_DEBUG ((LM_DEBUG,
                        "During FD_QOS, implies QOS with no "
                        "signaling set before accept/connect "
                        "(eventually)\n"));
          else
            {
              pOptions->qosOptions.qosIoctlSet = QOS_IOCTL_SET_BEFORE;
              ACE_DEBUG ((LM_DEBUG,
                          "Before connect\n"));
            }
          break;
        }

      if (pOptions->qosOptions.bConfirmResv )
        ACE_DEBUG ((LM_DEBUG,
                    "  RESV confirmation to be requested\n"));

      if (INVALID_SEND_PRIORITY != pOptions->qosOptions.SendPriority)
        ACE_DEBUG ((LM_DEBUG,
                    "  Qos TC SendPriority to be set to %d\n", 
                    pOptions->qosOptions.SendPriority));
      if (!pOptions->qosOptions.bReceiver)
        {
          if (pOptions->qosOptions.bWaitToSend)
            ACE_DEBUG ((LM_DEBUG,
                        "  Wait for RESV before sending data\n"));
          else
            ACE_DEBUG ((LM_DEBUG,
                        "  Do not wait for RESV to start sending data\n"));
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    "  Qos must be set during Accept = %s\n", 
                    (pOptions->qosOptions.bMustSetQosInAccept?"TRUE":"FALSE")));
           
      ACE_DEBUG ((LM_DEBUG,
                  "  Query for QOS buffer size: %s\n\n",
                  (pOptions->qosOptions.bQueryBufferSize?"TRUE":"FALSE")));
    }
  return;
}

// Print out usage table for the program.

void 
Usage (char *szProgramname,
       OPTIONS *pOptions)
{
  ACE_DEBUG ((LM_DEBUG,
              "usage:\n  %s -?\n\n"
              "  %s [-b:#] [-d:c] [-e:#] [-l:#] [-m:group] "
              "[-n:host] [-s:#] [-u]\n\t[-q:template [-i:[a|b|d|q]]"
              "[-c] [-rsvp-confirm] [-rsvp-wait]]\n\n",
              "  -?\t\tDisplay this help\n\n"
              "  -b:bufsize\tSize of send/recv buffer; in 1K increments (Def:%d)\n"
              "  -d:c\t\tCharacter used to fill buffer (Def:%c)\n"
              "  -e:port\tEndpoint number (port) to use (Def:%d)\n"
              "  -l:loop\tLoop count for sending buffer (0==>continuous)\n"
              "  -m:group\tMulticast group (IP) to join (Def:%s)\n"
              "  -n:host\tAct as the client and connect to 'host' (Def:%s)\n"
              "  -p:protocol\tTCP or UDP (def:TCP)\n"
              "  -s:#\t\tSleep # milliseconds between sends (def: 0)\n"
              "  -q:template\tEnable QoS and use 'template' to specify the name (Def:%s)\n"
              "  -c\t\tCycle enabling/disabling QOS on sending socket (Def: no cycling)\n"
              "  -i:[a|b|d|q]\tSet Qos After, Before, During accept/connect, or during FD_QOS\n\t\t\t(def: Before)\n"
              "  -rsvp-confirm\t\tRequest RESV confirmation be sent (Def: no confirmation)\n"
              "  -rsvp-wait\t\tWait for RESV from receiver before sending data\n",
              szProgramname,
              szProgramname,
              pOptions->nBufSize,
              pOptions->fillchar,
              pOptions->port,
              pOptions->nRepeat,
              DEFAULT_MULTICASTGROUP,
              pOptions->szHostname,
              pOptions->qosOptions.szTemplate));
  return;
}
