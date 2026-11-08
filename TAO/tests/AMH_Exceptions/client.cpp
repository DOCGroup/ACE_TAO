// -*- C++ -*-
#include "TestC.h"
#include "ace/Get_Opt.h"

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
};

static int
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
                           ACE_TEXT ("usage: %s [-k <ior>]\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  bool received_expected_exception {};
  try
    {
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));
      Client_Options options {};

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object (orb->string_to_object (options.ior));

      Test::Roundtrip_var const roundtrip (Test::Roundtrip::_narrow (object.in ()));

      if (CORBA::is_nil (roundtrip.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Nil Test::Roundtrip reference <%s>\n"),
                             options.ior),
                            1);
        }

      try
        {
          Test::Timestamp const time (10);
          roundtrip->test_method (time);
        }
      catch (Test::ServerOverload const &)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Received expected exception\n")));
          received_expected_exception = true;

          roundtrip->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("");
      return 1;
    }

  if (!received_expected_exception)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR, expecting a ServerOverload exception!!\n")),
                        1);
    }

  return 0;
}
