// $Id$

#include "Adapter_Activator.h"
#include "AdapterActivatorC.h"
#include "POAManagerC.h"

#define TAO_PORTABLESERVER_SAFE_INCLUDE
#include "PortableServerC.h"
#undef TAO_PORTABLESERVER_SAFE_INCLUDE

ACE_RCSID (PortableServer,
           Adapter_Activator,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {

#if (TAO_HAS_MINIMUM_POA == 0)

    Adapter_Activator::Adapter_Activator (
      PortableServer::POAManager_ptr pm)
      : poa_manager_ (PortableServer::POAManager::_duplicate (pm))
    {
    }

    CORBA::Boolean
    Adapter_Activator::unknown_adapter (PortableServer::POA_ptr parent,
                                        const char *name
                                        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      // Default policies
      CORBA::PolicyList default_policies;

      // This assumes that the lock on the parent is recursive
      PortableServer::POA_var child =
        parent->create_POA (name,
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

  }
}
