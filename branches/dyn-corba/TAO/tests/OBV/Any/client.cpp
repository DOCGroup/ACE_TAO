// $Id$

#include "AnyC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Any, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test started.\n"));

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Create and register factories.

      OBV_AnyTest::VA_init *va_factory = 0;
      ACE_NEW_RETURN (va_factory,
                      OBV_AnyTest::VA_init,
                      1); // supplied by mapping

      orb->register_value_factory (va_factory->tao_repository_id (),
                                   va_factory
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      va_factory->_remove_ref (); // release ownership


      OBV_AnyTest::VB_init *vb_factory = 0;
      ACE_NEW_RETURN (vb_factory,
                      OBV_AnyTest::VB_init,
                      1); // supplied by mapping

      orb->register_value_factory (vb_factory->tao_repository_id (),
                                   vb_factory
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      vb_factory->_remove_ref (); // release ownership

      // Obtain reference to the object

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      OBV_AnyTest::Test_var test =
        OBV_AnyTest::Test::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (test.in ()))
      {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Nil OBV_AnyTest::Test reference <%s>\n",
                         ior),
                        1);
      }

      // Do local test

      OBV_AnyTest::VA_var va1, va2;
      ACE_NEW_RETURN (va1, OBV_OBV_AnyTest::VA, 1);
      ACE_NEW_RETURN (va2, OBV_OBV_AnyTest::VA, 1);

      const CORBA::ULong magic = 3145;

      va1->id (magic);
      va2->id (magic);

      CORBA::Any a1, a2;

      // Test both copying and non-copying version of operator<<=
      a1 <<= va1.in ();

      OBV_AnyTest::VA *pva = va2._retn();
      a2 <<= &pva;

      OBV_AnyTest::VA* dst = 0;

      if (!(a1 >>= dst) || dst->id () != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - test failed.\n"),
                            1);
        }

      if (!(a2 >>= dst) || dst->id () != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - test failed.\n"),
                            1);
        }

      // Now do remote test

      // STEP 1.
      CORBA::Any_var result = test->get_something (
          0
          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!(result.inout () >>= dst) || dst->id () != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - test failed.\n"),
                            1);
        }

      // STEP 2.
      OBV_AnyTest::VB* dst_vb = 0;
      result = test->get_something (
          1
          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!(result.inout () >>= dst_vb) || dst_vb->id () != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - test failed.\n"),
                            1);
        }

      test->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test finished.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
