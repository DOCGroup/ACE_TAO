// $Id$

#include "tao/orbconf.h"

#include "Linear_Priority_Mapping.h"
#include "tao/debug.h"
#include "ace/Sched_Params.h"

#if !defined (__ACE_INLINE__)
# include "Linear_Priority_Mapping.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(Strategies, Linear_Priority_Mapping, "$Id$")

TAO_Linear_Priority_Mapping::TAO_Linear_Priority_Mapping (int policy)
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

TAO_Linear_Priority_Mapping::~TAO_Linear_Priority_Mapping (void)
{
}

CORBA::Boolean
TAO_Linear_Priority_Mapping::to_native (RTCORBA::Priority corba_priority,
                                        RTCORBA::NativePriority &native_priority)
{
  if (corba_priority < 0 || corba_priority > RTCORBA::maxPriority)
    return 0;

#if defined (ACE_WIN32)
  // Count up the number of distinct native priorities on current
  // platform.
  int n;
  int current_priority = this->min_;
  for (n = 1; current_priority != this->max_; ++n)
    {
      current_priority =
        ACE_Sched_Params::next_priority (this->policy_,
                                         current_priority);
    }
  int native_priority_index =
    1
    + ((n - 1)
       * corba_priority
       / (RTCORBA::maxPriority - RTCORBA::minPriority));

  // Now, find the value corresponding to this index.
  native_priority = this->min_;
  for (int i = 2; i <= native_priority_index; ++i)
    {
      native_priority = ACE_Sched_Params::next_priority (this->policy_,
                                                         native_priority);
    }
  return 1;

#else

  native_priority =
    this->min_
    + ((this->max_ - this->min_)
       * corba_priority
       / (RTCORBA::maxPriority - RTCORBA::minPriority));

  return 1;

#endif /* ACE_WIN32 */
}

CORBA::Boolean
TAO_Linear_Priority_Mapping::to_CORBA (RTCORBA::NativePriority native_priority,
                                       RTCORBA::Priority &corba_priority)
{
#if defined (ACE_WIN32)

  // Iterate over native priorities in order to 1) make sure
  // <native_priority> argument contains a valid value, 2) count its
  // index among all valid native pr. values, 3) get the total number
  // of native priority values for the current platform.

  int total;
  int native_priority_index = 0;
  int current_priority = this->min_;
  for (total = 1; ; ++total)
    {
      if (native_priority == current_priority)
        native_priority_index = total;

      if (current_priority == this->max_)
        break;

      current_priority =
        ACE_Sched_Params::next_priority (this->policy_,
                                         current_priority);
    }

  if (native_priority_index == 0)
    return 0;

  int delta = total - 1;
  if (delta != 0)
    {
      corba_priority =
        RTCORBA::minPriority
        + ((RTCORBA::maxPriority - RTCORBA::minPriority)
           * (native_priority_index - 1) / delta);
    }
  else
    {
      // There is only one native priority.
      corba_priority = RTCORBA::minPriority;
    }

  return 1;

#else

  if ((this->min_ < this->max_
       && (native_priority < this->min_
           || native_priority > this->max_))
      || (this->min_ > this->max_
          && (native_priority < this->max_
              || native_priority > this->min_)))
    {
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Linear_Priority_Mapping::to_CORBA: "
                    " priority %d out of range [%d,%d]\n",
                    native_priority, this->min_, this->max_));
      return 0;
    }

  int delta = this->max_ - this->min_;
  if (delta != 0)
    {
      corba_priority =
        RTCORBA::minPriority
        + ((RTCORBA::maxPriority - RTCORBA::minPriority)
           * (native_priority - this->min_) / delta);
    }
  else
    {
      // There is only one native priority.
      if (native_priority != this->min_)
        return 0;
      corba_priority = RTCORBA::minPriority;
    }

  return 1;

#endif /* ACE_WIN32 */
}
