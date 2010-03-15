// -*- C++ -*-
//
// $Id$

// Test for statelistener: ONE_BY_ONE. Expect the operations on_creation, on_deletion, on_one_update

#include "SL_OneByOne_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/dds/Utils.h"
#include "dds4ccm/impl/dds/TimeUtilities.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO_SL_OneByOne_Receiver_Impl
{
  //============================================================
  // StateListener_exec_i
  //============================================================
  StateListener_exec_i::StateListener_exec_i (Atomic_Boolean &on_many_updates,
                                              Atomic_Boolean &on_creation,
                                              Atomic_Boolean &on_one_update,
                                              Atomic_Boolean &on_deletion,
                                              Atomic_ThreadId &thread_id)
    : on_many_updates_ (on_many_updates),
      on_creation_ (on_creation),
      on_one_update_ (on_one_update),
      on_deletion_ (on_deletion),
      thread_id_  (thread_id)
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
    ACE_DEBUG ((LM_DEBUG, "StateListener_exec_i::on_creation - "
                          "Received sample: key <%C> - iteration <%d>\n",
                          datum.key.in (),
                          datum.x));
  }

  void
  StateListener_exec_i::on_one_update (const ::TestTopic & datum,
                                       const ::CCM_DDS::ReadInfo & info)
  {
    if (info.instance_status != CCM_DDS::INSTANCE_UPDATED)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected info.status ")
                              ACE_TEXT ("'CCM_DDS::INSTANCE_UPDATED'")
                              ACE_TEXT ("  with operation 'on_one_update' from StateListener in Receiver\n")
                    ));

      }

    ACE_DEBUG ((LM_DEBUG, "StateListener_exec_i::on_one_update - "
                          "Received sample: key <%C> - iteration <%d>\n",
                          datum.key.in (),
                          datum.x));

    if (!datum.key.in() == 0 && info.instance_status == CCM_DDS::INSTANCE_UPDATED)
      {
        this->on_one_update_ = true;
      }
  }

  void
  StateListener_exec_i::on_many_updates (const ::TestTopic_Seq & /*data*/,
                                         const ::CCM_DDS::ReadInfoSeq & /*infos*/)
  {
    this->on_many_updates_ = true;
  }

  void
  StateListener_exec_i::on_deletion (const ::TestTopic & datum,
                                    const ::CCM_DDS::ReadInfo & info)
  {
    if (info.instance_status != CCM_DDS::INSTANCE_DELETED)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected info.status ")
                              ACE_TEXT ("'CCM_DDS::INSTANCE_DELETED' ")
                              ACE_TEXT ("with operation 'on_deletion' from StateListener in Receiver\n")
                    ));

      }
    if (!datum.key.in() == 0 && info.instance_status == CCM_DDS::INSTANCE_DELETED)
      {
        this->on_deletion_ = true;
      }
    ACE_DEBUG ((LM_DEBUG, "StateListener_exec_i::on_deletion - "
                      "Received sample: key <%C> - iteration <%d>\n",
                      datum.key.in (),
                      datum.x));
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : on_many_updates_ (false),
      updater_data_ (false),
      on_creation_ (false),
      on_one_update_ (false),
      on_deletion_ (false),
      thread_id_listener_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Component attributes.
  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::TestTopic::CCM_StateListener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return new StateListener_exec_i (this->on_many_updates_,
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
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::StateListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (CORBA::is_nil (lc.in ()))
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
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
    CORBA::Boolean no_error = true;
    if (this->on_many_updates_.value ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Received unexpected ")
                               ACE_TEXT ("operation 'on_many_updates' from StateListener in Receiver\n")
                    ));
      }

    if (!this->on_creation_.value ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" operation 'on_creation' from StateListener in Receiver\n")
                    ));
      }
    if (!this->on_one_update_.value  ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" operation 'on_one_update' from StateListener in Receiver\n")
                    ));
      }
    if (!this->on_deletion_.value ())
      {
         no_error = false;
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT (" operation 'on_deletion' from StateListener in Receiver\n")
                    ));
      }

    if (no_error)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: Have received the expected ")
                               ACE_TEXT (" operations for ONE_BY_ONE from StateListener in Receiver\n")
                   ));
      }

    if (this->thread_id_listener_.value () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for StatusListener not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (ACE_OS::thr_equal (this->thread_id_listener_.value (),
                                ACE_Thread::self ()))
      {
        ACE_DEBUG ((LM_DEBUG, "OK: "
                              "Thread switch for StatusListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread switch for StatusListener "
                              "doesn't seem to work! "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    #else
    else if (ACE_OS::thr_equal (this->thread_id_listener_.value (),
                                ACE_Thread::self ()))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: StatusListener: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK: StatusListener: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    #endif
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
