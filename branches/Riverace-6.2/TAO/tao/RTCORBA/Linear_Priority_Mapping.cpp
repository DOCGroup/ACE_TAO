// $Id$

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Linear_Priority_Mapping.h"
#include "tao/debug.h"
#include "ace/Sched_Params.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Linear_Priority_Mapping::TAO_Linear_Priority_Mapping (long policy)
  : policy_ (policy)
  , min_ (ACE_Sched_Params::priority_min (this->policy_))
  , max_ (ACE_Sched_Params::priority_max (this->policy_))
{
}

TAO_Linear_Priority_Mapping::~TAO_Linear_Priority_Mapping (void)
{
}

CORBA::Boolean
TAO_Linear_Priority_Mapping::to_native (
  RTCORBA::Priority corba_priority,
  RTCORBA::NativePriority &native_priority)
{
  if (corba_priority < RTCORBA::minPriority
           // The line below will always be false unless the value of
           // RTCORBA::maxPriority, which is now assigned the value of
           // 32767, is changed in RTCORBA.pidl.
//      || corba_priority > RTCORBA::maxPriority
     )
    {
      return 0;
    }

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
       * (corba_priority - RTCORBA::minPriority)
       / (RTCORBA::maxPriority - RTCORBA::minPriority));

  // Now, find the value corresponding to this index.
  native_priority = static_cast<RTCORBA::NativePriority> (this->min_);
  for (int i = 2; i <= native_priority_index; ++i)
    {
      native_priority = static_cast<RTCORBA::NativePriority>
        (ACE_Sched_Params::next_priority (this->policy_, native_priority));
    }
  return 1;

#else

  native_priority =
    this->min_
    + ((this->max_ - this->min_)
       * (corba_priority - RTCORBA::minPriority)
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
      int numerator = (RTCORBA::maxPriority - RTCORBA::minPriority)
                            * (native_priority_index - 1);

      div_t corba_offset = div (numerator, delta);

      int rounding = 0;

      if (corba_offset.rem)
        {
          rounding = ((numerator < 0 && delta < 0) ||
                      (numerator >= 0 && delta >= 0) ? 1 : -1);
        }

      corba_priority = static_cast<RTCORBA::Priority>
        (RTCORBA::minPriority + corba_offset.quot + rounding);
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
        TAOLIB_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Linear_Priority_Mapping::to_CORBA: "
                    " priority %d out of range [%d,%d]\n",
                    native_priority, this->min_, this->max_));
      return 0;
    }

  int delta = this->max_ - this->min_;
  if (delta != 0)
    {
      int numerator = (RTCORBA::maxPriority - RTCORBA::minPriority)
                            * (native_priority - this->min_);

      div_t corba_offset = div (numerator, delta);

      int rounding = 0;

      if (corba_offset.rem)
        {
          rounding = ((numerator < 0 && delta < 0) ||
                      (numerator >= 0 && delta >= 0) ? 1 : -1);
        }

      corba_priority =
        RTCORBA::minPriority + corba_offset.quot + rounding;
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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
