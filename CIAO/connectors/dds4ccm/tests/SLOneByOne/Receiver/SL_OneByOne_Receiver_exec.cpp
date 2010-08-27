// -*- C++ -*-
//
// $Id$

// Test for statelistener: ONE_BY_ONE. Expect the operations on_creation, on_deletion, on_one_update

#include "SL_OneByOne_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/TimeUtilities.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#define ON_CREATION_EXPECTED 4
#define ON_ONE_UPDATE_EXPECTED 4
#define ON_DELETION_EXPECTED 4

namespace CIAO_SL_OneByOne_Receiver_Impl
{
  //============================================================
  // StateListener_exec_i
  //============================================================
  StateListener_exec_i::StateListener_exec_i (Atomic_Long &on_many_updates,
                                              Atomic_Long &on_creation,
                                              Atomic_Long &on_one_update,
                                              Atomic_Long &on_deletion,
                                              ACE_Thread_ID &thread_id)
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
    ACE_Thread_ID t_id;
    this->thread_id_.handle (t_id.handle ());
    this->thread_id_.id (t_id.id ());
    if (!datum.key.in() == 0 &&
        info.instance_status == CCM_DDS::INSTANCE_CREATED)
      {
        ++this->on_creation_;
      }
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("StateListener_exec_i::on_creation - ")
                          ACE_TEXT ("Received sample: key <%C> - iteration <%d>\n"),
                          datum.key.in (),
                          datum.x));
  }

  void
  StateListener_exec_i::on_one_update (const ::TestTopic & datum,
                                       const ::CCM_DDS::ReadInfo & info)
  {
    if (info.instance_status != CCM_DDS::INSTANCE_UPDATED)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: did not receive the expected info.status ")
                    ACE_TEXT ("'CCM_DDS::INSTANCE_UPDATED' ")
                    ACE_TEXT ("with operation 'on_one_update' ")
                    ACE_TEXT ("from StateListener in Receiver\n")));

      }

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("StateListener_exec_i::on_one_update - ")
                          ACE_TEXT ("Received sample: key <%C> - iteration <%d>\n"),
                          datum.key.in (),
                          datum.x));

    if (!datum.key.in() == 0 &&
        info.instance_status == CCM_DDS::INSTANCE_UPDATED)
      {
        ++this->on_one_update_;
      }
  }

  void
  StateListener_exec_i::on_many_updates (const ::TestTopicSeq & /*data*/,
                                         const ::CCM_DDS::ReadInfoSeq & infos)
  {
    this->on_many_updates_ += infos.length ();
  }

  void
  StateListener_exec_i::on_deletion (const ::TestTopic & datum,
                                    const ::CCM_DDS::ReadInfo & info)
  {
    if (info.instance_status != CCM_DDS::INSTANCE_DELETED)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: did not receive the expected info.status ")
                    ACE_TEXT ("'CCM_DDS::INSTANCE_DELETED' ")
                    ACE_TEXT ("with operation 'on_deletion' ")
                    ACE_TEXT ("from StateListener in Receiver\n")));

      }
    if (!datum.key.in() == 0 &&
        info.instance_status == CCM_DDS::INSTANCE_DELETED)
      {
        ++this->on_deletion_;
      }
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("StateListener_exec_i::on_deletion - ")
                          ACE_TEXT ("Received sample: key <%C> - iteration <%d>\n"),
                          datum.key.in (),
                          datum.x));
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : on_many_updates_ (0),
      updater_data_ (0),
      on_creation_ (0),
      on_one_update_ (0),
      on_deletion_ (0),
      thread_id_listener_ (0, 0)
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

  ::SL_OneByOne::SL_OneByOneConnector::CCM_StateListener_ptr
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

    if (::CORBA::is_nil (lc.in ()))
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw ::CORBA::INTERNAL ();
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
    if (this->on_many_updates_.value () != 0)
      {
         ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("ERROR: Received an unexpected ")
                     ACE_TEXT ("number of samples on 'on_many_updates' ")
                     ACE_TEXT ("expected <0> - received <%d>\n"),
                     this->on_many_updates_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("OK: Received the expected ")
                    ACE_TEXT ("number of samples on 'on_many_updates' ")
                    ACE_TEXT ("expected <0> - received <%d>\n"),
                    this->on_many_updates_.value ()));
      }
    if (this->on_creation_.value () != ON_CREATION_EXPECTED)
      {
         ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("ERROR: didn't receive the expected ")
                     ACE_TEXT (" number of samples on 'on_creation' ")
                     ACE_TEXT ("expected <%d> - received <%d>\n"),
                     ON_CREATION_EXPECTED,
                     this->on_creation_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("OK: Received the expected ")
                    ACE_TEXT ("number of samples on 'on_creation' ")
                     ACE_TEXT ("expected <%d> - received <%d>\n"),
                     ON_CREATION_EXPECTED,
                     this->on_creation_.value ()));
      }

    if (this->on_one_update_.value () != ON_ONE_UPDATE_EXPECTED)
      {
         ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("ERROR: didn't receive the expected ")
                     ACE_TEXT (" number of samples on 'on_one_update' ")
                     ACE_TEXT ("expected <%d> - received <%d>\n"),
                     ON_ONE_UPDATE_EXPECTED,
                     this->on_one_update_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("OK: Received the expected ")
                    ACE_TEXT ("number of samples on 'on_one_update' ")
                    ACE_TEXT ("expected <%d> - received <%d>\n"),
                    ON_ONE_UPDATE_EXPECTED,
                    this->on_one_update_.value ()));
      }

    if (this->on_deletion_.value () != ON_DELETION_EXPECTED)
      {
         ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("ERROR: didn't receive the expected ")
                     ACE_TEXT ("number of samples on 'on_deletion' ")
                     ACE_TEXT ("expected <%d> - received <%d>\n"),
                     ON_DELETION_EXPECTED,
                     this->on_deletion_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("OK: Received the expected ")
                    ACE_TEXT ("number of samples on 'on_deletion' ")
                    ACE_TEXT ("expected <%d> - received <%d>\n"),
                    ON_DELETION_EXPECTED,
                    this->on_deletion_.value ()));
      }

    char ccm_buf [65];
    ACE_Thread_ID ccm_thread_id;
    ccm_thread_id.to_string (ccm_buf);

    char list_buf [65];
    this->thread_id_listener_.to_string (list_buf);


    if (this->thread_id_listener_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for StatusListener not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_listener_ == ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "OK: "
                              "Thread switch for StatusListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                               "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
     }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread switch for StatusListener "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    #else
    else if (this->thread_id_listener_ == ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: StatusListener: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK: StatusListener: "
                              "DDS seems to use its own thread for its callback: "
                               "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
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
