/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Logging_Strategy.h
 *
 *  $Id$
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_LOGGING_STRATEGY_H
#define ACE_LOGGING_STRATEGY_H

#include "ace/Service_Object.h"
#include "ace/Log_Msg.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (ACE_DEFAULT_MAX_LOGFILE_SIZE)
#define ACE_DEFAULT_MAX_LOGFILE_SIZE 16384 /* KB */
#endif /* ACE_DEFAULT_MAX_LOGFILE_SIZE */

/**
 * @class ACE_Logging_Strategy
 *
 * @brief This class provides the hooks to control the output produced
 * by any of the network services.
 *
 * Depending upon when this service is invoked and with what
 * flags, the output of other network services can be
 * controlled. The output can be streamed to stderr, to a file,
 * to a logging daemon, or it can be set to be "silent".
 * If logging records are output to a file, the file can be set
 * to a maximum size and repeatedly split into new files.  The
 * log file size can be limited at any logging point (i.e.,
 * application, client logging daemon, or server logging daemon)
 * by specifying the -i <sample_interval_in_secs> and -m
 * <max_size_in_KB> options for the Logging_Strategy class in a
 * svc.conf file.
 */
class ACE_Export ACE_Logging_Strategy : public ACE_Service_Object
{
public:
  /// Constructor.
  ACE_Logging_Strategy (void);

  /// Dynamic linking initialization hook.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Dynamic linking termination hook.
  virtual int fini (void);

  /**
   * Timeout handler which tests logfile size.  If the current logfile
   * size exceeds <max_size_>, the current logfile is closed, saved to
   * logfile.old, and a new logfile is reopened.
   */
  virtual int handle_timeout (const ACE_Time_Value& tv, const void* arg);

  /// Parse svc.conf arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

private:
  /// Tokenize to set all the flags
  void tokenize (ACE_TCHAR *flag_string);

  /// Tokenize to set priorities (either process or thread one).
  void priorities (ACE_TCHAR *priority_string,
                   ACE_Log_Msg::MASK_TYPE mask);

  /// Current thread's priority mask set by <priorities>
  u_long thread_priority_mask_;

  /// Process-wide priority mask set by <priorities>
  u_long process_priority_mask_;

  /// Flags we keep track of.
  u_long flags_;

  /// File name we're logging to.
  ACE_TCHAR *filename_;

  /// If non-0 then wipeout the logfile, otherwise append to it.
  int wipeout_logfile_;

  /// If non-zero, sampling interval (in secs) at which maximum logfile
  /// size is checked, otherwise logfile size can grow indefinitely.
  u_long interval_;

  /// Maximum logfile size (in KB).
  u_long max_size_;
};

ACE_FACTORY_DECLARE (ACE, ACE_Logging_Strategy)

#endif /* ACE_LOGGING_STRATEGY_H */
