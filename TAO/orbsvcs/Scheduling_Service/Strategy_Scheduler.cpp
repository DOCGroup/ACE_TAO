// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    Strategy_Scheduler.cpp
//
// = CREATION DATE
//    22 December 1997
//
// = AUTHOR
//    Chris Gill
//
// ============================================================================

#include "math.h"
#include "float.h"

#include "ace/Sched_Params.h"

#include "Strategy_Scheduler.h"

#if ! defined (__ACE_INLINE__)
#include "Strategy_Scheduler.i"
#endif /* __ACE_INLINE__ */

//////////////////////////////////////////////
// helper function type definition for sort //
//////////////////////////////////////////////

// this is awkward, but it makes MSVC++ happy
extern "C"
{
typedef int (*COMP_FUNC) (const void*, const void*);
}

///////////////////////////////////////////////////
// class ACE_Strategy_Scheduler member functions //
///////////////////////////////////////////////////

ACE_Strategy_Scheduler::ACE_Strategy_Scheduler (ACE_Scheduler_Strategy &strategy)
  : ACE_Scheduler (),
    strategy_ (strategy)
{
}
  // = default ctor


ACE_Strategy_Scheduler::~ACE_Strategy_Scheduler ()
{
}
  // = virtual dtor


ACE_Scheduler::status_t
ACE_Strategy_Scheduler::schedule_i (void)
{
  // sort the entries in order of priority and subpriority
  strategy_.sort (ordered_task_entries_, tasks ());

  // assign priorities and sub_priorities
  return assign_priorities ();
}
  // = sets up the schedule in the order generated 
  //   by the strategy's comparison operators


ACE_Scheduler::status_t
ACE_Strategy_Scheduler::assign_priorities (void)
{
  // start with happy status
  ACE_Scheduler::status_t status = ACE_Scheduler::SUCCEEDED;

  // start with the highest OS priority in the given range and work downward:
  // if we run out of values to assign, lowest priority levels are merged.
  int current_OS_priority = maximum_priority_;

  // start subpriority count at 1
  u_int subpriorities = 1;

  // start scheduler priority at 0 (highest priority queue number)
  // NOTE: 0 is highest for priority, lowest for subpriority
  ACE_Sched_Priority current_scheduler_priority = 0;

  // value the OS and scheduler priorities in 0th entry's underlying RT_Info
  ordered_task_entries_[0]->rt_info ()->priority = current_OS_priority;
  ordered_task_entries_[0]->rt_info ()->preemption_priority = current_scheduler_priority;

  // traverse ordered task entry array, assigning decreasing priority and
  // subpriority (array is sorted from highest to lowest priority)
  for (u_int i = 1; i < tasks (); ++i)
  {   
    switch (strategy_.priority_comp (*ordered_task_entries_[i-1], 
                                     *ordered_task_entries_[i]))
    {
      case -1:  // the current entry is at lower priority than the previous
		{  
		// decrease priority by incrementing the current scheduling priority
        //  number: 0 is the highest priority number.
        ++current_scheduler_priority;

        // check for OS priority level boundaries: because OS priority values
        // can run in either increasing or decreasing order, there is no easy,
        // portable way to check other than exact comparison to the bounds
        // that were given in init () or that came from the platform itself.
        if ((current_OS_priority == minimum_priority_) ||
            (current_OS_priority == ACE_Sched_Params::previous_priority (
                                      ACE_SCHED_FIFO,
                                      current_OS_priority,
                                      ACE_SCOPE_PROCESS)))
        {
          // we're still in range, so decrement the current OS priority level
          current_OS_priority = 
            ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                 current_OS_priority,
                                                 ACE_SCOPE_PROCESS);
        }
        else
        {
          // if we have run out of priority levels to assign, indicate
          // this in the return status, but keep right on assigning the
          // minimum OS priority in the range to the remaining tasks.
          status = ACE_Scheduler::ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS;
        }

        // assign subpriorities to the RT_Infos of entries in previous level
        for (u_int j = 1; j <= subpriorities ; ++j)
        {
          ordered_task_entries_ [i - j]->rt_info ()->dynamic_subpriority = 
            subpriorities - j;
        }

        // reset the subpriority counter
        subpriorities = 0;

		break;
		}
      case 0:  // still at the same priority level

        break;

      default: // should never reach here: something *bad* has happened

        ACE_ERROR_RETURN ((LM_ERROR, 
                           "Priority assignment failure: tasks"
                           " \"%s\" and \"%s\" are out of order.\n", 
                           ordered_task_entries_ [i-1]->rt_info ()->entry_point, 
                           ordered_task_entries_ [i]->rt_info ()->entry_point), 
                          ACE_Scheduler::ST_INVALID_PRIORITY_ORDERING);
    }      

    // increment subpriority counter
    ++subpriorities;

    // assign the current OS priority to the current entry's underlying RT_Info
    ordered_task_entries_[i]->rt_info ()->priority = current_OS_priority;

    // set scheduling priority in current entry's RT_Info
    ordered_task_entries_[0]->rt_info ()->preemption_priority = current_scheduler_priority;
  }

  
  return status;  
}
  // = assigns priorities and sub-priorities to the sorted schedule, 
  //   according to the strategy's priority comparison operator.


////////////////////////////////////////////////////////////////////
// class template ACE_Strategy_Scheduler_Factory member functions //
////////////////////////////////////////////////////////////////////

template <class STRATEGY>
ACE_Strategy_Scheduler * 
ACE_Strategy_Scheduler_Factory<STRATEGY>::create ()
{
  ACE_Strategy_Scheduler *the_scheduler = 0;
  STRATEGY *the_strategy;

  the_strategy = STRATEGY::instance ();

  if (the_strategy)
  {
    ACE_NEW_RETURN (the_scheduler, ACE_Strategy_Scheduler (*the_strategy), 0);
  }
  else
  {
    ACE_ERROR ((LM_ERROR, "ACE_Strategy_Scheduler_Factory::create () "
                          "failed to acquire strategy instance.\n"));
  }

  return the_scheduler;
};
  // construct and return a scheduler strategized with
  // an instance of the the parameterized strategy type



/////////////////////////////////////////////////////////////////
// abstract base class ACE_Scheduler_Strategy member functions //
/////////////////////////////////////////////////////////////////

int 
ACE_Scheduler_Strategy::sort_comp
  (const ACE_Scheduler::Task_Entry &first_entry, 
   const ACE_Scheduler::Task_Entry &second_entry)
{
  // order first by the priority ordering
  int result = priority_comp (first_entry, second_entry);

  // within same priority, order by dynamic subpriority
  if (result == 0)
  {
    result = dynamic_subpriority_comp (first_entry, second_entry);
  }

  // if same dynamic subpriority, order by static subpriority
  if (result == 0)
  {
    result = static_subpriority_comp (first_entry, second_entry);
  }

  return result;
}
  // = comparison of two task entries using the specific priority, dynamic
  //   subpriority, and static subpriority method definitions provided by 
  //   the derived strategy class to produce the strategy specific sort 
  //   ordering: returns -1 if the first Task_Entry is greater in the order,
  //   0 if they are equivalent, or 1 if the second Task_Entry is greater in
  //   the order


ACE_Scheduler_Strategy::static_subpriority_comp (
  const ACE_Scheduler::Task_Entry &first_entry,
  const ACE_Scheduler::Task_Entry &second_entry)
{
  // order first by importance assigned to underlying RT_Info (descending)
  if (first_entry.rt_info ()->importance > second_entry.rt_info ()->importance)
  {
    return -1;
  }
  else if (first_entry.rt_info ()->importance < second_entry.rt_info ()->importance)
  {
    return 1;
  }
  else
  {
    // order last by the topological sort finishing time (ascending)
    if (first_entry.finished () < second_entry.finished ())
    {
      return -1;
    }
    else if (first_entry.finished () > second_entry.finished ())
    {
      return 1;
    }
    else
    {
      // should never get here: all entries should be ordered by finishing time
      ACE_ERROR ((LM_ERROR, 
                  "minimal ordering failure for tasks \"%s\" and \"%s\".\n",
                  first_entry.rt_info ()->entry_point, 
                  second_entry.rt_info ()->entry_point));
      return 0;
    }
  }
}

/////////////////////////////////////////////////////////////////////////
// class ACE_MUF_Scheduler_Strategy static data member initializations //
/////////////////////////////////////////////////////////////////////////

ACE_MUF_Scheduler_Strategy * ACE_MUF_Scheduler_Strategy::instance_ = 0;

///////////////////////////////////////////////////////
// class ACE_MUF_Scheduler_Strategy member functions //
///////////////////////////////////////////////////////

ACE_MUF_Scheduler_Strategy *
ACE_MUF_Scheduler_Strategy::instance ()
{
  if (0 == ACE_MUF_Scheduler_Strategy::instance_)
  {
    ACE_NEW_RETURN (ACE_MUF_Scheduler_Strategy::instance_, 
                    ACE_MUF_Scheduler_Strategy, 0);
  }

  return ACE_MUF_Scheduler_Strategy::instance_;
}

int 
ACE_MUF_Scheduler_Strategy::priority_comp (const ACE_Scheduler::Task_Entry &first_entry, 
                                           const ACE_Scheduler::Task_Entry &second_entry)
{
  // order by criticality (descending)
  if (first_entry.rt_info ()->criticality > second_entry.rt_info ()->criticality)
  {
    return -1;
  }
  else if (first_entry.rt_info ()->criticality < second_entry.rt_info ()->criticality)
  {
    return 1;
  }
  else
  {
    return 0;  // same priority level
  }
}
  // = comparison of two task entries by maximum criticality: returns -1 if the
  //   first Task_Entry is greater in the order, 0 if they're equivalent, or
  //   1 if the second Task_Entry is greater in the order.


void 
ACE_MUF_Scheduler_Strategy::sort (
  ACE_Scheduler::Task_Entry **ordered_task_entries_, u_int tasks)
{
  ::qsort ((void *) ordered_task_entries_, 
           tasks,
           sizeof (ACE_Scheduler::Task_Entry *),
           (COMP_FUNC) ACE_MUF_Scheduler_Strategy::sort_function);
}
  // = sort the task entry pointer array in descending urgency order


ACE_MUF_Scheduler_Strategy::ACE_MUF_Scheduler_Strategy () 
{
}
    // = default ctor

ACE_MUF_Scheduler_Strategy::~ACE_MUF_Scheduler_Strategy ()
{
}
    // = virtual dtor

int 
ACE_MUF_Scheduler_Strategy::dynamic_subpriority_comp
  (const ACE_Scheduler::Task_Entry &first_entry, 
   const ACE_Scheduler::Task_Entry &second_entry)
{
  // order by descending dynamic priority according to ascending laxity
  RtecScheduler::Time laxity1 = ((RtecScheduler::Time) first_entry.rt_info ()->period) -
                                first_entry.rt_info ()->worst_case_execution_time;

  RtecScheduler::Time laxity2 = ((RtecScheduler::Time) second_entry.rt_info_->period) -
                                second_entry.rt_info_->worst_case_execution_time;

  if (laxity1 < laxity2)
  {
    return -1;
  }
  else if (laxity1 > laxity2)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
  // = orders of two task entries by ascending laxity: returns -1 if the 
  // first Task_Entry is greater in the order, 0 if they're equivalent, 
  // 1 if the second Task_Entry is greater in the order.


int 
ACE_MUF_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_MUF_Scheduler_Strategy::instance ()->
           sort_comp (** ACE_static_cast (ACE_Scheduler::Task_Entry **, arg1), 
                      ** ACE_static_cast (ACE_Scheduler::Task_Entry **, arg2));
}
  // comparison function to pass to qsort

/////////////////////////////////////////////////////////////////////////
// class ACE_RMS_Scheduler_Strategy static data member initializations //
/////////////////////////////////////////////////////////////////////////

ACE_RMS_Scheduler_Strategy * ACE_RMS_Scheduler_Strategy::instance_ = 0;

///////////////////////////////////////////////////////
// class ACE_RMS_Scheduler_Strategy member functions //
///////////////////////////////////////////////////////

ACE_RMS_Scheduler_Strategy *
ACE_RMS_Scheduler_Strategy::instance ()
{
  if (0 == ACE_RMS_Scheduler_Strategy::instance_)
  {
    ACE_NEW_RETURN (ACE_RMS_Scheduler_Strategy::instance_, 
                    ACE_RMS_Scheduler_Strategy, 0);
  }

  return ACE_RMS_Scheduler_Strategy::instance_;
}

int 
ACE_RMS_Scheduler_Strategy::priority_comp (const ACE_Scheduler::Task_Entry &first_entry, 
                                           const ACE_Scheduler::Task_Entry &second_entry)
{
  // compare by decreasing period
  if (first_entry.rt_info_->period < second_entry.rt_info_->period)
  {
    return -1;
  }
  else if (first_entry.rt_info_->period > second_entry.rt_info_->period)
  {
    return 1;
  }
  else
  {
    return 0;  // same priority level
  }
}
  // = comparison of two task entries by minimum period: returns -1 if the
  //   first Task_Entry is greater in the order, 0 if they're equivalent,
  //   or 1 if the second Task_Entry is greater in the order.

void 
ACE_RMS_Scheduler_Strategy::sort (
  ACE_Scheduler::Task_Entry **ordered_task_entries_, u_int tasks)
{
  ::qsort ((void *) ordered_task_entries_, 
           tasks,
           sizeof (ACE_Scheduler::Task_Entry *),
           (COMP_FUNC) ACE_RMS_Scheduler_Strategy::sort_function);
}
  // = sort the task entry pointer array in descending RMS (rate) order

ACE_RMS_Scheduler_Strategy::ACE_RMS_Scheduler_Strategy ()
{
}
    // = default ctor

ACE_RMS_Scheduler_Strategy::~ACE_RMS_Scheduler_Strategy ()
{
}
    // = virtual dtor

int 
ACE_RMS_Scheduler_Strategy::dynamic_subpriority_comp
  (const ACE_Scheduler::Task_Entry &first_entry, 
   const ACE_Scheduler::Task_Entry &second_entry)
{
  return 0;
}
  // = all tasks in a given priority level have the same dynamic
  //   subpriority under RMS


int 
ACE_RMS_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_RMS_Scheduler_Strategy::instance ()->
           sort_comp (** ACE_static_cast (ACE_Scheduler::Task_Entry **, arg1), 
                      ** ACE_static_cast (ACE_Scheduler::Task_Entry **, arg2));
}
  // comparison function to pass to qsort


/////////////////////////////////////////////////////////////////////////
// class ACE_MLF_Scheduler_Strategy static data member initializations //
/////////////////////////////////////////////////////////////////////////

ACE_MLF_Scheduler_Strategy * ACE_MLF_Scheduler_Strategy::instance_ = 0;

///////////////////////////////////////////////////////
// class ACE_MLF_Scheduler_Strategy member functions //
///////////////////////////////////////////////////////

ACE_MLF_Scheduler_Strategy *
ACE_MLF_Scheduler_Strategy::instance ()
{
  if (0 == ACE_MLF_Scheduler_Strategy::instance_)
  {
    ACE_NEW_RETURN (ACE_MLF_Scheduler_Strategy::instance_, 
                    ACE_MLF_Scheduler_Strategy, 0);
  }

  return ACE_MLF_Scheduler_Strategy::instance_;
}

int
ACE_MLF_Scheduler_Strategy::priority_comp (const ACE_Scheduler::Task_Entry &first_entry, 
                                           const ACE_Scheduler::Task_Entry &second_entry)
{
  return 0;
}
  // = just returns 0, as all task entries are of equivalent priority under MLF.

void 
ACE_MLF_Scheduler_Strategy::sort (
  ACE_Scheduler::Task_Entry **ordered_task_entries_, u_int tasks)
{
  ::qsort ((void *) ordered_task_entries_, 
           tasks,
           sizeof (ACE_Scheduler::Task_Entry *),
           (COMP_FUNC) ACE_MLF_Scheduler_Strategy::sort_function);
}
  // = sort the task entry pointer array in ascending laxity order


ACE_MLF_Scheduler_Strategy::ACE_MLF_Scheduler_Strategy ()
{
}
    // = default ctor

ACE_MLF_Scheduler_Strategy::~ACE_MLF_Scheduler_Strategy ()
{
}
    // = virtual dtor


int
ACE_MLF_Scheduler_Strategy::dynamic_subpriority_comp
  (const ACE_Scheduler::Task_Entry &first_entry, 
   const ACE_Scheduler::Task_Entry &second_entry)
{
  // order by laxity (ascending)
  RtecScheduler::Time laxity1 = ((RtecScheduler::Time) first_entry.rt_info_->period) -
                                first_entry.rt_info_->worst_case_execution_time;

  RtecScheduler::Time laxity2 = ((RtecScheduler::Time) second_entry.rt_info_->period) -
                                second_entry.rt_info_->worst_case_execution_time;

  if (laxity1 < laxity2)
  {
    return -1;
  }
  else if (laxity1 > laxity2)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
  // = orders two task entries by ascending laxity: returns -1 if the
  //   first Task_Entry is greater in the order, 0 if they're equivalent,
  //   or 1 if the second Task_Entry is greater in the order.


int 
ACE_MLF_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_MLF_Scheduler_Strategy::instance ()->
           sort_comp (** ACE_static_cast (ACE_Scheduler::Task_Entry **, arg1), 
                      ** ACE_static_cast (ACE_Scheduler::Task_Entry **, arg2));
}
  // comparison function to pass to qsort


/////////////////////////////////////////////////////////////////////////
// class ACE_EDF_Scheduler_Strategy static data member initializations //
/////////////////////////////////////////////////////////////////////////

ACE_EDF_Scheduler_Strategy * ACE_EDF_Scheduler_Strategy::instance_ = 0;

///////////////////////////////////////////////////////
// class ACE_EDF_Scheduler_Strategy member functions //
///////////////////////////////////////////////////////

ACE_EDF_Scheduler_Strategy *
ACE_EDF_Scheduler_Strategy::instance ()
{
  if (0 == ACE_EDF_Scheduler_Strategy::instance_)
  {
    ACE_NEW_RETURN (ACE_EDF_Scheduler_Strategy::instance_, 
                    ACE_EDF_Scheduler_Strategy, 0);
  }

  return ACE_EDF_Scheduler_Strategy::instance_;
}


int
ACE_EDF_Scheduler_Strategy::priority_comp (const ACE_Scheduler::Task_Entry &first_entry, 
                                           const ACE_Scheduler::Task_Entry &second_entry)
{
  return 0;
}
  // = just returns 0, as all task entries are of equivalent priority under EDF.

void 
ACE_EDF_Scheduler_Strategy::sort (
  ACE_Scheduler::Task_Entry **ordered_task_entries_, u_int tasks)
{
  ::qsort ((void *) ordered_task_entries_, 
           tasks,
           sizeof (ACE_Scheduler::Task_Entry *),
           (COMP_FUNC) ACE_EDF_Scheduler_Strategy::sort_function);
}
  // = sort the task entry pointer array in ascending deadline (period) order


ACE_EDF_Scheduler_Strategy::ACE_EDF_Scheduler_Strategy ()
{
}
    // = default ctor

ACE_EDF_Scheduler_Strategy::~ACE_EDF_Scheduler_Strategy ()
{
}
    // = virtual dtor

int
ACE_EDF_Scheduler_Strategy::dynamic_subpriority_comp
  (const ACE_Scheduler::Task_Entry &first_entry, 
   const ACE_Scheduler::Task_Entry &second_entry)
{
  // order by period (ascending)
  if (first_entry.rt_info_->period < second_entry.rt_info_->period)
  {
    return -1;
  }
  else if (first_entry.rt_info_->period > second_entry.rt_info_->period)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
  // = orders two task entries by ascending time to deadline: returns -1 if
  //   the first Task_Entry is greater in the order, 0 if they're equivalent,
  //   or 1 if the second Task_Entry is greater in the order.


int 
ACE_EDF_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_EDF_Scheduler_Strategy::instance ()->
           sort_comp (** ACE_static_cast (ACE_Scheduler::Task_Entry **, arg1), 
                      ** ACE_static_cast (ACE_Scheduler::Task_Entry **, arg2));
}
  // comparison function to pass to qsort

/////////////////////////////////////////////////////////////////////////////
// class ACE_RMS_Dyn_Scheduler_Strategy static data member initializations //
/////////////////////////////////////////////////////////////////////////////

ACE_RMS_Dyn_Scheduler_Strategy * ACE_RMS_Dyn_Scheduler_Strategy::instance_ = 0;

///////////////////////////////////////////////////////////
// class ACE_RMS_Dyn_Scheduler_Strategy member functions //
///////////////////////////////////////////////////////////

ACE_RMS_Dyn_Scheduler_Strategy *
ACE_RMS_Dyn_Scheduler_Strategy::instance ()
{
  if (0 == ACE_RMS_Dyn_Scheduler_Strategy::instance_)
  {
    ACE_NEW_RETURN (ACE_RMS_Dyn_Scheduler_Strategy::instance_, 
                    ACE_RMS_Dyn_Scheduler_Strategy, 0);
  }

  return ACE_RMS_Dyn_Scheduler_Strategy::instance_;
}


int
ACE_RMS_Dyn_Scheduler_Strategy::priority_comp (const ACE_Scheduler::Task_Entry &first_entry, 
                                               const ACE_Scheduler::Task_Entry &second_entry)
{
  // order by criticality (descending)
  if (first_entry.rt_info_->criticality > second_entry.rt_info_->criticality)
  {
    return -1;
  }
  else if (first_entry.rt_info_->criticality < second_entry.rt_info_->criticality)
  {
    return 1;
  }
  else
  {
    // they're identical as far as we're concerned
    return 0;
  }
}
  // = comparison of two task entries by maximum criticality: returns -1 
  //   if the first Task_Entry is greater in the order, 0 if they're
  //   equivalent, or 1 if the second Task_Entry is greater in the order.


void 
ACE_RMS_Dyn_Scheduler_Strategy::sort (
  ACE_Scheduler::Task_Entry **ordered_task_entries_, u_int tasks)
{
  ::qsort ((void *) ordered_task_entries_, 
           tasks,
           sizeof (ACE_Scheduler::Task_Entry *),
           (COMP_FUNC) ACE_RMS_Dyn_Scheduler_Strategy::sort_function);
}
  // = sort the task entry pointer array in descending priority order


ACE_RMS_Dyn_Scheduler_Strategy::ACE_RMS_Dyn_Scheduler_Strategy ()
{
}
    // = default ctor

ACE_RMS_Dyn_Scheduler_Strategy::~ACE_RMS_Dyn_Scheduler_Strategy ()
{
}
    // = virtual dtor

int
ACE_RMS_Dyn_Scheduler_Strategy::dynamic_subpriority_comp
  (const ACE_Scheduler::Task_Entry &first_entry, 
   const ACE_Scheduler::Task_Entry &second_entry)
{
  if (first_entry.rt_info_->criticality >= RtecScheduler::HIGH_CRITICALITY)
  {
    // for HIGH_CRITICALITY and VERY_HIGH_CRITICALITY, 
    // order second by period (ascending)
    if (first_entry.rt_info_->period < second_entry.rt_info_->period)
    {
      return -1;
    }
    else if (first_entry.rt_info_->period > second_entry.rt_info_->period)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    // for VERY_LOW_CRITICALITY, LOW_CRITICALITY and MEDIUM_CRITICALITY,
    // order second by laxity (ascending)

    RtecScheduler::Time laxity1 = ((RtecScheduler::Time) first_entry.rt_info_->period) -
                                  first_entry.rt_info_->worst_case_execution_time;

    RtecScheduler::Time laxity2 = ((RtecScheduler::Time) second_entry.rt_info_->period) -
                                  second_entry.rt_info_->worst_case_execution_time;

    if (laxity1 < laxity2)
    {
      return -1;
    }
    else if (laxity1 > laxity2)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}
    // = comparison of two task entries within the very high and high 
    //   criticality sets by minimum period (RMS) or of two task entries
    //   within the medium, low, and very low criticality sets by minimum 
    //   laxity: returns -1 if the first Task_Entry is greater in the order,
    //   0 if they're equivalent, or 1 if the second Task_Entry is greater 
    //   in the order.

int 
ACE_RMS_Dyn_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_RMS_Dyn_Scheduler_Strategy::instance ()->
           sort_comp (** ACE_static_cast (ACE_Scheduler::Task_Entry **, arg1), 
                      ** ACE_static_cast (ACE_Scheduler::Task_Entry **, arg2));
}
  // comparison function to pass to qsort


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<ACE_Scheduler::Task_Entry *>;
template class ACE_Unbounded_Set<ACE_Scheduler::Task_Entry *>;
template class ACE_Unbounded_Set_Iterator<ACE_Scheduler::Task_Entry *>;
template class ACE_Strategy_Scheduler_Factory<ACE_MUF_Scheduler_Strategy>;
template class ACE_Strategy_Scheduler_Factory<ACE_RMS_Scheduler_Strategy>;
template class ACE_Strategy_Scheduler_Factory<ACE_MLF_Scheduler_Strategy>;
template class ACE_Strategy_Scheduler_Factory<ACE_EDF_Scheduler_Strategy>;
template class ACE_Strategy_Scheduler_Factory<ACE_RMS_Dyn_Scheduler_Strategy>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


// EOF
