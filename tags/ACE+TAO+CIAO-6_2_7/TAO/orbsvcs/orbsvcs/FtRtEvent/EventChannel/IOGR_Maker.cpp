// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/FtRtEvent/EventChannel/IOGR_Maker.h"
#include "tao/MProfile.h"
#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Object_KeyC.h"
#include "tao/Tagged_Components.h"
#include "../Utils/resolve_init.h"
#include "../Utils/Safe_InputCDR.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "orbsvcs/FtRtEvent/EventChannel/GroupInfoPublisher.h"

static IOGR_Maker* maker;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

IOGR_Maker::IOGR_Maker()
{
}

void
IOGR_Maker::init(CORBA::ORB_ptr orb)
{
    iorm_ = resolve_init<TAO_IOP::TAO_IOR_Manipulation>(orb,
                                                        TAO_OBJID_IORMANIPULATION);
    ft_tag_component_.group_domain_id = "ft_eventchannel";
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
IOGR_Maker::merge_iors(const TAO_IOP::TAO_IOR_Manipulation::IORList& list)
{
  CORBA::Object_var obj;
  if (list.length() != 1)
    obj = iorm_->merge_iors(list);
  else
    obj = CORBA::Object::_duplicate(list[0]);
  return obj._retn();
}


CORBA::Object_ptr
IOGR_Maker::make_iogr(const TAO_IOP::TAO_IOR_Manipulation::IORList& list,
                      CORBA::ULong object_group_ref_version)
{
  /// generate a new IOGR if the object group changes.
  CORBA::Object_var obj = merge_iors(list);

  FT::TagFTGroupTaggedComponent ft_tag_component(ft_tag_component_);
  /// the generated IOGR should use a new object_group_ref_version
  ft_tag_component.object_group_ref_version = object_group_ref_version;
  set_tag_components(obj.in(), list[0], ft_tag_component);

  return obj._retn();
}

void replace_key(char* ior, char* end_ior,
                const TAO::ObjectKey& oldkey,
                const TAO::ObjectKey& newkey);
/// the definition of replace_key() is moved
/// to replace_key.cpp.


CORBA::Object_ptr
IOGR_Maker::forge_iogr(CORBA::Object_ptr obj)
{
  /// forge an IOGR whose object_key is the same with that of \a obj.
  CORBA::Object_var merged;
  // make a copy of the object
  FtRtecEventChannelAdmin::EventChannel_var successor
    = GroupInfoPublisher::instance()->successor();
  if (! CORBA::is_nil(successor.in())) {
    TAO::ObjectKey_var newkey = obj->_key();

    CORBA::Object_var new_base = ior_replace_key(successor.in(), newkey.in());

    if (CORBA::is_nil( new_base.in() ))
      return CORBA::Object::_nil();

    TAO_MProfile& base_profiles = new_base->_stubobj ()->base_profiles ();

    TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();

    TAO_Stub *stub = orb_core->create_stub (CORBA::string_dup(obj->_stubobj ()->type_id.in ()), // give the id string
      base_profiles);

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
      iorm_->add_profiles(obj, temp_obj);
  }
  else
    merged = CORBA::Object::_duplicate(obj);

  set_tag_components(merged.in(), obj, ft_tag_component_);


  return merged._retn();
}

CORBA::Object_ptr
IOGR_Maker::ior_replace_key(CORBA::Object_ptr obj,
                            const TAO::ObjectKey& key)
{
    TAO_OutputCDR out_cdr;
    if (!(out_cdr << obj))
      return CORBA::Object::_nil();

    ACE_Message_Block mb;

    ACE_CDR::consolidate(&mb, out_cdr.begin());

    TAO::ObjectKey_var old_key = obj->_key();

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
          Safe_InputCDR cdr (
            reinterpret_cast<const char*> (tagged_components.component_data.get_buffer ()),
            tagged_components.component_data.length ());
          CORBA::Boolean byte_order;

          if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
            return false;

          cdr.reset_byte_order (static_cast<int> (byte_order));

          return (cdr >> ft_tag_component_) != 0;
       }
    }

    return false;
}

void
IOGR_Maker::set_ft_domain_id(const char* domain_id)
{
  ft_tag_component_.group_domain_id = domain_id;
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
  ORBSVCS_DEBUG((LM_DEBUG, "new object_group_ref_version = %d\n", ft_tag_component_.            object_group_ref_version+1));
  return ++ft_tag_component_.object_group_ref_version;
}

CORBA::ULong
IOGR_Maker::get_ref_version() const
{
  return ft_tag_component_.object_group_ref_version;
}


void
IOGR_Maker::set_tag_components(
  CORBA::Object_ptr merged,
  CORBA::Object_ptr primary,
  FT::TagFTGroupTaggedComponent& ft_tag_component)
{
    // set the primary
    TAO_FT_IOGR_Property prop (ft_tag_component);


    prop.remove_primary_tag(merged);

    iorm_->set_primary (&prop, merged, primary);
      // Set the property
    iorm_->set_property (&prop,
                        merged);


}

TAO_END_VERSIONED_NAMESPACE_DECL
