// -*- C++ -*-
//
// $Id$

// Test for statelistener: ONE_BY_ONE. Expect the operations on_creation, on_deletion, on_one_update 

#include "SL_OneByOne_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/ndds/Utils.h"

namespace CIAO_SL_OneByOne_Receiver_Impl
{
//============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
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
        this->pulse_callback_.read_all();
      }
    catch (...)
      {
        // @todo
      }
    return 0;
  }
  //============================================================
  // Facet Executor Implementation Class: StateListener_exec_i
  //============================================================
  StateListener_exec_i::StateListener_exec_i (Atomic_Boolean &no_operation,
                                              Atomic_Boolean &on_creation,
                                              Atomic_Boolean &on_one_update,
                                              Atomic_Boolean &on_deletion)
    :no_operation_(no_operation),
     on_creation_(on_creation),
     on_one_update_(on_one_update),
     on_deletion_(on_deletion)
  {
  }

  StateListener_exec_i::~StateListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::StateListener
  void
  StateListener_exec_i::on_creation (const ::TestTopic & /*datum*/,
                                     const ::CCM_DDS::ReadInfo & /*info*/)
  {
    this->on_creation_ = true;
  }

  void
  StateListener_exec_i::on_one_update (const ::TestTopic & /*datum*/,
                                       const ::CCM_DDS::ReadInfo & /*info*/)
  {
    this->on_one_update_ = true;
  }

  void
  StateListener_exec_i::on_many_updates (const ::TestTopic_Seq & /*data*/,
                                         const ::CCM_DDS::ReadInfoSeq & /*infos*/)
  {
    this->no_operation_ = false;
  }

  void
  StateListener_exec_i::on_deletion (const ::TestTopic & /*datum*/,
                                    const ::CCM_DDS::ReadInfo & /*info*/)
  {
    this->on_deletion_ = true;
  }
  //============================================================
  // Facet Executor Implementation Class: PortStatusListener_exec_i
  //============================================================
  PortStatusListener_exec_i::PortStatusListener_exec_i ()
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
  : rate_ (10),
    no_operation_(true),
    updater_data_(false),
    on_creation_(false),
    on_one_update_(false),
    on_deletion_(false)
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
    try
      {
        this->reader_->read_all(TestTopic_infos.out(), readinfoseq.out());
        for(CORBA::ULong i = 0; i < readinfoseq->length(); ++i)
          {
            this->updater_data_ = true;
            time_t tim = readinfoseq[i].source_timestamp.sec;
            tm* time = ACE_OS::localtime(&tim);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
                        ACE_TEXT ("-> UTC date = %02d:%02d:%02d.%d\n"),
                        time ? time->tm_hour : 0,
                        time ? time->tm_min : 0,
                        time ? time->tm_sec : 0,
                        readinfoseq[i].source_timestamp.nanosec));
          }
        for(CORBA::ULong i = 0; i < TestTopic_infos->length(); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL keyed test info : ")
                       ACE_TEXT ("Number <%d> : received TestTopic_info for <%C> at %u\n"),
                        i,
                        TestTopic_infos[i].key.in (),
                        TestTopic_infos[i].x));
          }
      
      }
    catch(CCM_DDS::InternalError& )
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("internal error or no data\n")));
      }
  }

  // Component attributes.
  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i ();
  }

  ::CCM_DDS::TestTopic::CCM_StateListener_ptr
    Receiver_exec_i::get_info_out_data_listener (void)
  {
    return new StateListener_exec_i(this->no_operation_, 
                                     this->on_creation_, 
                                     this->on_one_update_, 
                                     this->on_deletion_);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::SL_OneByOne::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    this->reader_ = this->context_->get_connection_info_out_data();
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::StateListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (CORBA::is_nil (lc.in ()))
      {
         ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    
    lc->mode (::CCM_DDS::ONE_BY_ONE);
    // calculate the interval time
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(3, usec),
                                          ACE_Time_Value(3, usec)) == -1)
      {
        ACE_ERROR ((LM_ERROR, "Unable to schedule Timer\n"));
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
    CORBA::Boolean no_error = true;
    if(!this->no_operation_.value ())
      {   
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did receive an unexpected ")
                               ACE_TEXT (" operation 'on_many_updates' from StateListener in Receiver")
                    )); 
      }
    if(!this->on_creation_ .value ())
      {   
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" operation 'on_creation' from StateListener in Receiver")
                    )); 
      }
    if(!this->on_one_update_.value  ())
      {   
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR:didn't receive the expected ")
                               ACE_TEXT (" operation 'on_one_update' from StateListener in Receiver")
                    )); 
      }
    if(!this->on_deletion_.value ())
      {   
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" operation 'on_deletion' from StateListener in Receiver")
                    )); 
      }
   
    if(no_error==true)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK : Have received the  expected ")
                               ACE_TEXT (" operations for ONE_BY_ONE from StateListener in Receiver")
                   ));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_OneByOne_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
