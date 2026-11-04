#include "TestC.h"
#include "Global.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/SString.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://server.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status {};
  Client_Options options;
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      Test::LongDoubleTest_var const long_double_test =
        Test::LongDoubleTest::_narrow (object.in ());

      if (CORBA::is_nil (long_double_test.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::LongDoubleTest reference <%s>\n",
                             ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ())),
                            1);
        }

      CORBA::LongDouble const ld = long_double_test->get_long_double ();

      if (ld == Global::get_long_double ())
        {
          std::cout << "Received: " << ld << std::endl;
        }
      else
        {
          ACE_ERROR ((LM_DEBUG,
                      "Client did not receive "
                      "expected CORBA::LongDouble\n"));
          std::cerr << "Received: " << ld << " instead of "
                    << Global::get_long_double () << std::endl;
          status = 1;
        }

      long_double_test->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      status = 1;
    }

  return status;
}
