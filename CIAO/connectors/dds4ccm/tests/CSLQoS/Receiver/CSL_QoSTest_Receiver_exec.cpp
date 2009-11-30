// -*- C++ -*-
//
// $Id$

#include "CSL_QoSTest_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"

namespace CIAO_CSL_QoSTest_Receiver_Impl
{

  
//============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &incompatible)
   : incompatible_ (incompatible)
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
      //printf("Receiver: ConnectorStatusListener_exec_i::on_incompatible_topic\n");
    }
  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)  {
     this->incompatible_ = true;
     //printf("ConnectorStatusListener_exec_i::on_requested_incompatible_qos, status = %dl\n",status);
    }
  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr /*the_reader*/, 
     const DDS::SampleRejectedStatus & /*status*/)  {
     //printf("ConnectorStatusListener_exec_i::on_sample_rejected\n");
    }
  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedDeadlineMissedStatus & /*status*/)  {
     //printf("ConnectorStatusListener_exec_i::on_offered_deadline_missed\n");
    }
  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/, 
     const DDS::OfferedIncompatibleQosStatus & /*status*/)  {
     // printf("Receiver: ConnectorStatusListener_exec_i::on_offered_incompatible_qos\n");
    }
  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind /*status_kind */)  {
//      CORBA::ULong kind = status_kind;
//      printf("Receiver :ConnectorStatusListener_exec_i::on_unexpected_status #### status_kind = %ld \n", kind);
  
    }
 

  //============================================================
  // Facet Executor Implementation Class: TestTopic_RawListener_exec_i
  //============================================================

  TestTopic_RawListener_exec_i::TestTopic_RawListener_exec_i (Atomic_ULong &received)
      : received_ (received)
  {
  }

  TestTopic_RawListener_exec_i::~TestTopic_RawListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::TestTopic_RawListener

  void
  TestTopic_RawListener_exec_i::on_one_data (
    const TestTopic & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
       ++this->received_;
    //printf(" receive data ============= %d\n",this->received_);
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("TestTopic_RawListener: ")
            ACE_TEXT ("received test_topic_info for <%C> at %u\n"),
            an_instance.key.in (),
            an_instance.x));
  }

  
  void
  TestTopic_RawListener_exec_i::on_many_data (
    const TestTopic_Seq & /*an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
    //printf(" receive data ============= %d\n",this->received_);
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
  // Component Executor Implementation Class: Receiver_exec_iTestTopic_RawListener_exec_i ();
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : incompatible_ (false)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Supported operations and attributes.
  // Component attributes.
  // Port operations.
  ::CCM_DDS::TestTopic::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    //printf ("*************** get_inf-_out_data_listener************************\n");
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new TestTopic RAW listener\n")));
    return new TestTopic_RawListener_exec_i (this->received_);
  }
  
  
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i ();
  }
 
  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    //printf ("*************** out connector status Receiver************************\n");
    return new ConnectorStatusListener_exec_i (this->incompatible_);
  }
 
  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CSL_QoSTest::CCM_Receiver_Context::_narrow (ctx);
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
    ::CCM_DDS::DataListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();


    if (CORBA::is_nil (lc.in ()))
      {
        CIAO_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    lc->mode (::CCM_DDS::ONE_BY_ONE);    

   
  }
  
  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }
  
  void
  Receiver_exec_i::ccm_remove (void)
  {
    //printf("*************in remove Receiver********** \n");
    if(!this->incompatible_.value ())
      {   
     
         CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                               ACE_TEXT (" error 'on_requested_incompatible_Qos' in Receiver")
                    )); 
      }
   
    else
      {
         CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Received the expected ")
                               ACE_TEXT ("'on_requested_incompatible_Qos' in Receiver\n")
                    ));
      }
 

  }
  
  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_QoSTest_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

