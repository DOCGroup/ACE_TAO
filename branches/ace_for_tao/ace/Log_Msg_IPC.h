// -*- C++ -*-

//=============================================================================
/**
 *  @file    Log_Msg_IPC.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef ACE_LOG_MSG_LOGGER_H
#define ACE_LOG_MSG_LOGGER_H
#include /**/ "ace/pre.h"

#include "ace/Log_Msg_Backend.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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

/// Defines the interfaces for ACE_Log_Msg backend.
/**
 * Implement an ACE_Log_Msg_Backend that logs to a remote logging
 * process.
 */
class ACE_Export ACE_Log_Msg_IPC : public ACE_Log_Msg_Backend
{
public:
  /// Constructor
  ACE_Log_Msg_IPC (void);

  /// Destructor
  virtual ~ACE_Log_Msg_IPC (void);

  /// Open a new connection
  virtual int open (const ACE_TCHAR *logger_key);
  virtual int reset (void);
  virtual int close (void);
  virtual int log (ACE_Log_Record &log_record);

private:
  ACE_LOG_MSG_IPC_STREAM message_queue_;
};

#include /**/ "ace/post.h"
#endif /* ACE_LOG_MSG_H */
