/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Server_Logging_Handler_T.h 
//
// = AUTHOR
//    Doug Schmidt and Per Andersson
// 
// ============================================================================

#if !defined (ACE_SERVER_LOGGING_HANDLER_T_H)
#define ACE_SERVER_LOGGING_HANDLER_T_H

#include "ace/Synch.h"
#include "ace/TLI_Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/SString.h"
#include "Base_Optimizer.h"

#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
#include "Base_Optimizer.h"
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_DECL, class LOG_MESSAGE_RECEIVER>
class ACE_Server_Logging_Handler_T : public ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_SYNCH_USE>
{
  // = TITLE
  //    Product object created by an <ACE_Server_Logging_Acceptor_T>.  An
  //    <ACE_Server_Logging_Handler_T> receives, and frames logging
  //    records. The logging record is then processed by the 
  //    <LOG_MESSAGE_RECEIVER>
  // 
  // = DESCRIPTION
  //     Defines the classes that perform server logging daemon
  //     functionality.
public:  
  ACE_Server_Logging_Handler_T (ACE_Thread_Manager *,
				const LOG_MESSAGE_RECEIVER &receiver );
  // Constructor.


  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Process remote logging records. 

protected:
  int handle_logging_record (void);
  // Receive the logging record from a client.

  int open_common (void);
  // Common parts of open function, sets hostname and diables NONBLOCK in peer
  // called from derived classes open method.
  
#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static COUNTER request_count_;
  // Count the number of logging records that arrive.
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
  Base_Optimizer<LOG_MESSAGE_RECEIVER, ACE_CString> receiver_;
  // Packs a LOG_MESSAGE_RECEIVER and ACE_CString attribute together
  // in a optimized fashion.  The LOG_MESSAGE_RECEIVER class is often
  // a class with no instance data.
#else
  LOG_MESSAGE_RECEIVER receiver_;
  ACE_CString host_name_;
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */  
  const char *host_name (void);
  // Name of the host we are connected to.
  
  LOG_MESSAGE_RECEIVER &receiver (void){ return receiver_; }
  // The receiver of log records
};

#if !defined (ACE_HAS_TLI)
#define LOGGING_PEER_ACCEPTOR ACE_SOCK_ACCEPTOR
#define LOGGING_PEER_STREAM ACE_SOCK_STREAM
#else /* use sockets */
#define LOGGING_PEER_ACCEPTOR ACE_TLI_ACCEPTOR
#define LOGGING_PEER_STREAM ACE_TLI_STREAM
#endif /* ACE_HAS_TLI */

template<class SERVER_LOGGING_HANDLER, class LOG_MESSAGE_RECEIVER, class SCHEDULE_STRATEGY>
class ACE_Server_Logging_Acceptor_T : public ACE_Strategy_Acceptor<SERVER_LOGGING_HANDLER, LOGGING_PEER_ACCEPTOR>
  // = TITLE
  //     Factory that creates <SERVER_LOGGING_HANDLER>s scheduled with
  //     <SCHEDULE_STRATEGY> and logging records proccessed by a
  //     <LOG_MESSAGE_RECEIVER>
  //
  // = DESCRIPTION
  //     This class contains the service-specific methods that can't
  //     easily be factored into the <ACE_Strategy_Acceptor>.
{
public:
  ACE_Server_Logging_Acceptor_T (void); 
  virtual int init (int argc, char *argv[]);
  // Dynamic linking hook.

protected:
  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.
  
  virtual int make_svc_handler (SERVER_LOGGING_HANDLER *&);
  // Factory that creates a new <SERVER_LOGGING_HANDLER>.  We need to
  // specialize this since the <LOG_MESSAGE_RECEIVER> held by this Acceptor must be
  // passed into the <SERVER_LOGGING_HANDLER>. 

private:
  // At the moment each ACE_Server_Logging_Acceptor_T contains
  // a <LOG_MESSAGE_RECEIVER> attribute that is passed to the
  // <SERVER_LOGGING_HANDLER> at construction. A better idea might 
  // be to have accessor class as template argument. The accessor
  // should be a factory/strategy that hands the 
  // ACE_Server_Logging_Acceptor_T instance references
  // to a <LOG_MESSAGE_RECEIVER>. This makes it possible
  // to change how <LOG_MESSAGE_RECEIVER> are created without chaning the
  // ACE_Server_Logging_Acceptor_T code.

#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
  Base_Optimizer<LOG_MESSAGE_RECEIVER, SCHEDULE_STRATEGY> receiver_;
  // Packs a LOG_MESSAGE_RECEIVER and ACE_CString attribute together
  // in a optimized fashion. The LOG_MESSAGE_RECEIVER class is often a
  // class with no instance data.
#else
  LOG_MESSAGE_RECEIVER receiver_;
  SCHEDULE_STRATEGY schedule_strategy_;
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */

  SCHEDULE_STRATEGY &scheduling_strategy (void);
  // The scheduling strategy for the service.
  
  LOG_MESSAGE_RECEIVER &receiver (void);
  // The receiver of log records
};

template<class LOG_MESSAGE_RECEIVER> 
class ACE_Server_Logging_Handler : public ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, u_long, ACE_NULL_SYNCH, LOG_MESSAGE_RECEIVER> 
  // = TITLE
  //    Product object created by a 
  //    <ACE_Server_Logging_Acceptor_T<ACE_Server_Logging_Handler> >.  An
  //    <ACE_Server_Logging_Handler> receives, frames. The logging record 
  //    is then processed by the <LOG_MESSAGE_RECEIVER>
  //
  // = DESCRIPTION
  //     All clients are handled in the same thread.
{
public:
  ACE_Server_Logging_Handler (ACE_Thread_Manager * = 0);
  ACE_Server_Logging_Handler (ACE_Thread_Manager *,
			      const LOG_MESSAGE_RECEIVER &receiver);
                                  
  virtual int open (void* = 0);
};

#if defined (ACE_HAS_THREADS) 
typedef ACE_Atomic_Op <ACE_Thread_Mutex, u_long> ACE_LOGGER_COUNTER;
#define ACE_LOGGER_SYNCH ACE_MT_SYNCH 
#else
typedef u_long ACE_LOGGER_COUNTER;
#define ACE_LOGGER_SYNCH ACE_NULL_SYNCH 
#endif /* ACE_HAS_THREADS */

template<class LOG_MESSAGE_RECEIVER> 
class ACE_Thr_Server_Logging_Handler : public ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, ACE_LOGGER_COUNTER, ACE_LOGGER_SYNCH, LOG_MESSAGE_RECEIVER> 
  // = TITLE
  //    Product object created by a 
  //    <ACE_Server_Logging_Acceptor_T<ACE_Thr_Server_Logging_Handler> >.  An
  //    <ACE_Thr_Server_Logging_Handler> receives, frames. The logging record is then processed by the 
  //    <LOG_MESSAGE_RECEIVER>
  //
  // = DESCRIPTION
  //     Each client is handled in its own separate thread.
{
public:
  ACE_Thr_Server_Logging_Handler (ACE_Thread_Manager * = 0);
  ACE_Thr_Server_Logging_Handler (ACE_Thread_Manager *,
				  const LOG_MESSAGE_RECEIVER &receiver);
  virtual int open (void * = 0);
  virtual int svc (void);
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Server_Logging_Handler_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Server_Logging_Handler_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_SERVER_LOGGING_HANDLER_T_H */
