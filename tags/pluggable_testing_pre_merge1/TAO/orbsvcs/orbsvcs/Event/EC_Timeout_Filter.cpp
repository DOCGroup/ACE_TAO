// $Id$

#include "EC_Timeout_Filter.h"
#include "EC_Timeout_Generator.h"
#include "EC_Event_Channel.h"
#include "EC_ProxySupplier.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Timeout_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Timeout_Filter, "$Id$")

TAO_EC_Timeout_Filter::TAO_EC_Timeout_Filter (
      TAO_EC_Event_Channel *event_channel,
      TAO_EC_ProxyPushSupplier *supplier,
      const TAO_EC_QOS_Info& qos_info,
      RtecEventComm::EventType type,
      RtecEventComm::Time period)
  : event_channel_ (event_channel),
    supplier_ (supplier),
    qos_info_ (qos_info),
    type_ (type)
{
  ACE_Time_Value tv_delta;
  ORBSVCS_Time::TimeT_to_Time_Value (tv_delta, period);

  TAO_EC_Timeout_Generator *tg =
    this->event_channel_->timeout_generator ();

  if (type == ACE_ES_EVENT_INTERVAL_TIMEOUT)
    {
      ACE_Time_Value tv_interval;
      ORBSVCS_Time::TimeT_to_Time_Value (tv_interval, period);

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

  //  ACE_DEBUG ((LM_DEBUG,
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
                                      TAO_EC_QOS_Info& qos_info,
                                      CORBA::Environment& ACE_TRY_ENV)
{
  this->supplier_->push_timeout (this,
                                 event,
                                 qos_info,
                                 ACE_TRY_ENV);
}

int
TAO_EC_Timeout_Filter::filter (const RtecEventComm::EventSet& event,
                               TAO_EC_QOS_Info& qos_info,
                               CORBA::Environment& ACE_TRY_ENV)
{
  return 0;
}

int
TAO_EC_Timeout_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                      TAO_EC_QOS_Info& qos_info,
                                      CORBA::Environment& ACE_TRY_ENV)
{
  return 0;
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

int
TAO_EC_Timeout_Filter::add_dependencies (
      const RtecEventComm::EventHeader&,
      const TAO_EC_QOS_Info &,
      CORBA::Environment &)
{
  return 0;
}
