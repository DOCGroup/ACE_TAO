// -*- C++ -*-
//
// $Id$
#include "Throughput_Receiver_exec.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "ciao/Logger/Log_Macros.h"
#include "ace/High_Res_Timer.h"


namespace CIAO_Throughput_Receiver_Impl
{
  //============================================================
  // ThroughputTest_Listener_exec_i
  //============================================================
  ThroughputTest_Listener_exec_i::ThroughputTest_Listener_exec_i (
            Receiver_exec_i &callback)
    : callback_ (callback)
  {
  }

  ThroughputTest_Listener_exec_i::~ThroughputTest_Listener_exec_i (void)
  {
  }

  void
  ThroughputTest_Listener_exec_i::on_one_data (
                                  const ThroughputTest & an_instance,
                                  const ::CCM_DDS::ReadInfo & /*info*/)
  {
    this->callback_.record_data (const_cast<ThroughputTest&> (an_instance));
  }

  void
  ThroughputTest_Listener_exec_i::on_many_data (
                                  const ThroughputTestSeq & /*data_seq*/,
                                  const ::CCM_DDS::ReadInfoSeq & /*info_seq*/)
  {
  }

  //============================================================
  // ThroughputCommand_Listener_exec_i
  //============================================================
  ThroughputCommand_Listener_exec_i::ThroughputCommand_Listener_exec_i (
            Receiver_exec_i &callback)
    : callback_ (callback)
  {
  }

  ThroughputCommand_Listener_exec_i::~ThroughputCommand_Listener_exec_i (void)
  {
  }

  void
  ThroughputCommand_Listener_exec_i::on_one_data (
                                  const ThroughputCommand & an_instance,
                                  const ::CCM_DDS::ReadInfo & /*info*/)
  {
    this->callback_.handle_run (const_cast<ThroughputCommand&> (an_instance));
  }

  void
  ThroughputCommand_Listener_exec_i::on_many_data (
                                  const ThroughputCommandSeq & /*data_seq*/,
                                  const ::CCM_DDS::ReadInfoSeq & /*info_seq*/)
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : count_ (0L), // total count of all received messages
      interval_messages_received_(0L),
      interval_bytes_received_(0L),
      interval_data_length_(0L),
      run_(0),
      first_time_ (0L),
      messages_lost_(0),
      log_(false)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  void
  Receiver_exec_i::start ()
  {
    ::CCM_DDS::DataListenerControl_var dlccmd =
            this->context_->get_connection_command_listen_data_control ();
    dlccmd->mode (::CCM_DDS::ONE_BY_ONE);
    ::CCM_DDS::DataListenerControl_var dlc =
            this->context_->get_connection_info_listen_data_control ();
    dlc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  void
  Receiver_exec_i::reset_results()
  {
    this->interval_messages_received_ = 0;
    this->interval_bytes_received_ = 0;
    this->seq_num_ = 0;
    this->messages_lost_= 0;
  }

  void
  Receiver_exec_i::handle_run (ThroughputCommand & an_instance)
  {
    if (an_instance.command == THROUGHPUT_COMMAND_START)
      {
        this->reset_results();
        this->interval_data_length_ = an_instance.data_length;
        this->demand_ = an_instance.current_publisher_effort;
        ACE_High_Res_Timer::gettimeofday_hr ().to_usec (this->first_time_);
        this->log_ = true;
      }
    else if (an_instance.command == THROUGHPUT_COMMAND_COMPLETE)
      {
        this->log_ = false;
        ++this->run_;
        ACE_UINT64 last_time;
        ACE_High_Res_Timer::gettimeofday_hr ().to_usec (last_time);
        ACE_UINT64 interval_time =  (last_time  - this->first_time_);
        this->show_results (interval_time);
      }
  }

  void
  Receiver_exec_i::record_data (ThroughputTest & an_instance)
  {
    ++this->count_; // total count of all received messages

    if (this->log_.value ())
      {
        ++this->interval_messages_received_;
        this->interval_bytes_received_ += this->interval_data_length_;
        if (an_instance.seq_num != this->seq_num_)
          {
            ++this->messages_lost_;
            this->seq_num_ = an_instance.seq_num;
          }
        ++this->seq_num_;
      }
  }

  void
  Receiver_exec_i::show_results (const ACE_UINT64& interval_time)
  {
    if (this->count_ > 0 && interval_time > 0)
      {
        double per_sec = (double)1000000/ interval_time;
        double mbps = (this->interval_bytes_received_* per_sec)* (8.0/1000.0/1000.0);

        if(this->run_ == 1)
          {
            #if (CIAO_DDS4CCM_CONTEXT_SWITCH==1)
              ACE_DEBUG ((LM_DEBUG, "\n\nYES, we're using a threadswitch between "
                                    "DDS and CMM\n\n"));
            #else
              ACE_DEBUG ((LM_DEBUG, "\n\nNO, we're not using a threadswitch between "
                                    "DDS and CMM\n\n"));
            #endif
             ACE_DEBUG ((LM_DEBUG,
                        " bytes, demand, samples,sample/s, Mbit/s,lost samples\n"));
             ACE_DEBUG ((LM_DEBUG,
                        "------,-------,--------,--------,-------,------------\n"));
          }
        ACE_DEBUG ((LM_DEBUG, "%6u,%7q,%8q,%8.1f,%7.1f,%7q\n",
                        this->interval_data_length_,
                        this->demand_,
                        this->interval_messages_received_,
                        this->interval_messages_received_ * per_sec,
                        mbps,
                        this->messages_lost_));
      }
  }

  ::Throughput::ThroughputTestConn::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    return new ThroughputTest_Listener_exec_i (*this);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

   ::Throughput::ThroughputCommandConn::CCM_Listener_ptr
  Receiver_exec_i::get_command_listen_data_listener (void)
  {
    return new ThroughputCommand_Listener_exec_i (*this);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_command_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Throughput::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    this->start();
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
    if (this->count_ > 0)
      {
        ACE_DEBUG((LM_DEBUG, "SUMMARY RECEIVER:\n "
                             "Number of samples received: %u \n",
                             this->count_));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR RECEIVER: "
                              "No samples received\n"));
      }
  }


  void
  Receiver_exec_i::ccm_remove (void)
  {
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Throughput_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
