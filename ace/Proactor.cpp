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

  int dispatch (u_long bytes_transferred);
  // Callback the appropriate handle_* method on handler_.

  int initiate (u_long &bytes_transferred);
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
ACE_Overlapped_IO::dispatch (u_long bytes_transferred)
{
  if (this->file_ != 0)
    // Move the file pointer forward.
    file_->lseek (bytes_transferred, SEEK_CUR);

  if (this->mask_ == ACE_Event_Handler::WRITE_MASK)
    {
      // Update the message length to reflect what was sent.
      this->message_->rd_ptr (bytes_transferred);
      return handler_->handle_output_complete (this->message_,
					       bytes_transferred);
    }
  else // this->mask_ == ACE_Event_Handler::READ_MASK
    {
      // Update the message length to reflect what was received.
      this->message_->wr_ptr (bytes_transferred);
      return this->handler_->handle_input_complete (this->message_, 
						    bytes_transferred);
    }
}

// When we port this to use Posix async I/O, these calls will be
// replace will generic ACE_OS calls.

int
ACE_Overlapped_IO::initiate (u_long &bytes_transferred)
{
#if defined (ACE_WIN32)
  if (this->mask_ == ACE_Event_Handler::WRITE_MASK)
    {
      // Try to write.
      return ::WriteFile (this->handler_->get_handle (),
			  this->message_->rd_ptr (),
			  this->message_->length (),
			  &bytes_transferred,
			  this);
    }
  else
    {
      // READ_MASK is set, so try to read.
      return ::ReadFile (this->handler_->get_handle (),
			 this->message_->wr_ptr (),
			 this->message_->size (),
			 &bytes_transferred,
			 this);
    }
#else
  bytes_transferred = bytes_transferred;
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */
}

ACE_Overlapped_IO::operator ACE_OVERLAPPED * (void)
{
  return (ACE_OVERLAPPED *) this;
}

ACE_Proactor::ACE_Proactor (size_t number_of_threads, ACE_Timer_Queue *tq)
  : timer_queue_ (tq),
    completion_port_ (0), // This *MUST* be 0, *NOT* ACE_INVALID_HANDLE!!!!
    number_of_threads_ (number_of_threads)
{
  if (this->timer_queue_ == 0)
    {
      ACE_NEW (this->timer_queue_, ACE_Timer_Queue);
      this->delete_timer_queue_ = 1;
    }

#if defined (ACE_WIN32)
  // Create an IO completion port that is not associated with a file
  // handle.  This will allow us to use GetQueuedCompletionStatus as a
  // timer mechanism only.
  ACE_HANDLE cp;

  cp = ::CreateIoCompletionPort (INVALID_HANDLE_VALUE,
				 this->completion_port_,
				 (u_long) 0, // 0 completion key
				 this->number_of_threads_);

  if (cp != 0)
    // Success.
    this->completion_port_ = cp;
  else // Failure.
    {
      int error = ACE_OS::last_error ();
      // If errno == ERROR_INVALID_PARAMETER, then this handle was
      // already registered.
      if (error != ERROR_INVALID_PARAMETER)
	ACE_ERROR ((LM_ERROR, 
		    "%p CreateIoCompletionPort failed errno = %d.\n",
		    "ACE_Proactor::initiate", error));
    }
#endif /* ACE_WIN32 */
}

ACE_Proactor::~ACE_Proactor (void)
{
  if (this->delete_timer_queue_)
    delete this->timer_queue_;
}

int
ACE_Proactor::close (void)
{
  if (this->completion_port_ != 0)
    ACE_OS::close (this->completion_port_);

  // @@ Should we call shared_event_.remove ()?
  return 0;
}

int
ACE_Proactor::handle_signal (int, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_Proactor::handle_signal");

  ACE_Time_Value timeout (0, 0);

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

  return this->timer_queue_->schedule 
    (handler, arg, ACE_OS::gettimeofday () + delta_time, interval);
}

#define ACE_TIMEOUT_OCCURRED 258

int
ACE_Proactor::handle_events (ACE_Time_Value *max_wait_time)
{
  // Stash the current time -- the destructor of this object will
  // automatically compute how much time elapsed since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);

  max_wait_time = timer_queue_->calculate_timeout (max_wait_time);

  ACE_Overlapped_IO *overlapped = 0;
  u_long bytes_transferred = 0;

  int error = 0;
#if defined (ACE_WIN32)
  ACE_HANDLE io_handle = ACE_INVALID_HANDLE;
  int timeout = max_wait_time == 0 ? INFINITE : max_wait_time->msec ();

  BOOL result = 0;
 
  // When we port this to use Posix async I/O, this call will be
  // replace will a generic ACE_OS call.
  result = ::GetQueuedCompletionStatus (completion_port_,
					&bytes_transferred,
					(u_long *) &io_handle,
					(ACE_OVERLAPPED **) &overlapped,
					timeout);

  // Check for a failed dequeue.  This can happen either because
  // of problems with the IO completion port (in which case
  // overlapped == 0) or due to problems with the completion
  // operation (in which case overlapped != 0).  In either case,
  // we'll stash the error value so that we can update errno
  // appropriate later on.
  if (result == FALSE)
    error = ACE_OS::last_error ();
#endif /* ACE_WIN32 */

  // Check for any timers that can be handled before we dispatch the
  // dequeued event.  Note that this is done irrespective of whether
  // an error occurred.
  this->timer_queue_->expire ();

  // @@ Need to make sure that if GetQueuedCompletionStatus fails due
  // to a time out that this information is propagated correctly to
  // the caller!

  // GetQueued returned because of a error or timer.
  if (error != 0 && overlapped == 0)
    { 
      // @@  What's the WIN32 constant for 258?!?!?!
      if (error == ACE_TIMEOUT_OCCURRED)
	// Returning because of timeout.
	return 0;
      // Returning because of error.
      ACE_ERROR_RETURN ((LM_ERROR, 
			 "%p GetQueuedCompletionStatus failed errno = %d.\n",
			 "ACE_Proactor::handle_events", error), -1);
    }

  // Dequeued a failed or successful operation.  Dispatch the
  // Event_Handler.  Note that GetQueuedCompletionStatus returns false
  // when operations fail, but they still need to be dispatched.
  int dispatch_result = this->dispatch (overlapped, bytes_transferred, error);

  // Return -1 (failure), or return 1.  Remember that 0 is reserved
  // for timeouts only, so we have to turn dispatch_results to 1.  So,
  // if this->dispatch() returns a 1 or 0, we return 1.  Otherwise,
  // we return -1.
  return dispatch_result == -1 ? -1 : 1;
}

// Returns 0 or 1 on success, -1 on failure.
int
ACE_Proactor::dispatch (ACE_Overlapped_IO *overlapped,
			u_long bytes_transferred,
			int error)
{
  // We propagate the error status to the callee by setting errno  =
  // error (which is the value returned by ::GetLastError().
  errno = error;

  // Call back the Event_Handler and do what it wants based on the
  // return value.
  int dispatch_result = overlapped->dispatch (bytes_transferred);

  switch (dispatch_result)
    {
    case 1: // Start another operation.
      // Reinitialize by getting a new message and resetting the
      // overlapped offset.
      overlapped->re_init ();
      return this->initiate (overlapped);
    case -1: // Handler is closing.
      overlapped->handler_->handle_close 
	(overlapped->handler_->get_handle (), overlapped->mask_);
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
  ACE_Overlapped_IO *overlapped = 0;
  
  ACE_NEW_RETURN (overlapped,
		  ACE_Overlapped_IO (mask, handler, msg,
				     file, this->get_handle ()),
		  -1);

  // Tell the handler that *this* <Proactor> is dispatching it.
  handler->proactor (this);
  return this->initiate (overlapped);
}

// Returns 0 or 1 on success, -1 on failure.
// Returns 1 when initiate succeeded immediately.
int
ACE_Proactor::initiate (ACE_Overlapped_IO *overlapped)
{
#if defined (ACE_WIN32)
  u_long bytes_transferred = 0;
  ACE_HANDLE io_handle = overlapped->handler_->get_handle ();
  ACE_HANDLE cp = 0;
  cp = ::CreateIoCompletionPort (io_handle,
				 this->completion_port_,
				 (u_long) io_handle,
				 this->number_of_threads_);

  if (cp != 0)
    // Success.
    this->completion_port_ = cp;
  else // Failure.
    {
      int error = ACE_OS::last_error ();
      // If errno == ERROR_INVALID_PARAMETER, then this handle was
      // already registered.
      if (error != ERROR_INVALID_PARAMETER)
	ACE_ERROR_RETURN ((LM_ERROR, 
			   "%p CreateIoCompletionPort failed errno = %d.\n",
			   "ACE_Proactor::initiate", error), -1);
    }

  // Initiate a WriteFile/ReadFile.  If it succeeds, dispatch the
  // handler.
  int initiate_result = overlapped->initiate (bytes_transferred);

  if (initiate_result)
    // Return 1; the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  int err = ACE_OS::last_error ();
  switch (err)
    {
    case ERROR_HANDLE_EOF:
    case ERROR_NETNAME_DELETED:
      // The OVERLAPPED will *not* get queued for this case.  Thus, we
      // have to dispatch immediately. 
      return this->dispatch (overlapped, bytes_transferred, err);
 
    case ERROR_IO_PENDING:
      // The IO will complete proactively.
      return 0;
    default:
      // This is a *bad* error.
      ACE_ERROR ((LM_ERROR, "I/O error %d\n", err));
      return -1;
    }
#else
  overlapped = overlapped;
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */
}     

ACE_Overlapped_File::ACE_Overlapped_File (const ACE_Overlapped_File &file)
  : offset_ (file.offset ()),
    file_size_ (0),
    handle_ (file.get_handle ()),
    delete_handle_ (0)
{
}

ACE_Overlapped_File::ACE_Overlapped_File (void)
  : offset_ (0),
    file_size_ (0),
    handle_ (ACE_INVALID_HANDLE),
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
  file_name = file_name;
  access = access;
  share = share;
  security = security;
  creation = creation;
  flags = flags;
  template_file = template_file;
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
