// $Id$

#include "IOGR_Maker.h"
#include "tao/MProfile.h"
#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Object_KeyC.h"
#include "tao/Tagged_Components.h"
#include <algorithm>
#include "../Utils/resolve_init.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "GroupInfoPublisher.h"
#include "../Utils/Log.h"

ACE_RCSID (EventChannel,
           IOGR_Maker,
           "$Id$")


static IOGR_Maker* maker;

IOGR_Maker::IOGR_Maker()
{
}

void
IOGR_Maker::init(CORBA::ORB_ptr orb
                 ACE_ENV_ARG_DECL)
{
    iorm_ = resolve_init<TAO_IOP::TAO_IOR_Manipulation>(orb,
                                                        TAO_OBJID_IORMANIPULATION
                                                        ACE_ENV_ARG_PARAMETER);
    ft_tag_component_.ft_domain_id = "ft_eventchannel";
    ft_tag_component_.object_group_id = 0;
    ft_tag_component_.object_group_ref_version = 0;
    maker = this;
}


IOGR_Maker*
IOGR_Maker::instance()
{
  return maker;
}

CORBA::Object_ptr
IOGR_Maker::merge_iors(const TAO_IOP::TAO_IOR_Manipulation::IORList& list
                       ACE_ENV_ARG_DECL)
{
  CORBA::Object_var obj;
  if (list.length() != 1)
    obj = iorm_->merge_iors(list ACE_ENV_ARG_PARAMETER);
  else
    obj = CORBA::Object::_duplicate(list[0].in());
  return obj._retn();
}


CORBA::Object_ptr
IOGR_Maker::make_iogr(const TAO_IOP::TAO_IOR_Manipulation::IORList& list
                      ACE_ENV_ARG_DECL)
{
  CORBA::Object_var obj = merge_iors(list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(CORBA::Object::_nil());

  set_tag_components(obj.in(), list[0]
                     ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN(CORBA::Object::_nil());
  return obj._retn();
}


void replace_key(char* ior, char* end_ior,
                const TAO::ObjectKey& oldkey,
                const TAO::ObjectKey& newkey)
{
  size_t keylen = oldkey.length();
  ACE_ASSERT(keylen == newkey.length());

  char* pos = ior;
  const char* oldkey_begin = (const char*)oldkey.get_buffer();
  const char* oldkey_end = oldkey_begin + keylen;

  while ((pos = std::search(pos, end_ior,oldkey_begin, oldkey_end)) != end_ior) {
    memcpy(pos, newkey.get_buffer(), keylen);
    pos+= keylen;
  }
}


CORBA::Object_ptr
IOGR_Maker::forge_iogr(CORBA::Object_ptr obj
                       ACE_ENV_ARG_DECL)
{
  CORBA::Object_var merged;
  // make a copy of the object
  FtRtecEventChannelAdmin::EventChannel_var successor
    = GroupInfoPublisher::instance()->successor();
  if (! CORBA::is_nil(successor.in())) {
    TAO::ObjectKey_var newkey = obj->_key(ACE_ENV_SINGLE_ARG_PARAMETER);

    CORBA::Object_var new_base = ior_replace_key(successor.in(), newkey.in()
                                                 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(CORBA::Object::_nil());

    if (CORBA::is_nil( new_base.in() ))
      return CORBA::Object::_nil();

    TAO_MProfile& base_profiles = new_base->_stubobj ()->base_profiles ();

    TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();

    TAO_Stub *stub = orb_core->create_stub (CORBA::string_dup(obj->_stubobj ()->type_id.in ()), // give the id string
      base_profiles
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (CORBA::Object::_nil ());

    // Make the stub memory allocation exception safe for the duration
    // of this method.
    TAO_Stub_Auto_Ptr safe_stub (stub);

    // Create the CORBA level proxy
    CORBA::Object_ptr temp_obj = CORBA::Object::_nil ();
    ACE_NEW_THROW_EX (temp_obj,
      CORBA::Object (safe_stub.get ()),
      CORBA::NO_MEMORY ());

    // Release ownership of the pointers protected by the auto_ptrs since they
    // no longer need to be protected by this point.
    stub = safe_stub.release ();


    merged =
      iorm_->add_profiles(obj, temp_obj
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (CORBA::Object::_nil ());
  }
  else
    merged = CORBA::Object::_duplicate(obj);

  set_tag_components(merged.in(), obj
                     ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN(CORBA::Object::_nil ());

  return merged._retn();
}

CORBA::Object_ptr
IOGR_Maker::ior_replace_key(CORBA::Object_ptr obj,
                            const TAO::ObjectKey& key
                            ACE_ENV_ARG_DECL)
{
    TAO_OutputCDR out_cdr;
    if (!(out_cdr << obj))
      return CORBA::Object::_nil();

    ACE_Message_Block mb;

    ACE_CDR::consolidate(&mb, out_cdr.begin());

    TAO::ObjectKey_var old_key = obj->_key(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN(CORBA::Object::_nil ());

    replace_key(mb.base(), mb.end(),
                old_key.in(), key);

    CORBA::Object_var new_obj;
    TAO_InputCDR in_cdr(&mb);
    if (!(in_cdr >> new_obj))
      return CORBA::Object::_nil();
    return new_obj._retn();
}

bool
IOGR_Maker::copy_ft_group_component(CORBA::Object_ptr ior)
{
  // Get the MProfile
  TAO_MProfile &mprofile =
    ior->_stubobj ()->base_profiles ();

  // Looking for a tagged component with a GROUP flag.
  IOP::TaggedComponent tagged_components;
  tagged_components.tag = IOP::TAG_FT_GROUP;

   if (mprofile.profile_count () > 0)
    {
      // Get the Tagged Components
      const TAO_Tagged_Components &pfile_tagged =
        mprofile.get_profile (0)->tagged_components ();

       if (pfile_tagged.get_component (tagged_components) == 1)
       {

          // Grab the object group version
          // @@ NOTE: This involves an allocation and a dellocation. This is
          // really bad.
          TAO_InputCDR cdr (
            ACE_reinterpret_cast (const char*,
                                  tagged_components.component_data.get_buffer ()),
            tagged_components.component_data.length ());
          CORBA::Boolean byte_order;

          if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
            return false;

          cdr.reset_byte_order (ACE_static_cast (int,byte_order));

          return (cdr >> ft_tag_component_) != 0;
       }
    }

    return false;
}

void
IOGR_Maker::set_ft_domain_id(const char* domain_id)
{
  ft_tag_component_.ft_domain_id = domain_id;
}

void
IOGR_Maker::set_group_id(CORBA::ULongLong id)
{
  ft_tag_component_.object_group_id = id;
}

void
IOGR_Maker::set_ref_version(CORBA::ULong version)
{
  ft_tag_component_.object_group_ref_version = version;
}

CORBA::ULong
IOGR_Maker::increment_ref_version()
{
  TAO_FTRTEC::Log(1, "new object_group_ref_version = %d\n", ft_tag_component_.object_group_ref_version+1);
  return ++ft_tag_component_.object_group_ref_version;
}

CORBA::ULong
IOGR_Maker::get_ref_version() const
{
  return ft_tag_component_.object_group_ref_version;
}


void
IOGR_Maker::set_tag_components(CORBA::Object_ptr merged, CORBA::Object_ptr primary
                               ACE_ENV_ARG_DECL)
{
    // set the primary
    TAO_FT_IOGR_Property prop (ft_tag_component_);


    prop.remove_primary_tag(merged
                         ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    iorm_->set_primary (&prop, merged, primary
                       ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
      // Set the property
    iorm_->set_property (&prop,
                        merged
                        ACE_ENV_ARG_PARAMETER);


}

