// -*- C++ -*-
//
// $Id$

#include "Perf_Keyed_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "ace/High_Res_Timer.h"



namespace CIAO_Perf_Keyed_Test_Receiver_Impl
{
  //============================================================
  // PerfKeyedTest_Listener_exec_i
  //============================================================
  PerfKeyedTest_Listener_exec_i::PerfKeyedTest_Listener_exec_i (
            Receiver_exec_i &callback)
    : callback_ (callback)
  {
  }

  PerfKeyedTest_Listener_exec_i::~PerfKeyedTest_Listener_exec_i (void)
  {
  }

  void
  PerfKeyedTest_Listener_exec_i::on_one_data (
                                  const PerfKeyedTest & an_instance,
                                  const ::CCM_DDS::ReadInfo & /*info*/)
  {
    //ACE_OS::sleep(1); //temporarily set to 1 sec for testing time results 
    /*ACE_DEBUG ((LM_DEBUG, "RECEIVER PerfKeyedTest_Listener_exec_i::on_one_data:\n "
                            "key <%C> - seq_num <%d> - latency_ping <%d>\n",
                            an_instance.key.in (),
                            an_instance.seq_num,
                            an_instance.latency_ping));
    */ 
    // Record time, not for ping messages , already pinged back.
    if (an_instance.latency_ping != -1L)
    {
      this->callback_.record_time (an_instance.nanotime);
    }
 
     // Send back a packet if this is a ping
    if (an_instance.latency_ping == 1L)
      {
//       ACE_OS::sleep(1); //temporarily set to 1 sec for testing time results
        this->callback_.write_one(an_instance);
      }
  }

  void
  PerfKeyedTest_Listener_exec_i::on_many_data (
                                  const PerfKeyedTest_Seq & /*an_instance*/,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
  }

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (
            Receiver_exec_i &callback)
    : callback_ (callback),
      started_(false)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr ,
     const DDS::InconsistentTopicStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr ,
     const DDS::RequestedIncompatibleQosStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr ,
     const DDS::SampleRejectedStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr ,
     const DDS::OfferedDeadlineMissedStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr ,
     const DDS::OfferedIncompatibleQosStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr ,
    ::DDS::StatusKind  status_kind)
  {
    if ((status_kind == ::DDS::DATA_ON_READERS_STATUS) && !this->started_.value ())
      {
        this->started_ = true;
        this->callback_.start ();
      }
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : tv_total_ (0L),
      tv_max_ (0L),
      tv_min_ (0L),
      count_ (0L),
      started_ (false),
      iterations_ (10),
      keys_ (5)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }
  void
  Receiver_exec_i::write_one ( const PerfKeyedTest & an_instance)
  {
     PerfKeyedTest * dup_ =  new PerfKeyedTest;
     dup_->seq_num = an_instance.seq_num;
     dup_->key = CORBA::string_dup(an_instance.key);
     dup_->seq_num = an_instance.seq_num;
     dup_->nanotime = an_instance.nanotime;
     dup_->latency_ping = -1L;
     dup_->bin_data = CORBA::string_alloc(an_instance.data_len);
     dup_->bin_data = CORBA::string_dup(an_instance.bin_data);
     this->writer_->write_one (*dup_, ::DDS::HANDLE_NIL);
  }

  void
  Receiver_exec_i::start ()
  {
    if (!this->started_.value ())
      {
        this->started_ = true;
        ::CCM_DDS::DataListenerControl_var dlc =
            this->context_->get_connection_info_listen_data_control ();
        dlc->mode (::CCM_DDS::ONE_BY_ONE);
      }
  }

  void 
  Receiver_exec_i::record_time ( unsigned long long nanotime)
  {
    ++this->count_;
    //ACE_Time_Value dur = now - ccm;
    //printf("COUNT na = %lu , dur sec % u and usec %u \n",this->count_.value (),static_cast <CORBA::Long>(dur.sec()),dur.usec());
    //long duration = dur.usec () + (static_cast <CORBA::Long>(dur.sec() * 1000000)); 
 
    ACE_UINT64 testend;
    ACE_High_Res_Timer::gettimeofday_hr ().to_usec (testend);
    ACE_UINT64 interval =  (testend  - nanotime);

    long duration = static_cast <CORBA::Long>(interval); 
     this->tv_total_ += duration;
    if (duration > this->tv_max_.value ()|| (this->tv_max_.value () == 0))
      this->tv_max_ = duration;
    if (duration < this->tv_min_.value () || (this->tv_min_.value () == 0))
      this->tv_min_ = duration;
  //  double avg =  this->tv_total_.value () / this->count_.value ();


  /*    ACE_DEBUG((LM_ERROR, "=========\n RECEIVER: record_time\n "
                           "samples <%u>\n - duration <%u> total time <%u>\n "
                           "- avg <%6.01f>\n - min <%u>\n - max <%u>\n ================\n",
                           this->count_.value (),
                           duration, 
                           this->tv_total_.value (),
                           avg,
                           this->tv_min_.value (),
                           this->tv_max_.value ()));
  */
  }

  ::CCM_DDS::PerfKeyedTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    return new PerfKeyedTest_Listener_exec_i (*this);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_listen_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (*this);
  }

  ::CORBA::UShort
  Receiver_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Receiver_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
  }

  ::CORBA::UShort
  Receiver_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Receiver_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Perf_Keyed_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
     this->writer_ = this->context_->get_connection_write_ping_data ();
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
     
     if (this->count_.value () > 0)
     {  
       double avg =  this->tv_total_.value () / this->count_.value ();
        ACE_DEBUG((LM_ERROR, "SUMMARY RECEIVER:\n "
                           "samples <%u>\n - total time <%u> usec\n "
                           "- avg <%6.01f> usec\n - min <%u> usec\n - max <%u> usec\n",
                           this->count_.value (),
                           this->tv_total_.value (),
                           avg,
                           this->tv_min_.value (),
                           this->tv_max_.value ()));
     }
     else
     {
               ACE_DEBUG((LM_ERROR, "SUMMARY RECEIVER:\n "
                           "No samples received\n "));
     }
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    
    
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Perf_Keyed_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
