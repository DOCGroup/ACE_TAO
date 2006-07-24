// $Id$

#include "FactoryC.h"
#include "FactoryC_impl.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Factory, client, "$Id$")

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

bool
no_factory (OBV_FactoryTest::Test_ptr test)
{
  bool succeed = false;
  ACE_TRY_NEW_ENV
    {
      // Calling this without a factory registred should give a marshal
      // exception with minor code 1
      OBV_FactoryTest::BaseValue_var base_value =
        test->get_base_value (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::MARSHAL, ex)
    {
      if ((ex.minor() & 0xFFFU) == 1)
        {
          succeed = true;
        }
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, no_factory failed\n"));
  }

  return succeed;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Obtain reference to the object
      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      OBV_FactoryTest::Test_var test =
        OBV_FactoryTest::Test::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (test.in ()))
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil OBV_FactoryTest::Test reference <%s>\n",
                           ior),
                          1);
      }

      // Check if we get the correct exception with minor code because no
      // factory has been set.
      if (!no_factory (test.in ()))
        return 1;

      // Create factories.
      OBV_FactoryTest::BaseValue_init *base_factory = 0;
      ACE_NEW_RETURN (base_factory,
                      OBV_FactoryTest::BaseValue_init,
                      1); // supplied by mapping

      orb->register_value_factory (base_factory->tao_repository_id (),
                                   base_factory
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      base_factory->_remove_ref (); // release ownership

      OBV_FactoryTest::Value1_init *value1_factory = 0;
      ACE_NEW_RETURN (value1_factory,
                      OBV_FactoryTest::Value1_init,
                      1); // supplied by mapping

      orb->register_value_factory (value1_factory->tao_repository_id (),
                                   value1_factory
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      value1_factory->_remove_ref ();

      OBV_FactoryTest::Value2_init *value2_factory = 0;
      ACE_NEW_RETURN (value2_factory,
                      Value2_init_impl,
                      1); // custom implementation

      orb->register_value_factory (value2_factory->tao_repository_id (),
                                   value2_factory
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      value2_factory->_remove_ref ();

      // Now perform the test. I don't check return values.
      // I just hope to get MARSHAL.
      OBV_FactoryTest::BaseValue_var base_value =
        test->get_base_value (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      OBV_FactoryTest::Value1_var value1 =
        test->get_value1 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      OBV_FactoryTest::Value2_var value2 =
        test->get_value2 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Test factories.

      value2 = value2_factory->create_default (1);

      OBV_FactoryTest::BaseValue::BV_Data data;
      data.value = 2;

      value2 = value2_factory->create (1, data);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test finished\n"));

      test->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
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
