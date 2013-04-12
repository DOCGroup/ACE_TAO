/* -*- C++ -*- $Id$ */

#include "orbsvcs/Log_Macros.h"
#include "Validate_Client_Task.h"
#include "EventChannelFactory.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "ace/Reactor.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_validate_client_Task::
     TAO_Notify_validate_client_Task (const ACE_Time_Value &delay,
                                const ACE_Time_Value &interval,
                                TAO_Notify_EventChannelFactory *ecf)
  : delay_ (delay),
    interval_ (interval),
    ecf_ (ecf),
    shutdown_ (false)
{
  if (this->activate (THR_NEW_LWP | THR_JOINABLE, 1) == -1)
  {
    ORBSVCS_ERROR ((LM_ERROR,
      ACE_TEXT("(%P|%t)TAO_Notify_validate_client_Task: %p\n"),"activate"));
  }
}


TAO_Notify_validate_client_Task::~TAO_Notify_validate_client_Task (void)
{
}


int
TAO_Notify_validate_client_Task::svc (void)
{
  ACE_Time_Value due = ACE_OS::gettimeofday () + this->delay_;
  while (! this->shutdown_)
  {
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, *(this->condition_.mutex()), -1);
      this->condition_.wait (&due);
    }

    if (this->shutdown_)
      break;

    try
    {
      if (TAO_debug_level > 0)
      {
        ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("%P|%t)%T TAO_Notify_validate_client_Task::svc validate start\n")));
      }
      this->ecf_->validate ();
      if (TAO_debug_level > 0)
      {
        ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("%P|%t)%T TAO_Notify_validate_client_Task::svc validate end\n")));
      }
    }
    catch (...)
    {
      // Ignore all exceptions
    }

    if (this->interval_ == ACE_Time_Value::zero)
      break;

    due = ACE_OS::gettimeofday () + this->interval_;
  }

  return 0;
}


void
TAO_Notify_validate_client_Task::shutdown (void)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *(this->condition_.mutex()));
    this->shutdown_ = true;
    this->condition_.signal ();
  }
  this->wait ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
