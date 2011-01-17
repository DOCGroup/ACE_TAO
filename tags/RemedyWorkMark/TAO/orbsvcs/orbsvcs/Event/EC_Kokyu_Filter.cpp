// $Id$

#include "orbsvcs/Event/EC_Kokyu_Filter.h"
#include "orbsvcs/Event/EC_QOS_Info.h"
#include "ace/Log_Msg.h"



TAO_EC_Kokyu_Filter::
    TAO_EC_Kokyu_Filter (const char* name,
                         RtecScheduler::handle_t rt_info,
                         RtecScheduler::Scheduler_ptr scheduler,
                         TAO_EC_Filter* body,
                         RtecScheduler::handle_t body_info,
                         RtecScheduler::handle_t parent_info,
                         RtecScheduler::Info_Type_t info_type)

  :  rt_info_ (rt_info),
     rt_info_computed_ (0),
     name_ (name),
     scheduler_ (RtecScheduler::Scheduler::_duplicate (scheduler)),
     body_ (body),
     body_info_ (body_info),
     parent_info_ (parent_info),
     info_type_ (info_type)
{
  this->adopt_child (this->body_);

#if 1 //by VS
this->rt_info_computed_ = 1;
#endif
}

TAO_EC_Kokyu_Filter::~TAO_EC_Kokyu_Filter (void)
{
  delete this->body_;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Kokyu_Filter::begin (void) const
{
  return this->body_->begin ();
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Kokyu_Filter::end (void) const
{
  return this->body_->end ();
}

int
TAO_EC_Kokyu_Filter::size (void) const
{
  return this->body_->size ();
}

int
TAO_EC_Kokyu_Filter::filter (const RtecEventComm::EventSet &event,
                             TAO_EC_QOS_Info& qos_info)
{
  return this->body_->filter (event, qos_info);
}

int
TAO_EC_Kokyu_Filter::filter_nocopy (RtecEventComm::EventSet &event,
                                    TAO_EC_QOS_Info& qos_info)
{
  return this->body_->filter_nocopy (event, qos_info);
}

// This is private, so we can make it inline in the .cpp file...
void
TAO_EC_Kokyu_Filter::compute_qos_info (TAO_EC_QOS_Info& qos_info)
{
  this->init_rt_info ();

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
                                    p_priority);
        qos_info.preemption_priority = p_priority;
      }
    }
}

void
TAO_EC_Kokyu_Filter::push (const RtecEventComm::EventSet &event,
                           TAO_EC_QOS_Info& qos_info)
{
  if (this->parent () != 0)
    {
      this->compute_qos_info (qos_info);

      this->parent ()->push (event, qos_info);
    }
}

void
TAO_EC_Kokyu_Filter::push_nocopy (RtecEventComm::EventSet &event,
                                  TAO_EC_QOS_Info& qos_info)
{
  if (this->parent () != 0)
    {
      this->compute_qos_info (qos_info);

      this->parent ()->push_nocopy (event, qos_info);
    }
}

void
TAO_EC_Kokyu_Filter::clear (void)
{
  this->body_->clear ();
}

CORBA::ULong
TAO_EC_Kokyu_Filter::max_event_size (void) const
{
  return this->body_->max_event_size ();
}

int
TAO_EC_Kokyu_Filter::can_match (const RtecEventComm::EventHeader& header) const
{
  return this->body_->can_match (header);
}

/*
    Kokyu_Filter
        |
        |body
        |
    Con/DisjunctionFilter
        |
        |children
        |*
    Kokyu_Filter
        |
        |body
        |
    Type_Filter

The entire hierarchy will have the same rt_info as the root. Only the root
rt_info will be part of the dependency graph in the scheduler. The root
rt_info will have a dependency on the consumer rt_info.

<--- shows dependency (oneway)

supplier1<-----|
          con/disj_rt_info<-------consumer_rt_info
supplier2<-----|
*/

int
TAO_EC_Kokyu_Filter::add_dependencies (const RtecEventComm::EventHeader& header,
                                       const TAO_EC_QOS_Info &qos_info)
{
#ifdef EC_KOKYU_LOGGING
  ACE_DEBUG ((LM_DEBUG, "Entering EC_Kokyu_Filter::add_dependencies\n"));
#endif
  this->init_rt_info ();

#ifdef EC_KOKYU_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "this->rt_info_ = %d, header.type = %d, qos_info.rt_info = %d\n",
              this->rt_info_, header.type, qos_info.rt_info));
#endif

  //this call the add_dependencies() on con/disjunction filter
  int matches = this->body_->add_dependencies (header,
                                               qos_info);

  if (matches != 0)
    {
#ifdef EC_KOKYU_LOGGING
      ACE_DEBUG ((LM_DEBUG, "Kokyu_Filter::matches != 0\n"));
#endif
      this->scheduler_->add_dependency (this->rt_info_, qos_info.rt_info, 1,
                                        RtecBase::ONE_WAY_CALL);

      RtecScheduler::RT_Info_var info =
        this->scheduler_->get (qos_info.rt_info);
      ACE_DEBUG ((LM_DEBUG, "[%s][%d] ----> [%s][%d]\n",
                  this->name_.c_str (),
                  this->rt_info_,
                  info->entry_point.in (),
                  qos_info.rt_info));
    }
  else
    {
#ifdef EC_KOKYU_LOGGING
      ACE_DEBUG ((LM_DEBUG, "Kokyu_Filter::matches == 0\n"));
#endif
    }

#ifdef EC_KOKYU_LOGGING
  ACE_DEBUG ((LM_DEBUG, "about to iterate thru children\n"));
#endif
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin (); i != end; ++i)
    {
      (*i)->add_dependencies (header, qos_info);
    }
#ifdef EC_KOKYU_LOGGING
  ACE_DEBUG ((LM_DEBUG, "Exiting EC_Kokyu_Filter: add_dependencies\n"));
#endif
  return 0;
}

void
TAO_EC_Kokyu_Filter::get_qos_info (TAO_EC_QOS_Info& qos_info)
{
  this->init_rt_info ();

  qos_info.rt_info = this->rt_info_;
}

void
TAO_EC_Kokyu_Filter::init_rt_info (void)
{
  if (this->rt_info_computed_)
    return;

#if 1 //ifdef'ed from 1 to 0 by VS

  // Provide dummy values the scheduler will compute them based on the
  // dependencies and the fact that this is a DISJUNCTION.
  this->scheduler_->set (this->rt_info_,
                         RtecScheduler::VERY_LOW_CRITICALITY,
                         0, // worst_cast_execution_time
                         0, // typical_cast_execution_time
                         0, // cached_cast_execution_time
                         0, // period
                         RtecScheduler::VERY_LOW_IMPORTANCE,
                         0, // quantum
                         0, // threads
                         this->info_type_);
#endif  //ifdef'ed by VS

#if 0 //ifdef changed from 1 to 0 by VS
  if (this->body_info_ != this->rt_info_)
    {
      this->scheduler_->add_dependency (this->rt_info_,
                                        this->body_info_,
                                        1,
                                        RtecBase::TWO_WAY_CALL);

      RtecScheduler::RT_Info_var info =
        this->scheduler_->get (this->body_info_);
      ACE_DEBUG ((LM_DEBUG, "[%s] ----> [%s]\n",
                  info->entry_point.in (),
                  this->name_.c_str ()));
    }
#endif /* 0 */

#if 0 //ifdef changed from 1 to 0 by VS
  this->scheduler_->add_dependency (this->parent_info_,
                                    this->rt_info_,
                                    1,
                                    RtecBase::TWO_WAY_CALL);

  RtecScheduler::RT_Info_var info =
    this->scheduler_->get (this->parent_info_);
  ACE_DEBUG ((LM_DEBUG, "[%s] ----> [%s]\n",
              this->name_.c_str (),
              info->entry_point.in ()));
#endif /* 0 */

  this->rt_info_computed_ = 1;
}
