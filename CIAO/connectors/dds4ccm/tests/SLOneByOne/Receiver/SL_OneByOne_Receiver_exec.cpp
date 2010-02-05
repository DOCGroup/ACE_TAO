// -*- C++ -*-
//
// $Id$

// Test for statelistener: ONE_BY_ONE. Expect the operations on_creation, on_deletion, on_one_update 

#include "SL_OneByOne_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/TimeUtilities.h"

namespace CIAO_SL_OneByOne_Receiver_Impl
{
  //============================================================
  // read_action_Generator
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
  // StateListener_exec_i
  //============================================================
  StateListener_exec_i::StateListener_exec_i (Atomic_Boolean &no_operation,
                                              Atomic_Boolean &on_creation,
                                              Atomic_Boolean &on_one_update,
                                              Atomic_Boolean &on_deletion,
                                              Atomic_ThreadId &thread_id)
    :no_operation_(no_operation),
     on_creation_(on_creation),
     on_one_update_(on_one_update),
     on_deletion_(on_deletion),
     thread_id_ (thread_id)
  {
  }

  StateListener_exec_i::~StateListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::StateListener
  void
  StateListener_exec_i::on_creation (const ::TestTopic & datum,
                                     const ::CCM_DDS::ReadInfo & info)
  {
    this->thread_id_ = ACE_Thread::self ();
    if (!datum.key.in() == 0 && info.instance_status == CCM_DDS::INSTANCE_CREATED)
      {
        this->on_creation_ = true;
      }
  }

  void
  StateListener_exec_i::on_one_update (const ::TestTopic & datum,
                                       const ::CCM_DDS::ReadInfo & info)
  {
    this->thread_id_ = ACE_Thread::self ();
    if (info.instance_status != CCM_DDS::INSTANCE_UPDATED)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected info.status ")
                              ACE_TEXT ("'CCM_DDS::INSTANCE_UPDATED'")
                              ACE_TEXT ("  with operation 'on_one_update' from StateListener in Receiver\n")
                    ));

      }
    if (!datum.key.in() == 0 && info.instance_status == CCM_DDS::INSTANCE_UPDATED)
      {
        this->on_one_update_ = true;
      }
  }

  void
  StateListener_exec_i::on_many_updates (const ::TestTopic_Seq & /*data*/,
                                         const ::CCM_DDS::ReadInfoSeq & /*infos*/)
  {
    this->no_operation_ = false;
  }

  void
  StateListener_exec_i::on_deletion (const ::TestTopic & datum,
                                    const ::CCM_DDS::ReadInfo & info)
  {
    this->thread_id_ = ACE_Thread::self ();
    if (info.instance_status != CCM_DDS::INSTANCE_DELETED)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected info.status ")
                              ACE_TEXT ("'CCM_DDS::INSTANCE_DELETED'")
                              ACE_TEXT ("  with operation 'on_deletion' from StateListener in Receiver\n")
                    ));

      }
    if (!datum.key.in() == 0 && info.instance_status == CCM_DDS::INSTANCE_DELETED)
      {
        this->on_deletion_ = true;
      }
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
  : rate_ (10),
    no_operation_(true),
    updater_data_(false),
    on_creation_(false),
    on_one_update_(false),
    on_deletion_(false),
    thread_id_listener_ (0)
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
        this->reader_->read_all (TestTopic_infos.out(), readinfoseq.out());
        for(CORBA::ULong i = 0; i < readinfoseq->length(); ++i)
          {
            this->updater_data_ = true;
          ACE_Time_Value tv;
          tv <<= readinfoseq[i].source_timestamp;
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
                                ACE_TEXT ("-> UTC date =%#T\n"),
                                &tv));
          }
        for(CORBA::ULong i = 0; i < TestTopic_infos->length (); ++i)
          {
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
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::TestTopic::CCM_StateListener_ptr
    Receiver_exec_i::get_info_out_data_listener (void)
  {
    return new StateListener_exec_i(this->no_operation_,
                                     this->on_creation_,
                                     this->on_one_update_,
                                     this->on_deletion_,
                                     this->thread_id_listener_);
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
    this->reader_ = this->context_->get_connection_info_out_data ();
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
                               ACE_TEXT (" operation 'on_many_updates' from StateListener in Receiver\n")
                    ));
      }
    if(!this->on_creation_ .value ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" operation 'on_creation' from StateListener in Receiver\n")
                    ));
      }
    if(!this->on_one_update_.value  ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" operation 'on_one_update' from StateListener in Receiver\n")
                    ));
      }
    if(!this->on_deletion_.value ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" operation 'on_deletion' from StateListener in Receiver\n")
                    ));
      }

    if(no_error==true)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK : Have received the  expected ")
                               ACE_TEXT (" operations for ONE_BY_ONE from StateListener in Receiver\n")
                   ));
      }
    if (this->thread_id_listener_.value () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                               "Thread ID for StatusListener not set!\n"));
      }
    else if (this->thread_id_listener_.value () == ACE_Thread::self ())
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                               "Thread switch for StatusListener "
                               "doesn't seem to work! "
                               "listener <%u> - component <%u>\n",
                               this->thread_id_listener_.value (),
                               ACE_Thread::self ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK : "
                               "Thread switch for StatusListener seems OK. "
                               "listener <%u> - component <%u>\n",
                               this->thread_id_listener_.value (),
                               ACE_Thread::self ()));
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
