// $Id$

#include "helloC.h"
#include "ace/Read_Buffer.h"

int
invoke (CORBA::Object_ptr obj,
        const char *msg
        ACE_ENV_ARG_DECL)
{
  // Narrow
  HelloHome_var hh = HelloHome::_narrow (obj
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1); ;

  if  (CORBA::is_nil (hh.in ()))
    ACE_ERROR_RETURN ((LM_ERROR, "Argument is not a HelloHome reference\n"), -1);

  HelloWorld_var hw = hh->create ();

  if  (CORBA::is_nil (hw.in ()))
    ACE_ERROR_RETURN ((LM_ERROR, "Fail to create a HelloWorld reference\n"), -1);

  CORBA::String_var name = CORBA::string_dup (msg);
  CORBA::String_var hi = hw->sayhello (name
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_INFO, "%s\n", hi.in ()));

  hh->remove_component (hw.in ()
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv ACE_ENV_ARG_PARAMETER);

      // Resolve HomeFinder interface

#if 0
      CORBA::Object_var obj = orb->resolve_initial_references ("HomeFinder"
                                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Components::HomeFinder_var home_finder =
        Components::HomeFinder::_narrow (obj ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if  (CORBA::is_nil (home_finder.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Can't resolve initial HomeFinder reference\n"), -1);

      // Now get the HelloHome

      obj = home_finder->find_home_by_name ("HelloHome"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      invoke (obj.in (),
              "Frodo Baggins using 'find_home_by_name'"
              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = home_finder->find_home_by_home_type
        ("IDL:omg.org/HelloHome:1.0"
         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      invoke (obj.in (),
              "Samwise Gamgee using 'find_home_by_home_type'"
              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = home_finder->find_home_by_component_type
        ("IDL:omg.org/HelloWorld:1.0"
         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      invoke (obj.in (),
              "Meriadoc Brandybuck using 'find_home_by_component_type'"
              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

#else
      CORBA::Object_var obj
        = orb->string_to_object ("file://hello.ior" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      invoke (obj.in (),
              "Meriadoc Brandybuck using 'file://hello.ior'"
              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif


      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Who is the culprit \n");
      cerr << "Uncaught CORBA exception" << endl;
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
