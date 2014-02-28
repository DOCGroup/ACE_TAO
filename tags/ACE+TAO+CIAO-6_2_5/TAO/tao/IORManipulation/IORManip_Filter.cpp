// $Id$

#include "tao/IORManipulation/IORManip_Filter.h"
#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Stub.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IORManip_Filter::TAO_IORManip_Filter (void)
{
}


TAO_IORManip_Filter::~TAO_IORManip_Filter (void)
{
}


CORBA::Object_ptr
TAO_IORManip_Filter::sanitize_profiles (CORBA::Object_ptr object,
                                        TAO_Profile* profile)
{
  return this->sanitize (object, profile);
}


CORBA::Object_ptr
TAO_IORManip_Filter::sanitize (CORBA::Object_ptr object,
                               TAO_Profile* guideline)
{
  TAO_MProfile profiles = object->_stubobj ()->base_profiles ();
  TAO_MProfile new_profiles (profiles.profile_count ());
  TAO_Profile* profile = 0;

  while ((profile = profiles.get_next ()) != 0)
    {
      // Call the filter implementation
      this->filter_and_add (profile, new_profiles, guideline);
    }

  // The remainder of this code has been lifted from IORManipulation.cpp
  CORBA::String_var id =
      CORBA::string_dup (object->_stubobj ()->type_id.in ());

  TAO_ORB_Core *orb_core = object->_stubobj ()->orb_core ();
  if (orb_core == 0)
    orb_core = TAO_ORB_Core_instance ();

  TAO_Stub *stub = orb_core->create_stub (id.in (),  // give the id string to stub
                                          new_profiles);

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

  // Release ownership of the pointers protected by the auto_ptrs since
  // they no longer need to be protected by this point.
  stub = safe_stub.release ();

  return new_obj._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
