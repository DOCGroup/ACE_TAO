//
// $Id$
//

#include "ace/OS.h"

#include "Timeprobe.h"

#if !defined (__ACE_INLINE__)
#include "Timeprobe.i"
#endif /* __ACE_INLINE__ */

ACE_Timeprobe *ACE_Timeprobe::instance_ = 0;

ACE_Timeprobe &
ACE_Timeprobe::instance ()
{
  if (instance_ == 0)
    {
      // if this allocation fails, we're in big trouble . . .
      ACE_NEW_RETURN (instance_, ACE_Timeprobe (), *instance_);
    }

  return *instance_;
}

void
ACE_Timeprobe::timeprobe (const char *id)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  timeprobes [current_slot_].id_ = id;
  timeprobes [current_slot_].time_ = ACE_OS::gethrtime ();
  timeprobes [current_slot_].thread_ = ACE_OS::thr_self ();

  ++current_slot_;

  ACE_ASSERT (current_slot_ < SLOTS);
}

void
ACE_Timeprobe::print_times () const
{
  ACE_OS::printf ("\nACE_Timeprobe; %u timeprobes were recorded:\n",
                  current_slot_ > 1  ?  current_slot_ - 1 :  0);

  if (current_slot_ <= 0)
    {
      return;
    }

  ACE_OS::printf ("%-52.52s  %8.8s %10.10s\n", "NAME", "thread", "usec");
  ACE_OS::printf ("\"%-50s\" %8.8x %10.10s\n",
		  timeprobes[0].id_, timeprobes[0].thread_, "START");
  for (u_int i = 1; i < current_slot_; ++i)
    {
      ACE_hrtime_t elapsed = timeprobes [i].time_ - timeprobes [i-1].time_;
      ACE_OS::printf ("\"%-50s\" %8.8x %10.3f\n",
		      timeprobes [i].id_,
		      timeprobes [i].thread_,
		      (double) (elapsed / 1000) /* nanosec/microsec */);
    }
#if 0
  ACE_hrtime_t elapsed2 = (timeprobes [current_slot_ - 1].time_
			   - timeprobes [0].time_);

  double total = elapsed2/1000.0; // nanoseconds/microsecond
  // print the total time
  ACE_OS::printf ("%52.52s =========\n"
		  "%46.46s total %10.3f usec\n",
		  "", "",
		  total);
#endif /* 0 */
}

