// $Id$

#if !defined (ACE_TIMEPROBE_C)
#define ACE_TIMEPROBE_C

#include "ace/Timeprobe.h"

#if defined (ACE_ENABLE_TIMEPROBES)

#if !defined (__ACE_INLINE__)
#include "ace/Timeprobe.i"
#endif /* __ACE_INLINE__ */

template <class ACE_LOCK>
ACE_Timeprobe<ACE_LOCK>::ACE_Timeprobe (u_long size,
                                        ACE_Allocator *alloc)
  : allocator_ (alloc != 0 ? alloc : ACE_Allocator::instance ()),
    timeprobes_ (0),
    lock_ (),
    max_size_ (size),
    current_size_ (0)
{
  void *space = this->allocator_->malloc ((sizeof timeprobe_t) * this->max_size_);
  this->timeprobes_ = new ((timeprobe_t *) space) timeprobe_t[this->max_size_];
}

template <class ACE_LOCK>
ACE_Timeprobe<ACE_LOCK>::~ACE_Timeprobe (void)
{
  for (int i = 0; i < this->max_size_; i++)
    this->timeprobes_[i].timeprobe_t::~timeprobe_t ();

  this->allocator_->free (this->timeprobes_);
}

template <class ACE_LOCK> void
ACE_Timeprobe<ACE_LOCK>::timeprobe (u_long event)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  ACE_ASSERT (this->current_size_ < this->max_size_);

  this->timeprobes_[this->current_size_].event_.event_number_ = event;
  this->timeprobes_[this->current_size_].event_type_ = timeprobe_t::NUMBER;
  this->timeprobes_[this->current_size_].time_ = ACE_OS::gettimeofday ();
  this->timeprobes_[this->current_size_].thread_ = ACE_OS::thr_self ();

  this->current_size_++;
}

template <class ACE_LOCK> void
ACE_Timeprobe<ACE_LOCK>::timeprobe (const char *event)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  ACE_ASSERT (this->current_size_ < this->max_size_);

  this->timeprobes_[this->current_size_].event_.event_description_ = event;
  this->timeprobes_[this->current_size_].event_type_ = timeprobe_t::STRING;
  this->timeprobes_[this->current_size_].time_ = ACE_OS::gettimeofday ();
  this->timeprobes_[this->current_size_].thread_ = ACE_OS::thr_self ();

  this->current_size_++;
}

template <class ACE_LOCK> void
ACE_Timeprobe<ACE_LOCK>::reset (void)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  this->current_size_ = 0;
}

template <class ACE_LOCK> void
ACE_Timeprobe<ACE_LOCK>::print_times (const char *event_descriptions[]) const
{
  ACE_DEBUG ((LM_DEBUG,
              "\nACE_Timeprobe; %d timestamps were recorded:\n",
              this->current_size_));

  if (this->current_size_ == 0)
    return;

  ACE_DEBUG ((LM_DEBUG,
              "\n%-50.50s %8.8s %10.10s\n\n",
              "Event",
              "thread",
              "usec"));

  const char *description = 0;
  if (this->timeprobes_[0].event_type_ == timeprobe_t::STRING)
    description = this->timeprobes_[0].event_.event_description_;
  else
    description = event_descriptions[this->timeprobes_[0].event_.event_number_];
                                    
  ACE_DEBUG ((LM_DEBUG,
              "%-50s %8.8x %10.10s\n",
              description,
              this->timeprobes_[0].thread_,
              "START"));

  for (u_int i = 1; i < this->current_size_; i++)
    {
      ACE_Time_Value elapsed =
        this->timeprobes_[i].time_ - this->timeprobes_[i-1].time_;

      if (this->timeprobes_[i].event_type_ == timeprobe_t::STRING)
        description = this->timeprobes_[i].event_.event_description_;
      else
        description = event_descriptions[this->timeprobes_[i].event_.event_number_];
                                    
      ACE_DEBUG ((LM_DEBUG,
                  "%-50s %8.8x %10.3d\n",
                  description,
                  this->timeprobes_[i].thread_,
                  elapsed.msec () * 1000));
    }
}

#endif /* ACE_ENABLE_TIMEPROBES */

#endif /* ACE_TIMEPROBE_C */
