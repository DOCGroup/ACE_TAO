// $Id$

#include "tao/orbconf.h"

#include "Direct_Priority_Mapping.h"
#include "ace/Sched_Params.h"

#if !defined (__ACE_INLINE__)
# include "Direct_Priority_Mapping.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(Strategies, Direct_Priority_Mapping, "$Id$")

TAO_Direct_Priority_Mapping::TAO_Direct_Priority_Mapping (int policy)
  :  policy_ (policy)
{
  // We have special behavior for SUNs.  This is because the results
  // from ACE_Sched_Params::priority_min() and
  // ACE_Sched_Params::priority_max() are not correct.

#if defined (sun)

  ACE_utsname name;
  ACE_OS::uname (&name);

  // If we are on Solaris 5.7.
  if (ACE_OS::strcmp (name.release, "5.7") == 0)
    {
      if (policy == ACE_SCHED_OTHER)
        {
          this->min_ = 0;
          this->max_ = 127;
        }
      else
        {
          this->min_ = 0;
          this->max_ = 59;
        }
    }
  // If we are on Solaris 5.8.
  else if (ACE_OS::strcmp (name.release, "5.8") == 0)
    {
      if (policy == ACE_SCHED_OTHER)
        {
          this->min_ = -20;
          this->max_ = 127;
        }
      else
        {
          this->min_ = 0;
          this->max_ = 59;
        }
    }
  // Not sure about the other SUN releases.  Therefore, lets stick to
  // the default behavior.
  else
    {
      this->min_ = ACE_Sched_Params::priority_min (this->policy_);
      this->max_ = ACE_Sched_Params::priority_max (this->policy_);
    }

#else /* sun */

  // Other platforms should be ok.
  this->min_ = ACE_Sched_Params::priority_min (this->policy_);
  this->max_ = ACE_Sched_Params::priority_max (this->policy_);

#endif /* sun */

}

TAO_Direct_Priority_Mapping::~TAO_Direct_Priority_Mapping (void)
{
}

CORBA::Boolean
TAO_Direct_Priority_Mapping::to_native (RTCORBA::Priority corba_priority,
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
TAO_Direct_Priority_Mapping::to_CORBA (RTCORBA::NativePriority native_priority,
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
