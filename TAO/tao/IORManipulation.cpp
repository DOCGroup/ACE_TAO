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
#include "tao/corba.h"
#include "ace/Auto_Ptr.h"
#include "tao/MProfile.h"

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
      count += iors[i]->_stubobj ()->get_base_profiles ().profile_count ();
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
  auto_ptr<TAO_MProfile> tmp_pfiles (iors[0]->_stubobj ()->get_profiles ());
  if (Merged_Profiles.add_profiles (tmp_pfiles.get ())< 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                      CORBA::Object::_nil ());
  CORBA::String_var type_id = iors[0]->_stubobj ()->type_id;

  for (i = 1; i < iors.length () ; i++)
    {
      // this gets a copy of the MProfile, hense ther auto_ptr;
      tmp_pfiles.reset (iors[i]->_stubobj ()->get_profiles ());

      // check to see if any of the profile in tmp_pfiles are already
      // in Merged_Profiles.  If so raise exception.
      if (Merged_Profiles.is_equivalent (tmp_pfiles.get ()))
        ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Duplicate (),
                          CORBA::Object::_nil ());

      // If the object type_is's differ then raise an exception.
      if (type_id && iors[i]->_stubobj ()->type_id &&
          ACE_OS::strcmp (type_id, iors[i]->_stubobj ()->type_id))
        ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                          CORBA::Object::_nil ());

      // append profiles
      if (Merged_Profiles.add_profiles (tmp_pfiles.get ()) < 0)
        ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                          CORBA::Object::_nil ());

    }

  tmp_pfiles.reset (0); // get rid of last MProfile

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  // @@ need some sort of auto_ptr here
  TAO_Stub *stub;
  ACE_NEW_THROW_EX (stub,
                    TAO_Stub (type_id,
                              Merged_Profiles,
                              orb_core),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Create the CORBA level proxy
  CORBA_Object *new_obj;
  ACE_NEW_THROW_EX (new_obj,
                    CORBA_Object (stub),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Clean up in case of errors.
  if (CORBA::is_nil (new_obj))
    {
      stub->_decr_refcnt ();
      ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                        CORBA::Object::_nil ());
    }

  return new_obj;
}

CORBA::Object_ptr
TAO_IOR_Manipulation_impl::add_profiles (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2,
    CORBA::Environment &ACE_TRY_ENV)
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
{
  // First verify they are the same type!
  CORBA::String_var type_id = ior1->_stubobj ()->type_id;
  if (type_id && ior2->_stubobj ()->type_id &&
      ACE_OS::strcmp (type_id, ior2->_stubobj ()->type_id))
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                      CORBA::Object::_nil ());

  // Since we are removing from ior1 ...
  CORBA::ULong count = ior1->_stubobj ()->get_base_profiles ().profile_count ();

  // make sure we have some profiles
  if (count == 0 ||
      ior2->_stubobj ()->get_base_profiles ().profile_count () == 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList (),
                      CORBA::Object::_nil ());

  // initialize with estimated pfile count.
  TAO_MProfile Diff_Profiles (count);

  auto_ptr<TAO_MProfile> tmp_pfiles (ior1->_stubobj ()->get_profiles ());
  if (Diff_Profiles.add_profiles (tmp_pfiles.get ()) < 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                      CORBA::Object::_nil ());

  tmp_pfiles.reset (ior2->_stubobj ()->get_profiles ());
  if (Diff_Profiles.remove_profiles (tmp_pfiles.get ()) < 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::NotFound (),
                      CORBA::Object::_nil ());


  tmp_pfiles.reset (0); // get rid of last MProfile

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  TAO_Stub *stub;
  ACE_NEW_THROW_EX (stub,
                    TAO_Stub (type_id,
                              Diff_Profiles,
                              orb_core),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Create the CORBA level proxy
  CORBA_Object *new_obj;
  ACE_NEW_THROW_EX (new_obj,
                    CORBA_Object (stub),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (CORBA::Object::_nil ());


  // Clean up in case of errors.
  if (CORBA::is_nil (new_obj))
    {
      stub->_decr_refcnt ();
      ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR (),
                        CORBA::Object::_nil ());
    }

  return new_obj;
}

CORBA::ULong
TAO_IOR_Manipulation_impl::is_in_ior (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2,
    CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong count=0;
  TAO_Profile *pfile1, *pfile2;
  auto_ptr<TAO_MProfile> tmp_pfiles1 (ior1->_stubobj ()->get_profiles ());
  auto_ptr<TAO_MProfile> tmp_pfiles2 (ior2->_stubobj ()->get_profiles ());

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
{
  CORBA::ULong count;
  count = ior->_stubobj ()->get_base_profiles ().profile_count ();

  if (count == 0)
    ACE_THROW_RETURN (TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList (),
                      0);

  return count;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr<TAO_MProfile>;
template class ACE_Auto_Basic_Ptr<TAO_MProfile>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr<TAO_MProfile>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_MProfile>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
