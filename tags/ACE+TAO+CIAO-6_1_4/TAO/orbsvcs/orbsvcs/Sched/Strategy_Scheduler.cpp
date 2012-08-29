//=============================================================================
/**
 *  @file    Strategy_Scheduler.cpp
 *
 *  $Id$
 *
 *  @author Chris Gill
 */
//=============================================================================


#include "orbsvcs/Sched/Strategy_Scheduler.h"
#include "ace/Sched_Params.h"

//////////////////////////////////////////////
// Helper function type definition for sort //
//////////////////////////////////////////////

#if defined (ACE_HAS_WINCE)
typedef int (_cdecl* COMP_FUNC) (const void*, const void*);
#else
// This is awkward, but it makes MSVC++ happy.
extern "C"
{
typedef int (*COMP_FUNC) (const void*, const void*);
}
#endif  // ACE_HAS_WINCE

///////////////////////////////////////////////////
// class ACE_Strategy_Scheduler member functions //
///////////////////////////////////////////////////

// = Constructor.

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_Strategy_Scheduler::ACE_Strategy_Scheduler (ACE_Scheduler_Strategy &strategy)
  : ACE_DynScheduler (),
    strategy_ (strategy)
{
}


// = Virtual destructor.

ACE_Strategy_Scheduler::~ACE_Strategy_Scheduler ()
{
}


// = Sets up the schedule in the order generated
//   by the strategy's comparison operators.

ACE_DynScheduler::status_t
ACE_Strategy_Scheduler::sort_dispatches (Dispatch_Entry **dispatches,
                                         u_int count)
{
  // Sort the entries in order of priority and subpriority.
  strategy_.sort (dispatches, count);

  return ACE_DynScheduler::SUCCEEDED;
}

// = Assigns priorities and sub-priorities to the sorted schedule,
//   according to the strategy's priority comparison operator.

ACE_DynScheduler::status_t
ACE_Strategy_Scheduler::assign_priorities (
  Dispatch_Entry **dispatches,
  u_int count,
  ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set)
{
  // Start with happy status.
  ACE_DynScheduler::status_t status = SUCCEEDED;

  RtecScheduler::Scheduling_Anomaly * anomaly = 0;

  // Start with the highest OS priority in the given range and work downward:
  // if we run out of values to assign, return an error.
  int current_OS_priority = maximum_priority_;

  // Start scheduler priority at 0 (highest priority queue number)
  // NOTE: 0 is highest for priority, lowest for dynamic and static subpriority.
  Preemption_Priority current_scheduler_priority = 0;

  // Value the OS and scheduler priorities in 0th dispatch entry.
  dispatches[0]->OS_priority (current_OS_priority);
  dispatches[0]->priority (current_scheduler_priority);

  // Store the dispatch configuration for the highest priority level.
  Config_Info *config_ptr;
  ACE_NEW_RETURN (config_ptr, Config_Info, ST_VIRTUAL_MEMORY_EXHAUSTED);
  config_ptr->preemption_priority = current_scheduler_priority;
  config_ptr->thread_priority = current_OS_priority;
  config_ptr->dispatching_type = strategy_.dispatch_type (*(dispatches[0]));
  if (config_info_entries_->insert (config_ptr) < 0)
  {
    return ST_VIRTUAL_MEMORY_EXHAUSTED;
  }

  // Traverse ordered dispatch entry array, assigning priority
  // (array is sorted from highest to lowest priority).
  for (u_int i = 1; i < count; ++i)
  {
    switch (strategy_.priority_comp (*(dispatches[i-1]),
                                     *(dispatches[i])))
    {
      case -1:  // The current entry is at lower priority than the previous.
                {
        // Decrease priority by incrementing the current scheduling priority
        // number: 0 is the highest priority number.
        ++current_scheduler_priority;

        // Check for OS priority level boundaries: because OS priority values
        // can run in either increasing or decreasing order, there is no easy,
        // portable way to check other than exact comparison to the bounds
        // that were given in init () or that came from the platform itself.
        if ((current_OS_priority == minimum_priority_) ||
            (current_OS_priority == ACE_Sched_Params::previous_priority (
                                      ACE_SCHED_FIFO,
                                      current_OS_priority,
                                      ACE_SCOPE_PROCESS)))
        {
          // If we have run out of priority levels to assign, indicate
          // this in the return status, unless a more severe problem is
          // already reflected there.  Log an anomaly but keep right on
          // assigning the minimum OS priority in the range to the remaining
          // tasks.
          status = (status == SUCCEEDED)
                   ? ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS
                   : status;

          // Log the anomaly.
          anomaly =
            create_anomaly (ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS);
          if (anomaly)
            {
              anomaly_set.insert (anomaly);
            }
          else
            {
              return ST_VIRTUAL_MEMORY_EXHAUSTED;
            }
        }
        else
        {
          // We're still in range, so decrement the current OS priority level.
          current_OS_priority =
            ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                 current_OS_priority,
                                                 ACE_SCOPE_PROCESS);
        }

        // Store the dispatch configuration for the new priority level.
        ACE_NEW_RETURN(config_ptr, Config_Info, ST_VIRTUAL_MEMORY_EXHAUSTED);
        config_ptr->preemption_priority = current_scheduler_priority;
        config_ptr->thread_priority = current_OS_priority;
        config_ptr->dispatching_type = strategy_.dispatch_type (*(dispatches[i]));
        if (config_info_entries_->insert (config_ptr) < 0)
        {
          return ST_VIRTUAL_MEMORY_EXHAUSTED;
        }

                break;
                }
      case 0:  // Still at the same priority level.

        break;

      default: // Should never reach here: something *bad* has happened.

        ACE_ERROR ((
          LM_ERROR,
          "Priority assignment failure: tasks"
          " \"%s\" and \"%s\" are out of order.\n",
          dispatches [i-1]->task_entry ().rt_info ()->entry_point.in (),
          dispatches [i]->task_entry ().rt_info ()->entry_point.in ()));

          status = ACE_DynScheduler::ST_INVALID_PRIORITY_ORDERING;

          // Log the anomaly.
          anomaly =
            create_anomaly (ST_INVALID_PRIORITY_ORDERING);
          if (anomaly)
            {
              anomaly_set.insert (anomaly);
            }
          else
            {
              return ST_VIRTUAL_MEMORY_EXHAUSTED;
            }
    }

    // Set OS priority of the current dispatch entry.
    dispatches[i]->OS_priority (current_OS_priority);

    // Set scheduler priority of the current dispatch entry.
    dispatches[i]->priority (current_scheduler_priority);
  }

  return status;
}


// = Assigns dynamic and static sub-priorities to the sorted dispatch
//   schedule, according to the strategy's subpriority comparisons.

ACE_DynScheduler::status_t
ACE_Strategy_Scheduler::assign_subpriorities (
  Dispatch_Entry **dispatches,
  u_int count,
  ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set)
{
  ACE_DynScheduler::status_t status = ACE_DynScheduler::SUCCEEDED;
  RtecScheduler::Scheduling_Anomaly * anomaly = 0;

  // Start subpriority levels and element counts at 1, set level values in
  // the first entry, increment the static subpriority level.
  Sub_Priority dynamic_subpriority_level = 0;
  Sub_Priority static_subpriority_level = 0;
  u_int dynamic_subpriority_elements = 1;
  u_int static_subpriority_elements = 1;
  dispatches [0]->dynamic_subpriority (dynamic_subpriority_level);
  dispatches [0]->static_subpriority (static_subpriority_level);

  // Advance the static subpriority level.
  static_subpriority_level++;

  u_int i,j;
  // Traverse ordered dispatch entry array, assigning priority
  // (array is sorted from highest to lowest priority).
  for (i = 1; i < count; ++i)
  {
    switch (strategy_.priority_comp (*(dispatches [i-1]),
                                     *(dispatches [i])))
    {
      case -1:  // The current entry is at lower priority than the previous.
      {
        // Fill in the high to low dynamic subpriority values by subtracting
        // the previously assigned subpriority value of each of element in the
        // current priority level from the value of last subpriority level.
        for (j = 1; j <= dynamic_subpriority_elements; ++j)
        {
          dispatches [i - j]->
                          dynamic_subpriority (dynamic_subpriority_level -
                                   dispatches [i - j]-> dynamic_subpriority ());
        }
        for (j = 1; j <= static_subpriority_elements; ++j)
        {
          dispatches [i - j]->
                          static_subpriority (static_subpriority_level -
                                  dispatches [i - j]-> static_subpriority () - 1);
        }

        // Reset the subpriority counters, set these values in the
        // current entry, and increment the static subpriority counter.
        dynamic_subpriority_elements = 1;
        static_subpriority_elements = 1;
        dynamic_subpriority_level = 0;
        static_subpriority_level = 0;
        dispatches [i]->dynamic_subpriority (dynamic_subpriority_level);
        dispatches [i]->static_subpriority (static_subpriority_level);

        // Advance the static subpriority level.
        static_subpriority_level++;

        break;
      }

      case 0:  // Still at the same priority level.

        // Compare the dynamic subpriorities.
        switch (strategy_.dynamic_subpriority_comp (*(dispatches[i-1]),
                                                    *(dispatches[i])))
        {
          case -1:  // The current entry is at lower dynamic subpriority.

            // Increment the dynamic subpriority level.
            ++dynamic_subpriority_level;

            // Update the static subpriority as well: this avoids problems
            // with non-determinism if due to run-time conditions, two
            // dispatches line up with identical dynamic subpriority that
            // were considered different with respect to the critical instant.
            dispatches [i]->static_subpriority (static_subpriority_level);
            static_subpriority_level++;
            static_subpriority_elements++;

            break;

          case 0:  // Still at the same dynamic subpriority level.
          {
            switch (strategy_.static_subpriority_comp (*(dispatches[i-1]),
                                                       *(dispatches[i])))
            {
              case -1:
              case  0:

                // Assign and then increment the static subpriority: even if
                // still at the same dynamic or static subpriority level as
                // far as the scheduling strategy is concerned, assign a new
                // one anyway, to give a completely deterministic schedule
                // even if the dynamic subpriorities happen to align due to
                // run-time variation.
                dispatches [i]->static_subpriority (static_subpriority_level);
                static_subpriority_level++;
                static_subpriority_elements++;
                break;

              default: // We should never reach here: something *bad* has happened.

                ACE_ERROR ((
                  LM_ERROR,
                  "Static subpriority assignment failure: tasks"
                  " \"%s\" and \"%s\" are out of order.\n",
                  dispatches [i-1]->task_entry ().rt_info ()->entry_point.in (),
                  dispatches [i]->task_entry ().rt_info ()->entry_point.in ()));

                  status = ST_INVALID_PRIORITY_ORDERING;

                  // Log the anomaly.
                  anomaly =
                    create_anomaly (ST_INVALID_PRIORITY_ORDERING);
                  if (anomaly)
                    {
                      anomaly_set.insert (anomaly);
                    }
                  else
                    {
                      return ST_VIRTUAL_MEMORY_EXHAUSTED;
                    }
            }
            break;
          }

          default: // We should never reach here: something *bad* has happened.

            ACE_ERROR ((
              LM_ERROR,
              "Dynamic subpriority assignment failure: tasks"
              " \"%s\" and \"%s\" are out of order.\n",
              dispatches [i-1]->task_entry ().rt_info ()->entry_point.in (),
              dispatches [i]->task_entry ().rt_info ()->entry_point.in ()));

              status = ACE_DynScheduler::ST_INVALID_PRIORITY_ORDERING;

              // Log the anomaly.
              anomaly =
                create_anomaly (ST_INVALID_PRIORITY_ORDERING);
              if (anomaly)
                {
                  anomaly_set.insert (anomaly);
                }
              else
                {
                  return ST_VIRTUAL_MEMORY_EXHAUSTED;
                }
        }

        dispatches [i]->dynamic_subpriority (dynamic_subpriority_level);
        dynamic_subpriority_elements++;

        break;

      default: // We should never reach here: something *bad* has happened.

        ACE_ERROR ((
          LM_ERROR,
          "Priority assignment failure: tasks"
          " \"%s\" and \"%s\" are out of order.\n",
          dispatches [i-1]->task_entry ().rt_info ()->entry_point.in (),
          dispatches [i]->task_entry ().rt_info ()->entry_point.in ()));

        status = ACE_DynScheduler::ST_INVALID_PRIORITY_ORDERING;

        // Log the anomaly.
        anomaly =
          create_anomaly (ST_INVALID_PRIORITY_ORDERING);
        if (anomaly)
          {
            anomaly_set.insert (anomaly);
          }
        else
          {
            return ST_VIRTUAL_MEMORY_EXHAUSTED;
          }
    }
  }

  // Fill in the high to low subpriority values for the last priority
  // level by subtracting the previously assigned subpriorities from
  // the total number of subpriorities.
  for (j = 1; j <= dynamic_subpriority_elements; ++j)
  {
    dispatches [i - j]->
      dynamic_subpriority (dynamic_subpriority_level -
                           dispatches [i - j]->dynamic_subpriority ());
  }
  for (j = 1; j <= static_subpriority_elements; ++j)
  {
    dispatches [i - j]->
      static_subpriority (static_subpriority_level -
                          dispatches [i - j]->static_subpriority () - 1);
  }

  return status;
}

// = Determine the minimum critical priority number.

ACE_DynScheduler::Preemption_Priority
ACE_Strategy_Scheduler::minimum_critical_priority ()
{
  return strategy_.minimum_critical_priority ();
}


// = Schedules a dispatch entry into the timeline being created.

ACE_DynScheduler::status_t
ACE_Strategy_Scheduler::schedule_timeline_entry (
  Dispatch_Entry &dispatch_entry,
  ACE_Unbounded_Queue <Dispatch_Entry *> &reschedule_queue)
{
  status_t status = SUCCEEDED;

  // Timeline entries cover the execution time of the dispatch.
  Time remaining_time =
    dispatch_entry.task_entry().rt_info ()->worst_case_execution_time;

  // Initialize last stop time to arrival time of the dispatch.
  Time last_stop = dispatch_entry.arrival ();

  TimeLine_Entry *last_entry = 0;
  TimeLine_Entry *current_entry = 0;
  ACE_Ordered_MultiSet_Iterator <TimeLine_Entry_Link> iter (*timeline_);
  for (iter.first (); (remaining_time > 0U) && (iter.done () == 0);
       iter.advance ())
  {
    TimeLine_Entry_Link *link;
    if ((iter.next (link) == 0) || (! link))
    {
      return ST_BAD_INTERNAL_POINTER;
    }

    // For each entry already in the timeline that is the first one for a
    // dispatch, and has lower dynamic subpriority and does not have greater
    // static priority, and starts in the period in which the new entry would
    // execute, advance the iterator to the next timeline entry
    // having a different dispatch entry (if there is such), add its dispatch
    // entry to the reschedule set, remove all TimeLine_Entry_Links that
    // correspond to that dispatch entry, and delete all its TimeLine_Entry
    // objects as well.  NOTE: 0 is highest priority, 1 next, etc.
    while ((iter.done () == 0) &&
           (link->entry ().start() < last_stop + remaining_time) &&
           (link->entry ().start() >= last_stop) &&
           (link->entry ().prev () == 0) &&
           (link->entry ().dispatch_entry().priority () >=
            dispatch_entry.priority ()) &&
           (strategy_.dynamic_subpriority (dispatch_entry, link->entry ().start ()) >
            strategy_.dynamic_subpriority (link->entry ().dispatch_entry (),
                                           link->entry ().start ())))
    {
      // Point to the dispatch entry whose timeline entries will be removed and
      // rescheduled, and to the timeline entry heading the bilinked list of
      // timeline entries to be removed.
      Dispatch_Entry *removed_dispatch_entry
        = &(link->entry ().dispatch_entry());
      TimeLine_Entry *remove_entry = & (link->entry ());

      // Put the dispatch entry into the set of entries that will be
      // rescheduled at the end of this method (tail recursively).
      reschedule_queue.enqueue_tail (removed_dispatch_entry);

      // Advance the iterator to the next timeline entry (if there is one)
      // that is not for the dispatch entry being removed.
      while (iter.done () == 0)
      {
        // Point to the current link.
        if ((iter.next (link) == 0) || (! link))
        {
          return ST_BAD_INTERNAL_POINTER;
        }

        // Advance until a different dispatch entry is found,
        // or we run off the end of the timeline.
        if (&(link->entry ().dispatch_entry ()) ==
            removed_dispatch_entry)
        {
          iter.advance ();
        }
        else
        {
          break;
        }
      }

      // Remove entries corresponding to the rescheduled
      // dispatch from the timeline and destroy them.
      TimeLine_Entry *next_remove_entry = 0;
      while (remove_entry)
      {
        next_remove_entry = remove_entry->next ();

        timeline_->remove (TimeLine_Entry_Link (*remove_entry));
        delete remove_entry;

        remove_entry = next_remove_entry;
      }
    }

    // Exit the outer loop if there are no more entries in the timeline.
    if (iter.done () != 0)
    {
      break;
    }

    // If there's room, schedule a new timeline entry for the dispatch.
    if (link->entry ().start() > last_stop)
    {
      ACE_NEW_RETURN (
        current_entry,
        TimeLine_Entry (
          dispatch_entry,
          last_stop,
          (((remaining_time + last_stop) < link->entry ().start())
             ? (remaining_time + last_stop) : link->entry ().start()),
          dispatch_entry.arrival (),
          dispatch_entry.deadline (),
          (TimeLine_Entry *) 0, last_entry),
        ST_VIRTUAL_MEMORY_EXHAUSTED);

      // Patch up the pointers within the list of entries for this dispatch.
      if (last_entry)
      {
        last_entry->next (current_entry);
      }
      last_entry = current_entry;

      timeline_->insert(TimeLine_Entry_Link(*current_entry));

      // Update the remaining time and last stop values.
      remaining_time -= ((remaining_time < (link->entry ().start() - last_stop))
                          ? remaining_time : (link->entry ().start() - last_stop));
    }

    // Update the last stop time.
    if (last_stop < link->entry ().stop ())
    {
      last_stop = link->entry ().stop ();
    }
  }

  // If there is still dispatch time remaining, and we've
  // reached the end of the list, insert what's left.
  if (remaining_time > 0U)
  {
    ACE_NEW_RETURN (
      current_entry,
      TimeLine_Entry (
        dispatch_entry,
        last_stop,
        remaining_time + last_stop,
        dispatch_entry.arrival (),
        dispatch_entry.deadline (),
        0, last_entry),
      ST_VIRTUAL_MEMORY_EXHAUSTED);

    // Patch up the pointers within the list of entries for this dispatch.
    if (last_entry)
    {
      last_entry->next (current_entry);
    }

    timeline_->insert(TimeLine_Entry_Link(*current_entry));
  }

  return status;
}



////////////////////////////////////////////////////////////////////
// class template ACE_Strategy_Scheduler_Factory member functions //
////////////////////////////////////////////////////////////////////

// = Constructs and returns a scheduler strategized with
//   an instance of the the parameterized strategy type.

template <class STRATEGY> ACE_Strategy_Scheduler *
ACE_Strategy_Scheduler_Factory<STRATEGY>::create (RtecScheduler::Preemption_Priority_t minimum_critical_priority)
{
  ACE_Strategy_Scheduler *the_scheduler = 0;
  STRATEGY *the_strategy;

  ACE_NEW_RETURN(the_strategy, STRATEGY(minimum_critical_priority), 0);

  ACE_NEW_RETURN (the_scheduler, ACE_Strategy_Scheduler (*the_strategy), 0);

  return the_scheduler;
}



/////////////////////////////////////////////////////////////////
// abstract base class ACE_Scheduler_Strategy member functions //
/////////////////////////////////////////////////////////////////

// = Constructor.

ACE_Scheduler_Strategy::ACE_Scheduler_Strategy (
  ACE_DynScheduler::Preemption_Priority minimum_critical_priority)
  : minimum_critical_priority_ (minimum_critical_priority)
{
}

ACE_Scheduler_Strategy::~ACE_Scheduler_Strategy (void)
{
}

// = Compares two dispatch entries using the specific priority, dynamic
//   subpriority, and static subpriority method definitions provided by
//   the derived strategy class to produce the strategy specific sort
//   ordering: returns -1 if the first Dispatch_Entry is greater in the order,
//   0 if they are equivalent, or 1 if the second Dispatch_Entry is greater in
//   the order.

int
ACE_Scheduler_Strategy::sort_comp
  (const Dispatch_Entry &first_entry,
   const Dispatch_Entry &second_entry)
{
  // Order first by the priority ordering.
  int result = priority_comp (first_entry, second_entry);

  // Within same priority, order by dynamic subpriority.
  if (result == 0)
  {
    result = dynamic_subpriority_comp (first_entry, second_entry);
  }

  // If same dynamic subpriority, order by static subpriority.
  if (result == 0)
  {
    result = static_subpriority_comp (first_entry, second_entry);
  }

  return result;
}

// = Provides a lowest level ordering based first on importance (descending),
//   and then on the dependency topological sort finishing time (ascending).

int
ACE_Scheduler_Strategy::static_subpriority_comp (
  const Dispatch_Entry &first_entry,
  const Dispatch_Entry &second_entry)
{
  // Order first by importance assigned to underlying RT_Info (descending).
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
    // Order last by the topological sort finishing time (ascending).
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
      return 0;
    }
  }
}


// = Base class supplies default behavior: returns 0
//   for minimum critical priority number.

ACE_DynScheduler::Preemption_Priority
ACE_Scheduler_Strategy::minimum_critical_priority ()
{
  return 0;
}



/////////////////////////////////////////////////////////////////////////
// class ACE_MUF_Scheduler_Strategy static data member initializations //
/////////////////////////////////////////////////////////////////////////

ACE_MUF_Scheduler_Strategy * ACE_MUF_Scheduler_Strategy::instance_ = 0;

///////////////////////////////////////////////////////
// class ACE_MUF_Scheduler_Strategy member functions //
///////////////////////////////////////////////////////

// = Returns an instance of the strategy.

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

// = Compares two dispatch entries by maximum criticality: returns -1 if the
//   first Dispatch_Entry is greater in the order, 0 if they're equivalent, or
//   1 if the second Dispatch_Entry is greater in the order.

int
ACE_MUF_Scheduler_Strategy::priority_comp (const Dispatch_Entry &first_entry,
                                           const Dispatch_Entry &second_entry)
{
  // Order by criticality (descending).
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
    return 0;  // Same priority level.
  }
}


// = Sorts the dispatch entry pointer array in descending urgency order.

void
ACE_MUF_Scheduler_Strategy::sort (Dispatch_Entry **dispatch_entries, u_int size)
{
  ACE_OS::qsort ((void *) dispatch_entries,
                 size,
                 sizeof (Dispatch_Entry *),
                 (COMP_FUNC) ACE_MUF_Scheduler_Strategy::sort_function);
}


// = Default constructor.

ACE_MUF_Scheduler_Strategy::ACE_MUF_Scheduler_Strategy (
  ACE_DynScheduler::Preemption_Priority minimum_critical_priority)
  :ACE_Scheduler_Strategy (minimum_critical_priority)
{
}


// = Virtual destructor.

ACE_MUF_Scheduler_Strategy::~ACE_MUF_Scheduler_Strategy ()
{
}


// = Returns a dynamic subpriority value for the given entry and the
//   current time: if the operation has non-negative laxity, then the
//   value is positive, and a lower laxity gives a higher dynamic
//   subpriority; if the operation has negative laxity, the value
//   is the (negative) laxity value.

long
ACE_MUF_Scheduler_Strategy::dynamic_subpriority (Dispatch_Entry &entry,
                                                 RtecScheduler::Time current_time)
{
  long laxity =
    ACE_U64_TO_U32 (entry.deadline () - current_time -
      entry.task_entry ().rt_info ()->worst_case_execution_time);

  return (laxity > 0) ? LONG_MAX - laxity : laxity;
}


// = Orders two dispatch entries by ascending laxity: returns -1 if the
// first Dispatch_Entry is greater in the order, 0 if they're equivalent,
// 1 if the second Dispatch_Entry is greater in the order.

int
ACE_MUF_Scheduler_Strategy::dynamic_subpriority_comp
  (const Dispatch_Entry &first_entry,
   const Dispatch_Entry &second_entry)
{
  // Order by descending dynamic priority according to ascending laxity.
  u_long laxity1 =
    ACE_U64_TO_U32 (first_entry.deadline () - first_entry.arrival () -
      first_entry.task_entry ().rt_info ()->worst_case_execution_time);

  u_long laxity2 =
    ACE_U64_TO_U32 (second_entry.deadline () - first_entry.arrival () -
      second_entry.task_entry ().rt_info ()->worst_case_execution_time);

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


// = Comparison function to pass to qsort.

int
ACE_MUF_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_MUF_Scheduler_Strategy::instance ()->
           sort_comp (** static_cast<Dispatch_Entry **> (arg1),
                      ** static_cast<Dispatch_Entry **> (arg2));
}


// = Returns the minimum critical priority number.

ACE_DynScheduler::Preemption_Priority
ACE_MUF_Scheduler_Strategy::minimum_critical_priority ()
{
  return minimum_critical_priority_;
}


// = Provides the dispatching queue type for the given dispatch entry.

ACE_DynScheduler::Dispatching_Type
ACE_MUF_Scheduler_Strategy::dispatch_type (const Dispatch_Entry & /* entry */)
{
  return RtecScheduler::LAXITY_DISPATCHING;
}




/////////////////////////////////////////////////////////////////////////
// class ACE_RMS_Scheduler_Strategy static data member initializations //
/////////////////////////////////////////////////////////////////////////

ACE_RMS_Scheduler_Strategy * ACE_RMS_Scheduler_Strategy::instance_ = 0;

///////////////////////////////////////////////////////
// class ACE_RMS_Scheduler_Strategy member functions //
///////////////////////////////////////////////////////

// = Returns an instance of the strategy.

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

// = Compares two dispatch entries by minimum period: returns -1 if the
//   first Dispatch_Entry is greater in the order, 0 if they're equivalent,
//   or 1 if the second Dispatch_Entry is greater in the order.

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


// = Sorts the dispatch entry pointer array in descending RMS (rate) order.

void
ACE_RMS_Scheduler_Strategy::sort (
  Dispatch_Entry **dispatch_entries_, u_int size)
{
  ACE_OS::qsort ((void *) dispatch_entries_,
                 size,
                 sizeof (Dispatch_Entry *),
                 (COMP_FUNC) ACE_RMS_Scheduler_Strategy::sort_function);
}


// = Default constructor.

ACE_RMS_Scheduler_Strategy::ACE_RMS_Scheduler_Strategy (
  ACE_DynScheduler::Preemption_Priority minimum_critical_priority)
  :ACE_Scheduler_Strategy (minimum_critical_priority)
{
}


// = Virtual destructor.

ACE_RMS_Scheduler_Strategy::~ACE_RMS_Scheduler_Strategy ()
{
}


// = All entries have the same dynamic subpriority value.

long
ACE_RMS_Scheduler_Strategy::dynamic_subpriority (
  Dispatch_Entry & /* entry */,
  RtecScheduler::Time /* current_time */)
{
  return 0;
}


// = All tasks in a given priority level have the same dynamic
//   subpriority under RMS.

int
ACE_RMS_Scheduler_Strategy::dynamic_subpriority_comp
  (const Dispatch_Entry & /* first_entry */,
   const Dispatch_Entry & /* second_entry */)
{
  return 0;
}


// Comparison function to pass to qsort.

int
ACE_RMS_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_RMS_Scheduler_Strategy::instance ()->
           sort_comp (** static_cast<Dispatch_Entry **> (arg1),
                      ** static_cast<Dispatch_Entry **> (arg2));
}


// = Returns minimum critical priority number.

ACE_DynScheduler::Preemption_Priority
ACE_RMS_Scheduler_Strategy::minimum_critical_priority ()
{
  return minimum_critical_priority_;
}


// = Provide the dispatching queue type for the given dispatch entry.

ACE_DynScheduler::Dispatching_Type
ACE_RMS_Scheduler_Strategy::dispatch_type (const Dispatch_Entry & /* entry */)
{
  return RtecScheduler::STATIC_DISPATCHING;
}



/////////////////////////////////////////////////////////////////////////
// class ACE_MLF_Scheduler_Strategy static data member initializations //
/////////////////////////////////////////////////////////////////////////

ACE_MLF_Scheduler_Strategy * ACE_MLF_Scheduler_Strategy::instance_ = 0;

///////////////////////////////////////////////////////
// class ACE_MLF_Scheduler_Strategy member functions //
///////////////////////////////////////////////////////

// = Returns an instance of the strategy.

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


// = Just returns 0, as all dispatch entries are of equivalent
//   static priority under MLF.

int
ACE_MLF_Scheduler_Strategy::priority_comp (const Dispatch_Entry & /* first_entry */,
                                           const Dispatch_Entry & /* second_entry */)
{
  return 0;
}


// = Sorts the dispatch entry pointer array in ascending laxity order.

void
ACE_MLF_Scheduler_Strategy::sort (
  Dispatch_Entry **dispatch_entries_, u_int size)
{
  ACE_OS::qsort ((void *) dispatch_entries_,
                 size,
                 sizeof (Dispatch_Entry *),
                 (COMP_FUNC) ACE_MLF_Scheduler_Strategy::sort_function);
}


// = Default constructor.

ACE_MLF_Scheduler_Strategy::ACE_MLF_Scheduler_Strategy (
  ACE_DynScheduler::Preemption_Priority /* minimum_critical_priority */)
  :ACE_Scheduler_Strategy (0)
{
}


// = Virtual destructor

ACE_MLF_Scheduler_Strategy::~ACE_MLF_Scheduler_Strategy ()
{
}


// = Returns a dynamic subpriority value for the given entry and the
//   current time relative to its arrival.

long
ACE_MLF_Scheduler_Strategy::dynamic_subpriority (Dispatch_Entry &entry,
                                                 RtecScheduler::Time current_time)
{
  long laxity =
    ACE_U64_TO_U32 (entry.deadline () - current_time -
      entry.task_entry ().rt_info ()->worst_case_execution_time);

  return (laxity > 0) ? LONG_MAX - laxity : laxity;
}


// = Orders two dispatch entries by ascending laxity: returns -1 if the
//   first Dispatch_Entry is greater in the order, 0 if they're equivalent,
//   or 1 if the second Dispatch_Entry is greater in the order.

int
ACE_MLF_Scheduler_Strategy::dynamic_subpriority_comp
  (const Dispatch_Entry &first_entry,
   const Dispatch_Entry &second_entry)
{
  // Order by descending dynamic priority according to ascending laxity.
  u_long laxity1 =
    ACE_U64_TO_U32 (first_entry.deadline () - first_entry.arrival () -
      first_entry.task_entry ().rt_info ()->worst_case_execution_time);

  u_long laxity2 =
    ACE_U64_TO_U32 (second_entry.deadline () - first_entry.arrival () -
      second_entry.task_entry ().rt_info ()->worst_case_execution_time);

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


// = Comparison function to pass to qsort.

int
ACE_MLF_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_MLF_Scheduler_Strategy::instance ()->
           sort_comp (** static_cast<Dispatch_Entry **> (arg1),
                      ** static_cast<Dispatch_Entry **> (arg2));
}


// = Provides the dispatching queue type for the given dispatch entry.

ACE_DynScheduler::Dispatching_Type
ACE_MLF_Scheduler_Strategy::dispatch_type (const Dispatch_Entry & /* entry */)
{
  return RtecScheduler::LAXITY_DISPATCHING;
}



/////////////////////////////////////////////////////////////////////////
// class ACE_EDF_Scheduler_Strategy static data member initializations //
/////////////////////////////////////////////////////////////////////////

ACE_EDF_Scheduler_Strategy * ACE_EDF_Scheduler_Strategy::instance_ = 0;

///////////////////////////////////////////////////////
// class ACE_EDF_Scheduler_Strategy member functions //
///////////////////////////////////////////////////////

// = Returns an instance of the strategy.

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

// = Just returns 0, as all dispatch entries are of
//   equivalent static priority under EDF.

int
ACE_EDF_Scheduler_Strategy::priority_comp (const Dispatch_Entry & /* first_entry */,
                                           const Dispatch_Entry & /* second_entry */)
{
  return 0;
}


// = Sort the dispatch entry pointer array in ascending deadline (period) order.

void
ACE_EDF_Scheduler_Strategy::sort (
  Dispatch_Entry **dispatch_entries_, u_int size)
{
  ACE_OS::qsort ((void *) dispatch_entries_,
                 size,
                 sizeof (Dispatch_Entry *),
                 (COMP_FUNC) ACE_EDF_Scheduler_Strategy::sort_function);
}


// = Default constructor.

ACE_EDF_Scheduler_Strategy::ACE_EDF_Scheduler_Strategy (
  ACE_DynScheduler::Preemption_Priority /* minimum_critical_priority */)
  :ACE_Scheduler_Strategy (0)
{
}


// = Virtual destructor.

ACE_EDF_Scheduler_Strategy::~ACE_EDF_Scheduler_Strategy ()
{
}

// = Returns a dynamic subpriority value for the given entry and the
//   current time relative to its arrival.

long
ACE_EDF_Scheduler_Strategy::dynamic_subpriority (Dispatch_Entry &entry,
                                                 RtecScheduler::Time current_time)
{
  long time_to_deadline =
      ACE_U64_TO_U32 (entry.deadline () - current_time);

  return (time_to_deadline > 0)
         ? LONG_MAX - time_to_deadline : time_to_deadline;
}


// = Orders two dispatch entries by ascending time to deadline: returns -1 if
//   the first Dispatch_Entry is greater in the order, 0 if they're equivalent,
//   or 1 if the second Dispatch_Entry is greater in the order.

int
ACE_EDF_Scheduler_Strategy::dynamic_subpriority_comp
  (const Dispatch_Entry &first_entry,
   const Dispatch_Entry &second_entry)
{
  // Order by dispatchable interval (ascending).
  if (first_entry.deadline () - first_entry.arrival () <
      second_entry.deadline () - first_entry.arrival ())
  {
    return -1;
  }
  else if (first_entry.deadline () - first_entry.arrival () >
           second_entry.deadline () - first_entry.arrival ())
  {
    return 1;
  }
  else
  {
    return 0;
  }
}


// = Comparison function to pass to qsort.

int
ACE_EDF_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_EDF_Scheduler_Strategy::instance ()->
           sort_comp (** static_cast<Dispatch_Entry **> (arg1),
                      ** static_cast<Dispatch_Entry **> (arg2));
}

// = Provides the dispatching queue type for the given dispatch entry.

ACE_DynScheduler::Dispatching_Type
ACE_EDF_Scheduler_Strategy::dispatch_type (const Dispatch_Entry & /* entry */)
{
  return RtecScheduler::DEADLINE_DISPATCHING;
}


//////////////////////////////////////////////
// class ACE_Criticality_Scheduler_Strategy //
//   static data member initializations     //
//////////////////////////////////////////////

ACE_Criticality_Scheduler_Strategy *
ACE_Criticality_Scheduler_Strategy::instance_ = 0;

///////////////////////////////////////////////////////////////
// class ACE_Criticality_Scheduler_Strategy member functions //
///////////////////////////////////////////////////////////////

// = Returns an instance of the strategy.

ACE_Criticality_Scheduler_Strategy *
ACE_Criticality_Scheduler_Strategy::instance ()
{
  if (0 == ACE_Criticality_Scheduler_Strategy::instance_)
  {
    ACE_NEW_RETURN (ACE_Criticality_Scheduler_Strategy::instance_,
                    ACE_Criticality_Scheduler_Strategy, 0);
  }

  return ACE_Criticality_Scheduler_Strategy::instance_;
}

// = Compares two dispatch entries by minimum period: returns -1 if the
//   first Dispatch_Entry is greater in the order, 0 if they're equivalent,
//   or 1 if the second Dispatch_Entry is greater in the order.

int
ACE_Criticality_Scheduler_Strategy::priority_comp (
    const Dispatch_Entry &first_entry,
    const Dispatch_Entry &second_entry)
{
  // Order by criticality (descending).
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
    return 0;  // Same priority level.
  }
}

// = Sorts the dispatch entry pointer array in descending criticality order.

void
ACE_Criticality_Scheduler_Strategy::sort (
  Dispatch_Entry **dispatch_entries_, u_int size)
{
  ACE_OS::qsort ((void *) dispatch_entries_,
                 size,
                 sizeof (Dispatch_Entry *),
                 (COMP_FUNC) ACE_Criticality_Scheduler_Strategy::sort_function);
}


// = Default constructor.

ACE_Criticality_Scheduler_Strategy::ACE_Criticality_Scheduler_Strategy (
  ACE_DynScheduler::Preemption_Priority minimum_critical_priority)
  :ACE_Scheduler_Strategy (minimum_critical_priority)
{
}


// = Virtual destructor.

ACE_Criticality_Scheduler_Strategy::~ACE_Criticality_Scheduler_Strategy ()
{
}

// = All entries have the same dynamic subpriority value.

long
ACE_Criticality_Scheduler_Strategy::dynamic_subpriority (
  Dispatch_Entry & /* entry */,
  RtecScheduler::Time /* current_time */)
{
  return 0;
}


// = All tasks in a given priority level have the same dynamic
//   subpriority under this strategy.

int
ACE_Criticality_Scheduler_Strategy::dynamic_subpriority_comp
  (const Dispatch_Entry & /* first_entry */,
   const Dispatch_Entry & /* second_entry */)
{
  return 0;
}


// = Comparison function to pass to qsort.

int
ACE_Criticality_Scheduler_Strategy::sort_function (void *arg1, void *arg2)
{
  return ACE_Criticality_Scheduler_Strategy::instance ()->
           sort_comp (** static_cast<Dispatch_Entry **> (arg1),
                      ** static_cast<Dispatch_Entry **> (arg2));
}


// = Returns minimum critical priority number.

ACE_DynScheduler::Preemption_Priority
ACE_Criticality_Scheduler_Strategy::minimum_critical_priority ()
{
  return minimum_critical_priority_;
}

// = Provides the dispatching queue type for the given dispatch entry.

ACE_DynScheduler::Dispatching_Type
ACE_Criticality_Scheduler_Strategy::dispatch_type (
  const Dispatch_Entry & /* entry */)
{
  return RtecScheduler::STATIC_DISPATCHING;
}

TAO_END_VERSIONED_NAMESPACE_DECL
