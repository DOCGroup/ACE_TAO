// $Id$

#ifndef ACE_TIMEPROBE_T_C
#define ACE_TIMEPROBE_T_C

#include "ace/OS/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Timeprobe_T, "$Id$")

#if defined (ACE_COMPILE_TIMEPROBES)

#include "ace/Timer/Timeprobe.h"
#include "ace/Timer/High_Res_Timer.h"

template <class ACE_LOCK>
ACE_Timeprobe<ACE_LOCK>::ACE_Timeprobe (u_long size)
  : timeprobes_ (0),
    lock_ (),
    max_size_ (size),
    current_size_ (0)
{
  ACE_NEW (this->timeprobes_,
           ACE_timeprobe_t[this->max_size_]);

#if defined (VXWORKS)
  if (sysProcNumGet () == 0)
    this->current_slot_vme_address_ = (u_int *) 0xDa010000;
  else
    this->current_slot_vme_address_ = (u_int *) 0xD8010000;
#endif /* VXWORKS */
}

template <class ACE_LOCK>
ACE_Timeprobe<ACE_LOCK>::ACE_Timeprobe (const ACE_Timeprobe<ACE_LOCK> &)
{
  //
  // Stupid MSVC is forcing me to define this; please don't use it.
  //

  ACE_ERROR ((LM_ERROR,
              ACE_LIB_TEXT ("ACE_NOTSUP: %s, line %d\n"), __FILE__, __LINE__));
  errno = ENOTSUP;
}

template <class ACE_LOCK>
ACE_Timeprobe<ACE_LOCK>::~ACE_Timeprobe (void)
{
  delete [] this->timeprobes_;
}

template <class ACE_LOCK> void
ACE_Timeprobe<ACE_LOCK>::timeprobe (u_long event)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  ACE_ASSERT (this->current_size_ < this->max_size_);

  this->timeprobes_[this->current_size_].event_.event_number_ = event;
  this->timeprobes_[this->current_size_].event_type_ = ACE_timeprobe_t::NUMBER;
  this->timeprobes_[this->current_size_].time_ = ACE_OS::gethrtime ();
  this->timeprobes_[this->current_size_].thread_ = ACE_OS::thr_self ();

  this->current_size_++;

#if defined (VMETRO_TIME_TEST) && (VXWORKS)
  // If we are using the VMETRO board to get time samples, then write
  // to the other boards VME address.
  *this->current_slot_vme_address_ = event;
#endif /* VMETRO_TIME_TEST && VXWORKS */
}

template <class ACE_LOCK> void
ACE_Timeprobe<ACE_LOCK>::timeprobe (const char *event)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  ACE_ASSERT (this->current_size_ < this->max_size_);

  this->timeprobes_[this->current_size_].event_.event_description_ = event;
  this->timeprobes_[this->current_size_].event_type_ = ACE_timeprobe_t::STRING;
  this->timeprobes_[this->current_size_].time_ = ACE_OS::gethrtime ();
  this->timeprobes_[this->current_size_].thread_ = ACE_OS::thr_self ();

  this->current_size_++;
}

template <class ACE_LOCK> void
ACE_Timeprobe<ACE_LOCK>::reset (void)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  this->current_size_ = 0;
}

template <class ACE_LOCK> ACE_Unbounded_Set<ACE_Event_Descriptions> &
ACE_Timeprobe<ACE_LOCK>::event_descriptions (void)
{
  return this->event_descriptions_;
}

template <class ACE_LOCK> ACE_Unbounded_Set<ACE_Event_Descriptions> &
ACE_Timeprobe<ACE_LOCK>::sorted_event_descriptions (void)
{
  return this->sorted_event_descriptions_;
}

template <class ACE_LOCK> u_int *
ACE_Timeprobe<ACE_LOCK>::current_slot_vme_address (void)
{
  return this->current_slot_vme_address_;
}

template <class ACE_LOCK> ACE_timeprobe_t *
ACE_Timeprobe<ACE_LOCK>::timeprobes (void)
{
  return this->timeprobes_;
}

template <class ACE_LOCK> ACE_LOCK &
ACE_Timeprobe<ACE_LOCK>::lock (void)
{
  return this->lock_;
}

template <class ACE_LOCK> u_long
ACE_Timeprobe<ACE_LOCK>::max_size (void)
{
  return this->max_size_;
}

template <class ACE_LOCK> u_long
ACE_Timeprobe<ACE_LOCK>::current_size (void)
{
  return this->current_size_;
}

template <class ACE_LOCK> int
ACE_Timeprobe<ACE_LOCK>::event_descriptions (const char **descriptions,
                                             u_long minimum_id)
{
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  ACE_Event_Descriptions events;
  events.descriptions_ = descriptions;
  events.minimum_id_ = minimum_id;

  this->event_descriptions_.insert (events);

  return 0;
}

template <class ACE_LOCK> void
ACE_Timeprobe<ACE_LOCK>::print_times (void)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  // Sort the event descriptions
  this->sort_event_descriptions_i ();

  ACE_DEBUG ((LM_DEBUG,
              "\nACE_Timeprobe; %d timestamps were recorded:\n",
              this->current_size_));

  if (this->current_size_ == 0)
    return;

  ACE_DEBUG ((LM_DEBUG,
              "\n%-50.50s %8.8s %13.13s\n\n",
              "Event",
              "thread",
              "usec"));

  ACE_DEBUG ((LM_DEBUG,
              "%-50.50s %8.8x %13.13s\n",
              this->find_description_i (0),
              this->timeprobes_[0].thread_,
              "START"));

  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  for (u_long i = 1; i < this->current_size_; i++)
    {
      ACE_hrtime_t time_difference =
        this->timeprobes_[i].time_ - this->timeprobes_[i-1].time_;

      ACE_UINT32 elapsed_time_in_micro_seconds =
        (ACE_UINT32) (time_difference / gsf);
      ACE_UINT32 remainder =
        (ACE_UINT32) (time_difference % gsf);
      // Convert to the fractional part in microseconds, with 3 digits
      // of precision (hence the 1000).
      ACE_UINT32 fractional = remainder * 1000 / gsf;

      ACE_DEBUG ((LM_DEBUG,
                  "%-50.50s %8.8x %10u.%03.3u\n",
                  this->find_description_i (i),
                  this->timeprobes_[i].thread_,
                  (unsigned int) elapsed_time_in_micro_seconds,
                  (unsigned int) fractional));
    }
}

template <class ACE_LOCK> void
ACE_Timeprobe<ACE_LOCK>::print_absolute_times (void)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  // Sort the event descriptions
  this->sort_event_descriptions_i ();

  ACE_DEBUG ((LM_DEBUG,
              "\nACE_Timeprobe; %d timestamps were recorded:\n",
              this->current_size_));

  if (this->current_size_ == 0)
    return;

  ACE_DEBUG ((LM_DEBUG,
              "\n%-50.50s %8.8s %13.13s\n\n",
              "Event",
              "thread",
              "stamp"));

  for (u_long i = 0; i < this->current_size_; i++)
    {
      char buf[64];
      ACE_OS::sprintf (buf, "%llu", this->timeprobes_[i].time_);
      ACE_DEBUG ((LM_DEBUG,
                  "%-50.50s %8.8x %13.13s\n",
                  this->find_description_i (i),
                  this->timeprobes_[i].thread_,
                  buf));
    }
}

template <class ACE_LOCK> const char *
ACE_Timeprobe<ACE_LOCK>::find_description_i (u_long i)
{
  if (this->timeprobes_[i].event_type_ == ACE_timeprobe_t::STRING)
    return this->timeprobes_[i].event_.event_description_;
  else
    {
      EVENT_DESCRIPTIONS::iterator iterator = this->sorted_event_descriptions_.begin ();
      for (u_long j = 0;
           j < this->sorted_event_descriptions_.size () - 1;
           iterator++, j++)
        {
          EVENT_DESCRIPTIONS::iterator next_event_descriptions = iterator;
          next_event_descriptions++;

          if (this->timeprobes_[i].event_.event_number_ < (*next_event_descriptions).minimum_id_)
            break;
        }
      return (*iterator).descriptions_[this->timeprobes_[i].event_.event_number_ - (*iterator).minimum_id_];
    }
}

template <class ACE_LOCK> void
ACE_Timeprobe<ACE_LOCK>::sort_event_descriptions_i (void)
{
  size_t total_elements = this->event_descriptions_.size ();

  for (size_t i = 0;
       i < total_elements;
       i++)
    {
      EVENT_DESCRIPTIONS::iterator iterator = this->event_descriptions_.begin ();
      ACE_Event_Descriptions min_entry = *iterator;

      for (;
           iterator != this->event_descriptions_.end ();
           iterator++)
        if ((*iterator).minimum_id_ < min_entry.minimum_id_)
          min_entry = *iterator;

      this->sorted_event_descriptions_.insert (min_entry);
      this->event_descriptions_.remove (min_entry);
    }
}

template <class Timeprobe>
ACE_Function_Timeprobe<Timeprobe>::ACE_Function_Timeprobe (Timeprobe &timeprobe,
                                                           u_long event)
  : timeprobe_ (timeprobe),
    event_ (event)
{
  this->timeprobe_.timeprobe (this->event_);
}

template <class Timeprobe>
ACE_Function_Timeprobe<Timeprobe>::~ACE_Function_Timeprobe (void)
{
  this->timeprobe_.timeprobe (this->event_ + 1);
}

#endif /* ACE_COMPILE_TIMEPROBES */
#endif /* ACE_TIMEPROBE_T_C */
