/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Process.h
//
// = AUTHOR
//    Tim Harrison <harrison@cs.wustl.edu>
// 
// ============================================================================

#if !defined (ACE_PROCESS_H)
#define ACE_PROCESS_H

#include "ace/OS.h"

class ACE_Export ACE_Process
// = TITLE
//     A Portable encapsulation for creating new processes and
//     allows assignment of STDIN, STDOUT, and STDERR of the new
//     process. 
//
// = DESCRIPTION
//     On UNIX, ACE_Process uses fork and exec.  On Win32, it uses
//     CreateProcess.  Since we can set the standard handles, we can
//     mimic UNIX pipes on Win32 by building chains of processes.
//     This class should be used instead ACE_OS::fork_exec.  I'm
//     implementing the functionality that I need as I go, instead of
//     trying to build an all encompassing process abstraction.  If
//     anyone needs more functionality, please feel free to add it and
//     send us the updates.  We'll put it in ACE.
{
public:
  ACE_Process (void);
  // Default construction.

  ACE_Process (char *argv[],
	       ACE_HANDLE std_in,
	       ACE_HANDLE std_out = ACE_INVALID_HANDLE,
	       ACE_HANDLE std_err = ACE_INVALID_HANDLE);
  // Set the standard handles of the new process to the respective
  // handles and start the new process.  -argv- must be specified.  It
  // should be of the following form: argv = {
  // "c:\full\path\to\foo.exe", "-a", "arg1", "etc", 0 } Returns the
  // new process id on success, -1 on failure.  If you want to affect
  // a subset of the handles, make sure to set the others to
  // ACE_INVALID_HANDLE.

  ~ACE_Process (void);
  // Death incarnate.

  int set_handles (ACE_HANDLE std_in,
		   ACE_HANDLE std_out = ACE_INVALID_HANDLE,
		   ACE_HANDLE std_err = ACE_INVALID_HANDLE);
  // Set the standard handles of the new process to the respective
  // handles.  If you want to affect a subset of the handles, make
  // sure to set the others to ACE_INVALID_HANDLE.  Returns 0 on
  // success, -1 on failure.

  int start (char *argv[]);
  // Start the new process.  -argv- must be specified.  It should be
  // of the following form: argv = { "c:\full\path\to\foo.exe", "-a",
  // "arg1", "etc", 0 } Returns the new process id on success, -1 on
  // failure.

  int wait (void);
  // Wait for the process we just created to exit.

  int kill (int signum = SIGINT);
  // Send the process a signal.

  pid_t getpid (void);
  // Return the pid of the new process.
  
private:
#if defined (ACE_WIN32)
  PROCESS_INFORMATION process_info_;
  STARTUPINFO startup_info_;
  int set_handles_called_;
  // Is 1 if stdhandles was called.
#else /* ACE_WIN32 */
  ACE_HANDLE stdin_;
  ACE_HANDLE stdout_;
  ACE_HANDLE stderr_;
  pid_t child_id_;
#endif /* ACE_WIN32 */
};

#if defined (__ACE_INLINE__)
#include "ace/Process.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_PROCESS_H */
