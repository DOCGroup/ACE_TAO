// $Id$

#include "tao/Policy_Manager.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#include "tao/Environment.h"
#include "tao/ORB_Core.h"

#include "tao/Buffering_Constraint_Policy.h"
#include "tao/Messaging_Policy_i.h"
#include "tao/Client_Priority_Policy.h"

#if !defined (__ACE_INLINE__)
# include "tao/Policy_Manager.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Policy_Manager, "$Id$")

TAO_Policy_Manager_Impl::~TAO_Policy_Manager_Impl (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->cleanup_i (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions...
    }
  ACE_ENDTRY;
}

void
TAO_Policy_Manager_Impl::copy_from (TAO_Policy_Manager_Impl *source,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  if (source == 0)
    return;

  this->cleanup_i (ACE_TRY_ENV);
  ACE_CHECK;

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

  if (source->relative_roundtrip_timeout_ != 0)
    {
      this->relative_roundtrip_timeout_ =
        source->relative_roundtrip_timeout_->clone ();

      this->count_++;
    }

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  if (source->client_priority_ != 0)
    {
      this->client_priority_ =
        source->client_priority_->clone ();

      this->count_++;
    }

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

  if (source->sync_scope_ != 0)
    {
      this->sync_scope_ =
        source->sync_scope_->clone ();

      this->count_++;
    }

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  if (source->buffering_constraint_ != 0)
    {
      this->buffering_constraint_ =
        source->buffering_constraint_->clone ();

      this->count_++;
    }

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  for (CORBA::ULong i = 0;
       i < source->other_policies_.length ();
       ++i)
    {
      CORBA::Policy_ptr policy = source->other_policies_[i];
      if (CORBA::is_nil (policy))
        continue;

      CORBA::Policy_var copy = policy->copy (ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::ULong length = this->other_policies_.length ();
      this->other_policies_.length (length + 1);
      this->other_policies_[length] = copy._retn ();
      this->count_++;
    }
}

void
TAO_Policy_Manager_Impl::cleanup_i (CORBA::Environment &ACE_TRY_ENV)
{
  for (CORBA::ULong i = 0;
       i < this->other_policies_.length ();
       ++i)
    {
      this->other_policies_[i]->destroy (ACE_TRY_ENV);
      ACE_CHECK;
      this->other_policies_[i] = CORBA::Policy::_nil ();
    }
  this->other_policies_.length (0);

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

  CORBA::release (this->relative_roundtrip_timeout_);
  this->relative_roundtrip_timeout_ = 0;

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  CORBA::release (this->client_priority_);
  this->client_priority_ = 0;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

  CORBA::release (this->sync_scope_);
  this->sync_scope_ = 0;

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  CORBA::release (this->buffering_constraint_);
  this->buffering_constraint_ = 0;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  this->count_ = 0;
}

  // @@ !!! Add comments regarding Policy lifetimes, etc.
void
TAO_Policy_Manager_Impl::set_policy_overrides (
    const CORBA::PolicyList &policies,
    CORBA::SetOverrideType set_add,
    CORBA::Environment &ACE_TRY_ENV)
{
  // @@ The spec does not say what to do on this case.
  if (set_add != CORBA::SET_OVERRIDE
      && set_add != CORBA::ADD_OVERRIDE)
    ACE_THROW (CORBA::BAD_PARAM ());

  if (set_add == CORBA::SET_OVERRIDE)
    {
      this->cleanup_i (ACE_TRY_ENV);
      ACE_CHECK;
    }

  for (CORBA::ULong i = 0; i < policies.length ();  ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      if (CORBA::is_nil (policy))
        continue;

      CORBA::ULong slot = policy->policy_type (ACE_TRY_ENV);
      ACE_CHECK;

      switch (slot)
        {

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

        case TAO_MESSAGING_RELATIVE_RT_TIMEOUT_POLICY_TYPE:
          {
            Messaging::RelativeRoundtripTimeoutPolicy_var p =
              Messaging::RelativeRoundtripTimeoutPolicy::_narrow (policy);

            if (CORBA::is_nil (p.in ()))
              ACE_THROW (CORBA::INTERNAL (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_POLICY_NARROW_CODE,
                        0),
                      CORBA::COMPLETED_NO));

            CORBA::release (this->relative_roundtrip_timeout_);
            this->relative_roundtrip_timeout_ =
              ACE_dynamic_cast (TAO_RelativeRoundtripTimeoutPolicy*,
                                p._retn ());
            this->count_++;
          }
          break;

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

        case TAO_CLIENT_PRIORITY_POLICY_TYPE:
          {
            TAO::ClientPriorityPolicy_var p =
              TAO::ClientPriorityPolicy::_narrow (policy);

            if (CORBA::is_nil (p.in ()))
              ACE_THROW (CORBA::INTERNAL (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_POLICY_NARROW_CODE,
                        0),
                      CORBA::COMPLETED_NO));

            CORBA::release (this->client_priority_);
            this->client_priority_ =
              ACE_dynamic_cast (TAO_Client_Priority_Policy *,
                                p._retn ());

            this->count_++;
          }
          break;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

        case TAO_MESSAGING_SYNC_SCOPE_POLICY_TYPE:
          {
            Messaging::SyncScopePolicy_var p =
              Messaging::SyncScopePolicy::_narrow (policy);

            if (CORBA::is_nil (p.in ()))
              ACE_THROW (CORBA::INTERNAL (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_POLICY_NARROW_CODE,
                        0),
                      CORBA::COMPLETED_NO));

            CORBA::release (this->sync_scope_);
            this->sync_scope_ =
              ACE_dynamic_cast (TAO_Sync_Scope_Policy*,
                                p._retn ());

            this->count_++;
          }
          break;

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

        case TAO_BUFFERING_CONSTRAINT_POLICY_TYPE:
          {
            TAO::BufferingConstraintPolicy_var p =
              TAO::BufferingConstraintPolicy::_narrow (policy);

            if (CORBA::is_nil (p.in ()))
              ACE_THROW (CORBA::INTERNAL (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_POLICY_NARROW_CODE,
                        0),
                      CORBA::COMPLETED_NO));

            CORBA::release (this->buffering_constraint_);
            this->buffering_constraint_ =
              ACE_dynamic_cast (TAO_Buffering_Constraint_Policy *,
                                p._retn ());

            this->count_++;
          }
          break;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

        case TAO_MESSAGING_REBIND_POLICY_TYPE:
        case TAO_MESSAGING_REQUEST_PRIORITY_POLICY_TYPE:
        case TAO_MESSAGING_REPLY_PRIORITY_POLICY_TYPE:
        case TAO_MESSAGING_REQUEST_START_TIME_POLICY_TYPE:
        case TAO_MESSAGING_REQUEST_END_TIME_POLICY_TYPE:
        case TAO_MESSAGING_REPLY_START_TIME_POLICY_TYPE:
        case TAO_MESSAGING_REPLY_END_TIME_POLICY_TYPE:
        case TAO_MESSAGING_RELATIVE_REQ_TIMEOUT_POLICY_TYPE:
        case TAO_MESSAGING_ROUTING_POLICY_TYPE:
        case TAO_MESSAGING_MAX_HOPS_POLICY_TYPE:
        case TAO_MESSAGING_QUEUE_ORDER_POLICY_TYPE:
        default:
          {
            CORBA::Policy_var copy = policy->copy (ACE_TRY_ENV);
            ACE_CHECK;

            CORBA::ULong j = 0;
            CORBA::ULong length = this->other_policies_.length ();
            while (j != length)
              {
                CORBA::ULong current =
                  this->other_policies_[j]->policy_type (ACE_TRY_ENV);
                ACE_CHECK;

                if (current == slot)
                  {
                    this->other_policies_[j]->destroy (ACE_TRY_ENV);
                    ACE_CHECK;
                    this->other_policies_[j] = copy._retn ();
                    break;
                  }
                ++j;
              }
            if (j == length)
              {
                this->other_policies_.length (length + 1);
                this->other_policies_[j] = copy._retn ();
              }
            this->count_++;
          }
          break;
        }
    }
}

CORBA::PolicyList *
TAO_Policy_Manager_Impl::get_policy_overrides (
    const CORBA::PolicyTypeSeq &types,
    CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong types_length = types.length ();

  CORBA::ULong slots = types_length;
  if (slots == 0)
    slots = this->count_;


  CORBA::PolicyList *policy_list_ptr;

  ACE_NEW_THROW_EX (policy_list_ptr,
                    CORBA::PolicyList (slots),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::PolicyList_var policy_list (policy_list_ptr);

  policy_list->length (slots);

  CORBA::ULong n = 0;
  if (types_length == 0)
    {

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

      if (this->relative_roundtrip_timeout_ != 0)
        {
          policy_list[n++] =
            CORBA::Policy::_duplicate (this->relative_roundtrip_timeout_);
          ACE_CHECK_RETURN (0);
        }

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

      if (this->client_priority_ != 0)
        {
          policy_list[n++] =
            CORBA::Policy::_duplicate (this->client_priority_);
          ACE_CHECK_RETURN (0);
        }

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

      if (this->sync_scope_ != 0)
        {
          policy_list[n++] =
            CORBA::Policy::_duplicate (this->sync_scope_);
          ACE_CHECK_RETURN (0);
        }

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

      if (this->buffering_constraint_ != 0)
        {
          policy_list[n++] =
            CORBA::Policy::_duplicate (this->buffering_constraint_);
          ACE_CHECK_RETURN (0);
        }

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

      CORBA::ULong length = this->other_policies_.length ();
      for (CORBA::ULong i = 0; i != length; ++i)
        {
          if (CORBA::is_nil (this->other_policies_[i].in ()))
            continue;
          policy_list[n++] =
            CORBA::Policy::_duplicate (this->other_policies_[i].in ());
        }
    }
  else
    {
      for (CORBA::ULong j = 0; j < types.length ();  ++j)
        {
          CORBA::ULong slot = types[j];

          switch (slot)
            {

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

            case TAO_MESSAGING_RELATIVE_RT_TIMEOUT_POLICY_TYPE:
              if (this->relative_roundtrip_timeout_ != 0)
                {
                  policy_list[n++] =
                    CORBA::Policy::_duplicate (this->relative_roundtrip_timeout_);
                  ACE_CHECK_RETURN (0);
                }
              break;

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

            case TAO_CLIENT_PRIORITY_POLICY_TYPE:
              if (this->client_priority_ != 0)
                {
                  policy_list[n++] =
                    CORBA::Policy::_duplicate (this->client_priority_);
                  ACE_CHECK_RETURN (0);
                }
              break;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

            case TAO_MESSAGING_SYNC_SCOPE_POLICY_TYPE:
              if (this->sync_scope_ != 0)
                {
                  policy_list[n++] =
                    CORBA::Policy::_duplicate (this->sync_scope_);
                  ACE_CHECK_RETURN (0);
                }
              break;

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

            case TAO_BUFFERING_CONSTRAINT_POLICY_TYPE:
              if (this->buffering_constraint_ != 0)
                {
                  policy_list[n++] =
                    CORBA::Policy::_duplicate (this->buffering_constraint_);
                  ACE_CHECK_RETURN (0);
                }
              break;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

            case TAO_MESSAGING_REBIND_POLICY_TYPE:
            case TAO_MESSAGING_REQUEST_PRIORITY_POLICY_TYPE:
            case TAO_MESSAGING_REPLY_PRIORITY_POLICY_TYPE:
            case TAO_MESSAGING_REQUEST_START_TIME_POLICY_TYPE:
            case TAO_MESSAGING_REQUEST_END_TIME_POLICY_TYPE:
            case TAO_MESSAGING_REPLY_START_TIME_POLICY_TYPE:
            case TAO_MESSAGING_REPLY_END_TIME_POLICY_TYPE:
            case TAO_MESSAGING_RELATIVE_REQ_TIMEOUT_POLICY_TYPE:
            case TAO_MESSAGING_ROUTING_POLICY_TYPE:
            case TAO_MESSAGING_MAX_HOPS_POLICY_TYPE:
            case TAO_MESSAGING_QUEUE_ORDER_POLICY_TYPE:
            default:
              {
                CORBA::ULong length = this->other_policies_.length ();
                for (CORBA::ULong i = 0;
                     i != length;
                     ++i)
                  {
                    CORBA::ULong current =
                      this->other_policies_[i]->policy_type (ACE_TRY_ENV);
                    ACE_CHECK_RETURN (0);

                    if (current != slot)
                      continue;

                    policy_list[n++] =
                      CORBA::Policy::_duplicate (this->other_policies_[i].in ());
                    break;
                  }
              }
            }
        }
    }

  policy_list->length (n);
  return policy_list._retn ();
}

CORBA::Policy_ptr
TAO_Policy_Manager_Impl::get_policy (CORBA::PolicyType type,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  switch (type)
    {

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

    case TAO_MESSAGING_RELATIVE_RT_TIMEOUT_POLICY_TYPE:
      if (this->relative_roundtrip_timeout_ != 0)
        {
          return CORBA::Policy::_duplicate (this->relative_roundtrip_timeout_);
        }
      return CORBA::Policy::_nil ();

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

    case TAO_CLIENT_PRIORITY_POLICY_TYPE:
      if (this->client_priority_ != 0)
        {
          return CORBA::Policy::_duplicate (this->client_priority_);
        }
      return CORBA::Policy::_nil ();

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

    case TAO_MESSAGING_SYNC_SCOPE_POLICY_TYPE:
      if (this->sync_scope_ != 0)
        {
          return CORBA::Policy::_duplicate (this->sync_scope_);
        }
      return CORBA::Policy::_nil ();

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

    case TAO_BUFFERING_CONSTRAINT_POLICY_TYPE:
      if (this->buffering_constraint_ != 0)
        {
          return CORBA::Policy::_duplicate (this->buffering_constraint_);
        }
      return CORBA::Policy::_nil ();

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

    case TAO_MESSAGING_REBIND_POLICY_TYPE:
    case TAO_MESSAGING_REQUEST_PRIORITY_POLICY_TYPE:
    case TAO_MESSAGING_REPLY_PRIORITY_POLICY_TYPE:
    case TAO_MESSAGING_REQUEST_START_TIME_POLICY_TYPE:
    case TAO_MESSAGING_REQUEST_END_TIME_POLICY_TYPE:
    case TAO_MESSAGING_REPLY_START_TIME_POLICY_TYPE:
    case TAO_MESSAGING_REPLY_END_TIME_POLICY_TYPE:
    case TAO_MESSAGING_RELATIVE_REQ_TIMEOUT_POLICY_TYPE:
    case TAO_MESSAGING_ROUTING_POLICY_TYPE:
    case TAO_MESSAGING_MAX_HOPS_POLICY_TYPE:
    case TAO_MESSAGING_QUEUE_ORDER_POLICY_TYPE:
    default:
      {
        CORBA::ULong length = this->other_policies_.length ();
        for (CORBA::ULong i = 0;
             i != length;
             ++i)
          {
            CORBA::ULong current =
              this->other_policies_[i]->policy_type (ACE_TRY_ENV);
            ACE_CHECK_RETURN (0);

            if (current != type)
              continue;

            return CORBA::Policy::_duplicate (this->other_policies_[i].in ());
          }
      }
    }
  return CORBA::Policy::_nil ();
}

// ****************************************************************

TAO_Policy_Current_Impl &
TAO_Policy_Current::implementation (TAO_Policy_Current_Impl &current)
{
  TAO_TSS_Resources *tss =
    TAO_TSS_RESOURCES::instance ();

  TAO_Policy_Current_Impl *old = tss->policy_current_;
  tss->policy_current_ = &current;
  return *old;
}

TAO_Policy_Current_Impl &
TAO_Policy_Current::implementation (void) const
{
  return *TAO_TSS_RESOURCES::instance ()->policy_current_;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
