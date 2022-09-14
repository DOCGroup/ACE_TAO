#include "ace/Event_Base.h"

#if !defined (__ACE_INLINE__)
#include "ace/Event_Base.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Log_Category.h"
#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_Event_Base)

ACE_Event_Base::ACE_Event_Base ()
  : removed_ (false)
{
}

ACE_Event_Base::~ACE_Event_Base ()
{
  this->remove ();
}

int
ACE_Event_Base::remove ()
{
  int result = 0;
  if (!this->removed_)
    {
      this->removed_ = true;
      result = ACE_OS::event_destroy (&this->handle_);
    }
  return result;
}

int
ACE_Event_Base::wait ()
{
  return ACE_OS::event_wait (&this->handle_);
}

int
ACE_Event_Base::wait (const ACE_Time_Value *abstime, int use_absolute_time)
{
  return ACE_OS::event_timedwait (&this->handle_,
                                  const_cast <ACE_Time_Value *> (abstime),
                                  use_absolute_time);
}

int
ACE_Event_Base::signal ()
{
  return ACE_OS::event_signal (&this->handle_);
}

int
ACE_Event_Base::pulse ()
{
  return ACE_OS::event_pulse (&this->handle_);
}

int
ACE_Event_Base::reset ()
{
  return ACE_OS::event_reset (&this->handle_);
}

void
ACE_Event_Base::dump () const
{
#if defined (ACE_HAS_DUMP)
  ACELIB_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACELIB_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

ACE_END_VERSIONED_NAMESPACE_DECL
