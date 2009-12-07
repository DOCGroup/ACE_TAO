// -*- C++ -*-
// $Id$



#include "PSL_SampleLost_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_PSL_SampleLost_Sender_Impl
{

  ///============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i ()
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
  // Pulse generator
  //============================================================

  pulse_Generator::pulse_Generator (Sender_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    // Notify the subscribers
    this->pulse_callback_.tick ();
    return 0;
  }
  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_test_topic_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i ();
  }

  // Supported operations and attributes.

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::PSL_SampleLost::CCM_Sender_Context::_narrow (ctx);

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
  Sender_exec_i::tick ()
  {
    //printf("tick");
    for (PSL_DLTest_Table::iterator i = this->_ktests_.begin ();
        i != this->_ktests_.end ();
        ++i)
      {
         try
         {
           if (!CORBA::is_nil (this->writer_) ) {
              this->writer_->write_one(i->second,::DDS::HANDLE_NIL);
              printf("wrote x = %ld\n", i->second->x++);
              i->second->x++;
           }
         }
         catch (CCM_DDS::InternalError& )
         {
           CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while creating topic for <%C>.\n"),
                    i->first.c_str ()));
         }
       }
  }

  void
  Sender_exec_i::start (void)
  {
    long sec = 1L;
    long usec = 0L;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (sec, usec),
                ACE_Time_Value (sec, usec)) == -1)
    {
      CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                             ACE_TEXT ("Error scheduling timer")));
    }
  }

  void
  Sender_exec_i::stop (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
    delete this->ticker_;
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

     this->start ();
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->stop ();
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
  }


  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PSL_SampleLost_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}

