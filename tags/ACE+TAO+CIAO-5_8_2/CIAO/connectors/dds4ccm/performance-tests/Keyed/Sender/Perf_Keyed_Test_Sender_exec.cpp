// -*- C++ -*-
// $Id$

#include "Perf_Keyed_Test_Sender_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"
#include "ace/High_Res_Timer.h"


namespace CIAO_Perf_Keyed_Test_Sender_Impl
{
  //============================================================
  // PerfKeyedTest_Listener_exec_i
  //============================================================
  PerfKeyedTest_Listener_exec_i::PerfKeyedTest_Listener_exec_i (
            Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  PerfKeyedTest_Listener_exec_i::~PerfKeyedTest_Listener_exec_i (void)
  {
  }

  //read messages, only 'ping'messages (ping = -1) which returned from the subscribers, to measure latency time are important.
  void
  PerfKeyedTest_Listener_exec_i::on_one_data (
                                  const PerfKeyedTest & an_instance,
                                  const ::CCM_DDS::ReadInfo & /*info*/)
  {
    //only interested in messages received with a latency_ping = -1 ('ping'messages)
    if( an_instance.latency_ping == -1)
    {
      this->callback_.record_time (an_instance.nanotime);
    }
  }

  void
  PerfKeyedTest_Listener_exec_i::on_many_data (
                                  const PerfKeyedTestSeq & /*an_instance*/,
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
      keys_ (1),
      latency_count_(100),
      sleep_(10),
      spin_(100),
      datalen_(100),
      matched_(false),
      number_of_subscribers_(1),
      tv_total_ (0L),
      tv_max_ (0L),
      tv_min_ (0L),
      count_ (0),  // Number of returned pings
      number_of_msg_(0), // Number of sent messages
      timer_(false)
  {
    this->ticker_ = new WriteTicker (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::write_one (void)
  {
    if( this->number_of_msg_ == 0)
    {
      //first message
      this->last_key_->second->data_len = INITIALIZE_SIZE;
      this->last_key_->second->bin_data = CORBA::string_alloc(0);

    }
    else if((this->iterations_ != 0) &&
            (this->number_of_msg_ == ((this->iterations_ * this->keys_) -1)))
    {
      //last message
      this->last_key_->second->data_len = FINISHED_SIZE;
      this->last_key_->second->bin_data = CORBA::string_alloc(0);

    }
    else
    {
      this->last_key_->second->data_len = this->datalen_;
      this->last_key_->second->bin_data = CORBA::string_alloc(this->datalen_);
    }

    if((this->iterations_ != 0) && (this->number_of_msg_ >=
                                   (this->iterations_ * this->keys_)))
    {
      this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
      this->timer_ = false;
    }
    else
      {
        try
          {
            this->last_key_->second->seq_num =  this->number_of_msg_;
            // Send some messages (latency_ping = 1L) with indicator that
            // message has to be returned by the subscriber
            // TO DO : use other selection if more then one key is used.
            if (( this->number_of_msg_ % this->latency_count_) == 0)
              {
                 this->last_key_->second->latency_ping = 1L;
                 ACE_High_Res_Timer::gettimeofday_hr ().to_usec (
                               this->last_key_->second->nanotime);
              }
            else
              {
                this->last_key_->second->latency_ping = 0L;
              }

            this->writer_->write_one (this->last_key_->second,
                                      ::DDS::HANDLE_NIL);
          }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                        ACE_TEXT ("while writing a sample.\n")));
          }
        ++this->last_key_;
        ++this->number_of_msg_;

        if (this->last_key_ == this->samples_.end ())
          {
            // start over again.
            this->last_key_ = this->samples_.begin ();
          }
      }
 }

  void
  Sender_exec_i::write_one_with_spin (void)
  {
    double ad, bd, cd;
    volatile double * a, * b, * c;

    a = &ad;
    b = &bd;
    c = &cd;

    for (long loop=0; (this->iterations_ == 0)||(loop <this->iterations_);
         ++loop )
      {
        if ( this->spin_ > 0 )
          {
            // spin, spin, spin
            for (int m=0; m<this->spin_; ++m) {
              *a = 1.1;
              *b = 3.1415;
              *c = *a/(*b)*m;
            }
          }
          if( this->number_of_msg_ == 0)
            {
              // first message
              this->last_key_->second->data_len = INITIALIZE_SIZE;
              this->last_key_->second->bin_data = CORBA::string_alloc(0);

            }
            else if( (this->iterations_ != 0) &&
                     (this->number_of_msg_ ==
                     ((this->iterations_ * this->keys_) -1)))
            {
              // last message
              this->last_key_->second->data_len = FINISHED_SIZE;
              this->last_key_->second->bin_data = CORBA::string_alloc(0);

            }
            else
            {
              this->last_key_->second->data_len = this->datalen_;
              this->last_key_->second->bin_data =
                              CORBA::string_alloc(this->datalen_);
            }
            try
          {
            this->last_key_->second->seq_num = loop;
            this->last_key_->second->latency_ping = 0L;

            // if ping required
            if (( this->number_of_msg_ % this->latency_count_) == 0)
            {
              // send some messages (latency_ping = 1L) with indicator that
              // message has to be returned by the subscriber
              this->last_key_->second->latency_ping = 1L;
              ACE_High_Res_Timer::gettimeofday_hr ().to_usec (
                            this->last_key_->second->nanotime);
            }
            this->writer_->write_one (this->last_key_->second,
                                       ::DDS::HANDLE_NIL);
          }
          catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                        ACE_TEXT ("while updating writer info for <%C>.\n"),
                        this->last_key_->first.c_str ()));
          }
          ++this->last_key_;
          if (this->last_key_ == this->samples_.end ())
          {       // onto the next iteration
            this->last_key_ = this->samples_.begin ();
          }
          ++this->number_of_msg_;
         }
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->matched_,
                                               this->number_of_subscribers_,
                                               *this);
  }

  ::CCM_DDS::PerfKeyedTest::CCM_Listener_ptr
  Sender_exec_i::get_ping_listen_data_listener (void)
  {
    return new PerfKeyedTest_Listener_exec_i (*this);
  }

   ::CCM_DDS::CCM_PortStatusListener_ptr
  Sender_exec_i::get_ping_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  void
  Sender_exec_i::start (void)
  {
    for (CORBA::UShort i = 1; i < this->keys_ + 1; ++i)
      {
        char key[7];
        PerfKeyedTest *new_key = new PerfKeyedTest;
        ACE_OS::sprintf (key, "KEY_%d", i);
        new_key->key = CORBA::string_dup(key);
        new_key->seq_num = 0;
        ::CORBA::ULong length = this->datalen_;
        new_key->data_len = length;
        new_key->bin_data = CORBA::string_alloc(length);
        this->samples_[key] = new_key;
      }
    this->last_key_ = this->samples_.begin ();
    // this->sleep_ is in ms
    unsigned int sec = this->sleep_/1000;
    unsigned int usec = (this->sleep_ % 1000) * 1000;

    // if sleep and spin both > 0, use sleep value and ignore spin value
    if(this->sleep_ > 0) // use reactor timer to sleep
      {
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
    else //use spin i.o sleep
    {
      ACE_OS::sleep(5);
      this->write_one_with_spin();
    }
  }

void
Sender_exec_i::record_time (unsigned long long nanotime)
  {
    ACE_UINT64 testend;
    ACE_High_Res_Timer::gettimeofday_hr ().to_usec (testend);
    ACE_UINT64 interval =  (testend  - nanotime);

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
  Sender_exec_i::latency_count (void)
  {
    return this->latency_count_;
  }

  void
  Sender_exec_i::latency_count (::CORBA::UShort latency_count)
  {
    this->latency_count_ = latency_count;
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
  Sender_exec_i::spin (void)
  {
    return this->spin_;
  }

  void
  Sender_exec_i::spin (::CORBA::UShort spin)
  {
    this->spin_ = spin;
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
    if(datalen <= OVERHEAD_BYTES)
    {
       ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: datalen has to be bigger as %u\n"),
                    OVERHEAD_BYTES));
       throw ::CORBA::BAD_PARAM ();
    }
    this->datalen_ = datalen - OVERHEAD_BYTES;
  }


  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::Perf_Keyed_Test::CCM_Sender_Context::_narrow (ctx);

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
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::ccm_activate: Exception caught\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::ccm_activate:"
                    " Unknown exception caught\n")));
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
       ACE_DEBUG ((LM_DEBUG, "SUMMARY SENDER latency time-one way,in usec :\n"
                            "Total time<%u>,\nNumber of latency messages <%u>,"
                            "\nAvg <%6.01f>,\nMin <%u>,\nMax <%u>.\n",
                            this->tv_total_.value ()/2,
                            this->count_.value (),
                            avg/2,
                            this->tv_min_.value ()/2,
                            this->tv_max_.value ()/2));
   }
   else
   {
      ACE_ERROR ((LM_ERROR, "SUMMARY SENDER latency time:\n "
                            "No samples reveived back.\n"));
   }
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Perf_Keyed_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
