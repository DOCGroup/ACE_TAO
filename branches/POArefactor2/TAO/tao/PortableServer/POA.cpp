#include "tao/PortableServer/POA.h"

ACE_RCSID (PortableServer,
           POA,
           "$Id$")

//
#if !defined (__ACE_INLINE__)
# include "POA.i"
#endif /* ! __ACE_INLINE__ */

TAO_POA::TAO_POA (const TAO_POA::String &name,
                  TAO_POA_Manager &poa_manager,
                  const TAO_POA_Policy_Set &policies,
                  TAO_Root_POA *parent,
                  ACE_Lock &lock,
                  TAO_SYNCH_MUTEX &thread_lock,
                  TAO_ORB_Core &orb_core,
                  TAO_Object_Adapter *object_adapter
                  ACE_ENV_ARG_DECL)
  : TAO_Root_POA (name,
                  poa_manager,
                  policies,
                  parent,
                  lock,
                  thread_lock,
                  orb_core,
                  object_adapter
                  ACE_ENV_ARG_PARAMETER)
{
}

TAO_POA::~TAO_POA (void)
{
}

