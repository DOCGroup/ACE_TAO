/* -*- C++ -*- */
// $Id: Proactor.h,v

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Proactor.h
//
// = AUTHOR
//    Irfan Pyarali (irfan@cs.wustl.edu)
//    Tim Harrison (harrison@cs.wustl.edu)
// 
// ============================================================================

#if !defined (ACE_PROACTOR_H)
#define ACE_PROACTOR_H

#include "ace/OS.h"

#if defined (ACE_WIN32)
// This only works on Win32 platforms

class ACE_Timer_Queue;
class ACE_Asynch_Result;

class ACE_Export ACE_Proactor
  //     
  // = TITLE
  //
  //     A Proactor for asynchronous I/O events.
  // 
  // = DESCRIPTION
  //     
  //     A manager for the I/O completion port.
{
public:
  ACE_Proactor (size_t number_of_threads = 0, 
		ACE_Timer_Queue *tq = 0);
  // A do nothing constructor.

  virtual ~ACE_Proactor (void);
  // Virtual destruction.

  virtual int close (void);
  // Close the IO completion port

  virtual int register_handle (ACE_HANDLE handle, 
			       const void *completion_key);
  // This method adds the <handle> to the I/O completion port

  virtual int handle_events (ACE_Time_Value &wait_time);
  // Dispatch a single set of events.  If <wait_time> elapses before
  // any events occur, return.

  virtual int handle_events (void);
  // Block indefinitely until at least one event is dispatched.

  // = Event loop management methods.
  int run_proactor_event_loop (void);
  // Run the event loop until the this->handle_events returns -1 or
  // the this->end_event_loop is invoked.

  int run_event_loop (ACE_Time_Value &tv);
  // Run the event loop until the this->handle_events returns -1, the
  // this->end_event_loop is invoked, or <tv> expires.

  int end_event_loop (void);
  // Terminates a this->run_event_loop call.

  sig_atomic_t event_loop_done (void);
  // Report if the Proactor's event loop is finished.

  int wake_up_dispatch_threads (void);
  // Add wakeup dispatch threads (reinit).

  int close_dispatch_threads (int wait);
  // Close all dispatch threads.

  size_t number_of_threads (void) const;
  void number_of_threads (size_t threads);
  // Number of thread used as a parameter to CreatIoCompletionPort

protected:

  void application_specific_code (ACE_Asynch_Result *asynch_result,
				  u_long bytes_transferred,
				  int success,
				  const void *completion_key,
				  u_long error);
  // Protect against structured exceptions caused by user code when
  // dispatching handles

  ACE_HANDLE completion_port_;
  size_t number_of_threads_;
};

#if defined (__ACE_INLINE__)
#include "ace/Proactor.i"
#endif /* __ACE_INLINE__ */

#else /* NOT WIN32 */

#include "ace/Timer_Queue.h"

class ACE_Export ACE_Proactor
{
public:
  ACE_Proactor (size_t number_of_threads = 0, 
		ACE_Timer_Queue *tq = 0) {}
  virtual int handle_events (void) { return -1; }
  virtual int handle_events (ACE_Time_Value &) { return -1; }
};

#endif /* ACE_WIN32 */
#endif /* ACE_PROACTOR_H */

