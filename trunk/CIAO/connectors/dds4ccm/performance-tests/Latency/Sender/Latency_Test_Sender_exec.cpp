// -*- C++ -*-
// $Id$

#include "Latency_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"
#include "ace/High_Res_Timer.h"

namespace CIAO_Latency_Test_Sender_Impl
{
  //============================================================
  // LatencyTest_Listener_exec_i
  //============================================================
  LatencyTest_Listener_exec_i::LatencyTest_Listener_exec_i (
            Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  LatencyTest_Listener_exec_i::~LatencyTest_Listener_exec_i (void)
  {
  }

  //read messages.
  void
  LatencyTest_Listener_exec_i::on_one_data (
                                  const LatencyTest & an_instance,
                                  const ::CCM_DDS::ReadInfo &)
  {
    ACE_UINT64  receive_time = 0;

    //only interested in messages received with a latency_ping = 0 (messages sent beck by receiver)
    if( an_instance.ping == 0)
    {
      ACE_High_Res_Timer::gettimeofday_hr ().to_usec ( receive_time);
      this->callback_.read(an_instance, receive_time);
    }
  }

  void
  LatencyTest_Listener_exec_i::on_many_data (
                                  const LatencyTest_Seq & /*an_instance*/,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
  }
  //============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &matched, int number_of_subscribers, Sender_exec_i &callback)
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
     const DDS::RequestedIncompatibleQosStatus & /*status*/)  {
    }

  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::SampleRejectedStatus & /*status*/)  {
    }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedDeadlineMissedStatus & /*status*/)  {
    }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedIncompatibleQosStatus & /*status*/)  {
    }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr the_entity,
    ::DDS::StatusKind  status_kind)  {
      CORBA::ULong kind = status_kind;
      if((!CORBA::is_nil(the_entity)) && (kind==DDS::PUBLICATION_MATCHED_STATUS))
        {
          ::DDS::PublicationMatchedStatus_var stat;
          ::DDS::DataWriter::_narrow(the_entity)->get_publication_matched_status(stat.out());
          if((stat.in().current_count >= (this->number_of_subscribers_ + 1))  && !this->matched_.value())
            {
              this->matched_ = true;
              this->callback_.start();
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
      keys_ (1),
      datalen_(100),
      sleep_(10),
      matched_(false),
      number_of_subscribers_(1),
      tv_total_ (0L),
      tv_max_ (0L),
      tv_min_ (0L),
      count_ (0),  //number of returned messages
      number_of_msg_(0), //number of sent messages
      timer_(false),
      received_(false),
      seq_num_(0)
  {
    this->ticker_ = new WriteTicker (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::write_one (void)
  {
    //first message sent always, next messages only as previous sent message is received back
    // TO DO: what if a message is lost?
    if( (this->number_of_msg_ == 0) || ( this->received_.value()))
    {
      // all messages send, stop timer
      if((this->iterations_ != 0) && (this->number_of_msg_ >= (this->iterations_ * this->keys_)))
        {
          this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
          this->timer_ = false;
        }
      else
        {
        try
          {
            //send messages with indicator (ping = 1L) so that subscriber knows that this message has to sent back.
            this->last_key_->second->ping = 1L;
            this->last_key_->second->seq_num = this->number_of_msg_;

            //keep last sent seq_num, in order to control if message is sent back.
            this->seq_num_ = this->number_of_msg_;

             ACE_High_Res_Timer::gettimeofday_hr ().to_usec (this->start_time_);
             this->writer_->write_one (this->last_key_->second, ::DDS::HANDLE_NIL);
           }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                        ACE_TEXT ("while updating writer info for <%C>.\n"),
                          this->last_key_->first.c_str ()));
          }
        ++this->number_of_msg_;
       }
    }
  }

  void
    Sender_exec_i::read(LatencyTest an_instance,ACE_UINT64  receive_time)
  {
    if (an_instance.seq_num == this->seq_num_.value())
    {
      this->record_time( receive_time);
      this->received_ = true;
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

  ::CCM_DDS::LatencyTest::CCM_Listener_ptr
  Sender_exec_i::get_ping_listen_data_listener (void)
  {
    return new LatencyTest_Listener_exec_i (*this);
  }

   ::CCM_DDS::CCM_PortStatusListener_ptr
  Sender_exec_i::get_ping_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  void
  Sender_exec_i::start (void)
  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->mutex_, CORBA::INTERNAL ());
    for (CORBA::UShort i = 1; i < this->keys_ + 1; ++i)
      {
        char key[7];
        LatencyTest *new_key = new LatencyTest;
        ACE_OS::sprintf (key, "KEY_%d", i);
        new_key->key = CORBA::string_dup(key);
        new_key->seq_num = 0;
        new_key->data.allocbuf(this->datalen_);
        // to do : freebuf, where ?
        this->samples_[key] = new_key;
      }
    this->last_key_ = this->samples_.begin ();

    //this->sleep_ is in ms
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
    long duration = static_cast <CORBA::Long>(interval);
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
    this->iterations_ = iterations;
  }

  ::CORBA::UShort
  Sender_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Sender_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;
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

  ::CORBA::UShort
  Sender_exec_i::datalen (void)
  {
    return this->datalen_;
  }

  void
  Sender_exec_i::datalen (::CORBA::UShort datalen)
  {
    // 7 is length of key, has to be removed
    int overhead_size = sizeof(CORBA::ULong) + sizeof(CORBA::ULong) + 7;
    if((datalen <= overhead_size) || (datalen > MAX_DATA_SEQUENCE_LENGTH))
    {
       ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: datalen has to be bigger as %u and smaller as %u\n"), overhead_size, MAX_DATA_SEQUENCE_LENGTH));
       throw ::CORBA::BAD_PARAM ();
    }
    this->datalen_ = datalen - overhead_size;
  }


  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Latency_Test::CCM_Sender_Context::_narrow (ctx);

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
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    if (this->timer_.value ())
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
  }

  void
  Sender_exec_i::ccm_remove (void)
  {

   ACE_DEBUG ((LM_DEBUG, "SUMMARY SENDER number of messages sent: %u\n",
                          (this->number_of_msg_ + 1)));

   if( this->count_.value () > 0)
     {
       double avg = this->tv_total_.value () / this->count_.value ();
       ACE_DEBUG ((LM_DEBUG,"Collecting statistics on %d samples with message size %u.\n"
         " TO DO: calculate stdev, 50%%,90%%,99%% and 99.99%%\n"
         "This is the roundtrip time, *not* the one-way-latency\n"
         "bytes ,stdev us,ave us, min us, 50%% us, 90%% us, 99%% us, 99.99%%, max us\n"
         "------,-------,-------,-------,-------,-------,-------,-------,-------\n"
         "%6d,-------,%7.1f,%7u,-------,-------,-------,-------,%7u\n",
         this->count_.value (),
         this->datalen_,
         this->datalen_,
         avg,
         this->tv_min_.value (),
         this->tv_max_.value ()));
     }
   else
     {
       ACE_ERROR ((LM_ERROR, "SUMMARY SENDER latency time:\n "
                            "No samples reveived back.\n"));
     }
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Latency_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
