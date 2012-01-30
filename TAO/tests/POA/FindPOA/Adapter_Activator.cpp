// -*- C++ -*-
// $Id$

#include "Adapter_Activator.h"

#if (TAO_HAS_MINIMUM_POA == 0)
#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/AdapterActivatorC.h"
#include "tao/PortableServer/POAManagerC.h"
#include "tao/PortableServer/PortableServer.h"

namespace TAO
{
  namespace Portable_Server
  {
    Adapter_Activator::Adapter_Activator (
      PortableServer::POAManager_ptr pm)
      : poa_manager_ (PortableServer::POAManager::_duplicate (pm))
    {
    }

    CORBA::Boolean
    Adapter_Activator::unknown_adapter (PortableServer::POA_ptr parent,
                                        const char *name)
    {
      // Default policies
      CORBA::PolicyList default_policies;

      // This assumes that the lock on the parent is recursive
      PortableServer::POA_var child =
        parent->create_POA (name, this->poa_manager_.in (), default_policies);

      try
        {
          child->the_activator (this);
        }
      catch (const ::CORBA::Exception&)
        {
          (void) child->destroy (1, 1);

          return false;
        }

      // Finally everything is fine
      return true;
    }
  }
}

#endif
#endif /* TAO_HAS_MINIMUM_POA == 0 */
