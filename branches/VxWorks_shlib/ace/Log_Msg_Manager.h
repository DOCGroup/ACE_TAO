/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Log_Msg_Manager.h
//
// = AUTHOR
//    David L. Levine
//
// ============================================================================

#ifndef ACE_LOG_MSG_MANAGER_H
#define ACE_LOG_MSG_MANAGER_H

#include "ace/inc_user_config.h"

// IPC conduit between sender and client daemon.  This should be
// included in the <ACE_Log_Msg> class, but due to "order of include"
// problems it can't be...
#if defined (ACE_HAS_STREAM_PIPES)
# include "ace/SPIPE_Connector.h"
typedef ACE_SPIPE_Stream ACE_LOG_MSG_IPC_STREAM;
typedef ACE_SPIPE_Connector ACE_LOG_MSG_IPC_CONNECTOR;
typedef ACE_SPIPE_Addr ACE_LOG_MSG_IPC_ADDR;
#else
# include "ace/SOCK_Connector.h"
typedef ACE_SOCK_Stream ACE_LOG_MSG_IPC_STREAM;
typedef ACE_SOCK_Connector ACE_LOG_MSG_IPC_CONNECTOR;
typedef ACE_INET_Addr ACE_LOG_MSG_IPC_ADDR;
#endif /* ACE_HAS_STREAM_PIPES */

class ACE_Recursive_Thread_Mutex;

class ACE_Log_Msg_Manager
  // = TITLE
  //      Synchronize output operations.
  //
  // = DESCRIPTION
  //     Provides global point of contact for all ACE_Log_Msg instances
  //     in a process.
  //
  //     For internal use by ACE, only!
{
public:
  static ACE_LOG_MSG_IPC_STREAM *message_queue_;

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  static ACE_Recursive_Thread_Mutex *get_lock (void);

  static void close (void);

private:
  static ACE_Recursive_Thread_Mutex *lock_;
#endif /* ! ACE_MT_SAFE */
};

// This is only needed here because we can't afford to call
// ACE_LOG_MSG->instance() from within ACE_Log_Msg::instance() or else
// we will recurse infinitely!  Not for public use!
#define ACE_NEW_RETURN_I(POINTER,CONSTRUCTOR,RET_VAL) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
     } while (0)

#endif /* ACE_LOG_MSG_MANAGER_H */
