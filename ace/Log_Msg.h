/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Log_Msg.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_LOG_MSG_H)
#define ACE_LOG_MSG_H

// This stuff must come first to avoid problems with circular
// headers...

// The following ASSERT macro is courtesy of Alexandre Karev
// <akg@na47sun05.cern.ch>.
#if defined (ACE_NDEBUG)
#define	ACE_ASSERT(x)
#else
#define	ACE_ASSERT(X) \
  do { if(!(X)) { int __ace_error = errno; ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
  ace___->set (__FILE__, __LINE__, -1, __ace_error, ace___->restart (), ace___->msg_ostream ()); \
  ace___->log (LM_ERROR, "ACE_ASSERT: file %N, line %l assertion failed for '%s'.%a\n", #X, -1); \
  } } while (0)
#endif	/* ACE_NDEBUG */

#if defined (ACE_NLOGGING)
#define ACE_HEX_DUMP(X) 
#define ACE_RETURN(Y) do { return (Y); } while (0)
#define ACE_ERROR_RETURN(X, Y) return (Y)
#define ACE_ERROR_BREAK(X) { break; }
#define ACE_ERROR(X) 
#define ACE_DEBUG(X) 
#define ACE_ERROR_INIT(VALUE, FLAGS)
#else
#define ACE_HEX_DUMP(X) \
  do { int __ace_error = errno; \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->set (__FILE__, __LINE__, 0, __ace_error, ace___->restart (), \
    ace___->msg_ostream ()); \
    ace___->log_hexdump X; \
   } while (0)
#define ACE_RETURN(Y) \
  do { int __ace_error = errno; \
    ACE_Log_Msg::instance ()->set (__FILE__, __LINE__, Y, __ace_error); \
    return Y; \
  } while (0)
#define ACE_ERROR_RETURN(X, Y) \
  do { int __ace_error = errno; \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->set (__FILE__, __LINE__, Y, __ace_error, ace___->restart (), ace___->msg_ostream ()); \
    ace___->log X; \
    return Y; \
  } while (0)
#define ACE_ERROR(X) \
  do { int __ace_error = errno; \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->set (__FILE__, __LINE__, -1, __ace_error, ace___->restart (), ace___->msg_ostream ()); \
    ace___->log X; \
  } while (0)
#define ACE_DEBUG(X) \
  do { int __ace_error = errno; \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->set (__FILE__, __LINE__, 0, __ace_error, ace___->restart (), ace___->msg_ostream ()); \
    ace___->log X; \
  } while (0)
#define ACE_ERROR_INIT(VALUE, FLAGS) \
  do { \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->set_flags (FLAGS); ace___->op_status (VALUE); \
  } while (0)
#define ACE_ERROR_BREAK(X) { ACE_ERROR (X); break; }
#endif /* ACE_NLOGGING */

#include "ace/Log_Record.h"

#define ACE_LOG_MSG ACE_Log_Msg::instance ()

class ACE_Export ACE_Log_Msg 
  // = TITLE
  //     Provides a variable length argument message logging
  //     abstraction.
  //
  // = DESCRIPTION
  //     This class is very flexible since it allows formatted error
  //     messages to be printed in a thread-safe manner to stderr or a
  //     distributed logger.  Moreover, the message is kept in a
  //     thread-specific storage location, which can be used to
  //     communicate errors between framework methods and callers.
{
public:
  // Logger Flags.
  enum 
  {
    STDERR = 01,  
    // Write messages to stderr. 
    LOGGER = 02,  
    // Write messages to the local client logger deamon.
    OSTREAM = 04,
    // Write messages to the ostream * stored in thread-specific
    // storage.
    VERBOSE = 010,
    // Display messages in a verbose manner.
    SILENT = 020  
    // Do not print messages at all (just leave in thread-specific
    // storage for later inspection).
  };

  // = Initialization and termination routines.

  static ACE_Log_Msg *instance (void);
  // Returns a pointer to the Singleton.

  ACE_Log_Msg (void);
  // Initialize logger.

  ~ACE_Log_Msg (void);

  int open (const char *prog_name, 
	    u_long options_flags = ACE_Log_Msg::STDERR, 
	    LPCTSTR logger_key = 0);
  // Initialize the ACE error handling facility.  <prog_name> is the
  // name of the executable program.  <flags> are a bitwise-or of
  // options flags passed to the Logger (see the enum above for the valid
  // values).  If the <LOGGER> bit in <flags> is enabled then
  // <logger_key> is the name of ACE_FIFO rendezvous point where the
  // local client logger daemon is listening for logging messages.

  // = Set/get the options flags.

  void set_flags (u_long f);
  // Enable the bits in the logger's options flags.
  void clr_flags (u_long f);
  // Disable the bits in the logger's options flags.
  u_long flags (void);
  // Return the bits in the logger's options flags.

  // = Operations that allow applications to acquire and release the
  // synchronization lock used internally by the ACE_Log_Msg
  // implementation.  This allows applications to hold the lock
  // atomically over a number of calls to ACE_Log_Msg.
  int acquire (void);
  // Acquire the internal lock.
  int release (void);
  // Release the internal lock.

  void sync (const char *program_name);
  // Call after doing a fork() to resynchronize the PID and
  // PROGRAM_NAME variables.

  // = Set/get methods.  Note that these are non-static and thus will
  // be thread-specific.

  void op_status (int status);
  // Set the result of the operation status (by convention, -1 means error).

  int op_status (void);
  // Get the result of the operation status (by convention, -1 means error).

  void errnum (int);
  // Set the value of the errnum (by convention this corresponds to errno).

  int errnum (void);
  // Get the value of the errnum (by convention this corresponds to errno).

  void linenum (int);
  // Set the line number where an error occurred.

  int linenum (void);
  // Get the line number where an error occurred.

  void file (const char *);
  // Set the file name where an error occurred.

  const char *file (void);
  // Get the file name where an error occurred.

  void msg (char *);
  // Set the message that describes what type of error occurred.

  char *msg (void);
  // Get the message that describes what type of error occurred.

  void restart (int);
  // Set the field that indicates whether interrupted calls should be
  // restarted.

  int restart (void);
  // Get the field that indicates whether interrupted calls should be
  // restarted.

  void msg_ostream (ostream *);
  // Set the ostream that is used to print error messages.

  ostream *msg_ostream (void);
  // Get the ostream that is used to print error messages.

  // = Nesting depth increment and decrement.
  int inc (void);
  int dec (void);

  // = Get/set trace active status.
  int trace_active (void);
  void trace_active (int value);

  // = Get/set the current thread state.
  ACE_Thread_State *thr_state (void);
  void thr_state (ACE_Thread_State *);

  // = Get/set the current thread ACE_hthread_t.
  ACE_hthread_t *thr_handle (void);
  void thr_handle (ACE_hthread_t *);

  // = Stop/start/query tracing status on a per-thread basis...
  void stop_tracing (void);
  void start_tracing (void);
  int  tracing_enabled (void);

  // = Get/set the priority mask.
  u_long priority_mask (void);
  // Get the current <ACE_Log_Priority> mask.
  u_long priority_mask (u_long);
  // Set the <ACE_Log_Priority> mask, returns original mask.

  pid_t getpid (void) const; 
  // Optimize reading of the pid (avoids a system call if the
  // value is cached...).

  // = Set/get the name of the local host.
  const char *local_host (void) const;
  void local_host (const char *);

  void set (const char *file, 
	    int line,
	    int op_status = -1,
	    int errnum = 0,
	    int restart = 1,
	    ostream *os = 0);
  // Set the line number, file name, operational status, error number,
  // restart flag, and ostream.  This combines all the other set
  // methods into a single method.

  ssize_t log (ACE_Log_Priority priority, const char *format, ...);
  // Format a message to the thread-safe ACE logging mechanism.  Valid
  // options (prefixed by '%', as in printf format strings) include:
  // 'a': exit the program at this point (var-argument is the exit status!)
  // 'c': print a character
  // 'i', 'd': print a decimal number
  // 'e', 'E', 'f', 'F', 'g', 'G': print a double
  // 'l', print line number where an error occurred.
  // 'N': print file name where the error occurred.
  // 'n': print the name of the program (or "<unknown>" if not set)
  // 'o': print as an octal number
  // 'P': print out the current process id
  // 'p': print out the appropriate errno value from sys_errlist
  // 'r': call the function pointed to by the corresponding argument
  // 'R': print return status
  // 'S': print out the appropriate _sys_siglist entry corresponding to var-argument.
  // 's': print out a character string
  // 'T': print timestamp in hour:minute:sec:usec format.
  // 't': print thread id (1 if single-threaded)
  // 'u': print as unsigned int
  // 'X', 'x': print as a hex number
  // '%': print out a single percent sign, '%' 

  ssize_t log (const char *format, ACE_Log_Priority priority, va_list argp);
  // An alternative logging mechanism that makes it possible to
  // integrate variable argument lists from other logging mechanisms
  // into the ACE mechanism.

  int log_hexdump (ACE_Log_Priority log_priority, 
		   char *buffer, 
		   int size,
		   char *text = 0);
  // Method to log hex dump.  This is useful for debugging.  Calls
  // <log> to do the actual print, but formats first to make the chars
  // printable.
  
  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int status_;
  // Status of operation (-1 means failure, >= 0 means success).

  int errnum_;
  // Type of error that occurred (see <sys/errno.h>).

  int linenum_;
  // Line number where the error occurred.

  char file_[MAXNAMELEN];
  // File where the error occurred.

  char msg_[ACE_Log_Record::MAXLOGMSGLEN];
  // The error message.

  int restart_;
  // Indicates whether we should restart system calls that are
  // interrupted.

  ostream *ostream_;
  // The ostream where logging messages can be written.

  int trace_depth_;
  // Depth of the nesting for printing traces.

  ACE_hthread_t *thr_handle_;
  // If we're running in the context of an <ACE_Thread_Manager> this
  // will point to the <thr_handle_> field in the
  // <ACE_Thread_Descriptor>.  Threads can use this to rapidly
  // determine their real ACE_hthread_t.

  int trace_active_;
  // Are we already within an ACE_Trace constructor call?

  int tracing_enabled_;
  // Are we allowing tracing in this thread?

  ACE_Thread_State *thr_state_;
  // If we're running in the context of an <ACE_Thread_Manager> this
  // will point to the <thr_state_> field in the
  // <ACE_Thread_Descriptor>.  Threads can use this to rapidly
  // determine if they've got a cancellation pending.

  u_long priority_mask_;
  // Keeps track of all the <ACE_Log_Priority> values that are
  // currently enabled.  Default is for all logging priorities to be
  // enabled.

  // = The following fields are *not* kept in thread-specific storage
  // since we only want one instance for the entire process!
  
  static const char *program_name_; 
  // Records the program name. 

  static const char *local_host_;
  // Name of the local host (used when printing messages).

  static pid_t pid_;
  // Process id of the current process. 

  static u_long flags_;
  // Options flags.

  static int msg_off_;
  // Offset of msg_[].
};

// #if defined (__ACE_INLINE__)
// #include "ace/Log_Msg.i"
// #endif /* __ACE_INLINE__ */
#endif /* ACE_LOG_MSG_H */
