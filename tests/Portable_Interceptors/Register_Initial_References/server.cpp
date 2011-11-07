// $Id$

#include "Server_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "test_i.h"

int test_orb (CORBA::ORB_ptr orb)
{
  int errors = 0;

  POA_TestModule::test* test = 0;
  ACE_NEW_RETURN (test,
                  test_i, 1);
  PortableServer::ServantBase_var safe (test);

  CORBA::Object_var object = test->_this ();

  orb->register_initial_reference ("ORBMyService",
                                    object.in ());

  bool invalid_name = false;
  try
    {
      // Registering with an empty string should give an exception
      orb->register_initial_reference ("",
                                       object.in ());
    }
  catch (const CORBA::ORB::InvalidName&)
    {
      invalid_name = true;
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!invalid_name)
    {
      errors++;
      ACE_ERROR ((LM_ERROR, "ERROR: Registering with an empty string with the ORB"
                            "doesn't throw an exception\n"));
    }

  bool duplicate_name = false;
  try
    {
      // Registering with an duplicate string should give an exception
      orb->register_initial_reference ("ORBMyService",
                                        object.in ());
    }
  catch (const CORBA::ORB::InvalidName&)
    {
      duplicate_name = true;
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!duplicate_name)
    {
      errors++;
      ACE_ERROR ((LM_ERROR, "ERROR: Registering with a duplicate with ORB "
                            "doesn't throw the expected exception\n"));
    }

  bool invalid_object = false;
  try
    {
      // Registering with a nil object
      orb->register_initial_reference ("ORBNilServer",
                                       CORBA::Object::_nil());
    }
  catch (const CORBA::BAD_PARAM& ex)
    {
      if ((ex.minor() & 0xFFFU) == 27)
        {
          invalid_object = true;
        }
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!invalid_object)
    {
      errors++;
      ACE_ERROR ((LM_ERROR, "ERROR: Registering with a nil object to ORB "
                            "doesn't throw bad param with minor code 27\n"));
    }

  return errors;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Server_ORBInitializer *initializer = 0;
  int retval = 0;

  ACE_NEW_RETURN (initializer,
                  Server_ORBInitializer,
                  -1);  // No exceptions yet!

  PortableInterceptor::ORBInitializer_var initializer_var =
    initializer;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      retval = test_orb (orb.in ());

      PortableInterceptor::register_orb_initializer (initializer_var.in ());

      CORBA::ORB_var second_orb =
        CORBA::ORB_init (argc, argv, "SecondORB");

      second_orb->destroy ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception in server:");
      return 1;
    }

  return retval;
}
