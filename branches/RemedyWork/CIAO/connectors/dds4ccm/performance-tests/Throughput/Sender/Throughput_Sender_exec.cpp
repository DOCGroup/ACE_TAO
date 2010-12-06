// -*- C++ -*-
// $Id$

#include "Throughput_Sender_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"
#include "ace/High_Res_Timer.h"

namespace CIAO_Throughput_Sender_Impl
{
  //============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (
                                           int number_of_subscribers,
                                           Sender_exec_i &callback)
    : callback_ (callback),
      number_of_subscribers_ (number_of_subscribers),
      started_ (false)
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
    ::DDS::StatusKind status_kind)
  {
    if (!CORBA::is_nil (the_entity) &&
        status_kind == DDS::PUBLICATION_MATCHED_STATUS)
      {
        ::DDS::PublicationMatchedStatus_var stat;
        DDS::DataWriter_var wr = ::DDS::DataWriter::_narrow(the_entity);
        if(CORBA::is_nil(wr.in ()))
          {
            throw ::CORBA::INTERNAL ();
          }
        ::DDS::ReturnCode_t retval =
                          wr->get_publication_matched_status(stat.out());
        if (retval == DDS::RETCODE_OK)
          {
            if (stat.in ().current_count >= this->number_of_subscribers_ &&
                !this->started_.value ())
              {
                ACE_DEBUG ((LM_DEBUG, "ConnectorStatusListener_exec_i::on_unexpected_status - "
                          "on_publication_matched status received. Starting application\n"));
                this->started_ = true;
                this->callback_.start();
              }
          }
      }
  }

  //============================================================
  // WriteTickerHandler
  //============================================================
  WriteTicker::WriteTicker (Sender_exec_i &callback,
                            Atomic_Boolean &running)
    : callback_ (callback),
      running_ (running)
  {
  }

  int
  WriteTicker::handle_timeout (const ACE_Time_Value &, const void *)
  {
    // Notify the subscribers
    if (!this->running_.value ())
      {
        this->callback_.write();
      }
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : ticker_ (0),
      max_load_ (1000),
      start_load_ (100),
      incr_load_ (100),
      datalen_ (1024),
      recover_time_ (1), // Specifies how long (in s) to sleep after writing a
                         // specific effort specified by start_load , incr_load
      duration_run_ (10),
      number_of_subscribers_ (1),
      number_of_msg_ (0),
      load_ (0),
      overhead_size_ (0),
      running_ (false)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::write(void)
  {
    this->running_ = true;
    CORBA::Boolean test_complete = false;
    this->load_ += this->incr_load_;
    if (this->load_ >= this->max_load_)
      {
        this->stop();
      }
    else
      {
        this->test_topic_cmd_.command = THROUGHPUT_COMMAND_START;
        this->test_topic_cmd_.data_length = this->datalen_;
        this->test_topic_cmd_.current_publisher_effort = this->load_;
        this->test_topic_cmd_.final_publisher_effort = this->max_load_;
        try
          {
            this->cmd_writer_->write_one(this->test_topic_cmd_,
                                         ::DDS::HANDLE_NIL);
          }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                         ACE_TEXT ("while writing command.\n")));
          }
        test_complete = false;
        // get start time
        ACE_UINT64 start_time;
        ACE_High_Res_Timer::gettimeofday_hr ().to_usec (start_time);
        while (!test_complete)
          {
            for (CORBA::ULongLong current_load = 0;
                 current_load < this->load_ && !test_complete;
                 ++current_load, ++this->test_topic_.seq_num)
              {
                try
                  {
                    this->writer_->write_one(this->test_topic_,
                                             ::DDS::HANDLE_NIL);
                    ++this->number_of_msg_;
                  }
                catch (const CCM_DDS::InternalError& err)
                  {
                    if (err.error_code == DDS::RETCODE_TIMEOUT)
                      {
                        ACE_ERROR ((LM_ERROR,
                                    ACE_TEXT ("ERROR: Internal Error ")
                                    ACE_TEXT ("Write Timeout please increase "
                                              "-maxBlockingTime parameter for "
                                              "test\n")));
                      }
                    else
                      {
                        ACE_ERROR ((LM_ERROR,
                                    ACE_TEXT ("ERROR: Internal Error ")
                                    ACE_TEXT ("while updating writer ")
                                    ACE_TEXT ("info for <%u>.\n"),
                                    this->test_topic_.seq_num));
                        test_complete= true;
                      }
                  }
              }
            ACE_UINT64 end_time;
            ACE_High_Res_Timer::gettimeofday_hr ().to_usec (end_time);
            ACE_UINT64 interval = end_time - start_time;
            if(interval > (this->duration_run_ * 1000 * 1000))
              {
                test_complete = true;
                this->test_topic_cmd_.command = THROUGHPUT_COMMAND_COMPLETE;
                this->cmd_writer_->write_one (this->test_topic_cmd_,
                                              ::DDS::HANDLE_NIL);
              }
            if (!test_complete)
              {
                ACE_OS::sleep (this->recover_time_);
              }
          }
      }
    this->running_ = false;
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (
      this->number_of_subscribers_,
      *this);
  }

  void
  Sender_exec_i::start (void)
  {
    ACE_UINT64 const sec = this->duration_run_ + 5;
    (void) ACE_High_Res_Timer::global_scale_factor ();
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->timer_queue()->gettimeofday (&ACE_High_Res_Timer::gettimeofday_hr);
    this->ticker_ = new WriteTicker (*this, this->running_);
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                    this->ticker_,
                    0,
                    ACE_Time_Value (5, 0),
                    ACE_Time_Value (sec, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                              ACE_TEXT ("Error scheduling timer")));
      }
   }

  ::CORBA::ULongLong
  Sender_exec_i::max_load (void)
  {
    return this->max_load_;
  }

  void
  Sender_exec_i::max_load (::CORBA::ULongLong max_load)
  {
    if ((max_load <= 100) || (max_load > 90000))
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: max_load must be greater as '100' "
                              "and smaller as '90001'\n")));
        throw ::CORBA::BAD_PARAM ();
      }
    else
      {
        this->max_load_ = max_load;
      }
  }

  ::CORBA::ULong
  Sender_exec_i::start_load (void)
  {
    return this->start_load_;
  }

  void
  Sender_exec_i::start_load (::CORBA::ULong start_load)
  {
    if (start_load < 100)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: start_load must be greater as '100' "
                              "and smaller as max_load\n")));
        throw ::CORBA::BAD_PARAM ();
      }
    else
    {
      this->start_load_ = start_load;
    }
  }

  ::CORBA::ULong
  Sender_exec_i::incr_load (void)
  {
    return this->incr_load_;
  }

  void
  Sender_exec_i::incr_load (::CORBA::ULong incr_load)
  {
    if (incr_load < 100)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR:incr_load must be greater as '99'\n")));
        throw ::CORBA::BAD_PARAM ();
      }
    else
      {
        this->incr_load_ = incr_load;
      }
  }

  ::CORBA::UShort
  Sender_exec_i::recover_time (void)
  {
    return this->recover_time_;
  }

  void
  Sender_exec_i::recover_time (::CORBA::UShort recover_time)
  {
    this->recover_time_ = recover_time;
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

  ::CORBA::UShort
  Sender_exec_i::datalen (void)
  {
    return this->datalen_;
  }

  void
  Sender_exec_i::datalen (::CORBA::UShort datalen)
  {
    this->overhead_size_ = sizeof(CORBA::ULong) + sizeof(CORBA::ULongLong);
    if (datalen <= this->overhead_size_ ||
        datalen > MAX_DATA_SEQUENCE_LENGTH)
    {
       ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR: datalen has to be bigger"
                            " as %u and smaller as %u\n"),
                            this->overhead_size_,
                            MAX_DATA_SEQUENCE_LENGTH));
       throw ::CORBA::BAD_PARAM ();
    }
    this->datalen_ = datalen;
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::Throughput::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    try
      {
        this->writer_ = this->context_->get_connection_info_write_data ();
        this->cmd_writer_ =
               this->context_->get_connection_command_write_data ();
        if(CORBA::is_nil (this->writer_.in ()))
          {
            throw ::CORBA::INTERNAL ();
          }
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::configuration_complete: "
                    "Exception caught\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::configuration_complete:"
                    " Unknown exception caught\n")));
      }
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    // Make instances of Topic
    this->test_topic_.key = 1;
    this->test_topic_.seq_num = 0;
    this->test_topic_.data.length (this->datalen_ - this->overhead_size_);
  }

  void
  Sender_exec_i::stop (void)
  {
    if (this->ticker_)
      {
        this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
        delete this->ticker_;
        this->ticker_ = 0;
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
    if (this->number_of_msg_ == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR SENDER: No messages "
                              "sent during test.\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "SUMMARY SENDER number of messages sent: %u\n",
                              this->number_of_msg_));
      }
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Throughput_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
