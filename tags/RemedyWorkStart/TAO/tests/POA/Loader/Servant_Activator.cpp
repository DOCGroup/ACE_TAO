
//=============================================================================
/**
 *  @file     Servant_Activator.cpp
 *
 *  $Id$
 *
 *   Implementation of <ServantActivator>, which is used by a POA
 *   with a RETAIN policy.
 *
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "Servant_Activator.h"
#include "ace/OS_NS_string.h"

// Initialization.The dllname is used by the Loactor to load it into
// memory. The factory function is the point of entry into the dll and
// is used for obtaining the servant. The garbage_collection_function
// is used to kill the servant.

ServantActivator::ServantActivator (CORBA::ORB_ptr orb,
                                    const ACE_TCHAR *dllname,
                                    const ACE_TCHAR *factory_function,
                                    const ACE_TCHAR *garbage_collection_function)
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
  void *symbol = this->dll_.symbol (factory_function);
#if defined (ACE_OPENVMS) && (!defined (__INITIAL_POINTER_SIZE) || (__INITIAL_POINTER_SIZE < 64))
  int function = reinterpret_cast<int> (symbol);
#else
  intptr_t function = reinterpret_cast<intptr_t> (symbol);
#endif

  servant_supplier_ =
    reinterpret_cast<SERVANT_FACTORY> (function);

  // Obtain the symbol for the function which will destroy the
  // servant.
  symbol = this->dll_.symbol (garbage_collection_function);
#if defined (ACE_OPENVMS) && (!defined (__INITIAL_POINTER_SIZE) || (__INITIAL_POINTER_SIZE < 64))
  function = reinterpret_cast<int> (symbol);
#else
  function = reinterpret_cast<intptr_t> (symbol);
#endif
  servant_garbage_collector_ =
    reinterpret_cast<SERVANT_GARBAGE_COLLECTOR> (function);
}

// This method associates an servant with the ObjectID.

PortableServer::Servant
ServantActivator::incarnate (const PortableServer::ObjectId &oid,
                             PortableServer::POA_ptr poa)
{
  // Obtain the servant else exception.
  PortableServer::Servant servant =
    (*servant_supplier_) (oid,
                          poa,
                          this->orb_.in ());
  if (servant != 0)
    return servant;
  else
    throw CORBA::OBJECT_NOT_EXIST ();
}

// This is the method invoked when the object is deactivated or the
// entire POA is is deactivated or destroyed.

void
ServantActivator::etherealize (const PortableServer::ObjectId &oid,
                               PortableServer::POA_ptr poa,
                               PortableServer::Servant servant,
                               CORBA::Boolean,
                               CORBA::Boolean remaining_activations)
{
  // If there are no remaining activations i.e ObjectIds associated
  // with test servant, deactivate it by calling the
  // garbage_collection_function.  Etheralization happens on
  // POA::destroy() and/or Object::deactivate().

  if (remaining_activations == 0)
    (*servant_garbage_collector_) (oid,
                                   poa,
                                   servant);

}
