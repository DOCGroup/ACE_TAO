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
  // = ctor


ACE_Strategy_Scheduler::~ACE_Strategy_Scheduler ()
{
}
  // = virtual dtor


ACE_Scheduler::status_t
ACE_Strategy_Scheduler::sort_dispatches (Dispatch_Entry **dispatches, 
                                         u_int count)
{
  // sort the entries in order of priority and subpriority
  strategy_.sort (dispatches, count);

  return ACE_Scheduler::SUCCEEDED;
}
  // = sets up the schedule in the order generated 
  //   by the strategy's comparison operators

ACE_Scheduler::status_t
ACE_Strategy_Scheduler::assign_priorities (Dispatch_Entry **dispatches, 
                                           u_int count)
{
  // start with happy status
  ACE_Scheduler::status_t status = ACE_Scheduler::SUCCEEDED;

  // start with the highest OS priority in the given range and work downward:
  // if we run out of values to assign, return an error.
  int current_OS_priority = maximum_priority_;

  // start scheduler priority at 0 (highest priority queue number)
  // NOTE: 0 is highest for priority, lowest for dynamic and static subpriority
  Preemption_Priority current_scheduler_priority = 0;

  // value the OS and scheduler priorities in 0th dispatch entry
  dispatches[0]->OS_priority (current_OS_priority);
  dispatches[0]->priority (current_scheduler_priority);

  // traverse ordered dispatch entry array, assigning priority
  // (array is sorted from highest to lowest priority)
  for (u_int i = 1; i < count; ++i)
  {
    switch (strategy_.priority_comp (*(dispatches[i-1]), 
                                     *(dispatches[i])))
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
          // if we have run out of priority levels to assign, indicate
          // this in the return status, but keep right on assigning the
          // minimum OS priority in the range to the remaining tasks.
          status = ACE_Scheduler::ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS;
        }
        else
        {
          // we're still in range, so decrement the current OS priority level
          current_OS_priority = 
            ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                 current_OS_priority,
                                                 ACE_SCOPE_PROCESS);
        }

		break;
		}
      case 0:  // still at the same priority level

        break;

      default: // should never reach here: something *bad* has happened

        ACE_ERROR_RETURN ((
          LM_ERROR, 
          "Priority assignment failure: tasks"
          " \"%s\" and \"%s\" are out of order.\n", 
          dispatches [i-1]->task_entry ().rt_info ()->entry_point,
          dispatches [i]->task_entry ().rt_info ()->entry_point), 
          ACE_Scheduler::ST_INVALID_PRIORITY_ORDERING);
    }

    // set OS priority of the current dispatch entry
    dispatches[i]->OS_priority (current_OS_priority);

    // set scheduler priority of the current dispatch entry
    dispatches[i]->priority (current_scheduler_priority);
  }
  
  return status;  
}
  // = assigns priorities and sub-priorities to the sorted schedule,   
  //   according to the strategy's priority comparison operator.

ACE_Scheduler::status_t
ACE_Strategy_Scheduler::assign_subpriorities (Dispatch_Entry **dispatches, 
                                              u_int count)
{
  // start both subpriority counts at 0, set these values in 
  // the first entry, and increment the static subpriority count
  Sub_Priority dynamic_subpriorities = 0;
  Sub_Priority static_subpriorities = 0;
  dispatches[0]->dynamic_subpriority (dynamic_subpriorities);
  dispatches[0]->static_subpriority (static_subpriorities++);


  // traverse ordered dispatch entry array, assigning priority
  // (array is sorted from highest to lowest priority)
  for (u_int i = 1; i < count; ++i)
  {
    switch (strategy_.priority_comp (*(dispatches[i-1]), 
                                     *(dispatches[i])))
    {
      case -1:  // the current entry is at lower priority than the previous
		{ 
        // fill in the high to low subpriority values by subtracting the 
        // previously assigned subpriorities from the total number of 
        // subpriorities
        int j;
        for (j = 1; j <= dynamic_subpriorities; ++j)
        {
          dispatches[i - j]->
			  dynamic_subpriority (dynamic_subpriorities -
                                   dispatches[i - j]->
								     dynamic_subpriority ());
        }
        for (j = 1; j <= static_subpriorities; ++j)
        {
          dispatches[i - j]->
			  static_subpriority (static_subpriorities -
                                  dispatches[i - j]->
								    static_subpriority ());
        }

        // reset the subpriority counters, set these values in the
        // current entry, and increment the static subpriority counter
        dynamic_subpriorities = 0;
        static_subpriorities = 0;
        dispatches[i]->dynamic_subpriority (dynamic_subpriorities);
        dispatches[i]->static_subpriority (static_subpriorities++);

        break;
		}

      case 0:  // still at the same priority level 

        // compare the dynamic subpriorities
        switch (strategy_.dynamic_subpriority_comp (*(dispatches[i-1]),
                                                    *(dispatches[i])))
        {
          case -1:  // the current entry is at lower dynamic subpriority
          { 
            // increment dynamic subpriority counter
            ++dynamic_subpriorities;

            // fill in the high to low static subpriority values by
            // subtracting the previously assigned subpriorities from
            // the total number of subpriorities
            for (int j = 1; j <= static_subpriorities; ++j)
            {
              dispatches[i - j]->
				  static_subpriority (static_subpriorities -
                                      dispatches[i - j]->
									    static_subpriority ());
            }

            // reset the static subpriority counter, set this value in the
            // current entry, and increment the static subpriority counter
            static_subpriorities = 0;
            dispatches[i]->static_subpriority (static_subpriorities++);

            break;
          }

          case 0:  // still at the same dynamic subpriority level 

			 {
            switch (strategy_.static_subpriority_comp (*(dispatches[i-1]),
                                                       *(dispatches[i])))
            {
              case -1:    
              case  0:    

                // assign and then increment the static subpriority: even if
                // still at the same static subpriority level as far as the
                // scheduling strategy is concerned, assign a new one 
                // anyway, to give a completely deterministic schedule
                dispatches[i]->static_subpriority (static_subpriorities++);
                break;

              default: // should never reach here: something *bad* has happened

                ACE_ERROR_RETURN ((
                  LM_ERROR, 
                  "Static subpriority assignment failure: tasks"
                  " \"%s\" and \"%s\" are out of order.\n", 
                  dispatches[i-1]->task_entry ().rt_info ()->entry_point,
                  dispatches[i]->task_entry ().rt_info ()->entry_point), 
                  ACE_Scheduler::ST_INVALID_PRIORITY_ORDERING);
            }

            break;
			 }
          
          default: // should never reach here: something *bad* has happened

            ACE_ERROR_RETURN ((
              LM_ERROR, 
              "Dynamic subpriority assignment failure: tasks"
              " \"%s\" and \"%s\" are out of order.\n", 
              dispatches[i-1]->task_entry ().rt_info ()->entry_point,
              dispatches[i]->task_entry ().rt_info ()->entry_point), 
              ACE_Scheduler::ST_INVALID_PRIORITY_ORDERING);
        }

        dispatches[i]->dynamic_subpriority (dynamic_subpriorities);
        break;

      default: // should never reach here: something *bad* has happened

        ACE_ERROR_RETURN ((
          LM_ERROR, 
          "Priority assignment failure: tasks"
          " \"%s\" and \"%s\" are out of order.\n", 
          dispatches[i-1]->task_entry ().rt_info ()->entry_point,
          dispatches[i]->task_entry ().rt_info ()->entry_point), 
          ACE_Scheduler::ST_INVALID_PRIORITY_ORDERING);
    }
  }

  return ACE_Scheduler::SUCCEEDED;
}


ACE_Scheduler::Preemption_Priority 
ACE_Strategy_Scheduler::minimum_critical_priority ()
{
  return strategy_.minimum_critical_priority ();
}
  // = determine the minimum critical priority number


////////////////////////////////////////////////////////////////////
// class template ACE_Strategy_Scheduler_Factory member functions //
////////////////////////////////////////////////////////////////////

template <class STRATEGY>
ACE_Strategy_Scheduler * 
ACE_Strategy_Scheduler_Factory<STRATEGY>::create (ACE_Scheduler::Preemption_Priority minimum_critical_priority)
{
  ACE_Strategy_Scheduler *the_scheduler = 0;
  STRATEGY *the_strategy;

  ACE_NEW_RETURN(the_strategy, STRATEGY(minimum_critical_priority), 0);

  ACE_NEW_RETURN (the_scheduler, ACE_Strategy_Scheduler (*the_strategy), 0);
  
  return the_scheduler;
};
  // construct and return a scheduler strategized with
  // an instance of the the parameterized strategy type



/////////////////////////////////////////////////////////////////
// abstract base class ACE_Scheduler_Strategy member functions //
/////////////////////////////////////////////////////////////////


ACE_Scheduler_Strategy::ACE_Scheduler_Strategy (
  ACE_Scheduler::Preemption_Priority minimum_critical_priority)
  : minimum_critical_priority_ (minimum_critical_priority)
{
}
  // ctor

int 
ACE_Scheduler_Strategy::sort_comp
  (const Dispatch_Entry &first_entry, 
   const Dispatch_Entry &second_entry)
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
  // = comparison of two dispatch entries using the specific priority, dynamic
  //   subpriority, and static subpriority method definitions provided by 
  //   the derived strategy class to produce the strategy specific sort 
  //   ordering: returns -1 if the first Dispatch_Entry is greater in the order,
  //   0 if they are equivalent, or 1 if the second Dispatch_Entry is greater in
  //   the order


ACE_Scheduler_Strategy::static_subpriority_comp (
  const Dispatch_Entry &first_entry,
  const Dispatch_Entry &second_entry)
{
  // order first by importance assigned to underlying RT_Info (descending)
  if (first_entry.task_entry ().rt_info ()->importance   > 
      second_entry.task_entry ().rt_info ()->importance)
  {
    return -1;
  }
  else if (first_entry.task_entry ().rt_info ()->importance   < 
           second_entry.task_entry ().rt_info ()->importance)
  {
    return 1;
  }
  else
  {
    // order last by the topological sort finishing time (ascending)
    if (first_entry.task_entry ().finished ()   < 
        second_entry.task_entry ().finished ())
    {
      return -1;
    }
    else if (first_entry.task_entry ().finished ()   > 
             second_entry.task_entry ().finished ())
    {
      return 1;
    }
    else
    {
      // should never get here: all entries should be ordered by finishing time
      ACE_ERROR ((LM_ERROR, 
                  "minimal ordering failure for tasks \"%s\" and \"%s\".\n",
                  first_entry.task_entry ().rt_info ()->entry_point, 
                  second_entry.task_entry ().rt_info ()->entry_point));
      return 0;
    }
  }
}

ACE_Scheduler::Preemption_Priority 
ACE_Scheduler_Strategy::minimum_critical_priority ()
{
  return 0;
}
  // = returns 0 for minimum critical priority number


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
ACE_MUF_Scheduler_Strategy::priority_comp (const Dispatch_Entry &first_entry, 
                                           const Dispatch_Entry &second_entry)
{
  // order by criticality (descending)
  if (first_entry.task_entry ().rt_info ()->criticality   > 
      second_entry.task_entry ().rt_info ()->criticality)
  {
    return -1;
  }
  else if (first_entry.task_entry ().rt_info ()->criticality   < 
           second_entry.task_entry ().rt_info ()->criticality)
  {
    return 1;
  }
  else
  {
    return 0;  // same priority level
  }
}
  // = comparison of two dispatch entries by maximum criticality: returns -1 if the
  //   first Dispatch_Entry is greater in the order, 0 if they're equivalent, or
  //   1 if the second Dispatch_Entry is greater in the order.


void 
ACE_MUF_Scheduler_Strategy::sort (Dispatch_Entry **dispatch_entries, u_int size)
{
  ::qsort ((void *) dispatch_entries, 
           size,
           sizeof (Dispatch_Entry *),
           (COMP_FUNC) ACE_MUF_Scheduler_Strategy::sort_function);
}
  // = sort the dispatch entry pointer array in descending urgency order


ACE_MUF_Scheduler_Strategy::ACE_MUF_Scheduler_Strategy (
  ACE_Scheduler::Preemption_Priority minimum_critical_priority) 
  :ACE_Scheduler_Strategy (minimum_critical_priority)
{
}
    // = default ctor

ACE_MUF_Scheduler_Strategy::~ACE_MUF_Scheduler_Strategy ()
{
}
    // = virtual dtor

int 
ACE_MUF_Scheduler_Strategy::dynamic_subpriority_comp
  (const Dispatch_Entry &first_entry, 
   const Dispatch_Entry &second_entry)
{
  // order by descending dynamic priority according to ascending laxity
  u_long laxity1 = 
    first_entry.deadline () - 
    first_entry.task_entry ().rt_info ()->worst_case_execution_time;

  u_long laxity2 =
    second_entry.deadline () -
    second_entry.task_entry ().rt_info ()->worst_case_execution_time;


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
  // = orders of two dispatch entries by ascending laxity: returns -1 if the 
  // first Dispatch_Entry is greater in the order, 0 if they're equivalent, 
  // 1 if the second Dispatch_Entry is greater in the order.


int 
ACE_MUF_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_MUF_Scheduler_Strategy::instance ()->
           sort_comp (** ACE_static_cast (Dispatch_Entry **, arg1), 
                      ** ACE_static_cast (Dispatch_Entry **, arg2));
}
  // comparison function to pass to qsort

ACE_Scheduler::Preemption_Priority 
ACE_MUF_Scheduler_Strategy::minimum_critical_priority ()
{
  return minimum_critical_priority_;
}
  // = returns 0 for minimum critical priority number

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
ACE_RMS_Scheduler_Strategy::priority_comp (const Dispatch_Entry &first_entry, 
                                           const Dispatch_Entry &second_entry)
{
  // compare by decreasing dispatch period
  if ((first_entry.deadline () - first_entry.arrival ())    < 
      (second_entry.deadline () - second_entry.arrival ()))
  {
    return -1;
  }
  else if ((first_entry.deadline () - first_entry.arrival ())    >
           (second_entry.deadline () - second_entry.arrival ()))
  {
    return 1;
  }
  else
  {
    return 0;  // same priority level
  }
}
  // = comparison of two dispatch entries by minimum period: returns -1 if the
  //   first Dispatch_Entry is greater in the order, 0 if they're equivalent,
  //   or 1 if the second Dispatch_Entry is greater in the order.

void 
ACE_RMS_Scheduler_Strategy::sort (
  Dispatch_Entry **dispatch_entries_, u_int size)
{
  ::qsort ((void *) dispatch_entries_, 
           size,
           sizeof (Dispatch_Entry *),
           (COMP_FUNC) ACE_RMS_Scheduler_Strategy::sort_function);
}
  // = sort the dispatch entry pointer array in descending RMS (rate) order

ACE_RMS_Scheduler_Strategy::ACE_RMS_Scheduler_Strategy (
  ACE_Scheduler::Preemption_Priority minimum_critical_priority) 
  :ACE_Scheduler_Strategy (0)
{
}
    // = default ctor

ACE_RMS_Scheduler_Strategy::~ACE_RMS_Scheduler_Strategy ()
{
}
    // = virtual dtor

int 
ACE_RMS_Scheduler_Strategy::dynamic_subpriority_comp
  (const Dispatch_Entry &first_entry, 
   const Dispatch_Entry &second_entry)
{
  return 0;
}
  // = all tasks in a given priority level have the same dynamic
  //   subpriority under RMS


int 
ACE_RMS_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_RMS_Scheduler_Strategy::instance ()->
           sort_comp (** ACE_static_cast (Dispatch_Entry **, arg1), 
                      ** ACE_static_cast (Dispatch_Entry **, arg2));
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
ACE_MLF_Scheduler_Strategy::priority_comp (const Dispatch_Entry &first_entry, 
                                           const Dispatch_Entry &second_entry)
{
  return 0;
}
  // = just returns 0, as all dispatch entries are of equivalent priority under MLF.

void 
ACE_MLF_Scheduler_Strategy::sort (
  Dispatch_Entry **dispatch_entries_, u_int size)
{
  ::qsort ((void *) dispatch_entries_, 
           size,
           sizeof (Dispatch_Entry *),
           (COMP_FUNC) ACE_MLF_Scheduler_Strategy::sort_function);
}
  // = sort the dispatch entry pointer array in ascending laxity order


ACE_MLF_Scheduler_Strategy::ACE_MLF_Scheduler_Strategy (
  ACE_Scheduler::Preemption_Priority minimum_critical_priority) 
  :ACE_Scheduler_Strategy (0)
{
}
    // = default ctor

ACE_MLF_Scheduler_Strategy::~ACE_MLF_Scheduler_Strategy ()
{
}
    // = virtual dtor


int
ACE_MLF_Scheduler_Strategy::dynamic_subpriority_comp
  (const Dispatch_Entry &first_entry, 
   const Dispatch_Entry &second_entry)
{
  // order by laxity (ascending)
  // order by descending dynamic priority according to ascending laxity
  u_long laxity1 = 
    first_entry.deadline () - 
    first_entry.task_entry ().rt_info ()->worst_case_execution_time;

  u_long laxity2 =
    second_entry.deadline () -
    second_entry.task_entry ().rt_info ()->worst_case_execution_time;

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
  // = orders two dispatch entries by ascending laxity: returns -1 if the
  //   first Dispatch_Entry is greater in the order, 0 if they're equivalent,
  //   or 1 if the second Dispatch_Entry is greater in the order.


int 
ACE_MLF_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_MLF_Scheduler_Strategy::instance ()->
           sort_comp (** ACE_static_cast (Dispatch_Entry **, arg1), 
                      ** ACE_static_cast (Dispatch_Entry **, arg2));
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
ACE_EDF_Scheduler_Strategy::priority_comp (const Dispatch_Entry &first_entry, 
                                           const Dispatch_Entry &second_entry)
{
  return 0;
}
  // = just returns 0, as all dispatch entries are of equivalent priority under EDF.

void 
ACE_EDF_Scheduler_Strategy::sort (
  Dispatch_Entry **dispatch_entries_, u_int size)
{
  ::qsort ((void *) dispatch_entries_, 
           size,
           sizeof (Dispatch_Entry *),
           (COMP_FUNC) ACE_EDF_Scheduler_Strategy::sort_function);
}
  // = sort the dispatch entry pointer array in ascending deadline (period) order


ACE_EDF_Scheduler_Strategy::ACE_EDF_Scheduler_Strategy (
  ACE_Scheduler::Preemption_Priority minimum_critical_priority) 
  :ACE_Scheduler_Strategy (0)
{
}
    // = default ctor

ACE_EDF_Scheduler_Strategy::~ACE_EDF_Scheduler_Strategy ()
{
}
    // = virtual dtor

int
ACE_EDF_Scheduler_Strategy::dynamic_subpriority_comp
  (const Dispatch_Entry &first_entry, 
   const Dispatch_Entry &second_entry)
{
  // order by dispatchable interval (ascending)
  if (first_entry.deadline () < second_entry.deadline ())
  {
    return -1;
  }
  else if (first_entry.deadline () > second_entry.deadline ())
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
  // = orders two dispatch entries by ascending time to deadline: returns -1 if
  //   the first Dispatch_Entry is greater in the order, 0 if they're equivalent,
  //   or 1 if the second Dispatch_Entry is greater in the order.


int 
ACE_EDF_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_EDF_Scheduler_Strategy::instance ()->
           sort_comp (** ACE_static_cast (Dispatch_Entry **, arg1), 
                      ** ACE_static_cast (Dispatch_Entry **, arg2));
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
ACE_RMS_Dyn_Scheduler_Strategy::priority_comp (const Dispatch_Entry &first_entry, 
                                               const Dispatch_Entry &second_entry)
{
  if ((first_entry.task_entry ().rt_info ()->criticality >= 
       RtecScheduler::HIGH_CRITICALITY)                    &&
      (second_entry.task_entry ().rt_info ()->criticality >= 
       RtecScheduler::HIGH_CRITICALITY))
  {
    // if they're both in the high criticality bracket, 
    // order by dispatch period as in RMS scheduling
    if ((first_entry.deadline () - first_entry.arrival ())    < 
        (second_entry.deadline () - second_entry.arrival ()))
    {
      return -1;
    }
    else if ((first_entry.deadline () - first_entry.arrival ())    >
             (second_entry.deadline () - second_entry.arrival ()))
    {
      return 1;
    }

    return 0;  // same priority level
  }
  else if ((first_entry.task_entry ().rt_info ()->criticality < 
            RtecScheduler::HIGH_CRITICALITY)                   &&
           (second_entry.task_entry ().rt_info ()->criticality <
            RtecScheduler::HIGH_CRITICALITY))
  {
    // if they're both in the low criticality bracket, they have the same priority
    return 0;
  }

  // they're in different criticality brackets: order by criticality (descending)
  return (first_entry.task_entry ().rt_info ()->criticality   > 
          second_entry.task_entry ().rt_info ()->criticality) 
         ? -1 : 1;
}
  // = comparison of two dispatch entries by maximum criticality: returns -1 
  //   if the first Dispatch_Entry is greater in the order, 0 if they're
  //   equivalent, or 1 if the second Dispatch_Entry is greater in the order.


void 
ACE_RMS_Dyn_Scheduler_Strategy::sort (
  Dispatch_Entry **dispatch_entries_, u_int size)
{
  ::qsort ((void *) dispatch_entries_, 
           size,
           sizeof (Dispatch_Entry *),
           (COMP_FUNC) ACE_RMS_Dyn_Scheduler_Strategy::sort_function);
}
  // = sort the dispatch entry pointer array in descending priority order


ACE_RMS_Dyn_Scheduler_Strategy::ACE_RMS_Dyn_Scheduler_Strategy (
  ACE_Scheduler::Preemption_Priority minimum_critical_priority) 
  :ACE_Scheduler_Strategy (minimum_critical_priority)
{
}
    // = default ctor

ACE_RMS_Dyn_Scheduler_Strategy::~ACE_RMS_Dyn_Scheduler_Strategy ()
{
}
    // = virtual dtor

int
ACE_RMS_Dyn_Scheduler_Strategy::dynamic_subpriority_comp
  (const Dispatch_Entry &first_entry, 
   const Dispatch_Entry &second_entry)
{
  // if either is in the high criticality bracket, we do not
  // distinguish between them on the basis of dynamic subpriority
  if ((first_entry.task_entry ().rt_info ()->criticality >= 
       RtecScheduler::HIGH_CRITICALITY)                    || 
      (second_entry.task_entry ().rt_info ()->criticality >= 
       RtecScheduler::HIGH_CRITICALITY))
  {      
    // for HIGH_CRITICALITY and VERY_HIGH_CRITICALITY, all
    // entries have the same dynamic subpriority as in RMS
    return 0;
  }
  else
  {
    // for VERY_LOW_CRITICALITY, LOW_CRITICALITY and MEDIUM_CRITICALITY,
    // order second by laxity (ascending)
    u_long laxity1 = 
      first_entry.deadline () - 
      first_entry.task_entry ().rt_info ()->worst_case_execution_time;

    u_long laxity2 =
      second_entry.deadline () -
      second_entry.task_entry ().rt_info ()->worst_case_execution_time;    

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
  // = comparison of two dispatch entries within the very high and high 
  //   criticality sets by minimum period (RMS) or of two dispatch entries
  //   within the medium, low, and very low criticality sets by minimum 
  //   laxity: returns -1 if the first Dispatch_Entry is greater in the order,
  //   0 if they're equivalent, or 1 if the second Dispatch_Entry is greater 
  //   in the order.

int 
ACE_RMS_Dyn_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_RMS_Dyn_Scheduler_Strategy::instance ()->
           sort_comp (** ACE_static_cast (Dispatch_Entry **, arg1), 
                      ** ACE_static_cast (Dispatch_Entry **, arg2));
}
  // comparison function to pass to qsort


ACE_Scheduler::Preemption_Priority 
ACE_RMS_Dyn_Scheduler_Strategy::minimum_critical_priority ()
{
  return minimum_critical_priority_;
}
  // = returns 0 for minimum critical priority number



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<Dispatch_Entry *>;
template class ACE_Unbounded_Set<Dispatch_Entry *>;
template class ACE_Unbounded_Set_Iterator<Dispatch_Entry *>;
template class ACE_Strategy_Scheduler_Factory<ACE_MUF_Scheduler_Strategy>;
template class ACE_Strategy_Scheduler_Factory<ACE_RMS_Scheduler_Strategy>;
template class ACE_Strategy_Scheduler_Factory<ACE_MLF_Scheduler_Strategy>;
template class ACE_Strategy_Scheduler_Factory<ACE_EDF_Scheduler_Strategy>;
template class ACE_Strategy_Scheduler_Factory<ACE_RMS_Dyn_Scheduler_Strategy>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


// EOF

