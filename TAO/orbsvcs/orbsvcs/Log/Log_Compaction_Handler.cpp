// $Id$

#include "orbsvcs/Log/Log_Compaction_Handler.h"
#include "orbsvcs/Log/Log_i.h"
#include "ace/Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Log_Compaction_Handler::TAO_Log_Compaction_Handler (ACE_Reactor* reactor,
                                                        TAO_Log_i* log,
                                                        const ACE_Time_Value& interval)
  : timer_id_(-1),
    reactor_(reactor),
    log_(log),
    interval_(interval)
{
}


TAO_Log_Compaction_Handler::~TAO_Log_Compaction_Handler ()
{
  this->cancel ();
}


void
TAO_Log_Compaction_Handler::schedule ()
{
  this->cancel ();
  this->timer_id_ =
    this->reactor_->schedule_timer (this, 0, this->interval_, this->interval_);
}


void
TAO_Log_Compaction_Handler::cancel ()
{
  if (this->timer_id_ != -1)
    {
      this->reactor_->cancel_timer (timer_id_);
      this->timer_id_ = -1;
    }
}


int
TAO_Log_Compaction_Handler::handle_timeout (const ACE_Time_Value&,
                                            const void *)
{
  try
    {
      this->log_->remove_old_records();
    }
  catch (const CORBA::Exception&)
    {
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
