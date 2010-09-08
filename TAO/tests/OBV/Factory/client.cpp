// $Id$

#include "FactoryC.h"
#include "FactoryC_impl.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
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
  try
    {
      // Calling this without a factory registred should give a marshal
      // exception with minor code 1
      OBV_FactoryTest::BaseValue_var base_value =
        test->get_base_value ();
    }
  catch (const CORBA::MARSHAL& ex)
    {
      if ((ex.minor() & 0xFFFU) == 1)
        {
          succeed = true;
        }
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, no_factory failed\n"));
  }

  return succeed;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // Obtain reference to the object
      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      OBV_FactoryTest::Test_var test =
        OBV_FactoryTest::Test::_narrow(tmp.in ());

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
                                   base_factory);
      base_factory->_remove_ref (); // release ownership

      OBV_FactoryTest::Value1_init *value1_factory = 0;
      ACE_NEW_RETURN (value1_factory,
                      OBV_FactoryTest::Value1_init,
                      1); // supplied by mapping

      orb->register_value_factory (value1_factory->tao_repository_id (),
                                   value1_factory);
      value1_factory->_remove_ref ();

      OBV_FactoryTest::Value2_init *value2_factory = 0;
      ACE_NEW_RETURN (value2_factory,
                      Value2_init_impl,
                      1); // custom implementation

      orb->register_value_factory (value2_factory->tao_repository_id (),
                                   value2_factory);
      value2_factory->_remove_ref ();

      // Now perform the test. I don't check return values.
      // I just hope to get MARSHAL.
      OBV_FactoryTest::BaseValue_var base_value =
        test->get_base_value ();

      OBV_FactoryTest::Value1_var value1 =
        test->get_value1 ();

      OBV_FactoryTest::Value2_var value2 =
        test->get_value2 ();

      // Test factories.

      value2 = value2_factory->create_default (1);

      OBV_FactoryTest::BaseValue::BV_Data data;
      data.value = 2;

      value2 = value2_factory->create (1, data);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test finished\n"));

      test->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
