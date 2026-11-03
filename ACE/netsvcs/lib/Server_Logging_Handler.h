/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Server_Logging_Handler.h
 *
 *  @author Doug Schmidt and Per Andersson <Per.Andersson@hfera.ericsson.se>
 */
//=============================================================================


#ifndef ACE_SERVER_LOGGING_HANDLER_H
#define ACE_SERVER_LOGGING_HANDLER_H

#include "Log_Message_Receiver.h"
#include "Server_Logging_Handler_T.h"
#include "ace/svc_export.h"

// Typedefs for Logging Handlers & acceptors using a static type based
// log message receivers.

// Synched and NULL synched message receivers
using Null_Synch_Static_Receiver = Static_Log_Message_Receiver<ACE_NULL_SYNCH>;
using Synch_Static_Receiver = Static_Log_Message_Receiver<ACE_LOGGER_SYNCH>;

// NULL synched logging handler
using Null_Synch_Logging_Handler_Static_Receiver =
  ACE_Server_Logging_Handler<Null_Synch_Static_Receiver>;

// synched logging handlers
using Synch_Logging_Handler_Static_Receiver = ACE_Server_Logging_Handler<Synch_Static_Receiver>;
using Synch_Thr_Logging_Handler_Static_Receiver = ACE_Thr_Server_Logging_Handler<Synch_Static_Receiver>;

// NULL synched logging acceptor
using Null_Synch_Logging_Handler_Static_Receiver_Acceptor =
  ACE_Server_Logging_Acceptor_T<Null_Synch_Logging_Handler_Static_Receiver,
                                Null_Synch_Static_Receiver,
                                ACE_Schedule_All_Reactive_Strategy<Null_Synch_Logging_Handler_Static_Receiver> >;

// NULL synched logging acceptors
using Synch_Logging_Handler_Static_Receiver_Acceptor =
  ACE_Server_Logging_Acceptor_T<Synch_Logging_Handler_Static_Receiver,
                                Synch_Static_Receiver,
                                ACE_Schedule_All_Reactive_Strategy<Synch_Logging_Handler_Static_Receiver> >;

using Synch_Thr_Logging_Handler_Static_Receiver_Acceptor =
  ACE_Server_Logging_Acceptor_T<Synch_Thr_Logging_Handler_Static_Receiver,
                                Synch_Static_Receiver,
                                ACE_Schedule_All_Threaded_Strategy<Synch_Thr_Logging_Handler_Static_Receiver> >;

// typedefs for Logging Handlers & acceptors using a instance based
// log message receivers.

// Synched message receivers
using Synch_Receiver = Log_Message_Receiver<ACE_LOGGER_SYNCH>;

// synched logging handlers
using Synch_Logging_Handler_Receiver = ACE_Server_Logging_Handler<Synch_Receiver>;
using Synch_Thr_Logging_Handler_Receiver = ACE_Thr_Server_Logging_Handler<Synch_Receiver>;

// synched logging acceptors
using Synch_Logging_Handler_Receiver_Acceptor =
  ACE_Server_Logging_Acceptor_T<Synch_Logging_Handler_Receiver,
                                Synch_Receiver,
                                ACE_Schedule_All_Reactive_Strategy<Synch_Logging_Handler_Receiver> >;

using Synch_Thr_Logging_Handler_Receiver_Acceptor =
  ACE_Server_Logging_Acceptor_T<Synch_Thr_Logging_Handler_Receiver,
                                Synch_Receiver,
                                ACE_Schedule_All_Threaded_Strategy<Synch_Thr_Logging_Handler_Receiver> >;


// Define external acceptors

// Acceptors that use static/type based log message receiver.
using ACE_Server_Logging_Acceptor = Null_Synch_Logging_Handler_Static_Receiver_Acceptor;
using ACE_Thr_Server_Logging_Acceptor = Synch_Thr_Logging_Handler_Static_Receiver_Acceptor;

ACE_SVC_FACTORY_DECLARE (ACE_Server_Logging_Acceptor)
ACE_SVC_FACTORY_DECLARE (ACE_Thr_Server_Logging_Acceptor)

#endif /* ACE_SERVER_LOGGING_HANDLER_H */
