#include "tao/params.h"
#include "tao/connect.h"
#include "tao/objtable.h"

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


// Pointer to the Singleton instance.
TAO_OA_Parameters *TAO_OA_Parameters::instance_ = 0;

// Lock the creation of the singleton.  
ACE_SYNCH_MUTEX TAO_OA_Parameters::ace_singleton_lock_;

#if !defined(__ACE_INLINE__)
#  include "params.i"
#endif

