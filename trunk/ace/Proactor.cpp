// Proactor.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Proactor.h"

#if !defined (__ACE_INLINE__)
#include "ace/Proactor.i"
#endif /* __ACE_INLINE__ */

class ACE_Overlapped_IO : public ACE_OVERLAPPED
  // = TITLE
  //     A wrapper for Win32 OVERLAPPED.
  // 
  // = DESCRIPTION
  //     Acts as a magic cookie storing additional state associated
  //     with overlapped I/O operations.  ReadFile and WriteFile take
  //     OVERLAPPED, so we pass in Overlapped_IO.  OVERLAPPEDs are
  //     returned through GetQueuedCompletionStatus.  They are cast
  //     back into Overlapped_IOs to get the handler_ etc.
{
public:
  // = Initialization and termination methods.
  ACE_Overlapped_IO (ACE_Reactor_Mask mask,
		     ACE_Event_Handler *handler,
		     ACE_Message_Block *message,
		     ACE_Overlapped_File *file,
		     ACE_HANDLE event_handle);

  ~ACE_Overlapped_IO (void);
  // Death.

  int dispatch (u_long bytes_transfered);
  // Callback the appropriate handle_* method on handler_.

  int initiate (u_long &bytes_transfered);
  // Call ReadFile or Writefile.

  operator ACE_OVERLAPPED * (void);
  // Return this.

  void re_init (void);
  // Reset the object to be reused.  Calls get_message on the handler_
  // for a new message.

  ACE_Reactor_Mask mask_;
  // Reading or writing.
  ACE_Event_Handler *handler_;
  // The IO handler.
  ACE_Message_Block *message_;
  // The current message to send/recv.
  ACE_Overlapped_File *file_;
  // The optional file pointer to update.

private:
  void init (void);
  // Reset everything. 
};

ACE_Overlapped_IO::ACE_Overlapped_IO (ACE_Reactor_Mask mask,
				      ACE_Event_Handler *handler,
				      ACE_Message_Block *message,
				      ACE_Overlapped_File *file,
				      ACE_HANDLE event_handle)
  : mask_ (mask),
    handler_ (handler),
    message_ (message),
    file_ (file)
{
  this->hEvent = event_handle;
  this->init ();
}

void 
ACE_Overlapped_IO::init (void)
{
  if (file_ == 0)
    this->Offset = 0;
  else
    this->Offset = file_->offset ();

  this->Internal = 0;
  this->InternalHigh = 0;
  this->OffsetHigh = 0;
}

void 
ACE_Overlapped_IO::re_init (void)
{
  this->message_ = this->handler_->get_message ();

  this->init ();
}

ACE_Overlapped_IO::~ACE_Overlapped_IO (void)
{
}

int
ACE_Overlapped_IO::dispatch (u_long bytes_transfered)
{
  if (this->file_ != 0)
    // Move the file pointer forward.
    file_->lseek (bytes_transfered, SEEK_CUR);

  if (this->mask_ == ACE_Event_Handler::WRITE_MASK)
    {
      // Update the message length to reflect what was sent.
      this->message_->rd_ptr (bytes_transfered);
      return handler_->handle_output_complete (this->message_,
					       bytes_transfered);
    }
  else // this->mask_ == ACE_Event_Handler::READ_MASK
    {
      // Update the message length to reflect what was received.
      this->message_->wr_ptr (bytes_transfered);
      return this->handler_->handle_input_complete (this->message_, 
						    bytes_transfered);
    }
}

// When we port this to use Posix async I/O, these calls will be
// replace will generic ACE_OS calls.

int
ACE_Overlapped_IO::initiate (u_long &bytes_transfered)
{
#if defined (ACE_WIN32)
  if (this->mask_ == ACE_Event_Handler::WRITE_MASK)
    {
      // Try to write.
      return ::WriteFile (this->handler_->get_handle (),
			  this->message_->rd_ptr (),
			  this->message_->length (),
			  &bytes_transfered,
			  this);
    }
  else
    {
      // READ_MASK is set, so try to read.
      return ::ReadFile (this->handler_->get_handle (),
			 this->message_->wr_ptr (),
			 this->message_->size (),
			 &bytes_transfered,
			 this);
    }
#else
  ACE_NOTSUP_RETURN (-1);
#endif
}

ACE_Overlapped_IO::operator ACE_OVERLAPPED * (void)
{
  return (ACE_OVERLAPPED *) this;
}

ACE_Proactor::ACE_Proactor (size_t number_of_threads)
  : completion_port_ (0),
    number_of_threads_ (number_of_threads),
    timer_skew_ (0, ACE_TIMER_SKEW)
{
#if defined (ACE_WIN32)
  // Create an "auto-reset" event to indicate that one or more I/O
  // overlapped events have completed.
  this->global_handle_ = ::CreateEvent (NULL, TRUE, FALSE, NULL);
#endif /* ACE_HAS_WIN32 */
}

int
ACE_Proactor::close (void)
{
  if (this->completion_port_ != 0)
    ACE_OS::close (this->completion_port_);

  ACE_OS::close (this->global_handle_);
  return 0;
}

int
ACE_Proactor::handle_signal (int index, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_Proactor::handle_signal");

  ACE_Time_Value timeout (0, 0);

#if defined (ACE_WIN32)
  // Reset the handle to a non-signaled state.
  if (::ResetEvent (global_handle_) == FALSE)
    ACE_ERROR_RETURN ((LM_ERROR, "ResetEvent failed.\n"), -1);
#endif /* ACE_HAS_WIN32 */

  // Perform a non-blocking "poll" for all the I/O events that have
  // completed in the I/O completion queue.

  int result;

  while ((result = this->handle_events (&timeout)) == 1)
    continue;

  // If our handle_events failed, we'll report a failure to the
  // ReactorEx.
  return result == -1 ? -1 : 0;
}


int
ACE_Proactor::schedule_timer (ACE_Event_Handler *handler,
			      const void *arg,
			      const ACE_Time_Value &delta_time, 
			      const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Proactor::schedule_timer");

  return this->timer_queue_.schedule 
    (handler, arg, ACE_OS::gettimeofday () + delta_time, interval);
}

int
ACE_Proactor::handle_events (ACE_Time_Value *how_long)
{
  // Stash the current time.
  ACE_Time_Value prev_time = ACE_OS::gettimeofday ();

  how_long = timer_queue_.calculate_timeout (how_long);

  ACE_Overlapped_IO *overlapped = 0;
  u_long bytes_transfered = 0;

#if defined (ACE_WIN32)
  int error = 0;
  ACE_HANDLE io_handle = ACE_INVALID_HANDLE;

  // When we port this to use Posix async I/O, this call will be
  // replace will a generic ACE_OS call.
  BOOL result;

  result = ::GetQueuedCompletionStatus (this->completion_port_,
					&bytes_transfered,
					(u_long *) &io_handle,
					(ACE_OVERLAPPED **) &overlapped,
  					how_long == 0 ? INFINITE : how_long->msec ());

  // Check for a failed dequeue.  Stash the error value.
  if (result == FALSE && overlapped == 0)
    error = ::GetLastError ();

  // Check for any timers that can be handled before we dispatch the
  // dequeued event.  Note that this is done irrespective of whether
  // an error occurred.
  if (!this->timer_queue_.is_empty ())
    // Fudge factor accounts for problems with Solaris timers...
    this->timer_queue_.expire (ACE_OS::gettimeofday () + this->timer_skew_);

  // @@ Need to make sure that if GetQueuedCompletionStatus fails due
  // to a time out that this information is propagated correctly to
  // the caller!

  // GetQueued returned because of a error or timer.
  if (error != 0)
    { 
      // Compute the time while the Proactor is processing.
      ACE_Time_Value elapsed_time = ACE_OS::gettimeofday () - prev_time;

      // Update -how_long- to reflect the amount of time since
      // handle_events was called.
      if (how_long != 0)
	{
	  if (*how_long > elapsed_time)
	    *how_long = *how_long - elapsed_time;
	  else
	    *how_long = ACE_Time_Value::zero; // Used all of timeout.
	}

      // @@  What's the WIN32 constant for 258?!?!?!
      if (error == 258)
	// Returning because of timeout.
	return 0;
      // Returning because of error.
      ACE_ERROR_RETURN ((LM_ERROR, 
			 "%p GetQueuedCompletionStatus failed errno = %d.\n"
			 "ACE_Proactor::handle_events", error), -1);
    }

#endif /* ACE_WIN32 */

  // Dequeued a failed or successful operation.  Dispatch the
  // Event_Handler.  Note that GetQueuedCompletionStatus returns false
  // when operations fail, but they still need to be dispatched.
  // Should we propogate this to the handler somehow?  Maybe an extra
  // failed/succeeded flag in the dispatch call?
  int dispatch_result = this->dispatch (overlapped, bytes_transfered);

  // Compute the time while the Proactor is processing.
  ACE_Time_Value elapsed_time = ACE_OS::gettimeofday () - prev_time;

  // Update <how_long> to reflect the amount of time since
  // <handle_events> was called.
  if (how_long != 0)
    {
      if (*how_long > elapsed_time)
	*how_long = *how_long - elapsed_time;
      else
	*how_long = ACE_Time_Value::zero; // Used all of timeout.
    }

  // Return -1 (failure), or return 1.  Remember that 0 is reserved
  // for timeouts only, so we have to turn dispatch_results to 1.  So,
  // if this->dispatch() returns a 1 or 0, we return 1.  Otherwise,
  // we return -1.
  return dispatch_result == -1 ? -1 : 1;
}

// Returns 0 or 1 on success, -1 on failure.
int
ACE_Proactor::dispatch (ACE_Overlapped_IO *overlapped, 
			u_long bytes_transfered)
{
  // Call back the Event_Handler and do what it wants based on the
  // return value.
  int dispatch_result = overlapped->dispatch (bytes_transfered);

  switch (dispatch_result)
    {
    case 1: // Start another operation.
      // Reinitialize by getting a new message and resetting the
      // overlapped offset.
      overlapped->re_init ();
      return this->initiate (overlapped);
    case -1: // Handler is closing.
      overlapped->handler_->
	handle_close (overlapped->handler_->get_handle (),
		      overlapped->mask_);
      // Fallthrough.
    default: 
      // Handler succeeded, but does not want another operation
      // started.
      delete overlapped;
      return 0;
    }
}

// Returns 0 or 1 on success, -1 on failure.
int
ACE_Proactor::initiate (ACE_Event_Handler *handler,
			ACE_Reactor_Mask mask,
			ACE_Message_Block *msg,
			ACE_Overlapped_File *file)
{
  if (msg == 0)
    msg = handler->get_message ();

  // Create the state for this operation.  This object "is-a"
  // OVERLAPPED structure, and holds other data and methods for this
  // operation.
  ACE_Overlapped_IO *overlapped;
  
  ACE_NEW_RETURN (overlapped,
		  ACE_Overlapped_IO (mask, handler, msg, 
				     file, this->global_handle_),
		  -1);

  return this->initiate (overlapped);
}

// Returns 0 or 1 on success, -1 on failure.
// Returns 1 when initiate succeeded immediately.
int
ACE_Proactor::initiate (ACE_Overlapped_IO *overlapped)
{
#if defined (ACE_WIN32)
  u_long bytes_transfered = 0;

  ACE_HANDLE io_handle = overlapped->handler_->get_handle ();
  ACE_HANDLE cp;

  cp = ::CreateIoCompletionPort (io_handle,
				 this->completion_port_,
				 (u_long) io_handle,
				 this->number_of_threads_);

  if (cp != 0)
    // Success.
    this->completion_port_ = cp;
  else // Failure.
    {
      int error = ::GetLastError ();
      // If errno == ERROR_INVALID_PARAMETER, then this handle was
      // already registered.
      if (error != ERROR_INVALID_PARAMETER)
	ACE_ERROR_RETURN ((LM_ERROR, 
			   "%p CreateIoCompletionPort failed errno = %d.\n",
			   "ACE_Proactor::initiate", error), -1);
    }

  // Initiate a WriteFile/ReadFile.  If it succeeds, dispatch the
  // handler.
  int initiate_result = overlapped->initiate (bytes_transfered);

  if (initiate_result)
    // Return 1; the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  int err = ::GetLastError ();
  switch (err)
    {
    case ERROR_HANDLE_EOF:
    case ERROR_NETNAME_DELETED:
      // The OVERLAPPED will *not* get queued for this case.  Thus, we
      // have to dispatch immediately. 
      return this->dispatch (overlapped, bytes_transfered);
 
    case ERROR_IO_PENDING:
      // The IO will complete proactively.
      return 0;
    default:
      // This is a *bad* error.
      ACE_ERROR ((LM_ERROR, "I/O error %d\n", err));
      return -1;
    }
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */
}     

ACE_Overlapped_File::ACE_Overlapped_File (const ACE_Overlapped_File &file)
  : offset_ (file.offset ()),
    handle_ (file.get_handle ()),
    file_size_ (0),
    delete_handle_ (0)
{
}

ACE_Overlapped_File::ACE_Overlapped_File (void)
  : offset_ (0),
    handle_ (ACE_INVALID_HANDLE),
    file_size_ (0),
    delete_handle_ (0)
{
}

ACE_Overlapped_File::ACE_Overlapped_File (const char *file_name, 
					  int mode, 
					  int perms)
  : delete_handle_ (1)
{
  this->open (file_name, mode, perms);
}

ACE_Overlapped_File::~ACE_Overlapped_File (void)
{
  this->close ();
}

void
ACE_Overlapped_File::close (void)
{
  if (this->handle_ != ACE_INVALID_HANDLE
      && this->delete_handle_ != 0)
    {
      ACE_OS::close (this->handle_);
      this->handle_ = ACE_INVALID_HANDLE;
    }
}

int
ACE_Overlapped_File::open (ACE_HANDLE handle)
{
  this->handle_ = handle;
  this->delete_handle_ = 0;

  if (this->handle_ == ACE_INVALID_HANDLE)
    return -1;
  else
    return 0;
}

int
ACE_Overlapped_File::open (const char *file_name,
			   int access,
			   int share,
			   LPSECURITY_ATTRIBUTES security,
			   int creation,
			   int flags,
			   ACE_HANDLE template_file)
{
#if defined (ACE_WIN32)
  if (file_name != 0)
    this->handle_ = ::CreateFile (file_name, access, share, 
				  security, creation, flags, 
				  template_file);

  if (this->handle_ == ACE_INVALID_HANDLE)
    {
      errno = ENOENT;
      return -1;
    }
  else
    {
      this->delete_handle_ = 1;
      return 0;
    }
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */
}

off_t
ACE_Overlapped_File::lseek (off_t offset, 
			    int whence)
{
  switch (whence)
    {
    case SEEK_SET:
      this->offset_ = offset;
      break;
    case SEEK_CUR:
      this->offset_ += offset;
      break;
    case SEEK_END:
      if (handle_ == ACE_INVALID_HANDLE)
	{
	  errno = ENFILE;
	  return -1;
	}
      else
	this->offset_ = ACE_OS::filesize (handle_) + offset;
      break;
    default :
      errno = EINVAL;
      ACE_ERROR_RETURN ((LM_ERROR, "ACE_Overlapped_File::lseek"
			 "Invalid whence = %d.\n"), -1);
    }

  return this->offset_;
}
