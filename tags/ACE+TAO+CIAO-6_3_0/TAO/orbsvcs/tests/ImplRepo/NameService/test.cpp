// $Id$

#include "orbsvcs/CosNamingC.h"
#include "ace/Log_Msg.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var ns_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (ns_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Could not resolve Naming Service"),
                          1);

      CosNaming::NamingContext_var inc =
        CosNaming::NamingContext::_narrow (ns_obj.in ());

      if (CORBA::is_nil (inc.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Could not resolve Naming Service"),
                          1);

      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("yourself");

      inc->bind (name, ns_obj.in ());

      ACE_DEBUG ((LM_DEBUG, "Test Successful\n"));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test");
      return 1;
    }

  return 0;
}
