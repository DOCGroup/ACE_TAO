// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     IORManipulation.cpp
//
// = DESCRIPTION
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#include "tao/IORManipulation.h"
#include "tao/MProfile.h"
#include "tao/Stub.h"

#include "ace/Auto_Ptr.h"

ACE_RCSID(tao, IORManipulation, "$Id$")

TAO_IOR_Manipulation_impl::TAO_IOR_Manipulation_impl (void)
{
}

TAO_IOR_Manipulation_impl::~TAO_IOR_Manipulation_impl (void)
{
}

CORBA::Object_ptr
TAO_IOR_Manipulation_impl::merge_iors (
    const TAO_IOP::TAO_IOR_Manipulation::IORList & iors,
    CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList,
                       TAO_IOP::TAO_IOR_Manipulation::Duplicate,
                       TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR))
{
  // we need to create a new CORBA::Object which has the union of the
  // two profile lists.  However, if any profiles are duplicates (i.e. in
  // bott lisis) then an exception is raised.

  // Deterinine how many profiles we have
  // Get an estimate of the size - pfile count could change since we
  // neither lock nor get a copy in this loop.
  CORBA::ULong i, count=0;
  for (i = 0; i < iors.length (); i++)
    {
      count += iors[i]->_stubobj ()->base_profiles ().profile_count ();
    }

  // make sure we have some profiles
  if (count == 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList (),
                      CORBA::Object::_nil ());

  // initialize with estimated pfile count.
  TAO_MProfile Merged_Profiles (count);

  // get the profile lists, start by initialize the composite reference
  // by using the first Object.  Then for each subsequent Object verify
  // they are the same type and they do not have duplicate profiles.
  auto_ptr<TAO_MProfile> tmp_pfiles (iors[0]->_stubobj ()->make_profiles ());
  if (Merged_Profiles.add_profiles (tmp_pfiles.get ())< 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                      CORBA::Object::_nil ());
  CORBA::String_var id =
    CORBA::string_dup (iors[0]->_stubobj ()->type_id.in ());

  for (i = 1; i < iors.length () ; i++)
    {
      // this gets a copy of the MProfile, hense the auto_ptr;

      ACE_AUTO_PTR_RESET (tmp_pfiles,
                          iors[i]->_stubobj ()->make_profiles (),
                          TAO_MProfile);

      // check to see if any of the profile in tmp_pfiles are already
      // in Merged_Profiles.  If so raise exception.
      if (Merged_Profiles.is_equivalent (tmp_pfiles.get ()))
        ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Duplicate (),
                          CORBA::Object::_nil ());

      // If the object type_id's differ then raise an exception.
      if (id.in () && iors[i]->_stubobj ()->type_id.in () &&
          ACE_OS::strcmp (id.in (), iors[i]->_stubobj ()->type_id.in ()))
        ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                          CORBA::Object::_nil ());

      // append profiles
      if (Merged_Profiles.add_profiles (tmp_pfiles.get ()) < 0)
        ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                          CORBA::Object::_nil ());

    }

  // MS C++ knows nothing about reset!
  // tmp_pfiles.reset (0); // get rid of last MProfile

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();

  TAO_Stub *stub = 0;
  ACE_NEW_THROW_EX (stub,
                    TAO_Stub (id._retn (),  // give the id string to stub
                              Merged_Profiles,
                              orb_core),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Make the stub memory allocation exception safe for the duration
  // of this method.
  TAO_Stub_Auto_Ptr safe_stub (stub);

  // Create the CORBA level proxy
  CORBA::Object_ptr temp_obj = CORBA::Object::_nil ();
  ACE_NEW_THROW_EX (temp_obj,
                    CORBA::Object (safe_stub.get ()),
                    CORBA::NO_MEMORY ());

  CORBA::Object_var new_obj = temp_obj;

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Clean up in case of errors.
  if (CORBA::is_nil (new_obj.in ()))
    {
      ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                        CORBA::Object::_nil ());
    }

  // Release ownership of the pointers protected by the auto_ptrs since they
  // no longer need to be protected by this point.
  stub = safe_stub.release ();

  return new_obj._retn ();
}

CORBA::Object_ptr
TAO_IOR_Manipulation_impl::add_profiles (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2,
    CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList,
        TAO_IOP::TAO_IOR_Manipulation::Duplicate,
        TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR
      ))
{

  // Get an estimate of the number of profiles
  CORBA::Object_ptr buffer [2];
  buffer [0] = ior1;
  buffer [1] = ior2;
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (2, 2, buffer, 0);
  return this->merge_iors (iors, ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_IOR_Manipulation_impl::remove_profiles (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2,
    CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR,
        TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList,
        TAO_IOP::TAO_IOR_Manipulation::NotFound
      ))
{
  // First verify they are the same type!
  CORBA::String_var id =
    CORBA::string_dup (ior1->_stubobj ()->type_id.in ());
  if (id.in () && ior2->_stubobj ()->type_id.in () &&
      ACE_OS::strcmp (id.in (), ior2->_stubobj ()->type_id.in ()))
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                      CORBA::Object::_nil ());

  // Since we are removing from ior1 ...
  CORBA::ULong count = ior1->_stubobj ()->base_profiles ().profile_count ();

  // make sure we have some profiles
  if (count == 0 ||
      ior2->_stubobj ()->base_profiles ().profile_count () == 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList (),
                      CORBA::Object::_nil ());

  // initialize with estimated pfile count.
  TAO_MProfile Diff_Profiles (count);

  auto_ptr<TAO_MProfile> tmp_pfiles (ior1->_stubobj ()->make_profiles ());
  if (Diff_Profiles.add_profiles (tmp_pfiles.get ()) < 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                      CORBA::Object::_nil ());

  ACE_AUTO_PTR_RESET (tmp_pfiles,
                      ior2->_stubobj ()->make_profiles (),
                      TAO_MProfile);

  if (Diff_Profiles.remove_profiles (tmp_pfiles.get ()) < 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::NotFound (),
                      CORBA::Object::_nil ());

  // MS C++ knows nothing about reset!
  // tmp_pfiles.reset (0); // get rid of last MProfile

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();

  TAO_Stub *stub = 0;
  ACE_NEW_THROW_EX (stub,
                    TAO_Stub (id._retn (), // give id string to stub
                              Diff_Profiles,
                              orb_core),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Make the stub memory allocation exception safe for the duration
  // of this method.
  TAO_Stub_Auto_Ptr safe_stub (stub);

  // Create the CORBA level proxy
  CORBA::Object_ptr temp_obj = CORBA::Object::_nil ();
  ACE_NEW_THROW_EX (temp_obj,
                    CORBA::Object (safe_stub.get ()),
                    CORBA::NO_MEMORY ());

  CORBA::Object_var new_obj = temp_obj;

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Clean up in case of errors.
  if (CORBA::is_nil (new_obj.in ()))
    {
      ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                        CORBA::Object::_nil ());
    }

  // Exception safety is no longer an issue by this point so release
  // the TAO_Stub from the TAO_Stub_Auto_Ptr.
  stub = safe_stub.release ();

  return new_obj._retn ();
}

CORBA::ULong
TAO_IOR_Manipulation_impl::is_in_ior (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2,
    CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::TAO_IOR_Manipulation::NotFound
      ))
{
  CORBA::ULong count=0;
  TAO_Profile *pfile1, *pfile2;
  auto_ptr<TAO_MProfile> tmp_pfiles1 (ior1->_stubobj ()->make_profiles ());
  auto_ptr<TAO_MProfile> tmp_pfiles2 (ior2->_stubobj ()->make_profiles ());

  tmp_pfiles1->rewind ();
  while ((pfile1 = tmp_pfiles1->get_next ()) > 0)
    {
      tmp_pfiles2->rewind ();
      while ((pfile2 = tmp_pfiles2->get_next ()) > 0)
        {
          if (pfile1->is_equivalent (pfile2))
            count++;
        }
    }

  if (count == 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::NotFound (),
                      0);

  return count;
}

CORBA::ULong
TAO_IOR_Manipulation_impl::get_profile_count (
    CORBA::Object_ptr ior,
    CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList
      ))
{
  CORBA::ULong count;
  count = ior->_stubobj ()->base_profiles ().profile_count ();

  if (count == 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList (),
                      0);

  return count;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Object_Manager<CORBA_Object,CORBA_Object_var>;
template class TAO_Unbounded_Object_Sequence<CORBA_Object,CORBA_Object_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Object_Manager<CORBA_Object,CORBA_Object_var>
#pragma instantiate TAO_Unbounded_Object_Sequence<CORBA_Object,CORBA_Object_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
