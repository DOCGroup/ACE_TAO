// $Id$

#include "tao/Invocation_Endpoint_Selectors.h"

#if !defined (__ACE_INLINE__)
#include "tao/Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#include "tao/Invocation.h"
#include "tao/Client_Priority_Policy.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Endpoint.h"
#include "tao/RT_Invocation_Endpoint_Selectors.h"

ACE_RCSID(tao, Invocation_Endpoint_Selectors, "$Id$")


#if (TAO_HAS_RT_CORBA == 1)

TAO_Endpoint_Selection_State::~TAO_Endpoint_Selection_State (void)
{
  CORBA::release (this->priority_model_policy_);
  CORBA::release (this->client_protocol_policy_);
  CORBA::release (this->bands_policy_);
  CORBA::release (this->private_connection_);
}

#endif /* TAO_HAS_RT_CORBA == 1 */

// ****************************************************************

TAO_Invocation_Endpoint_Selector::~TAO_Invocation_Endpoint_Selector (void)
{
}

// ****************************************************************

TAO_Default_Endpoint_Selector::~TAO_Default_Endpoint_Selector (void)
{
}

void
TAO_Default_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation
                                                *invocation,
                                                CORBA::Environment &ACE_TRY_ENV)
{
  invocation->profile_ = invocation->stub_->profile_in_use ();
  invocation->endpoint_ = invocation->profile_->endpoint ();

  if (invocation->endpoint_ == 0)
    {
      // Unknown protocol - move onto the next profile.
      this->next (invocation, ACE_TRY_ENV);
      ACE_CHECK;
      this->select_endpoint (invocation, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_Default_Endpoint_Selector::next (TAO_GIOP_Invocation
                                     *invocation,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  if (invocation->stub_->next_profile_retry () == 0)
    ACE_THROW (CORBA::TRANSIENT (
                                 CORBA_SystemException::_tao_minor_code (
                                      TAO_INVOCATION_CONNECT_MINOR_CODE,
                                      errno),
                                 CORBA::COMPLETED_NO));
}

void
TAO_Default_Endpoint_Selector::forward (TAO_GIOP_Invocation
                                        *invocation,
                                        const TAO_MProfile &mprofile,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  invocation->stub_->add_forward_profiles (mprofile);
  // This has to be and is thread safe.
  // TAO_Stub::add_forward_profiles() already makes a copy of the
  // MProfile, so do not make a copy here.


  // We may not need to do this since TAO_GIOP_Invocations
  // get created on a per-call basis. For now we'll play it safe.
  if (invocation->stub_->next_profile () == 0)
    ACE_THROW (CORBA::TRANSIENT (
                                 CORBA_SystemException::_tao_minor_code (
                                       TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
                                       errno),
                                 CORBA::COMPLETED_NO));
}

void
TAO_Default_Endpoint_Selector::success (TAO_GIOP_Invocation *invocation)
{
  invocation->stub_->set_valid_profile ();
}

void
TAO_Default_Endpoint_Selector::close_connection (TAO_GIOP_Invocation *invocation)
{
  // Get rid of any forwarding profiles and reset
  // the profile list to point to the first profile!
  // FRED For now we will not deal with recursive forwards!
  invocation->stub_->reset_profiles ();
}
