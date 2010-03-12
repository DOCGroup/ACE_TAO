// -*- C++ -*-
//
// $Id$

// Test for statelistener: MANY_BY_MANY. Expect the operations on_creation, on_deletion, on_many_updates

#include "SL_ManyByMany_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/dds/Utils.h"
#include "dds4ccm/impl/dds/TimeUtilities.h"

namespace CIAO_SL_ManyByMany_Receiver_Impl
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
                                              Atomic_Boolean &on_many_update,
                                              Atomic_Boolean &on_deletion,
                                              Atomic_Boolean &create_data,
                                              Atomic_Boolean &update_data)
    :no_operation_(no_operation),
     on_creation_(on_creation),
     on_many_update_(on_many_update),
     on_deletion_(on_deletion),
     create_data_(create_data),
     update_data_(update_data)
  {
  }

  StateListener_exec_i::~StateListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::StateListener
  void
  StateListener_exec_i::on_creation (const ::TestTopic & data,
                                     const ::CCM_DDS::ReadInfo & readinfo)
  {
    this->on_creation_ = true;
    ACE_Time_Value tv;
    tv <<= readinfo.source_timestamp;
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
                          ACE_TEXT ("-> UTC date =%#T\n"),
                          &tv));

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Statelistener:on_creation : ")
               ACE_TEXT ("Received datum for <%C> at %u\n"),
                data.key.in (),
                data.x));
    //one of the data must have the key 'KEY_1' with x == 1
    if((strcmp(data.key.in() ,"KEY_1")==0) && (data.x == 1L))
      {
        this->create_data_ = true;
      }
  }

  void
  StateListener_exec_i::on_one_update (const ::TestTopic & /*datum*/,
                                       const ::CCM_DDS::ReadInfo & /*info*/)
  {
    this->no_operation_ = false;
  }

  void
  StateListener_exec_i::on_many_updates (const ::TestTopic_Seq & data,
                                         const ::CCM_DDS::ReadInfoSeq & readinfoseq)
  {
    this->on_many_update_ = true;
    for(CORBA::ULong i = 0; i < readinfoseq.length(); ++i)
      {
        if( readinfoseq[i].instance_status != CCM_DDS::INSTANCE_UPDATED)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected info.instance_status ")
                                  ACE_TEXT ("'CCM_DDS::INSTANCE_UPDATED'")
                                  ACE_TEXT ("  with operation 'on_many_updates' from StateListener in Receiver\n")
                        ));

          }
        ACE_Time_Value tv;
        tv <<= readinfoseq[i].source_timestamp;
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
                              ACE_TEXT ("-> UTC date =%#T\n"),
                              &tv));
      }
    for(CORBA::ULong i = 0; i < data.length(); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL keyed test info : ")
                    ACE_TEXT ("Number <%d> : received TestTopic_info for <%C> at %u\n"),
                    i,
                    data[i].key.in (),
                    data[i].x));
        //one of the data must have the key 'KEY_1' with x == 2
        if((strcmp(data[i].key,"KEY_1")==0 ) && (data[i].x == 2L))
          {
            this->update_data_ = true;
          }
       }
  }

  void
  StateListener_exec_i::on_deletion (const ::TestTopic & datum,
                                    const ::CCM_DDS::ReadInfo & info)
  {
    if(info.instance_status != CCM_DDS::INSTANCE_DELETED)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected info.instance_status ")
                              ACE_TEXT ("'CCM_DDS::INSTANCE_DELETED'")
                              ACE_TEXT ("  with operation 'on_deletion' from StateListener in Receiver\n")
                    ));

      }
    if((!datum.key.in()==0) && (info.instance_status == CCM_DDS::INSTANCE_DELETED))
      {
        this->on_deletion_ = true;
      }
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
    on_creation_(false),
    on_many_update_(false),
    on_deletion_(false),
    create_data_(false),
    update_data_(false),
    reader_data_(false)
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
            ACE_Time_Value tv;
            tv <<= readinfoseq[i].source_timestamp;
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
                                  ACE_TEXT ("-> UTC date =%#T\n"),
                                  &tv));
          }
        for(CORBA::ULong i = 0; i < TestTopic_infos->length(); ++i)
          {
            this->reader_data_= true;
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL keyed test info : ")
                       ACE_TEXT ("Number <%d> : received TestTopic_info for <%C> at %u\n"),
                        i,
                        TestTopic_infos[i].key.in (),
                        TestTopic_infos[i].x));
          }
      }
    catch (const CCM_DDS::InternalError& )
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
                                     this->on_many_update_,
                                     this->on_deletion_,
                                     this->create_data_,
                                     this->update_data_);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::SL_ManyByMany::CCM_Receiver_Context::_narrow (ctx);
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

    lc->mode (::CCM_DDS::MANY_BY_MANY);
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
                               ACE_TEXT (" operation 'on_one_update' from StateListener in Receiver\n")
                    ));
      }
    if(!this->on_creation_ .value ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" operation 'on_creation' from StateListener in Receiver\n")
                    ));
      }
    if(!this->create_data_ .value ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" data with 'on_creation' from StateListener in Receiver\n")
                    ));
      }
    if(!this->on_many_update_.value  ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR:didn't receive the expected ")
                               ACE_TEXT (" operation 'on_many_updates' from StateListener in Receiver\n")
                    ));
      }
    if(!this->update_data_.value  ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR:didn't receive the expected ")
                               ACE_TEXT (" data with 'on_many_updates' from StateListener in Receiver\n")
                    ));
      }
    if(!this->on_deletion_.value ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" operation 'on_deletion' from StateListener in Receiver\n")
                    ));
      }
      if(this->reader_data_.value ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did receive unexpected ")
                               ACE_TEXT (" data on the Reader in combination with StateListener in Receiver\n")
                    ));
      }
    if(no_error==true)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK : Have received the  expected ")
                               ACE_TEXT (" operations for MANY_BY_MANY from StateListener in Receiver\n")
                   ));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_ManyByMany_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
