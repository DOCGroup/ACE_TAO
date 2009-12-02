// -*- C++ -*-
//
// $Id$

// Test for REJECTED_SAMPLE status  

#include "CSL_SRTest_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/ndds/Utils.h"

namespace CIAO_CSL_SRTest_Receiver_Impl
{

  
//============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &rejected)
   : rejected_ (rejected)
  {
  }
  
  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
    
  }
  
  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr /*the_topic*/, 
     const DDS::InconsistentTopicStatus & /*status*/){
     //printf("Receiver: ConnectorStatusListener_exec_i::on_incompatible_topic\n");
    }
  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)  {
     //printf("ConnectorStatusListener_exec_i::on_requested_incompatible_qos, status = %dl\n",status);
    }
  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr /*the_reader*/, 
     const DDS::SampleRejectedStatus & /*status*/)  {
     //  printf("Receiver:ConnectorStatusListener_exec_i::on_sample_rejected\n");
       this->rejected_ = true;
    }
  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedDeadlineMissedStatus & /*status*/)  {
     //printf("Receiver: ConnectorStatusListener_exec_i::on_offered_deadline_missed\n");
    }
  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/, 
     const DDS::OfferedIncompatibleQosStatus & /*status*/)  {
     //printf("Receiver: ConnectorStatusListener_exec_i::on_offered_incompatible_qos\n");
    }
  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind /*status_kind*/)  {
    //CORBA::ULong kind = status_kind;
    //printf("Sender :ConnectorStatusListener_exec_i::on_unexpected_status #### status_kind = %d %s \n", kind, ::CIAO::DDS4CCM::translate_statuskind (kind));

    }
read_action_Generator::read_action_Generator (Receiver_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  read_action_Generator::~read_action_Generator ()
  {
  }

  int
  read_action_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    try
      {
        if (pulse_callback_.read_data ())
          {
            this->pulse_callback_.read_all();
          }
       }
    catch (...)
      {
        // @todo
      }
    return 0;
  }

  //============================================================
  // Facet Executor Implementation Class: TestTopic_Listener_exec_i
  //============================================================

  TestTopic_Listener_exec_i::TestTopic_Listener_exec_i (Atomic_ULong &received)
      : received_ (received)
  {
  }

  TestTopic_Listener_exec_i::~TestTopic_Listener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::TestTopic_Listener
  void
  TestTopic_Listener_exec_i::on_many_data (
    const TestTopic_Seq & /* an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }

  void
  TestTopic_Listener_exec_i::on_one_data (
    const TestTopic & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ++this->received_;
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("TestTopic_Listener: ")
            ACE_TEXT ("received testtopic_info for <%C> at %dl\n"),
            an_instance.key.in (),
            an_instance.x));
  }

  //============================================================
  // Facet Executor Implementation Class: PortStatusListener_exec_i
  //============================================================

  PortStatusListener_exec_i::PortStatusListener_exec_i (void)
  {
  }

  PortStatusListener_exec_i::~PortStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PortStatusListener

  void
  PortStatusListener_exec_i::on_requested_deadline_missed (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  PortStatusListener_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleLostStatus & /* status */)
  {

  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iTestTopic_Listener_exec_i ();
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : rate_ (0),
      read_data_ (true),
      raw_listen_ (false),
      received_ (0),
      rejected_(0)

  {
    this->ticker_ = new read_action_Generator (*this);
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Supported operations and attributes.
  
  void
  Receiver_exec_i::read_all (void)
  {
    if (CORBA::is_nil (this->reader_.in ()))
      {
        return;
      }
    TestTopic_Seq_var TestTopic_infos;
    ::CCM_DDS::ReadInfoSeq_var readinfoseq;
    this->reader_->read_all(TestTopic_infos.out(), readinfoseq.out());
    for(CORBA::ULong i = 0; i < readinfoseq->length(); ++i)
      {
        time_t tim = readinfoseq[i].source_timestamp.sec;
        tm* time = ACE_OS::localtime(&tim);
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
            ACE_TEXT ("-> UTC date = %02d:%02d:%02d.%d\n"),
                            time ? time->tm_hour : 0,
                            time ? time->tm_min : 0,
                            time ? time->tm_sec : 0,
                            readinfoseq[i].source_timestamp.nanosec));
      }
    for(CORBA::ULong i = 0; i < TestTopic_infos->length(); ++i)
      {
         CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL keyed test info : ")
              ACE_TEXT ("Number <%d> : received TestTopic_info for <%C> at %u\n"),
            i,
            TestTopic_infos[i].key.in (),
            TestTopic_infos[i].x));
      }
  }

 
  // Component attributes.
  ::CORBA::ULong
  Receiver_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Receiver_exec_i::rate (
    ::CORBA::ULong rate)
  {
    this->rate_ = rate;
  }

  ::CORBA::Boolean
  Receiver_exec_i::read_data (void)
  {
    return this->read_data_;
  }

  void
  Receiver_exec_i::read_data (
    ::CORBA::Boolean read_data)
  {
    this->read_data_ = read_data;
  }

  ::CORBA::Boolean
  Receiver_exec_i::raw_listen (void)
  {
    return this->raw_listen_;
  }

  void
  Receiver_exec_i::raw_listen (
    ::CORBA::Boolean raw_listen)
  {
    this->raw_listen_ = raw_listen;
  }

  // Port operations.
  ::CCM_DDS::TestTopic::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new TestTopic RAW listener\n")));
    return new TestTopic_Listener_exec_i (this->received_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i ();
  }

   ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->rejected_);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CSL_SRTest::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    if (this->read_data ())
      {
       this->reader_ = this->context_->get_connection_info_out_data();
      }
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::DataListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (CORBA::is_nil (lc.in ()))
      {
        CIAO_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    lc->mode (this->raw_listen_ ? ::CCM_DDS::ONE_BY_ONE : ::CCM_DDS::NOT_ENABLED);

    // calculate the interval time
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(0, usec),
                                          ACE_Time_Value(0, usec)) == -1)
      {
        CIAO_ERROR ((LM_ERROR, "Unable to schedule Timer\n"));
      }
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    delete this->ticker_;
  } 

  void
  Receiver_exec_i::ccm_remove (void)
  {
     if(!this->rejected_.value ())
      {   
         CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                               ACE_TEXT (" warning 'on_sample_rejected' in the Receiver")
                    )); 
      }
   
    else
      {
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("OK : Have received the expected ")
                               ACE_TEXT ("'on_sample_rejected' in the Receiver\n")
                    ));
      }
 

  }
 
  
  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_SRTest_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

