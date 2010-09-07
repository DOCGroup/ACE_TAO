// $Id$

#include "tao/ORB_Constants.h"
#include "tao/debug.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"
#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/Policy_ManagerC.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_time.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Containers_T.h"
#include "ace/Sample_History.h"
#include "Custom_Network_Priority_Mapping.h"
#include "Sender_exec.h"

static ACE_UINT32 gsf = 0;

double
to_seconds (ACE_UINT64 hrtime,
            ACE_UINT32 sf)
{
  double seconds =
#if defined ACE_LACKS_LONGLONG_T
    hrtime / sf;
#else  /* ! ACE_LACKS_LONGLONG_T */
  static_cast<double> (ACE_UINT64_DBLCAST_ADAPTER (hrtime / sf));
#endif /* ! ACE_LACKS_LONGLONG_T */
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
          Protocols::test_ptr test,
          ::CORBA::ULong iterations,
          ::CORBA::ULong invocation_rate,
          ::CORBA::Boolean count_missed_end_deadlines,
          ::CORBA::Boolean do_dump_history,
          ::CORBA::Boolean print_missed_invocations,
          ::CORBA::ULong message_size,
          ::CORBA::ULong test_protocol_tag,
          ::CORBA::ULong number_of_connection_attempts,
          ::CORBA::Boolean enable_diffserv_code_points,
          ::Protocols::Sender_Controller::Test_Type test_type);

  void run (void);

  void print_stats (void);

  void setup (void);

private:

  ACE_hrtime_t deadline_for_current_call (CORBA::ULong i);
  void missed_start_deadline (CORBA::ULong invocation);
  void missed_end_deadline (CORBA::ULong invocation);

  RTCORBA::RTORB_var rtorb_;
  CORBA::PolicyManager_var policy_manager_;
  Protocols::test_var test_;
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

  ::CORBA::ULong iterations_;
  ::CORBA::ULong invocation_rate_;
  ::CORBA::Boolean count_missed_end_deadlines_;
  ::CORBA::Boolean do_dump_history_;
  ::CORBA::Boolean print_missed_invocations_;
  ::CORBA::ULong message_size_;
  ::CORBA::ULong test_protocol_tag_;
  ::CORBA::ULong number_of_connection_attempts_;
  ::CORBA::Boolean enable_diffserv_code_points_;
  ::Protocols::Sender_Controller::Test_Type test_type_;
};

Worker::Worker (CORBA::ORB_ptr orb,
                RTCORBA::RTORB_ptr rtorb,
                CORBA::PolicyManager_ptr policy_manager,
                Protocols::test_ptr test,
                ::CORBA::ULong iterations,
                ::CORBA::ULong invocation_rate,
                ::CORBA::Boolean count_missed_end_deadlines,
                ::CORBA::Boolean do_dump_history,
                ::CORBA::Boolean print_missed_invocations,
                ::CORBA::ULong message_size,
                ::CORBA::ULong test_protocol_tag,
                ::CORBA::ULong number_of_connection_attempts,
                ::CORBA::Boolean enable_diffserv_code_points,
                ::Protocols::Sender_Controller::Test_Type test_type)
  : rtorb_ (RTCORBA::RTORB::_duplicate (rtorb)),
    policy_manager_ (CORBA::PolicyManager::_duplicate (policy_manager)),
    test_ (Protocols::test::_duplicate (test)),
    history_ (iterations),
    interval_between_calls_ (),
    missed_start_deadlines_ (0),
    missed_end_deadlines_ (0),
    missed_start_invocations_ (iterations),
    missed_end_invocations_ (iterations),
    iterations_ (iterations),
    invocation_rate_ (invocation_rate),
    count_missed_end_deadlines_ (count_missed_end_deadlines),
    do_dump_history_ (do_dump_history),
    print_missed_invocations_ (print_missed_invocations),
    message_size_ (message_size),
    test_protocol_tag_ (test_protocol_tag),
    number_of_connection_attempts_ (number_of_connection_attempts),
    enable_diffserv_code_points_ (enable_diffserv_code_points),
    test_type_ (test_type)
{
  // Each sender will have a random session id.  This helps in
  // identifying late packets arriving at the server.
  ACE_OS::srand ((unsigned) ACE_OS::time (0));
  this->session_id_ = ACE_OS::rand ();

  // Interval is inverse of rate.
  this->interval_between_calls_ =
    to_hrtime (1 / double (this->invocation_rate_), gsf);

  // Base protocol is used for setting up and tearing down the test.
  this->base_protocol_policy_.length (1);

  // Test protocol is the one being tested.
  this->test_protocol_policy_.length (1);

  RTCORBA::ProtocolProperties_var base_transport_protocol_properties =
    TAO_Protocol_Properties_Factory::create_transport_protocol_property (IOP::TAG_INTERNET_IOP,
                                                                         orb->orb_core ());

  RTCORBA::TCPProtocolProperties_var tcp_base_transport_protocol_properties =
    RTCORBA::TCPProtocolProperties::_narrow (base_transport_protocol_properties.in ());

  tcp_base_transport_protocol_properties->enable_network_priority (this->enable_diffserv_code_points_);

  RTCORBA::ProtocolList protocols;
  protocols.length (1);
  protocols[0].transport_protocol_properties =
    base_transport_protocol_properties;
  protocols[0].orb_protocol_properties =
    RTCORBA::ProtocolProperties::_nil ();

  // IIOP is always used for the base protocol.
  protocols[0].protocol_type = IOP::TAG_INTERNET_IOP;

  this->base_protocol_policy_[0] =
    this->rtorb_->create_client_protocol_policy (protocols);

  // User decides the test protocol.
  protocols[0].protocol_type = test_protocol_tag;

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
    this->iterations_ - missed_total_deadlines;

  ACE_DEBUG ((LM_DEBUG,
              "\n************ Statistics ************\n\n"));

  //
  // Senders-side stats for PACED invocations are not too relevant
  // since we are doing one way calls.
  //
  if (this->test_type_ == ::Protocols::Sender_Controller::PACED)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Rate = %d/sec; Iterations = %d; ",
                  this->invocation_rate_,
                  this->iterations_));

      if (this->count_missed_end_deadlines_)
        ACE_DEBUG ((LM_DEBUG,
                    "Deadlines made/missed[start,end]/%% = %d/%d[%d,%d]/%.2f%%; Effective Rate = %.2f\n",
                    made_total_deadlines,
                    missed_total_deadlines,
                    this->missed_start_deadlines_,
                    this->missed_end_deadlines_,
                    made_total_deadlines * 100 / (double) this->iterations_,
                    made_total_deadlines / to_seconds (this->test_end_ - this->test_start_, gsf)));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Deadlines made/missed/%% = %d/%d/%.2f%%; Effective Rate = %.2f\n",
                    made_total_deadlines,
                    missed_total_deadlines,
                    made_total_deadlines * 100 / (double) this->iterations_,
                    made_total_deadlines / to_seconds (this->test_end_ - this->test_start_, gsf)));

      if (this->print_missed_invocations_)
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

          if (this->count_missed_end_deadlines_)
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
  if (this->test_type_ == ::Protocols::Sender_Controller::PACED ||
      this->test_type_ == ::Protocols::Sender_Controller::LATENCY)
    {
      if (this->do_dump_history_)
        {
          this->history_.dump_samples (ACE_TEXT("HISTORY"), gsf);
        }

      ACE_Basic_Stats stats;
      this->history_.collect_basic_stats (stats);
      stats.dump_results (ACE_TEXT("Total"), gsf);

      ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"), gsf,
                                             this->test_end_ - this->test_start_,
                                             this->iterations_);
    }
  else
    {
      ACE_hrtime_t elapsed_time =
        this->test_end_ - this->test_start_;

      double seconds =
        to_seconds (elapsed_time, gsf);

      ACE_hrtime_t bits = this->iterations_;
      bits *= this->message_size_ * 8;

      ACE_DEBUG ((LM_DEBUG,
                  "%Q bits sent in %5.1f seconds at a rate of %5.2f Mbps\n",
                  bits,
                  seconds,
                  bits / seconds / 1000 / 1000));
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
  if (this->count_missed_end_deadlines_)
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
  for (CORBA::ULong j = 0;;)
    {
      try
        {
          // Send a message to ensure that the connection is setup.
          this->test_->oneway_sync ();

          break;
        }
      catch (CORBA::TRANSIENT &)
        {
          ++j;

          if (j < this->number_of_connection_attempts_)
            {
              ACE_OS::sleep (1);

              continue;
            }
        }

      ACE_ERROR ((LM_ERROR,
                  "Cannot setup test protocol\n"));

      ACE_OS::exit (-1);
    }

  const char *test_protocol = 0;
  if (this->test_protocol_tag_ == IOP::TAG_INTERNET_IOP)
    test_protocol = "IIOP";
  else if (this->test_protocol_tag_ == TAO_TAG_DIOP_PROFILE)
    test_protocol = "DIOP";
  else if (this->test_protocol_tag_ == TAO_TAG_SCIOP_PROFILE)
    test_protocol = "SCIOP";

  // Use IIOP for setting up the test since the test protocol maybe
  // unreliable.
  this->policy_manager_->set_policy_overrides (this->base_protocol_policy_,
                                               CORBA::SET_OVERRIDE);

  // Since the network maybe unavailable temporarily, make sure to try
  // for a few times before giving up.
  for (CORBA::ULong k = 0;;)
    {
      try
        {
          // Let the server know what to expect..
          this->test_->start_test (this->session_id_,
                                   test_protocol,
                                   this->invocation_rate_,
                                   this->message_size_,
                                   this->iterations_);

          break;
        }
      catch (CORBA::TRANSIENT &)
        {
          ACE_OS::sleep (1);

          if (k < this->number_of_connection_attempts_)
            {
              ACE_OS::sleep (1);

              continue;
            }
        }

      ACE_ERROR ((LM_ERROR,
                  "Cannot setup base protocol\n"));

      ACE_OS::exit (-1);
    }

  return;
}

void
Worker::run (void)
{
  // Select the test protocol for these invocation.
  this->policy_manager_->set_policy_overrides (this->test_protocol_policy_,
                                               CORBA::SET_OVERRIDE);

  // Payload.
  ::Protocols::test::octets_var payload (new ::Protocols::test::octets);
  payload->length (this->message_size_);

  CORBA::Octet *buffer =
    payload->get_buffer ();

  // Not necessary but good for debugging.
  ACE_OS::memset (buffer,
                  1,
                  this->message_size_ * sizeof (CORBA::Octet));

  // Record the start time of the test.
  this->test_start_ =
    ACE_OS::gethrtime ();

  // Test with several iterations.
  for (CORBA::ULong i = 0;
       i < this->iterations_;
       ++i)
    {
      ACE_hrtime_t time_before_call = 0;
      ACE_hrtime_t deadline_for_current_call = 0;

      // For PACED and LATENCY, each sender call is individually
      // noted.
      if (this->test_type_ == ::Protocols::Sender_Controller::PACED ||
          this->test_type_ == ::Protocols::Sender_Controller::LATENCY)
        {
          time_before_call =
            ACE_OS::gethrtime ();

          // Pacing code.
          if (this->test_type_ == ::Protocols::Sender_Controller::PACED)
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
      if (this->test_type_ == ::Protocols::Sender_Controller::PACED ||
          this->test_type_ == ::Protocols::Sender_Controller::THROUGHPUT)
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
      if (this->test_type_ == ::Protocols::Sender_Controller::PACED ||
          this->test_type_ == ::Protocols::Sender_Controller::LATENCY)
        {
          ACE_hrtime_t time_after_call =
            ACE_OS::gethrtime ();

          if (this->test_type_ == ::Protocols::Sender_Controller::LATENCY)
            this->history_.sample ((time_after_call - time_before_call) / 2);
          else
            this->history_.sample (time_after_call - time_before_call);

          if (this->test_type_ == ::Protocols::Sender_Controller::PACED)
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
  if (this->test_type_ == ::Protocols::Sender_Controller::THROUGHPUT &&
      this->test_protocol_tag_ != TAO_TAG_DIOP_PROFILE)
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

CIDL_SenderImpl::SenderExec_i::SenderExec_i (void)
{
  ACE_DEBUG ((LM_DEBUG, "CIDL_SenderImpl::SenderExec_i::SenderExec_i\n"));
}

void
CIDL_SenderImpl::SenderExec_i::start (
    ::CORBA::ULong iterations,
    ::CORBA::ULong invocation_rate,
    ::CORBA::Boolean count_missed_end_deadlines,
    ::CORBA::Boolean do_dump_history,
    ::CORBA::Boolean print_missed_invocations,
    ::CORBA::ULong message_size,
    ::CORBA::ULong test_protocol_tag,
    ::CORBA::Boolean print_statistics,
    ::CORBA::ULong number_of_connection_attempts,
    ::CORBA::Boolean enable_diffserv_code_points,
    ::CORBA::Short priority,
    ::Protocols::Sender_Controller::Test_Type test_type
  )
{
  ACE_DEBUG ((LM_DEBUG, "CIDL_SenderImpl::SenderExec_i::start\n"));

  gsf = ACE_High_Res_Timer::global_scale_factor ();

  int argc = 0;
  char **argv = 0;

  this->orb_ =
    CORBA::ORB_init (argc, argv);

  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RTORB");

  RTCORBA::RTORB_var rtorb =
    RTCORBA::RTORB::_narrow (object.in ());

  object =
    this->orb_->resolve_initial_references ("ORBPolicyManager");

  CORBA::PolicyManager_var policy_manager =
    CORBA::PolicyManager::_narrow (object.in ());

  object =
    this->orb_->resolve_initial_references ("NetworkPriorityMappingManager");

  RTCORBA::NetworkPriorityMappingManager_var mapping_manager =
    RTCORBA::NetworkPriorityMappingManager::_narrow (object.in ());

  Custom_Network_Priority_Mapping *custom_network_priority_mapping =
    new Custom_Network_Priority_Mapping;

  // Set the desired corba priority on the network mapping manager
  custom_network_priority_mapping->corba_priority (priority);

  mapping_manager->mapping (custom_network_priority_mapping);

  Protocols::test_var test =
    this->context_->get_connection_reader ();

  Worker worker (this->orb_.in (),
                 rtorb.in (),
                 policy_manager.in (),
                 test.in (),
                 iterations,
                 invocation_rate,
                 count_missed_end_deadlines,
                 do_dump_history,
                 print_missed_invocations,
                 message_size,
                 test_protocol_tag,
                 number_of_connection_attempts,
                 enable_diffserv_code_points,
                 test_type);

  worker.setup ();

  worker.run ();

  if (print_statistics)
    worker.print_stats ();
}

void
CIDL_SenderImpl::SenderExec_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "CIDL_SenderImpl::SenderExec_i::shutdown\n"));
  this->orb_->shutdown ();
}

void
CIDL_SenderImpl::SenderExec_i::set_session_context (
    Components::SessionContext_ptr ctx
  )
{
  ACE_DEBUG ((LM_DEBUG, "CIDL_SenderImpl::SenderExec_i::set_session_context\n"));

  this->context_ =
    ::CIAO_Protocols_Sender_Impl::Sender_Exec_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    throw CORBA::INTERNAL ();
}

void
CIDL_SenderImpl::SenderExec_i::ccm_activate (void)
{
  ACE_DEBUG ((LM_DEBUG, "CIDL_SenderImpl::SenderExec_i::ccm_activate\n"));
}

void
CIDL_SenderImpl::SenderExec_i::ccm_passivate (void)
{
  ACE_DEBUG ((LM_DEBUG, "CIDL_SenderImpl::SenderExec_i::ccm_passivate\n"));
}

void
CIDL_SenderImpl::SenderExec_i::ccm_remove (void)
{
  ACE_DEBUG ((LM_DEBUG, "CIDL_SenderImpl::SenderExec_i::ccm_remove\n"));
}


void
CIDL_SenderImpl::SenderExec_i::configuration_complete (void)
{
  ACE_DEBUG ((LM_DEBUG, "CIDL_SenderImpl::SenderExec_i::configuration_complete\n"));
}

::Components::EnterpriseComponent_ptr
CIDL_SenderImpl::SenderHomeExec_i::create (void)
{
  ACE_DEBUG ((LM_DEBUG, "CIDL_SenderImpl::SenderHome_exec::create\n"));
  return new CIDL_SenderImpl::SenderExec_i;
}

extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
createSenderHome_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "createSenderHome_Impl\n"));
  return new CIDL_SenderImpl::SenderHomeExec_i;
}
