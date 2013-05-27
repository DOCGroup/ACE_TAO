//
// $Id$
//

#include "Client_Task.h"
#include "tao/AnyTypeCode/Any.h"

namespace
{
  const ACE_Time_Value TRANSIENT_HOLDOFF (0, 2000); // 2ms delay
  const int TRANSIENT_LIMIT = 100;
}

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
  , tasks_running_(0)
  , done_(false)
  , id_ (ident)
{
}

bool
Client_Task::done(void) const
{
  if (tasks_running_ == 0)
    return done_;

  //else
  return false;
}

const char *
Client_Task::ID (void) const
{
  return id_.c_str ();
}

int
Client_Task::svc (void)
{
  ++tasks_running_;
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) Client_Task %s started\n", this->ID ()));
    }
  Test::Payload payload (this->event_size_);
  payload.length (this->event_size_);

  for (CORBA::ULong j = 0; j != payload.length (); ++j)
    {
      payload[j] = (j % 256);
    }

  try
    {
      this->validate_connection ();

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("PolicyCurrent");
      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      CORBA::Any scope_as_any;
      scope_as_any <<= this->sync_scope_;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        this->orb_->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                                   scope_as_any);

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::ADD_OVERRIDE);

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client_Task %s sending %d payloads.\n",
                     this->ID (), this->event_count_));
        }
      int transient_count= 0;
      for (int i = 0; i != this->event_count_; ++i)
        {
          try
            {
              this->payload_receiver_->more_data (payload, this->sync_scope_ != Messaging::SYNC_WITH_TARGET);
              transient_count= 0;
            }
          catch (const CORBA::TRANSIENT &)
            {
              if (++transient_count < TRANSIENT_LIMIT)
                {
                  if (transient_count == TRANSIENT_LIMIT / 2)
                    {
                      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client_Task %s large TRANSIENTS encountered at payload %d/%d.\n",
                                this->ID (), i+1, this->event_count_));
                    }
                  ACE_OS::sleep (TRANSIENT_HOLDOFF);
                  --i; // Re-try the same message
                }
              else
                {
                  break; // Abort the message sending.
                }
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client_Task %s: ", this->ID ()));
      ex._tao_print_exception ("CORBA Exception caught:");
      --tasks_running_;
      done_ = true;
      return -1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P) Unknown exception caught\n"));
      --tasks_running_;
      done_ = true;
      return -1;
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client_Task %s finished.\n", this->ID ()));
    }
  --tasks_running_;
  done_ = true;
  return 0;
}

void
Client_Task::validate_connection (void)
{
  try
    {
      int transient_count= 0;
      Test::Payload payload(0);
      for (int i = 0; i != 100; ++i)
        {
          try
            {
              this->payload_receiver_->ping();
              transient_count= 0;
            }
          catch (const CORBA::TRANSIENT &)
            {
              if (++transient_count < TRANSIENT_LIMIT)
                {
                  if (transient_count == TRANSIENT_LIMIT / 2)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) Client Task %s large TRANSIENTS encountered at validation %d%%.\n",
                        this->ID (), i+1));
                    }
                  ACE_OS::sleep (TRANSIENT_HOLDOFF);
                  --i; // Re-try the same message
                }
              else
                {
                  break; // Abort the message sending.
                }
            }
        }
    }
  catch (const CORBA::Exception &)
    {
      // Deliberatly ignores any other corba exceptions
    }
}
