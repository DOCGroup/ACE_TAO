// $Id$
// Server_Logging_Handler.cpp

#if !defined (ACE_SERVER_LOGGING_HANDLER_C)
#define ACE_SERVER_LOGGING_HANDLER_C

#define ACE_BUILD_SVC_DLL
#include "ace/Synch.h"
#include "Server_Logging_Handler.h"

// The following are "Factories" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the
// single-threaded and multi-threaded logging server.

ACE_SVC_FACTORY_DEFINE (ACE_Server_Logging_Acceptor)
ACE_SVC_FACTORY_DEFINE (ACE_Thr_Server_Logging_Acceptor)

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION) 
template class Log_Message_Receiver_Impl<ACE_NULL_SYNCH>;
template class Static_Log_Message_Receiver<ACE_NULL_SYNCH>;
template class ACE_Acceptor<Null_Synch_Logging_Handler_Static_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Accept_Strategy<Null_Synch_Logging_Handler_Static_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Concurrency_Strategy<Null_Synch_Logging_Handler_Static_Receiver>;
template class ACE_Creation_Strategy<Null_Synch_Logging_Handler_Static_Receiver>;
template class ACE_Schedule_All_Reactive_Strategy<Null_Synch_Logging_Handler_Static_Receiver>;
template class ACE_Scheduling_Strategy<Null_Synch_Logging_Handler_Static_Receiver>;
template class ACE_Strategy_Acceptor<Null_Synch_Logging_Handler_Static_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Server_Logging_Acceptor_T<Null_Synch_Logging_Handler_Static_Receiver, 
                                             Null_Synch_Static_Receiver, 
                                             ACE_Schedule_All_Reactive_Strategy<Null_Synch_Logging_Handler_Static_Receiver> >;
template class ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, u_long, ACE_NULL_SYNCH, Null_Synch_Static_Receiver>;
template class ACE_Server_Logging_Handler<Null_Synch_Static_Receiver>;


template class Static_Log_Message_Receiver<ACE_LOGGER_SYNCH>;
template class Log_Message_Receiver<ACE_LOGGER_SYNCH>;
template class ACE_Acceptor<Synch_Logging_Handler_Static_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Acceptor<Synch_Thr_Logging_Handler_Static_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Acceptor<Synch_Logging_Handler_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Acceptor<Synch_Thr_Logging_Handler_Receiver, LOGGING_PEER_ACCEPTOR>;

template class ACE_Accept_Strategy<Synch_Logging_Handler_Static_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Accept_Strategy<Synch_Thr_Logging_Handler_Static_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Accept_Strategy<Synch_Logging_Handler_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Accept_Strategy<Synch_Thr_Logging_Handler_Receiver, LOGGING_PEER_ACCEPTOR>;

template class ACE_Concurrency_Strategy<Synch_Logging_Handler_Static_Receiver>;
template class ACE_Concurrency_Strategy<Synch_Thr_Logging_Handler_Static_Receiver>;
template class ACE_Concurrency_Strategy<Synch_Logging_Handler_Receiver>;
template class ACE_Concurrency_Strategy<Synch_Thr_Logging_Handler_Receiver>;

template class ACE_Creation_Strategy<Synch_Logging_Handler_Static_Receiver>;
template class ACE_Creation_Strategy<Synch_Thr_Logging_Handler_Static_Receiver>;
template class ACE_Creation_Strategy<Synch_Logging_Handler_Receiver>;
template class ACE_Creation_Strategy<Synch_Thr_Logging_Handler_Receiver>;

template class ACE_Schedule_All_Reactive_Strategy<Synch_Logging_Handler_Static_Receiver>;
template class ACE_Schedule_All_Reactive_Strategy<Synch_Logging_Handler_Receiver>;

template class ACE_Schedule_All_Threaded_Strategy<Synch_Thr_Logging_Handler_Static_Receiver>;
template class ACE_Schedule_All_Threaded_Strategy<Synch_Thr_Logging_Handler_Receiver>;

template class ACE_Scheduling_Strategy<Synch_Logging_Handler_Static_Receiver>;
template class ACE_Scheduling_Strategy<Synch_Logging_Handler_Receiver>;
template class ACE_Scheduling_Strategy<Synch_Thr_Logging_Handler_Static_Receiver>;
template class ACE_Scheduling_Strategy<Synch_Thr_Logging_Handler_Receiver>;

template class ACE_Strategy_Acceptor<Synch_Logging_Handler_Static_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Strategy_Acceptor<Synch_Thr_Logging_Handler_Static_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Strategy_Acceptor<Synch_Logging_Handler_Receiver, LOGGING_PEER_ACCEPTOR>;
template class ACE_Strategy_Acceptor<Synch_Thr_Logging_Handler_Receiver, LOGGING_PEER_ACCEPTOR>;

template class ACE_Server_Logging_Acceptor_T<Synch_Logging_Handler_Static_Receiver, 
                                            Synch_Static_Receiver, 
                                            ACE_Schedule_All_Reactive_Strategy<Synch_Logging_Handler_Static_Receiver> >;
template class ACE_Server_Logging_Acceptor_T<Synch_Thr_Logging_Handler_Static_Receiver, 
                                             Synch_Static_Receiver, 
                                             ACE_Schedule_All_Threaded_Strategy<Synch_Thr_Logging_Handler_Static_Receiver> >;
template class ACE_Server_Logging_Acceptor_T<Synch_Logging_Handler_Receiver, 
                                             Synch_Receiver, 
                                             ACE_Schedule_All_Reactive_Strategy<Synch_Logging_Handler_Receiver> >;
template class ACE_Server_Logging_Acceptor_T<Synch_Thr_Logging_Handler_Receiver, 
                                             Synch_Receiver, 
                                             ACE_Schedule_All_Threaded_Strategy<Synch_Thr_Logging_Handler_Receiver> >;

template class ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, u_long, ACE_NULL_SYNCH, Synch_Static_Receiver>;
template class ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, u_long, ACE_NULL_SYNCH, Synch_Receiver>;

  #if defined (ACE_HAS_THREADS) 
  template class ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, ACE_LOGGER_COUNTER, ACE_MT_SYNCH, Synch_Static_Receiver>;
  template class ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, ACE_LOGGER_COUNTER, ACE_MT_SYNCH, Synch_Receiver>;
  template class Log_Message_Receiver_Impl<ACE_MT_SYNCH>;
  #endif /* ACE_HAS_THREADS */

template class ACE_Server_Logging_Handler<Synch_Static_Receiver>;
template class ACE_Thr_Server_Logging_Handler<Synch_Static_Receiver>;
template class ACE_Server_Logging_Handler<Synch_Receiver>;
template class ACE_Thr_Server_Logging_Handler<Synch_Receiver>;

template class ACE_Svc_Handler<LOGGING_PEER_STREAM, ACE_SYNCH>;
  
  #if defined (ACE_HAS_TLI)
  template class ACE_Svc_Handler<LOGGING_PEER_STREAM, ACE_NULL_SYNCH>;
  // else LOGGING_PEER_STREAM is ACE_SOCK_STREAM,
  // and the specialization would be
  //   template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
  // That specialization is in TS_Server_Handler.cpp, so we don't need it
  // here.
  #endif /* ACE_HAS_TLI */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#endif /* ACE_SERVER_LOGGING_HANDLERT_C */

