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
//     Defines a <ServantActivator_i> class, which uses a
//     Servant_Manager to activate a servant by loading it and
//     associates it with an object on-demand.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ================================================================

#ifndef SERVANT_ACTIVATOR_H
#define SERVANT_ACTIVATOR_H

#include "Servant_Manager.h"

class ServantActivator_i : public PortableServer::ServantActivator
{
  //= TITLE
  //   Servant Activator for the MyFoo servant.
  //
  //= DESCRIPTION
  //   This class associates an unassociated servant with an object in
  //   the POA Active Object Map.
public:
  ServantActivator_i (CORBA::ORB_ptr orb);
  // Initialization.

  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr poa
                                             TAO_ENV_ARG_DECL)
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
                            TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This method is invoked whenever a MyFooServant for a MyFoo object
  // is deactivated. This occurs when the POA is destroyed or the
  // Object is deactivated. When the POA is getting destroyed, it
  // needs to deactivate every  object in the Active Object Map and on
  // that call the  ServantActivator invokes this method which will
  // destroy the servant associated with the object.

  PortableServer::ObjectId_var create_dll_object_id (const char *dllname,
                                                     const char *factory_function);
  // Returns an ObjectId when given a DLL name and the factory
  // function in the DLL that will create an appropriate Servant.

private:
  ServantManager_i servant_manager_;
  // An ServantManager object that provides utility methods.  The
  // methods include obtaining the servant using an ACE_DLL object,
  // destroying the servant and extracting the dllname and factory
  // function from the ObjectId.
};

#endif /* SERVANT_ACTIVATOR_H */
