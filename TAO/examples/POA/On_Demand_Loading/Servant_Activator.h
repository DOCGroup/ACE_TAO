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
//     Defines a Dir_Service_Activator class, an Servant_Manager interface which 
//     activates a servant by loading it and associates it with an object on demand.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//==================================================================================

#include "tao/corba.h"
#include "ace/DLL.h"


class Dir_Service_Activator : public POA_PortableServer::ServantActivator
{
  //= TITLE
  //   Servant Activator for the directory service servant.
  //
  //= DESCRIPTION
  //   This class associates an unassociated servant with an object in
  //   the POA Active Object Map.

public:

  typedef PortableServer::Servant (*Servant_Creator_Prototype) (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);
  // This typedef is used to typecast the void* obtained on finding a 
  // symbol in the library.

  Dir_Service_Activator (CORBA::ORB_ptr orb);
  // Initialization.

  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr poa,
                                             CORBA::Environment &env);
  // This method is invoked by a POA with USE_SERVANT_MANAGER and
  // RETAIN policies, whenever it receives a request for a Dir_Service
  // object that is not currently active.

  virtual void etherealize (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr adapter,
                            PortableServer::Servant servant,
                            CORBA::Boolean cleanup_in_progress,
                            CORBA::Boolean remaining_activations,
                            CORBA::Environment &env);
  // This method is invoked whenever a MyFooServant for a MyFoo object
  // is deactivated.

private:

   PortableServer::Servant activate_servant (const char *str,
                                             PortableServer::POA_ptr poa);
  // Gets the servant on activation by loading the appropriate library
  // and getting the servant object.

  void deactivate_servant (PortableServer::Servant servant);
  // The servant is killed and care is taken to close the library
  // loaded.

  void parse_string (const char* s);
  // Parse the string to obtain the library name and the symbol which
  // will get us the servant pointer.

  CORBA::ORB_var orb_;
  // A reference to the ORB.

  CORBA::String_var dllname_;
  // The name of the library containing the servant.

  CORBA::String_var create_symbol_;
  // The symbol which on getting invoked will give us the servant
  // pointer.

  ACE_DLL dll_;
  // The library object.
};
