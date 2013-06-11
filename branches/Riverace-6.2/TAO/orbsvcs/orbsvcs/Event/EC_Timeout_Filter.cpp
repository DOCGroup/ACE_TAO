// $Id$

#include "orbsvcs/Event/EC_Timeout_Filter.h"
#include "orbsvcs/Event/EC_Timeout_Generator.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Timeout_Filter.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Timeout_Filter::TAO_EC_Timeout_Filter (
      TAO_EC_Event_Channel_Base *event_channel,
      TAO_EC_ProxyPushSupplier *supplier,
      const TAO_EC_QOS_Info& qos_info,
      RtecEventComm::EventType type,
      RtecEventComm::Time period)
  : event_channel_ (event_channel),
    supplier_ (supplier),
    qos_info_ (qos_info),
    type_ (type),
    period_ (period),
    id_ (-1)
{
  ACE_Time_Value tv_delta;
  ORBSVCS_Time::TimeT_to_Time_Value (tv_delta, period);

  TAO_EC_Timeout_Generator *tg =
    this->event_channel_->timeout_generator ();

  if (this->type_ == ACE_ES_EVENT_INTERVAL_TIMEOUT
      || this->type_ == ACE_ES_EVENT_DEADLINE_TIMEOUT)
    {
      ACE_Time_Value tv_interval;
      ORBSVCS_Time::TimeT_to_Time_Value (tv_interval, this->period_);

      this->id_ =
        tg->schedule_timer (this,
                            tv_delta,
                            tv_interval);
    }
  else
    {
      this->id_ =
        tg->schedule_timer (this,
                            tv_delta,
                            ACE_Time_Value::zero);
    }

  //  ORBSVCS_DEBUG ((LM_DEBUG,
  //              "EC_Timeout_Filter - interval = %d:%d, ID = %d\n",
  //              tv_interval.sec (), tv_interval.usec (),
  //              this->id_));
}

TAO_EC_Timeout_Filter::~TAO_EC_Timeout_Filter (void)
{
  this->event_channel_->timeout_generator ()->cancel_timer (this->qos_info_,
                                                            this->id_);
}

void
TAO_EC_Timeout_Filter::push_to_proxy (const RtecEventComm::EventSet& event,
                                      TAO_EC_QOS_Info& qos_info)
{
  qos_info.timer_id_ = this->id_;

  if (this->supplier_ != 0)
    this->supplier_->filter (event,
                             qos_info);
}

int
TAO_EC_Timeout_Filter::filter (const RtecEventComm::EventSet &event,
                               TAO_EC_QOS_Info &qos_info)
{
  if (qos_info.timer_id_ == this->id_
      && this->parent () != 0)
    {
      this->parent ()->push (event, qos_info);
      return 1;
    }
  return 0;
}

int
TAO_EC_Timeout_Filter::filter_nocopy (RtecEventComm::EventSet &event,
                                      TAO_EC_QOS_Info &qos_info)
{
  if (qos_info.timer_id_ == this->id_
      && this->parent () != 0)
    {
      this->parent ()->push_nocopy (event, qos_info);
      return 1;
    }
  return 0;
}

void
TAO_EC_Timeout_Filter::push (const RtecEventComm::EventSet&,
                          TAO_EC_QOS_Info&)
{
}

void
TAO_EC_Timeout_Filter::push_nocopy (RtecEventComm::EventSet&,
                                    TAO_EC_QOS_Info&)
{
}

void
TAO_EC_Timeout_Filter::clear (void)
{
  if (this->type_ == ACE_ES_EVENT_DEADLINE_TIMEOUT)
    {
      TAO_EC_Timeout_Generator *tg =
        this->event_channel_->timeout_generator ();

      tg->cancel_timer (this->qos_info_,
                        this->id_);

      ACE_Time_Value tv_interval;
      ORBSVCS_Time::TimeT_to_Time_Value (tv_interval, this->period_);
      ACE_Time_Value tv_delta = tv_interval;

      this->id_ =
        tg->schedule_timer (this,
                            tv_delta,
                            tv_interval);
    }
}

CORBA::ULong
TAO_EC_Timeout_Filter::max_event_size (void) const
{
  return 1;
}

int
TAO_EC_Timeout_Filter::can_match (
      const RtecEventComm::EventHeader&) const
{
  return 0;
}

int
TAO_EC_Timeout_Filter::add_dependencies (
      const RtecEventComm::EventHeader&,
      const TAO_EC_QOS_Info &)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
