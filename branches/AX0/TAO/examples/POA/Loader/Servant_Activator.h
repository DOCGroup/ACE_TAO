// This may look like C, but it's really -*- C++ -*-
// $Id$

// ================================================================
//
// = LIBRARY
//     TAO/examples/POA/On_Demand_Loading
//
// = FILENAME
//     Servant_Activator.h
//
// = DESCRIPTION
//     Defines a <ServantActivator_i> class, which activates a servant by
//     obtaining it and associates it with an object on-demand.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ================================================================

#include "ace/OS.h"

#ifndef SERVANT_ACTIVATOR_H
#define SERVANT_ACTIVATOR_H

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/DLL.h"

class ServantActivator_i : public PortableServer::ServantActivator
{
  //= TITLE
  //   Servant Activator for the MyFoo servant.
  //
  //= DESCRIPTION
  //   This class associates an unassociated servant with an object in
  //   the POA Active Object Map.
public:
  typedef PortableServer::Servant
           (*SERVANT_FACTORY) (const PortableServer::ObjectId &oid,
                               PortableServer::POA_ptr poa,
                               CORBA::ORB_ptr orb);
  // This typedef is used to typecast the void* obtained when finding
  // a symbol in the dll. Invoking the function pointer obtained would
  // get a servant.

  typedef void
          (*SERVANT_GARBAGE_COLLECTOR) (const PortableServer::ObjectId &oid,
                                        PortableServer::POA_ptr,
                                        PortableServer::Servant servant);
  // This typedef is used to obtain the garbage_collection_function symbol
  // in the dll. Invoking the function pointer obtained would then destroy
  // the servant.

  ServantActivator_i (CORBA::ORB_ptr orb,
                      const char *dllname,
                      const char *factory_function,
                      const char *garbage_collection_function);
  // Initialization.

  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr poa
                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));
  // This method is invoked by a POA with USE_SERVANT_MANAGER and
  // RETAIN policies, whenever it receives a request for a
  // MyFooServant object that is not currently active. When an servant
  // pointer corresponding to objectId is not found in the Active
  // Object Map, the POA hands over the job of obtaining the servant
  // to the Servant Manager. Depending upon whether the POA is created
  // with RETAIN or NON_RETAIN as the servant_retention policy, the
  // Servant Activator or the Servant Locator interface is invoked
  // respectively.

  virtual void etherealize (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr adapter,
                            PortableServer::Servant servant,
                            CORBA::Boolean cleanup_in_progress,
                            CORBA::Boolean remaining_activations
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This method is invoked whenever a MyFooServant for a MyFoo object
  // is deactivated. This occurs when the POA is destroyed or the
  // Object is deactivated. When the POA is getting destroyed, it
  // needs to deactivate every  object in the Active Object Map and on
  // that call the  ServantActivator invokes this method which will
  // destroy the servant associated with the object.

private:
  ACE_DLL dll_;
  // The ACE_DLL object which performs the task of loading the dll
  // and accessing it.

  SERVANT_FACTORY servant_supplier_;
  // The function pointer of factory_function type.

  SERVANT_GARBAGE_COLLECTOR servant_garbage_collector_;
  // The function pointer of garbage_collection_function type.

  CORBA::ORB_var orb_;
  // A reference to the ORB.
};

#endif /* SERVANT_ACTIVATOR_H */
