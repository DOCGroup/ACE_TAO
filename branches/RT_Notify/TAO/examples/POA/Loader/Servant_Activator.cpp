// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO/examples/POA/Loader
//
// = FILENAME
//     Servant_Activator.cpp
//
// = DESCRIPTION
//     Implementation of <ServantActivator_i>, which is used by a POA
//     with a RETAIN policy.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "Servant_Activator.h"

ACE_RCSID(Loader, Servant_Activator, "$Id$")

// Initialization.The dllname is used by the Loactor to load it into
// memory. The factory function is the point of entry into the dll and
// is used for obtaining the servant. The garbage_collection_function
// is used to kill the servant.

ServantActivator_i::ServantActivator_i (CORBA::ORB_ptr orb,
                                        const char *dllname,
                                        const char *factory_function,
                                        const char *garbage_collection_function)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  // The dll is opened using the dllname passed.
   if (this->dll_.open (dllname) == -1)
     ACE_ERROR ((LM_ERROR,
                 "%p\n",
                 this->dll_.error ()));

  // Obtain the symbol for the function that will get the servant
  // object.

  // Cannot go from void* to function pointer directly. Cast the void*
  // to long first.
  char *function_name = ACE::ldname (factory_function);
  void *symbol = this->dll_.symbol (function_name);
  long function = ACE_reinterpret_cast (long, symbol);
  delete [] function_name;

  servant_supplier_ =
    ACE_reinterpret_cast (SERVANT_FACTORY, function);

  // Obtain the symbol for the function which will destroy the
  // servant.
  function_name = ACE::ldname (garbage_collection_function);
  symbol = this->dll_.symbol (function_name);
  function = ACE_reinterpret_cast (long, symbol);
  delete [] function_name;
  servant_garbage_collector_ =
    ACE_reinterpret_cast (SERVANT_GARBAGE_COLLECTOR, function);
}

// This method associates an servant with the ObjectID.

PortableServer::Servant
ServantActivator_i::incarnate (const PortableServer::ObjectId &oid,
                               PortableServer::POA_ptr poa
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  // Obtain the servant else exception.
  PortableServer::Servant servant =
    (*servant_supplier_) (oid,
                          poa,
                          this->orb_.in ());
  if (servant != 0)
    return servant;
  else
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      0);
}

// This is the method invoked when the object is deactivated or the
// entire POA is is deactivated or destroyed.

void
ServantActivator_i::etherealize (const PortableServer::ObjectId &oid,
                                 PortableServer::POA_ptr poa,
                                 PortableServer::Servant servant,
                                 CORBA::Boolean,
                                 CORBA::Boolean remaining_activations
                                 ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // If there are no remaining activations i.e ObjectIds associated
  // with MyFooServant object, deactivate it by calling the garbage_collection_function.
  // Etheralization happens on POA::destroy() and/or Object::deactivate().

  if (remaining_activations == 0)
    (*servant_garbage_collector_) (oid,
                                   poa,
                                   servant);

}
