// $Id$

//========================================================================
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is the driver program that compares copy and non-copy performance
//     of arg string member assignment from std::string.
//
// = AUTHOR
//     Jeff Parsons <j.parsons@vanderbilt.edu>
//
//=========================================================================

#include "ace/High_Res_Timer.h"
#include "ace/Basic_Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_ctype.h"
#include "ace/Get_Opt.h"

#include "StringMemberC.h"

const char *ior_file = "file://StringMember.ior";
int niterations = 10000;
CORBA::ULong len = 40;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:l:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'l':
        len = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <iterations> "
                           "-l <string length> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

char *
gen_string (void)
{
  char *buf = CORBA::string_alloc (len);
  CORBA::ULong i = 0;

  while (i < len)
    {
      int c = ACE_OS::rand () % 128;

      if (ACE_OS::ace_isprint (c) && !ACE_OS::ace_isspace (c))
        {
          buf [i] = c;
          i++;
        }
    }

  buf[i] = 0;
  return buf;
}

int
ACE_TMAIN (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj = orb->string_to_object (ior_file);

      Test_var target = Test::_narrow (obj.in ());

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::String_var str = gen_string ();
      std::string data (str.in ());

      Foo arg;

      ACE_Sample_History no_copy_history (niterations);

      for (int i = 0; i < niterations; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          arg.strmem = data;
          target->test_op (arg);
          ACE_hrtime_t now = ACE_OS::gethrtime ();
          no_copy_history.sample (now - start);
        }

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_Basic_Stats no_copy_stats;
      no_copy_history.collect_basic_stats (no_copy_stats);
      no_copy_stats.dump_results (ACE_TEXT ("No Copy"), gsf);

      ACE_Sample_History copy_history (niterations);

      for (int i = 0; i < niterations; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          arg.strmem = data.c_str ();
          target->test_op (arg);
          ACE_hrtime_t now = ACE_OS::gethrtime ();
          copy_history.sample (now - start);
        }

      ACE_Basic_Stats copy_stats;
      copy_history.collect_basic_stats (copy_stats);
      copy_stats.dump_results (ACE_TEXT ("Copy"), gsf);

//      target->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client: CORBA Exception caught");
      return -1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "client: unknown exception caught\n"),
                        -1);
    }

  return 0;
}
