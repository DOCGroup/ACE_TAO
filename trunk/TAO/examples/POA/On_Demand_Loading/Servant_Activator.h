// $Id$

//=================================================================================
//
// = LIBRARY
//     TAO/tests/POA/On_Demand_Loading
//
// = FILENAME
//     Servant_Activator.h
//
// = DESCRIPTION
//     Defines a ServantActivator_i class, an Servant_Manager interface which
//     activates a servant by loading it and associates it with an object on demand.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//==================================================================================

#ifndef SERVANT_ACTIVATOR_H
#define SERVANT_ACTIVATOR_H

#include "Servant_Manager.h"

class ServantActivator_i : public POA_PortableServer::ServantActivator
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
                                             PortableServer::POA_ptr poa,
                                             CORBA::Environment &env);
  // This method is invoked by a POA with USE_SERVANT_MANAGER and
  // RETAIN policies, whenever it receives a request for a
  // MyFooServant object that is not currently active.

  virtual void etherealize (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr adapter,
                            PortableServer::Servant servant,
                            CORBA::Boolean cleanup_in_progress,
                            CORBA::Boolean remaining_activations,
                            CORBA::Environment &env);
  // This method is invoked whenever a MyFooServant for a MyFoo object
  // is deactivated.

  PortableServer::ObjectId_var create_dll_object_id (const char *dllname,
                                                     const char *factory_function);
  // Returns an ObjectId when given a DLL name and the factory
  // function in the DLL that will create an appropriate Servant.

private:
  ServantManager_i servant_manager_;
  // An ServantManager object which provides utility methods.
};

#endif /* SERVANT_ACTIVATOR_H */
