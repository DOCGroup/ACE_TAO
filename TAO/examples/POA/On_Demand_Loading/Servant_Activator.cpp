// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO/tests/POA/On_Demand_Loading
//
// = FILENAME
//     Servant_Activator.cpp
//
// = DESCRIPTION
//     Implementation of MyFooServantActivator, which is used by a
//     POA with a RETAIN policy.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "Servant_Activator.h"
#include "MyFooServant.h"

ACE_RCSID(On_Demand_Loading, Servant_Activator, "$Id$")

// Initialization.

MyFooServantActivator::MyFooServantActivator (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

// This method associates an servant with the ObjectID.

PortableServer::Servant
MyFooServantActivator::incarnate (const PortableServer::ObjectId &oid,
                                  PortableServer::POA_ptr poa,
                                  CORBA::Environment &env)
{
  /*#if 0
  return poa->load_servant (oid, poa, env);

  #else*/
  // Convert ObjectId to String.

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);

  // Activate and return the servant else exception.

  PortableServer::Servant servant = this->activate_servant (s.in (),
                                                            poa,
                                                            27);
  //#endif
  if (servant != 0)
    return servant;
  else
    TAO_THROW_ENV_RETURN (CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO), env, 0);
}

// This is the method invoked when the object is deactivated or the
// entire POA is is deactivated or destroyed.

void
MyFooServantActivator::etherealize (const PortableServer::ObjectId &oid,
                                    PortableServer::POA_ptr poa,
                                    PortableServer::Servant servant,
                                    CORBA::Boolean cleanup_in_progress,
                                    CORBA::Boolean remaining_activations,
                                    CORBA::Environment &env)
{
  ACE_UNUSED_ARG (oid);
  ACE_UNUSED_ARG (poa);
  ACE_UNUSED_ARG (cleanup_in_progress);
  ACE_UNUSED_ARG (env);

  // If there are no remaining activations i.e ObjectIds associated
  // with MyFooServant object, deactivate it.  Etheralization happens
  // on POA::destroy() and/or Object::deactivate().

  if (remaining_activations == 0)
    this->deactivate_servant (servant, oid);
}

// This method loads the dynamically linked dll which is the
// servant and returns the servant object which is then used for other
// operations in the dll.

PortableServer::Servant
MyFooServantActivator::activate_servant (const char *str,
                                         PortableServer::POA_ptr poa,
                                         long value)
{
  // The string format is dllname:factory_function that must be
  // parsed.
  parse_string (str); 

  ACE_DLL *dll_;

  // Create the dll object.
  ACE_NEW_RETURN (dll_,
                  ACE_DLL,
                  0);

   PortableServer::ObjectId_var oid =
     PortableServer::string_to_ObjectId (str);

  // Make an HASH_MAP entry by binding the object_id and the dll
  // object associated with it together.
   int result = this->servant_map_.bind (oid.in (), 
                                         dll_);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,                      
                       "%p\n", 
                       "Bind failed"),
                      0);
   
  // Now that the dll name is available we open the dll.
  result = dll_->open (dllname_);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p", dll_->error ()),
                      0);

  // The next step is to obtain the symbol for the function that will
  // create the servant object and return it to us.
  SERVANT_FACTORY servant_creator = ACE_reinterpret_cast
    (SERVANT_FACTORY,
     dll_->symbol (create_symbol_.in ()));

  // Checking whether it is possible to create the servant.
  if (servant_creator == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p", dll_->error ()),
                      0);

  // Now create and return the servant using the <servant_creator>
  // factory function.
  return (*servant_creator) (this->orb_.in (),
                             poa,
                             value);
}

// This method removes the servant and the dll object associated with
// it after it has performed the opening and symbol obtaining
// operations.

void
MyFooServantActivator::deactivate_servant (PortableServer::Servant servant,
                                           const PortableServer::ObjectId &oid)
{
  // The servant is destroyed.
  delete servant;

  // @@ Kirthika, you'll need to clean this stuff up if you don't use
  // dynamic memory allocation for the ACE_DLL stuff.
  ACE_DLL *dll;
  
  // Since the servant is no more the dll object associated with it
  // has to be destroyed too.
  this->servant_map_.unbind (oid,
                             dll);
  delete dll;                          
}

// The objectID is in a format of dll:factory_function which has
// to be parsed and separated into tokens to be used.

void
MyFooServantActivator::parse_string (const char *s)
{
  // The format of the object dll:factory_function.  This string
  // is parsed to obtain the dll name and the function name which
  // will create trhe servant and return it to us.

  // @@ Kirthika, make sure that you first figure out how big the
  // string <s> is using ACE_OS::strlen() and then use dynamic memory
  // allocation to make these copies.  Don't forget to delete this
  // stuff.
  char str[BUFSIZ], func[BUFSIZ], libname [BUFSIZ];
  char at[2];

  at[0]= ':';
  at[1]= '\0';

  ACE_OS::strcpy (func, "");

  // As strtok() puts a NULL in the position after it gives back a
  // token, we make two copies of the input string.
  ACE_OS::strcpy (str, s);

  // The strtok() method returns the string until ':' i.e. the
  // function name.
  ACE_OS::strcpy (libname, ACE_OS::strtok (str, at));

  // Get to ':' and make func point to the next location in the
  // string.
  ACE_OS::strcpy (func, ACE_OS::strchr (s,':') + 1);

  // Assign the respective strings obtained.

  this->dllname_ = CORBA::string_dup (libname);
  this->create_symbol_ = CORBA::string_dup (func);

  ACE_DEBUG ((LM_DEBUG,
              "the servant dll:%s\n the factory_function:%s\n ",
              this->dllname_.in (),
              this->create_symbol_.in ()));
}

// This method returns an ObjectId when given an dll name and the
// factory method to be invoked in the dll.  The format of the
// ObjectId is libname:factory_function.

PortableServer::ObjectId_var
MyFooServantActivator::create_objectId (const char *libname,
                                        const char *factory_function)
{
  // @@ Please make sure that you dynamically allocate <format_string>
  // so that it's guaranteed to be large enough to contain a
  // ACE_OS::strlen (libname) + ACE_OS::strlen (factory_function) +
  // sizeof (':') + 1 (for the terminating '\0').
  char format_string [BUFSIZ];

  ACE_OS::strcpy (format_string, libname);
  ACE_OS::strcat (format_string, ":");
  ACE_OS::strcat (format_string, factory_function);

  // The object ID is created.

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (format_string);

  return oid;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<PortableServer::ObjectId,ACE_DLL*>;
template class ACE_Hash_Map_Manager<PortableServer::ObjectId,ACE_DLL*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base<PortableServer::ObjectId,ACE_DLL*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<PortableServer::ObjectId,ACE_DLL*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<PortableServer::ObjectId,ACE_DLL*, ACE_Null_Mutex>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
 
#pragma instantiate ACE_Hash_Map_Entry<PortableServer::ObjectId,ACE_DLL*,ACE_Null_Mutex> 
#pragma instantiate ACE_Hash_Map_Manager<PortableServer::ObjectId,ACE_DLL*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base<PortableServer::ObjectId,ACE_DLL*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<PortableServer::ObjectId,ACE_DLL*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterato<PortableServer::ObjectId,ACE_DLL*,ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
