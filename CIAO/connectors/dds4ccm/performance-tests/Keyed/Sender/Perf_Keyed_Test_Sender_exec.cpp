// -*- C++ -*-
// $Id$

#include "Perf_Keyed_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
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
//    ACE_DEBUG ((LM_DEBUG, "SENDER: PerfKeyedTest_Listener_exec_i::on_one_data: \n"
//                            "key <%C> - ses_num <%d> - latency_ping <%d>\n",
//                            an_instance.key.in (),
//                            an_instance.seq_num,
//                            an_instance.latency_ping));

    //only interested in messages received with a latency_ping = -1 ('ping'messages)
    if( an_instance.latency_ping == -1)
    {     
      this->callback_.record_time (an_instance.nanotime);
    }
  }

  void
  PerfKeyedTest_Listener_exec_i::on_many_data (
                                  const PerfKeyedTest_Seq & /*an_instance*/,
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
          (dynamic_cast <DDS::DataWriter_ptr> (the_entity))->get_publication_matched_status(stat.out()); 
          if((stat.in().current_count >= this->number_of_subscribers_)  && !this->matched_.value())      
            {  
              this->matched_ = true;
              this->callback_.start();
            }
         }
      }
  //============================================================
  // WriteManyHandler
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
  // WriteManyHandler
  //============================================================
  WriteManyHandler::WriteManyHandler (Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  WriteManyHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.write_one (); //???
    return 0;
  }


  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (10),
      keys_ (5),
      latency_count_(100),
      sleep_(10),
      spin_(10),
      datalen_(100),
      matched_(false),
      number_of_subscribers_(1),
      tv_total_ (0L),
      tv_max_ (0L),
      tv_min_ (0L),
      count_ (0),  //number of returned pings
      number_of_msg_(0), //number of sent messages
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
    if( this->number_of_msg_ >= (this->iterations_ * this->keys_))
    {  
      this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
      this->timer_ = false;
    }
    if (this->last_key_ != this->samples_.end ())
      {
        try
          {
            this->last_key_->second->seq_num =  this->number_of_msg_;
            ACE_High_Res_Timer::gettimeofday_hr ().to_usec (this->last_key_->second->nanotime);
            //this->last_key_->second->nanotime = ACE_OS::gethrtime ();
  
 
            //send some messages (latency_ping = 1L) with indicator that message has to be returned by the subscriber
            // TO DO : use other selection 
            if ((( this->number_of_msg_/*/this->keys_*/) % this->latency_count_) == 0)
              {
                 this->last_key_->second->latency_ping = 1L;
              }
            else
              {
                this->last_key_->second->latency_ping = 0L;
              }

            this->writer_->write_one (this->last_key_->second, ::DDS::HANDLE_NIL);
//          ACE_DEBUG ((LM_DEBUG, "Written key <%C> - <%u>\n",
//                          this->last_key_->second->key.in (),
//                          this->last_key_->second->seq_num));
          }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                        ACE_TEXT ("while updating writer info for <%C>.\n"),
                          this->last_key_->first.c_str ()));
          }
        ++this->last_key_;
        this->number_of_msg_++;
      }
    else
      {
        //onto the next iteration
        this->last_key_ = this->samples_.begin ();
        if (this->last_key_ == this->samples_.end ())
          {
            //start over again.
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
    for (long loop=0; (this->iterations_ == 0)||(loop <this->iterations_); ++loop ) 
     { 
       //if ( this->sleep_ > 0 ) {
       //  printf("write_one_with_spin in sleep, loop = %u, iterations = %u \n", loop, this->iterations_);
       //  ACE_OS::sleep(this->sleep_);
       //  }
       if ( this->spin_ > 0 )
         {
          // spin, spin, spin
           for (int m=0; m<this->spin_; ++m) {
             *a = 1.1;
             *b = 3.1415;
             *c = *a/(*b)*m;
           }                    
         }
       try
       {
         this->last_key_->second->seq_num = loop;
         this->last_key_->second->latency_ping = 0L;
         ACE_High_Res_Timer::gettimeofday_hr ().to_usec (this->last_key_->second->nanotime);
 
         //if ping required
         if ((( this->number_of_msg_/*/this->keys_*/) % this->latency_count_) == 0)
         {
           //send some messages (latency_ping = 1L) with indicator that message has to be returned by the subscriber
           this->last_key_->second->latency_ping = 1L;
         }
         this->writer_->write_one (this->last_key_->second, ::DDS::HANDLE_NIL);
//           ACE_DEBUG ((LM_DEBUG, "Written key <%C> - <%u>\n",
//                          this->last_key_->second->key.in (),
//                          this->last_key_->second->seq_num));
       }
       catch (const CCM_DDS::InternalError& )
       {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                        ACE_TEXT ("while updating writer info for <%C>.\n"),
                          this->last_key_->first.c_str ()));
       }
         ++this->last_key_;
         if (this->last_key_ == this->samples_.end ())
           {     //onto the next iteration
             this->last_key_ = this->samples_.begin ();
           }
           
        this->number_of_msg_++;
      }
  }


 ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->matched_, this->number_of_subscribers_, *this);
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
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->mutex_, CORBA::INTERNAL ());

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
    //this->sleep_ in ms
    unsigned int sec = this->sleep_/1000;
    unsigned int usec = (this->sleep_ % 1000) * 1000;
   // printf("sleep = %u, sec  = %u,  usec = %u, spin = %u, datalen = %u\n", this->sleep_, sec, usec, this->spin_, this->datalen_); 
 
    //if sleep and spin both > 0, use sleep value and ignore spin value
    if(this->sleep_ > 0) //use reactor timer to sleep  
      {
        if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer(
                    this->ticker_,
                    0,
                    ACE_Time_Value (5, 50000),
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
    //ACE_hrtime_t testend = ACE_OS::gethrtime ();  // in nanosec's
//    printf("teststart time in nanosec = %I64u\n",nanotime);
//    printf("testend time in nanosec = %I64u\n",testend);
  
    
    ++this->count_;
    long duration = static_cast <CORBA::Long>(interval); 
    this->tv_total_ += duration;
    if (duration > this->tv_max_.value ()|| (this->tv_max_.value () == 0L))
      this->tv_max_ = duration;
    if (duration < this->tv_min_.value () || (this->tv_min_.value () == 0L))
      this->tv_min_ = duration;
  //  double avg = this->tv_total_.value () / this->count_.value ();
                
  
   /* ACE_DEBUG ((LM_ERROR, "Sender Listener_exec_i::on_one_data:\n"
                          " duration = %u\n total.value <%u>,\nthis->count <%u>\n,Avg <%6.01f>\nMin <%u>\nMax <%u> \n ++++++++++++++++++\n",
                            duration,
                            this->tv_total_.value (),
                            this->count_.value (),
                            avg,
                            this->tv_min_.value (),
                            this->tv_max_.value ()));
*/

  
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
  Sender_exec_i::datalen (void)
  {
    return this->datalen_;
  }

  void
  Sender_exec_i::datalen (::CORBA::UShort datalen)
  {
    this->datalen_ = datalen;
  }


  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Perf_Keyed_Test::CCM_Sender_Context::_narrow (ctx);

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
     
   ACE_DEBUG ((LM_ERROR, "SUMMARY SENDER number of messages sent: %u\n ",
                          this->number_of_msg_));

   if( this->count_.value () > 0)
     {
       double avg = this->tv_total_.value () / this->count_.value ();
       ACE_DEBUG ((LM_ERROR, "SUMMARY SENDER latency time in usec :\n "
                            "Total <%u>\n , Count <%u>\n, Avg <%6.01f>\nMin <%u>\nMax <%u> \n",
                            this->tv_total_.value (), 
                            this->count_.value (),
                            avg,
                            this->tv_min_.value (),
                            this->tv_max_.value ()));
   }
   else
   {
      ACE_DEBUG ((LM_ERROR, "SUMMARY SENDER latency time:\n "
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
