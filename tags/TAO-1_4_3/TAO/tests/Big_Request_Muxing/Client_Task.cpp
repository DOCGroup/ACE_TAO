//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(Big_Request_Muxing, Client_Task, "$Id$")

Client_Task::Client_Task (ACE_Thread_Manager *thr_mgr,
                          Test::Payload_Receiver_ptr payload_receiver,
                          CORBA::Long event_count,
                          CORBA::ULong event_size,
                          CORBA::ORB_ptr orb,
                          Messaging::SyncScope sync_scope)
  : ACE_Task_Base (thr_mgr)
  , payload_receiver_ (Test::Payload_Receiver::_duplicate (payload_receiver))
  , event_count_ (event_count)
  , event_size_ (event_size)
  , orb_ (CORBA::ORB::_duplicate (orb))
  , sync_scope_ (sync_scope)
{
}

int
Client_Task::svc (void)
{
  Test::Payload payload (this->event_size_);
  payload.length (this->event_size_);

  for (CORBA::ULong j = 0; j != payload.length (); ++j)
    payload[j] = (j % 256);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->validate_connection (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("PolicyCurrent"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Any scope_as_any;
      scope_as_any <<= this->sync_scope_;


      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        this->orb_->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                                   scope_as_any
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::ADD_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (int i = 0; i != this->event_count_; ++i)
        {
          this->payload_receiver_->more_data (payload ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

void
Client_Task::validate_connection (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      for (int i = 0; i != 100; ++i)
        {
          (void) this->payload_receiver_->get_message_count (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY {} ACE_ENDTRY;
}
