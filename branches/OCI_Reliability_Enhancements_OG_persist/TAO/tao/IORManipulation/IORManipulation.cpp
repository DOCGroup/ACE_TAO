// -*- C++ -*-
// $Id$

#include "tao/IORManipulation/IORManipulation.h"

#include "tao/MProfile.h"
#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IOR_Manipulation_impl::TAO_IOR_Manipulation_impl (void)
{
}

TAO_IOR_Manipulation_impl::~TAO_IOR_Manipulation_impl (void)
{
}

CORBA::Object_ptr
TAO_IOR_Manipulation_impl::merge_iors (
    const TAO_IOP::TAO_IOR_Manipulation::IORList & iors)
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
    throw TAO_IOP::EmptyProfileList ();

  // initialize with estimated pfile count.
  TAO_MProfile Merged_Profiles (count);

  // get the profile lists, start by initialize the composite reference
  // by using the first Object.  Then for each subsequent Object verify
  // they are the same type and they do not have duplicate profiles.
  auto_ptr<TAO_MProfile> tmp_pfiles (iors[0]->_stubobj ()->make_profiles ());
  if (Merged_Profiles.add_profiles (tmp_pfiles.get ())< 0)
    throw TAO_IOP::Invalid_IOR ();
  CORBA::String_var id =
    CORBA::string_dup (iors[0]->_stubobj ()->type_id.in ());

  for (i = 1; i < iors.length () ; i++)
    {
      // this gets a copy of the MProfile, hense the auto_ptr;

      ACE_auto_ptr_reset (tmp_pfiles,
                          iors[i]->_stubobj ()->make_profiles ());

      // check to see if any of the profile in tmp_pfiles are already
      // in Merged_Profiles.  If so raise exception.
      if (Merged_Profiles.is_equivalent (tmp_pfiles.get ()))
        throw TAO_IOP::Duplicate ();

      // If the object type_id's differ then raise an exception.
      if (id.in () && iors[i]->_stubobj ()->type_id.in () &&
          ACE_OS::strcmp (id.in (), iors[i]->_stubobj ()->type_id.in ()))
        throw TAO_IOP::Invalid_IOR ();

      // append profiles
      if (Merged_Profiles.add_profiles (tmp_pfiles.get ()) < 0)
        throw TAO_IOP::Invalid_IOR ();

    }

  // MS C++ knows nothing about reset!
  // tmp_pfiles.reset (0); // get rid of last MProfile

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();

  TAO_Stub *stub = orb_core->create_stub (id.in (), // give the id string
                                          Merged_Profiles);

  // Make the stub memory allocation exception safe for the duration
  // of this method.
  TAO_Stub_Auto_Ptr safe_stub (stub);

  // Create the CORBA level proxy
  CORBA::Object_ptr temp_obj = CORBA::Object::_nil ();
  ACE_NEW_THROW_EX (temp_obj,
                    CORBA::Object (safe_stub.get ()),
                    CORBA::NO_MEMORY ());

  CORBA::Object_var new_obj = temp_obj;


  // Clean up in case of errors.
  if (CORBA::is_nil (new_obj.in ()))
    {
      throw TAO_IOP::Invalid_IOR ();
    }

  // Release ownership of the pointers protected by the auto_ptrs since they
  // no longer need to be protected by this point.
  stub = safe_stub.release ();

  return new_obj._retn ();
}

CORBA::Object_ptr
TAO_IOR_Manipulation_impl::add_profiles (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2)
{

  // Get an estimate of the number of profiles
  CORBA::Object_ptr buffer [2];
  buffer [0] = ior1;
  buffer [1] = ior2;
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (2, 2, buffer, 0);
  return this->merge_iors (iors);
}

CORBA::Object_ptr
TAO_IOR_Manipulation_impl::remove_profiles (
    CORBA::Object_ptr group,
    CORBA::Object_ptr ior2
    )
{
  // First verify they are the same type!
  CORBA::String_var id =
    CORBA::string_dup (group->_stubobj ()->type_id.in ());
  if (id.in () && ior2->_stubobj ()->type_id.in () &&
      ACE_OS::strcmp (id.in (), ior2->_stubobj ()->type_id.in ()))
    throw TAO_IOP::Invalid_IOR ();

  // Since we are removing from group ...
  CORBA::ULong count = group->_stubobj ()->base_profiles ().profile_count ();

  // make sure we have some profiles
  if (count == 0 ||
      ior2->_stubobj ()->base_profiles ().profile_count () == 0)
    throw TAO_IOP::EmptyProfileList ();

  // initialize with estimated pfile count.
  TAO_MProfile Diff_Profiles (count);

  auto_ptr<TAO_MProfile> tmp_pfiles (group->_stubobj ()->make_profiles ());
  if (Diff_Profiles.add_profiles (tmp_pfiles.get ()) < 0)
    throw TAO_IOP::Invalid_IOR ();

  // We are done with add_profiles.
  // At this point, we don't do remove_profiles()
  // immediately like before,
  // because it could result in an
  // Object Reference with 0 profile. And it would not pass
  // the ::CORBA::is_nil() evaluation.
  // Instead, we create the Object Reference right here, which is
  // earlier than before.(Actually, I just moved some code
  // from below up to here).
  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();

  TAO_Stub *stub = orb_core->create_stub (id.in (), // give the id string
                                          Diff_Profiles
                                         );

  // Make the stub memory allocation exception safe for the duration
  // of this method.
  TAO_Stub_Auto_Ptr safe_stub (stub);

  // Create the CORBA level proxy
  CORBA::Object_ptr temp_obj = CORBA::Object::_nil ();
  ACE_NEW_THROW_EX (temp_obj,
                    CORBA::Object (safe_stub.get ()),
                    CORBA::NO_MEMORY ());

  CORBA::Object_var new_obj = temp_obj;

  // Exception safety is no longer an issue by this point so release
  // the TAO_Stub from the TAO_Stub_Auto_Ptr.
  stub = safe_stub.release ();

  // Clean up in case of errors.
  if (CORBA::is_nil (new_obj.in ()))
    {
      throw TAO_IOP::Invalid_IOR ();
    }

  // Now we can remove the profiles which we want to eliminate from
  // the Object.
  ACE_auto_ptr_reset (tmp_pfiles,
                      ior2->_stubobj ()->make_profiles ());

  TAO_MProfile& mp = stub -> base_profiles();
  if (mp.remove_profiles (tmp_pfiles.get ()) < 0)
    throw TAO_IOP::NotFound ();

  // MS C++ knows nothing about reset!
  // tmp_pfiles.reset (0); // get rid of last MProfile

  return new_obj._retn ();
}

CORBA::Boolean
TAO_IOR_Manipulation_impl::set_property (
    TAO_IOP::TAO_IOR_Property_ptr prop,
    CORBA::Object_ptr group)
{
  // make sure we have some profiles
  if (group->_stubobj ()->base_profiles ().profile_count () == 0)
    throw TAO_IOP::Invalid_IOR ();

  // Call the implementation object to
  return prop->set_property (group);
}

//@@ note awkward argument order
CORBA::Boolean
TAO_IOR_Manipulation_impl::set_primary (
    TAO_IOP::TAO_IOR_Property_ptr prop,
    CORBA::Object_ptr new_primary,
    CORBA::Object_ptr group)
{
  // make sure we have some profiles in GROUP
  if (group->_stubobj ()->base_profiles ().profile_count () == 0)
    throw TAO_IOP::Invalid_IOR ();

  // Make sure we have only one profile in new_primary
  // @@ Will fail if the object has been
  /*if (new_primary->_stubobj ()->base_profiles ().profile_count () > 1)
    throw TAO_IOP::MultiProfileList ();*/

  // Call the callback object to do the rest of the processing.
  return prop->set_primary (new_primary, group);
}

CORBA::Object_ptr
TAO_IOR_Manipulation_impl::get_primary (
      TAO_IOP::TAO_IOR_Property_ptr prop,
      CORBA::Object_ptr group)
{
  // make sure we have some profiles in IOR
  if (group->_stubobj ()->base_profiles ().profile_count () == 0)
    throw TAO_IOP::NotFound ();
  // @@ Bala: this was throwing TAO_IOP::Invalid_IOR, but it was not
  // in the throw spec, that will result in a CORBA::UNKNOWN at
  // run-time (if it does not crash).  Any idea about what is going on
  // here?

  return prop->get_primary (group);
}

CORBA::Boolean
TAO_IOR_Manipulation_impl::is_primary_set (
    TAO_IOP::TAO_IOR_Property_ptr prop,
    CORBA::Object_ptr group)
{
  return prop->is_primary_set (group);
}

CORBA::Boolean
TAO_IOR_Manipulation_impl:: remove_primary_tag (
    TAO_IOP::TAO_IOR_Property_ptr prop,
    CORBA::Object_ptr group)
{
  return prop->remove_primary_tag (group);
}

CORBA::ULong
TAO_IOR_Manipulation_impl::is_in_ior (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2)
{
  CORBA::ULong count = 0;
  TAO_Profile *pfile1 = 0;
  TAO_Profile *pfile2 = 0;
  auto_ptr<TAO_MProfile> tmp_pfiles1 (ior1->_stubobj ()->make_profiles ());
  auto_ptr<TAO_MProfile> tmp_pfiles2 (ior2->_stubobj ()->make_profiles ());

  tmp_pfiles1->rewind ();
  while ((pfile1 = tmp_pfiles1->get_next ()) != 0)
    {
      tmp_pfiles2->rewind ();
      while ((pfile2 = tmp_pfiles2->get_next ()) != 0)
        {
          if (pfile1->is_equivalent (pfile2))
            ++count;
        }
    }

  if (count == 0)
    throw TAO_IOP::NotFound ();

  return count;
}

CORBA::ULong
TAO_IOR_Manipulation_impl::get_profile_count (CORBA::Object_ptr group)
{
  CORBA::ULong const count = group->_stubobj ()->base_profiles ().profile_count ();

  if (count == 0)
    throw TAO_IOP::EmptyProfileList ();

  return count;
}

TAO_END_VERSIONED_NAMESPACE_DECL
