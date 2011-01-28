// $Id$

#include "TestC.h"
#include "Global.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");

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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int status = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object (ior);

      Test::LongDoubleTest_var longDoubleTest =
        Test::LongDoubleTest::_narrow (tmp.in ());

      if (CORBA::is_nil (longDoubleTest.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::LongDoubleTest reference <%s>\n",
                             ior),
                            1);
        }

      CORBA::LongDouble ld =
        longDoubleTest->get_long_double ();

      if (ld == Global::get_long_double ())
        {
          cout << "Received: " << ld << endl;
        }
      else
        {
          ACE_ERROR ((LM_DEBUG,
                      "Client did not receive "
                      "expected CORBA::LongDouble\n"));
          cerr << "Received: " << ld << " instead of "
               << Global::get_long_double () << endl;
          status = 1;
        }

      longDoubleTest->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      status = 1;
    }

  return status;
}
