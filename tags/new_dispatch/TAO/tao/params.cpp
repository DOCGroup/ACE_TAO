#if 0
#include "tao/params.h"
#include "tao/connect.h"
#include "tao/objtable.h"
#endif

#include "tao/corba.h"

TAO_OA_Parameters *
TAO_OA_Parameters::instance (void)
{
  ACE_TRACE ("TAO_OA_Parameters::instance");

  TAO_OA_Parameters *&singleton = TAO_OA_Parameters::instance_i ();

  // Perform the Double-Check pattern...
  if (singleton == 0)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, (TAO_OA_Parameters::singleton_lock_i ()), 0);

      if (singleton == 0)
	ACE_NEW_RETURN (singleton, TAO_OA_Parameters, 0);
    }

  return singleton;
}

TAO_OA_Parameters *
TAO_OA_Parameters::instance (TAO_OA_Parameters *new_instance)
{
  ACE_TRACE ("TAO_OA_Parameters::set_instance");

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, (TAO_OA_Parameters::singleton_lock_i ()), 0);

  TAO_OA_Parameters *&singleton = TAO_OA_Parameters::instance_i ();
  TAO_OA_Parameters *old_instance = singleton;
  singleton = new_instance;

  return old_instance;
}

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

// Pointer to the Singleton instance.
TAO_OA_Parameters *TAO_OA_Parameters::instance_ = 0;

// Lock the creation of the singleton.  
ACE_SYNCH_MUTEX TAO_OA_Parameters::ace_singleton_lock_;

#if !defined (__ACE_INLINE__)
#  include "params.i"
#endif /* __ACE_INLINE__ */

