// This may look like C, but it's really -*- C++ -*-
// $Id$

//================================================================================
//
// = LIBRARY
//     TAO/examples/POA/On_Demand_Loading
//
// = FILENAME
//     Servant_Manager.h
//
// = DESCRIPTION
//     Helper class for <ServantActivator_i> and <ServantLoactor_i>.
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
  //   ServantLocator_i classes.
  //
  // = DESCRIPTION
  //   The methods provided by this class are used by the ServantActivator_i
  //   and ServantLocator_i classes. This class contains the common methods
  //   needed by them.
  //  
public:
   typedef PortableServer::Servant 
           (*SERVANT_FACTORY) (CORBA::ORB_ptr orb,
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
  // Returns an ObjectId when given an DLL name and the factory method
  // to be invoked in the DLL. The application developer can initialise the 
  // ServantActivator object by providing the dllname and the factory function.

  // @@ *done*Kirthika, please explain what this function is USED for, i.e.,
  // who calls it and why?
  
  PortableServer::Servant obtain_servant (const char *str,
                                          PortableServer::POA_ptr poa,
                                          long value);
  // Obtains a servant on activation by linking and loading the
  // appropriate DLL and creating the servant object.  The <str>
  // argument is the ObjectId that contains the servant DLL name and
  // the factory function name. The <long> argument is an
  // servant-specific argument needed to create the servant for this
  // particular use-case.
 
  void destroy_servant (PortableServer::Servant servant,
                        const PortableServer::ObjectId &oid);
  // The servant is destroyed and the DLL that was dynamically linked
  // is closed.

 private:
  void parse_string (const char *s);
  // Parse the string to obtain the DLL name and the factory function
  // symbol that we will used to dynamically obtain the servant
  // pointer.
  
  CORBA::ORB_var orb_;
  // A reference to the ORB.
  
  ACE_CString dllname_;
  // The name of the dll containing the servant.
  
  ACE_CString create_symbol_;
  // The symbol which on getting invoked will give us the servant
  // pointer.
  
  typedef ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId,
                                  ACE_DLL *,
                                  TAO_ObjectId_Hash,
                                  ACE_Equal_To<PortableServer::ObjectId>,
                                  ACE_Null_Mutex>
          SERVANT_MAP;
  
  SERVANT_MAP servant_map_;
  // This is the hash map object. The hash map is used to provide
  // an quick access to the dll object associated with every servant
  // using the unique ObjectId as key.
  
};
#endif /* SERVANT_MANAGER_H */

