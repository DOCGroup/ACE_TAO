/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Proactor.h
//
// = AUTHOR
//    Doug Schmidt (schmidt@cs.wustl.edu),
//    Tim Harrison (harrison@cs.wustl.edu), and
//    Irfan Pyarali (ip1@cs.wustl.edu).
// 
// ============================================================================

#if !defined (ACE_Proactor_H)
#define ACE_Proactor_H

#include "ace/OS.h"
#include "ace/Message_Block.h"
#include "ace/Timer_Queue.h"
#include "ace/Event_Handler.h"

// Forward declaration.
class ACE_Overlapped_File;

// Cheshire cat declaration (meow).
class ACE_Overlapped_IO;

class ACE_Export ACE_Proactor : public ACE_Event_Handler
// = TITLE
//     An abstraction for Proactive I/O.
// 
// = DESCRIPTION
//
// The ACE_Proactor encapsulates Win32 overlapped I/O.  The ACE_Proactor
// is also an ACE_Event_Handler which can be registered with the
// ACE_ReactorEx, as follows:
//
// int
// main ()
// {
//   // ...
//
//   // Register Proactor with ReactorEx.
//   ACE_Service_Config::reactorEx ()->register_handler
// 			      (ACE_Service_Config::proactor ());
//
//   // Demultiplex all ReactorEx and Proactor events from a single 
//   // thread.
//   ACE_Service_Config::run_reactorEx_event_loop ();
//
//   return 42;
// }
//
// This makes it possible to seemlessly integrate the ACE_Proactor (which
// handles only overlapped I/O) with other forms of Win32 HANDLE-based
// synchronization (e.g., Mutexes, Semaphores, Threads, Processes, etc.).
{
public:
  // = Initialization and termination methods.

  ACE_Proactor (size_t number_of_threads = 0);
  // Initialize a proactor and give it the number of threads to allow
  // to run concurrently (note that we don't spawn the threads, the NT
  // kernel does).

  int close (void);
  // Close completion port.

  // = Event demultiplexing hooks inherited from Event_Handler.
  virtual int handle_signal (int, siginfo_t * = 0, ucontext_t * = 0);
  // Called back when used in the context of the ReactorEx.

  virtual ACE_HANDLE get_handle (void) const;
  // Returns the underlying Win32 Event HANDLE that is used to
  // integrate I/O completion ports with the ReactorEx.

  // = Event loop methods.
  virtual int handle_events (ACE_Time_Value *how_long = 0);
  virtual int handle_events (ACE_Time_Value &how_long);
  // Main event loop driver that blocks for -how_long- before
  // returning (will return earlier if I/O or signal events occur).
  // Note that -how_long- can be 0, in which case this method blocks
  // until I/O events or signals occur.  handle_events just blocks
  // on GetQueuedCompletionStatus at completion_port_.  When I/O
  // completions arrive, it calls back the Event_Handler associated
  // with completed I/O operation.  Returns 0 if -how_long- elapses
  // before an event occurs, 1 when if an event occured, and -1 on
  // failure.

  // = Communication method.
  virtual int initiate (ACE_Event_Handler *handler,
			ACE_Reactor_Mask mask = ACE_Event_Handler::WRITE_MASK,
			ACE_Message_Block *msg = 0,
			ACE_Overlapped_File *file = 0);
  // Invoke proactive I/O on <handler>.  If <msg> == 0, the Proactor
  // will call handler::get_message to obtain an ACE_Message_Block
  // to send/recv according to <mask>.  If <mask> ==
  // ACE_Event_Handler::WRITE_MASK, the Proactor calls WriteFile using
  // the <msg> and Event_Handler::get_handle.  Returns 1 if the operation
  // succeeded immediately, 0 if the operation is pending (in which
  // case the <handler> will be called back), and -1 if an error
  // occurred.  <file> represents the offset into the file to initiate
  // the operation with.  When using the proactor for overlapped file
  // I/O, the user is responsible for maintaining the pointer to the
  // file.  If you perform multiple initiates with the same or no
  // File_Pointer value, initiate will fill in the same file data into
  // multiple Message_Blocks.  <file> is ignored for network I/O or if
  // <file> == 0.  If <file> != 0 it is updated (via lseek) respective to
  // the operation initiated.

  // = Timer management. 
  virtual int schedule_timer (ACE_Event_Handler *,
			      const void *arg,
			      const ACE_Time_Value &delta,
			      const ACE_Time_Value &interval = ACE_Time_Value::zero);
  // Schedule an <Event_Handler> that will expire after <delay> amount
  // of time.  If it expires then <arg> is passed in as the value to
  // the <Event_Handler>'s <handle_timeout> callback method.  If
  // <interval> is != to <ACE_Time_Value::zero> then it is used to
  // reschedule the <Event_Handler> automatically.  This method
  // returns a timer handle that uniquely identifies the
  // <Event_Handler> in an internal list.  This timer handle can be
  // used to cancel an <Event_Handler> before it expires.  The
  // cancellation ensures that timer_ids are unique up to values of
  // greater than 2 billion timers.  As long as timers don't stay
  // around longer than this there should be no problems with
  // accidentally deleting the wrong timer.

  virtual int cancel_timer (ACE_Event_Handler *handler);
  // Cancel all <Event_Handlers> that match the address of
  // <Event_Handler>.

  virtual int cancel_timer (int timer_id, const void **arg = 0);
  // Cancel the single <ACE_Event_Handler> that matches the <timer_id>
  // value (which was returned from the <schedule> method).  If arg is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the <Event_Handler> was registered.  This
  // makes it possible to free up the memory and avoid memory leaks.

protected:

  ACE_Timer_Queue timer_queue_;
  // Maintains the list of pending timers.

  ACE_Time_Value timer_skew_;
  // Adjusts for timer skew in various clocks.

  virtual int initiate (ACE_Overlapped_IO *overlapped);
  // Helper to initiate.

  int dispatch (ACE_Overlapped_IO *overlapped, 
		u_long bytes_transfered);
  // Helper function which dispatches results to Event_Handlers.

  ACE_HANDLE completion_port_;
  // The completion_port_ is where <handler> should tell a completed
  // I/O operation to queue up.  All proactive I/O operation
  // completions queue up on this handle.  This handle is set by the
  // <invoke> method.

  size_t number_of_threads_;
  // Max threads that will be allowed to run in a completion port.

  ACE_HANDLE global_handle_;
  // Win32 HANDLE associated with every operation that signals when
  // any operation completes (used to transparently integrate the
  // <ACE_Proactor> with the <ACE_Dispatcher>).
};

class ACE_Export ACE_Overlapped_File
  // = TITLE
  //    A wrapper for overlapped file I/O.
  //    
  // = DESCRIPTION
  //    ACE_Overlapped_File is place-holder for file I/O.  When
  //    performing overlapped I/O in win32, a file pointer is not
  //    managed by the kernel.  Instead, the user is responsible for
  //    maintaining file pointers for all open files.  This wrapper
  //    provides an abstraction for a file pointer.  The Proactor
  //    updates Overlapped_File objects when overlapped I/O operations
  //    complete.  Eventually, this class may be integrated with
  //    ACE_FILE_IO.
{
public:
  // = Initialization and termination methods.
  ACE_Overlapped_File (void);
  // Open must be called.

  ACE_Overlapped_File (const ACE_Overlapped_File &file);
  // Copy <file>.

  ACE_Overlapped_File (const char *file_name, int mode, int perms = 0);
  // Construction of an ACE_Overlapped_File.  Calls open.

  ~ACE_Overlapped_File (void);
  // Destruction.  Calls close.

  int open (const char *file_name,
	    int access = GENERIC_READ,
	    int share = FILE_SHARE_READ,
	    LPSECURITY_ATTRIBUTES security = 0,
	    int creation = OPEN_EXISTING,
	    int flags = FILE_ATTRIBUTE_NORMAL,
	    ACE_HANDLE template_file = ACE_INVALID_HANDLE);
  // Opens <file_name> according to <mode> and <perms>.  This method
  // is equivalent to CreateFile.  Returns 0 on success, -1 on failure
  // with errno == reason.

  int open (ACE_HANDLE handle);
  // Uses the given <handle>.  Returns 0 on success, -1 on failure.
  // This will only return -1 when <handle> == ACE_INVALID_HANDLE.

  void close (void);
  // Closes the file if open.  Can be called explicitly, or implicitly
  // through the destructor.

  off_t offset (void) const;
  // Return the current offset into the file.

  off_t size (void) const;
  // Return the current size of the file.

  off_t lseek (off_t offset, int whence);
  // If <whence> == SEEK_SET, then the file pointer is set to
  // <offset>.  If <whence> == SEEK_CUR, then the file pointer is set
  // to its current location plus <offset>.  If <whence> == SEEK_END,
  // the file pointer is set to the size of the file plus <offset>.

  ACE_HANDLE get_handle (void) const;
  // Get the handle to the file.

protected:
  off_t offset_;
  // Current offset into the file.

  off_t file_size_;
  // Size of the file.

  ACE_HANDLE handle_;
  // Handle to the I/O device.

  int delete_handle_;
  // Keeps track of whether we need to delete the <handle_>.
};

#if defined (__ACE_INLINE__)
#include "ace/Proactor.i"
#endif /* __ACE_INLINE__ */
#endif /* ACE_Proactor_H */

