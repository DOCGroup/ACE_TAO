// -*- C++ -*-

//=============================================================================
/**
 *  @file    Log_Msg_Backend.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_LOG_MSG_BACKEND_H
#define ACE_LOG_MSG_BACKEND_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Log_Record;

/**
 * @class ACE_Log_Msg_Backend
 *
 * @brief Define the interface for ACE_Log_Msg backend strategies.
 *
 * The ACE_Log_Msg class can log to multiple backend strategies, for
 * example, some send messages to a remote logger, others dump to a
 * file, or simply to stderr.  In the future we could define
 * interfaces that log to the syslog daemon (on UNIX), the Event log
 * (on NT) a temporary ring buffer, etc.
 */
class ACE_Export ACE_Log_Msg_Backend
{
public:
  /// No-op virtual destructor.
  virtual ~ACE_Log_Msg_Backend (void);

  /// Open a new connection
  virtual int open (const ACE_TCHAR *logger_key) = 0;

  /*
   * Reset the backend.  When changing the logging destination the
   * backend may need to properly disconnect from the remote logging
   * daemon and reclaim some local resources.  But we try to reduce
   * the number of local allocations/deallocations.
   */
  virtual int reset (void) = 0;

  /// Close the backend completely.
  virtual int close (void) = 0;

  /// Backend routine.  This is called when we want to log a message.
  /// Since this routine is pure virtual, it must be overwritten by the
  /// subclass.
  virtual int log (ACE_Log_Record &log_record) = 0;
};

#include "ace/post.h"
#endif /* ACE_LOG_MSG_BACKEND_H */
