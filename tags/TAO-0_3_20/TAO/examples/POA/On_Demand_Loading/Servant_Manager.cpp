// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO/examples/POA/On_Demand_Loading
//
// = FILENAME
//     Servant_Manager.cpp
//
// = DESCRIPTION
//     Implementation of the helper class for the ServantActivator_i
//     and the ServantLocator_i.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "Servant_Manager.h"

ACE_RCSID(On_Demand_Activation, Servant_Manager, "$Id$")

// Initialization.
ServantManager_i::ServantManager_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Destruction.
ServantManager_i::~ServantManager_i (void)
{
}

// This method loads the dynamically linked DLL which is the servant
// and returns the servant object which is then used for other
// operations in the DLL.

PortableServer::Servant
ServantManager_i::obtain_servant (const char *str,
                                  PortableServer::POA_ptr poa,
                                  long value)
{
  // The string format is <dllname:factory_function> that must be
  // parsed.
  this->parse_string (str);

  // Create the DLL object.
  ACE_DLL *dll;

  ACE_NEW_RETURN (dll,
                  ACE_DLL,
                  0);

  // Obtain the ObjectId from the string argument.

   PortableServer::ObjectId_var oid =
     PortableServer::string_to_ObjectId (str);

   ACE_DEBUG ((LM_DEBUG,
               "before bind\n"));
  // Make an HASH_MAP entry by binding the object_id and the DLL
  // object associated with it together.
   if (this->servant_map_.bind (oid.in (),
                                dll) == -1)
     ACE_ERROR_RETURN ((LM_ERROR,
                        "%p\n",
                        "Bind failed"),
                       0);
   // Now that the dll name is available we open the dll.
   if (dll->open (dllname_.c_str ()) == -1)
     ACE_ERROR_RETURN ((LM_ERROR,
                        "%p",
                        dll->error ()),
                       0);

   // The next step is to obtain the symbol for the function that will
  // create the servant object and return it.
  SERVANT_FACTORY servant_creator =
    (SERVANT_FACTORY) dll->symbol (create_symbol_.c_str ());

  // Checking whether it is possible to create the servant.
  if (servant_creator == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p",
                       dll->error ()),
                      0);
  // Now create and return the servant using the <servant_creator>
  // factory function.
  return (*servant_creator) (this->orb_.in (),
                             poa,
                             value);
}

// The objectID is in a format of dllname:factory_function which has
// to be parsed and separated into tokens to be used.

void
ServantManager_i::parse_string (const char *s)
{
  // The format of the objectid is <dll:factory_function>.  This
  // string is parsed to obtain the dll name and the function name
  // which will create trhe servant and return it to us.

  ACE_CString str (s);

  int index = str.find (':');
  // On error, npos is returned.
  if (index == ACE_CString::npos)
    ACE_ERROR ((LM_ERROR,
                "Required character absent!\n"));

  // The index gives us the location which is equivalent to the size
  // of the dllname_ string.
  this->dllname_ = str.substr (0, index);

  // Obtain the substring from the offset which is one greater than
  // the location of ':'.
  this->create_symbol_ = str.substr (index + 1);

  ACE_DEBUG ((LM_DEBUG,
              "the servant dll:%s\n the factory_function:%s\n ",
              this->dllname_.c_str (),
              this->create_symbol_.c_str ()));
}

// This method returns an ObjectId when given a DLL name and the
// factory function to be invoked in the DLL.  The format of the
// ObjectId is libname:factory_function.

PortableServer::ObjectId_var
ServantManager_i::create_dll_object_id (const char *libname,
                                        const char *factory_function)
{
  ACE_CString format_string = libname;
  format_string += ":";
  format_string += factory_function;

  ACE_DEBUG ((LM_DEBUG,
              "format-string is %s\n",
              format_string.c_str ()));
  // The object ID is created.
  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (format_string.c_str ());
  return oid;
}

// This method destroys the servant and its caretaking DLL object.

void
ServantManager_i::destroy_servant (PortableServer::Servant servant,
                                   const PortableServer::ObjectId &oid)
{
  // The servant is destroyed.
  delete servant;

  // Since the servant is no more the DLL object associated with it
  // has to be destroyed too.

  ACE_DLL *dll = 0;

  // Since the servant is no more the DLL object associated with it
  // has to be destroyed too.

  if (this->servant_map_.unbind (oid,
                                 dll) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "Unbind failed!\n"));
  delete dll;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<PortableServer::ObjectId, ACE_DLL *>;
template class ACE_Equal_To<PortableServer::ObjectId>;
template class ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId, ACE_DLL *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectId, ACE_DLL *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectId, ACE_DLL *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectId, ACE_DLL *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<PortableServer::ObjectId, ACE_DLL *>
#pragma instantiate ACE_Equal_To<PortableServer::ObjectId>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId, ACE_DLL *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectId, ACE_DLL *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectId, ACE_DLL *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectId, ACE_DLL *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
