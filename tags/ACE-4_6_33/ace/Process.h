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

#ifndef ACE_PROCESS_H
#define ACE_PROCESS_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Process_Options
{
  // = TITLE
  //    Process Options
  //
  // = DESCRIPTION
  //    This class controls the options passed to <CreateProcess> (or <fork>
  //    and <exec>).
  //    Notice that on Windows CE, creating a process merely means
  //    instantiating a new process.  You can't set the handles (since
  //    there's no stdin, stdout and stderr,) specify process/thread
  //    options, set environment,...  So, basically, this class only
  //    set the command line and nothing else.
  //
  //    Notice that on UNIX platforms, if the <setenv> is used, the
  //    <spawn> is using the <execve> system call. It means that the
  //    <command_line> should include a full path to the program file
  //    (<execve> does not search the PATH).  If <setenv> is not used
  //    then, the <spawn> is using the <execvp> which searches for the
  //    program file in the PATH variable.
public:
  enum
  {
    DEFAULT_COMMAND_LINE_BUF_LEN = 1024,
    // UNIX process creation flags.
#if defined (ACE_WIN32)
    NO_EXEC = 0
#else
    NO_EXEC = 1
#endif /* ACE_WIN32 */
  };

protected:
  // = Default settings not part of public Interface.
  //
  // @@ These sizes should be taken from the appropriate
  // POSIX/system header files and/or defined dynamically.
  enum 
  {
    MAX_COMMAND_LINE_OPTIONS = 128,
    ENVIRONMENT_BUFFER = 16 * 1024, // 16K
    MAX_ENVIRONMENT_ARGS = 512 //
  };

public:
  ACE_Process_Options (int inherit_environment = 1,
                       int command_line_buf_len = DEFAULT_COMMAND_LINE_BUF_LEN,
                       int env_buf_len = ENVIRONMENT_BUFFER,
                       int max_env_args = MAX_ENVIRONMENT_ARGS);
  // If <inherit_environment> == 1, the new process will inherit the
  // environment of the current process.  <command_line_buf_len> is the
  // max strlen for command-line arguments.

  ~ACE_Process_Options (void);
  // Death.

  // ************************************************************
  // = These operations are used by applications to portably set
  // process creation options.
  // ************************************************************

  int set_handles (ACE_HANDLE std_in,
                   ACE_HANDLE std_out = ACE_INVALID_HANDLE,
                   ACE_HANDLE std_err = ACE_INVALID_HANDLE);
  // Set the standard handles of the new process to the respective
  // handles.  If you want to affect a subset of the handles, make
  // sure to set the others to ACE_INVALID_HANDLE.  Returns 0 on
  // success, -1 on failure.

  int setenv (LPCTSTR format, ...);
  // <format> must be of the form "VARIABLE= VALUE".  There can not be
  // any spaces between VARIABLE and the equal sign.

  int setenv (LPCTSTR variable_name, LPCTSTR format, ...);
  // Set a single environment variable, <variable_name>.  Since
  // different platforms separate each environment variable
  // differently, you must call this method once for each variable.
  // <format> can be any printf format string.
  // So options->setenv ("FOO","one + two = %s", "three") will result
  // in "FOO=one + two = three".

  int setenv (LPTSTR envp[]);
  // Same as above with argv format.  <envp> must be null terminated.

  void working_directory (LPCTSTR wd);
  // Set the working directory for the process.  strlen of <wd> must
  // be <= MAXPATHLEN.

  int command_line (LPCTSTR format, ...);
  // Set the command-line arguments.  <format> can use any printf
  // formats.  The first token in <format> should be the path to the
  // application.  This can either be a full path, relative path, or
  // just an executable name.  If an executable name is used, we rely
  // on the platform's support for searching paths.  Since we need a
  // path to run a process, this method *must* be called!  Returns 0
  // on success, -1 on failure.

  int command_line (LPTSTR argv[]);
  // Same as above in argv format.  <argv> must be null terminated.

  u_long creation_flags (void) const;
  // Get.
  void creation_flags (u_long);
  // Set.

  // ************************************************************
  // = These operations are used by ACE_Process to retrieve options
  // values.
  // ************************************************************

  LPTSTR working_directory (void);
  // Current working directory.  Returns "" if nothing has been set.

  LPTSTR command_line_buf (void);
  // Buffer of command-line options.  Returns exactly what was passed
  // to this->command_line.

  LPTSTR env_buf (void);
  // Null-terminated buffer of null terminated strings.  Each string
  // is an environment assignment "VARIABLE=value".  This buffer
  // should end with two null characters.

#if defined (ACE_WIN32)
  // = Non-portable accessors for when you "just have to use them."

  STARTUPINFO *startup_info (void);
  // Used for setting and getting.

  LPSECURITY_ATTRIBUTES get_process_attributes (void) const;
  // Get the process_attributes.  Returns NULL if
  // set_process_attributes has not been set.
  LPSECURITY_ATTRIBUTES set_process_attributes (void);
  // If this is called, a non-null process attributes is sent to
  // CreateProcess.
  LPSECURITY_ATTRIBUTES get_thread_attributes (void) const;
  // Get the thread_attributes.  Returns NULL if set_thread_attributes
  // has not been set.
  LPSECURITY_ATTRIBUTES set_thread_attributes (void);
  // If this is called, a non-null thread attributes is sent to
  // CreateProcess.

  int handle_inheritence (void);
  // Default is TRUE.
  void handle_inheritence (int);
  // Allows disabling of handle inheritence.
#else /* All things not WIN32 */

  char * const *command_line_argv (void);
  // argv-style command-line options.  Parses and modifies the string
  // created from this->command_line.  All spaces not in quotes ("" or
  // '') are replaced with null (\0) bytes.  An argv array is built
  // and returned with each entry pointing to the start of
  // null-terminated string.  Returns { 0 } if nothing has been set.

  char * const *env_argv (void);
  // argv-style array of environment settings.

  // = Accessors for the standard handles.
  ACE_HANDLE get_stdin (void);
  ACE_HANDLE get_stdout (void);
  ACE_HANDLE get_stderr (void);

#endif /* ACE_WIN32 */

protected:

#if !defined (ACE_HAS_WINCE)
  int setenv_i (LPTSTR assignment, int len);
  // Add <assignment> to environment_buf_ and adjust
  // environment_argv_.  <len> is the strlen of <assignment>.

  int inherit_environment_;
  // Whether the child process inherits the current process
  // environment.
#endif /* !ACE_HAS_WINCE */

  u_long creation_flags_;
  // Default 0.

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
  void inherit_environment (void);
  // Helper function to grab win32 environment and stick it in
  // environment_buf_ using this->setenv_i.

  int environment_inherited_;
  // Ensures once only call to inherit environment.

  STARTUPINFO startup_info_;

  BOOL handle_inheritence_;
  // Default TRUE.

  LPSECURITY_ATTRIBUTES process_attributes_;
  // Pointer to security_buf1_.

  LPSECURITY_ATTRIBUTES thread_attributes_;
  // Pointer to security_buf2_.

  SECURITY_ATTRIBUTES security_buf1_;
  // Data for process_attributes_.

  SECURITY_ATTRIBUTES security_buf2_;
  // Data for thread_attributes_.

#else /* !ACE_WIN32 */
  ACE_HANDLE stdin_;
  ACE_HANDLE stdout_;
  ACE_HANDLE stderr_;
#endif /* ACE_WIN32 */

#if !defined (ACE_HAS_WINCE)
  int set_handles_called_;
  // Is 1 if stdhandles was called.

  int environment_buf_index_;
  // Pointer into environment_buf_.  This should point to the next
  // free spot.

  int environment_argv_index_;
  // Pointer to environment_argv_.

  LPTSTR environment_buf_;
  // Pointer to buffer of the environment settings.

  int environment_buf_len_;
  // Size of the environment buffer. Configurable

  LPTSTR* environment_argv_;
  // Pointers into environment_buf_.

  int max_environment_args_;
  // Maximum number of environment variables. Configurable

  int max_environ_argv_index_;
  // Maximum index of environment_argv_ buffer

  TCHAR working_directory_[MAXPATHLEN + 1];
  // The current working directory.
#endif /* !ACE_HAS_WINCE */

  int command_line_argv_calculated_;
  // Ensures command_line_argv is only calculated once.

  LPTSTR command_line_buf_;
  // Pointer to buffer of command-line arguments.  E.g., "-f foo -b bar".

  LPTSTR command_line_argv_[MAX_COMMAND_LINE_OPTIONS];
  // Argv-style command-line arguments.
};

// ************************************************************

class ACE_Export ACE_Process
{
  // = TITLE
  //     Process
  //
  // = DESCRIPTION
  //     A Portable encapsulation for creating new processes.
  //
  //    Notice that on UNIX platforms, if the <setenv> is used, the
  //    <spawn> is using the <execve> system call. It means that the
  //    <command_line> should include a full path to the program file
  //    (<execve> does not search the PATH).  If <setenv> is not used
  //    then, the <spawn> is using the <execvp> which searches for the
  //    program file in the PATH variable.
public:
  ACE_Process (void);
  // Default construction.  Must use ACE_Process::start.

  ~ACE_Process (void);
  // Destructor.

  pid_t spawn (ACE_Process_Options &options);
  // Launch the process described by <options>.

  int wait (int *status = 0);
  // Wait for the process we just created to exit.

  int wait (const ACE_Time_Value &tv);
  // Timed wait for the process we just created to exit.

  int kill (int signum = SIGINT);
  // Send the process a signal.  This is only portable to operating
  // systems that support signals (e.g., POSIX).

  int terminate (void);
  // Terminate the process.  This call doesn't give the process a
  // chance to cleanup, so use with caution...

  pid_t getpid (void);
  // Return the pid of the new process.

#if defined (ACE_WIN32)
  PROCESS_INFORMATION process_info (void);
#endif /* ACE_WIN32 */

protected:
#if defined (ACE_WIN32)
  PROCESS_INFORMATION process_info_;
#else /* ACE_WIN32 */
  pid_t child_id_;
  // Process id of the child.
#endif /* ACE_WIN32 */
};

#include "ace/SString.h"

#if defined (__ACE_INLINE__)
#include "ace/Process.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_PROCESS_H */
