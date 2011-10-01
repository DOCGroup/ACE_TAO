/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    receiver.cpp
 *
 *  $Id$
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#define QOSEVENT_MAIN

#include "ace/QoS/QoS_Session.h"
#include "ace/QoS/QoS_Session_Factory.h"
#include "ace/QoS/QoS_Decorator.h"
#include "ace/QoS/SOCK_Dgram_Mcast_QoS.h"

#include "QoS_Util.h"
#include "Fill_ACE_QoS.h"
#include "QoS_Signal_Handler.h"
#include "Receiver_QoS_Event_Handler.h"

// To open QOS sockets administrative access is required on the
// machine.  Fill in default values for QoS structure.  The default
// values were simply choosen from existing QOS templates available
// via WSAGetQosByName.  Notice that ProviderSpecific settings are
// being allowed when picking the "default" template but not for
// "well-known" QOS templates.  Also notice that since data is only
// flowing from sender to receiver, different flowspecs are filled in
// depending upon whether this application is acting as a sender or
// receiver.


// This function fills up the ACE_QoS_Params with the supplied iovec
// and ACE_QoS.

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
ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{

  QoS_Util qos_util(argc, argv);

  if (qos_util.parse_args () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in parsing args\n"),
                      -1);

  // This is a multicast application.
  if (qos_util.multicast_flag ())
    {
       Fill_ACE_QoS  fill_ace_qos;

       // The application adds the flow specs that it wants into the
       // Fill_ACE_QoS. The Fill_ACE_QoS indexes the flow specs by the
       // flow spec names. Here the new flowspec being added is g_711.
       ACE_CString g_711 ("g_711");

       switch (fill_ace_qos.map ().bind (g_711,
                                         new ACE_Flow_Spec (9200,
                                                            708,
                                                            18400,
                                                            0,
                                                            0,
                                                            ACE_SERVICETYPE_CONTROLLEDLOAD,
                                                            368,
                                                            368,
                                                            25,
                                                            1)))
         {
         case 1 :
           ACE_ERROR_RETURN ((LM_ERROR,
                              "Unable to bind the new flow spec\n"
                              "The Flow Spec name already exists\n"),
                             -1);
           break;
         case -1 :
           ACE_ERROR_RETURN ((LM_ERROR,
                              "Unable to bind the new flow spec\n"),
                             -1);
           break;
         }

       ACE_DEBUG ((LM_DEBUG,
                   "g_711 Flow Spec bound successfully\n"));

       // This is a receiver. So we fill in the receiving QoS parameters.
       ACE_QoS ace_qos_receiver;
       if (fill_ace_qos.fill_simplex_receiver_qos (ace_qos_receiver,
                                                   g_711) !=0)
         ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to fill simplex receiver qos\n"),
                           -1);
       else
         ACE_DEBUG ((LM_DEBUG,
                     "Filled up the Receiver QoS parameters\n"));

      // Opening a new Multicast Datagram.
      ACE_SOCK_Dgram_Mcast_QoS dgram_mcast_qos;

      // Multicast Session Address specified by user at command line.
      // If this address is not specified,
      // <localhost:ACE_DEFAULT_MULTICAST_PORT> is assumed.
      ACE_INET_Addr mult_addr (*(qos_util.mult_session_addr ()));

      // Fill the ACE_QoS_Params to be passed to the <ACE_OS::join_leaf>
      // through subscribe.

      ACE_QoS_Params qos_params;
      FillQoSParams (qos_params, 0, &ace_qos_receiver);

      // Create a QoS Session Factory.
      ACE_QoS_Session_Factory session_factory;

      // Ask the factory to create a QoS session. This could be RAPI or
      // GQoS based on the parameter passed.
      ACE_QoS_Session *qos_session =
        session_factory.create_session ();

      // Create a destination address for the QoS session. The same
      // address should be used for the subscribe call later. A copy
      // is made below only to distinguish the two usages of the dest
      // address.

      ACE_INET_Addr dest_addr (mult_addr);

      // A QoS session is defined by the 3-tuple [DestAddr, DestPort,
      // Protocol]. Initialize the QoS session.
      if (qos_session->open (mult_addr,
                             IPPROTO_UDP) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error in opening the QoS session\n"),
                          -1);
      else
        ACE_DEBUG ((LM_DEBUG,
                    "QoS session opened successfully\n"));

      // The following call opens the Dgram_Mcast and calls the
      // <ACE_OS::join_leaf> with the qos_params supplied here. Note
      // the QoS session object is passed into this call. This
      // subscribes the underlying socket to the passed in QoS
      // session. For joining multiple multicast sessions, the
      // following subscribe call should be made with different
      // multicast addresses and a new QoS session object should be
      // passed in for each such call. The QoS session objects can be
      // created only through the session factory. Care should be
      // taken that the mult_addr for the subscribe() call matches the
      // dest_addr of the QoS session object. If this is not done, the
      // subscribe call will fail. A more abstract version of
      // subscribe will be added that constrains the various features
      // of GQoS like different flags etc.

      if (dgram_mcast_qos.subscribe (mult_addr,
                                     qos_params,
                                     1,
                                     0,
                                     AF_INET,
                                     // ACE_FROM_PROTOCOL_INFO,
                                     0,
                                     0, // ACE_Protocol_Info,
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
                    "Dgram_Mcast subscribe succeeds\n"));

      int nIP_TTL = 25;
      char achInBuf [BUFSIZ];
      u_long dwBytes;

      // Should this be abstracted into QoS objects ?? Doesnt seem to have
      // to do anything directly with QoS.
      if (ACE_OS::ioctl (dgram_mcast_qos.get_handle (), // Socket.
                         ACE_SIO_MULTICAST_SCOPE, // IO control code.
                         &nIP_TTL, // In buffer.
                         sizeof (nIP_TTL), // Length of in buffer.
                         achInBuf, // Out buffer.
                         BUFSIZ, // Length of Out buffer.
                         &dwBytes, // bytes returned.
                         0, // Overlapped.
                         0) == -1) // Func.
        ACE_ERROR ((LM_ERROR,
                    "Error in Multicast scope ACE_OS::ioctl()\n"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Setting TTL with Multicast scope ACE_OS::ioctl call succeeds\n"));

      int bFlag = 0;

      // Should this be abstracted into QoS objects ?? Doesnt seem to have
      // to do anything directly with QoS.
      if (ACE_OS::ioctl (dgram_mcast_qos.get_handle (), // Socket.
                         ACE_SIO_MULTIPOINT_LOOPBACK, // IO control code.
                         &bFlag, // In buffer.
                         sizeof (bFlag), // Length of in buffer.
                         achInBuf, // Out buffer.
                         BUFSIZ, // Length of Out buffer.
                         &dwBytes, // bytes returned.
                         0, // Overlapped.
                         0) == -1) // Func.
        ACE_ERROR ((LM_ERROR,
                    "Error in Loopback ACE_OS::ioctl()\n"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Disable Loopback with ACE_OS::ioctl call succeeds\n"));

      // This is a receiver.
      qos_session->flags (ACE_QoS_Session::ACE_QOS_RECEIVER);

      ACE_QoS_Manager qos_manager = dgram_mcast_qos.qos_manager ();

      // Set the QoS for the session. Replaces the ioctl () call that
      // was being made previously.
      if (qos_session->qos (&dgram_mcast_qos,
                            &qos_manager,
                            ace_qos_receiver) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to set QoS\n"),
                          -1);
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Setting QOS succeeds.\n"));

      // Register a signal handler that helps to gracefully close the
      // open QoS sessions.
      QoS_Signal_Handler qos_signal_handler (qos_session);

      // Register the usual SIGINT signal handler with the Reactor for
      // the application to gracefully release the QoS session and
      // shutdown.
      if (ACE_Reactor::instance ()->register_handler
          (SIGINT, &qos_signal_handler) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error in registering the Signal Handler.\n"),
                          -1);

      // Handler to process QoS and Data events for the reciever.
      Receiver_QoS_Event_Handler qos_event_handler (dgram_mcast_qos,
                                                    qos_session);

      // Decorate the above handler with QoS functionality.
      ACE_QoS_Decorator qos_decorator (&qos_event_handler,
                                       qos_session);

      // Initialize the Decorator.
      if (qos_decorator.init () != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "QoS Decorator init () failed.\n"),
                          -1);

      // Register the decorated Event Handler with the Reactor.
      if (ACE_Reactor::instance ()->register_handler (&qos_decorator,
                                                      ACE_Event_Handler::QOS_MASK |
                                                      ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error in registering the Decorator with the Reactor\n"),
                          -1);


      // Start the event loop.
      ACE_DEBUG ((LM_DEBUG,
                  "Running the Event Loop ...\n"));

      ACE_Reactor::instance ()->run_event_loop ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) shutting down server logging daemon\n"));
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                "Specify a -m option for multicast application\n"));
  return 0;
}



