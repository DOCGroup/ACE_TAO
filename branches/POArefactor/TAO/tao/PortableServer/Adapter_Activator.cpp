// $Id$

#include "Adapter_Activator.h"
#include "PortableServerC.h"

ACE_RCSID(PortableServer, Adapter_Activator, "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

TAO_Adapter_Activator::TAO_Adapter_Activator (PortableServer::POAManager_ptr poa_manager)
  : poa_manager_ (PortableServer::POAManager::_duplicate (poa_manager))
{
}

CORBA::Boolean
TAO_Adapter_Activator::unknown_adapter (PortableServer::POA_ptr parent,
                                        const char *name
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Default policies
  CORBA::PolicyList default_policies;

  // This assumes that the lock on the parent is recursive
  PortableServer::POA_var child = parent->create_POA (name,
                                                      this->poa_manager_.in (),
                                                      default_policies
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_TRY
    {
      child->the_activator (this ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      child->destroy (1,
                      1
                      ACE_ENV_ARG_PARAMETER);

      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  // Finally everything is fine
  return 1;
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

