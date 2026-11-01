#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/ACE.h"
#include <string>
#include <vector>

ACE_TCHAR const *log_file_;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("l:"));

  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
      {
        case 'l':
          log_file_ = get_opts.opt_arg ();
          return 0;
      }
    }
  return 1;
}


int
check_logging ()
{
  ACE_TCHAR buf[1024];
  FILE *fp = ACE_OS::fopen (log_file_,
                            ACE_TEXT ("r"));

  if (fp == nullptr)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("error opening log file %s\n"),
                  log_file_));
      return 1;
    }


  while (ACE_OS::fgets (buf, sizeof (buf), fp) != nullptr)
    {
      ///When verbose is on, a line contains a timestamp and a @ character
      if (ACE_OS::strchr (buf, ACE_TEXT('@')) != nullptr)
        {
          ACE_DEBUG ((LM_DEBUG,
                     ACE_TEXT ("Test OK, a timestamp was found\n")));
          (void) ACE_OS::fclose (fp);
          return 0;
        }
    }
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Test failed, no timestamp found in log\n")));
  (void) ACE_OS::fclose (fp);
  return 1;
}

int
check_flags (u_long flags, int verbose)
{
  int result {};
  switch (verbose)
    {
      case 0:
        /// Both VERBOSE and VERBOSE_LITE shouldn't be set
        if ((flags & ACE_Log_Msg::VERBOSE) != 0 &&
            (flags & ACE_Log_Msg::VERBOSE_LITE) != 0)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Test failed: Verbose flag not set properly when verbose=0\n")));
            result = 1;
          }
        break;
      case 1:
        /// VERBOSE_LITE should be set
        if ((flags & ACE_Log_Msg::VERBOSE_LITE) == 0)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Test failed: Verbose flag not set properly when verbose=1\n")));
            result = 1;
          }
        break;
      default:
        /// VERBOSE should be set
        if ((flags & ACE_Log_Msg::VERBOSE) == 0)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Test failed: Verbose flag not set properly when verbose=unknown\n")));
            result = 1;
          }
        break;
    }
  return result;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result {};
  constexpr int extra_argc = 25;

  try
    {
      if (parse_args (argc, argv) != 0)
      {
        ACE_ERROR ((LM_ERROR,
                   ACE_TEXT ("Usage:\n\n")
                   ACE_TEXT ("OPTIONS:\n\n")
                   ACE_TEXT ("\t[-l Log file]\n\n")));
        return 1;
      }

      for (int verbose = 0; verbose < 3; verbose++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Start ORB_init with ORBVerboseLogging=%d\n"),
                      verbose));

          std::vector<std::basic_string<ACE_TCHAR>> extra;
          extra.reserve (extra_argc);
          extra.emplace_back (ACE_TEXT ("-ORBCDRTradeoff"));
          extra.emplace_back (ACE_TEXT ("2048"));
          extra.emplace_back (ACE_TEXT ("-ORBCollocation"));
          extra.emplace_back (ACE_TEXT ("per-orb"));
          extra.emplace_back (ACE_TEXT ("-ORBDebug"));
          extra.emplace_back (ACE_TEXT ("-ORBDebugLevel"));
          extra.emplace_back (ACE_TEXT ("10"));
          extra.emplace_back (ACE_TEXT ("-ORBDefaultInitRef"));
          extra.emplace_back (ACE_TEXT ("corbaloc::localhost:4177"));
          extra.emplace_back (ACE_TEXT ("-ORBDottedDecimalAddresses"));
          extra.emplace_back (ACE_TEXT ("1"));
          extra.emplace_back (ACE_TEXT ("-ORBId"));
          extra.emplace_back (ACE_TEXT ("Client"));
          extra.emplace_back (ACE_TEXT ("-ORBListenEndpoints"));
          extra.emplace_back (ACE_TEXT ("iiop://:4178"));
          extra.emplace_back (ACE_TEXT ("-ORBLogFile"));
          extra.emplace_back (log_file_);
          extra.emplace_back (ACE_TEXT ("-ORBNegotiateCodesets"));
          extra.emplace_back (ACE_TEXT ("0"));
          extra.emplace_back (ACE_TEXT ("-ORBNodelay"));
          extra.emplace_back (ACE_TEXT ("1"));
          extra.emplace_back (ACE_TEXT ("-ORBSvcConfDirective"));
          extra.emplace_back (ACE_TEXT ("\"static Client_Strategy_Factory '-ORBWaitStrategy mt_noupcall'\""));
          extra.emplace_back (ACE_TEXT ("-ORBVerboseLogging"));
          ACE_TCHAR verbose_arg[16];
          ACE_OS::sprintf (verbose_arg, ACE_TEXT ("%d"), verbose);
          extra.emplace_back (verbose_arg);

          std::vector<ACE_TCHAR *> largv;
          largv.reserve (argc + extra_argc);
          for (int i = 0; i < argc; ++i)
            largv.push_back (argv[i]);

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("server adding args: ")));
          for (std::basic_string<ACE_TCHAR> &arg : extra)
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s "), arg.c_str ()));
              largv.push_back (arg.data ());
            }
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

          int orb_argc = argc + extra_argc;

          ACE_TCHAR orb_name[25];
          ACE_OS::sprintf (orb_name, ACE_TEXT ("VERBOSE_LOGGING_TEST_%d"), verbose);

          CORBA::ORB_var orb = CORBA::ORB_init (orb_argc, largv.data (), orb_name);

          u_long const log_flags = ACE_LOG_MSG->flags ();

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("log flags %d verbose: %d\n"),
                      log_flags, verbose));

          result += check_flags (log_flags, verbose);

          orb->destroy ();
          orb = CORBA::ORB::_nil ();
          ACE_LOG_MSG->restart ();

        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  result += check_logging ();

  if (result == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Test passed!")));
  else
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Test failed. Result: %d\n"),
                result));
  return result;
}
