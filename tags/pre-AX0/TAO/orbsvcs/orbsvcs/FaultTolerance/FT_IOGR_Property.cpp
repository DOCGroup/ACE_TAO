#include "FT_Service_Activate.h"
#include "tao/MProfile.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(FaultTolerance, TAO_FT_IOGR_Property, "$Id$")

#if !defined (__ACE_INLINE__)
# include "FT_IOGR_Property.i"
#endif /* ! __ACE_INLINE__ */


CORBA::Boolean
TAO_FT_IOGR_Property::set_property (
    CORBA::Object_ptr ior
    ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_IOP::Invalid_IOR))
{
  // We need to apply the property for every profile in the IOR

  // Make profiles from the sent <ior>
  TAO_MProfile &tmp_pfiles =
    ior->_stubobj ()->base_profiles ();

  // Get the profile count.
  CORBA::ULong count =
    ior->_stubobj ()->base_profiles ().profile_count ();

  // Make the output CDR stream
  TAO_OutputCDR cdr;
  CORBA::Boolean retval = 0;
  retval =
    cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);

  IOP::TaggedComponent tagged_components;
  tagged_components.tag = IOP::TAG_FT_GROUP;

  // Encode the property in to the tagged_components
  retval = retval &&
    this->encode_properties (cdr,
                             tagged_components);

  // Go through every profile and set the TaggedComponent field
  for (CORBA::ULong i = 0; i < count ; i++)
    {
      // Get the tagged components in the profile
      TAO_Tagged_Components &tag_comp =
        tmp_pfiles.get_profile (i)->tagged_components ();

      // Finally set the <tagged_component> in the
      // <TAO_Tagged_Component>
      tag_comp.set_component (tagged_components);
    }

  // Success
  return 1;

}


CORBA::Boolean
TAO_FT_IOGR_Property::is_primary_set (
    CORBA::Object_ptr ior
    ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  if (this->get_primary_profile (ior) != 0)
    return 1;

  return 0;
}


CORBA::Object_ptr
TAO_FT_IOGR_Property::get_primary (
    CORBA::Object_ptr ior
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::NotFound
      ))
{

  TAO_Profile *pfile =
    this->get_primary_profile (ior);

  if (pfile == 0)
    ACE_THROW_RETURN (TAO_IOP::NotFound (),
                      CORBA::Object::_nil ());

  // Search for the IOP::TAG_FT_PRIMARY in the tagged component of
  // the profile

  // Get the repository id
  CORBA::String_var id =
    CORBA::string_dup (ior->_stubobj ()->type_id.in ());

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();

  // Make a stub Object
  TAO_Stub *stub = 0;

  // Make a MProfile object for the primary profile alone. So, the
  // size is 1
  CORBA::ULong sz = 1;
  TAO_MProfile mpfile (sz);
  if (mpfile.add_profile (pfile) != -1)
    {
      ACE_NEW_THROW_EX (stub,
                        TAO_Stub (id._retn (),  // give the id string to stub
                                  mpfile,
                                  orb_core),
                        CORBA::NO_MEMORY ());
    }

  // Make the stub memory allocation exception safe for
  // the duration of this method.
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
      ACE_THROW_RETURN (TAO_IOP::NotFound (),  // ** Changed from Invalid_IOR () **
                        CORBA::Object::_nil ());
    }

  // Release ownership of the pointers protected by the auto_ptrs since they
  // no longer need to be protected by this point.
  stub = safe_stub.release ();

  return new_obj._retn ();

}


CORBA::Boolean
TAO_FT_IOGR_Property::set_primary (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::NotFound,
        TAO_IOP::Duplicate
      ))
{
  // Check for primary in <ior2>
  IOP::TaggedComponent tagged_components;
  tagged_components.tag = IOP::TAG_FT_PRIMARY;

  TAO_MProfile &mprofile =
    ior2->_stubobj ()->base_profiles ();

  for (CORBA::ULong i = 0;
       i < mprofile.profile_count ();
       i++)
    {
      TAO_Tagged_Components &tag_comp =
        mprofile.get_profile (i)->tagged_components ();

      if (tag_comp.get_component (tagged_components) == 1)
        ACE_THROW_RETURN (TAO_IOP::Duplicate (),
                          0);
    }

  // Check whether ior1 exists in ior2
  CORBA::Long index = -1;

  // Even if it is a multi-profile (in case of multi-homed hosts we
  // could get a multiple IOR profile), we are going to make the first one
  // in that a primary
  TAO_Profile *prim_profile =
    ior1->_stubobj ()->base_profiles ().get_profile (0);

  CORBA::ULong count =
    ior2->_stubobj ()->base_profiles ().profile_count ();

  for (CORBA::ULong ctr = 0;
       ctr < count;
       ctr ++)
    {
      if (prim_profile->is_equivalent (
          ior2->_stubobj ()->base_profiles ().get_profile (ctr)) == 1)
        {
          // We know that <ior1> exists in <io2>
          // Take the index of that and break out of the loop
          index = ctr;
          break;
        }
    }

  // At the end of the loop check whether we have seen <ior1> in
  // <ior2>
  if (index == -1)
    ACE_THROW_RETURN (TAO_IOP::NotFound (),
                      0);

  CORBA::Boolean val = 1;

  TAO_OutputCDR cdr;
  cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
  cdr << ACE_OutputCDR::from_boolean (val);

  // Get the length of the CDR stream
  CORBA::ULong length =
    cdr.total_length ();

  // Set the length
  tagged_components.component_data.length (length);

  // Get the pointer to the underlying buffer
  CORBA::Octet *buf =
    tagged_components.component_data.get_buffer ();

  for (const ACE_Message_Block *mb = cdr.begin ();
       mb != 0;
       mb = mb->cont ())
    {
      ACE_OS::memcpy (buf, mb->rd_ptr (), mb->length ());
      buf += mb->length ();
    }

  // Set the <tagged_component>
  TAO_Tagged_Components &tagg =
    ior2->_stubobj ()->base_profiles ().get_profile (index)->tagged_components ();

  tagg.set_component (tagged_components);

  return 1;
}

CORBA::Boolean
TAO_FT_IOGR_Property::encode_properties (
    TAO_OutputCDR &cdr,
    IOP::TaggedComponent &tagged_components)
{
  // the version info
  CORBA::Boolean status =
    cdr << this->ft_group_tagged_component_.version;

  // the domain id
  status = status &&
    cdr << this->ft_group_tagged_component_.ft_domain_id.in ();

  // Object group id
  status = status &&
    cdr << this->ft_group_tagged_component_.object_group_id;

  // Object group reference version
  status = status &&
    cdr << this->ft_group_tagged_component_.object_group_ref_version;

  // Get the length of the CDR stream
  CORBA::ULong length =
    cdr.total_length ();

  // Set the length
  tagged_components.component_data.length (length);

  // Get the pointer to the underlying buffer
  CORBA::Octet *buf =
    tagged_components.component_data.get_buffer ();

  for (const ACE_Message_Block *i = cdr.begin ();
       i != 0;
       i = i->cont ())
    {

      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }


  return status;
}


TAO_Profile *
TAO_FT_IOGR_Property::get_primary_profile (
    CORBA::Object_ptr ior)
{
  // Get the MProfile
  TAO_MProfile &mprofile =
    ior->_stubobj ()->base_profiles ();

  // Looking for a tagged component with a PRIMARY flag.
  IOP::TaggedComponent tagged_components;
  tagged_components.tag = IOP::TAG_FT_PRIMARY;

  for (CORBA::ULong i = 0;
       i < mprofile.profile_count ();
       i++)
    {
      // Get the Tagged Components
      const TAO_Tagged_Components &pfile_tagged =
        mprofile.get_profile (i)->tagged_components ();

      // Look for the primary
      if (pfile_tagged.get_component (tagged_components) == 1)
        {
          // If there is one return the pfile
          return mprofile.get_profile (i);
        }
    }
  return 0;
}
