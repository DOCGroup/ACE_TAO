// $Id$

#include "tao/orbconf.h"

#if defined (TAO_HAS_RT_CORBA)

#include "tao/Linear_Priority_Mapping.h"
#include "tao/debug.h"
#include "ace/Sched_Params.h"

#if !defined (__ACE_INLINE__)
# include "tao/Linear_Priority_Mapping.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Linear_Priority_Mapping, "$Id$")

TAO_Linear_Priority_Mapping::TAO_Linear_Priority_Mapping (int policy)
  :  policy_ (policy)
{
  this->min_ = ACE_Sched_Params::priority_min (this->policy_);
  this->max_ = ACE_Sched_Params::priority_max (this->policy_);
}

TAO_Linear_Priority_Mapping::~TAO_Linear_Priority_Mapping (void)
{
}

CORBA::Boolean
TAO_Linear_Priority_Mapping::to_native (RTCORBA::Priority corba_priority,
                                 RTCORBA::NativePriority &native_priority)
{
  if (corba_priority < 0)
    return 0;

  native_priority =
    this->min_
    + ((this->max_ - this->min_)
       * corba_priority
       / (RTCORBA::maxPriority - RTCORBA::minPriority));

  return 1;
}

CORBA::Boolean
TAO_Linear_Priority_Mapping::to_CORBA (RTCORBA::NativePriority native_priority,
                                RTCORBA::Priority &corba_priority)
{
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
           * native_priority / delta);
    }
  else
    {
      corba_priority = this->min_;
    }

  return 1;
}

#endif /* TAO_HAS_RT_CORBA */
