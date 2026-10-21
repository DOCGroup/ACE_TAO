#include "orbsvcs/Log/Log_Flush_Handler.h"
#include "orbsvcs/Log/Log_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Log_Flush_Handler::TAO_Log_Flush_Handler (ACE_Reactor * reactor,
                                              TAO_Log_i * log,
                                              ACE_Time_Value const & interval)
  : timer_id_ (-1),
    reactor_ (reactor),
    log_ (log),
    interval_ (interval)
{
}

TAO_Log_Flush_Handler::~TAO_Log_Flush_Handler ()
{
  this->cancel ();
}

void
TAO_Log_Flush_Handler::schedule ()
{
  this->cancel ();
  this->timer_id_ =
    this->reactor_->schedule_timer (this, nullptr, this->interval_, this->interval_);
}

void
TAO_Log_Flush_Handler::cancel ()
{
  if (this->timer_id_ != -1)
    {
      this->reactor_->cancel_timer (timer_id_);
      this->timer_id_ = -1;
    }
}

int
TAO_Log_Flush_Handler::handle_timeout (ACE_Time_Value const &, void const *)
{
  try
    {
      log_->flush ();
    }
  catch (CORBA::Exception const &)
    {
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
