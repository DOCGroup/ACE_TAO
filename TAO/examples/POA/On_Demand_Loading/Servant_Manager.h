// $Id$

//================================================================================
//
// = LIBRARY
//     TAO/tests/POA/On_Demand_Loading
//
// = FILENAME
//     Servant_Manager.h
//
// = DESCRIPTION
//     Helper class for ServantActivator and ServantLoactor.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//==================================================================================

#include "ace/OS.h"

#ifndef SERVANT_MANAGER_H
#define SERVANT_MANAGER_H

#include "ace/DLL.h"
#include "ace/Containers.h"
#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ServantManager_i
{
  // = TITLE
  //   This class is the helper class for the ServantActivator_i and
  //   ServantLocator_i classes, both of which inherit from this class
  //   and share its functions.
public:
   typedef PortableServer::Servant (*SERVANT_FACTORY) (CORBA::ORB_ptr orb,
                                                      PortableServer::POA_ptr poa,
                                                      CORBA::Long value);
  // This typedef is used to typecast the void* obtained when finding
  // a symbol in the DLL.

  ServantManager_i (CORBA::ORB_ptr orb);
  // Initialization.

   ~ServantManager_i (void);
  // Destruction.

  PortableServer::ObjectId_var create_dll_object_id (const char *libname, 
                                                     const char *factory_function);
  // Returns an ObjectId when given an dll name and the factory method
  // to be invoked in the dll.
  
  void parse_string (const char *s);
  // Parse the string to obtain the dll name and the symbol which will
  // get us the servant pointer.
  
  PortableServer::Servant obtain_servant (const char *str,
                                          PortableServer::POA_ptr poa,
                                          long value);
  // Gets the servant on activation by loading the appropriate dll and
  // getting the servant object. The str argument is the ObjectId
  // which contains the servant dll name as well as the factory
  // function. The long argument is an argument needed to create the
  // servant.
 
  void destroy_servant (PortableServer::Servant servant,
                        const PortableServer::ObjectId &oid);
  // The servant is destroyed and the dll that was dynamically linked
  // is closed.

  CORBA::ORB_var orb_;
  // A reference to the ORB.
  
  CORBA::String_var dllname_;
  // The name of the dll containing the servant.
  
  CORBA::String_var create_symbol_;
  // The symbol which on getting invoked will give us the servant
  // pointer.
  
  typedef ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId,
                                  ACE_DLL *,
                                  TAO_ObjectId_Hash,
                                  ACE_Equal_To<PortableServer::ObjectId>,
                                  ACE_Null_Mutex>
          SERVANT_MAP;
  
  SERVANT_MAP servant_map_;
  // This is the hash map object.

};
#endif /* SERVANT_MANAGER_H */

