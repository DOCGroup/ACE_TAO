// $Id$

#include "EC_Sched_Filter.h"
#include "EC_QOS_Info.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Sched_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Sched_Filter, "$Id$")

TAO_EC_Sched_Filter::
    TAO_EC_Sched_Filter (const char* name,
                         RtecScheduler::Scheduler_ptr scheduler,
                         TAO_EC_Filter* body,
                         RtecScheduler::handle_t body_info,
                         RtecScheduler::Info_Type_t info_type)

  :  rt_info_ (-1),
     name_ (name),
     scheduler_ (RtecScheduler::Scheduler::_duplicate (scheduler)),
     body_ (body),
     body_info_ (body_info),
     info_type_ (info_type)
{
  this->adopt_child (this->body_);
}

TAO_EC_Sched_Filter::~TAO_EC_Sched_Filter (void)
{
  delete this->body_;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Sched_Filter::begin (void) const
{
  return this->body_->begin ();
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Sched_Filter::end (void) const
{
  return this->body_->end ();
}

int
TAO_EC_Sched_Filter::size (void) const
{
  return this->body_->size ();
}

int
TAO_EC_Sched_Filter::filter (const RtecEventComm::EventSet &event,
                             TAO_EC_QOS_Info& qos_info,
                             CORBA::Environment &ACE_TRY_ENV)
{
  return this->body_->filter (event, qos_info, ACE_TRY_ENV);
}

int
TAO_EC_Sched_Filter::filter_nocopy (RtecEventComm::EventSet &event,
                                    TAO_EC_QOS_Info& qos_info,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  return this->body_->filter_nocopy (event, qos_info, ACE_TRY_ENV);
}

// This is private, so we can make it inline in the .cpp file...
ACE_INLINE void
TAO_EC_Sched_Filter::compute_qos_info (TAO_EC_QOS_Info& qos_info,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  this->init_rt_info (ACE_TRY_ENV);
  ACE_CHECK;

  qos_info.rt_info = this->rt_info_;
  switch (this->info_type_)
    {
    default:
    case RtecScheduler::DISJUNCTION:
      break;

    case RtecScheduler::CONJUNCTION:
    case RtecScheduler::OPERATION:
      {
        RtecScheduler::OS_Priority os_priority;
        RtecScheduler::Preemption_Subpriority_t p_subpriority;
        RtecScheduler::Preemption_Priority_t p_priority;
        this->scheduler_->priority (this->rt_info_,
                                    os_priority,
                                    p_subpriority,
                                    p_priority,
                                    ACE_TRY_ENV);
        ACE_CHECK;
        qos_info.preemption_priority = p_priority;
      }
    }
}

void
TAO_EC_Sched_Filter::push (const RtecEventComm::EventSet &event,
                           TAO_EC_QOS_Info& qos_info,
                           CORBA::Environment &ACE_TRY_ENV)
{
  if (this->parent () != 0)
    {
      this->compute_qos_info (qos_info, ACE_TRY_ENV);
      ACE_CHECK;

      this->parent ()->push (event, qos_info, ACE_TRY_ENV);
    }
}

void
TAO_EC_Sched_Filter::push_nocopy (RtecEventComm::EventSet &event,
                                  TAO_EC_QOS_Info& qos_info,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  if (this->parent () != 0)
    {
      this->compute_qos_info (qos_info, ACE_TRY_ENV);
      ACE_CHECK;

      this->parent ()->push_nocopy (event, qos_info, ACE_TRY_ENV);
    }
}

void
TAO_EC_Sched_Filter::clear (void)
{
  this->body_->clear ();
}

CORBA::ULong
TAO_EC_Sched_Filter::max_event_size (void) const
{
  return this->body_->max_event_size ();
}

int
TAO_EC_Sched_Filter::can_match (const RtecEventComm::EventHeader& header) const
{
  return this->body_->can_match (header);
}

int
TAO_EC_Sched_Filter::add_dependencies (const RtecEventComm::EventHeader& header,
                                       const TAO_EC_QOS_Info &qos_info,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  this->init_rt_info (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  int matches = this->body_->add_dependencies (header,
                                               qos_info,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (matches != 0)
    this->scheduler_->add_dependency (this->rt_info_, qos_info.rt_info, 1,
                                      RtecScheduler::TWO_WAY_CALL,
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin (); i != end; ++i)
    {
      (*i)->add_dependencies (header, qos_info, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }
  return 0;
}

void
TAO_EC_Sched_Filter::get_qos_info (TAO_EC_QOS_Info& qos_info,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  this->init_rt_info (ACE_TRY_ENV);
  ACE_CHECK;

  qos_info.rt_info = this->rt_info_;
}

void
TAO_EC_Sched_Filter::init_rt_info (CORBA::Environment &ACE_TRY_ENV)
{
  if (this->rt_info_ != -1)
    return;

  // Create an entry in the Scheduling service...
  RtecScheduler::handle_t rt_info =
    this->scheduler_->create (this->name_.c_str (),
                              ACE_TRY_ENV);
  ACE_CHECK;

  // Provide dummy values the scheduler will compute them based on the
  // dependencies and the fact that this is a DISJUNCTION.
  this->scheduler_->set (rt_info,
                         RtecScheduler::VERY_LOW_CRITICALITY,
                         0, // worst_cast_execution_time
                         0, // typical_cast_execution_time
                         0, // cached_cast_execution_time
                         0, // period
                         RtecScheduler::VERY_LOW_IMPORTANCE,
                         0, // quantum
                         0, // threads
                         this->info_type_,
                         ACE_TRY_ENV);
  ACE_CHECK;

  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin (); i != end; ++i)
    {
      TAO_EC_Filter* filter = *i;

      TAO_EC_QOS_Info child;
      filter->get_qos_info (child, ACE_TRY_ENV);
      ACE_CHECK;

      this->scheduler_->add_dependency (rt_info, child.rt_info, 1,
                                        RtecScheduler::TWO_WAY_CALL,
                                        ACE_TRY_ENV);
      ACE_CHECK;
    }
  this->scheduler_->add_dependency (this->body_info_, rt_info, 1,
                                    RtecScheduler::TWO_WAY_CALL,
                                    ACE_TRY_ENV);
  ACE_CHECK;

  this->rt_info_ = rt_info;
}
