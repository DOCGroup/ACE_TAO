// $Id$

#include "Buffering_Strategy.h"

ACE_INLINE void
TAO_Notify_ThreadPool_Task::update_qos_properties (const TAO_Notify_QoSProperties& qos_properties)
{
  this->buffering_strategy_->update_qos_properties (qos_properties);
}

ACE_INLINE TAO_Notify_Buffering_Strategy*
TAO_Notify_ThreadPool_Task::buffering_strategy (void)
{
  return this->buffering_strategy_;
}
