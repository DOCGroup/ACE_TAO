/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    server.cpp
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#define QOSEVENT_MAIN

#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/OS.h"
#include "ace/QoS_Session_Factory.h"
#include "ace/QoS_Session.h"

#include "QosEvent.h"
#include "Receiver_QOS_Event_Handler.h"
    
static int ValidOptions (char *argv[],
                         int argc,
                         OPTIONS *pOptions);
static void PrintOptions (OPTIONS *pOptions);
static void Usage (char *szProgramname,
                   OPTIONS *pOptions);

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

// To open QOS sockets administrative access is required on the
// machine.  Fill in default values for QoS structure.  The default
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
                                  ACE_SERVICETYPE_GUARANTEED, 
                                  // SERVICETYPE_CONTROLLEDLOAD  
                                  // doesnt work here for a yet unknown
                                  // reason.
                                  368,
                                  368,
                                  25, // TTL. ACE specific. Not used on NT.
                                  1	// Priority. ACE specific. Not used on NT.
                                  );
	
  ACE_Flow_Spec ace_default_notraffic (ACE_QOS_NOT_SPECIFIED,
                                       ACE_QOS_NOT_SPECIFIED,
                                       ACE_QOS_NOT_SPECIFIED,
                                       ACE_QOS_NOT_SPECIFIED,
                                       ACE_QOS_NOT_SPECIFIED,
                                       ACE_SERVICETYPE_NOTRAFFIC,
                                       ACE_QOS_NOT_SPECIFIED,
                                       ACE_QOS_NOT_SPECIFIED,
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

// Fill up the ACE_Flow_Spec with the default_g711 values
// as defined in the QoSEvent.h

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

// This function fills up the ACE_QoS_Params with the supplied iovec and ACE_QoS.

int
FillQoSParams (ACE_QoS_Params &qos_params,
               iovec* iov, 
               ACE_QoS* qos)
{
  qos_params.callee_data (iov);
  qos_params.caller_data (0);
  qos_params.socket_qos (qos);
  qos_params.group_socket_qos (0);
  qos_params.flags (ACE_JL_BOTH);

  return 0;
}

int
main (int argc, char * argv[])
{  
  ACE_Protocol_Info protocol_info, *pinfo=0;

  OPTIONS options;

  if (!ValidOptions (argv,
                     argc,
                     &options))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in ValidOptions\n"),
                      -1);
 
  // XX pinfo = &protocol_info;
  // XX FindServiceProvider is not needed by RSVP
  // XX if (FindServiceProvider (options.spOptions.iProtocol,
  // XX                          options.spOptions.bQos,
  // XX                          options.spOptions.bMulticast,
  // XX                          pinfo) == FALSE)
  // XX   ACE_ERROR_RETURN ((LM_ERROR,
  // XX                      "Error in FindServiceProvider\n"),
  // XX                     -1);
	
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

  // Opening a new Multicast Datagram.
  ACE_SOCK_Dgram_Mcast dgram_mcast;

  // The windows example code uses PF_INET for the address family.
  // Winsock.h defines PF_INET to be AF_INET. The following
  // code internally uses AF_INET as a default for the underlying socket.

  ACE_INET_Addr mult_addr (options.port,
                           options.szHostname);

  // Fill the ACE_QoS_Params to be passed to the <ACE_OS::join_leaf>
  // through subscribe.

  ACE_QoS_Params qos_params;
  FillQoSParams (qos_params, 0, &qos);

  // Create a QoS Session Factory.
  ACE_QoS_Session_Factory session_factory;

  // Ask the factory to create a QoS session. This could be RAPI or
  // GQoS based on the parameter passed.
  ACE_QoS_Session *qos_session = 
    session_factory.create_session (ACE_QoS_Session_Factory::ACE_RAPI_SESSION);
// XX Shouldn't have to specify GQOS or RAPI?!?
// XX it is not clear that we need to pass in a key indicating the type
// XX of object to create.  Since we use RAPI flag at compile time can
// XX we assume rapi here also?  Or could we have RAPI and GQoS?

  // Create a destination address for the QoS session. The same
  // address should be used for the subscribe call later. A copy is
  // made below only to distinguish the two usages of the dest
  // address.

  ACE_INET_Addr dest_addr (mult_addr);

  // A QoS session is defined by the 3-tuple [DestAddr, DestPort,
  // Protocol]. Initialize the QoS session.
  if (qos_session->open (mult_addr,
                         IPPROTO_UDP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in opening the QoS session\n"),
                      -1);

  // The following call opens the Dgram_Mcast and calls the
  // <ACE_OS::join_leaf> with the qos_params supplied here. Note the
  // QoS session object is passed into this call. This subscribes the
  // underlying socket to the passed in QoS session. For joining
  // multiple multicast sessions, the following subscribe call should
  // be made with different multicast addresses and a new QoS session
  // object should be passed in for each such call. The QoS session
  // objects can be created only through the session factory. Care
  // should be taken that the mult_addr for the subscribe() call
  // matches the dest_addr of the QoS session object. If this is not
  // done, the subscribe call will fail. A more abstract version of
  // subscribe will be added that constrains the various features of
  // GQoS like different flags etc.

  if (dgram_mcast.subscribe (mult_addr,
                             qos_params,
                             1,
                             0,
                             AF_INET,
                             // ACE_FROM_PROTOCOL_INFO,
                             0,
                             pinfo,
                             0,
                             ACE_OVERLAPPED_SOCKET_FLAG 
                             | ACE_FLAG_MULTIPOINT_C_LEAF 
                             | ACE_FLAG_MULTIPOINT_D_LEAF,
                             qos_session) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in subscribe\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Dgram_Mcast subscribe succeeds \n"));
			
  int nIP_TTL = 25;
  char achInBuf [BUFSIZ];
  u_long dwBytes;

  // Should this be abstracted into QoS objects ?? Doesnt seem to have
  // to do anything directly with QoS.
  if (ACE_OS::ioctl (dgram_mcast.get_handle (), // Socket.
                     ACE_SIO_MULTICAST_SCOPE, // IO control code.
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

  // Should this be abstracted into QoS objects ?? Doesnt seem to have
  // to do anything directly with QoS.
  if (ACE_OS::ioctl (dgram_mcast.get_handle (), // Socket.
                     ACE_SIO_MULTIPOINT_LOOPBACK, // IO control code.
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

  // Make sure the flowspec is set in the correct direction for the
  // sender/client.
  ACE_Flow_Spec sending_flowspec;
  ACE_Flow_Spec receiving_flowspec;
  const iovec iov = {0, 0};

  FillQoSTraffic (receiving_flowspec);
  FillQoSNoTraffic (sending_flowspec);

  ace_qos.sending_flowspec (sending_flowspec);     
  ace_qos.receiving_flowspec (receiving_flowspec);
  ace_qos.provider_specific (iov); 
  
  // Set the QoS for the session. Replaces the ioctl () call that was
  // being made previously.
  if (qos_session->qos (&dgram_mcast,
                        ace_qos) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to set QoS\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Setting QOS succeeds.\n"));

  // Instantiate a QOS Event Handler and pass the Dgram_Mcast and QoS
  // session object into it.
  ACE_QOS_Event_Handler qos_event_handler (dgram_mcast,
                                           qos_session);
	
  // Register the QOS Handler with the Reactor.
  if (ACE_Reactor::instance ()->register_handler 
      (&qos_event_handler,
       ACE_Event_Handler::QOS_MASK | ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in registering QOS Handler\n"),
                      -1);

  // Start the event loop.
  ACE_DEBUG ((LM_DEBUG,
              "Running the Event Loop ... \n"));

  ACE_Reactor::instance ()->run_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) shutting down server logging daemon\n"));
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

  else if (ACE_ENOBUFS != (dwErr = ACE_OS::set_errno_to_wsa_last_error ()))
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
                          if ((ACE_XP1_QOS_SUPPORTED == ((ACE_XP1_QOS_SUPPORTED
                                                          & protocol_buffer[i].dwServiceFlags1)))
                              && (ACE_XP1_SUPPORT_MULTIPOINT == (ACE_XP1_SUPPORT_MULTIPOINT &
                                                                 protocol_buffer[i].dwServiceFlags1)))
                            {
                              *pProtocolInfo = protocol_buffer[i];
                              bProtocolFound = TRUE;
                              break;
                            }
                        }
                      else if (bQos)
                        {
                          if (ACE_XP1_QOS_SUPPORTED == (ACE_XP1_QOS_SUPPORTED
                                                        & protocol_buffer[i].dwServiceFlags1))
                            {
                              *pProtocolInfo = protocol_buffer[i];
                              bProtocolFound = TRUE;
                              break;
                            } 
                        }
                      else if (bMulticast)
                        {
                          if ((ACE_XP1_SUPPORT_MULTIPOINT == (ACE_XP1_SUPPORT_MULTIPOINT &   
                                                              protocol_buffer[i].dwServiceFlags1)) 
                              && (ACE_XP1_QOS_SUPPORTED != (ACE_XP1_QOS_SUPPORTED & 
                                                            protocol_buffer[i].dwServiceFlags1)))
                            {
                              *pProtocolInfo = protocol_buffer[i];
                              bProtocolFound = TRUE;
                              break;
                            }
                        }
                      else if ((ACE_XP1_QOS_SUPPORTED != (ACE_XP1_QOS_SUPPORTED 
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
      if ((argv[i][0] == '-') || (argv[i][0] == '/') ) 
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
              if (!pOptions->spOptions.bMulticast)
                {
                  if (ACE_OS::strlen (argv[i]) > 3)
                    ACE_OS::strcpy (pOptions->szHostname,
                                    &argv[i][3]);
                }
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
                  "running on XX\n"));
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

//      Print out usage table for the program
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
