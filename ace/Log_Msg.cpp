// Log_Msg.cpp
// $Id$

#define ACE_BUILD_DLL
// We need this to get the status of ACE_NTRACE...
#include "ace/config.h"

// Turn off tracing for the duration of this file.
#if defined (ACE_NTRACE)
#undef ACE_NTRACE
#endif /* ACE_NTRACE */
#define ACE_NTRACE 1

// This must come first to avoid "order of include" problems...

#if !defined (ACE_HAS_INLINED_OSCALLS) && !defined (ACE_WIN32)
#define ACE_HAS_INLINED_OSCALLS
#include "ace/ACE.h"
#undef ACE_HAS_INLINED_OSCALLS
#else
#include "ace/ACE.h"
#endif /* !ACE_HAS_INLINED_OSCALLS */

#include "ace/Thread.h"
#include "ace/Synch.h"
#include "ace/Signal.h"

#if defined (ACE_HAS_UNICODE)
#define ACE_WSPRINTF(BUF,VALUE) ::wsprintf (BUF, "%S", VALUE)
#else
#define ACE_WSPRINTF(BUF,VALUE) ::sprintf (BUF, "%s", VALUE)
#endif /* ACE_HAS_UNICODE */

// IPC conduit between sender and client daemon.  This should be
// included in the <ACE_Log_Msg> class, but due to "order of include"
// problems it can't be...
#if defined (ACE_WIN32)
#include "ace/SPIPE_Connector.h"
static ACE_SPIPE_Stream message_queue_;
#else
#include "ace/FIFO_Send_Msg.h"
static ACE_FIFO_Send_Msg message_queue_;
#endif /* ACE_WIN32 */

ACE_ALLOC_HOOK_DEFINE(ACE_Log_Msg)

#if defined (ACE_MT_SAFE)
// Synchronize output operations.
static ACE_Thread_Mutex *lock_ = 0;

#if !defined(VXWORKS)
static ACE_thread_key_t key_;
#endif /* VXWORKS */

/* static */
#if defined (ACE_HAS_THR_C_FUNC)
extern "C" 
#endif /* ACE_HAS_THR_C_FUNC */
void
ACE_TSS_cleanup (void *ptr)
{
  delete (ACE_Log_Msg *) ptr;
}
#endif /* ACE_MT_SAFE */

// This is only needed here because we can't afford to call
// ACE_LOG_MSG->instance() from within ACE_Log_Msg::instance() or else
// we will recurse infinitely!

#define ACE_NEW_RETURN_I(POINTER,CONSTRUCTOR,RET_VAL) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
     } while (0)

ACE_Log_Msg *
ACE_Log_Msg::instance (void)
{
#if defined (ACE_MT_SAFE)
#if defined (VXWORKS)
  // TSS Singleton implementation for VxWorks.
  static int once_ = 0;

  // This isn't thread safe . . .
  if (once_ == 0 && lock_ == 0)
    {
      // Initialize the static recursive lock here.  Note that we
      // can't rely on the constructor being called at this point.
      ACE_NEW_RETURN_I (lock_, ACE_Thread_Mutex, 0);
      once_ = 1;
    }  

  // Get the tss_log_msg from thread-specific storage, using one of
  // the "spare" fields in the task control block.  Note that no locks
  // are required here since this is within our thread context.
  // This assumes that the sizeof the spare1 field is the same size
  // as a pointer; it is (it's an int) in VxWorks 5.2-5.3.
  ACE_Log_Msg **tss_log_msg = (ACE_Log_Msg **) &taskIdCurrent->spare1;

  // Check to see if this is the first time in for this thread.
  // This assumes that the spare1 field in the task control block is
  // initialized to 0, which holds true for VxWorks 5.2-5.3.
  if (*(int **) tss_log_msg == 0)
    // Allocate memory off the heap and store it in a pointer in
    // thread-specific storage (i.e., on the stack...).
    ACE_NEW_RETURN_I (*tss_log_msg, ACE_Log_Msg, 0);

  return *tss_log_msg;
#elif !defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
#error "Platform must support thread-specific storage if threads are used..."
#else
  // TSS Singleton implementation.

  static int once_ = 0;

  if (once_ == 0)
    {
      // Synchronize Singleton creation (note that this may lose big
      // if the compiler doesn't perform thread-safe initialization of
      // local statics...).
      static ACE_Thread_Mutex keylock_;
      ACE_thread_mutex_t &lock = (ACE_thread_mutex_t &) keylock_.lock ();

      ACE_OS::thread_mutex_lock (&lock);
      if (once_ == 0)
	{
	  // Initialize the static recursive lock here.  Note that we
	  // can't rely on the constructor being called at this point.
	  ACE_NEW_RETURN_I (lock_, ACE_Thread_Mutex, 0);

	  if (ACE_OS::thr_keycreate (&key_,
				     &ACE_TSS_cleanup) != 0)
	    {
	      ACE_OS::thread_mutex_unlock (&lock);
	      return 0; // Major problems, this should *never* happen!
	    }
	  once_ = 1;
	}
      ACE_OS::thread_mutex_unlock (&lock);
    }  

  ACE_Log_Msg *tss_log_msg = 0;

  // Get the tss_log_msg from thread-specific storage.  Note that no locks
  // are required here...
  if (ACE_OS::thr_getspecific (key_, 
			       (void **) &tss_log_msg) == -1)
    return 0; // This should not happen!

  // Check to see if this is the first time in for this thread.
  if (tss_log_msg == 0)
    {
      // Allocate memory off the heap and store it in a pointer in
      // thread-specific storage (on the stack...).

      ACE_NEW_RETURN_I (tss_log_msg, ACE_Log_Msg, 0);

      // Store the dynamically allocated pointer in thread-specific
      // storage.
      if (ACE_OS::thr_setspecific (key_, 
				   (void *) tss_log_msg) != 0)
	return 0; // Major problems, this should *never* happen!
    }

  return tss_log_msg;
#endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */
#else 
  // Singleton implementation.
  static ACE_Log_Msg log_msg;

  return &log_msg;
#endif /* defined (ACE_MT_SAFE) */
}
#undef ACE_NEW_RETURN_I

// Name of the local host.
const char *ACE_Log_Msg::local_host_ = 0;

// Records the program name. 
const char *ACE_Log_Msg::program_name_ = 0;

// Default is to use stderr.
u_long ACE_Log_Msg::flags_ = ACE_Log_Msg::STDERR;

// Process id of the current process. 
pid_t ACE_Log_Msg::pid_ = -1;

// Current offset of msg_[].
int ACE_Log_Msg::msg_off_ = 0;

// Call after a fork to resynchronize the PID and PROGRAM_NAME
// variables.

void
ACE_Log_Msg::sync (const char *prog_name)
{
  ACE_TRACE ("ACE_Log_Msg::sync");
  if (prog_name)
    {
      ACE_OS::free ((void *) ACE_Log_Msg::program_name_);
      ACE_Log_Msg::program_name_ = ACE_OS::strdup (prog_name);
    }
  ACE_Log_Msg::pid_ = ACE_OS::getpid ();  
  ACE_Log_Msg::msg_off_ = 0;
}

u_long
ACE_Log_Msg::flags (void)
{
  ACE_TRACE ("ACE_Log_Msg::flags");
  u_long result;
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *lock_, 0));

  result = ACE_Log_Msg::flags_;
  return result;
}

void
ACE_Log_Msg::set_flags (u_long flgs)
{
  ACE_TRACE ("ACE_Log_Msg::set_flags");
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, *lock_));

  ACE_SET_BITS (ACE_Log_Msg::flags_, flgs);
}

void
ACE_Log_Msg::clr_flags (u_long flgs)
{
  ACE_TRACE ("ACE_Log_Msg::clr_flags");
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, *lock_));

  ACE_CLR_BITS (ACE_Log_Msg::flags_, flgs);
}

int
ACE_Log_Msg::acquire (void)
{
  ACE_TRACE ("ACE_Log_Msg::acquire");
#if defined (ACE_MT_SAFE)
  return lock_->acquire ();
#else
  return 0;
#endif /* ACE_MT_SAFE */
}

u_long
ACE_Log_Msg::priority_mask (u_long n_mask) 
{
  u_long o_mask = this->priority_mask_;
  this->priority_mask_ = n_mask;
  return o_mask;
}

u_long
ACE_Log_Msg::priority_mask (void)
{
  return this->priority_mask_;
}

int
ACE_Log_Msg::release (void)
{
  ACE_TRACE ("ACE_Log_Msg::release");

#if defined (ACE_MT_SAFE)
  return lock_->release ();
#else
  return 0;
#endif /* ACE_MT_SAFE */
}

ACE_Log_Msg::ACE_Log_Msg (void)
  : status_ (0),
    errnum_ (0),
    linenum_ (0),
    restart_ (1),  // Restart by default...
    ostream_ (0),
    trace_depth_ (0),
    thr_handle_ (0),
    trace_active_ (0),
    tracing_enabled_ (1), // On by default?
    thr_state_ (0),
    priority_mask_ (LM_SHUTDOWN // By default, all priorities are enabled.
		    | LM_TRACE 
		    | LM_DEBUG
		    | LM_INFO
		    | LM_NOTICE
		    | LM_WARNING
		    | LM_STARTUP
		    | LM_ERROR
		    | LM_CRITICAL
		    | LM_ALERT
		    | LM_EMERGENCY)
{
  // ACE_TRACE ("ACE_Log_Msg::ACE_Log_Msg");
}

// Open the sender-side of the Message ACE_Queue.

int
ACE_Log_Msg::open (const char *prog_name, 
		   u_long flags, 
		   LPCTSTR logger_key)
{
  ACE_TRACE ("ACE_Log_Msg::open");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *lock_, -1));

  if (prog_name)
    ACE_Log_Msg::program_name_ = ACE_OS::strdup (prog_name);

  int status = 0;

  // Note that if we fail to open the message queue the default action
  // is to use stderr (set via static initialization in the
  // ACE_Log_Msg.C file).
  
  if (ACE_BIT_ENABLED (flags, ACE_Log_Msg::LOGGER))
    {
      if (logger_key == 0)
	status = -1;
      else
#if defined (ACE_WIN32)
	{
	  ACE_SPIPE_Connector con;
	  status = con.connect (message_queue_, ACE_SPIPE_Addr (logger_key));
	}
#else
	status = message_queue_.open (logger_key);
#endif /* ACE_WIN32 */

      if (status == -1)
	ACE_SET_BITS (ACE_Log_Msg::flags_, ACE_Log_Msg::STDERR);
      else
	ACE_SET_BITS (ACE_Log_Msg::flags_, ACE_Log_Msg::LOGGER);
    }

  // Remember, ACE_Log_Msg::STDERR bit is on by default... 
  if (status != -1 
      && ACE_BIT_ENABLED (flags, ACE_Log_Msg::STDERR) == 0)
    ACE_CLR_BITS (ACE_Log_Msg::flags_, ACE_Log_Msg::STDERR);

  if (ACE_BIT_ENABLED (flags, ACE_Log_Msg::VERBOSE))
    ACE_SET_BITS (ACE_Log_Msg::flags_, ACE_Log_Msg::VERBOSE);

  if (ACE_BIT_ENABLED (flags, ACE_Log_Msg::OSTREAM))
    {
      ACE_SET_BITS (ACE_Log_Msg::flags_, ACE_Log_Msg::OSTREAM);
      // Only set this to cerr if it hasn't already been set.
      if (ACE_LOG_MSG->msg_ostream () == 0)
	ACE_LOG_MSG->msg_ostream (&cerr);
    }
  if (ACE_BIT_ENABLED (flags, ACE_Log_Msg::SILENT))
    ACE_SET_BITS (ACE_Log_Msg::flags_, ACE_Log_Msg::SILENT);

  return status;
}

// Valid Options (prefixed by '%', as in printf format strings) include:
//   'a': exit the program at this point (var-argument is the exit status!)
//   'c': print a character
//   'i', 'd': print a decimal number
//   'e', 'E', 'f', 'F', 'g', 'G': print a double
//   'l', print line number where an error occurred.
//   'N': print file name where the error occurred.
//   'n': print the name of the program (or "<unknown>" if not set)
//   'o': print as an octal number
//   'P': format the current process id
//   'p': format the appropriate errno value from sys_errlist
//   'r': call the function pointed to by the corresponding argument
//   'R': print return status
//   'S': format the appropriate _sys_siglist entry corresponding to var-argument.
//   's': format a character string
//   'T': print timestamp in hour:minute:sec:usec format.
//   't': print thread id (1 if single-threaded)
//   'u': print as unsigned int
//   'X', 'x': print as a hex number
//   '%': format a single percent sign, '%' 

ssize_t
ACE_Log_Msg::log (ACE_Log_Priority log_priority, 
		  const char *format_str, ...)
{ 
  ACE_TRACE ("ACE_Log_Msg::log");

  // Start of variable args section. 
  va_list argp;		

  va_start (argp, format_str);

  int result = this->log (format_str, log_priority, argp);

  va_end (argp);

  return result;
}

ssize_t
ACE_Log_Msg::log (const char *format_str,
		  ACE_Log_Priority log_priority, 
		  va_list argp)
{ 
  ACE_TRACE ("ACE_Log_Msg::log");
  // External decls. 

  extern int sys_nerr;
  typedef void (*PTF)(...);

  // Only print the message if <priority_mask_> hasn't been reset to
  // exclude this logging priority.

  if (ACE_BIT_DISABLED (this->priority_mask_, log_priority))
    return 0;

  ACE_Log_Record log_record (log_priority, 
			     ACE_OS::gettimeofday (),
			     this->getpid ());
  char *bp = this->msg ();
  int abort_prog = 0;
  int exit_value = 0;
  int result = 0;
  char *format = ACE_OS::strdup (format_str);
  char *save_p = format; // Remember pointer for ACE_OS::free()

  if (format == 0)
    return -1;

  if (ACE_BIT_ENABLED (ACE_Log_Msg::flags_, ACE_Log_Msg::VERBOSE))
    {
      // Prepend the program name onto this message 

      if (ACE_Log_Msg::program_name_ != 0)
	{
	  for (const char *s = ACE_Log_Msg::program_name_; (*bp = *s) != '\0'; s++)
	    bp++;

	  *bp++ = '|';
	}
    }

  while (*format != '\0')
    {
      // Copy input to output until we encounter a %, however a
      // % followed by another % is not a format specification. 

      if (*format != '%')
	*bp++ = *format++;
      else if (format[1] == '%') // An "escaped" '%' (just print one '%').
	{
	  *bp++ = *format++;	// Store first % 
	  format++;	// but skip second % 
	}
      else
	{
	  char c;		// high use character	
	  char *fp;		// local format pointer	
	  int  wpc;		// width/precision cnt  
	  const int CONTINUE = 0;
	  const int SKIP_SPRINTF = -1;  // We must skip the sprintf phase 
	  const int SKIP_NUL_LOCATE = -2; // Skip locating the NUL character 
	  int type = CONTINUE;	// conversion type	
	  int  w[2];		// width/precision vals 

	  // % starts a format specification that ends with one of
	  // "arnPpSsdciouxXfFeEgG".  An optional width and/or precision
	  // (indicated by an "*") may be encountered prior to the
	  // nend of the specification, each consumes an int arg.
	  // A call to sprintf() does the actual conversion. 

	  fp = format++;	// Remember beginning of format.   
	  wpc = 0;		// Assume no width/precision seen. 

	  while (type == CONTINUE)
	    {
	      switch (*format++)
		{
		case 'a': // Abort program after handling all of format string. 
		  type = SKIP_SPRINTF;
		  abort_prog = 1; 
		  exit_value = va_arg (argp, int); 
		  ACE_OS::sprintf (bp, ""); // Make sure to NUL-terminate this...
		  break;
		case 'l':
		  ACE_OS::sprintf (bp, "%d", this->linenum ());
		  type = SKIP_SPRINTF;
		  break;
		case 'N':
		  {
		    // @@ UNICODE
		    const char *file = this->file ();
		    ACE_OS::sprintf (bp, "%s", file ? file : "<unknown file>");
		    type = SKIP_SPRINTF;
		    break;
		  }
		case 'n': // Print the name of the program. 
		  type = SKIP_SPRINTF;
		  // @@ UNICODE
		  ACE_OS::strcpy (bp, ACE_Log_Msg::program_name_ ? 
				  ACE_Log_Msg::program_name_ : "<unknown>");
		  break;
                case 'P': // Format the current process id.
		  type = SKIP_SPRINTF;
		  ACE_OS::sprintf (bp, "%d", this->getpid ());
		  break;
		case 'p': // Format the string assocated with the value of errno. 
		  {
		    type = SKIP_SPRINTF;
		    errno = ACE::map_errno (this->errnum ());
		    if (errno >= 0 && errno < sys_nerr)
		      ACE_OS::sprintf (bp, "%s: %s", 
				       va_arg (argp, char *), strerror (errno));
		    else
		      {
#if defined (ACE_WIN32)			
			LPVOID lpMsgBuf;
 
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				      NULL,
				      errno,
				      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				      (LPTSTR) &lpMsgBuf,
				      0,
				      NULL);

			ACE_OS::sprintf (bp, "%s: %s", 
					 va_arg (argp, char *), lpMsgBuf);

			// Free the buffer.
			LocalFree( lpMsgBuf );
#else
			ACE_OS::sprintf (bp, "%s: <unknown error> = %d",
				       va_arg (argp, char *), errno);
#endif /* ACE_WIN32 */		       
		      }
		    break;
		  }
		case 'R': // Format the return status of the operation.
		  this->op_status (va_arg (argp, int));
		  ACE_OS::sprintf (bp, "%d", this->op_status ());
		  break;
		case 'r': // Run (invoke) this subroutine. 
		  {
		    int osave = ACE_Log_Msg::msg_off_;
 
		    if (ACE_BIT_ENABLED (ACE_Log_Msg::flags_, ACE_Log_Msg::SILENT))
		      *bp++ = '{';
		    ACE_Log_Msg::msg_off_ =  bp - this->msg_;
 
		    type = SKIP_SPRINTF;
		    (*va_arg (argp, PTF))(); 

		    if (ACE_BIT_ENABLED (ACE_Log_Msg::flags_, ACE_Log_Msg::SILENT)) 
		      {
			bp += ACE_OS::strlen (bp);
			*bp++ =  '}';
		      }
		    *bp = '\0';

		    ACE_Log_Msg::msg_off_ = osave;
		    break;
		  }
		case 'S': // format the string associated with this signal number. 
		  {
		    int sig = va_arg (argp, int);
		    type = SKIP_SPRINTF;
#if defined (ACE_HAS_SYS_SIGLIST)
		    if (sig >= 0 && sig < NSIG)
		      ACE_OS::strcpy (bp, _sys_siglist[sig]);
		    else
		      ACE_OS::sprintf (bp, "<unknown signal> %d", sig);
#else
		    ACE_OS::sprintf (bp, "signal %d", sig);
#endif /* ACE_HAS_SYS_SIGLIST */
		    break;
		  }
		case 'T': // Format the timestamp.
		  {
		    type = SKIP_SPRINTF;
		    char day_and_time[35];
		    ACE_OS::sprintf (bp, "%s", 
				     ACE::timestamp (day_and_time, 
						     sizeof day_and_time));
		    break;		  
		  }
		case 't': // Format thread id. 
		  type = SKIP_SPRINTF;
#if defined (ACE_WIN32)
		  ACE_OS::sprintf (bp, "%u", ACE_Thread::self ());
#else
		  ACE_hthread_t t_id;
		  ACE_Thread::self (t_id);
		  ACE_OS::sprintf (bp, "%u", t_id);
#endif /* ACE_WIN32 */
		  break;
		case 's':
		  type = 1 + wpc; // 1, 2, 3 
		  break;
		case 'W':
		  // @@ UNICODE
		case 'd': case 'c': case 'i': case 'o': 
		case 'u': case 'x': case 'X': 
		  type = 4 + wpc; // 4, 5, 6 
		  break;
		case 'F': case 'f': case 'e': case 'E':
		case 'g': case 'G':
		  type = 7 + wpc; // 7, 8, 9 
		  break;
		case '*':	// consume width/precision 
		  w[wpc++] = va_arg (argp, int);
		  break;
		default:
		  // ? 
		  break;
		}
	    }

	  if (type != SKIP_SPRINTF)
	    {
	      c = *format;	// Remember char before we overwrite. 
	      *format = 0;	// Overwrite, terminating format. 

	      switch (type)
		{
		case 1:
		  ACE_OS::sprintf (bp, fp, va_arg (argp, char *));
		  break;
		case 2:
		  ACE_OS::sprintf (bp, fp, w[0], va_arg (argp, char *));
		  bp += w[0];
		  type = SKIP_NUL_LOCATE;
		  break;
		case 3:
		  ACE_OS::sprintf (bp, fp, w[0], w[1], va_arg (argp, char *));
		  bp += w[0];
		  type = SKIP_NUL_LOCATE;
		  break;
		case 4:
		  ACE_OS::sprintf (bp, fp, va_arg (argp, int));
		  break;
		case 5:
		  ACE_OS::sprintf (bp, fp, w[0], va_arg (argp, int));
		  break;
		case 6:
		  ACE_OS::sprintf (bp, fp, w[0], w[1], va_arg (argp, int));
		  break;
		case 7:
		  ACE_OS::sprintf (bp, fp, va_arg (argp, double));
		  break;
		case 8:
		  ACE_OS::sprintf (bp, fp, w[0], va_arg (argp, double));
		  break;
		case 9:
		  ACE_OS::sprintf (bp, fp, w[0], w[1], va_arg (argp, double));
		  break;
		}
	      *format = c;	// Restore char we overwrote. 
	    }

	  if (type != SKIP_NUL_LOCATE)
	    while (*bp != '\0')	// Locate end of bp. 
	      bp++;
	}
    }

  *bp = '\0'; // Terminate bp, but don't auto-increment this!

  ACE_OS::free (ACE_MALLOC_T (save_p));

  // Only print the message if "SILENT" mode is disabled.
  if (ACE_BIT_DISABLED (ACE_Log_Msg::flags_, ACE_Log_Msg::SILENT))
    {
      // Copy the message from thread-specific storage into the
      // transfer buffer (this can be optimized away by changing other
      // code...).
      log_record.msg_data (this->msg ());
      this->stop_tracing ();

      // Make this block signal safe.
      ACE_Sig_Guard sb;

      // Make sure that the lock is held during all this.
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *lock_, -1));

      if (ACE_BIT_ENABLED (ACE_Log_Msg::flags_, ACE_Log_Msg::STDERR) 
	  && abort_prog == 0) // We'll get this further down.
	log_record.print (ACE_Log_Msg::local_host_,
			  ACE_BIT_ENABLED (ACE_Log_Msg::flags_, ACE_Log_Msg::VERBOSE),
			  stderr);
      if (ACE_BIT_ENABLED (ACE_Log_Msg::flags_, ACE_Log_Msg::LOGGER))
	{
	  ACE_Str_Buf log_msg ((void *) &log_record, 
			       int (log_record.length ()));
#if defined (ACE_HAS_STREAM_PIPES)
	  result = message_queue_.send (int (log_record.type ()), 
					&log_msg);
#elif !defined (ACE_WIN32)
	  result = message_queue_.send (log_msg);
#else
	  result = message_queue_.send ((const ACE_Str_Buf *) &log_msg, 
					(const ACE_Str_Buf *) 0);
#endif /* ACE_HAS_STREAM_PIPES */
	}
      // Format the message and print it to stderr and/or ship it
      // off to the log_client daemon, and/or print it to the
      // ostream.  This must come last, after the other two print
      // operations (see the ACE_Log_Record::print method for
      // details).

      if (ACE_BIT_ENABLED (ACE_Log_Msg::flags_, ACE_Log_Msg::OSTREAM)
	  && this->msg_ostream () != 0)
	log_record.print (ACE_Log_Msg::local_host_,
			  ACE_BIT_ENABLED (ACE_Log_Msg::flags_, ACE_Log_Msg::VERBOSE),
			  *this->msg_ostream ());
      this->start_tracing ();
    }

  if (abort_prog)
    {
      // *Always* print a message to stderr if we're aborting.  We
      // don't use verbose, however, to avoid recursive aborts if
      // something is hosed.
      log_record.print (ACE_Log_Msg::local_host_, 0);
      ACE_OS::exit (exit_value);
    }

  return result;
}

// Calls log to do the actual print, but formats first.

int 
ACE_Log_Msg::log_hexdump (ACE_Log_Priority log_priority, 
			  char *buffer, 
			  int size,
			  char *text)
{
  char buf[ACE_Log_Record::MAXLOGMSGLEN - ACE_Log_Record::VERBOSE_LEN - 58];
  // 58 for the HEXDUMP header;

  char msg_buf[80];

  buf[0] = 0; // in case size = 0

  int len = ACE::format_hexdump (buffer, size, buf, sizeof buf);

  int sz = 0;

  if (text)
    sz = ::sprintf (msg_buf, "%s - ", text);

  sz += ::sprintf (msg_buf + sz, "HEXDUMP %d bytes", size);

  if (len < size)
    ::sprintf (msg_buf + sz, " (showing first %d bytes)", len);

  // Now print out the formatted buffer.
  this->log (log_priority, "%s\n%s", msg_buf, buf);
  return 0;
}

void
ACE_Log_Msg::set (const char *filename, 
		  int line,
		  int status, 
		  int err, 
		  int rs,
		  ostream *os)
{
  ACE_TRACE ("ACE_Log_Msg::set");
  this->file (filename);
  this->linenum (line);
  this->op_status (status);
  this->errnum (err);
  this->restart (rs);
  this->msg_ostream (os);
}

void
ACE_Log_Msg::dump (void) const
{
  ACE_TRACE ("ACE_Log_Msg::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "status_ = %d\n", this->status_));
  ACE_DEBUG ((LM_DEBUG, "\nerrnum_ = %d\n", this->errnum_));
  ACE_DEBUG ((LM_DEBUG, "\nlinenum_ = %d\n", this->linenum_));
  ACE_DEBUG ((LM_DEBUG, "\nfile_ = %s\n", this->file_));
  ACE_DEBUG ((LM_DEBUG, "\nmsg_ = %s\n", this->msg_));
  ACE_DEBUG ((LM_DEBUG, "\nrestart_ = %d\n", this->restart_));
  ACE_DEBUG ((LM_DEBUG, "\nostream_ = %x\n", this->ostream_));
  ACE_DEBUG ((LM_DEBUG, "\nprogram_name_ = %s\n", this->program_name_ ? this->program_name_ : "<unknown>"));
  ACE_DEBUG ((LM_DEBUG, "\nlocal_host_ = %s\n", this->local_host_ ? this->local_host_ : "<unknown>"));
  ACE_DEBUG ((LM_DEBUG, "\npid_ = %d\n", this->getpid ()));
  ACE_DEBUG ((LM_DEBUG, "\nflags_ = %x\n", this->flags_));
  ACE_DEBUG ((LM_DEBUG, "\ntrace_depth_ = %d\n", this->trace_depth_));
  ACE_DEBUG ((LM_DEBUG, "\trace_active_ = %d\n", this->trace_active_));
  ACE_DEBUG ((LM_DEBUG, "\tracing_enabled_ = %d\n", this->tracing_enabled_));
  ACE_DEBUG ((LM_DEBUG, "\npriority_mask_ = %d\n", this->priority_mask_));
  if (this->thr_state_ != 0)
    ACE_DEBUG ((LM_DEBUG, "\thr_state_ = %d\n", *this->thr_state_));
  ACE_DEBUG ((LM_DEBUG, "\nmsg_off_ = %d\n", this->msg_off_));
  message_queue_.dump ();
#if defined (ACE_MT_SAFE)  
  lock_->dump ();
  // Synchronize output operations.  
#endif /* ACE_MT_SAFE */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void 
ACE_Log_Msg::op_status (int status)
{
  this->status_ = status;
}

int 
ACE_Log_Msg::op_status (void)
{
  return this->status_;
}

void 
ACE_Log_Msg::restart (int r)
{
  this->restart_ = r;
}

int 
ACE_Log_Msg::restart (void)
{
  return this->restart_;
}

int 
ACE_Log_Msg::errnum (void)
{
  return this->errnum_;
}

void
ACE_Log_Msg::errnum (int e)
{
  this->errnum_ = e;
}

int 
ACE_Log_Msg::linenum (void)
{
  return this->linenum_;
}

void
ACE_Log_Msg::linenum (int l)
{
  this->linenum_ = l;
}

int 
ACE_Log_Msg::inc (void) 
{ 
  return this->trace_depth_++; 
}

int 
ACE_Log_Msg::dec (void) 
{ 
  return --this->trace_depth_; 
}

int 
ACE_Log_Msg::trace_active (void) 
{ 
  return this->trace_active_; 
}

void 
ACE_Log_Msg::trace_active (int value) 
{ 
  this->trace_active_ = value; 
}

ACE_Thread_State *
ACE_Log_Msg::thr_state (void) 
{ 
  return this->thr_state_; 
}

void 
ACE_Log_Msg::thr_state (ACE_Thread_State *ts)
{ 
  this->thr_state_ = ts; 
}

ACE_hthread_t *
ACE_Log_Msg::thr_handle (void) 
{ 
  return this->thr_handle_; 
}

void 
ACE_Log_Msg::thr_handle (ACE_hthread_t *th)
{ 
  this->thr_handle_ = th; 
}

// Enable the tracing facility on a per-thread basis.

void 
ACE_Log_Msg::start_tracing (void) 
{ 
  this->tracing_enabled_ = 1;
}

// Disable the tracing facility on a per-thread basis.

void 
ACE_Log_Msg::stop_tracing (void)  
{ 
  this->tracing_enabled_ = 0; 
}

int
ACE_Log_Msg::tracing_enabled (void)
{ 
  return this->tracing_enabled_;
}

const char *
ACE_Log_Msg::file (void)
{
  return this->file_;
}

void
ACE_Log_Msg::file (const char *s)
{
  ACE_OS::strncpy (this->file_, s, 
		   (sizeof this->file_ / sizeof (char)));
}

char *
ACE_Log_Msg::msg (void)
{
  return this->msg_ + ACE_Log_Msg::msg_off_;
}

void
ACE_Log_Msg::msg (char *m)
{
  ACE_OS::strncpy (this->msg_, m, 
		   (sizeof this->msg_ / sizeof (char)));
}

ostream *
ACE_Log_Msg::msg_ostream (void)
{
  return this->ostream_;
}

void
ACE_Log_Msg::msg_ostream (ostream *m)
{
  this->ostream_ = m;
}

void
ACE_Log_Msg::local_host (const char *s)
{
  if (s)
    {
      ACE_OS::free ((void *) ACE_Log_Msg::local_host_);
      ACE_Log_Msg::local_host_ = ACE_OS::strdup (s);
    }
}

const char *
ACE_Log_Msg::local_host (void) const
{
  return ACE_Log_Msg::local_host_;
}

pid_t
ACE_Log_Msg::getpid (void) const
{
  if (ACE_Log_Msg::pid_ == -1)
    ACE_Log_Msg::pid_ = ACE_OS::getpid ();

  return ACE_Log_Msg::pid_;
}

// #if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
// ACE_MT (template class ACE_Guard<ACE_Thread_Mutex>);
// #endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
