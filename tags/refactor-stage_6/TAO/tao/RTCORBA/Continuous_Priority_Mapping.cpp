// $Id$

#include "Continuous_Priority_Mapping.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "ace/Sched_Params.h"

#if !defined (__ACE_INLINE__)
# include "Continuous_Priority_Mapping.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(RTCORBA, Continuous_Priority_Mapping, "$Id: ")

TAO_Continuous_Priority_Mapping::TAO_Continuous_Priority_Mapping (int policy)
  :  policy_ (policy)
{
  this->min_ =
    ACE_Sched_Params::priority_min (this->policy_);

  this->max_ =
    ACE_Sched_Params::priority_max (this->policy_);
}

TAO_Continuous_Priority_Mapping::~TAO_Continuous_Priority_Mapping (void)
{
}

CORBA::Boolean
TAO_Continuous_Priority_Mapping::to_native (RTCORBA::Priority corba_priority,
                                            RTCORBA::NativePriority &native_priority)
{
  if (corba_priority < 0)
    return 0;

#if defined (ACE_WIN32)

  int current_native_priority = this->min_;
  int next_native_priority;
  for (int i = 1; i <= corba_priority; ++i)
    {
      next_native_priority =
        ACE_Sched_Params::next_priority (this->policy_,
                                         current_native_priority);

      if (next_native_priority == current_native_priority)
        return 0;

      current_native_priority = next_native_priority;
    }

  native_priority = current_native_priority;
  return 1;

#else

  if (this->min_ < this->max_)
    {
      native_priority = corba_priority + this->min_;
      if (native_priority > this->max_)
        return 0;
    }
  else if (this->min_ > this->max_)
    {
      native_priority = this->min_ - corba_priority;
      if (native_priority < this->max_)
        return 0;
    }
  else
    {
      // There is only one native priority.
      if (corba_priority != 0)
        return 0;

      native_priority = this->min_;
    }

  return 1;

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
        return 1;

      else if (current_native_priority == this->max_)
        return 0;

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
        return 0;
      corba_priority = native_priority - this->min_;
    }
  else if (this->min_ > this->max_)
    {
      if (native_priority > this->min_
          || native_priority < this->max_)
        return 0;
      corba_priority = this->min_ - native_priority;
    }
  else if (this->min_ == this->max_)
    {
      if (native_priority != this->min_)
        return 0;
      corba_priority = 0;
    }

  return 1;

#endif /* ACE_WIN32 */

}

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
