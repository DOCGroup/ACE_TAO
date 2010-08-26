// -*- C++ -*-
// $Id$

#include "TestC.h"
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
        ior = get_opts.opt_arg ();
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int received_expected_exception = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

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
