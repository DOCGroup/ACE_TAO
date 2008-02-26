/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================


#include "testSchedC.h"
#include <orbsvcs/RTCosScheduling/RTCosScheduling_ClientScheduler_i.h>
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "tao/Version.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"


/// In the test, these are the default values that specify
/// how long (in seconds) the test methods should run before going
/// to the server, while on the server, and after returning from
/// the server
CORBA::Long  before_    = 1;
CORBA::Long  remote_    = 1;
CORBA::Long  after_     = 1;
u_int use_realtime_     = 1;

/// This is the name of the node that the client executes on.
char *node_ = 0;

/// the name of the config file holding the scheduling information
char *file_ = 0;

/// the name of the activity to run (the name of the string passed in
/// schedule_activity(activity_)
char * activity_ = 0;

/// The name of the output file
char *client_output_file_ = 0;

CORBA::String_var serv_output_;

char date_and_time[35];
char client_output_[2048];
char buf[100];


/// For the timestampe
const int time_size = 35;
ACE_TCHAR day_and_time[time_size];

void do_work(const CORBA::Long value);

ACE_RCSID(
   tests,
   client,
   "client.cpp,v 1.0 2003/08/07 15:59:21 murphy_m Exp")

/// Standard parse args method
/*
 *  parse_args allows the user to configure the test to use values other
 *  than those specified above.
 */
int
parse_args (int argc, char *argv[])
{
  /// We set the '-' flag for getopts because we have to do this
  /// parsing before the ORB does it's parsing, and we'd confuse it to reorder
  ACE_Get_Opt get_opts (argc, argv, "-B:R:A:X:N:C:F:T:?:");
  int c;
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'B':
            before_ =ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case 'R':
            remote_ =ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case 'A':
            after_ =ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case 'X':
            use_realtime_ =ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case 'N':
            node_ = ACE_OS::strdup(get_opts.opt_arg ());
            break;
          case 'C':
            client_output_file_ = ACE_OS::strdup(get_opts.opt_arg ());
            break;
          case 'F':
            file_ = ACE_OS::strdup(get_opts.opt_arg ());
            break;
          case 'T':
            activity_ = ACE_OS::strdup(get_opts.opt_arg ());
            break;
          case '?':
            ACE_DEBUG ((LM_DEBUG,
                        "usage: %n [-b precall_execution] "
                                   "[-r remote_execution] "
                                   "[-a postcall_workload] "
                                   "[-N node] "
                                   "[-F config_file] "
                                   "[-c activity_name]\n"));
            ACE_OS::exit (1);
        }
    }
  if (node_ == 0)
    node_ = ACE_OS::strdup("1");
  if (file_ == 0)
    file_ = ACE_OS::strdup("schedule.cfg");
  if (activity_ == 0)
    activity_ = ACE_OS::strdup("");
  if (client_output_file_ == 0)
    client_output_file_ = ACE_OS::strdup("");

  return 0;
}

/**
 * This class runs the test as a new thread since the main thread
 * cannot set RT priorities as of TAO 1.3.3
 *
 */
class ORB_Thread : public ACE_Task<ACE_SYNCH>
{

public:
ORB_Thread(CORBA::ORB_var orb,
           int argc,
           char **argv)
: orb_(orb),
  argc_(argc),
  argv_(argv)
{
}

int svc(void) {
  try
    {
      if (parse_args (argc_, argv_) != 0)
        {
          return 1;
        }

      if (ACE_OS::strcmp(activity_,"Client2") == 0 ||
          ACE_OS::strcmp(activity_,"Client3") == 0)
          {
            ACE_OS::sleep(1);
          }

      CORBA::Object_var obj =
        orb_->string_to_object ("file://server.ior"
                                );


      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P) orb_->string_to_object ")
                             ACE_TEXT ("(\"file://server.ior\") failed.\n")),
                             -1);
        }

      testSched::Object1_var object1 =
        testSched::Object1::_narrow (obj.in ());


      if (CORBA::is_nil (object1.in ()))
        {
           ACE_ERROR_RETURN ((LM_ERROR,
             "ERROR: Object reference is nil, Aborting\n"), 1);
        }


      if (use_realtime_)
        {
          /// Create the ClientScheduler, note that you must pass in the
          /// orb and the node name.  The node name is required so that the
          /// appropriate information can be read from the config file
          TAO::RTCosScheduling_ClientScheduler_i *client_sched = 0;
          ACE_NEW_THROW_EX(client_sched,
                           TAO::RTCosScheduling_ClientScheduler_i (
                             orb_,
                             node_,
                             file_),
                           CORBA::NO_MEMORY());


          // Check to see that the ClientScheduler started correctly
          if (CORBA::is_nil (client_sched))
            {
              /// There was an error in setting up the RTORB,
              /// RT Policies were not set, throw an exception
             ACE_OS::exit(1);
            }



          /// Create a Current object so we can check on the priority locally
          obj = orb_->resolve_initial_references ("RTCurrent");
          RTCORBA::Current_var current =
            RTCORBA::Current::_narrow (obj.in ());


          /// Test to make sure the priority model is exposed
          CORBA::Policy_var policy =
            object1->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE);

          RTCORBA::PriorityModelPolicy_var priority_policy =
            RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

          if (CORBA::is_nil (priority_policy.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "ERROR: Priority Model not exposed!\n"),
                                 1);
            }

          /// Test to make sure we are using Server Declared Priority model
          RTCORBA::PriorityModel priority_model =
            priority_policy->priority_model ();

          if (priority_model != RTCORBA::SERVER_DECLARED)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "ERROR: priority_model != "
                                 "RTCORBA::SERVER_DECLARED!\n"),
                                 1);
            }

          do_work(before_);

          try
            {
               client_sched->schedule_activity (::activity_);
            }
          catch (const CORBA::Exception& ex)
            {
               ex._tao_print_exception ("Invalid activity name\n");
               return -1;
            }

          ACE_OS::sprintf(buf,
                          "%s\t%s\tBeginning activity at priority %d\n",
                          ACE::timestamp(date_and_time, time_size),
                          activity_,
                          current->the_priority());
          ACE_DEBUG ((LM_DEBUG,
                      "%s",
                      buf));
          ACE_OS::strcat(client_output_, buf);

          do_work(before_);


          ACE_OS::sprintf(buf,
                          "%s\t%s\tCalling method1 at priority %d\n",
                          ACE::timestamp(date_and_time, time_size),
                          activity_,
                          current->the_priority());
          ACE_DEBUG ((LM_DEBUG,
                      "%s",
                      buf));
          ACE_OS::strcat(client_output_, buf);
          serv_output_ = ACE_OS::strdup("");
          object1->method1 (activity_, remote_, serv_output_.inout());

          ACE_OS::strcat(client_output_, serv_output_.in());


          ACE_OS::sprintf(buf,
                          "%s\t%s\tDone with method1 at priority %d\n",
                          ACE::timestamp(date_and_time, time_size),
                          activity_,
                          current->the_priority());
          ACE_DEBUG ((LM_DEBUG,
                      "%s",
                      buf));
          ACE_OS::strcat(client_output_, buf);


          do_work(after_);

          ACE_OS::sprintf(buf,
                          "%s\t%s\tDone with test at priority %d\n",
                          ACE::timestamp(date_and_time, time_size),
                          activity_,
                          current->the_priority());
          ACE_DEBUG ((LM_DEBUG,
                      "%s",
                      buf));
          ACE_OS::strcat(client_output_, buf);


        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                     "%T\t%s\tClient\tBeginning Activity\n",
                     activity_));
          do_work(before_);
          ACE_DEBUG ((LM_DEBUG,
                      "%T\t%s\tClient\tCalling method1\n",
                      activity_));

          object1->method1 (activity_, remote_, serv_output_.inout());
          ACE_OS::strcat(client_output_,serv_output_.in());
          /// Finished with remote call
          ACE_DEBUG ((LM_DEBUG,
                      "%T\t%s\tClient\tDone with method1\n",
                      activity_));
          do_work(after_);
        }

      FILE *fp = ACE_OS::fopen(client_output_file_, "w");
      if (fp)
        {
          ACE_OS::fprintf(fp,
                          "%s",
                           client_output_);
        }
      else
      {
          ACE_DEBUG((LM_DEBUG,"No file to write to\n"));
      }
      ACE_OS::fclose(fp);


      // Finally destroy the ORB
      orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in running the client\n");
      return -1;
    }
  return 0;
 }


private:
  CORBA::ORB_var orb_;
  CORBA::ORB_var orb2_;
  int argc_; char ** argv_;
};



int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
    CORBA::ORB_var orb =
    CORBA::ORB_init (argc, argv, "client_orb");

    ORB_Thread ot(orb, argc, argv);

    // Need to set the main thread pthread scope and pthread policy to
    // the values that are specified in svc.conf.  This change was
    // recommended by irfan@oomworks.com
    long flags = THR_NEW_LWP | THR_JOINABLE |

    orb->orb_core ()->orb_params ()->thread_creation_flags ();

    ot.activate(flags);
    return ot.wait();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ERROR in running the client\n");
    }
  return 0;
}

void
do_work(const CORBA::Long value)
{
  const int scale_factor = 2000;
  int work;

  /// Simulate some work
  static CORBA::ULong prime_number = 9619;

  work = scale_factor * value;
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "test_i::method: %hd units of work\n", work));
  for (; work != 0; work--)
    ACE::is_prime (prime_number, 2, prime_number / 2);

}
