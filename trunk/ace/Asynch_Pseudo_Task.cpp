// $Id$

#include "ace/Asynch_Pseudo_Task.h"

#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_signal.h"

ACE_RCSID(ace, Asynch_Pseudo_Task, "$Id$")

ACE_Asynch_Pseudo_Task::ACE_Asynch_Pseudo_Task()
  : flg_active_ (0),
    select_reactor_ (),               // should be initialized before reactor_
    reactor_ (&select_reactor_, 0),   // don't delete implementation
    token_ (select_reactor_.lock ()), // we can use reactor token	
    finish_count_ (0)
{
}

ACE_Asynch_Pseudo_Task::~ACE_Asynch_Pseudo_Task()
{
  stop();
}

int 
ACE_Asynch_Pseudo_Task::is_active (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));
  return flg_active_;
}

int 
ACE_Asynch_Pseudo_Task::start (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));

  if (this->flg_active_)
    return 0;

  if (this->reactor_.initialized () == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%N:%l:%p\n"),
                       ACE_LIB_TEXT ("ACE_Asynch_Pseudo_Task::start reactor is not initialized")),
                       -1);


  if (this->activate () != 0)
    return -1;

  this->flg_active_ = 1;
  return 0;
}

int 
ACE_Asynch_Pseudo_Task::stop (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));

    if (this->flg_active_ == 0)  // already stopped 
      return 0;

    reactor_.end_reactor_event_loop ();
  }

  if (-1 == this->wait ())
    return -1;

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));
    this->flg_active_ = 0;

    this->reactor_.close ();

    while (this->finish_count_ > 0)
      {
        ACE_MT (ace_mon.release ());
        finish_event_.wait ();

        ACE_MT (ace_mon.acquire ());
        finish_event_.reset ();
      }
  }

  return 0;
}

int
ACE_Asynch_Pseudo_Task::lock_finish (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));
  finish_count_ ++;
  return 0;
}

int
ACE_Asynch_Pseudo_Task::unlock_finish (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));

  --finish_count_;
  finish_event_.signal ();

  return 0;
}

int
ACE_Asynch_Pseudo_Task::svc (void)
{
#if !defined (ACE_WIN32) 

  sigset_t RT_signals;

  if (sigemptyset (&RT_signals) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("Error:(%P | %t):%p\n"),
                ACE_LIB_TEXT ("sigemptyset failed")));

  int member = 0;

  for (int si = ACE_SIGRTMIN; si <= ACE_SIGRTMAX; si++)
    {
      member = sigismember (& RT_signals , si);
      if (member == 1)
        {
          sigaddset (&RT_signals, si);
        }
    }

  if (ACE_OS::pthread_sigmask (SIG_BLOCK, &RT_signals, 0) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("Error:(%P | %t):%p\n"),
                ACE_LIB_TEXT ("pthread_sigmask failed")));
#endif

  reactor_.owner (ACE_Thread::self());

  reactor_.run_reactor_event_loop ();

  return 0;
}



int
ACE_Asynch_Pseudo_Task::register_io_handler (ACE_HANDLE handle,
                                             ACE_Event_Handler *handler,
                                             ACE_Reactor_Mask mask,
                                             int flg_suspend)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));

  if (this->flg_active_ == 0)
    {
      ACE_OS::last_error (ESHUTDOWN);
      return -1;
    }

  // Register the handler with the reactor.
  int retval = this->reactor_.register_handler (handle, handler, mask);
                                                    
  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%N:%l:%p\n"),
                       ACE_LIB_TEXT ("ACE_Asynch_Pseudo_Task::register_io_handler")),
                      -1);

  if (flg_suspend == 0 )
    return 0;

  // Suspend the <handle> now. Enable only when the <accept> is issued
  // by the application.
  retval = this->reactor_.suspend_handler (handle);

  if (retval == -1)
    {
      ACE_ERROR
        ((LM_ERROR,
          ACE_LIB_TEXT ("%N:%l:%p\n"),
          ACE_LIB_TEXT ("ACE_Asynch_Pseudo_Task::register_io_handler (suspended)")));
      this->reactor_.remove_handler (handle,
                                     ACE_Event_Handler::ALL_EVENTS_MASK
                                     | ACE_Event_Handler::DONT_CALL); 
      return -1;
    }

  return 0;
}

int
ACE_Asynch_Pseudo_Task::remove_io_handler (ACE_HANDLE handle)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));

  if (this->flg_active_ == 0)
    {
      ACE_OS::last_error (ESHUTDOWN);
      return -1;
    }
    
  int retval =
    this->reactor_.remove_handler (handle ,
                                   ACE_Event_Handler::ALL_EVENTS_MASK
                                   | ACE_Event_Handler::DONT_CALL); 
  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%N:%l:%p\n")
                       ACE_TEXT ("ACE_Asynch_Pseudo_Task::remove_io_handler")),
                      -1);

  return 0;
}

int
ACE_Asynch_Pseudo_Task::remove_io_handler (ACE_Handle_Set &set)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));

  if (this->flg_active_ == 0)
    {
      ACE_OS::last_error (ESHUTDOWN);
      return -1;
    }
    
  int retval =
    this->reactor_.remove_handler (set,
                                   ACE_Event_Handler::ALL_EVENTS_MASK
                                   | ACE_Event_Handler::DONT_CALL); 
  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%N:%l:%p\n")
                       ACE_TEXT ("ACE_Asynch_Pseudo_Task::remove_io_handler")),
                      -1);

  return 0;
}

int
ACE_Asynch_Pseudo_Task::suspend_io_handler (ACE_HANDLE handle)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));

  if (this->flg_active_ == 0)
    {
      ACE_OS::last_error (ESHUTDOWN);
      return -1;
    }
    
  int retval = this->reactor_.suspend_handler (handle);

  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%N:%l:%p\n")
                       ACE_TEXT ("ACE_Asynch_Pseudo_Task::suspend_io_handler")),
                      -1);

  return 0;
}

int
ACE_Asynch_Pseudo_Task::resume_io_handler (ACE_HANDLE handle)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, this->token_, -1));

  if (this->flg_active_ == 0)
    {
      ACE_OS::last_error (ESHUTDOWN);
      return -1;
    }
    
  return this->reactor_.resume_handler (handle);
}
