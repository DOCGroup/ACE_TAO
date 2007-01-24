//
// $Id$
//

#include "Client_Task.h"
#include "tao/AnyTypeCode/Any.h"

ACE_RCSID(Big_Request_Muxing, Client_Task, "$Id$")

Client_Task::Client_Task (ACE_Thread_Manager *thr_mgr,
                          Test::Payload_Receiver_ptr payload_receiver,
                          CORBA::Long event_count,
                          CORBA::ULong event_size,
                          CORBA::ORB_ptr orb,
                          Messaging::SyncScope sync_scope,
                          const ACE_CString & ident)
  : ACE_Task_Base (thr_mgr)
  , payload_receiver_ (Test::Payload_Receiver::_duplicate (payload_receiver))
  , event_count_ (event_count)
  , event_size_ (event_size)
  , orb_ (CORBA::ORB::_duplicate (orb))
  , sync_scope_ (sync_scope)
  , done_(false)
  , id_ (ident)
{
}

bool
Client_Task::done(void) const
{
  return done_;
}

void
Client_Task::do_invocations(Test::Payload& payload ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG((LM_DEBUG, "(%P|%t)Client_Task %s sending %d payloads.\n",
             this->id_.c_str(), this->event_count_));

  for (int i = 0; i != this->event_count_; ++i)
    {
      this->payload_receiver_->more_data (payload ACE_ENV_ARG_PARAMETER);
    }
}

void
Client_Task::do_sync_none_invocations(Test::Payload& payload ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG((LM_DEBUG, "(%P|%t)Client_Task %s sending %d SYNC_NONE payloads.\n",
             this->id_.c_str(), this->event_count_));

  for (int i = 0; i != this->event_count_; ++i)
    {
      this->payload_receiver_->sync_none_more_data (payload ACE_ENV_ARG_PARAMETER);
    }
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Client_Task %s started\n",this->id_.c_str()));
  Test::Payload payload (this->event_size_);
  payload.length (this->event_size_);

  for (CORBA::ULong j = 0; j != payload.length (); ++j)
    payload[j] = (j % 256);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->validate_connection ();

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("PolicyCurrent"
                                                ACE_ENV_ARG_PARAMETER);
      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

      CORBA::Any scope_as_any;
      scope_as_any <<= this->sync_scope_;


      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        this->orb_->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                                   scope_as_any
                                   ACE_ENV_ARG_PARAMETER);

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::ADD_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);

      if (this->sync_scope_ == Messaging::SYNC_NONE)
        this->do_sync_none_invocations(payload ACE_ENV_SINGLE_ARG_PARAMETER);
      else
        this->do_invocations(payload ACE_ENV_SINGLE_ARG_PARAMETER);

    }
  ACE_CATCHANY
    {
      ACE_DEBUG((LM_DEBUG, "(%P|%t)Client_Task %s: ",
                 this->id_.c_str()));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "");
      done_ = true;
      return -1;
    }
  ACE_ENDTRY;
  ACE_DEBUG((LM_DEBUG, "(%P|%t)Client_Task %s finished.\n", this->id_.c_str()));
  done_ = true;
  return 0;
}

void
Client_Task::validate_connection (void)
{
  ACE_TRY
    {
      Test::Payload payload(0);
      for (int i = 0; i != 100; ++i)
        {
          (void) this->payload_receiver_->more_data (payload ACE_ENV_ARG_PARAMETER);
        }
    }
  ACE_CATCHANY {} ACE_ENDTRY;
}
