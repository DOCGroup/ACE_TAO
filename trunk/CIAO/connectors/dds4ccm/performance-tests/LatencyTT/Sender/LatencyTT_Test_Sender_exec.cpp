// -*- C++ -*-
// $Id$

#include "LatencyTT_Test_Sender_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"
#include "ace/High_Res_Timer.h"

namespace CIAO_LatencyTT_Test_Sender_Impl
{

  //============================================================
  // LatencyTTTestSec_Listener_exec_i
  //============================================================
  LatencyTTTestSec_Listener_exec_i::LatencyTTTestSec_Listener_exec_i (
            Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  LatencyTTTestSec_Listener_exec_i::~LatencyTTTestSec_Listener_exec_i (void)
  {
  }

  void
  LatencyTTTestSec_Listener_exec_i::on_one_data (
                                  const LatencyTTTestSec & an_instance,
                                  const ::CCM_DDS::ReadInfo &)
  {
    ACE_UINT64  receive_time = 0;

    // Only interested in messages received with a latency_ping = 0
    // (messages sent back by receiver)
    if( an_instance.ping == 0)
      {
        ACE_High_Res_Timer::gettimeofday_hr ().to_usec ( receive_time);
        this->callback_.read(const_cast<LatencyTTTestSec&> (an_instance), receive_time);
      }
   }

  void
  LatencyTTTestSec_Listener_exec_i::on_many_data (
                                  const LatencyTTTestSecSeq & /*an_instance*/,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
  }
  //============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (
                                        Atomic_Boolean &matched,
                                        int number_of_subscribers,
                                        Sender_exec_i &callback)
   : callback_ (callback),
    matched_ (matched),
    number_of_subscribers_ (number_of_subscribers)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr /*the_topic*/,
     const DDS::InconsistentTopicStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::SampleRejectedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedDeadlineMissedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr the_entity,
    ::DDS::StatusKind  status_kind)
  {
    CORBA::ULong kind = status_kind;
    if((! ::CORBA::is_nil(the_entity)) &&
       (kind==DDS::PUBLICATION_MATCHED_STATUS))
      {
        ::DDS::PublicationMatchedStatus_var stat;
        DDS::DataWriter_var wr = ::DDS::DataWriter::_narrow(the_entity);
        if(::CORBA::is_nil(wr))
         {
            throw ::CORBA::INTERNAL ();
         }
        ::DDS::ReturnCode_t retval = wr->get_publication_matched_status(stat.out ());
        if (retval == DDS::RETCODE_OK)
          {

            if((stat.in().current_count >=
             (this->number_of_subscribers_ + 1)) &&
             !this->matched_.value())
            {
              this->matched_ = true;
              this->callback_.start();
            }
          }
      }
  }
  //============================================================
  // WriteTickerHandler
  //============================================================
  WriteTicker::WriteTicker (Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  WriteTicker::handle_timeout (const ACE_Time_Value &, const void *)
  {
    // Notify the subscribers
    this->callback_.write_one ();
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (1000),
      datalen_(100),
      datalen_idx_(0),
      nr_of_runs_(10),
      sleep_(10),
      matched_(false),
      number_of_subscribers_(1),
      tv_total_ (0L),
      tv_max_ (0L),
      tv_min_ (0L),
      count_ (0),  // Number of returned messages.
      number_of_msg_(0), // Number of sent messages.
      timer_(false),
      received_(false),
      seq_num_(0),
      sigma_duration_squared_(0)
  {
    this->ticker_ = new WriteTicker (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  static int compare_two_longs(const void *long1, const void *long2)
  {
    return (*(CORBA::Long*)long1 - *(CORBA::Long*)long2 );
  }

  void
  Sender_exec_i::write_one (void)
  {
    if((this->number_of_msg_ == 0) && (this->datalen_idx_ == 0))
      {
        ACE_High_Res_Timer::gettimeofday_hr ().to_usec (this->start_time_test_);
      }
    // First message sent always, next messages only as previous sent message
    // is received back.
    if( (this->number_of_msg_ == 0) || ( this->received_.value()))
    {
      // All messages send, stop timer.
      if((this->iterations_ != 0) &&
         (this->number_of_msg_ >= this->iterations_ ))
        {
           if( this->datalen_idx_ >= (this->nr_of_runs_ - 1))
            {
              this->stop();
              this->timer_ = false;
              this->calc_results();
              ACE_High_Res_Timer::gettimeofday_hr ().to_usec (this->end_time_test_);

             }
          else
            {
              this->calc_results();
              this->reset_results();
              ++this->datalen_idx_;
              this->datalen_ = this->datalen_range_[this->datalen_idx_];
              this->test_topic_.data.length(this->datalen_);
            }
        }
      else
        {
        try
          {
            // Send messages with indicator (ping = 1L) so that subscriber knows
            // that this message has to sent back.
            this->test_topic_.ping = 1L;
            this->test_topic_.seq_num =  this->number_of_msg_;

            // Keep last sent seq_num, to control if message is sent back.
            this->seq_num_ = this->number_of_msg_;
            this->received_ = false;
            ACE_High_Res_Timer::gettimeofday_hr ().to_usec (this->start_time_);
            this->writer_->write_one (this->test_topic_, ::DDS::HANDLE_NIL);
          }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                        ACE_TEXT ("while wrinting sample with sequence_number <%u>.\n"),
                        this->test_topic_.seq_num));
          }
        ++this->number_of_msg_;
       }
    }
  }

  void
  Sender_exec_i::read(LatencyTTTestSec & an_instance,ACE_UINT64  receive_time)
  {
    if (an_instance.seq_num == this->seq_num_.value())
    {
      this->record_time( receive_time);
      this->received_ = true;
    }
  }

  void
  Sender_exec_i::reset_results()
  {
    this->count_ = 0;
    this->duration_times = new CORBA::Long[this->iterations_];
    this->tv_total_ = 0L;
    this->tv_max_ = 0L;
    this->tv_min_ = 0L;
    this->number_of_msg_ = 0;
    this->received_ = false;
    this->seq_num_ = 0;
    this->sigma_duration_squared_ = 0;
  }

  void
  Sender_exec_i::calc_results()
  {
    // Sort all duration times.
    qsort(this->duration_times,
          this->count_,
          sizeof(CORBA::Long),
          compare_two_longs);

    // Show latency_50_percentile, latency_90_percentile,
    // latency_99_percentile and latency_99.99_percentile.
    // For example duration_times[per50] is the median i.e. 50% of the
    // samples have a latency time  <=  duration_times[per50]
    int per50 = this->count_/2;
    int per90 = (int)(this->count_ * 0.90);
    int per99 = (int)(this->count_ * 0.990);
    int per9999 = (int)(this->count_ * 0.9999);

    double avg = this->tv_total_.value () / this->count_;
    // Calculate standard deviation.
    double _roundtrip_time_std  = sqrt(
        (this->sigma_duration_squared_ / (double)this->count_) -
        (avg * avg));

    // Show values as float, in order to be comparable with RTI performance test.
    if( this->count_ > 0)
      {
        if( this->datalen_idx_ == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
             "Collecting statistics on %d samples per message size.\n"
             "This is the roundtrip time, *not* the one-way-latency\n"
             "bytes ,stdev us,ave us, min us, 50%% us, 90%% us, 99%% us, 99.99%%,"
             " max us\n"
             "------,-------,-------,-------,-------,-------,-------,-------,"
              "-------\n"
             "%6d,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f\n",
              this->count_,
              this->datalen_,
              _roundtrip_time_std,
              avg,
              (double)this->tv_min_.value (),
              (double)this->duration_times[per50-1],
              (double)this->duration_times[per90-1],
              (double)this->duration_times[per99-1],
              (double)this->duration_times[per9999-1],
              (double)this->tv_max_.value ()));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
              "%6d,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f\n",
              this->datalen_,
              _roundtrip_time_std,
              avg,
              (double)this->tv_min_.value (),
              (double)this->duration_times[per50-1],
              (double)this->duration_times[per90-1],
              (double)this->duration_times[per99-1],
              (double)this->duration_times[per9999-1],
              (double)this->tv_max_.value ()));
          }
       }
     else
       {
         ACE_ERROR ((LM_ERROR, "SUMMARY SENDER latency time:\n "
                               "No samples received back.\n"));
       }
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (
      this->matched_,
      this->number_of_subscribers_,
      *this);
  }

  ::CCM_DDS::LatencyTTTestSec::CCM_Listener_ptr
  Sender_exec_i::get_ping_listen_data_listener (void)
  {
    return new LatencyTTTestSec_Listener_exec_i (*this);
  }

   ::CCM_DDS::CCM_PortStatusListener_ptr
  Sender_exec_i::get_ping_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  void
  Sender_exec_i::start (void)
  {
    // This->sleep_ is in ms
    unsigned int sec = this->sleep_/1000;
    unsigned int usec = (this->sleep_ % 1000) * 1000;
    (void) ACE_High_Res_Timer::global_scale_factor ();
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->timer_queue()->gettimeofday (&ACE_High_Res_Timer::gettimeofday_hr);
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer(
                    this->ticker_,
                    0,
                    ACE_Time_Value (5, 0),
                    ACE_Time_Value (sec, usec)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                              ACE_TEXT ("Error scheduling timer")));
      }
      this->timer_ = true;
   }

void
Sender_exec_i::record_time (ACE_UINT64  receive_time)
  {
    ACE_UINT64 interval =  ( receive_time  - this->start_time_);
    ++this->count_;
    long duration = static_cast <CORBA::Long>(interval) - this->_clock_overhead_;
    int i = this->count_;
    this->duration_times[i-1] = duration;
    this->sigma_duration_squared_ += (double)duration * (double)duration;
    this->tv_total_ += duration;
    if (duration > this->tv_max_.value ()|| (this->tv_max_.value () == 0L))
      this->tv_max_ = duration;
    if (duration < this->tv_min_.value () || (this->tv_min_.value () == 0L))
      this->tv_min_ = duration;
  }

  ::CORBA::ULong
  Sender_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Sender_exec_i::iterations (::CORBA::ULong iterations)
  {
    if (iterations == 0)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: Number of iterations must be greater as '0'\n")));
        throw ::CORBA::BAD_PARAM ();
      }
    else
    {
      this->iterations_ = iterations;
    }
  }

  ::CORBA::UShort
  Sender_exec_i::sleep (void)
  {
    return this->sleep_;
  }

  void
  Sender_exec_i::sleep (::CORBA::UShort sleep)
  {
    this->sleep_ = sleep;
  }

  ::CORBA::UShort
  Sender_exec_i::number_of_sub (void)
  {
    return this->number_of_subscribers_;
  }

  void
  Sender_exec_i::number_of_sub (::CORBA::UShort number_of_sub)
  {
    if (number_of_sub > 0)
      {
        this->number_of_subscribers_ = number_of_sub;
      }
    else
      {
        this->number_of_subscribers_ = 1;
      }
  }

  void
  Sender_exec_i::calculate_clock_overhead()
  {
    int num_of_loops_clock = 320;
    ACE_UINT64 begin_time;
    ACE_UINT64 clock_roundtrip_time;
    ACE_High_Res_Timer::gettimeofday_hr ().to_usec (begin_time);
    for (int i = 0; i < num_of_loops_clock; ++i)
      {
        ACE_High_Res_Timer::gettimeofday_hr ().to_usec (clock_roundtrip_time);
      }
    ACE_UINT64 total_time =  clock_roundtrip_time - begin_time;
    this->_clock_overhead_ = (long)(total_time /num_of_loops_clock);
  }

  void
  Sender_exec_i::init_values (void)
  {
    this->duration_times = new CORBA::Long[this->iterations_];
    this->datalen_range_ = new CORBA::Short[this->nr_of_runs_];
    int start = 16;
    for(int i = 0; i < this->nr_of_runs_; i++)
      {
        this->datalen_range_[i] = start;
        start = 2 * start;
      }

    this->datalen_ = this->datalen_range_[0];

    //make  instances of Topic
    this->test_topic_.seq_num = 0;
    this->test_topic_.ping = 0;
    this->test_topic_.data.length (this->datalen_);
    calculate_clock_overhead();
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::LatencyTT_Test::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    try
      {
        this->writer_ = this->context_->get_connection_info_write_data ();
        ::CCM_DDS::DataListenerControl_var dlc =
            this->context_->get_connection_ping_listen_data_control ();
        dlc->mode (::CCM_DDS::ONE_BY_ONE);
    }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::ccm_activate: Exception caught\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::ccm_activate: Unknown exception caught\n")));
      }
    this->init_values();
  }

  void
  Sender_exec_i::stop (void)
  {
    if (this->timer_.value ())
     {
       this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
       delete this->ticker_;
    }
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->stop();
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if((this->nr_of_runs_ -1) != this->datalen_idx_)
      {
        ACE_DEBUG ((LM_DEBUG, "SUMMARY SENDER : %u of %u runs completed.\n"
                             " Number of messages sent of last run (%u): %u\n",
                          (this->datalen_idx_),
                           this->nr_of_runs_,
                          (this->datalen_idx_ + 1),
                           this->number_of_msg_));
      }
    else
      {
        ACE_UINT64 test_time_usec = this->end_time_test_ -
                                    this->start_time_test_;

        double sec =  (double)test_time_usec / (1000 * 1000);
        ACE_DEBUG ((LM_DEBUG, "TEST successful, number of runs (%u) of "
                              "%u messages in %3.3f seconds.\n",
                               this->nr_of_runs_,
                               this->number_of_msg_, sec));
      }
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LatencyTT_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
