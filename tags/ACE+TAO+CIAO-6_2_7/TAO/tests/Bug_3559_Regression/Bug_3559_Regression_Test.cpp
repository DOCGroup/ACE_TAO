// $Id$

#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/ACE.h"

const ACE_TCHAR* log_file_;

int
parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("l:"));

  int c;

  while ((c = get_opts()) != -1)
    {
      switch (c)
      {
        case 'l':
          log_file_ = get_opts.opt_arg();
          return 0;
      }
    }
  return 1;
}


int
check_logging()
{
  ACE_TCHAR buf[1024];
  FILE* fp = ACE_OS::fopen (log_file_,
                            ACE_TEXT ("r"));

  if (fp == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("error opening log file %s\n"),
                  log_file_));
      return 1;
    }


  while (ACE_OS::fgets (buf, sizeof (buf), fp) != 0)
    {
      ///When verbose is on, a line contains a timestamp and a @ character
      if  (ACE_OS::strchr (buf, ACE_TEXT('@')) != 0)
        {
          ACE_DEBUG((LM_DEBUG,
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
check_flags (const u_long& flags, const int& verbose)
{
  int result = 0;
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  int extra_argc = 25;

  try
    {
      if (parse_args(argc, argv) != 0)
      {
        ACE_ERROR((LM_ERROR,
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

          ACE_TCHAR **extra = 0;
          ACE_NEW_RETURN (extra, ACE_TCHAR *[extra_argc], -1);
          extra[0] = ACE::strnew (ACE_TEXT ("-ORBCDRTradeoff"));
          extra[1] = ACE::strnew (ACE_TEXT ("2048"));
          extra[2] = ACE::strnew (ACE_TEXT ("-ORBCollocation"));
          extra[3] = ACE::strnew (ACE_TEXT ("per-orb"));
          extra[4] = ACE::strnew (ACE_TEXT ("-ORBDebug"));
          extra[5] = ACE::strnew (ACE_TEXT ("-ORBDebugLevel"));
          extra[6] = ACE::strnew (ACE_TEXT ("10"));
          extra[7] = ACE::strnew (ACE_TEXT ("-ORBDefaultInitRef"));
          extra[8] = ACE::strnew (ACE_TEXT ("corbaloc::localhost:4177"));
          extra[9] = ACE::strnew (ACE_TEXT ("-ORBDottedDecimalAddresses"));
          extra[10] = ACE::strnew (ACE_TEXT ("1"));
          extra[11] = ACE::strnew (ACE_TEXT ("-ORBId"));
          extra[12] = ACE::strnew (ACE_TEXT ("Client"));
          extra[13] = ACE::strnew (ACE_TEXT ("-ORBListenEndpoints"));
          extra[14] = ACE::strnew (ACE_TEXT ("iiop://:4178"));
          extra[15] = ACE::strnew (ACE_TEXT ("-ORBLogFile"));
          ACE_NEW_RETURN (extra[16],
                          ACE_TCHAR[ACE_OS::strlen(log_file_) + 1],
                          -1);
          ACE_OS::sprintf (extra[16], ACE_TEXT("%s"), log_file_);
          extra[17] = ACE::strnew (ACE_TEXT ("-ORBNegotiateCodesets"));
          extra[18] = ACE::strnew (ACE_TEXT ("0"));
          extra[19] = ACE::strnew (ACE_TEXT ("-ORBNodelay"));
          extra[20] = ACE::strnew (ACE_TEXT ("1"));
          extra[21] = ACE::strnew (ACE_TEXT ("-ORBSvcConfDirective"));
          extra[22] = ACE::strnew (ACE_TEXT ("\"static Client_Strategy_Factory '-ORBWaitStrategy mt_noupcall'\""));
          extra[23] = ACE::strnew (ACE_TEXT ("-ORBVerboseLogging"));
          ACE_NEW_RETURN (extra[24],
                          ACE_TCHAR[2],
                          -1);
          ACE_OS::sprintf (extra[24], ACE_TEXT ("%d"), verbose);

          ACE_TCHAR **largv = new ACE_TCHAR *[argc+extra_argc];
          for (int i = 0; i < argc; i++)
            largv[i] = argv[i];

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("server adding args: ")));
          for (int i = 0; i < extra_argc; i++)
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s "), extra[i]));
              largv[argc+i] = extra[i];
            }
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

          argc += extra_argc;

          ACE_TCHAR orb_name[25];
          ACE_OS::sprintf (orb_name, ACE_TEXT ("VERBOSE_LOGGING_TEST_%d"), verbose);

          CORBA::ORB_var orb = CORBA::ORB_init (argc, largv, orb_name);

          u_long log_flags = ACE_LOG_MSG->flags();

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("log flags %d verbose: %d\n"),
                      log_flags, verbose));

          result += check_flags (log_flags, verbose);

          orb->destroy ();
          orb = CORBA::ORB::_nil ();
          ACE_LOG_MSG->restart ();

          for (int i = 0; i < extra_argc; i++)
            ACE::strdelete (extra[i]);
          delete [] extra;
          delete [] largv;
        }
    }
  catch (const CORBA::Exception& ex)
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
