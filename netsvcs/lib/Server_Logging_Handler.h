/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Server_Logging_Handler.h 
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SERVER_LOGGING_HANDLER_H)
#define ACE_SERVER_LOGGING_HANDLER_H

#include "ace/Synch.h"
#include "ace/TLI_Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_1>
class ACE_Server_Logging_Handler : public ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_SYNCH_2>
{
  // = TITLE
  //    Product object created by an <ACE_Server_Logging_Acceptor>.  An
  //    <ACE_Server_Logging_Handler> receives, frames, and processes logging
  //    records.
  // 
  // = DESCRIPTION
  //     Defines the classes that perform server logging daemon
  //     functionality.
public:
  ACE_Server_Logging_Handler (ACE_Thread_Manager * = 0,
			      ACE_SYNCH_MUTEX_T *lock = 0);

  virtual int open (void * = 0);
  // Hook called by <Server_Logging_Acceptor> when connection is
  // established.

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Process remote logging records. 

protected:
  int handle_logging_record (void);
  // Receive the logging record from a client.
  
#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static COUNTER request_count_;
  // Count the number of logging records that arrive.
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

  char host_name_[MAXHOSTNAMELEN + 1];
  // Name of the host we are connected to.

  ACE_SYNCH_MUTEX_T &lock_;
  // Reference to the lock used to serialize output.
};

#if !defined (ACE_HAS_TLI)
#define LOGGING_PEER_ACCEPTOR ACE_SOCK_ACCEPTOR
#define LOGGING_PEER_STREAM ACE_SOCK_STREAM
#else /* use sockets */
#define LOGGING_PEER_ACCEPTOR ACE_TLI_ACCEPTOR
#define LOGGING_PEER_STREAM ACE_TLI_STREAM
#endif /* ACE_HAS_TLI */

typedef ACE_Server_Logging_Handler<LOGGING_PEER_STREAM, u_long, ACE_NULL_SYNCH>
	SERVER_LOGGING_HANDLER;

class ACE_Server_Logging_Acceptor : public ACE_Strategy_Acceptor<SERVER_LOGGING_HANDLER, LOGGING_PEER_ACCEPTOR>
  // = TITLE
  //     This class implements the ACE single-threaded logging service.
  //
  // = DESCRIPTION
  //     This class contains the service-specific methods that can't
  //     easily be factored into the <ACE_Strategy_Acceptor>.
{
public:
  virtual int init (int argc, char *argv[]);
  // Dynamic linking hook.

protected:
  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.

  virtual int make_svc_handler (SERVER_LOGGING_HANDLER *&);
  // Factory that creates a new <SERVER_LOGGING_HANDLER>.  We need to
  // specialize this since the <lock_> held by this Acceptor must be
  // passed into the <SERVER_LOGGING_HANDLER>.

private:
  ACE_SYNCH_MUTEX lock_;
  // Lock used to serialize output by the various
  // <ACE_Server_Logging_Handler>'s.

  ACE_Schedule_All_Reactive_Strategy<SERVER_LOGGING_HANDLER> scheduling_strategy_;
  // The scheduling strategy is designed for Reactive services.
};

#if !defined (ACE_HAS_THREADS) 
typedef u_long COUNTER;
#define ACE_LOGGER_SYNCH ACE_NULL_SYNCH 
#else
typedef ACE_Atomic_Op <ACE_Thread_Mutex, u_long> COUNTER;
#define ACE_LOGGER_SYNCH ACE_MT_SYNCH 
#endif /* ACE_HAS_THREADS */

class ACE_Svc_Export ACE_Thr_Server_Logging_Handler : public ACE_Server_Logging_Handler<LOGGING_PEER_STREAM, COUNTER, ACE_LOGGER_SYNCH> 
  // = TITLE
  //    Product object created by a <ACE_Thr_Server_Logging_Acceptor>.  An
  //    <ACE_Thr_Server_Logging_Handler> receives, frames, and processes
  //    logging records.
  //
  // = DESCRIPTION
  //     Each client is handled in its own separate thread.
{
public:
  ACE_Thr_Server_Logging_Handler (ACE_Thread_Manager * = 0);

  virtual int open (void * = 0);
  // Override activation definition in the ACE_Svc_Handler class (will
  // spawn a new thread if we've got threads).

  virtual int svc (void);
  // Process remote logging records. 
};

ACE_SVC_FACTORY_DECLARE (ACE_Server_Logging_Acceptor)
ACE_SVC_FACTORY_DECLARE (ACE_Thr_Server_Logging_Acceptor)

#endif /* ACE_SERVER_LOGGING_HANDLER_H */
