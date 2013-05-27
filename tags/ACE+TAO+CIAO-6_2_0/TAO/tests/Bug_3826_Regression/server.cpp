// $Id$

#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/Resource_Factory.h"

ACE_TCHAR const *orb1_args =
  ACE_TEXT ("AAA -ORBGestalt LOCAL -ORBSvcConf MY_TEST_ORB_1.conf");
ACE_TCHAR const *orb2_args =
  ACE_TEXT ("BBB");
int expected_value = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:b:e:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        orb1_args = get_opts.opt_arg ();
        break;
      case 'b':
        orb2_args = get_opts.opt_arg ();
        break;
      case 'e':
        expected_value = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s")
                           ACE_TEXT (" -a <orb1_args>")
                           ACE_TEXT (" -b <orb2_args>")
                           ACE_TEXT (" -e <expected value of ORBConnectionCacheMax>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      ACE_ARGV orb1_argv (orb1_args);
      int orb1_argc = orb1_argv.argc ();

      ACE_ARGV orb2_argv (orb2_args);
      int orb2_argc = orb2_argv.argc ();

      ACE_DEBUG ((LM_DEBUG,
                  "Initialize ORB instances...\n"));

      CORBA::ORB_var orb1 =
        CORBA::ORB_init (orb1_argc, orb1_argv.argv (), "AAA");

      CORBA::ORB_var orb2 =
        CORBA::ORB_init (orb2_argc, orb2_argv.argv (), "BBB");

      ACE_DEBUG ((LM_DEBUG,
                  "After ORB_init...\n"));

      TAO_Resource_Factory *trf1 = orb1->orb_core ()->resource_factory ();

      if (trf1 == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ERROR: Can not resolve ")
                             ACE_TEXT ("Resource Factory for orb1.\n")),
                            -1);
        }

      TAO_Resource_Factory *trf2 = orb2->orb_core ()->resource_factory ();

      if (trf2 == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ERROR: Can not resolve ")
                             ACE_TEXT ("Resource Factory for orb2.\n")),
                            -1);
        }

      if (trf1->cache_maximum () == trf2->cache_maximum () ||
          (0 != expected_value && trf1->cache_maximum () != expected_value))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ERROR: '-ORBConnectionCacheMax' is ")
                             ACE_TEXT ("applied globally while it's expected ")
                             ACE_TEXT ("to apply to the first ORB only.\n")),
                            -1);
        }

      orb1->destroy ();
      orb2->destroy ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("'-ORBConnectionCacheMax' is applied to the first ")
                  ACE_TEXT ("ORB only as expected.\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught Exception in main:\n");
      return 1;
    }

  return 0;
}
