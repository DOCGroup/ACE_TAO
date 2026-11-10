
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "ace/OS_NS_stdlib.h"
#include "tao/Object_T.h"
#include "testC.h"

struct Client_Options
{
  ACE_TString ior { ACE_TEXT ("file://test.ior") };
  CORBA::ULong threads { 2u };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:t:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          options.ior = get_opts.opt_arg ();
          break;

        case 't':
          options.threads = ACE_OS::strtoul (get_opts.opt_arg (), nullptr, 10);
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -k <ior> -t <number of server threads>\n"),
                             argv[0]),
                            -1);
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("args to be used: -k '%s' -t %u\n"),
              options.ior.c_str (),
              static_cast<unsigned int> (options.threads)));
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      Client_Options options;

      if (parse_args (argc, argv, options) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: wrong arguments\n")),
                          -1);

      CORBA::Object_var const obj = orb->string_to_object (options.ior.c_str ());

      Test::UIPMC_Object_var const test =
        TAO::Narrow_Utils<Test::UIPMC_Object>::unchecked_narrow (obj.in ());

      if (CORBA::is_nil (test.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: IOR is not an UIPMC_Object\n")),
                          -1);

      for (CORBA::ULong j {}; j < options.threads; ++j)
        {
          try
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Sending request %u\n"),
                          static_cast<unsigned int> (j + 1u)));
              test->process ();
            }
          catch (CORBA::Exception const &ex)
            {
              ex._tao_print_exception ("Exception caught during send\n");
            }
        }

      // Give a chance to flush all OS buffers for client.
      while (orb->work_pending ())
        orb->perform_work ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client shutting down\n")));
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in client main ():");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nClient finished successfully\n")));
  return 0;
}
