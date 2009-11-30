// -*- C++ -*-
// $Id$


// Test for OFFERED_DEADLINE_MISSED status : Writer failed to write data within the deadline time period set in the profile. 

#include "CSL_DeadlineTest_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_CSL_DeadlineTest_Sender_Impl
{

  //============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &deadline_missed)
   : deadline_missed_ (deadline_missed)
  {
  }
  
  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
    
  }
  
  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
    ::DDS::Topic_ptr /*the_topic*/, 
     const DDS::InconsistentTopicStatus & /*status*/){
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
     this->deadline_missed_ = true;
    }
  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/, 
     const DDS::OfferedIncompatibleQosStatus & /*status*/)  {
     //printf("Sender:ConnectorStatusListener_exec_i::on_offered_incompatible_qos status = %dl\n", status);
   
    }
  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  /*status_kind*/)  {
      //CORBA::ULong kind = status_kind;
      //printf("Sender :ConnectorStatusListener_exec_i::on_unexpected_status #### status_kind = %d \n", kind);
    }
  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : deadline_missed_ (false)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_test_topic_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->deadline_missed_);
  }
 
  // Supported operations and attributes.
 
  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::CSL_DeadlineTest::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
   }

  void
  Sender_exec_i::configuration_complete (void)
  {
    this->writer_  = this->context_->get_connection_test_topic_write_data ();
  }

  void
  Sender_exec_i::write (void)
  {
    //to force an 'offered_deadline_missed'  write the topics with a pause of 2 sec in between and 
    //in the profile the deadline is set to 1 sec.
    for (CSL_QoSTest_Table::iterator i = this->_ktests_.begin ();
            i != this->_ktests_.end ();
            ++i)  
    {
      try
      {
        if (!CORBA::is_nil (this->writer_) ) {
          ACE_OS::sleep (2);
          ::DDS::InstanceHandle_t hnd = this->writer_->register_instance (i->second);
          this->writer_->write_one(i->second,hnd);
        }
      }
      catch (CCM_DDS::InternalError& )
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while writing topic for <%C>.\n"),
                        i->first.c_str ()));
      }
    }
  }

 void
  Sender_exec_i::add_instance_of_topic (const char * key, int x)
  {
    TestTopic *new_key = new TestTopic;
    new_key->key = CORBA::string_dup(key);
    new_key->x = x;
    this->_ktests_[key] = new_key;
  }
  void
  Sender_exec_i::ccm_activate (void)
  {
    //add 2 different instances of topic
    this->add_instance_of_topic ("EEN",1);
    this->add_instance_of_topic ("TWEE",2);

    //write the instances once for the test
    this->write();  
  }
  
  

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
   
    if(!this->deadline_missed_.value ())
      {   
     
         CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                               ACE_TEXT (" warning 'on_offered_deadline_missed' in Sender")
                    )); 
      }
   
    else
      {
         CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Received the expected ")
                               ACE_TEXT ("'on_offered_deadline_missed' in Sender\n")
                    ));
      }

  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_DeadlineTest_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}

