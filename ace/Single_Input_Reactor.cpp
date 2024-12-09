#include "Single_Input_Reactor.h"

#include "Null_Mutex.h"
#include "Lock_Adapter_T.h"

#if defined (ACE_WIN32) || defined (VXWORKS)
#include "OS_NS_stropts.h"
#else
#include "Flag_Manip.h"
#endif

int ACE_Single_Input_Reactor::initialized ()
{
  return 1;
}

size_t ACE_Single_Input_Reactor::size () const
{
  return event_handler_.handler () != nullptr;
}

int ACE_Single_Input_Reactor::register_handler (ACE_Event_Handler *event_handler, ACE_Reactor_Mask)
{
  event_handler_ = event_handler;
  event_handler->add_reference ();
  // disable ACE_NONBLOCK, see ACE_IPC_SAP::disable()
  const auto handle = event_handler->get_handle ();
#if defined ACE_WIN32 || defined VXWORKS
  u_long nonblock = 0;
  ACE_OS::ioctl (handle, FIONBIO, &nonblock);
#else
  ACE::clr_flags (handle, ACE_NONBLOCK);
#endif
  return 0;
}

int ACE_Single_Input_Reactor::handle_events (ACE_Time_Value*)
{
  const auto handler = event_handler_.handler ();
  if (handler) {
    handler->handle_input (handler->get_handle ());
  }
  return 0;
}

ACE_Lock &ACE_Single_Input_Reactor::lock ()
{
  static ACE_Lock_Adapter<ACE_Null_Mutex> lock;
  return lock;
}

int ACE_Single_Input_Reactor::open (size_t, int, ACE_Sig_Handler *, ACE_Timer_Queue *, int, ACE_Reactor_Notify *)
{
  return 0;
}

int ACE_Single_Input_Reactor::current_info (ACE_HANDLE, size_t &)
{
  return 0;
}

int ACE_Single_Input_Reactor::set_sig_handler (ACE_Sig_Handler *)
{
  return 0;
}

int ACE_Single_Input_Reactor::timer_queue (ACE_Timer_Queue *)
{
  return 0;
}

ACE_Timer_Queue *ACE_Single_Input_Reactor::timer_queue () const
{
  return nullptr;
}

int ACE_Single_Input_Reactor::close ()
{
  return 0;
}

int ACE_Single_Input_Reactor::work_pending (const ACE_Time_Value &)
{
  return 0;
}

int ACE_Single_Input_Reactor::alertable_handle_events (ACE_Time_Value *)
{
  return 0;
}

int ACE_Single_Input_Reactor::handle_events (ACE_Time_Value &)
{
  return 0;
}

int ACE_Single_Input_Reactor::alertable_handle_events (ACE_Time_Value &)
{
  return 0;
}

int ACE_Single_Input_Reactor::deactivated ()
{
  return 0;
}

void ACE_Single_Input_Reactor::deactivate (int)
{}

int ACE_Single_Input_Reactor::register_handler (ACE_HANDLE, ACE_Event_Handler *, ACE_Reactor_Mask)
{
  return 0;
}

#ifdef ACE_WIN32
int ACE_Single_Input_Reactor::register_handler (ACE_Event_Handler *, ACE_HANDLE)
{
  return 0;
}
#endif

int ACE_Single_Input_Reactor::register_handler (ACE_HANDLE, ACE_HANDLE, ACE_Event_Handler *, ACE_Reactor_Mask)
{
  return 0;
}

int ACE_Single_Input_Reactor::register_handler (const ACE_Handle_Set &, ACE_Event_Handler *, ACE_Reactor_Mask)
{
  return 0;
}

int ACE_Single_Input_Reactor::register_handler (int, ACE_Event_Handler *, ACE_Sig_Action *, ACE_Event_Handler **, ACE_Sig_Action *)
{
  return 0;
}

int ACE_Single_Input_Reactor::register_handler (const ACE_Sig_Set &, ACE_Event_Handler *, ACE_Sig_Action *)
{
  return 0;
}

int ACE_Single_Input_Reactor::remove_handler (ACE_Event_Handler *, ACE_Reactor_Mask)
{
  return 0;
}

int ACE_Single_Input_Reactor::remove_handler (ACE_HANDLE, ACE_Reactor_Mask)
{
  return 0;
}

int ACE_Single_Input_Reactor::remove_handler (const ACE_Handle_Set &, ACE_Reactor_Mask)
{
  return 0;
}

int ACE_Single_Input_Reactor::remove_handler (int signum, ACE_Sig_Action *, ACE_Sig_Action *, int)
{
  return 0;
}

int ACE_Single_Input_Reactor::remove_handler (const ACE_Sig_Set &)
{
  return 0;
}

int ACE_Single_Input_Reactor::suspend_handler (ACE_Event_Handler *)
{
  return 0;
}

int ACE_Single_Input_Reactor::suspend_handler (ACE_HANDLE)
{
  return 0;
}

int ACE_Single_Input_Reactor::suspend_handler (const ACE_Handle_Set &)
{
  return 0;
}

int ACE_Single_Input_Reactor::suspend_handlers ()
{
  return 0;
}

int ACE_Single_Input_Reactor::resume_handler (ACE_Event_Handler *)
{
  return 0;
}

int ACE_Single_Input_Reactor::resume_handler (ACE_HANDLE)
{
  return 0;
}

int ACE_Single_Input_Reactor::resume_handler (const ACE_Handle_Set &)
{
  return 0;
}

int ACE_Single_Input_Reactor::resume_handlers ()
{
  return 0;
}

int ACE_Single_Input_Reactor::resumable_handler ()
{
  return 0;
}

int ACE_Single_Input_Reactor::uses_event_associations ()
{
  return 0;
}

long ACE_Single_Input_Reactor::schedule_timer (ACE_Event_Handler *, const void *, const ACE_Time_Value &, const ACE_Time_Value &)
{
  return 0;
}

int ACE_Single_Input_Reactor::reset_timer_interval (long, const ACE_Time_Value &)
{
  return 0;
}

int ACE_Single_Input_Reactor::cancel_timer (ACE_Event_Handler *, int)
{
  return 0;
}

int ACE_Single_Input_Reactor::cancel_timer (long, const void **, int)
{
  return 0;
}

int ACE_Single_Input_Reactor::schedule_wakeup (ACE_Event_Handler *, ACE_Reactor_Mask)
{
  return 0;
}

int ACE_Single_Input_Reactor::schedule_wakeup (ACE_HANDLE, ACE_Reactor_Mask)
{
  return 0;
}

int ACE_Single_Input_Reactor::cancel_wakeup (ACE_Event_Handler *, ACE_Reactor_Mask)
{
  return 0;
}

int ACE_Single_Input_Reactor::cancel_wakeup (ACE_HANDLE, ACE_Reactor_Mask)
{
  return 0;
}

int ACE_Single_Input_Reactor::notify (ACE_Event_Handler *, ACE_Reactor_Mask, ACE_Time_Value *)
{
  return 0;
}

void ACE_Single_Input_Reactor::max_notify_iterations (int)
{}

int ACE_Single_Input_Reactor::max_notify_iterations ()
{
  return 0;
}

int ACE_Single_Input_Reactor::purge_pending_notifications (ACE_Event_Handler *, ACE_Reactor_Mask)
{
  return 0;
}

ACE_Event_Handler* ACE_Single_Input_Reactor::find_handler (ACE_HANDLE)
{
  return nullptr;
}

int ACE_Single_Input_Reactor::handler (ACE_HANDLE, ACE_Reactor_Mask, ACE_Event_Handler **)
{
  return 0;
}

int ACE_Single_Input_Reactor::handler (int, ACE_Event_Handler **)
{
  return 0;
}

void ACE_Single_Input_Reactor::wakeup_all_threads ()
{}

int ACE_Single_Input_Reactor::owner (ACE_thread_t, ACE_thread_t *)
{
  return 0;
}

int ACE_Single_Input_Reactor::owner (ACE_thread_t *)
{
  return 0;
}

int ACE_Single_Input_Reactor::restart ()
{
  return 0;
}

int ACE_Single_Input_Reactor::restart (int)
{
  return 0;
}

void ACE_Single_Input_Reactor::requeue_position (int)
{}

int ACE_Single_Input_Reactor::requeue_position ()
{
  return 0;
}

int ACE_Single_Input_Reactor::mask_ops (ACE_Event_Handler *, ACE_Reactor_Mask, int)
{
  return 0;
}

int ACE_Single_Input_Reactor::mask_ops (ACE_HANDLE, ACE_Reactor_Mask, int)
{
  return 0;
}

int ACE_Single_Input_Reactor::ready_ops (ACE_Event_Handler *, ACE_Reactor_Mask, int)
{
  return 0;
}

int ACE_Single_Input_Reactor::ready_ops (ACE_HANDLE, ACE_Reactor_Mask, int)
{
  return 0;
}

void ACE_Single_Input_Reactor::dump () const
{}
