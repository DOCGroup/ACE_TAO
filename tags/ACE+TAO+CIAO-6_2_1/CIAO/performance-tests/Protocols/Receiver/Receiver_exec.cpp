// $Id$

#include "Receiver_exec.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "tao/debug.h"

static int do_dump_history = 0;
static int print_missed_invocations = 0;
static ACE_High_Res_Timer::global_scale_factor_type gsf = 0;

namespace CIDL_ReceiverImpl
{
  test_i::test_i (void)
    : inter_arrival_times_ (0),
      iterations_ (0),
      session_id_ (-1)
  {
    gsf = ACE_High_Res_Timer::global_scale_factor ();
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
                protocol,
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

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Inter-arrival times"), gsf,
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
                         const Protocols::test::octets &payload)
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
                         Protocols::test::octets &payload)
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

  ReceiverExec_i::ReceiverExec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, "ReceiverExec_i::ReceiverExec_i\n"));

    int argc = 0;
    char **argv = 0;

    this->orb_ =
      CORBA::ORB_init (argc, argv);
  }

  ::Protocols::CCM_test_ptr
  ReceiverExec_i::get_writer (void)
  {
    ACE_DEBUG ((LM_DEBUG, "ReceiverExec_i::get_writer\n"));
    return new test_i;
  }

  void
  ReceiverExec_i::shutdown (void)
  {
    ACE_DEBUG ((LM_DEBUG, "ReceiverExec_i::shutdown\n"));
    this->orb_->shutdown ();
  }

  void
  ReceiverExec_i::set_session_context (Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, "ReceiverExec_i::set_session_context\n"));

    this->context_ =
      ::Protocols::CCM_Receiver_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
      throw CORBA::INTERNAL ();
  }

  void
  ReceiverExec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "ReceiverExec_i::ccm_activate\n"));
  }

  void
  ReceiverExec_i::ccm_passivate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "ReceiverExec_i::ccm_passivate\n"));
  }

  void
  ReceiverExec_i::ccm_remove (void)
  {
    ACE_DEBUG ((LM_DEBUG, "ReceiverExec_i::ccm_remove\n"));
  }

  void
  ReceiverExec_i::configuration_complete (void)
  {
    ACE_DEBUG ((LM_DEBUG, "ReceiverExec_i::configuration_complete\n"));
  }

  ::Components::EnterpriseComponent_ptr
  ReceiverHomeExec_i::create (void)
  {
    ACE_DEBUG ((LM_DEBUG, "ReceiverHome_exec::create\n"));
    return new ReceiverExec_i;
  }

  extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createReceiverHome_Impl (void)
  {
    ACE_DEBUG ((LM_DEBUG, "createReceiverHome_Impl\n"));
    return new ReceiverHomeExec_i;
  }
}
