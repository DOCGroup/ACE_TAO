// -*- C++ -*-
//
// $Id$

#include "IORInfo.h"
#include "PolicyC.h"
#include "IOPC.h"
#include "CONV_FRAMEC.h"
#include "MProfile.h"
#include "GIOP_Message_State.h"

ACE_RCSID (tao, IORInfo, "$Id$")


TAO_IORInfo::TAO_IORInfo (TAO_ORB_Core *orb_core,
			  TAO_MProfile &mp,
			  CORBA::PolicyList *policy_list)
  : orb_core_ (orb_core),
    mp_ (mp),
    policy_list_ (policy_list)
{
}

TAO_IORInfo::~TAO_IORInfo (void)
{
}

CORBA::Policy_ptr
TAO_IORInfo::get_effective_policy (CORBA::PolicyType type,
				   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Check the policy list supplied by the POA.
  CORBA::ULong policy_count = this->policy_list_->length ();

  for (CORBA::ULong i = 0; i < policy_count; ++i)
    {
      CORBA::PolicyType pt =
	(*(this->policy_list_))[i]->policy_type (
          TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      if (pt == type)
	return CORBA::Policy::_duplicate ((*(this->policy_list_))[i]);
    }

  // TODO: Now check the global ORB policies.
  // ........

  ACE_THROW_RETURN (CORBA::INV_POLICY (
                      CORBA_SystemException::_tao_minor_code (
			TAO_DEFAULT_MINOR_CODE,
                        EINVAL),
                      CORBA::COMPLETED_NO),
		    CORBA::Policy::_nil ());
}

void
TAO_IORInfo::add_ior_component (const IOP::TaggedComponent &component,
				CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Add the given tagged component to all profiles.

  CORBA::ULong profile_count = this->mp_.profile_count ();
  for (CORBA::ULong i = 0; i != profile_count; ++i)
    {
      TAO_Profile *profile = this->mp_.get_profile (i);

      profile->add_tagged_component (component, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_IORInfo::add_ior_component_to_profile (
    const IOP::TaggedComponent &component,
    IOP::ProfileId profile_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Add the given tagged component to all profiles matching the given
  // ProfileId.

  CORBA::ULong profile_count = this->mp_.profile_count ();
  for (CORBA::ULong i = 0; i != profile_count; ++i)
    {
      TAO_Profile *profile = this->mp_.get_profile (i);

      if (profile->tag () == profile_id)
	{
	  profile->add_tagged_component (component, ACE_TRY_ENV);
          ACE_CHECK;
	}
    }
}
