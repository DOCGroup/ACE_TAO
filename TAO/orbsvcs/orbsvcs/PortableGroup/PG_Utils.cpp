// $Id$

#include "orbsvcs/PortableGroup/PG_Utils.h"

#include "tao/MProfile.h"
#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/Tagged_Components.h"
#include "tao/CDR.h"

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /*static*/ CORBA::Boolean
  PG_Utils::set_tagged_component (
      PortableGroup::ObjectGroup *&ior,
      PortableGroup::TagGroupTaggedComponent &tg)
  {
    if (ior->_stubobj () == 0)
      return 0;

    // We need to apply the property for every profile in the IOR
    TAO_MProfile &tmp_pfiles =
      ior->_stubobj ()->base_profiles ();

    // Create the output CDR stream
    TAO_OutputCDR cdr;

    IOP::TaggedComponent tagged_components;
    tagged_components.tag = IOP::TAG_FT_GROUP;

    // Encode the property in to the tagged_components
    CORBA::Boolean retval =
      PG_Utils::encode_properties (cdr,
                                   tg);

    if (retval == 0)
      return retval;

    // Get the length of the CDR stream
    CORBA::ULong length = static_cast<CORBA::ULong> (cdr.total_length ());

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
    const IOP::TaggedComponent &tmp_tc = tagged_components;

    // Get the profile count.
    CORBA::ULong count =
      ior->_stubobj ()->base_profiles ().profile_count ();

    // Go through every profile and set the TaggedComponent field
    for (CORBA::ULong p_idx = 0; p_idx < count ; ++p_idx)
      {
        // Get the tagged components in the profile
        TAO_Tagged_Components &tag_comp =
          tmp_pfiles.get_profile (p_idx)->tagged_components ();

        // Finally set the <tagged_component> in the
        // <TAO_Tagged_Component>
        tag_comp.set_component (tmp_tc);
      }

    // Success
    return 1;
  }

  /*static*/ CORBA::Boolean
  PG_Utils::get_tagged_component (
      PortableGroup::ObjectGroup *&ior,
      PortableGroup::TagGroupTaggedComponent &tg)
  {
    if (ior->_stubobj () == 0)
      return 0;

    TAO_MProfile &mprofile =
      ior->_stubobj ()->base_profiles ();

    // Looking for a tagged component with a TAG_FT_GROUP flag.
    IOP::TaggedComponent tc;
    tc.tag = IOP::TAG_FT_GROUP;

    CORBA::ULong count =
      mprofile.profile_count ();

    for (CORBA::ULong i = 0;
         i < count;
         i++)
      {

        // Get the Tagged Components
        const TAO_Tagged_Components &pfile_tagged =
          mprofile.get_profile (i)->tagged_components ();

        // Look for the primary
        if (pfile_tagged.get_component (tc) == 1)
          {
            TAO_InputCDR cdr (reinterpret_cast<const char*> (tc.component_data.get_buffer ()),
                            tc.component_data.length ());

            CORBA::Boolean byte_order;

            cdr >> ACE_InputCDR::to_boolean (byte_order);

            if (!cdr.good_bit ())
              return 0;

            cdr.reset_byte_order (static_cast<int> (byte_order));

            cdr >> tg;

            if (cdr.good_bit ())
              return 1;
          }
      }

    return 0;
  }

  CORBA::Boolean
  PG_Utils::encode_properties (
      TAO_OutputCDR &cdr,
      PortableGroup::TagGroupTaggedComponent &tg)
  {
    cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);

    if (!cdr.good_bit ())
      return 0;

    // the version info
    cdr << tg.component_version;

    if (!cdr.good_bit ())
      return 0;

    // the domain id
    cdr << tg.group_domain_id.in ();

    if (!cdr.good_bit ())
      return 0;

    // Object group id
    cdr << tg.object_group_id;

    if (!cdr.good_bit ())
      return 0;

    // Object group reference version
    cdr << tg.object_group_ref_version;

    if (!cdr.good_bit ())
      return 0;

    return cdr.good_bit ();
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
