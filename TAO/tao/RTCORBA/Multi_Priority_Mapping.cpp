// $Id$

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Multi_Priority_Mapping.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"
#include "ace/Sched_Params.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Multi_Priority_Mapping::TAO_Multi_Priority_Mapping (
  int base_native_priority,
  int base_corba_priority,
  int priority_spacing,
  int priorities_contiguous,
  int policy)
  :  base_native_priority_ (base_native_priority)
  ,  base_corba_priority_ (base_corba_priority)
  ,  priority_spacing_ (priority_spacing)
  ,  priorities_contiguous_(priorities_contiguous)
  ,  policy_ (policy)
  ,  min_ (ACE_Sched_Params::priority_min (this->policy_))
  ,  max_ (ACE_Sched_Params::priority_max (this->policy_))
{
  if ( this->min_ < this->max_ )
  {
     if (base_native_priority_ < this->min_)
     {
        if (TAO_debug_level > 2)
        {
           ACE_DEBUG ((LM_DEBUG,
                       "TAO (%P|%t) - Multi_Priority_Mapping::ctor: "
                       " base_native_priority %d out of range [%d,%d]\n",
                        base_native_priority_, this->min_, this->max_));
        }
     }
  }
  else
  {
     if (base_native_priority_ > this->min_)
     {
        if (TAO_debug_level > 2)
        {
           ACE_DEBUG ((LM_DEBUG,
                       "TAO (%P|%t) - Multi_Priority_Mapping::ctor: "
                       " base_native_priority %d out of range [%d,%d]\n",
                        base_native_priority_, this->min_, this->max_));
        }
     }
  }

  if (base_corba_priority_ > RTCORBA::maxPriority)
    {
       if (TAO_debug_level > 2)
         {
           ACE_DEBUG ((LM_DEBUG,
             "TAO (%P|%t) - Multi_Priority_Mapping::ctor: "
             " base_corba_priority %d out of range [%d,%d]\n",
             base_corba_priority_, RTCORBA::minPriority, RTCORBA::maxPriority));
         }
    }
}

TAO_Multi_Priority_Mapping::~TAO_Multi_Priority_Mapping (void)
{
}

CORBA::Boolean
TAO_Multi_Priority_Mapping::to_native (RTCORBA::Priority corba_priority,
                                       RTCORBA::NativePriority &native_priority)
{
  // Check for an invalid native priority
  if (corba_priority < RTCORBA::minPriority || corba_priority > this->base_corba_priority_ )
  {
     if (TAO_debug_level > 2)
     {
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Multi_Priority_Mapping::to_native: "
                    " corba priority %d out of range [%d,%d]\n",
                    corba_priority, RTCORBA::minPriority, this->base_corba_priority_));
     }
     return 0;
  }

  if (corba_priority == base_corba_priority_)
  {
     // If this is the highest priority endpoint, then just give it the highest priority corba base priority
     native_priority = static_cast<RTCORBA::NativePriority> (base_native_priority_);
  }
  else
  {
     if (priorities_contiguous_ == 1)
     {
        if ( this->min_ < this->max_ )
        {
           native_priority = static_cast<RTCORBA::NativePriority> 
             (((corba_priority - base_corba_priority_) / priority_spacing_) + base_native_priority_);
        }
        else
        {
           native_priority = static_cast<RTCORBA::NativePriority> 
             (((base_corba_priority_ - corba_priority) / priority_spacing_) + base_native_priority_);
        }
     }
     else
     {
        // Start at the max priority and search until we reach the base priority
        int last_priority = this->base_corba_priority_;
        while (true)
        {
           if (last_priority == RTCORBA::minPriority) break;
           if (base_corba_priority_ >= --last_priority) break;
        }

        int priority_ndx = 0;
        while (true)
        {
           if (last_priority == RTCORBA::minPriority) break;
           if (corba_priority >= --last_priority) break;
           priority_ndx++;
        }

        // Start at the max priority and search until we reach the base priority
        last_priority = this->base_native_priority_;
        for (int current_ndx = 0; current_ndx < priority_ndx; current_ndx++)
        {
           native_priority = static_cast<RTCORBA::NativePriority> 
             (ACE_Sched_Params::previous_priority (this->policy_,
                                                   last_priority,
                                                   ACE_SCOPE_THREAD));
        }
     }
  }

  return 1;
}

CORBA::Boolean
TAO_Multi_Priority_Mapping::to_CORBA (RTCORBA::NativePriority native_priority,
                                       RTCORBA::Priority &corba_priority)
{
  // Check for an invalid native priority
  if ((this->min_ < this->max_ && (native_priority < this->min_ || native_priority > this->base_native_priority_ )) ||
      (this->min_ > this->max_ && (native_priority < this->base_corba_priority_ || native_priority > this->min_)))
  {
     if (TAO_debug_level > 2)
     {
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Multi_Priority_Mapping::to_CORBA: "
                    " priority %d out of range [%d,%d]\n",
                    native_priority, this->min_, this->base_corba_priority_));
     }
     return 0;
  }

  if (native_priority == base_native_priority_)
  {
     // If this is the highest priority endpoint, then just give it the highest priority corba base priority
     corba_priority = static_cast<RTCORBA::Priority> (base_corba_priority_);
  }
  else
  {
     if (priorities_contiguous_ == 1)
     {
        if ( this->min_ < this->max_ )
        {
           corba_priority = static_cast<RTCORBA::Priority> 
             (((native_priority - base_native_priority_) * priority_spacing_) + base_corba_priority_);
        }
        else
        {
           corba_priority = static_cast<RTCORBA::Priority> 
             (((base_native_priority_ - native_priority) * priority_spacing_) + base_corba_priority_);
        }
     }
     else
     {
        // Start at the max priority and search until we reach the base priority
        int last_priority = this->base_native_priority_;
        while (true)
        {
           int previous_priority = ACE_Sched_Params::previous_priority (this->policy_,
                                                                        last_priority,
                                                                        ACE_SCOPE_THREAD);
           last_priority = previous_priority;

           if (last_priority == this->min_)
           {
              break;
           }

           if ( this->min_ < this->max_ )
           {
              if (base_native_priority_ >= previous_priority) break;
           }
           else
           {
              if (base_native_priority_ <= previous_priority) break;
           }
        }

        int priority_ndx = 1;
        while (true)
        {
           if (last_priority == this->min_)
           {
              break;
           }

           if ( this->min_ < this->max_ )
           {
              if (native_priority >= last_priority) break;
           }
           else
           {
              if (native_priority <= last_priority) break;
           }

           int previous_priority = ACE_Sched_Params::previous_priority (this->policy_,
                                                                        last_priority,
                                                                        ACE_SCOPE_THREAD);
           last_priority = previous_priority;
           priority_ndx++;
        }

        corba_priority = static_cast<RTCORBA::Priority> 
          (base_corba_priority_ - priority_ndx);
     }
  }

  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
