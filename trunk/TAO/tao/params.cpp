#include "tao/corba.h"

void 
TAO_OA_Parameters::demux_strategy (const char* strategy)
{
  // Determine the demux strategy based on the given name
  if (!ACE_OS::strcmp (strategy, "linear"))
    this->demux_ = TAO_LINEAR;
  else if (!ACE_OS::strcmp (strategy, "dynamic_hash"))
    this->demux_ = TAO_DYNAMIC_HASH;
  else if (!ACE_OS::strcmp (strategy, "user_def"))
    this->demux_ = TAO_USER_DEFINED;
  else if (!ACE_OS::strcmp (strategy, "active_demux"))
    this->demux_ = TAO_ACTIVE_DEMUX;
  else
    // Provide fallback!
    this->demux_ = TAO_DYNAMIC_HASH;
}

#if !defined (__ACE_INLINE__)
#  include "params.i"
#endif /* __ACE_INLINE__ */

