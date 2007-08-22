// $Id$

#include "TestC.h"

const char *ior = "file://test.ior";

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int received_expected_exception = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in ());

      if (CORBA::is_nil (roundtrip.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Roundtrip reference <%s>\n",
                             ior),
                            1);
        }

      try{
          Test::Timestamp time = 10;
          roundtrip->test_method (time);
        }
      catch (const Test::ServerOverload& )
        {
          ACE_DEBUG ((LM_DEBUG, "Received expected exception\n"));
          received_expected_exception = 1;

          roundtrip->shutdown ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("");
      return 1;
    }

  if(!received_expected_exception)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ERROR, expecting a ServerOverload exception!!\n"),
                        1);
    }

  return 0;
}
