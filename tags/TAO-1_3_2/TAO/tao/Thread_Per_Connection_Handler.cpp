//$Id$
#include "Thread_Per_Connection_Handler.h"
#include "Connection_Handler.h"
#include "debug.h"
#include "ace/Flag_Manip.h"
#include "Transport.h"

ACE_RCSID (tao,
           Thread_Per_Connection_Handler,
           "$Id$")

TAO_Thread_Per_Connection_Handler::TAO_Thread_Per_Connection_Handler (
    TAO_Connection_Handler *ch)
  : ch_ (ch)
{
  // REFCNT: Matches decr_refcount() in the destructor.
  (void) this->ch_->incr_refcount ();
}

TAO_Thread_Per_Connection_Handler::~TAO_Thread_Per_Connection_Handler (void)
{
  // REFCNT: Matches incr_refcount() in the constructor.
  (void) this->ch_->decr_refcount ();
}

int
TAO_Thread_Per_Connection_Handler::activate (long flags,
                                             int n_threads,
                                             int force_active,
                                             long priority,
                                             int grp_id,
                                             ACE_Task_Base *task,
                                             ACE_hthread_t thread_handles[],
                                             void *stack[],
                                             size_t stack_size[],
                                             ACE_thread_t  thread_names[])
{
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_LIB_TEXT ("TAO (%P|%t) - IIOP_Connection_Handler::")
                 ACE_LIB_TEXT ("activate %d threads, flags = %d\n"),
                 n_threads,
                 flags,
                 THR_BOUND));

  return TAO_TPC_BASE::activate (flags,
                                 n_threads,
                                 force_active,
                                 priority,
                                 grp_id,
                                 task,
                                 thread_handles,
                                 stack,
                                 stack_size,
                                 thread_names);
}

int
TAO_Thread_Per_Connection_Handler::svc (void)
{
  ACE_Flag_Manip::clr_flags (
      this->ch_->transport ()->event_handler_i ()->get_handle (),
      ACE_NONBLOCK);

  // Call the implementation here
  return this->ch_->svc_i ();
}

int
TAO_Thread_Per_Connection_Handler::open (void*v)
{
  return this->ch_->open_handler (v);
}

int
TAO_Thread_Per_Connection_Handler::close (u_long)
{
  delete this;

  return 0;
}
