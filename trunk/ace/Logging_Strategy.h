/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Logging_Strategy.h
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_LOGGING_STRATEGY_H
#define ACE_LOGGING_STRATEGY_H

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (ACE_DEFAULT_MAX_LOGFILE_SIZE)
#define ACE_DEFAULT_MAX_LOGFILE_SIZE 16384 /* KB */
#endif /* ACE_DEFAULT_MAX_LOGFILE_SIZE */

class ACE_Logging_Strategy : public ACE_Service_Object
{
  // = TITLE
  //     This class provides the hooks to control the output produced
  //     by any of the network services.
  //
  // = DESCRIPTION
  //     Depending upon when this service is invoked and with what
  //     flags, the output of other network services can be
  //     controlled. The output can be streamed to stderr, to a file,
  //     to a logging daemon, or it can be set to be "silent".
  //     
  //     If logging records are output to a file, the file can be set
  //     to a maximum size and repeatedly split into new files.  The
  //     log file size can be limited at any logging point (i.e.,
  //     application, client logging daemon, or server logging daemon)
  //     by specifying the -i <sample_interval_in_secs> and -m
  //     <max_size_in_KB> options for the Logging_Strategy class in a
  //     svc.conf file.
public:
  ACE_Logging_Strategy (void);
  // Constructor.

  virtual int init (int argc, char *argv[]);
  // Dynamic linking initialization hook.

  virtual int fini (void);
  // Dynamic linking termination hook.

  virtual int handle_timeout (const ACE_Time_Value& tv, const void* arg);
  // Timeout handler which tests logfile size.  If the current logfile
  // size exceeds <max_size_>, the current logfile is closed, saved to
  // logfile.old, and a new logfile is reopened.

  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.

private:
  void tokenize (char *flag_string);
  // Tokenize to set all the flags

  u_long flags_;
  // Flags we keep track of.

  char *filename_;
  // File name we're logging to.

  int wipeout_logfile_;
  // If non-0 then wipeout the logfile, otherwise append to it.

  u_long interval_;
  // If non-zero, sampling interval (in secs) at which maximum logfile
  // size is checked, otherwise logfile size can grow indefinitely.

  u_long max_size_;
  // Maximum logfile size (in KB).
};

ACE_SVC_FACTORY_DECLARE (ACE_Logging_Strategy)

#endif /* ACE_LOGGING_STRATEGY_H */
