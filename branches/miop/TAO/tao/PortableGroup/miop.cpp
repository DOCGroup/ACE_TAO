// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     miop.cpp
//
// = DESCRIPTION
//
// = AUTHOR
//     Frank Hunleth <fhunleth@cs.wustl.edu>
//
// ============================================================================

#include "miop.h"

#include "UIPMC_Profile.h"
#include "tao/mprofile.h"
#include "tao/stub.h"

ACE_RCSID(tao, PortableGroup, "$Id$")

TAO_MOGF_impl::TAO_MOGF_impl (TAO_ORB_Core *orb_core,
                              const char *domain_id) :
  orb_core_ (orb_core),
  domain_id_ (domain_id),
  group_id_ (1)
{
}

TAO_MOGF_impl::~TAO_MOGF_impl (void)
{
}

PortableGroup::ObjectGroupId
TAO_MOGF_impl::next_group_id (void)
{
  // Don't worry about reusing an active group from wrapping,
  // since group_id_ is 64 bits.
  return this->group_id_++;
}

CORBA::Object_ptr
TAO_MOGF_impl::create_group (
          const char * type_id,
          const MIOP::MulticastObjectGroupFactory::ipaddr class_d_address,
          CORBA::UShort port,
          CORBA::Environment &ACE_TRY_ENV)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ))
{
  TAO_Stub *stub = 0;

  // Create a profile container and then put in the UIPMC profile.
  TAO_MProfile mp (1);

  TAO_UIPMC_Profile *uipmc_profile;

  ACE_NEW_THROW_EX (uipmc_profile,
                    TAO_UIPMC_Profile(class_d_address,
                                      port,
                                      this->orb_core_),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_MAYBE));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Set the group component to the UIPMC profile.
  uipmc_profile->set_group_info (this->domain_id_.c_str (),
                                 this->next_group_id (),
                                 0);


  // Relinquish ownership of the UIPMC profile to mp.
  mp.give_profile (uipmc_profile);


  //  Add the Polices contained in "policy_list" to each profile
  //  so that those policies will be exposed to the client in the IOR.
  //  In particular each CORBA::Policy has to be converted in to
  //  Messaging::PolicyValue, and then all the Messaging::PolicyValue
  //  should be embedded inside a Messaging::PolicyValueSeq which became
  //  in turns the "body" of the IOP::TaggedComponent. This conversion
  //  is a responsability of the CORBA::Profile class.
  //  (See orbos\98-05-05.pdf Section 5.4)

/* @@ Frank: Not sure what to do with this part yet...
  if (policy_list->length () != 0)
    {
      TAO_Profile * profile;

      for (CORBA::ULong i = 0; i < mp.profile_count (); ++i)
        {
          // Get the ith profile
          profile = mp.get_profile (i);
          profile->policies (policy_list, ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
    }

  // Iterate over the registered IOR interceptors so that they may be
  // given the opportunity to add tagged components to the profiles
  // for this servant.
  this->establish_components (mp, policy_list, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
*/

  // Done creating profiles.  Initialize a TAO_Stub object with them.
  ACE_NEW_THROW_EX (stub,
                    TAO_Stub (type_id, mp, this->orb_core_),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_MAYBE));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

/* @@ Frank: Not sure about this:  stub->base_profiles ().policy_list (policy_list); */

  TAO_Stub_Auto_Ptr safe_data (stub);

  CORBA::Object_ptr tmp;

  ACE_NEW_THROW_EX (tmp,
                    CORBA_Object (safe_data.get (),
                                  0),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  safe_data.get ()->servant_orb (this->orb_core_->orb ());

  // Transfer ownership to the Object.
  (void) safe_data.release ();

  return tmp;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
