// $Id$

#include "tao/RTCORBA/Continuous_Priority_Mapping.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "ace/Sched_Params.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Continuous_Priority_Mapping::TAO_Continuous_Priority_Mapping (int policy)
  :  policy_ (policy)
   , min_ (ACE_Sched_Params::priority_min (this->policy_))
   , max_ (ACE_Sched_Params::priority_max (this->policy_))
{
}

TAO_Continuous_Priority_Mapping::~TAO_Continuous_Priority_Mapping (void)
{
}

CORBA::Boolean
TAO_Continuous_Priority_Mapping::to_native (RTCORBA::Priority corba_priority,
                                            RTCORBA::NativePriority &native_priority)
{
  if (corba_priority < 0)
    return false;

#if defined (ACE_WIN32)

  int current_native_priority = this->min_;
  int next_native_priority;
  for (int i = 1; i <= corba_priority; ++i)
    {
      next_native_priority =
        ACE_Sched_Params::next_priority (this->policy_,
                                         current_native_priority);

      if (next_native_priority == current_native_priority)
        return false;

      current_native_priority = next_native_priority;
    }

  native_priority = static_cast<RTCORBA::NativePriority> (current_native_priority);
  return true;

#else
  int native;

  if (this->min_ < this->max_)
    {
      native = corba_priority + this->min_;
      if (native > this->max_)
        return false;
    }
  else if (this->min_ > this->max_)
    {
      native = this->min_ - corba_priority;
      if (native < this->max_)
        return false;
    }
  else
    {
      // There is only one native priority.
      if (corba_priority != 0)
        return false;

      native = this->min_;
    }

  native_priority = native;

  return true;

#endif /* ACE_WIN32 */

}

CORBA::Boolean
TAO_Continuous_Priority_Mapping::to_CORBA (RTCORBA::NativePriority native_priority,
                                           RTCORBA::Priority &corba_priority)
{

#if defined (ACE_WIN32)

  int current_native_priority = this->min_;
  for (corba_priority = 0; ; ++corba_priority)
    {
      if (current_native_priority == native_priority)
        return true;

      else if (current_native_priority == this->max_)
        return false;

      else
        current_native_priority =
          ACE_Sched_Params::next_priority (this->policy_,
                                           current_native_priority);
    }

#else

  if (this->min_ < this->max_)
    {
      if (native_priority < this->min_
          || native_priority > this->max_)
        return false;
      corba_priority = native_priority - this->min_;
    }
  else if (this->min_ > this->max_)
    {
      if (native_priority > this->min_
          || native_priority < this->max_)
        return false;
      corba_priority = this->min_ - native_priority;
    }
  else if (this->min_ == this->max_)
    {
      if (native_priority != this->min_)
        return false;
      corba_priority = 0;
    }

  return true;

#endif /* ACE_WIN32 */

}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
