
//=============================================================================
/**
 *  @file     Servant_Manager.cpp
 *
 *   Implementation of the helper class for the ServantActivator_i
 *   and the ServantLocator_i.
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================

#include "Servant_Manager.h"
#include "tao/debug.h"

#include <memory>

// Initialization.
ServantManager_i::ServantManager_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Destruction.
ServantManager_i::~ServantManager_i ()
{
  SERVANT_MAP::iterator const end = this->servant_map_.end ();
  for (SERVANT_MAP::iterator iter = this->servant_map_.begin ();
       iter != end;
       ++iter)
    delete (*iter).int_id_;
  this->servant_map_.unbind_all ();
}

// This method loads the dynamically linked DLL which is the servant
// and returns the servant object which is then used for other
// operations in the DLL.

PortableServer::Servant
ServantManager_i::obtain_servant (ACE_TCHAR const *str,
                                  PortableServer::POA_ptr poa)
{
  ACE_TString dllname;
  ACE_TString create_symbol;
  if (!this->parse_string (str, dllname, create_symbol))
    return nullptr;

  ACE_DLL *raw_dll {};
  ACE_NEW_RETURN (raw_dll, ACE_DLL, nullptr);
  std::unique_ptr<ACE_DLL> dll (raw_dll);

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (ACE_TEXT_ALWAYS_CHAR (str));

  // Open the DLL before registering it in the map.
  if (dll->open (dllname.c_str ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "%p", dll->error ()));
      return nullptr;
    }

  // The next step is to obtain the symbol for the function that will
  // create the servant object and return it.

  // Cannot go from void* to function pointer directly. Cast the void*
  // to long first.
  void * const symbol = dll->symbol (create_symbol.c_str ());
  intptr_t const function = reinterpret_cast<intptr_t> (symbol);

  SERVANT_FACTORY const servant_creator =
    reinterpret_cast<SERVANT_FACTORY> (function);

  // Checking whether it is possible to create the servant.
  if (servant_creator == nullptr)
    {
      ACE_ERROR ((LM_ERROR, "%p", dll->error ()));
      return nullptr;
    }

  // Now create and return the servant using the <servant_creator>
  // factory function.
  PortableServer::Servant const servant = (*servant_creator) (this->orb_.in (), poa);

  if (servant == nullptr)
    return nullptr;

  std::unique_ptr<PortableServer::ServantBase> servant_owner (servant);
  if (this->servant_map_.bind (oid.in (), dll.get ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "%p\n", "Bind failed"));
      return nullptr;
    }

  // The servant map owns the DLL while the returned servant is alive.
  dll.release ();
  servant_owner.release ();
  return servant;
}

// The objectID is in a format of dllname:factory_function which has
// to be parsed and separated into tokens to be used.

bool
ServantManager_i::parse_string (ACE_TCHAR const *s,
                                ACE_TString &dllname,
                                ACE_TString &create_symbol) const
{
  if (s == nullptr)
    return false;

  // The format of the objectid is <dll:factory_function>.  This
  // string is parsed to obtain the dll name and the function name
  // which will create trhe servant and return it to us.

  ACE_TString const str (s);

  ACE_TString::size_type const index = str.find (':');
  // On error, npos is returned.
  if (index == ACE_CString::npos)
    {
      ACE_ERROR ((LM_ERROR, "Required character absent in servant object id\n"));
      return false;
    }

  // The index gives us the location which is equivalent to the size
  // of the dllname_ string.
  dllname = str.substr (0, index);

  // Obtain the substring from the offset which is one greater than
  // the location of ':'.
  create_symbol = str.substr (index + 1);
  if (dllname.empty () || create_symbol.empty ())
    return false;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "the servant dll:%s\n the factory_function:%s\n",
                dllname.c_str (),
                create_symbol.c_str ()));
  return true;
}

// This method returns an ObjectId when given a DLL name and the
// factory function to be invoked in the DLL.  The format of the
// ObjectId is libname:factory_function.

PortableServer::ObjectId_var
ServantManager_i::create_dll_object_id (char const *libname,
                                        char const *factory_function)
{
  ACE_CString format_string = libname;
  format_string += ":";
  format_string += factory_function;

  ACE_DEBUG ((LM_DEBUG,
              "format-string is %C\n",
              format_string.c_str ()));
  // The object ID is created.
  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (format_string.c_str ());
  return oid;
}

// This method destroys the servant and its caretaking DLL object.

void
ServantManager_i::destroy_servant (PortableServer::Servant servant,
                                   PortableServer::ObjectId const &oid)
{
  std::unique_ptr<PortableServer::ServantBase> servant_owner (servant);
  ACE_DLL *raw_dll {};
  if (this->servant_map_.unbind (oid, raw_dll) == -1)
    {
      ACE_ERROR ((LM_ERROR, "%p\n", "Unbind failed!\n"));
      return;
    }

  std::unique_ptr<ACE_DLL> dll_owner (raw_dll);
  servant_owner.reset ();
  dll_owner.reset ();
}

