// $Id$

#include "tao/orbconf.h"

#if defined (TAO_HAS_RT_CORBA)

#include "tao/Direct_Priority_Mapping.h"
#include "tao/debug.h"
#include "ace/Sched_Params.h"

#if !defined (__ACE_INLINE__)
# include "tao/Direct_Priority_Mapping.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Direct_Priority_Mapping, "$Id$")

TAO_Direct_Priority_Mapping::TAO_Direct_Priority_Mapping (int policy)
  :  policy_ (policy)
{
  this->min_ = ACE_Sched_Params::priority_min (this->policy_);
  this->max_ = ACE_Sched_Params::priority_max (this->policy_);
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
      native_priority = this->min_;
    }

  return 1;
}

CORBA::Boolean
TAO_Direct_Priority_Mapping::to_CORBA (RTCORBA::NativePriority native_priority,
                                       RTCORBA::Priority &corba_priority)
{
  if (this->min_ < this->max_)
    {
      corba_priority = native_priority - this->min_;
      if (corba_priority < 0)
        return 0;
    }
  else if (this->min_ > this->max_)
    {
      corba_priority = this->min_ - native_priority;
      if (corba_priority < 0)
        return 0;
    }
  else if (this->min_ == this->max_)
    {
      corba_priority = 0;
    }

  return 1;
}

#endif /* TAO_HAS_RT_CORBA */
