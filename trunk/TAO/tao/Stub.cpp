// $Id$

// @ (#)iiopobj.cpp     1.9 95/11/04
// Copyright 1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// XXXX Bridge:         CORBA::Object operations
//
// Some CORBA::Object and other operations are specific to this STUB
// based implementation, and can neither be used by other kinds of
// objref nor have a default implementation.

#include "tao/Endpoint.h"
#include "tao/Stub.h"
#include "tao/Sequence.h"
#include "tao/Object.h"
#include "tao/NVList.h"
#include "tao/Invocation.h"
#include "tao/Asynch_Invocation.h"
#include "tao/DII_Invocation.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Sync_Strategies.h"
#include "tao/Buffering_Constraint_Policy.h"
#include "tao/Messaging_Policy_i.h"
#include "tao/Client_Priority_Policy.h"
#include "tao/debug.h"

#if (TAO_HAS_RT_CORBA == 1)
# include "tao/RT_Policy_i.h"
#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "tao/Stub.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, TAO_Stub, "$Id$")

  TAO_Stub::TAO_Stub (char *repository_id,
                      const TAO_MProfile &profiles,
                      TAO_ORB_Core* orb_core)
    : type_id (repository_id),

#if (TAO_HAS_RT_CORBA == 1)

      priority_model_policy_ (0),
      priority_banded_connection_policy_ (0),
      client_protocol_policy_ (0),
      are_policies_parsed_ (0),

#endif /* TAO_HAS_RT_CORBA == 1 */
      base_profiles_ ((CORBA::ULong) 0),
      forward_profiles_ (0),
      profile_in_use_ (0),
      profile_lock_ptr_ (0),
      profile_success_ (0),
      refcount_lock_ (),
      refcount_ (1),
      use_locate_request_ (0),
      first_locate_request_ (0),
      orb_core_ (orb_core),
      orb_ (),
      servant_orb_ (),
      #if (TAO_HAS_CORBA_MESSAGING == 1)
      policies_ (0),
      #endif /* TAO_HAS_CORBA_MESSAGING == 1 */
      addressing_mode_ (0)

{
  if (this->orb_core_ == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO: (%P|%t) TAO_Stub created with default ")
                      ACE_TEXT ("ORB core\n")));
        }
      this->orb_core_ = TAO_ORB_Core_instance ();
    }

  // Duplicate the ORB_Core, otherwise the allocators and other
  // resources that this class references (directly or indirectly)
  // could be destroyed before it is time.
  (void) this->orb_core_->_incr_refcnt ();

  // Cache the ORB pointer to respond faster to certain queries.
  this->orb_ = CORBA::ORB::_duplicate (this->orb_core_->orb ());

  this->profile_lock_ptr_ =
    this->orb_core_->client_factory ()->create_profile_lock ();

  this->base_profiles (profiles);
  TAO_Profile *profile = 0;
  for (CORBA::ULong i = 0; i < this->base_profiles_.profile_count (); ++i)
    {
      // Get the ith profile
      profile = this->base_profiles_.get_profile (i);
      profile->the_stub (this);
    }
}

TAO_Stub::~TAO_Stub (void)
{
  ACE_ASSERT (this->refcount_ == 0);

  if (this->forward_profiles_)
    reset_profiles ();

  if (this->profile_in_use_ != 0)
    {
      if (this->orb_->orb_core () != 0)
        {
          // The hint in the profile is a hint for a client connection
          // handler.  If the ORB core doesn't exist, perhaps due to
          // it being destroy()ed, then no connectors exist so do not
          // reset the hint in case it points to non-existent
          // connection handler.
          this->profile_in_use_->endpoint ()->reset_hint ();
        }

      // decrease reference count on profile
      this->profile_in_use_->_decr_refcnt ();
      this->profile_in_use_ = 0;
    }

  delete this->profile_lock_ptr_;

#if (TAO_HAS_CORBA_MESSAGING == 1)

  delete this->policies_;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  this->orb_core_->_decr_refcnt ();

#if (TAO_HAS_RT_CORBA == 1)

  if (this->priority_model_policy_)
    this->priority_model_policy_->destroy ();

  if (this->priority_banded_connection_policy_)
    this->priority_banded_connection_policy_->destroy ();

  if (this->client_protocol_policy_)
    this->client_protocol_policy_->destroy ();

#endif /* TAO_HAS_RT_CORBA */
}

void
TAO_Stub::add_forward_profiles (const TAO_MProfile &mprofiles)
{
  // we assume that the profile_in_use_ is being
  // forwarded!  Grab the lock so things don't change.
  ACE_MT (ACE_GUARD (ACE_Lock,
                     guard,
                     *this->profile_lock_ptr_));

  TAO_MProfile *now_pfiles = this->forward_profiles_;
  if (now_pfiles == 0)
    now_pfiles = &this->base_profiles_;

  ACE_NEW (this->forward_profiles_,
           TAO_MProfile (mprofiles));

  // forwarded profile points to the new IOR (profiles)
  this->profile_in_use_->forward_to (this->forward_profiles_);

  // new profile list points back to the list which was forwarded.
  this->forward_profiles_->forward_from (now_pfiles);

  // make sure we start at the beginning of mprofiles
  this->forward_profiles_->rewind ();

  // Since we have been forwarded, we must set profile_success_ to 0
  // since we are starting a new with a new set of profiles!
  this->profile_success_ = 0;

  // Reset any flags that may be appropriate in the services that
  // selects profiles for invocation
  this->orb_core_->reset_service_profile_flags ();
}




// Quick'n'dirty hash of objref data, for partitioning objrefs into
// sets.
//
// NOTE that this must NOT go across the network!

// @@ Use all profiles for hash function!!!!!  FRED
//    can get different values, depending on the profile_in_use!!
CORBA::ULong
TAO_Stub::hash (CORBA::ULong max,
                CORBA::Environment &ACE_TRY_ENV)
{
  // we rely on the profile object that its address info
  if (profile_in_use_)
    return profile_in_use_->hash (max, ACE_TRY_ENV);

  if (TAO_debug_level > 3)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) hash called on a null profile\n")));

  ACE_THROW_RETURN (CORBA::INTERNAL (
                                     CORBA_SystemException::_tao_minor_code (
                                                                             TAO_DEFAULT_MINOR_CODE,
                                                                             0),
                                     CORBA::COMPLETED_NO),
                    0);
}

// Expensive comparison of objref data, to see if two objrefs
// certainly point at the same object. (It's quite OK for this to
// return FALSE, and yet have the two objrefs really point to the same
// object.)
//
// NOTE that this must NOT go across the network!
// @@ Two object references are the same if any two profiles are the
//    same! This function is only test the profile in use!!!
CORBA::Boolean
TAO_Stub::is_equivalent (CORBA::Object_ptr other_obj)
{
  if (CORBA::is_nil (other_obj) == 1)
    return 0;

  TAO_Profile *other_profile = other_obj->_stubobj ()->profile_in_use_;
  TAO_Profile *this_profile = this->profile_in_use_;

  if (other_profile == 0 || this_profile == 0)
    return 0;

  // Compare the profiles
  return this_profile->is_equivalent (other_profile);
}

// Memory managment

CORBA::ULong
TAO_Stub::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    guard,
                    this->refcount_lock_,
                    0);

  return this->refcount_++;
}

CORBA::ULong
TAO_Stub::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                      mon,
                      this->refcount_lock_,
                      0);

    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

// Note that if the repository ID (typeID) is NULL, it will make
// narrowing rather expensive, though it does ensure that type-safe
// narrowing code gets thoroughly exercised/debugged!  Without a
// typeID, the _narrow will be required to make an expensive remote
// "is_a" call.

// THREADING NOTE: Code below this point is of course thread-safe (at
// least on supported threaded platforms), so the caller of these
// routines need only ensure that the data being passed in is not
// being modified by any other thread.
//
// As an _experiment_ (to estimate the performance cost) remote calls
// are currently deemed "cancel-safe".  That means that they can be
// called by threads when they're in asynchronous cancellation mode.
// The only effective way to do this is to disable async cancellation
// for the duration of the call.  There are numerous rude interactions
// with code generators for C++ ... cancellation handlers just do
// normal stack unwinding like exceptions, but exceptions are purely
// synchronous and sophisticated code generators rely on that to
// generate better code, which in some cases may be very hard to
// unwind.

class TAO_Synchronous_Cancellation_Required
// = TITLE
//     Stick one of these at the beginning of a block that can't
//     support asynchronous cancellation, and which must be
//     cancel-safe.
//
// = EXAMPLE
//     somefunc()
//     {
//       TAO_Synchronous_Cancellation_Required NOT_USED;
//       ...
//     }
{
public:
  // These should probably be in a separate inline file, but they're
  // only used within this one file right now, and we always want them
  // inlined, so here they sit.
  TAO_Synchronous_Cancellation_Required (void)
    : old_type_ (0)
  {
#if !defined (VXWORKS)
    ACE_OS::thr_setcanceltype (THR_CANCEL_DEFERRED, &old_type_);
#endif /* ! VXWORKS */
  }

  ~TAO_Synchronous_Cancellation_Required (void)
  {
#if !defined (VXWORKS)
    int dont_care;
    ACE_OS::thr_setcanceltype(old_type_, &dont_care);
#endif /* ! VXWORKS */
  }
private:
  int old_type_;
};

#if (TAO_HAS_MINIMUM_CORBA == 0)

// DII analogue of the above.

void
TAO_Stub::do_dynamic_call (const char *opname,
                           CORBA::Boolean is_roundtrip,
                           CORBA::NVList_ptr args,
                           CORBA::NamedValue_ptr result,
                           CORBA::Flags,
                           CORBA::ExceptionList_ptr exceptions,
                           int lazy_evaluation,
                           CORBA::Environment &ACE_TRY_ENV)
{
  // @@ TOCCST: why do we keep using this function when ACE+TAO code
  // is clearly not cancel safe anymore?  Furthermore, all the
  // generated code using the Invocation classes is probably not
  // cancel safe!
  TAO_Synchronous_Cancellation_Required NOT_USED;

  // Do a locate_request if necessary/wanted.
  // Suspect that you will be forwarded, so be proactive!
  // strategy for reducing overhead when you think a request will
  // be forwarded.  No standard way now to know.
  if (this->use_locate_request_ && this->first_locate_request_)
    {
      // @@ TOCCST: notice how the locate request is only sent for
      // dynamic and deferred (!!) calls, nothing is done for static
      // calls, the far more common case.  IMHO this should just go
      // away, after all we have _validate_connection to do the same
      // thing!
      TAO_GIOP_Locate_Request_Invocation call (this,
                                               this->orb_core_);

      // Simply let these exceptions propagate up
      // (if any of them occurs.)
      call.start (ACE_TRY_ENV);
      ACE_CHECK;

      call.invoke (ACE_TRY_ENV);
      ACE_CHECK;

      this->first_locate_request_ = 0;
    }

  if (is_roundtrip)
    {
      TAO_GIOP_Twoway_Invocation call (this,
                                       opname,
                                       ACE_OS::strlen (opname),
                                       this->orb_core_);

      // Loop as needed for forwarding; see above.

      for (;;)
        {
          call.start (ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

          call.prepare_header (ACE_static_cast (CORBA::Octet, flag),
                               ACE_TRY_ENV);
          ACE_CHECK;

          this->put_params (call,
                            args,
                            ACE_TRY_ENV);
          ACE_CHECK;

          // Make the call ... blocking for the response.
          int status =
            call.invoke (exceptions, ACE_TRY_ENV);
          ACE_CHECK;

          if (status == TAO_INVOKE_RESTART)
            continue;

          if (status == TAO_INVOKE_EXCEPTION)
            return; // Shouldn't happen

          if (status != TAO_INVOKE_OK)
            ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_MAYBE));

          // The only case left is status == TAO_INVOKE_OK, exit the
          // loop.  We cannot retry because at this point we either
          // got a reply or something with an status of
          // COMPLETED_MAYBE, thus we cannot reissue the request if we
          // are to satisfy the "at most once" semantics.
          break;
        }

      // Now, get all the "return", "out", and "inout" parameters
      // from the response message body ... return parameter is
      // first, the rest are in the order defined in the IDL spec
      // (which is also the order that DII users are required to
      // use).

      if (result != 0)
        {
          result->value ()->_tao_decode (call.inp_stream (),
                                         ACE_TRY_ENV);
          ACE_CHECK;
        }

      args->_tao_incoming_cdr (call.inp_stream (),
                               CORBA::ARG_OUT | CORBA::ARG_INOUT,
                               lazy_evaluation,
                               ACE_TRY_ENV);
    }
  else
    {
      TAO_GIOP_Oneway_Invocation call (this,
                                       opname,
                                       ACE_OS::strlen (opname),
                                       this->orb_core_);

      for (;;)
        {
          call.start (ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::Octet response_flag = ACE_static_cast (CORBA::Octet,
                                                        call.sync_scope ());

          call.prepare_header (response_flag,
                               ACE_TRY_ENV);
          ACE_CHECK;

          this->put_params (call,
                            args,
                            ACE_TRY_ENV);
          ACE_CHECK;

          int status = call.invoke (ACE_TRY_ENV);
          ACE_CHECK;

          if (status == TAO_INVOKE_RESTART)
            continue;

          if (status == TAO_INVOKE_EXCEPTION)
            return; // Shouldn't happen

          if (status != TAO_INVOKE_OK)
            ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_MAYBE));

          break;
        }
    }
}

void
TAO_Stub::do_deferred_call (const CORBA::Request_ptr req,
                            CORBA::Environment &ACE_TRY_ENV)
{
  // @@ TOCCST: why do we keep using this function when ACE+TAO code
  // is clearly not cancel safe anymore?  Furthermore, all the
  // generated code using the Invocation classes is probably not
  // cancel safe!
  TAO_Synchronous_Cancellation_Required NOT_USED;

  // Do a locate_request if necessary/wanted.
  // Suspect that you will be forwarded, so be proactive!
  // strategy for reducing overhead when you think a request will
  // be forwarded.  No standard way now to know.
  if (this->use_locate_request_ && this->first_locate_request_)
    {
      // @@ TOCCST: notice how the locate request is only sent for
      // dynamic and deferred (!!) calls, nothing is done for static
      // calls, the far more common case.  IMHO this should just go
      // away, after all we have _validate_connection to do the same
      // thing!
      TAO_GIOP_Locate_Request_Invocation call (this,
                                               this->orb_core_);

      // Simply let these exceptions propagate up
      // (if any of them occurs.)
      call.start (ACE_TRY_ENV);
      ACE_CHECK;

      call.invoke (ACE_TRY_ENV);
      ACE_CHECK;

      this->first_locate_request_ = 0;
    }

  TAO_GIOP_DII_Deferred_Invocation call (this,
                                         this->orb_core_,
                                         req);

  // Loop as needed for forwarding; see above.

  for (;;)
    {
      call.start (ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

      call.prepare_header (ACE_static_cast (CORBA::Octet, flag),
                           ACE_TRY_ENV);
      ACE_CHECK;

      this->put_params (call,
                        req->arguments (),
                        ACE_TRY_ENV);
      ACE_CHECK;

      // Make the call without blocking.
      CORBA::ULong status = call.invoke (ACE_TRY_ENV);
      ACE_CHECK;

      if (status == TAO_INVOKE_RESTART)
        continue;

      if (status != TAO_INVOKE_OK)
        ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                   CORBA::COMPLETED_MAYBE));

      // The only case left is status == TAO_INVOKE_OK, exit the
      // loop.  We cannot retry because at this point we either
      // got a reply or something with an status of
      // COMPLETED_MAYBE, thus we cannot reissue the request if we
      // are to satisfy the "at most once" semantics.
      break;
    }
}

void
TAO_Stub::put_params (TAO_GIOP_Invocation &call,
                      CORBA::NVList_ptr args,
                      CORBA::Environment &ACE_TRY_ENV)
{
  args->_tao_encode (call.out_stream (),
                     this->orb_core_,
                     CORBA::ARG_IN | CORBA::ARG_INOUT,
                     ACE_TRY_ENV);
}

#endif /* TAO_HAS_MINIMUM_CORBA */

// ****************************************************************

#if (TAO_HAS_CORBA_MESSAGING == 1)
#if (TAO_HAS_RT_CORBA == 1)

void
TAO_Stub::parse_policies (void)
{
  CORBA::PolicyList *policy_list
    = this->base_profiles_.policy_list ();

  CORBA::ULong length = policy_list->length ();
  //  CORBA::ULong index = 0;

  for (unsigned int i = 0; i < length; ++i)
    {
      if (((*policy_list)[i]->policy_type () == RTCORBA::PRIORITY_MODEL_POLICY_TYPE))
        this->exposed_priority_model ((*policy_list)[i].in ());

      else if (((*policy_list)[i]->policy_type () == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE))
        this->exposed_priority_banded_connection ((*policy_list)[i].in ());

      else if (((*policy_list)[i]->policy_type () == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE))
        this->exposed_client_protocol ((*policy_list)[i].in ());
    }

  this->are_policies_parsed_ = 1;
}

TAO_PriorityModelPolicy *
TAO_Stub::exposed_priority_model (void)
{
  if (!this->are_policies_parsed_)
    this->parse_policies ();

  if (!CORBA::is_nil (this->priority_model_policy_))
    this->priority_model_policy_->_add_ref ();

  return this->priority_model_policy_;
}

void TAO_Stub::exposed_priority_model (CORBA::Policy_ptr policy)
{
  if (!CORBA::is_nil (policy))
    {
      RTCORBA::PriorityModelPolicy *pm_policy = 0;
      pm_policy =
        RTCORBA::PriorityModelPolicy::_narrow (policy);

      if (!CORBA::is_nil (pm_policy))
        {
          this->priority_model_policy_ =
            ACE_static_cast (TAO_PriorityModelPolicy *,
                             pm_policy);

        }
    }
}

TAO_PriorityBandedConnectionPolicy *
TAO_Stub::exposed_priority_banded_connection (void)
{
  if (!this->are_policies_parsed_)
    this->parse_policies ();

  if (!CORBA::is_nil (this->priority_banded_connection_policy_))
    this->priority_banded_connection_policy_->_add_ref ();

  return this->priority_banded_connection_policy_;
}

void
TAO_Stub::exposed_priority_banded_connection (CORBA::Policy_ptr policy)
{
  if (!CORBA::is_nil (policy))
    {
      RTCORBA::PriorityBandedConnectionPolicy_ptr pbc_policy =
        RTCORBA::PriorityBandedConnectionPolicy::_narrow (policy);

      if (!CORBA::is_nil (pbc_policy))
        {
          this->priority_banded_connection_policy_ =
            ACE_static_cast (TAO_PriorityBandedConnectionPolicy *,
                             pbc_policy);
        }
    }
}
TAO_ClientProtocolPolicy *
TAO_Stub::exposed_client_protocol (void)
{
  if (!this->are_policies_parsed_)
    this->parse_policies ();

  if (!CORBA::is_nil (this->client_protocol_policy_))
    this->client_protocol_policy_->_add_ref ();

  return this->client_protocol_policy_;
}

void
TAO_Stub::exposed_client_protocol (CORBA::Policy_ptr policy)
{
  if (!CORBA::is_nil (policy))
    {
      RTCORBA::ClientProtocolPolicy_ptr cp_policy =
        RTCORBA::ClientProtocolPolicy::_narrow (policy);

      if (!CORBA::is_nil (cp_policy))
        {
          this->client_protocol_policy_ =
            ACE_static_cast (TAO_ClientProtocolPolicy *,
                             cp_policy);
        }
    }
}

#endif /* TAO_HAS_RT_CORBA == 1 */

CORBA::Policy_ptr
TAO_Stub::get_policy (CORBA::PolicyType type,
                      CORBA::Environment &ACE_TRY_ENV)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

#if (TAO_HAS_RT_CORBA == 1)

  // Validity check.  Make sure requested policy type is appropriate
  // for this scope.
  if (type == RTCORBA::THREADPOOL_POLICY_TYPE
      || type == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE)
    ACE_THROW_RETURN (CORBA::INV_POLICY (),
                      CORBA::Policy::_nil ());

  // If we are dealing with a client exposed policy, check if any
  // value came in the IOR/reconcile IOR value and overrides.
  if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    return this->exposed_priority_model ();

  if (type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    return this->effective_priority_banded_connection (ACE_TRY_ENV);

  if (type == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
    return this->effective_client_protocol (ACE_TRY_ENV);

#endif /* TAO_HAS_RT_CORBA == 1 */

  if (this->policies_ == 0)
    return CORBA::Policy::_nil ();

  // Some policies can only be set locally on the client, while others
  // can only be exported in the IOR by the server, and yet others can
  // be set by both by client and server.  Furthermore, reconciliation
  // between client-set values and the ones exported in the IOR is
  // policy-specific.  Therefore, with the current state of things, it
  // isn't possible to write generic code for <get_policy> that will
  // work for any policy type.
  // Currently, we take specific action
  // for each of the known client-exposed policies (above), and simply
  // look up effective override for any other policy type (below).
  // Later, if there is a need/desire for generic code, it can be
  // done by pushing the smarts into the policies
  // implementations, and will involve modifying PolicyC* and friends,
  // e.g., to add methods for policy specific reconciliation, etc.

  return this->get_client_policy (type,
                                  ACE_TRY_ENV);
}

CORBA::Policy_ptr
TAO_Stub::get_client_policy (CORBA::PolicyType type,
                             CORBA::Environment &ACE_TRY_ENV)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

#if (TAO_HAS_RT_CORBA == 1)

  // Validity check.  Make sure requested policy type is appropriate
  // for this scope.
  if (type == RTCORBA::THREADPOOL_POLICY_TYPE
      || type == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE
      || type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    ACE_THROW_RETURN (CORBA::INV_POLICY (),
                      CORBA::Policy::_nil ());

#endif /* TAO_HAS_RT_CORBA == 1 */

  CORBA::Policy_var result;
  if (this->policies_ != 0)
    {
      result = this->policies_->get_policy (type,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  if (CORBA::is_nil (result.in ()))
    {
      TAO_Policy_Current &policy_current = this->orb_core_->policy_current ();
      result = policy_current.get_policy (type,
                                          ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  if (CORBA::is_nil (result.in ()))
    {
      // @@ Must lock, but is is harder to implement than just modifying
      //    this call: the ORB does take a lock to modify the policy
      //    manager
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        {
          result = policy_manager->get_policy (type,
                                               ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::Policy::_nil ());
        }
    }

  if (CORBA::is_nil (result.in ()))
    {
      result = this->orb_core_->get_default_policy (type,
                                                    ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return result._retn ();
}

TAO_Stub *
TAO_Stub::set_policy_overrides (const CORBA::PolicyList & policies,
                                CORBA::SetOverrideType set_add,
                                CORBA::Environment &ACE_TRY_ENV)
{
#if (TAO_HAS_RT_CORBA == 1)

  // Validity check.  Make sure requested policies are allowed to be
  // set at this scope.
  for (CORBA::ULong i = 0; i < policies.length ();  ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      if (CORBA::is_nil (policy))
        continue;

      CORBA::ULong slot = policy->policy_type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (slot == RTCORBA::THREADPOOL_POLICY_TYPE
          || slot == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE
          || slot == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
        ACE_THROW_RETURN (CORBA::NO_PERMISSION (),
                          0);
    }

  // We are not required to check for consistency of <policies> with
  // overrides at other levels or with policies exported in the IOR.

#endif /* TAO_HAS_RT_CORBA == 1 */

  // Notice the use of an explicit constructor....
  auto_ptr<TAO_Policy_Manager_Impl> policy_manager (new TAO_Policy_Manager_Impl);

  if (set_add == CORBA::SET_OVERRIDE)
    {
      policy_manager->set_policy_overrides (policies,
                                            set_add,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }
  else if (this->policies_ == 0)
    {
      policy_manager->set_policy_overrides (policies,
                                            CORBA::SET_OVERRIDE,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }
  else
    {
      policy_manager->copy_from (this->policies_,
                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      policy_manager->set_policy_overrides (policies,
                                            set_add,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  TAO_Stub* stub;
  ACE_NEW_RETURN (stub, TAO_Stub (CORBA::string_dup (this->type_id.in ()),
                                  this->base_profiles_,
                                  this->orb_core_),
                  0);
  stub->policies_ = policy_manager.release ();

  // Copy the servant ORB if it is present.
  stub->servant_orb (this->servant_orb_var ().in ());

  return stub;
}

CORBA::PolicyList *
TAO_Stub::get_policy_overrides (const CORBA::PolicyTypeSeq &types,
                                CORBA::Environment &ACE_TRY_ENV)
{
  if (this->policies_ == 0)
    return 0;

  return this->policies_->get_policy_overrides (types,
                                                ACE_TRY_ENV);
}

CORBA::Boolean
TAO_Stub::validate_connection (CORBA::PolicyList_out inconsistent_policies,
                               CORBA::Environment &ACE_TRY_ENV)
{
  inconsistent_policies = 0;
  ACE_UNUSED_ARG (ACE_TRY_ENV);

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  // Check if we care about Client Priority policy, and store the
  // result in the variable called <set>.
  int set = 1;
  TAO::ClientPriorityPolicy *policy =
    this->client_priority ();
  if (policy == 0)
    set = 0;
  else
    // Policy is set.
    {
      TAO::PrioritySpecification priority_spec =
        policy->priority_specification (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      TAO::PrioritySelectionMode mode = priority_spec.mode;

      // Don't care about priority.
      if (mode == TAO::USE_NO_PRIORITY)
        set = 0;
    }

  // Use Locate Request to establish connection/make sure the object
  // is there ...
  TAO_GIOP_Locate_Request_Invocation locate_request (this,
                                                     this->orb_core_);

  //@@ Currently, if we select profiles based on priorities (i.e.,
  //   ClientPriorityPolicy is set), and we get a FORWARD reply to our
  //   location request, we don't go to the new location - just return
  //   0. This is because we don't yet have full support in
  //   MProfiles & friends for profiles used based on priorities.
  //   Once the support is there, we should follow a forwarded object
  //   to track it down, just like in the case where
  //   ClientPriorityPolicy is not set.  At that point, we can remove
  //   a lot of 'special case' code from this function, along with
  //   this comment ;-) (marina).
  for (;;)
    {
      locate_request.start (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      int status = locate_request.invoke (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // We'll get this only if the object was, in fact, forwarded.
      if (status == TAO_INVOKE_RESTART)
        if (set)
          return 0;
        else
          continue;

      if (status != TAO_INVOKE_OK)
        {
          ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_YES),
                            0);

        }
      break;
    }

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

  return 1;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

TAO_Sync_Strategy &
TAO_Stub::sync_strategy (void)
{

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  int has_synchronization;
  int scope;

  this->orb_core_->call_sync_scope_hook (this,
                                         has_synchronization,
                                         scope);

  if (has_synchronization == 1)
    return this->orb_core_->get_sync_strategy (this,
                                               scope);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  return this->orb_core_->transport_sync_strategy ();
}

CORBA::Policy *
TAO_Stub::relative_roundtrip_timeout (void)
{
  CORBA::Policy *result = 0;

  this->policies_ = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
      result = this->policies_->relative_roundtrip_timeout ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.relative_roundtrip_timeout ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->relative_roundtrip_timeout ();
    }

  if (result == 0)
    result = this->orb_core_->default_relative_roundtrip_timeout ();

  return result;
}

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

TAO_Client_Priority_Policy *
TAO_Stub::client_priority (void)
{
  TAO_Client_Priority_Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->client_priority ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.client_priority ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->client_priority ();
    }

  if (result == 0)
    result = this->orb_core_->default_client_priority ();

  return result;
}

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

CORBA::Policy *
TAO_Stub::sync_scope (void)
{
  CORBA::Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->sync_scope ();
  
  this->orb_core_->stubless_sync_scope (result);
  
  return result;
}

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

TAO_Buffering_Constraint_Policy *
TAO_Stub::buffering_constraint (void)
{
  TAO_Buffering_Constraint_Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->buffering_constraint ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.buffering_constraint ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->buffering_constraint ();
    }

  if (result == 0)
    result = this->orb_core_->default_buffering_constraint ();

  return result;
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

TAO_PrivateConnectionPolicy *
TAO_Stub::private_connection (void)
{
  TAO_PrivateConnectionPolicy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->private_connection ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.private_connection ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->private_connection ();
    }

  if (result == 0)
    result = this->orb_core_->default_private_connection ();

  return result;
}

TAO_ClientProtocolPolicy *
TAO_Stub::client_protocol (void)
{
  TAO_ClientProtocolPolicy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->client_protocol ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.client_protocol ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->client_protocol ();
    }

  // No default is used for client priority policy (default creates
  // conflict in case the policy is also set for the object on the
  // server side).

  return result;
}

TAO_PriorityBandedConnectionPolicy *
TAO_Stub::priority_banded_connection (void)
{
  TAO_PriorityBandedConnectionPolicy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->priority_banded_connection ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.priority_banded_connection ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->priority_banded_connection ();
    }

  if (result == 0)
    result = this->orb_core_->default_priority_banded_connection ();

  return result;
}

TAO_PriorityBandedConnectionPolicy *
TAO_Stub::effective_priority_banded_connection (CORBA::Environment &ACE_TRY_ENV)
{
  // Get effective override.
  TAO_PriorityBandedConnectionPolicy *override =
    this->priority_banded_connection ();

  // Get the value from the ior.
  TAO_PriorityBandedConnectionPolicy *exposed =
    this->exposed_priority_banded_connection ();

  // Reconcile client-exposed and locally set values.
  if (exposed == 0)
    return override;

  if (override == 0)
    return exposed;

  // Both override and exposed have been set.
  // See if either of them has empty priority bands.
  CORBA::Object_var auto_release_exp = exposed;
  CORBA::Object_var auto_release_ov = override;

  if (exposed->priority_bands_rep ().length () == 0)
    {
      auto_release_ov._retn ();
      return override;
    }

  if (override->priority_bands_rep ().length () == 0)
    {
      auto_release_exp._retn ();
      return exposed;
    }

  // Both override and exposed have been set and neither has empty
  // priority bands.  This is illegal (ptc/99-05-03, sec. 4.12.1).
  ACE_THROW_RETURN (CORBA::INV_POLICY (),
                    0);
}

TAO_ClientProtocolPolicy *
TAO_Stub::effective_client_protocol (CORBA::Environment &ACE_TRY_ENV)
{
  // Get effective override.
  TAO_ClientProtocolPolicy *override =
    this->client_protocol ();

  // Get the value from the ior.
  TAO_ClientProtocolPolicy *exposed =
    this->exposed_client_protocol ();

  // Reconcile client-exposed and locally set values.
  if (exposed == 0)
    return override;

  if (override == 0)
    return exposed;

  // Both override and exposed have been set.
  // See if either of them has empty priority bands.
  CORBA::Object_var auto_release_exp = exposed;
  CORBA::Object_var auto_release_ov = override;

  if (exposed->protocols_rep ().length () == 0)
    {
      auto_release_ov._retn ();
      return override;
    }

  if (override->protocols_rep ().length () == 0)
    {
      auto_release_exp._retn ();
      return exposed;
    }

  // Both override and exposed have been set and neither has empty
  // protocols.  This is illegal (ptc/99-05-03, sec. 4.15.4).
  ACE_THROW_RETURN (CORBA::INV_POLICY (),
                    0);
}

#endif /* TAO_HAS_RT_CORBA == 1 */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if (TAO_HAS_CORBA_MESSAGING == 1)

template class auto_ptr<TAO_Policy_Manager_Impl>;
template class ACE_Auto_Basic_Ptr<TAO_Policy_Manager_Impl>;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if (TAO_HAS_CORBA_MESSAGING == 1)

#pragma instantiate auto_ptr<TAO_Policy_Manager_Impl>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Policy_Manager_Impl>

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
