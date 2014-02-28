// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Sample_History.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "tao/debug.h"
#include "testS.h"

static const ACE_TCHAR *ior_file = ACE_TEXT ("receiver.ior");
static int do_dump_history = 0;
static int print_missed_invocations = 0;
static ACE_High_Res_Timer::global_scale_factor_type gsf = 0;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d:f:m:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        do_dump_history = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'f':
        ior_file = get_opts.opt_arg ();
        break;

      case 'm':
        print_missed_invocations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s\n"
                           "\t-d <show history> (defaults to %d)\n"
                           "\t-f <ior_file> (defaults to %s)\n"
                           "\t-m <print missed invocations for paced workers> (defaults to %d)\n"
                           "\n",
                           argv[0],
                           do_dump_history,
                           ior_file,
                           print_missed_invocations),
                          -1);
      }

  return 0;
}

class test_i :
  public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  ~test_i (void);

  void start_test (CORBA::Long session_id,
                   const char *protocol,
                   CORBA::ULong invocation_rate,
                   CORBA::ULong message_size,
                   CORBA::ULong iterations);

  void end_test (void);

  void oneway_sync (void);

  void twoway_sync (void);

  void oneway_method (CORBA::Long session_id,
                      CORBA::ULong iteration,
                      const ::test::octets &payload);

  void twoway_method (CORBA::Long &session_id,
                      CORBA::ULong &iteration,
                      ::test::octets &payload);

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (void);
  //FUZZ: enable check_for_lack_ACE_OS

  PortableServer::POA_ptr _default_POA (void);

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  typedef ACE_Array_Base<CORBA::Boolean> Invocations;
  Invocations invocations_received_;
  ACE_hrtime_t time_of_last_call_;
  ACE_hrtime_t test_start_;
  ACE_hrtime_t test_end_;
  CORBA::Boolean first_invocation_;
  ACE_Sample_History *inter_arrival_times_;
  CORBA::ULong iterations_;
  CORBA::ULong number_of_invocations_received_;
  CORBA::Long session_id_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    inter_arrival_times_ (0),
    iterations_ (0),
    session_id_ (-1)
{
}

test_i::~test_i (void)
{
}

void
test_i::start_test (CORBA::Long session_id,
                    const char *protocol,
                    CORBA::ULong invocation_rate,
                    CORBA::ULong message_size,
                    CORBA::ULong iterations)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Session id starts %d\n",
                  session_id));
    }

  ACE_DEBUG ((LM_DEBUG,
              "Protocol = %5s Invocation Rate = %3d Message Size = %5d Expected Latency = %4d ",
              ACE_TEXT_CHAR_TO_TCHAR (protocol),
              invocation_rate,
              message_size,
              1000 / invocation_rate));

  // Remember test parameters.
  this->session_id_ = session_id;
  this->iterations_ = iterations;
  this->number_of_invocations_received_ = 0;

  //
  // Initialize counters and tables.
  //
  this->inter_arrival_times_ =
    new ACE_Sample_History (iterations);

  this->first_invocation_ = 1;

  this->invocations_received_.size (iterations);
  for (CORBA::ULong i = 0;
       i < iterations;
       ++i)
    this->invocations_received_[i] = 0;

  // Record start time.
  this->test_start_ =
    ACE_OS::gethrtime ();
}

void
test_i::end_test (void)
{
  // Record end time.
  this->test_end_ =
    ACE_OS::gethrtime ();

  if (do_dump_history)
    {
      this->inter_arrival_times_->dump_samples (ACE_TEXT("Inter-arrival times"), gsf);
    }

  ACE_Basic_Stats stats;
  this->inter_arrival_times_->collect_basic_stats (stats);

  ACE_DEBUG ((LM_DEBUG,
              "Max Latency = %6d ",
              stats.max_ / gsf / 1000));

  ACE_DEBUG ((LM_DEBUG,
              "Invocations expected / received / missed / missed %% = %6d / %6d / %6d / %5.2f\n",
              this->iterations_,
              this->number_of_invocations_received_,
              this->iterations_ - this->number_of_invocations_received_,
              (this->iterations_ - this->number_of_invocations_received_) / (double) this->iterations_ * 100));

  if (print_missed_invocations)
    {
      ACE_DEBUG ((LM_DEBUG, "\nFollowing invocations were never received:\n"));

      for (CORBA::ULong i = 0;
           i < this->iterations_;
           ++i)
        {
          if (this->invocations_received_[i] == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%d ",
                          i));
            }
        }

      ACE_DEBUG ((LM_DEBUG, "\n"));
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Session id ends %d\n",
                  this->session_id_));

      stats.dump_results (ACE_TEXT("Inter-arrival times"), gsf);

      ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Inter-arrival times"),
                                             gsf,
                                             this->test_end_ - this->test_start_,
                                             stats.samples_count ());
    }

  this->session_id_ = -1;

  delete this->inter_arrival_times_;
}

void
test_i::oneway_sync (void)
{
}

void
test_i::twoway_sync (void)
{
}

void
test_i::oneway_method (CORBA::Long session_id,
                       CORBA::ULong iteration,
                       const ::test::octets &payload)
{
  if (this->session_id_ != session_id)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Late message with iteration id = %d: will not count message\n",
                  iteration));
      return;
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "test_i::oneway_method -> session id = %d iteration = %d payload size = %d\n",
                  session_id,
                  iteration,
                  payload.length ()));
    }

  this->invocations_received_[iteration] = 1;
  ++this->number_of_invocations_received_;

  ACE_hrtime_t time_of_current_call =
    ACE_OS::gethrtime ();

  if (this->first_invocation_)
    this->first_invocation_ = 0;
  else
    this->inter_arrival_times_->sample (time_of_current_call - this->time_of_last_call_);

  this->time_of_last_call_ = time_of_current_call;
}

void
test_i::twoway_method (CORBA::Long &session_id,
                       CORBA::ULong &iteration,
                       ::test::octets &payload)
{
  if (this->session_id_ != session_id)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Late message with iteration id = %d: will not count message\n",
                  iteration));
      return;
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "test_i::twoway_method -> session id = %d iteration = %d payload size = %d\n",
                  session_id,
                  iteration,
                  payload.length ()));
    }

  this->invocations_received_[iteration] = 1;
  ++this->number_of_invocations_received_;

  ACE_hrtime_t time_of_current_call =
    ACE_OS::gethrtime ();

  if (this->first_invocation_)
    this->first_invocation_ = 0;
  else
    this->inter_arrival_times_->sample (time_of_current_call - this->time_of_last_call_);

  this->time_of_last_call_ = time_of_current_call;
}

PortableServer::POA_ptr
test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::shutdown\n"));

  this->orb_->shutdown (0);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  gsf = ACE_High_Res_Timer::global_scale_factor ();

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      test_i *servant =
        new test_i (orb.in (),
        root_poa.in ());

      PortableServer::ServantBase_var safe_servant (servant);

      test_var test =
        servant->_this ();

      CORBA::String_var ior =
        orb->object_to_string (test.in ());

      FILE *output_file =
        ACE_OS::fopen (ior_file, "w");
      ACE_ASSERT (output_file != 0);

      u_int result =
        ACE_OS::fprintf (output_file,
                         "%s",
                         ior.in ());
      ACE_ASSERT (result == ACE_OS::strlen (ior.in ()));
      ACE_UNUSED_ARG (result);

      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();
      ACE_OS::sleep(1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
