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
//     Defines a MyFooServantActivator class, an Servant_Manager interface which
//     activates a servant by loading it and associates it with an object on demand.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//==================================================================================

#ifndef SERVANT_ACTIVATOR_H
#define SERVANT_ACTIVATOR_H

#include "tao/corba.h"
#include "ace/DLL.h"
#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @@ Kirthika and Irfan, do you think we should add the new
// create_dll_object_id() method to the ServantActivator base class or
// should it be provided as a subclass provided by TAO, e.g.,
// TAO_DLL_ServantActivator.
class MyFooServantActivator : public POA_PortableServer::ServantActivator
{
  //= TITLE
  //   Servant Activator for the MyFoo servant.
  //
  //= DESCRIPTION
  //   This class associates an unassociated servant with an object in
  //   the POA Active Object Map.
public:

  /*#if 0
  typedef PortableServer::Servant (*SERVANT_FACTORY) (const PortableServer:ObjectID &oid);
  #else*/
  typedef PortableServer::Servant (*SERVANT_FACTORY) (CORBA::ORB_ptr orb,
                                                      PortableServer::POA_ptr poa,
                                                      CORBA::Long value);
  //#endif
  // This typedef is used to typecast the void* obtained when finding
  // a symbol in the DLL.

  MyFooServantActivator (CORBA::ORB_ptr orb);
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

  // @@ Please rename this as per our discussion.
  virtual PortableServer::ObjectId_var create_objectId (const char *libname,
                                                        const char *factory_method);
  // Returns an ObjectId when given a DLL name and the factory
  // function in the DLL that will create an appropriate Servant.

private:
  PortableServer::Servant activate_servant (const char *str,
                                            PortableServer::POA_ptr poa,
                                            long value);
  // @@ *done*Kirthika, can you please change dll to DLL globally?
  // Gets the servant on activation by loading the appropriate dll
  // and getting the servant object.
  // @@ Kirthika, can you please explain what the various arguments mean?!

  void deactivate_servant (PortableServer::Servant servant,
                           const PortableServer::ObjectId &oid);
  // The servant is killed and care is taken to close the dll
  // loaded.

  void parse_string (const char* s);
  // Parse the string to obtain the dll name and the symbol which
  // will get us the servant pointer.

  
  CORBA::ORB_var orb_;
  // A reference to the ORB.

  CORBA::String_var dllname_;
  // The name of the dll containing the servant.

  CORBA::String_var create_symbol_;
  // The symbol which on getting invoked will give us the servant
  // pointer.

  // @@ Kirthika, can you please make this be a map of <ACE_DLL> and
  // NOT <ACE_DLL *>?  I think we should try to avoid dynamic memory
  // allocation unless it's absolutely necessary.

  typedef ACE_Hash_Map_Manager<PortableServer::ObjectId,
                               ACE_DLL *,
                               ACE_Null_Mutex>
          SERVANT_MAP;

  SERVANT_MAP servant_map_;
  // This is the hash map object.
  
};

#endif /* SERVANT_ACTIVATOR_H */
