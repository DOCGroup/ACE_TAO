// -*- C++ -*-

//=============================================================================
/**
 *  @file    Log_Msg_NT_Event_Log.h
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef ACE_LOG_MSG_NT_EVENT_LOG_H
#define ACE_LOG_MSG_NT_EVENT_LOG_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined ACE_HAS_LOG_MSG_NT_EVENT_LOG

#include "ace/Log_Msg_Backend.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

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
  ACE_Log_Msg_NT_Event_Log () = default;

  /// Destructor
  ~ACE_Log_Msg_NT_Event_Log () override;

  /// Open a new event log.
  /**
   * Initialize the event logging facility.
   * @param logger_key The name of the calling program. This name is
   *                   used in the Source field of the event log. If
   *                   it is 0 (no name), the application name as
   *                   returned from ACE_Log_Msg::program_name() is used.
   */
  int open (const ACE_TCHAR *logger_key) override;

  /// Reset the backend.
  int reset () override;

  /// Close the backend completely.
  int close () override;

  /// This is called when we want to log a message.
  ssize_t log (ACE_Log_Record &log_record) override;

private:
  HANDLE evlog_handle_ {0};
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_LOG_MSG_NT_EVENT_LOG */

#include /**/ "ace/post.h"
#endif /* ACE_LOG_MSG_NT_EVENT_LOG_H */
