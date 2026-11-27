#include "AppHelper.h"
#include <memory>

void
AppHelper::ref_to_file (CORBA::ORB_ptr orb,
                        CORBA::Object_ptr obj,
                        ACE_TCHAR const *filename)
{
  CORBA::String_var ior = orb->object_to_string (obj);

  std::unique_ptr<FILE, int (*) (FILE *)> ior_file (
    ACE_OS::fopen (filename, "w"), ACE_OS::fclose);

  if (!ior_file)
    {
      ACE_ERROR ((LM_ERROR,
                 "(%P|%t) Cannot open output file [%s] to write IOR.",
                 filename));
      throw TestAppException ();
    }

  ACE_OS::fprintf (ior_file.get (), "%s", ior.in ());
}

PortableServer::POA_ptr
AppHelper::create_poa (char const *name,
                       PortableServer::POA_ptr root_poa,
                       PortableServer::POAManager_ptr mgr,
                       CORBA::PolicyList &policies)
{
  PortableServer::POA_var child_poa = root_poa->create_POA (name,
                                                             mgr,
                                                             policies);

  if (CORBA::is_nil (child_poa.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Failed to create child POA: %s.\n", name));
      throw TestAppException ();
    }

  return child_poa._retn ();
}

CORBA::Object_ptr
AppHelper::activate_servant (PortableServer::POA_ptr poa,
                             PortableServer::Servant servant)
{
  // Activate the servant using the Child POA.
  PortableServer::ObjectId_var oid
    = poa->activate_object (servant);

  CORBA::Object_var obj
    = poa->servant_to_reference (servant);

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                 "(%P|%t) Failed to convert servant_to_ref.\n"));
      throw TestAppException ();
    }

  return obj._retn ();
}

bool
AppHelper::validate_connection (CORBA::Object_ptr obj)
{
  for (CORBA::ULong j = 0; j != 100; ++j)
    {
      try
        {
#if (TAO_HAS_CORBA_MESSAGING == 1)
          CORBA::PolicyList_var unused;
          obj->_validate_connection (unused);
#else
          obj->_is_a ("Not_An_IDL_Type");
#endif /* TAO_HAS_MESSAGING == 1 */
          return true;
        }
      catch (CORBA::Exception const &)
        {
        }
    }

  return false;
}
