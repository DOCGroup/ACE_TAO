// $Id$

#ifndef METRICS_LOCAL_CACHE_T_CPP
#define METRICS_LOCAL_CACHE_T_CPP

#include "Metrics_LocalCache_T.h"

#if defined VXWORKS
#include <cacheLib.h>
#endif

//////////////////////////////////////
// Class TAO_Metrics_LocalTimeprobe //g
//////////////////////////////////////

// Default constructor.

template <class ACE_LOCK, class ALLOCATOR>
TAO_Metrics_LocalTimeprobe<ACE_LOCK, ALLOCATOR>::
TAO_Metrics_LocalTimeprobe (u_int id,
                            const char *name,
                            u_long size,
                            ALLOCATOR * alloc)
  : ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR> (alloc, id, name, size),
    is_operation_ (0),
    is_hrt_ (0),
    data_set_ (0)
{
}

template <class ACE_LOCK, class ALLOCATOR>
void
TAO_Metrics_LocalTimeprobe<ACE_LOCK, ALLOCATOR>::report_intervals (int report_full_interval_times)
{
   u_long data_set_length = 0;

   // Modified by BRM.  This should also work for ACE_Based_Pointer since the conversion
   // operator should fire.
   //  Metrics::TimeprobeParameter_Set * data_set = data_set_.addr ();
   Metrics::TimeprobeParameter_Set * data_set = data_set_;

   //ACE_Time_Value full_start_time (ACE_Time_Value::zero);
   ACE_hrtime_t full_start_time = 0;
   //ACE_Time_Value full_stop_time (ACE_Time_Value::zero);
   ACE_hrtime_t full_stop_time = 0;

   if (!data_set)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalTimeprobe::report_intervals:"
                  " no data set installed"));
#endif
      return;
   }
   else
   {
      data_set_length = data_set->length ();
   }

   if (this->current_size_ == 0 && this->report_buffer_full_ == 0)
      return;

   u_long valid_intervals = 0;
   int good_interval;

   // Note the case of current_size_ and report_buffer_full_ both being
   // 0 was checked above, which simplifies this code.
   // This logic make no sense to me.  What are you going for here, Chris?
   // If the buffer fills, then report_buffer_full == 1, and current size == 0.
   // We always needs i to start at 0, though?????
   //  u_long i;
   //  i = (report_buffer_full_ == 0) ? 1 : this->current_size_;
   u_long start_evt_ndx = 0;
   u_long full_interval_start_evt_ndx = 0;
   u_long full_interval_start_ndx_set = 0;
   u_long stop_evt_ndx = 0;
   short any_cross_thread_event = 0;
   short is_cross_thread_event = 0;
   do
   {
      // Begin a new interval at each successive start event.
      if (is_event (this->timeprobes ()[start_evt_ndx],
                    ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::WORK_START))
      {
         // If we see the start event, then reset the cross thread indicator
         is_cross_thread_event = 0;

         if (!full_interval_start_ndx_set)
         {
            full_interval_start_ndx_set = 1;
            full_interval_start_evt_ndx = start_evt_ndx;
         }

         // Accumulates elapsed time in the measured interval, taking
         // into account paired suspend and resume markers.
         ACE_Time_Value elapsed_time (ACE_Time_Value::zero);

         // Stores current start time: either from a start or resume event.
         ACE_Time_Value start_time (this->timeprobes ()[start_evt_ndx].time_);

         // Check if this start time is less than the full interval for worse
         // case time.
         if (full_start_time == 0 ||
             this->timeprobes ()[start_evt_ndx].time_ < full_start_time)
         {
            full_start_time = this->timeprobes ()[start_evt_ndx].time_;
         }
         if (full_stop_time == 0 ||
             this->timeprobes ()[start_evt_ndx].time_ > full_stop_time)
         {
            full_stop_time = this->timeprobes ()[start_evt_ndx].time_;
         }

         good_interval = 1;

         // Increment past the start event and start looking for the stop event
         u_long stop_or_suspend_evt_ndx = (start_evt_ndx + 1) % this ->max_size_; // Modulus increment: loops around at the end.
         do
         {
            // If the index is at the end, skip and exit the loop.
            if (stop_or_suspend_evt_ndx == this->current_size_)
            {
               break;
            }

            // This needs to be rethought...  Events that occur across threads are a real thing.
            // So for now, the behavior is to just indicate the events that are cross thread
//            if (this->timeprobes ()[start_evt_ndx].thread_ != this->timeprobes ()[stop_or_suspend_evt_ndx].thread_)
//            {
//               stop_or_suspend_evt_ndx = (stop_or_suspend_evt_ndx + 1) % this ->max_size_; // Modulus increment: loops around at the end.
//               continue;
//            }

            // Check for the stop event
            if (is_event (this->timeprobes ()[stop_or_suspend_evt_ndx],
              ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::WORK_STOP))
            {
               ++valid_intervals;

              if (this->timeprobes ()[start_evt_ndx].thread_ != this->timeprobes ()[stop_or_suspend_evt_ndx].thread_)
              {
                 // If any probe is cross, thread then the interval is cross thread.
                 any_cross_thread_event = 1;

                 is_cross_thread_event = 1;
              }

              // done this way for efficiency: no temporaries constructed
              elapsed_time += this->timeprobes ()[stop_or_suspend_evt_ndx].time_;
              elapsed_time -= start_time;

               // This doesn't make sense.  Full start time better be the result of
               // a start event.
//               if (full_start_time == ACE_Time_Value::zero ||
//                   this->timeprobes ()[j].time_ < full_start_time)
//               {
//                  full_start_time.set (this->timeprobes()[j].time_.sec (),
//                     this->timeprobes()[j].time_.usec());
//               }

              if (full_stop_time == 0 ||
                  this->timeprobes ()[stop_or_suspend_evt_ndx].time_ > full_stop_time)
              {
                 full_stop_time = this->timeprobes()[stop_or_suspend_evt_ndx].time_;
              }

              stop_evt_ndx = stop_or_suspend_evt_ndx;
              // Increase the length of the data set and record
              // the interval info.
              ++data_set_length;
              data_set->length (data_set_length);
              (*(data_set))[data_set_length-1].is_full_interval = 0;
              (*(data_set))[data_set_length-1].probe_id = this->id_;
              (*(data_set))[data_set_length-1].is_operation = this->is_operation_;
              (*(data_set))[data_set_length-1].is_hrt = this->is_hrt_;
              (*(data_set))[data_set_length-1].cross_thread_probe = is_cross_thread_event;
              ORBSVCS_Time::Time_Value_to_TimeT
                  ((*(data_set)) [data_set_length-1].start_event_time,
                   this->timeprobes ()[start_evt_ndx].time_);
              ORBSVCS_Time::Time_Value_to_TimeT
                  ((*(data_set)) [data_set_length-1].stop_event_time,
                   this->timeprobes ()[stop_or_suspend_evt_ndx].time_);
              ORBSVCS_Time::Time_Value_to_TimeT
                 ((*(data_set)) [data_set_length-1].interval,
                 elapsed_time);
#if defined (METRICS_CACHE_DEBUG_OUTPUT_ENABLED)
              ACE_OS::printf (" %9lu\n",
                 (u_long)
                 ACE_U64_TO_U32((*(data_set))
                 [data_set_length-1].interval
                 / 10));
#endif
              break;
           }

            // If it wasn't a stop event, check for a suspend event
#if defined (ACE_HAS_METRICS_BASE_POINTERS)
            else if (this->timeprobes ()[stop_or_suspend_evt_ndx].event_ == (u_long)
                     ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::WORK_SUSPEND)
#else
            else if (this->timeprobes ()[stop_or_suspend_evt_ndx].event_.event_number_ == (u_long)
                     ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::WORK_SUSPEND)
#endif
           {
               u_long resume_evt_ndx = (stop_or_suspend_evt_ndx + 1) % this ->max_size_; // Modulus increment: loops around at the end.
              do
              {
                 // Skip until we reach the next event in the same thread, or if the
                 // index is at the end, skip and exit the loop.
//                  if (this->timeprobes ()[j].thread_ != this->timeprobes ()[k].thread_
//                     || k == this->current_size_)
//                  {
//                     continue;
//                  }

                  // If we saw a suspend event, the next event has to be a resume event
                  if (is_event (this->timeprobes ()[resume_evt_ndx],
                    ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::WORK_RESUME))
                 {
                    // Store the elapsed time between the last
                    // start and the suspend event.
                    elapsed_time +=
                        this->timeprobes ()[stop_or_suspend_evt_ndx].time_ - start_time;

                    // Update the start time to be that of the
                    // resume event.
                     start_time = this->timeprobes ()[resume_evt_ndx].time_;

                     // This code doesn't make sense here.  It always has to hit the stop to
                     // adjust the stop time.  Anything else is a partial and invalid.
//                     if (full_start_time == ACE_Time_Value::zero ||
//                         this->timeprobes ()[resume_evt_ndx].time_ < full_start_time)
//                     {
//                        full_start_time.
//                           set (this->timeprobes()[resume_evt_ndx].time_.sec (),
//                                this->timeprobes()[resume_evt_ndx].time_.usec ());
//                     }
//                     if (full_stop_time == ACE_Time_Value::zero ||
//                         this->timeprobes ()[resume_evt_ndx].time_ > full_stop_time)
//                     {
//                        full_stop_time.
//                           set (this->timeprobes()[resume_evt_ndx].time_.sec (),
//                               this->timeprobes()[resume_evt_ndx].time_.usec ());
//                          }


                    // Keep looking for the end of the interval
                    // after the resume event.
                     stop_or_suspend_evt_ndx = resume_evt_ndx;
                    break;
                 }
                  else if (is_event (this->timeprobes ()[resume_evt_ndx],
                                     ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::WORK_START) ||
                           is_event (this->timeprobes ()[resume_evt_ndx],
                                     ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::WORK_STOP) ||
                           is_event (this->timeprobes ()[resume_evt_ndx],
                                     ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::WORK_SUSPEND))
                 {
                    // Something went wrong during the current interval:
                    // just ignore the previous start time stamp.
                    good_interval = 0;
                    break;
                 }

                  resume_evt_ndx = (resume_evt_ndx + 1) % this ->max_size_; // Modulus increment: loops around at the end.

               } while (resume_evt_ndx != this->current_size_);
            }

            // If we got a start event, make sure that we don't see another start or resume
            // until we see a stop or suspend event
            else if (is_event (this->timeprobes ()[stop_or_suspend_evt_ndx],
                               ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::WORK_START) ||
                     is_event (this->timeprobes ()[stop_or_suspend_evt_ndx],
                               ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::WORK_RESUME))
           {
              // Something went wrong during the current interval:
              // just ignore the previous start time stamp.
              break;
           }

            stop_or_suspend_evt_ndx = (stop_or_suspend_evt_ndx + 1) % this ->max_size_; // Modulus increment: loops around at the end.

            } while (stop_or_suspend_evt_ndx != this->current_size_ && good_interval);

            // If we found the stop of
            start_evt_ndx = stop_or_suspend_evt_ndx;
        }

        start_evt_ndx = (start_evt_ndx + 1) % this ->max_size_; // Modulus increment: loops around at the end.

    } while (start_evt_ndx < this->current_size_);


    if (report_full_interval_times)
    {
       // Increase the length of the data set and record the full interval.
       ++data_set_length;
       data_set->length (data_set_length);
       (*(data_set))[data_set_length-1].is_full_interval = 1;
       (*(data_set))[data_set_length-1].probe_id = this->id_;
       (*(data_set))[data_set_length-1].is_operation = this->is_operation_;
       (*(data_set))[data_set_length-1].is_hrt = this->is_hrt_;
       (*(data_set))[data_set_length-1].cross_thread_probe = any_cross_thread_event;
       ORBSVCS_Time::Time_Value_to_TimeT
          ((*(data_set)) [data_set_length-1].start_event_time,
          this->timeprobes ()[start_evt_ndx].time_);
       ORBSVCS_Time::Time_Value_to_TimeT
          ((*(data_set)) [data_set_length-1].stop_event_time,
          this->timeprobes ()[stop_evt_ndx].time_);
       ORBSVCS_Time::Time_Value_to_TimeT(
          (*(data_set))[data_set_length-1].interval,
          full_stop_time - full_start_time);
    }
}


// Flush the TAO metrics local timeprobe into shared memory.

template <class ACE_LOCK, class ALLOCATOR>
void
TAO_Metrics_LocalTimeprobe<ACE_LOCK, ALLOCATOR>::flush_TAO_Metrics_LocalTimeprobe ()
{
// TBD - 1) replace VXWORKS with a nicer ACE/TAO #define
//       2) provide a general capability to apply an arbitrary
//          functor to the whole data structure framework (e.g., flush).

#if defined VXWORKS
  // TBD - implement this
#endif
}


//////////////////////////////////
// Class TAO_Metrics_LocalCache //
//////////////////////////////////


template <class ACE_LOCK, class ALLOCATOR>
void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::reset_statistics ()
{
  // Call the base class reset method first.
  this->reset_base_statistics ();

  // Iterate over consumer side base map,
  // resetting probes.
  METRICS_BASE_MONITOR_MAP_ITERATOR
    base_iter (this->base_monitor_maps_ [this->consumer_index_]);

  for (TAO_Metrics_Cache_Data_Base * data_base = 0;
       base_iter.done () == 0;
       ++base_iter)
    {
          // Modified by BRM.  This should also work for ACE_Based_Pointer since the conversion
          // operator should fire.
//          data_base = (*base_iter).int_id_.addr ();
          data_base = (*base_iter).int_id_;
      // Modified by BRM.  This should also work for ACE_Based_Pointer since the conversion
      // operator should fire.
//      if (data_base.addr ())
      if (data_base)
      {
         data_base->probe.reset ();
      }
    }


  // Iterate over consumer side monitor map,
  // resetting counts and probes.
  METRICS_MONITOR_MAP_ITERATOR data_iter (this->
                                            monitor_maps_ [this->
                                                             consumer_index_]);
  for (TAO_Metrics_Cache_Data * data = 0;
       data_iter.done () == 0;
       ++data_iter)
    {
          // Modified by BRM.  This should also work for ACE_Based_Pointer since the conversion
          // operator should fire.
//          data = (*data_iter).int_id_.addr ();
          data = (*data_iter).int_id_;
      if (data)
        {
          data->probe.reset ();
          data->missed_deadlines = 0;
          data->made_deadlines = 0;
          data->cancellations = 0;
        }
    }
}


// Flush the TAO local metrics cache into shared memory.

template <class ACE_LOCK, class ALLOCATOR>
void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
flush_TAO_Metrics_LocalCache ()
{
#if defined VXWORKS

  // TBD - fix this: is it complete?

  // TBD - add base metrics map flush

  // TBD - do probes need their own flush method?

  // flush the class
  // cacheFlush (
  //   DATA_CACHE,
  //   this,
  //   sizeof (TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>));

  // flush the monitor data map hash table
  // cacheFlush (DATA_CACHE,
  //             &monitor_maps_ [this->consumer_index_],
  //             sizeof (METRICS_MONITOR_MAP));

  // flush the nodes in the monitor data map hash table
  // and the data stored in the nodes.
  // METRICS_MONITOR_MAP_ITERATOR data_iter (this->
  //        monitor_maps_ [this->
  //                         consumer_index_]);
  // for (;
  //      data_iter.done () == 0;
  //      ++data_iter)
  //   {
  //     METRICS_MONITOR_MAP_ENTRY *entry;
  //     data_iter.next (entry);
  //     if (entry)
  //       {
  //         cacheFlush (DATA_CACHE,
  //         entry,
  //         sizeof (METRICS_MONITOR_MAP_ENTRY));
  //         if (entry->int_id_.addr ())
  //           {
  //           cacheFlush (DATA_CACHE,
  //                       entry->int_id_.addr (),
  //                       sizeof (TAO_Metrics_Cache_Data));
  //           }
  //       }
  //   }

  // flush the base class probe counts
  // cacheFlush (DATA_CACHE,
  //             this->
  //               enqueue_count_ [this->consumer_index_].addr (),
  //             this->probe_set_size_ * sizeof (u_long));

  // cacheFlush (DATA_CACHE,
  //             this->
  //               dequeue_count_ [this->consumer_index_].addr (),
  //             this->probe_set_size_ * sizeof (u_long));

  // flush the base class probe pointer arrays
  // cacheFlush (
  //   DATA_CACHE,
  //   this->enqueue_probes_ [this->consumer_index_],
  //   this->
  //     probe_set_size_
  //   * sizeof (ACE_Metrics_Timeprobe<ACE_LOCK,ALLOCATOR> *));
  // cacheFlush (
  //   DATA_CACHE,
  //   this->dequeue_probes_ [this->consumer_index_],
  //   this->probe_set_size_
  //   * sizeof (ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR> *));

  // flush the base class probes
  // for (u_int i = 0; i < this->probe_set_size_; ++i)
  //   {
  //    cacheFlush (
  //      DATA_CACHE,
  //      this->enqueue_probes_ [this->consumer_index_][i],
  //      sizeof (ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>));
  //    cacheFlush (
  //      DATA_CACHE,
  //      this->dequeue_probes_ [this->consumer_index_][i],
  //      sizeof (ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>));
  //   }
#endif
}

#if !defined (__ACE_INLINE__)
#include "Metrics_LocalCache_T.i"
#endif /* __ACE_INLINE__ */

#endif /* METRICS_LOCAL_CACHE_T_CPP */
