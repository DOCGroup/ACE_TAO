// -*- C++ -*-
//
// $Id$

namespace DAnCE
  {

    template <class ACE_LOCK>
    ACE_INLINE Completion_Counter_Base<ACE_LOCK>::~Completion_Counter_Base ()
      {
      }

    template <class ACE_LOCK>
    ACE_INLINE Completion_Counter_Base<ACE_LOCK>::Completion_Counter_Base (
        unsigned int exec_count,
        unsigned int fail_count)
      : exec_count_ (exec_count), fail_count_ (fail_count)
      {
      }

    template <class ACE_LOCK>
    ACE_INLINE void
    Completion_Counter_Base<ACE_LOCK>::decrement_exec_count ()
      {
        ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

        --this->exec_count_;

        if (this->exec_count_ == 0)
          {
            if (this->fail_count_ > 0)
            {
              this->on_all_completed_with_failure ();
            }
          else
            {
              this->on_all_completed ();
            }
          }
      }

    template <class ACE_LOCK>
    ACE_INLINE void
    Completion_Counter_Base<ACE_LOCK>::increment_fail_count (const char* error)
      {
        ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

        ++this->fail_count_;
        this->errors_.push_back (error);
      }

    template <class ACE_LOCK>
    ACE_INLINE unsigned int
    Completion_Counter_Base<ACE_LOCK>::exec_count ()
      {
        ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, 0);

        return this->exec_count_;
      }

    template <class ACE_LOCK>
    ACE_INLINE unsigned int
    Completion_Counter_Base<ACE_LOCK>::fail_count ()
      {
        ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, 0);

        return this->fail_count_;
      }

    template <class ACE_LOCK>
    ACE_INLINE bool
    Completion_Counter_Base<ACE_LOCK>::all_completed ()
      {
        ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, false);

        return (this->exec_count_ == 0);
      }

    template <class ACE_LOCK>
    ACE_INLINE
    const typename Completion_Counter_Base<ACE_LOCK>::errors_type&
    Completion_Counter_Base<ACE_LOCK>::errors () const
      {
        return this->errors_;
      }

    template <class ACE_LOCK>
    ACE_INLINE unsigned int
    Completion_Counter_Base<ACE_LOCK>::exec_count_i ()
      {
        return this->exec_count_;
      }

    template <class ACE_LOCK>
    ACE_INLINE unsigned int
    Completion_Counter_Base<ACE_LOCK>::fail_count_i ()
      {
        return this->fail_count_;
      }

  } /*DAnCE */
