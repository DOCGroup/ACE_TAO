//
// $Id$
//

#include "orbsvcs/CosNamingC.h"
#include "tao/corba.h"

int main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var ns_obj =
        orb->resolve_initial_references ("NameService" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (ns_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Could not resolve Naming Service"),
                          1);

      CosNaming::NamingContext_var inc =
        CosNaming::NamingContext::_narrow (ns_obj.in ()
                                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (inc.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Could not resolve Naming Service"),
                          1);

      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("yourself");

      inc->bind (name, ns_obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Test Successful\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Test");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
