// -*- C++ -*-
// $Id$

#include "orbsvcs/PortableGroup/PG_PropertyManager.h"
#include "orbsvcs/PortableGroup/PG_ObjectGroupManager.h"
#include "orbsvcs/PortableGroup/PG_Property_Utils.h"

#include "tao/ORB_Constants.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PG_PropertyManager::TAO_PG_PropertyManager (
  TAO_PG_ObjectGroupManager & object_group_manager)
  : object_group_manager_ (object_group_manager),
    default_properties_ (),
    type_properties_ (),
    lock_ (),
    property_validator_ ()
{
}


void
TAO_PG_PropertyManager::set_default_properties (
    const PortableGroup::Properties & props)
{
  // First verify that the "Factories" property is not in the
  // Properties sequence.  According to the spec, it is not allowed to
  // be set as part of the default properties.
  PortableGroup::Name factories;
  factories.length (1);
  factories[0].id = CORBA::string_dup ("org.omg.PortableGroup.Factories");

  CORBA::ULong len = props.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      PortableGroup::Property property = props[i];

      if (property.nam == factories)
        throw PortableGroup::InvalidProperty (property.nam, property.val);
    }

  this->property_validator_.validate_property (props);

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  this->default_properties_ = props;
}


PortableGroup::Properties *
TAO_PG_PropertyManager::get_default_properties ()
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

  PortableGroup::Properties * props = 0;
  ACE_NEW_THROW_EX (props,
                    PortableGroup::Properties (this->default_properties_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return props;
}


void
TAO_PG_PropertyManager::remove_default_properties (
    const PortableGroup::Properties &props)
{
  if (props.length () == 0)
    return;  // @@ Throw CORBA::BAD_PARAM instead?

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  this->remove_properties (props,
                           this->default_properties_);
}


void
TAO_PG_PropertyManager::set_type_properties (
    const char * type_id,
    const PortableGroup::Properties & overrides)
{
  this->property_validator_.validate_property (overrides);

  CORBA::ULong num_overrides = overrides.length ();

  if (num_overrides == 0)
    return;  // Throw CORBA::BAD_PARAM exception instead?

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  Type_Prop_Table::ENTRY * entry = 0;
  if (this->type_properties_.find (type_id, entry) != 0)
    throw CORBA::BAD_PARAM ();

  PortableGroup::Properties & props = entry->int_id_;
  props = overrides;
}


PortableGroup::Properties *
TAO_PG_PropertyManager::get_type_properties (
    const char * type_id)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);


  Type_Prop_Table::ENTRY * entry = 0;
  PortableGroup::Properties * type_properties = 0;

  if (this->type_properties_.find (type_id, entry) == 0)
    type_properties = &entry->int_id_;

  const CORBA::ULong def_props_len = this->default_properties_.length ();
  const CORBA::ULong type_props_len =
    (type_properties == 0 ? 0 : type_properties->length ());
  const CORBA::ULong props_len =
    (def_props_len > type_props_len ? def_props_len : type_props_len);

  PortableGroup::Properties * tmp_properties = 0;
  ACE_NEW_THROW_EX (tmp_properties,
                    PortableGroup::Properties (props_len),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableGroup::Properties_var properties = tmp_properties;

  // Set the length to the length of the largest of the two property
  // sequences.  The idea is to keep the cost of the incremental
  // growth that may occur in TAO_PG::override_properties() to a
  // minimum.
  properties->length (props_len);

  *tmp_properties = this->default_properties_;

  if (type_properties != 0 && type_props_len > 0)
    TAO_PG::override_properties (*type_properties, *tmp_properties);

  return properties._retn ();
}


void
TAO_PG_PropertyManager::remove_type_properties (
    const char * type_id,
    const PortableGroup::Properties & props)
{
  if (props.length () == 0)
    return;  // @@ Throw CORBA::BAD_PARAM instead?

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  Type_Prop_Table::ENTRY * entry = 0;
  if (this->type_properties_.find (type_id, entry) != 0)
    throw CORBA::BAD_PARAM ();

  PortableGroup::Properties & type_properties = entry->int_id_;

  this->remove_properties (props,
                           type_properties);
}


void
TAO_PG_PropertyManager::set_properties_dynamically (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    const PortableGroup::Properties & /* overrides */)
{
#if 0
  // First verify that the "InitialNumberMembers" property is not in
  // the Properties sequence.  According to the spec, it is not
  // allowed to be set as part of the default properties.
  PortableGroup::Name factories;
  factories.length (1);
  factories[0].id =
    CORBA::string_dup ("org.omg.PortableGroup.InitialNumberMembers");

  CORBA::ULong len = props.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      PortableGroup::Property property = props[i];

      if (property.nam == factories)
        throw PortableGroup::InvalidProperty (property.nam, property.val);
    }

  this->property_validator_.validate_property (overrides);

  // @todo Set the properties in the object group map entry.
#endif  /* 0 */

  throw CORBA::NO_IMPLEMENT ();
}


PortableGroup::Properties *
TAO_PG_PropertyManager::get_properties (
    PortableGroup::ObjectGroup_ptr object_group)
{
  CORBA::ULong properties_len = 0;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, property_map_guard, this->lock_, 0);

  // @@ Race condition here!
  PortableGroup::Properties_var dynamic_properties =
    this->object_group_manager_.get_properties (object_group);

  CORBA::ULong dyn_props_len = dynamic_properties->length ();
  if (dyn_props_len > properties_len)
    properties_len = dyn_props_len;

  CORBA::String_var type_id =
    this->object_group_manager_.type_id (object_group);

  CORBA::ULong type_props_len = 0;
  PortableGroup::Properties * type_properties = 0;
  Type_Prop_Table::ENTRY * type_entry = 0;
  if (this->type_properties_.find (type_id.in (), type_entry) == 0)
    {
      type_properties = &type_entry->int_id_;
      type_props_len = type_properties->length ();

      if (type_props_len > properties_len)
        properties_len = type_props_len;
    }

  CORBA::ULong def_props_len = this->default_properties_.length ();
  if (def_props_len > properties_len)
    properties_len = def_props_len;

  PortableGroup::Properties * tmp_properties = 0;
  ACE_NEW_THROW_EX (tmp_properties,
                    PortableGroup::Properties (properties_len),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableGroup::Properties_var properties = tmp_properties;

  // Set the length to the length of the largest of the three property
  // sequences.  The idea is to keep the cost of the incremental
  // growth that may occur in TAO_PG::override_properties() to a
  // minimum.
  properties->length (properties_len);

  // Start out with a copy of the default properties.
  *tmp_properties = this->default_properties_;

  // Override default properties with type-specific properties.
  if (type_properties != 0)
    TAO_PG::override_properties (*type_properties, *tmp_properties);

  // Now override with the dynamic (object group) properties.
  TAO_PG::override_properties (dynamic_properties.in (), *tmp_properties);

  return properties._retn ();
}


void
TAO_PG_PropertyManager::remove_properties (
    const PortableGroup::Properties & to_be_removed,
    PortableGroup::Properties &properties)
{
  const CORBA::ULong num_removed = to_be_removed.length ();
  if (num_removed == 0)
    return;  // @@ Throw CORBA::BAD_PARAM instead?

  const CORBA::ULong old_length = properties.length ();

  const CORBA::ULong new_length = old_length - num_removed;

  PortableGroup::Properties new_properties (new_length);
  new_properties.length (new_length);

  // @@ Slow O(n^2) operation.  Switching to a faster container for
  //    the default properties might be a good idea at some point in
  //    the future.
  CORBA::ULong n = 0;
  for (CORBA::ULong i = 0; i < num_removed; ++i)
    {
      const CORBA::ULong old_n = n;
      const PortableGroup::Property &remove = to_be_removed[i];

      for (CORBA::ULong j = 0; j < old_length; ++j)
        if (remove.nam != properties[j].nam)
          new_properties[n++] = properties[j];

      // The property to be removed doesn't exist in the current list
      // of default properties.
      if (n == old_n)
        throw PortableGroup::InvalidProperty (remove.nam, remove.val);
    }

  // All properties were successfully removed, and the remaining ones
  // were placed in the "new_properties" variable.  Now copy that
  // variable.
  properties = new_properties;
}

TAO_END_VERSIONED_NAMESPACE_DECL
