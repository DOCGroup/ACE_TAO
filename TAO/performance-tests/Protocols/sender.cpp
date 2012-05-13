// $Id$

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_time.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"
#include "tao/ORB_Constants.h"
#include "tao/Policy_ManagerC.h"
#include "Custom_Network_Priority_Mapping.h"
#include "Custom_Network_Priority_Mapping.cpp"
#include "tao/debug.h"
#include "testC.h"

// Types of tests supported.
enum Test_Type
  {
    PACED,
    THROUGHPUT,
    LATENCY
  };

static const ACE_TCHAR *ior = ACE_TEXT ("file://distributor.ior");
static int shutdown_server = 0;
static CORBA::ULong iterations = 5;
static CORBA::ULong invocation_rate = 5;
static int count_missed_end_deadlines = 0;
static ACE_High_Res_Timer::global_scale_factor_type gsf = 0;
static int do_dump_history = 0;
static int print_missed_invocations = 0;
static CORBA::ULong message_size = 0;
static const ACE_TCHAR *test_protocol = ACE_TEXT ("IIOP");
static int print_statistics = 1;
static int number_of_connection_attempts = 20;
static int enable_diffserv_code_points = 0;
static RTCORBA::Priority corba_priority = RTCORBA::minPriority;
static Test_Type test_type = PACED;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:b:c:d:e:i:k:m:p:r:s:t:x:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        test_type = static_cast<Test_Type> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;

      case 'b':
        enable_diffserv_code_points = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'c':
        corba_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        do_dump_history = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'e':
        count_missed_end_deadlines = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'm':
        print_missed_invocations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'p':
        test_protocol = get_opts.opt_arg ();
        break;

      case 'r':
        invocation_rate = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 's':
        message_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 't':
        print_statistics = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      default:
        {
          const ACE_TCHAR *test = 0;
          switch (test_type)
            {
            case PACED:
              test = ACE_TEXT ("PACED");
              break;
            case THROUGHPUT:
              test = ACE_TEXT ("THROUGHPUT");
              break;
            case LATENCY:
              test = ACE_TEXT ("LATENCY");
              break;
            }

          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s\n"
                             "\t-a <test type> (defaults to %s [valid values are PACED(%d), THROUGHPUT(%d), and LATENCY(%d))\n"
                             "\t-b <enable diffserv code points> (defaults to %d)\n"
                             "\t-c <corba priority> (defaults to %d)\n"
                             "\t-d <show history> (defaults to %d)\n"
                             "\t-e <count missed end deadlines> (defaults to %d)\n"
                             "\t-h <help: shows options menu>\n"
                             "\t-i <iterations> (defaults to %d)\n"
                             "\t-k <ior> (defaults to %s)\n"
                             "\t-m <print missed invocations for paced workers> (defaults to %d)\n"
                             "\t-p <test protocol> (defaults to %s [valid values are IIOP, DIOP, and SCIOP])\n"
                             "\t-r <invocation rate> (defaults to %d)\n"
                             "\t-s <message size> (defaults to %d)\n"
                             "\t-t <print stats> (defaults to %d)\n"
                             "\t-x <shutdown server> (defaults to %d)\n"
                             "\n",
                             argv[0],
                             test, PACED, THROUGHPUT, LATENCY,
                             enable_diffserv_code_points,
                             corba_priority,
                             do_dump_history,
                             count_missed_end_deadlines,
                             iterations,
                             ior,
                             print_missed_invocations,
                             test_protocol,
                             invocation_rate,
                             message_size,
                             print_statistics,
                             shutdown_server),
                            -1);
        }
      }

  return 0;
}

double
to_seconds (ACE_UINT64 hrtime,
            ACE_UINT32 sf)
{
  double seconds = static_cast<double> (
                     ACE_UINT64_DBLCAST_ADAPTER (hrtime / sf));
  seconds /= ACE_HR_SCALE_CONVERSION;

  return seconds;
}

ACE_UINT64
to_hrtime (double seconds,
           ACE_UINT32 sf)
{
  return ACE_UINT64 (seconds * sf * ACE_HR_SCALE_CONVERSION);
}

class Worker
{
public:
  Worker (CORBA::ORB_ptr orb,
          RTCORBA::RTORB_ptr rtorb,
          CORBA::PolicyManager_ptr policy_manager,
          test_ptr test);

  void run (void);

  void print_stats (void);

  void setup (void);

private:

  ACE_hrtime_t deadline_for_current_call (CORBA::ULong i);
  void missed_start_deadline (CORBA::ULong invocation);
  void missed_end_deadline (CORBA::ULong invocation);

  RTCORBA::RTORB_var rtorb_;
  CORBA::PolicyManager_var policy_manager_;
  test_var test_;
  ACE_Sample_History history_;
  ACE_hrtime_t interval_between_calls_;
  ACE_hrtime_t test_start_;
  ACE_hrtime_t test_end_;
  CORBA::ULong missed_start_deadlines_;
  CORBA::ULong missed_end_deadlines_;

  typedef ACE_Array_Base<CORBA::ULong> Missed_Invocations;
  Missed_Invocations missed_start_invocations_;
  Missed_Invocations missed_end_invocations_;

  CORBA::PolicyList base_protocol_policy_;
  CORBA::PolicyList test_protocol_policy_;

  CORBA::Long session_id_;
};

Worker::Worker (CORBA::ORB_ptr orb,
                RTCORBA::RTORB_ptr rtorb,
                CORBA::PolicyManager_ptr policy_manager,
                test_ptr test)
  : rtorb_ (RTCORBA::RTORB::_duplicate (rtorb)),
    policy_manager_ (CORBA::PolicyManager::_duplicate (policy_manager)),
    test_ (test::_duplicate (test)),
    history_ (iterations),
    interval_between_calls_ (),
    missed_start_deadlines_ (0),
    missed_end_deadlines_ (0),
    missed_start_invocations_ (iterations),
    missed_end_invocations_ (iterations)
{
  // Each sender will have a random session id.  This helps in
  // identifying late packets arriving at the server.
  ACE_OS::srand ((unsigned) ACE_OS::time (0));
  this->session_id_ = ACE_OS::rand ();

  // Interval is inverse of rate.
  this->interval_between_calls_ =
    to_hrtime (1 / double (invocation_rate), gsf);

  // Base protocol is used for setting up and tearing down the test.
  this->base_protocol_policy_.length (1);

  // Test protocol is the one being tested.
  this->test_protocol_policy_.length (1);

  RTCORBA::ProtocolProperties_var base_transport_protocol_properties =
    TAO_Protocol_Properties_Factory::create_transport_protocol_property (IOP::TAG_INTERNET_IOP,
                                                                         orb->orb_core ());

  RTCORBA::TCPProtocolProperties_var tcp_base_transport_protocol_properties =
    RTCORBA::TCPProtocolProperties::_narrow (base_transport_protocol_properties.in ());

  tcp_base_transport_protocol_properties->enable_network_priority (enable_diffserv_code_points);

  RTCORBA::ProtocolList protocols;
  protocols.length (1);
  protocols[0].transport_protocol_properties =
    base_transport_protocol_properties;
  protocols[0].orb_protocol_properties =
    RTCORBA::ProtocolProperties::_nil ();

  // IIOP is always used for the base protocol.
  protocols[0].protocol_type = IOP::TAG_INTERNET_IOP;

  // User decides the test protocol.
  this->base_protocol_policy_[0] =
    this->rtorb_->create_client_protocol_policy (protocols);

  if (ACE_OS::strcmp (test_protocol, ACE_TEXT ("DIOP")) == 0)
    {
      if (TAO_debug_level) ACE_DEBUG ((LM_DEBUG, "test protocol is DIOP\n"));
      protocols[0].protocol_type = TAO_TAG_DIOP_PROFILE;
    }
  else if (ACE_OS::strcmp (test_protocol, ACE_TEXT ("SCIOP")) == 0)
    {
      if (TAO_debug_level) ACE_DEBUG ((LM_DEBUG, "test protocol is SCIOP\n"));
      protocols[0].protocol_type = TAO_TAG_SCIOP_PROFILE;
    }
  else
    {
      if (TAO_debug_level) ACE_DEBUG ((LM_DEBUG, "test protocol is IIOP\n"));
      protocols[0].protocol_type = IOP::TAG_INTERNET_IOP;
    }

  RTCORBA::ProtocolProperties_var test_transport_protocol_properties =
    TAO_Protocol_Properties_Factory::create_transport_protocol_property (protocols[0].protocol_type,
                                                                         orb->orb_core ());

  if (protocols[0].protocol_type == TAO_TAG_DIOP_PROFILE)
    {
      RTCORBA::UserDatagramProtocolProperties_var udp_test_transport_protocol_properties =
        RTCORBA::UserDatagramProtocolProperties::_narrow (test_transport_protocol_properties.in ());

      udp_test_transport_protocol_properties->enable_network_priority (enable_diffserv_code_points);
    }
  else if (protocols[0].protocol_type == TAO_TAG_SCIOP_PROFILE)
    {
      RTCORBA::StreamControlProtocolProperties_var sctp_test_transport_protocol_properties =
        RTCORBA::StreamControlProtocolProperties::_narrow (test_transport_protocol_properties.in ());

      sctp_test_transport_protocol_properties->enable_network_priority (enable_diffserv_code_points);
    }
  else if (protocols[0].protocol_type == IOP::TAG_INTERNET_IOP)
    {
      RTCORBA::TCPProtocolProperties_var tcp_test_transport_protocol_properties =
        RTCORBA::TCPProtocolProperties::_narrow (test_transport_protocol_properties.in ());

      tcp_test_transport_protocol_properties->enable_network_priority (enable_diffserv_code_points);
    }

  protocols[0].transport_protocol_properties =
    test_transport_protocol_properties;

  this->test_protocol_policy_[0] =
    this->rtorb_->create_client_protocol_policy (protocols);
}

void
Worker::print_stats (void)
{
  CORBA::ULong missed_total_deadlines =
    this->missed_start_deadlines_ + this->missed_end_deadlines_;

  CORBA::ULong made_total_deadlines =
    iterations - missed_total_deadlines;

  ACE_DEBUG ((LM_DEBUG,
              "\n************ Statistics ************\n\n"));

  //
  // Senders-side stats for PACED invocations are not too relevant
  // since we are doing one way calls.
  //
  if (test_type == PACED)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Rate = %d/sec; Iterations = %d; ",
                  invocation_rate,
                  iterations));

      if (count_missed_end_deadlines)
        ACE_DEBUG ((LM_DEBUG,
                    "Deadlines made/missed[start,end]/%% = %d/%d[%d,%d]/%.2f%%; Effective Rate = %.2f\n",
                    made_total_deadlines,
                    missed_total_deadlines,
                    this->missed_start_deadlines_,
                    this->missed_end_deadlines_,
                    made_total_deadlines * 100 / (double) iterations,
                    made_total_deadlines / to_seconds (this->test_end_ - this->test_start_, gsf)));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Deadlines made/missed/%% = %d/%d/%.2f%%; Effective Rate = %.2f\n",
                    made_total_deadlines,
                    missed_total_deadlines,
                    made_total_deadlines * 100 / (double) iterations,
                    made_total_deadlines / to_seconds (this->test_end_ - this->test_start_, gsf)));

      if (print_missed_invocations)
        {
          ACE_DEBUG ((LM_DEBUG, "\nMissed start invocations are:\n"));

          for (CORBA::ULong j = 0;
               j < this->missed_start_deadlines_;
               ++j)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%d ",
                          this->missed_start_invocations_[j]));
            }

          ACE_DEBUG ((LM_DEBUG, "\n"));

          if (count_missed_end_deadlines)
            {
              ACE_DEBUG ((LM_DEBUG, "\nMissed end invocations are:\n"));

              for (CORBA::ULong j = 0;
                   j < this->missed_end_deadlines_;
                   ++j)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "%d ",
                              this->missed_end_invocations_[j]));
                }

              ACE_DEBUG ((LM_DEBUG, "\n"));
            }
        }
    }

  // Individual calls are relevant for the PACED and LATENCY tests.
  if (test_type == PACED ||
      test_type == LATENCY)
    {
      if (do_dump_history)
        {
          this->history_.dump_samples (ACE_TEXT("HISTORY"), gsf);
        }

      ACE_Basic_Stats stats;
      this->history_.collect_basic_stats (stats);
      stats.dump_results (ACE_TEXT("Total"), gsf);

      ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"), gsf,
                                             this->test_end_ - this->test_start_,
                                             iterations);
    }
  else
    {
      ACE_hrtime_t elapsed_time =
        this->test_end_ - this->test_start_;

      double seconds =
        to_seconds (elapsed_time, gsf);

      ACE_hrtime_t bits = iterations;
      bits *= message_size * 8;

      ACE_DEBUG ((LM_DEBUG,
                  "%Q bits sent in %5.1f seconds at a rate of %5.2f Mbps\n",
                  bits,
                  seconds,
                  ACE_UINT64_DBLCAST_ADAPTER (bits) / seconds / 1000 / 1000));
    }
}

ACE_hrtime_t
Worker::deadline_for_current_call (CORBA::ULong i)
{
  ACE_hrtime_t deadline_for_current_call =
    this->interval_between_calls_;

  deadline_for_current_call *= (i + 1);

  deadline_for_current_call += this->test_start_;

  return deadline_for_current_call;
}

void
Worker::missed_start_deadline (CORBA::ULong invocation)
{
  this->missed_start_invocations_[this->missed_start_deadlines_++] =
    invocation;
}

void
Worker::missed_end_deadline (CORBA::ULong invocation)
{
  if (count_missed_end_deadlines)
    this->missed_end_invocations_[this->missed_end_deadlines_++] =
      invocation;
}

void
Worker::setup (void)
{
  // Make sure we have a connection to the server using the test
  // protocol.
  this->policy_manager_->set_policy_overrides (this->test_protocol_policy_,
                                               CORBA::SET_OVERRIDE);

  // Since the network maybe unavailable temporarily, make sure to try
  // for a few times before giving up.
  for (int j = 0;;)
    {

    test_protocol_setup:

      try
        {
          // Send a message to ensure that the connection is setup.
          this->test_->oneway_sync ();

          goto test_protocol_success;
        }
      catch (const CORBA::TRANSIENT&)
        {
          ++j;

          if (j < number_of_connection_attempts)
            {
              ACE_OS::sleep (1);
              goto test_protocol_setup;
            }
        }

      ACE_ERROR ((LM_ERROR,
                  "Cannot setup test protocol\n"));

      ACE_OS::exit (-1);
    }

 test_protocol_success:

  // Use IIOP for setting up the test since the test protocol maybe
  // unreliable.
  this->policy_manager_->set_policy_overrides (this->base_protocol_policy_,
                                               CORBA::SET_OVERRIDE);

  // Since the network maybe unavailable temporarily, make sure to try
  // for a few times before giving up.
  for (int k = 0;;)
    {

    base_protocol_setup:

      try
        {
          // Let the server know what to expect..
          this->test_->start_test (this->session_id_,
                                   ACE_TEXT_ALWAYS_CHAR (test_protocol),
                                   invocation_rate,
                                   message_size,
                                   iterations);

          goto base_protocol_success;
        }
      catch (const CORBA::TRANSIENT&)
        {
          ACE_OS::sleep (1);

          if (k < number_of_connection_attempts)
            {
              ACE_OS::sleep (1);
              goto base_protocol_setup;
            }
        }

      ACE_ERROR ((LM_ERROR,
                  "Cannot setup base protocol\n"));

      ACE_OS::exit (-1);
    }

 base_protocol_success:

  return;
}

void
Worker::run (void)
{
  // Select the test protocol for these invocation.
  this->policy_manager_->set_policy_overrides (this->test_protocol_policy_,
                                               CORBA::SET_OVERRIDE);

  // Payload.
  ::test::octets_var payload (new ::test::octets);
  payload->length (message_size);

  CORBA::Octet *buffer =
    payload->get_buffer ();

  // Not necessary but good for debugging.
  ACE_OS::memset (buffer,
                  1,
                  message_size * sizeof (CORBA::Octet));

  // Record the start time of the test.
  this->test_start_ =
    ACE_OS::gethrtime ();

  // Test with several iterations.
  for (CORBA::ULong i = 0;
       i < iterations;
       ++i)
    {
      ACE_hrtime_t time_before_call = 0;
      ACE_hrtime_t deadline_for_current_call = 0;

      // For PACED and LATENCY, each sender call is individually
      // noted.
      if (test_type == PACED ||
          test_type == LATENCY)
        {
          time_before_call =
            ACE_OS::gethrtime ();

          // Pacing code.
          if (test_type == PACED)
            {
              deadline_for_current_call =
                this->deadline_for_current_call (i);

              if (time_before_call > deadline_for_current_call)
                {
                  this->missed_start_deadline (i);
                  continue;
                }
            }
        }

      // Use oneways for PACING and THROUGHPUT.
      if (test_type == PACED ||
          test_type == THROUGHPUT)
        {
          this->test_->oneway_method (this->session_id_,
                                      i,
                                      payload.in ());
        }
      else
        {
          // Use twoway calls for LATENCY.
          this->test_->twoway_method (this->session_id_,
                                      i,
                                      payload.inout ());
        }

      // For PACED and LATENCY, each sender call is individually
      // noted.
      if (test_type == PACED ||
          test_type == LATENCY)
        {
          ACE_hrtime_t time_after_call =
            ACE_OS::gethrtime ();

          if (test_type == LATENCY)
            this->history_.sample ((time_after_call - time_before_call) / 2);
          else
            this->history_.sample (time_after_call - time_before_call);

          if (test_type == PACED)
            {
              if (time_after_call > deadline_for_current_call)
                {
                  this->missed_end_deadline (i);
                  continue;
                }

              ACE_hrtime_t sleep_time =
                deadline_for_current_call - time_after_call;

              ACE_OS::sleep (ACE_Time_Value (0,
                                             long (to_seconds (sleep_time, gsf) *
                                                   ACE_ONE_SECOND_IN_USECS)));
            }
        }
    }

  // This call is used to ensure that all the THROUGHPUT related data
  // has reached the server.
  if (test_type == THROUGHPUT &&
      ACE_OS::strcmp (test_protocol, ACE_TEXT ("DIOP")) != 0)
    {
      this->test_->twoway_sync ();
    }

  // Record end time for the test.
  this->test_end_ = ACE_OS::gethrtime ();

  // Use IIOP to indicate end of test to server.
  this->policy_manager_->set_policy_overrides (this->base_protocol_policy_,
                                               CORBA::SET_OVERRIDE);

  // Tell server that the test is over.
  this->test_->end_test ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  gsf = ACE_High_Res_Timer::global_scale_factor ();

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rtorb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        orb->resolve_initial_references ("ORBPolicyManager");

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in ());

      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      // Resolve the Network priority Mapping Manager
      object =
        orb->resolve_initial_references ("NetworkPriorityMappingManager");

      RTCORBA::NetworkPriorityMappingManager_var mapping_manager =
        RTCORBA::NetworkPriorityMappingManager::_narrow (object.in ());

      // Initialize the custom priority mapping
      Custom_Network_Priority_Mapping *cnpm = 0;
      ACE_NEW_RETURN  (cnpm,
                       Custom_Network_Priority_Mapping,
                       -1);

      // Set the desired corba priority on the network mapping manager
      cnpm->corba_priority (corba_priority);

      // Load the custom network priority mapping object in the
      // network priority mapping manager. The user can thus add his
      // own priority mapping.
      mapping_manager->mapping (cnpm);

      object =
        orb->string_to_object (ior);

      test_var test =
        test::_narrow (object.in ());

      Worker worker (orb.in (),
                     rtorb.in (),
                     policy_manager.in (),
                     test.in ());

      worker.setup ();

      worker.run ();

      if (print_statistics)
        worker.print_stats ();

      if (shutdown_server)
        {
          test->shutdown ();
        }
      ACE_OS::sleep(1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Error!");
      return -1;
    }

  return 0;
}
