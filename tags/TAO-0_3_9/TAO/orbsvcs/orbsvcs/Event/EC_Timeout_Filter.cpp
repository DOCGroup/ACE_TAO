// $Id$

#include "EC_Timeout_Filter.h"
#include "EC_Timeout_Generator.h"
#include "EC_Event_Channel.h"
#include "orbsvcs/Time_Utilities.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Timeout_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Timeout_Filter, "$Id$")

TAO_EC_Timeout_Filter::TAO_EC_Timeout_Filter (
      TAO_EC_Event_Channel *event_channel,
      const TAO_EC_QOS_Info& qos_info,
      RtecEventComm::Time period)
  : event_channel_ (event_channel),
    qos_info_ (qos_info)
{
  ACE_Time_Value tv_delta;
  ORBSVCS_Time::TimeT_to_Time_Value (tv_delta, period);

  ACE_Time_Value tv_interval;
  ORBSVCS_Time::TimeT_to_Time_Value (tv_interval, period);

  TAO_EC_Timeout_Generator *tg =
    this->event_channel_->timeout_generator ();
  this->id_ =
    tg->schedule_timer (this,
                        tv_delta,
                        tv_interval);
}

TAO_EC_Timeout_Filter::~TAO_EC_Timeout_Filter (void)
{
  this->event_channel_->timeout_generator ()->cancel_timer (this->qos_info_,
                                                            this->id_);
}

int
TAO_EC_Timeout_Filter::filter (const RtecEventComm::EventSet& event,
                               TAO_EC_QOS_Info& qos_info,
                               CORBA::Environment& ACE_TRY_ENV)
{
  this->push (event, qos_info, ACE_TRY_ENV);
  return 1;
}

int
TAO_EC_Timeout_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info,
                                   CORBA::Environment& ACE_TRY_ENV)
{
  this->push_nocopy (event, qos_info, ACE_TRY_ENV);
  return 1;
}

void
TAO_EC_Timeout_Filter::push (const RtecEventComm::EventSet& event,
                          TAO_EC_QOS_Info& qos_info,
                          CORBA::Environment& ACE_TRY_ENV)
{
  if (this->parent () != 0)
    this->parent ()->push (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_Timeout_Filter::push_nocopy (RtecEventComm::EventSet& event,
                                 TAO_EC_QOS_Info& qos_info,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  if (this->parent () != 0)
    this->parent ()->push_nocopy (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_Timeout_Filter::clear (void)
{
}

CORBA::ULong
TAO_EC_Timeout_Filter::max_event_size (void) const
{
  return 1;
}

int
TAO_EC_Timeout_Filter::can_match (
      const RtecEventComm::EventHeader& header) const
{
  return 0;
}
