//
// $Id$
//

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "tao/Object_T.h"
#include "testC.h"

ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
CORBA::ULong threads = 2u;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 't':
        threads = ACE_OS::strtoul (get_opts.opt_arg (), 0, 10);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-k <ior> ")
                           ACE_TEXT ("-t <number of threads server has> ")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("args to be used: -k '%s' -t %d\n"),
              ior,
              threads));

  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: wrong arguments\n")),
                          -1);

      CORBA::Object_var obj = orb->string_to_object (ior);

      // Create Hello reference.
      Test::UIPMC_Object_var test =
        TAO::Narrow_Utils<Test::UIPMC_Object>::unchecked_narrow (obj.in ());

      if (CORBA::is_nil (test.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: IOR is not an UIPMC_Object\n")),
                          -1);

      for (CORBA::ULong j = 0u; j < threads; ++j)
        {
          try
            {
              ACE_DEBUG ((LM_DEBUG, "Sending request %u\n", (unsigned) (j+1u)));
              test->process ();
            }
          catch (const CORBA::Exception& ex)
            {
              ex._tao_print_exception ("Exception caught during send\n");
            }
        }

      // Give a chance to flush all OS buffers for client.
      while (orb->work_pending ())
        orb->perform_work ();

      ACE_DEBUG ((LM_DEBUG, "Client shutting down\n"));
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client main ():");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nClient finished successfully\n")));
  return 0;
}
