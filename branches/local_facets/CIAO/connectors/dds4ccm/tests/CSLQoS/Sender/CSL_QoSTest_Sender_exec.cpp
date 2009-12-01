// -*- C++ -*-
// $Id$

#include "CSL_QoSTest_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_CSL_QoSTest_Sender_Impl
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
       //printf("Sender : ConnectorStatusListener_exec_i::on_inconsistent_topic\n");
    }
  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)  {
       //printf("Sender:ConnectorStatusListener_exec_i::on_requested_incompatible_qos, status = %dl\n", status);
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
//     printf("Sender:ConnectorStatusListener_exec_i::on_offered_incompatible_qos status = %dl\n", status);
       this->incompatible_ = true;
    }
  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  /*status_kind*/)  {
    //  CORBA::ULong kind = status_kind;
    //  printf("Sender :ConnectorStatusListener_exec_i::on_unexpected_status #### status_kind = %d \n", kind);
    }
  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : incompatible_ (false)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_test_topic_connector_status (void)
  {
   // printf ("*************** out connector status sender ************************\n");
    return new ConnectorStatusListener_exec_i (this->incompatible_);
  }
 
  // Supported operations and attributes.
 
  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CSL_QoSTest::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
   }

  void
  Sender_exec_i::configuration_complete (void)
  {
    //printf("-------------configuration_complete ----------------\n"); 
  }
 
  void
  Sender_exec_i::ccm_activate (void)
  {
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    //printf("*************in remove Sender********** \n");
  
    if(!this->incompatible_.value ())
      {   
     
         CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                               ACE_TEXT (" error 'on_offered_incompatible_Qos' in Sender")
                    )); 
      }
   
    else
      {
         CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Received the expected ")
                               ACE_TEXT ("'on_offered_incompatible_qos' in Sender\n")
                    ));
      }

  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_QoSTest_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}

