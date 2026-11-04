#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/SString.h"
#include "tao/debug.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  int iterations {100};
  bool do_shutdown {true};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        options.do_shutdown = false;
        break;

      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> -i <niterations> "
                                     "-x (disable shutdown)\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      Test::Roundtrip_var const roundtrip =
        Test::Roundtrip::_narrow (object.in ());

      if (CORBA::is_nil (roundtrip.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Nil Test::Roundtrip reference <%C>\n"),
                             ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ())),
                            1);
        }

      for (int i {}; i < options.iterations; ++i)
        {
          ACE_hrtime_t const start = ACE_OS::gethrtime ();
          ACE_hrtime_t const retval = roundtrip->test_method (start);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "test return value: %Q\n", retval));
        }


      ACE_DEBUG ((LM_DEBUG, "test finished\n"));


      if (options.do_shutdown)
        {
          roundtrip->shutdown ();
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
