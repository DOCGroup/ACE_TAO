// -*- C++ -*-

//=============================================================================
/**
 *  @file    Log_Msg_NT_Event_Log.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef ACE_LOG_MSG_NT_EVENT_LOG_H
#define ACE_LOG_MSG_NT_EVENT_LOG_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

#include "ace/Log_Msg_Backend.h"

/**
 * @class ACE_Log_Msg_NT_Event_Log
 *
 * @brief Implements an ACE_Log_Msg_Backend that logs to the WinNT system
 * event log.
 */
class ACE_Export ACE_Log_Msg_NT_Event_Log : public ACE_Log_Msg_Backend
{
public:
  /// Constructor
  ACE_Log_Msg_NT_Event_Log (void);

  /// Destructor
  virtual ~ACE_Log_Msg_NT_Event_Log (void);

  /// Open a new event log.
  virtual int open (const ACE_TCHAR *logger_key);

  /// Reset the backend.
  virtual int reset (void);

  /// Close the backend completely.
  virtual int close (void);

  /// This is called when we want to log a message.
  virtual int log (ACE_Log_Record &log_record);

private:
  HANDLE evlog_handle_;
};

#endif /* ACE_WIN32 && !ACE_HAS_WINCE */

#include "ace/post.h"
#endif /* ACE_LOG_MSG_NT_EVENT_LOG_H */
