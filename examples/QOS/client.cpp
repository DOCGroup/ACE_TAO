// $Id$

#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/OS.h"

#define QOSEVENT_MAIN
#include "qosevent.h"
#include "Sender_QOS_Event_Handler.h"
    
static u_short SERVER_PORT = MY_DEFPORT;
static const char *const SERVER_HOST = DEFAULT_MULTICASTGROUP;
static const int MAX_ITERATIONS = 4;

static BOOL ValidOptions(char *argv[], int argc, OPTIONS *pOptions);
static VOID PrintOptions(OPTIONS *pOptions);
static VOID Usage(CHAR *szProgramname, OPTIONS *pOptions);

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

// To open QOS sockets administrative access is required on the machine.
 
// 
//      Fill in default values for QoS structure.  The default values were
//      simply choosen from existing QOS templates available via WSAGetQosByName.
//      Notice that ProviderSpecific settings are being allowed when picking the 
//      "default" template but not for "well-known" QOS templates.  Also notice 
//      that since data is only flowing from sender to receiver, different 
//      flowspecs are filled in depending upon whether this application is 
//      acting as a sender or receiver.
// 
static VOID FillQosFlowspecDefault(
                                   QOS             *pQos, 
                                   DWORD           *cbQosLen, 
                                   QOS_OPTIONS     *pQosOptions
                                   )
{
  if (pQosOptions->bReceiver)
    {
      pQos->ReceivingFlowspec = default_g711;
      pQos->SendingFlowspec   = default_notraffic;        
    }
  else
    {
      pQos->SendingFlowspec   = default_g711;     
      pQos->ReceivingFlowspec = default_notraffic;
    }

	
  ACE_DEBUG ((LM_DEBUG,
              "Enabling the QOS Signalling for the sender. \n"));

  // Enable the QOS signalling.
  pQos->SendingFlowspec.ServiceType &= ~(SERVICE_NO_QOS_SIGNALING);

  pQos->ProviderSpecific.len = 0;
  pQos->ProviderSpecific.buf = NULL;
  *cbQosLen = sizeof(QOS) + pQos->ProviderSpecific.len;

  return;
}


static VOID 
fill_ace_qos_flowspec_default (ACE_QoS *pQos,
                               DWORD *cbQosLen,
                               QOS_OPTIONS *pQosOptions)
{
  ACE_Flow_Spec ace_default_g711 (
                                  9200,
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

  const iovec iov = {0, NULL};

  pQos->provider_specific (iov);
  *cbQosLen = sizeof(QOS) + pQos->provider_specific ().iov_len;

  return;
}

BOOL SetQos(
            ACE_HANDLE      sd, 
            QOS_OPTIONS     *pQosOptions,
            BOOL            bSetQos,
            ACE_QoS         *pQos, 
            DWORD           *cbQosLen 
            )
{
    
  BOOL bError = FALSE;

  // fill in default values
  if (stricmp(pQosOptions->szTemplate, QOS_DEFAULTNAME) == 0)
    {
      fill_ace_qos_flowspec_default(pQos, cbQosLen, pQosOptions);
    }
  else 
    {       

      ACE_DEBUG ((LM_DEBUG,
                  "Run the program with -q:default option\n"));
    }

  return(!bError);
}

int 
FillQoSTraffic (ACE_Flow_Spec &afc)
{
  afc.token_rate (default_g711.TokenRate);                    /* In Bytes/sec */
  afc.token_bucket_size (default_g711.TokenBucketSize);       /* In Bytes */
  afc.peak_bandwidth (default_g711.PeakBandwidth);            /* In Bytes/sec */
  afc.latency (default_g711.Latency);					        /* In microseconds */
  afc.delay_variation (default_g711.DelayVariation);          /* In microseconds */
  afc.service_type (default_g711.ServiceType);
  afc.max_sdu_size (default_g711.MaxSduSize);                 /* In Bytes */
  afc.minimum_policed_size (default_g711.MinimumPolicedSize); /* In Bytes */

  return 0;
}

int
FillQoSNoTraffic (ACE_Flow_Spec &afc)
{
  afc.token_rate (default_notraffic.TokenRate);                    /* In Bytes/sec */
  afc.token_bucket_size (default_notraffic.TokenBucketSize);       /* In Bytes */
  afc.peak_bandwidth (default_notraffic.PeakBandwidth);            /* In Bytes/sec */
  afc.latency (default_notraffic.Latency);					     /* In microseconds */
  afc.delay_variation (default_notraffic.DelayVariation);          /* In microseconds */
  afc.service_type (default_notraffic.ServiceType);
  afc.max_sdu_size (default_notraffic.MaxSduSize);                 /* In Bytes */
  afc.minimum_policed_size (default_notraffic.MinimumPolicedSize); /* In Bytes */

  return 0;
}

int
FillQoSParams (ACE_QoS_Params &qos_params,
               iovec* iov, 
               ACE_QoS* qos)
{
  //qos_params.callee_data (iov);
  qos_params.callee_data (NULL);
  qos_params.caller_data (NULL);
  qos_params.socket_qos (qos);
  qos_params.group_socket_qos (NULL);
  qos_params.flags (JL_SENDER_ONLY);

  return 0;
}

int
main (int argc, char * argv[])
{
  WSADATA WSAData;
  ACE_Protocol_Info protocol_info;

  OPTIONS options;
  if (!ValidOptions(argv, argc, &options))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Error in ValidOptions\n"),
                        -1);
    }
	
  if (WSAStartup ( MAKEWORD(2,2), &WSAData) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in WSAStartup\n"),
                      -1);

  if (FindServiceProvider (options.spOptions.iProtocol,
                           options.spOptions.bQos,
                           options.spOptions.bMulticast,
                           &protocol_info) == FALSE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in FindServiceProvider\n"),
                      -1);
	
#define MAXCLIENTS 4

  // encapsulate socket, events, buffers, and various flags and counters that
  // control how/when data is received into a structure
  typedef struct _RECV_INFO
  {
    ACE_HANDLE      sd[MAXCLIENTS+1];
    WSAEVENT        hEvents[MAXCLIENTS+1];
    CHAR *          recvbuf;
    int             nTotalClients;
  } RECV_INFO;

  RECV_INFO RecvInfo;

  printf("Sender\n");

  // initialize data structures
  for (int i=0; i<MAXCLIENTS+1; i++)
    RecvInfo.sd[i] = ACE_INVALID_HANDLE;   

  ACE_QoS     qos;
  QOS         *pQos;
  DWORD       cbQosLen;  
	
  pQos = NULL;
  if (options.spOptions.bQos)
    {
      if (QOS_IOCTL_SET_BEFORE == options.qosOptions.qosIoctlSet)
        {
          if (SetQos(RecvInfo.sd[0], &options.qosOptions, TRUE, &qos, &cbQosLen))
            printf("  QOS set before accept\n");
          pQos = NULL;
        }
      else if (QOS_IOCTL_SET_QOS == options.qosOptions.qosIoctlSet)
        {
          // this one is tricky.  The intent is to send the RESV after 
          // receiving FD_QOS notification.  However, unless we somehow 
          // inform the GQOSSP that we are interested in QOS, it will not
          // send us FD_QOS when receiving a RESV.  However, we do not want to
          // send out a RESV until we explicitedly set QOS.  We therefore do not 
          // want QOS signalling (i.e. RSVP messages) to be sent.
          options.qosOptions.bDisableSignalling = TRUE;
          if (SetQos(RecvInfo.sd[0], &options.qosOptions, TRUE, &qos, &cbQosLen))
            {
              printf("  QOS set qos before accept - will be set again in FD_QOS\n");
              pQos = &qos;
            }
          else
            pQos = NULL;
        }
    }
  else
    pQos = NULL;
	
  // Opeing a new Multicast Datagram with the QoS socket opened previously.

  ACE_SOCK_Dgram_Mcast dgram_mcast;

  // It is absolutely necessary that the sender and the receiver subscribe 
  // to the same multicast addresses to make sure the "multicast sessions"
  // for the two are the same. This is used to match the RESV<->PATH states.

  SOCKADDR_IN addr;

  ZeroMemory((PVOID)&addr, sizeof(addr));
  addr.sin_family      = PF_INET;						//protocol_info.iAddressFamily;
  addr.sin_addr.s_addr = inet_addr("234.5.6.7");		//options.szHostname);
  addr.sin_port        = htons(5001);					//options.port);

  ACE_INET_Addr mult_addr (&addr, sizeof (addr));

  // Fill the ACE_QoS_Params to be passed to the ACE_OS::join_leaf () through subscribe.
  ACE_QoS_Params qos_params;
  FillQoSParams (qos_params, NULL, &qos);

  // Join the multicast group if specfied.  We may also specify a given QOS structure
  // at this time.

  if (dgram_mcast.subscribe (mult_addr,
                             qos_params,
                             1,
                             0,
                             PF_INET,
                             FROM_PROTOCOL_INFO,
                             &protocol_info,
                             0,
                             WSA_FLAG_OVERLAPPED | WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF
                             ) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in subscribe\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Dgram_Mcast subscribe succeeds \n"));
			

  int nIP_TTL = 25;
  char achInBuf [BUFSIZ];
  DWORD dwBytes;

  // Set IP TTL 
  if (ACE_OS::ioctl (dgram_mcast.get_handle (),                   // Socket.
                     SIO_MULTICAST_SCOPE,						    // IO control code.
                     &nIP_TTL,									// In buffer.
                     sizeof (nIP_TTL),							// Length of in buffer.
                     achInBuf,									// Out buffer.
                     BUFSIZ,										// Length of Out buffer.
                     &dwBytes,									// bytes returned.
                     NULL,										// Overlapped.
                     NULL) == SOCKET_ERROR)						// Func.
    ACE_ERROR ((LM_ERROR,
                "Error in Multicast scope ACE_OS::ioctl() \n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "Setting TTL with Multicast scope ACE_OS::ioctl call succeeds \n"));

  BOOL bFlag = FALSE;

  // Disable LoopBack.
  if (ACE_OS::ioctl (dgram_mcast.get_handle (),                   // Socket.
                     SIO_MULTIPOINT_LOOPBACK,					    // IO control code.
                     &bFlag,										// In buffer.
                     sizeof (bFlag),								// Length of in buffer.
                     achInBuf,									// Out buffer.
                     BUFSIZ,										// Length of Out buffer.
                     &dwBytes,									// bytes returned.
                     NULL,										// Overlapped.
                     NULL) == SOCKET_ERROR)						// Func.
    ACE_ERROR ((LM_ERROR,
                "Error in Loopback ACE_OS::ioctl() \n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "Disable Loopback with ACE_OS::ioctl call succeeds \n"));
		

  // Enable QoS on the already opened QoS enabled socket.
		
  QOS win_qos;
  FillQosFlowspecDefault (&win_qos, &cbQosLen, &options.qosOptions);		

  if (ACE_OS::ioctl (dgram_mcast.get_handle (),                   // Socket.
                     SIO_SET_QOS,								    // IO control code.						   						   	
                     (LPVOID) &win_qos,							// In buffer.
                     cbQosLen,									// Length of in buffer.
                     NULL,
                     0,
                     &dwBytes,									// bytes returned.
                     NULL,										// Overlapped.
                     NULL) == SOCKET_ERROR)						// Func.
    ACE_ERROR ((LM_ERROR,
                "Error in Qos set ACE_OS::ioctl() %d\n",
                dwBytes));
					   	
  else
    ACE_DEBUG ((LM_DEBUG,
                "Setting Default QOS with ACE_OS::ioctl succeeds \n"));

  // Instantiate a QOS Event Handler and pass the Dgram_Mcast into it.
  ACE_QOS_Event_Handler qos_event_handler (dgram_mcast);
	
  // Register the QOS Handler with the Reactor for the QOS_MASK.
  // Note the receiver registers for both QOS_MASK as well as READ_MASK.

  if (ACE_Reactor::instance ()->register_handler (&qos_event_handler,
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


BOOL FindServiceProvider(
                         int                 iProtocol,
                         BOOL                bQos,
                         BOOL                bMulticast,
                         ACE_Protocol_Info   *pProtocolInfo
                         )
{
  ACE_Protocol_Info *protocol_buffer = NULL;
  u_long buffer_length = 0;
  DWORD dwErr;
  BOOL bProtocolFound = FALSE;

  // first have enum_protocols () tell us how big a buffer is needed.
  int ret_val = ACE_OS::enum_protocols(NULL, 
                                       protocol_buffer, 
                                       &buffer_length);
  if (ret_val != -1)
    {
      printf("WSAEnumProtocols: should not have suceeded\n");
    }
  else if (WSAENOBUFS != (dwErr = WSAGetLastError()))
    {
      // WSAEnumProtocols failed for some reason not relating to buffer size 
      printf("WSAEnumProtocols(1): %d\n", WSAGetLastError());
    }
  else
    {
      // WSAEnumProtocols failed for the "expected" reason, allocate a buffer
      // of the needed size
      protocol_buffer = (ACE_Protocol_Info *)malloc(buffer_length);
      if (protocol_buffer)
        {
          // now we can call WSAEnumProtocols again with the expectation it will
          // succeed because we have allocated a big enough buffer.
          ret_val = ACE_OS::enum_protocols (NULL, 
                                            protocol_buffer, 
                                            &buffer_length);
          if (ret_val == -1)
            {
              printf("WSAEnumProtocols(3): %d\n", WSAGetLastError());
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "BUFFER LENGTh = %d",
                          buffer_length));
			  
              // loop thru protocols, looking for a matching service provider 
              for (int i = 0; i < ret_val; i++)
                {
                  printf("  sp <%s>\n", protocol_buffer[i].szProtocol);
                  if (AF_INET == protocol_buffer[i].iAddressFamily &&
                      iProtocol == protocol_buffer[i].iProtocol)
                    {
                      // look for 
                      if (bQos && bMulticast)
                        {
                          if ((XP1_QOS_SUPPORTED == (XP1_QOS_SUPPORTED &
                                                     protocol_buffer[i].dwServiceFlags1)) && 
                              (XP1_SUPPORT_MULTIPOINT == (XP1_SUPPORT_MULTIPOINT & protocol_buffer[i].dwServiceFlags1)))
                            {
                              *pProtocolInfo = protocol_buffer[i];
                              bProtocolFound = TRUE;
                              break;
                            }
                        }
                      else if (bQos)
                        {
                          if ((XP1_QOS_SUPPORTED == (XP1_QOS_SUPPORTED &                                                 
                                                     protocol_buffer[i].dwServiceFlags1)))
                            {
                              *pProtocolInfo = protocol_buffer[i];
                              bProtocolFound = TRUE;
                              break;
                            } 
                        }
                      else if (bMulticast)
                        {
                          if ((XP1_SUPPORT_MULTIPOINT == (XP1_SUPPORT_MULTIPOINT &   
                                                          protocol_buffer[i].dwServiceFlags1)) &&
                              (XP1_QOS_SUPPORTED != (XP1_QOS_SUPPORTED & 
                                                     protocol_buffer[i].dwServiceFlags1)))
                            {
                              *pProtocolInfo = protocol_buffer[i];
                              bProtocolFound = TRUE;
                              break;
                            }
                        }
                      else 
                        {
                          if ((XP1_QOS_SUPPORTED != (XP1_QOS_SUPPORTED & 
                                                     protocol_buffer[i].dwServiceFlags1)))
                            {
                              *pProtocolInfo = protocol_buffer[i];
                              bProtocolFound = TRUE;
                              break;
                            }
                        }
                    } 
                } // for    
            } // WSAEnumProtocols
          free(protocol_buffer);
        } // protocol_buffer
    } // WSAEnumProtocols

  if (bProtocolFound)
    printf("  Using service provider <%s>\n\n", pProtocolInfo->szProtocol);

  return(bProtocolFound);
}



BOOL ValidOptions(char *argv[], int argc, OPTIONS *pOptions)
{
  BOOL        bValidOptions = TRUE;
  OSVERSIONINFO   verInfo = {0};

  *pOptions = default_options;

  for (int i=1; i<argc; i++)
    {
      if ((argv[i][0] == '-') || (argv[i][0] == '/') ) 
        {
          switch(tolower(argv[i][1])) 
            {
            case 'b' :
              if (strlen(argv[i]) > 3)
                pOptions->nBufSize = 1024*atoi(&argv[i][3]);
              break;

            case 'c' :
              pOptions->qosOptions.bAlternateQos = TRUE;
              break;

            case 'd' :
              if (strlen(argv[i]) > 3)
                pOptions->fillchar = argv[i][3];
              break;

            case 'e' :
              if (strlen(argv[i]) > 3)
                pOptions->port = atoi(&argv[i][3]);
              break;

            case 'i' :
              if (strlen(argv[i]) > 3)
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
              if (strlen(argv[i]) > 3)
                pOptions->nRepeat = atoi(&argv[i][3]);
              break;

            case 'm' :
              pOptions->spOptions.bMulticast = TRUE;
              if (strlen(argv[i]) > 3)
                strcpy(pOptions->szHostname, &argv[i][3]);
              else
                strcpy(pOptions->szHostname, DEFAULT_MULTICASTGROUP);
              break;

            case 'n' : 
              pOptions->qosOptions.bReceiver = FALSE; 
              // multicast group overrides hostname on -n
              if (!pOptions->spOptions.bMulticast)
                {
                  if (strlen(argv[i]) > 3)
                    strcpy(pOptions->szHostname, &argv[i][3]);
                }
              break;

            case 'p' :
              if (strlen(argv[i]) > 3)
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
              if (strlen(argv[i]) > 3)
                strcpy(pOptions->qosOptions.szTemplate, &argv[i][3]);
              break;

            case 'r' :
              if (strlen(argv[i]) > 3)
                {
                  if ((stricmp(argv[i], "-rsvp-confirm") == 0) ||
                      (stricmp(argv[i], "-rsvp_confirm") == 0))
                    {
                      pOptions->qosOptions.bConfirmResv = TRUE;
                      pOptions->qosOptions.bProviderSpecific = TRUE;
                    }
                  else if ((stricmp(argv[i], "-rsvp-wait") == 0) ||
                           (stricmp(argv[i], "-rsvp_wait") == 0))
                    {
                      pOptions->qosOptions.bWaitToSend = TRUE;
                      pOptions->qosOptions.bProviderSpecific = TRUE;
                    }
                  else
                    {
                      printf("Ignoring option <%s>\n", argv[i]);
                    }
                }
              break;

            case 's' :
              if (strlen(argv[i]) > 3)
                pOptions->dwSleep = atoi(&argv[i][3]);
              break;

            case '?' :
              Usage(argv[0], &default_options);
              return(FALSE);
              break;

            default:
              printf("  unknown options flag %s\n", argv[i]);
              Usage(argv[0], &default_options);
              return(FALSE);
              break;
            }
        }
      else
        {
          printf("  unknown option %s\n", argv[i]);
          Usage(argv[0], &default_options);
          return(FALSE);
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
      if (pOptions->qosOptions.bReceiver && pOptions->qosOptions.bWaitToSend)
        pOptions->qosOptions.bWaitToSend = FALSE;  // not a valid receiver option

      if (!pOptions->qosOptions.bReceiver && pOptions->qosOptions.bConfirmResv)
        pOptions->qosOptions.bWaitToSend = FALSE;  // not a valid sender option

      if (pOptions->qosOptions.bAlternateQos)
        pOptions->nRepeat = 0;  // override repeat count to continuous mode

      if ((IPPROTO_UDP == pOptions->spOptions.iProtocol) && 
          (!pOptions->spOptions.bMulticast))
        {
          // using UDP, there WSAAccept will not be called, therefore do not wait to set qos
          pOptions->qosOptions.qosIoctlSet = QOS_IOCTL_SET_BEFORE;
        }

      verInfo.dwOSVersionInfoSize = sizeof(verInfo);
      GetVersionEx(&verInfo);
      switch (verInfo.dwPlatformId)
        {
        case VER_PLATFORM_WIN32_NT:
          pOptions->qosOptions.bQueryBufferSize = TRUE;
          pOptions->qosOptions.bMustSetQosInAccept = FALSE;
          if (verInfo.dwBuildNumber >= 1960)
            {
              printf("new build\n");
              pOptions->qosOptions.bFineGrainErrorAvail = TRUE;
              pOptions->qosOptions.bQosabilityIoctls = TRUE;
            }
          else
            {
              pOptions->qosOptions.bFineGrainErrorAvail = FALSE;
              pOptions->qosOptions.bQosabilityIoctls = FALSE;
            }
          printf("running on NT\n");
          break;

        case VER_PLATFORM_WIN32_WINDOWS:
          pOptions->qosOptions.bQueryBufferSize = FALSE;
          pOptions->qosOptions.bMustSetQosInAccept = TRUE;
          pOptions->qosOptions.bFineGrainErrorAvail = FALSE;
          pOptions->qosOptions.bQosabilityIoctls = FALSE;
          printf("running on 9x\n");
          break;

        default:
          printf ("requires Windows NT or Windows 98\n");
          pOptions->qosOptions.bQueryBufferSize = FALSE;
          pOptions->qosOptions.bMustSetQosInAccept = TRUE;
          pOptions->qosOptions.bFineGrainErrorAvail = FALSE;
          pOptions->qosOptions.bQosabilityIoctls = FALSE;
          break;
        }
    }

  if (!(pOptions->buf = (char *)malloc(pOptions->nBufSize)))
    return(FALSE);
  else
    {
      FillMemory(pOptions->buf, pOptions->nBufSize, pOptions->fillchar);
      return(TRUE);
    }
}


static VOID PrintOptions(OPTIONS *pOptions)
{
  printf("Options\n");
  printf("  Protocol %d\n", pOptions->spOptions.iProtocol);
  printf("  Port %d\n", pOptions->port);
  if (pOptions->qosOptions.bReceiver)
    printf("  Act as Receiver\n");
  else
    {
      printf("  Act as sender and send to %s\n", pOptions->szHostname);
      printf("  Sleep %d milliseconds between sends\n", pOptions->dwSleep);
      printf("  Fill buffer with <%c>\n", pOptions->fillchar);
      if (pOptions->nRepeat)
        printf("  Repeat sending the buffer %d times\n", pOptions->nRepeat);
      else
        printf("  Repeat sending the buffer continually\n");

    }
  printf("  Bufsize %d (1K increments)\n", pOptions->nBufSize);
  printf("  Multicast is %s\n", (pOptions->spOptions.bMulticast ? "Enabled" : "Disabled"));

  if (pOptions->spOptions.bQos)
    {
      printf("  Qos template %s\n", pOptions->qosOptions.szTemplate);
      printf("  Qos to be set ");
      switch (pOptions->qosOptions.qosIoctlSet)
        {
        case QOS_IOCTL_SET_BEFORE: 
          printf("Before accept/connect/joinleaf\n");
          break;
        case QOS_IOCTL_SET_AFTER:  
          printf("After accept/connect/joinleaf\n"); 
          break;
        case QOS_IOCTL_SET_DURING: 
          printf("During accept/connect/joinleaf\n");
          break;
        case QOS_IOCTL_SET_QOS: 
          if (pOptions->qosOptions.bReceiver)
            printf("During FD_QOS, implies QOS with no signaling set before accept/connect (eventually)\n");
          else
            {
              pOptions->qosOptions.qosIoctlSet = QOS_IOCTL_SET_BEFORE;
              printf("Before connect\n");
            }
          break;
        }

      if (pOptions->qosOptions.bConfirmResv )
        printf("  RESV confirmation to be requested\n");
      if (INVALID_SEND_PRIORITY != pOptions->qosOptions.SendPriority)
        printf("  Qos TC SendPriority to be set to %d\n", 
               pOptions->qosOptions.SendPriority);
      if (!pOptions->qosOptions.bReceiver)
        {
          if (pOptions->qosOptions.bWaitToSend)
            printf("  Wait for RESV before sending data\n");
          else
            printf("  Do not wait for RESV to start sending data\n");
        }
      else
        printf("  Qos must be set during Accept = %s\n", 
               (pOptions->qosOptions.bMustSetQosInAccept?"TRUE":"FALSE"));
           
      printf("  Query for QOS buffer size: %s\n",
             (pOptions->qosOptions.bQueryBufferSize?"TRUE":"FALSE"));
    }
  printf("\n");
  return;
}

// Abstract:
//      Print out usage table for the program
//
VOID Usage(CHAR *szProgramname, OPTIONS *pOptions)
{
  printf("usage:\n  %s -?\n\n", szProgramname);
  printf("  %s [-b:#] [-d:c] [-e:#] [-l:#] [-m:group] [-n:host] [-s:#] [-u]\n\t[-q:template [-i:[a|b|d|q]] [-c] [-rsvp-confirm] [-rsvp-wait]]\n\n",
         szProgramname);
  printf("  -?\t\tDisplay this help\n");
  printf("\n");
  printf("  -b:bufsize\tSize of send/recv buffer; in 1K increments (Def:%d)\n",
         pOptions->nBufSize);
  printf("  -d:c\t\tCharacter used to fill buffer (Def:%c)\n",
         pOptions->fillchar);
  printf("  -e:port\tEndpoint number (port) to use (Def:%d)\n",
         pOptions->port);
  printf("  -l:loop\tLoop count for sending buffer (0==>continuous)\n",
         pOptions->nRepeat);
  printf("  -m:group\tMulticast group (IP) to join (Def:%s)\n", 
         DEFAULT_MULTICASTGROUP);
  printf("  -n:host\tAct as the client and connect to 'host' (Def:%s)\n",
         pOptions->szHostname);
  printf("  -p:protocol\tTCP or UDP (def:TCP)\n");
  printf("  -s:#\t\tSleep # milliseconds between sends (def: 0)\n");
  printf("\n");
  printf("  -q:template\tEnable QoS and use 'template' to specify the name (Def:%s)\n",
         pOptions->qosOptions.szTemplate);
  printf("  -c\t\tCycle enabling/disabling QOS on sending socket (Def: no cycling)\n");
  printf("  -i:[a|b|d|q]\tSet Qos After, Before, During accept/connect, or during FD_QOS\n\t\t\t(def: Before)\n");
  printf("  -rsvp-confirm\t\tRequest RESV confirmation be sent (Def: no confirmation)\n");
  printf("  -rsvp-wait\t\tWait for RESV from receiver before sending data\n");
  return;
}
