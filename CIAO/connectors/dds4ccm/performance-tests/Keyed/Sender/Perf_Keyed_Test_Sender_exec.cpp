// -*- C++ -*-
// $Id$

#include "Perf_Keyed_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_sys_time.h"

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
    //time now
    ACE_Time_Value now = ACE_OS::gettimeofday ();
    //set time in message to an ACE_TIME_VAlue
    ACE_Time_Value ccm_rec (an_instance.timestamp_ccm_sec,
                            an_instance.timestamp_ccm_usec);
//    ACE_DEBUG ((LM_DEBUG, "SENDER: PerfKeyedTest_Listener_exec_i::on_one_data: \n"
//                            "key <%C> - iteration <%d> - latency_ping <%d>\n",
//                            an_instance.key.in (),
//                            an_instance.iteration,
//                            an_instance.latency_ping));

    if( an_instance.latency_ping == -1)
    {
       this->callback_.record_time (now, ccm_rec);
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
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &matched, int number_of_subscribers)
   : matched_ (matched),
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
          //printf("stat.current_count = %ld\n", stat.in().current_count);
          //printf("stat.total_count = %ld\n", stat.in().total_count);
          if(stat.in().current_count >= this->number_of_subscribers_)       
            this->matched_ = true;
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
    this->callback_.write_one ();
    return 0;
  }


  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (10),
      keys_ (5),
      latency_count_(10000),
      sleep_(10),
      matched_(false),
      number_of_subscribers_(1),
      tv_total_ (0L),
      tv_max_ (0L),
      tv_min_ (0L),
      count_ (0)
  {
    this->ticker_ = new WriteTicker (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::write_one (void)
  {
    if (this->last_key_ != this->samples_.end ())
      {
        try
          {
            ACE_Time_Value now = ACE_OS::gettimeofday();
      //    suseconds_t  a = now.usec();
      //     time_t b = ynow.sec() -> sec since 1970
            ++this->last_key_->second->iteration;
            this->last_key_->second->timestamp_ccm_sec = static_cast <CORBA::ULong>(now.sec());
            this->last_key_->second->timestamp_ccm_usec = now.usec();

            //send some messages (latency_ping = 1L) with indicator that message has to be returned by the subscriber
            // TO DO : use other selection 
            if ((this->last_key_->second->iteration % 2) == 0)
             {
                this->last_key_->second->latency_ping = 1L;
             }
             else
             {
               this->last_key_->second->latency_ping = 0L;
             }

            this->writer_->write_one (this->last_key_->second, ::DDS::HANDLE_NIL);
            ACE_DEBUG ((LM_DEBUG, "Written key <%C> - <%u>\n",
                          this->last_key_->second->key.in (),
                          this->last_key_->second->iteration));
          }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                        ACE_TEXT ("while updating writer info for <%C>.\n"),
                          this->last_key_->first.c_str ()));
          }
        ++this->last_key_;
      }
    else
      {
        //onto the next iteration
        this->last_key_ = this->samples_.begin ();
        while (this->last_key_ != this->samples_.end ())
          {
            if (this->last_key_->second->iteration == this->iterations_)
              {
                //next key
                ++this->last_key_;
              }
            else
              {
                break;
              }
          }
        if (this->last_key_ == this->samples_.end ())
          {
            //start over again.
            this->last_key_ = this->samples_.begin ();
          }
      }
  }
 ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->matched_, this->number_of_subscribers_);
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
        new_key->iteration = 0;

        this->samples_[key] = new_key;
      }
   this->last_key_ = this->samples_.begin ();
   if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (5, 50000),
                ACE_Time_Value (3, 50000)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                              ACE_TEXT ("Error scheduling timer")));
      }
  }
void 
  Sender_exec_i::record_time (const ACE_Time_Value &now,
                              const ACE_Time_Value &ccm)
  {
    ACE_Time_Value dur = now - ccm;
    ++this->count_;
    //printf("Sender Na count = %u\n",this->count_.value ());
//    printf(" Sender in record_time  now sec = %lu ,now -usec %lu \n",now.sec(), now.usec());
//    printf(" sender in record_time  ccm sec = %lu ,ccm -usec %lu \n",ccm.sec(),ccm.usec());
//    printf(" sender in record_time  dur sec = %lu ,ccm -usec %lu \n",dur.sec(),dur.usec());
 
    long duration =  static_cast <CORBA::Long>(dur.sec() * 1000 * 1000) + dur.usec();  //total duration in usec 
//    printf(" sender in record_time  duration = %lu \n",duration);

    this->tv_total_ += duration;
    if (duration > this->tv_max_.value ()|| (this->tv_max_.value () == 0L))
      this->tv_max_ = duration;
    if (duration < this->tv_min_.value () || (this->tv_min_.value () == 0L))
      this->tv_min_ = duration;
    double avg = this->tv_total_.value () / this->count_.value ();
                
  
    ACE_DEBUG ((LM_ERROR, "++++++++++++\nPerfKeyedTest_Sender Listener_exec_i::on_one_data:\n"
                          " dur sec =  <%u> usec = %u , duration = %u\n total.value <%u>,\nthis->count <%u>\n,Avg <%6.01f>\nMin <%u>\nMax <%u> \n ++++++++++++++++++\n",
                            static_cast <CORBA::Long>(dur.sec ()), dur.usec (),
                            duration,
                            this->tv_total_.value (),
                            this->count_.value (),
                            avg,
                            //this->tv_total_.value () / this->count_.value (),
                            this->tv_min_.value (),
                            this->tv_max_.value ()));


  
  }
  ::CORBA::UShort
  Sender_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Sender_exec_i::iterations (::CORBA::UShort iterations)
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
   //     wait_for_readers(1); 
                ::CCM_DDS::DataListenerControl_var dlc =
            this->context_->get_connection_ping_listen_data_control ();
            dlc->mode (::CCM_DDS::ONE_BY_ONE);

        if (this->matched_.value())
          {  
            start ();
    
        }
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
   Sender_exec_i::wait_for_readers(int numSubscribers)
   {
      //DDS_PublicationMatchedStatus status;
  //   DDS::PublicationMatchedStatus status;
  //    while (true)
  //      {
        //    this->writer_->  ->get_publication_matched_status(status);
  //          if (status.current_count >= numSubscribers)
  //          {
  //              break;
  //          }
            //perftest_cpp::MilliSleep(1000);
   //     }
    }


  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
     
   
   
      double avg = this->tv_total_.value () / this->count_.value ();
     ACE_DEBUG ((LM_ERROR, "SUMMARY SENDER latency time:\n "
                            "Total <%u>\n , Count <%u>\n, Avg <%6.01f>\nMin <%u>\nMax <%u> \n",
                            this->tv_total_.value (), 
                            this->count_.value (),
                            avg,
                            this->tv_min_.value (),
                            this->tv_max_.value ()));
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
