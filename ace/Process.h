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

class ACE_Export ACE_Process_Options
// = TITLE
//    Process Options
//
// = DESCRIPTION
//    This class controls the options passed to CreateProcess (or fork
//    and exec).
{
public:
  enum { DEFAULT_COMMAND_LINE_BUF_LEN = 1024 };

  ACE_Process_Options (int inherit_environment = 1,
		       int command_line_buf_len = DEFAULT_COMMAND_LINE_BUF_LEN);
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

  u_long creation_flags (void) const;
  // Get.
  void creation_flags (u_long);
  // Set.

  STARTUPINFO *startup_info (void);
  // Used for setting and getting.

  const LPSECURITY_ATTRIBUTES get_process_attributes (void) const;
  // Get the process_attributes.  Returns NULL if
  // set_process_attributes has not been set.
  LPSECURITY_ATTRIBUTES set_process_attributes (void);
  // If this is called, a non-null process attributes is sent to
  // CreateProcess.
  const LPSECURITY_ATTRIBUTES get_thread_attributes (void) const;
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

  int setenv_i (LPTSTR assignment, int len);
  // Add <assignment> to environment_buf_ and adjust
  // environment_argv_.  <len> is the strlen of <assignment>.

  enum { 
    MAX_COMMAND_LINE_OPTIONS = 128,
    ENVIRONMENT_BUFFER = 8192,
    MAX_ENVIRONMENT_ARGS = 128
  };

  int inherit_environment_;
  // Whether the child process inherits the current process
  // environment.

#if defined (ACE_WIN32)
  void inherit_environment (void);
  // Helper function to grab win32 environment and stick it in
  // environment_buf_ using this->setenv_i.

  int environment_inherited_;
  // Ensures once only call to inherit environment.

  STARTUPINFO startup_info_;
  int set_handles_called_;
  // Is 1 if stdhandles was called.

  BOOL handle_inheritence_;
  // Default TRUE.

  u_long creation_flags_;
  // Default 0.

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

  int environment_buf_index_;
  // Pointer into environment_buf_.  This should point to the next
  // free spot.

  int environment_argv_index_;
  // Pointer to environment_argv_.

  TCHAR environment_buf_[ENVIRONMENT_BUFFER];
  // Buffer containing all environment strings.

  LPTSTR environment_argv_[MAX_ENVIRONMENT_ARGS];
  // Pointers into environment_buf_.

  LPTSTR command_line_buf_;
  // Pointer to buffer of command-line arguments.  E.g., "-f foo -b bar".

  LPTSTR command_line_argv_[MAX_COMMAND_LINE_OPTIONS];
  // Argv-style command-line arguments.

  int command_line_argv_calculated_;
  // Ensures command_line_argv is only calculated once.

  TCHAR working_directory_[MAXPATHLEN + 1];
  // The current working directory.
};

// ************************************************************

class ACE_Export ACE_Process
// = TITLE
//     Process
//
// = DESCRIPTION
//     A Portable encapsulation for creating new processes.
{
public:
  ACE_Process (void);
  // Default construction.  Must use ACE_Process::start.

  ~ACE_Process (void);
  // Destructor.

  pid_t spawn (ACE_Process_Options &options);
  // Launch the process described by <options>.

  int wait (void);
  // Wait for the process we just created to exit.

  int wait (const ACE_Time_Value &tv);
  // Timed wait for the process we just created to exit.

  int kill (int signum = SIGINT);
  // Send the process a signal.

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

// ************************************************************

class ACE_Export ACE_Tokenizer
// = TITLE
//    Tokenizer
//
// = DESCRIPTION
//    Tokenizes a buffer.  Allows application to set delimiters and
//    preserve designators.  Does not allow special characters, yet
//    (e.g., printf ("\"like a quoted string\"").
{
public:
  ACE_Tokenizer (LPTSTR buffer);
  // <buffer> will be parsed.

  int delimiter (TCHAR d);
  // <d> is a delimiter.  Returns 0 on success, -1 if there is no
  // memory left.

  int delimiter_replace (TCHAR d, TCHAR replacement);
  // <d> is a delimiter and, when found, will be replaced by
  // <replacement>.  Returns 0 on success, -1 if there is no memory
  // left.

  int preserve_designators (TCHAR start, TCHAR stop, int strip=1);
  // For instance, quotes, or '(' and ')'.  Returns 0 on success, -1
  // if there is no memory left.  If <strip> == 1, then the preserve
  // designators will be stripped from the tokens returned by next.

  LPTSTR next (void);
  // Returns the next token.

  enum { 
    MAX_DELIMITERS=16,
    MAX_PRESERVES=16
  };

protected:
  int is_delimiter (TCHAR d, int &replace, TCHAR &r);
  // Returns 1 if <d> is a delimiter, 0 otherwise.  If <d> should be
  // replaced with <r>, <replace> is set to 1, otherwise 0.

  int is_preserve_designator (TCHAR start, TCHAR &stop, int &strip);
  // If <start> is a start preserve designator, returns 1 and sets
  // <stop> to the stop designator.  Returns 0 if <start> is not a
  // preserve designator.
 
private:
  LPTSTR buffer_;
  int index_;

  struct Preserve_Entry
    // = TITLE
    //    Preserve Entry
    // = DESCRIPTION
    //    Defines a set of characters that designate an area that
    //    should not be parsed, but should be treated as a complete
    //    token.  For instance, in: (this is a preserve region), start
    //    would be a left paren -(- and stop would be a right paren
    //    -)-.  The strip determines whether the designators should be
    //    removed from the token.
  {
    TCHAR start_;
    // E.g., "(".
    TCHAR stop_;
    // E.g., ")".
    int strip_;
    // Whether the designators should be removed from the token.
  };

  Preserve_Entry preserves_[MAX_PRESERVES];
  // The application can specify MAX_PRESERVES preserve designators. 

  int preserves_index_;
  // Pointer to the next free spot in preserves_.

  struct Delimiter_Entry
    // = TITLE
    //    Delimiter Entry
    // = DESCRIPTION
    //    Describes a delimiter for the tokenizer.
  {
    TCHAR delimiter_;
    // Most commonly a space ' '.
    TCHAR replacement_;
    // What occurrences of delimiter_ should be replaced with.
    int replace_;
    // Whether replacement_ should be used.  This should be replaced
    // with a technique that sets replacement_ = delimiter by
    // default.  I'll do that next iteration.
  };

  Delimiter_Entry delimiters_[MAX_DELIMITERS];
  // The tokenizer allows MAX_DELIMITERS number of delimiters.

  int delimiter_index_;
  // Pointer to the next free space in delimiters_.
};

#if defined (__ACE_INLINE__)
#include "ace/Process.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_PROCESS_H */
