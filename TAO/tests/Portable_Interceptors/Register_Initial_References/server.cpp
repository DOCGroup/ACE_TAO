// $Id$

#include "Server_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "test_i.h"

ACE_RCSID(Recursive_ORBInitializer,
          server,
          "$Id$")


int test_orb (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  int errors = 0;

  POA_TestModule::test* test = 0;
  ACE_NEW_RETURN (test,
                  test_i, 1);

  CORBA::Object_ptr object = test->_this ();

  orb->register_initial_reference ("ORBMyService",
                                    object
                                    ACE_ENV_ARG_PARAMETER);

  bool invalid_name = false;
  ACE_TRY
    {
      // Registering with an empty string should give an exception
      orb->register_initial_reference ("",
                                       object
                                       ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCH (CORBA::ORB::InvalidName, ex)
    {
      invalid_name = true;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  if (!invalid_name)
    {
      errors++;
      ACE_ERROR ((LM_ERROR, "ERROR: Registering with an empty string with the ORB"
                            "doesn't throw an exception\n"));
    }

  bool duplicate_name = false;
  ACE_TRY_EX (duplicate)
    {
      // Registering with an duplicate string should give an exception
      orb->register_initial_reference ("ORBMyService",
                                        object
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (duplicate);
    }
  ACE_CATCH (CORBA::ORB::InvalidName, ex)
    {
      duplicate_name = true;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  if (!duplicate_name)
    {
      errors++;
      ACE_ERROR ((LM_ERROR, "ERROR: Registering with a duplicate with ORB "
                            "doesn't throw the expected exception\n"));
    }

  bool invalid_object = false;
  ACE_TRY_EX (invalid)
    {
      // Registering with a nil object
      orb->register_initial_reference ("ORBNilServer",
                                       CORBA::Object::_nil()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (invalid);
    }
  ACE_CATCH (CORBA::BAD_PARAM , ex)
    {
      if ((ex.minor() & 0xFFFU) == 27)
        {
          invalid_object = true;
        }
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  if (!invalid_object)
    {
      errors++;
      ACE_ERROR ((LM_ERROR, "ERROR: Registering with a nil object to ORB "
                            "doesn't throw bad param with minor code 27\n"));
    }

  return errors;
}

int
main (int argc, char *argv[])
{
  Server_ORBInitializer *initializer = 0;
  int retval = 0;

  ACE_NEW_RETURN (initializer,
                  Server_ORBInitializer,
                  -1);  // No exceptions yet!

  PortableInterceptor::ORBInitializer_var initializer_var =
    initializer;

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      retval = test_orb (orb.in () ACE_ENV_ARG_PARAMETER);

      PortableInterceptor::register_orb_initializer (initializer_var.in ()
                                                     ACE_ENV_ARG_PARAMETER);

      CORBA::ORB_var second_orb =
        CORBA::ORB_init (argc, argv, "SecondORB" ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception in server:");
      return 1;
    }
  ACE_ENDTRY;

  return retval;
}
