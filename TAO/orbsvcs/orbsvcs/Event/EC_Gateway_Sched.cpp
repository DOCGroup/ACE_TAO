// $Id$

#include "orbsvcs/Event/EC_Gateway_Sched.h"
#include "orbsvcs/Time_Utilities.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Gateway_Sched::TAO_EC_Gateway_Sched (void)
  :  TAO_EC_Gateway_IIOP ()
{
}

TAO_EC_Gateway_Sched::~TAO_EC_Gateway_Sched (void)
{
}

void
TAO_EC_Gateway_Sched::init (RtecEventChannelAdmin::EventChannel_ptr supplier_ec,
                           RtecEventChannelAdmin::EventChannel_ptr consumer_ec,
                           RtecScheduler::Scheduler_ptr supplier_sched,
                           RtecScheduler::Scheduler_ptr consumer_sched,
                           const char* consumer_name,
                           const char* supplier_name)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  this->init_i (supplier_ec, consumer_ec);

  // @@ Should we throw a system exception here?
  if (CORBA::is_nil (supplier_sched)
      || CORBA::is_nil (consumer_sched)
      || consumer_name == 0
      || supplier_name == 0)
    throw CORBA::BAD_PARAM ();

  this->supplier_info_ =
    supplier_sched->create (supplier_name);

  // @@ TODO Many things are hard-coded in the RT_Info here.

  // The worst case execution time is far less than 500 usecs, but
  // that is a safe estimate....
  ACE_Time_Value tv (0, 500);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  supplier_sched->set (this->supplier_info_,
                       RtecScheduler::VERY_HIGH_CRITICALITY,
                       time, time, time,
                       25000 * 10,
                       RtecScheduler::VERY_LOW_IMPORTANCE,
                       time,
                       0,
                       RtecScheduler::OPERATION);

  this->consumer_info_ =
    consumer_sched->create (consumer_name);

  tv = ACE_Time_Value (0, 500);
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  consumer_sched->set (this->consumer_info_,
                       RtecScheduler::VERY_HIGH_CRITICALITY,
                       time, time, time,
                       25000 * 10,
                       RtecScheduler::VERY_LOW_IMPORTANCE,
                       time,
                       1,
                       RtecScheduler::REMOTE_DEPENDANT);
}

TAO_END_VERSIONED_NAMESPACE_DECL
