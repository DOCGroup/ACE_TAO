// -*- C++ -*-

//=============================================================================
/**
 * @file      Process_Log.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _PROCESS_LOG_H_
#define _PROCESS_LOG_H_

#include /**/ "ace/pre.h"

#include "cuts/config.h"
#include "ace/SString.h"
#include "ace/Thread_Mutex.h"
#include "ace/Unbounded_Set.h"
#include "ace/iosfwd.h"

// Forward decl.
class Process_Log;

// Forward decl.
class Process_Log_Entry;

typedef ACE_Unbounded_Set <Process_Log_Entry> Process_List;

//=============================================================================
/**
 * @class Process_Log_Entry
 */
//=============================================================================

class Process_Log_Entry
{
  // Friend decl.
  friend class Process_Log;

public:
  Process_Log_Entry (void);

  Process_Log_Entry (const Process_Log_Entry & entry);

  bool is_localhost (void) const;

  bool is_active (void) const;

  const Process_Log_Entry & operator = (const Process_Log_Entry &);

  u_short port (void) const;

  pid_t pid (void) const;

  bool operator == (const Process_Log_Entry & entry);

private:
  enum Flags
  {
    /// The process is active.
    PLE_ACTIVE  =   0x0001,

    /// The port visibility is local.
    PLE_LOCAL   =   0x0002
  };

  /// Flags for the entry.
  u_short flags_;

  /// Process id for the entry.
  pid_t pid_;

  /// The port number of the entry.
  u_short port_;
};

//=============================================================================
/**
 * @class Process_Log
 *
 * Logging mechanism for process spawned by a process manager. The
 * Process_Log is responsible for serializing the state of process.
 * In the case of failure/recovery, the process manager can learn
 * about which processes it was managing.
 */
//=============================================================================

class Process_Log
{
public:
  /// Default constructor.
  Process_Log (void);

  /**
   * Initializing constructor.
   *
   * @param[in]     log       Name of the log.
   */
  Process_Log (const char * log);

  /// Destructor
  ~Process_Log (void);

  /**
   * Set the name of the log file.
   *
   * @param[in]     log       Name of the log.
   */
  void log_file (const char * log);

  /**
   * Get the name of the log file.
   *
   * @return      Name of the log file (including path).
   */
  const ACE_CString & log_file (void) const;

  bool process_spawn (pid_t id, u_short port, bool localhost);

  bool process_exit (pid_t id);

  bool get_active_processes (Process_List & list);

  bool clean (size_t * active_count = 0);

private:
  static size_t batch_read (std::istream & in,
                            Process_Log_Entry * buffer,
                            size_t bufsize);

  /// Name of the log file.
  ACE_CString log_file_;

  /// Locking mechanism for synchronized access.
  ACE_Thread_Mutex lock_;
};

#if defined (__CUTS_INLINE__)
#include "Process_Log.inl"
#endif

#include /**/ "ace/post.h"

#endif  // _PROCESS_LOG_H_
