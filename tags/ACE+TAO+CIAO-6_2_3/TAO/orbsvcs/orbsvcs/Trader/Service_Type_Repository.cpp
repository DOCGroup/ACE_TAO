// $Id$

#include "orbsvcs/Trader/Service_Type_Repository.h"
#include "ace/Lock_Adapter_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Service_Type_Repository::TAO_Service_Type_Repository (ACE_Lock* lock)
  : lock_ (lock)
{
  this->incarnation_.low = 0;
  this->incarnation_.high = 0;

  // If a lock wasn't provided, let's assume the user doesn't want any
  // kind of lock at all.
  if (this->lock_ == 0)
    ACE_NEW (this->lock_,
             ACE_Lock_Adapter<ACE_Null_Mutex>);
}

TAO_Service_Type_Repository::~TAO_Service_Type_Repository (void)
{
  {
    // Make sure not to throw exceptions in destructors...
    ACE_WRITE_GUARD (ACE_Lock, ace_mon, *this->lock_);

    for (Service_Type_Map_Iterator service_map_iterator (this->type_map_);
         service_map_iterator.done () == 0;
         service_map_iterator++)
      {
        Type_Info *type_info = (*service_map_iterator).int_id_;
        delete type_info;
      }
  }

  delete this->lock_;
}

CosTradingRepos::ServiceTypeRepository::IncarnationNumber
TAO_Service_Type_Repository::incarnation (void)
{
  CosTradingRepos::ServiceTypeRepository::IncarnationNumber inc_num;

  if (this->lock_->acquire_read () == -1)
    {
      inc_num = this->incarnation_;
      this->lock_->release ();
    }
  else
    {
      inc_num.high = 0;
      inc_num.low = 0;
    }

  return inc_num;
}


CosTradingRepos::ServiceTypeRepository::IncarnationNumber
TAO_Service_Type_Repository::
add_type (const char *name,
          const char *if_name,
          const CosTradingRepos::ServiceTypeRepository::PropStructSeq &props,
          const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super_types)
{
  Prop_Map prop_map;
  Service_Type_Map super_map;

  ACE_WRITE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_, CORBA::INTERNAL ());

  // Make sure Type name is valid.
  if (TAO_Trader_Base::is_valid_identifier_name (name) == 0)
    throw CosTrading::IllegalServiceType (name);

  // Check if the service type already exists.
  CORBA::String_var type_name (name);
  if (this->type_map_.find (type_name) == 0)
    throw CosTradingRepos::ServiceTypeRepository::ServiceTypeExists ();

  // Make sure all property names are valid and appear only once.
  this->validate_properties (prop_map,
                             props);

  // Check that all super_types exist, and none are duplicated.
  this->validate_supertypes (super_map,
                             super_types);

  // NOTE: I don't really know a way to do this without an Interface
  // Repository, since the Interface Repository IDs don't contain
  // information about supertypes.
  //
  // make sure interface name is legal.
  //  this->validate_interface (if_name, super_types);
  //
  // Instead, we do this:
  //
  if (if_name == 0)
    throw CosTradingRepos::ServiceTypeRepository::InterfaceTypeMismatch ();

  // Collect and make sure that properties of all supertypes and this
  // type are compatible.  We can use prop_map and super_types_map for
  // the job.
  this->validate_inheritance (prop_map,
                              super_types);

  // We can now use prop_map to construct a sequence of all properties
  // the this type.
  this->update_type_map (name,
                         if_name,
                         props,
                         super_types,
                         prop_map,
                         super_map);

  CosTradingRepos::ServiceTypeRepository::IncarnationNumber return_value =
    this->incarnation_;

  // Increment incarnation number.
  this->incarnation_.low++;

  // If we wrapped around in lows...
  if (this->incarnation_.low == 0)
    this->incarnation_.high++;

  return return_value;
}

void
TAO_Service_Type_Repository::remove_type (const char *name)
{
  if (TAO_Trader_Base::is_valid_identifier_name (name) == 0)
    throw CosTrading::IllegalServiceType (name);

  ACE_WRITE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_, CORBA::INTERNAL ());

  // Check if the type exists.
  Service_Type_Map::ENTRY* type_entry = 0; ;
  if (this->type_map_.find (name,
                            type_entry) == -1)
    throw CosTrading::UnknownServiceType (name);

  // Check if it has any subtypes.
  Type_Info *type_info = type_entry->int_id_;
  if (type_info->has_subtypes_)
    throw CosTradingRepos::ServiceTypeRepository::HasSubTypes (name, "");

  // Remove the type from the map.
  this->type_map_.unbind (type_entry);
  delete type_info;
}

CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq *
TAO_Service_Type_Repository::
list_types (const CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes &which_types)
{
  ACE_READ_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_, CORBA::INTERNAL ());

  CORBA::ULong const length = static_cast<CORBA::ULong>(this->type_map_.current_size ());
  CORBA::ULong found = 0;
  // Initial allocate a result seq for maximum expected results.
  CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq_var types;
  ACE_NEW_RETURN (types,
                  CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq(length),
                  0);
  types->length(this->type_map_.current_size ());

  bool const all = which_types._d () == CosTradingRepos::ServiceTypeRepository::all;
  // FIXME: && (which_types.all_ () == true) ?

  CosTradingRepos::ServiceTypeRepository::IncarnationNumber num =
    which_types.incarnation ();

  for (Service_Type_Map_Iterator itr (this->type_map_);
       itr.done () == 0;
       itr++)
    {
      Type_Info* type_info = (*itr).int_id_;
      const char* type_name = (*itr).ext_id_.in ();

      if (all
          || num < type_info->type_struct_.incarnation)
        {
          (*types)[found++] = CORBA::string_dup (type_name);
        }
    }

  // Truncate to results we have found.
  types->length(found);
  return types._retn();
}

CosTradingRepos::ServiceTypeRepository::TypeStruct *
TAO_Service_Type_Repository::
describe_type (const char * name)
{
  if (TAO_Trader_Base::is_valid_identifier_name (name) == 0)
    {
      throw CosTrading::IllegalServiceType (name);
    }

  ACE_READ_GUARD_THROW_EX
    (ACE_Lock,
     ace_mon,
     *this->lock_,
     CORBA::INTERNAL ());

  // Make sure the type exists.
  CORBA::String_var type_name (name);
  Service_Type_Map::ENTRY *type_entry = 0;
  if (this->type_map_.find (type_name,
                            type_entry) == -1)
    throw CosTrading::UnknownServiceType (name);

  // Return appropriate information about the type.
  CosTradingRepos::ServiceTypeRepository::TypeStruct *descr = 0;
  ACE_NEW_RETURN (descr,
                  CosTradingRepos::ServiceTypeRepository::TypeStruct,
                  0);
  CosTradingRepos::ServiceTypeRepository::TypeStruct &s =
    type_entry->int_id_->type_struct_;

  descr->if_name = s.if_name;
  descr->masked = s.masked;
  descr->incarnation = s.incarnation;
  descr->super_types = s.super_types;
  descr->props = s.props;
  /*
  CORBA::ULong length = s.props.length ();
  CosTradingRepos::ServiceTypeRepository::PropStruct* pstructs =
    s.props.get_buffer (0);
  descr->props.replace (length, length, pstructs, 0);
  */
  return descr;
}

CosTradingRepos::ServiceTypeRepository::TypeStruct *
TAO_Service_Type_Repository::
fully_describe_type (const char *name)
{
  if (TAO_Trader_Base::is_valid_identifier_name (name) == 0)
    throw CosTrading::IllegalServiceType (name);

  ACE_READ_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_, CORBA::INTERNAL ());

  // Make sure the type exists.
  CORBA::String_var type_name (name);
  Service_Type_Map::ENTRY *type_entry = 0;
  if (this->type_map_.find (type_name,
                            type_entry) == -1)
    throw CosTrading::UnknownServiceType (name);

  // Return appropriate information about the type.
  CosTradingRepos::ServiceTypeRepository::TypeStruct *descr = 0;
  ACE_NEW_RETURN (descr,
                  CosTradingRepos::ServiceTypeRepository::TypeStruct,
                  0);
  CosTradingRepos::ServiceTypeRepository::TypeStruct &s =
    type_entry->int_id_->type_struct_;

  // Aggregate the Properties of this type and all its supertypes.
  // Compute the transitive closure of all supertypes.
  this->fully_describe_type_i (s,
                               descr->props,
                               descr->super_types);

  // We do the explicit copy, since otherwise we'd have excessive
  // properties copying.
  descr->if_name = s.if_name;
  descr->masked = s.masked;
  descr->incarnation = s.incarnation;

  return descr;
}

void
TAO_Service_Type_Repository::
mask_type (const char *name)
{
  if (TAO_Trader_Base::is_valid_identifier_name (name) == 0)
    throw CosTrading::IllegalServiceType (name);

  ACE_WRITE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_, CORBA::INTERNAL ());

  // Make sure the type exists.
  CORBA::String_var type_name (name);
  Service_Type_Map::ENTRY *type_entry = 0;
  if (this->type_map_.find (type_name,
                            type_entry) != -1)
    throw CosTrading::UnknownServiceType (name);

  // Make sure the type is unmasked.
  CORBA::Boolean &mask =
    type_entry->int_id_->type_struct_.masked;

  if (mask == 1)
    throw CosTradingRepos::ServiceTypeRepository::AlreadyMasked (name);
  else
    mask = 1;
}

void
TAO_Service_Type_Repository::unmask_type (const char *name)
{
  if (TAO_Trader_Base::is_valid_identifier_name (name) == 0)
    throw CosTrading::IllegalServiceType (name);

  ACE_WRITE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_, CORBA::INTERNAL ());

  // Make sure the type exists.
  CORBA::String_var type_name (name);
  Service_Type_Map::ENTRY *type_entry = 0;
  if (this->type_map_.find (type_name,
                            type_entry) != -1)
    throw CosTrading::UnknownServiceType (name);

  // Make sure the type is masked.
  CORBA::Boolean &mask = type_entry->int_id_->type_struct_.masked;

  if (mask == 0)
    throw CosTradingRepos::ServiceTypeRepository::NotMasked (name);
  else
    mask = 0;
}

void
TAO_Service_Type_Repository::
fully_describe_type_i (const CosTradingRepos::ServiceTypeRepository::TypeStruct &type_struct,
                       CosTradingRepos::ServiceTypeRepository::PropStructSeq &props,
                       CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super_types)
{
  TAO_String_Queue service_type_queue;
  this->collect_inheritance_hierarchy (type_struct,
                                       service_type_queue);

  // Count the total number of properties.
  CORBA::ULong num_props = 0;
  CORBA::ULong num_types = static_cast<CORBA::ULong> (service_type_queue.size ());

  TAO_String_Queue::ITERATOR iterator (service_type_queue);

  for (;
       iterator.done () == 0;
       iterator.advance ())
    {
      char **next_type_name = 0;
      Service_Type_Map::ENTRY *type_entry = 0;

      iterator.next (next_type_name);
      CORBA::String_var hash_key (const_cast<const char *> (*next_type_name));
      this->type_map_.find (hash_key,
                            type_entry);

      CosTradingRepos::ServiceTypeRepository::TypeStruct &tstruct =
        type_entry->int_id_->type_struct_;
      num_props += tstruct.props.length ();
    }

  num_props += type_struct.props.length ();
  props.length (num_props);
  super_types.length (num_types);

  // Copy in all properties.
  int i = 0;
  CORBA::ULong prop_index = 0;
  CORBA::ULong type_index = 0;

  for (i = type_struct.props.length () - 1;
       i >= 0;
       i--)
    props[prop_index++] = type_struct.props[i];

  for (iterator.first ();
       iterator.done () == 0;
       iterator.advance ())
    {
      char **next_type_name = 0;
      Service_Type_Map::ENTRY *type_entry = 0;

      iterator.next (next_type_name);
      CORBA::String_var hash_key (const_cast<const char *> (*next_type_name));
      this->type_map_.find (hash_key,
                            type_entry);

      // Should never be zero.
      if (type_entry != 0)
        {
          CosTradingRepos::ServiceTypeRepository::TypeStruct& tstruct =
            type_entry->int_id_->type_struct_;

          for (i = tstruct.props.length () - 1;
               i >= 0;
               i--)
            props[prop_index++] = tstruct.props[i];

          super_types[type_index++] = hash_key.in ();
        }
    }
}

void
TAO_Service_Type_Repository::
collect_inheritance_hierarchy (const CosTradingRepos::ServiceTypeRepository::TypeStruct &type_struct,
                               TAO_String_Queue &target)
{
  // Recurse for each super_type
  for (int i = type_struct.super_types.length () - 1;
       i >= 0;
       i--)
    {
      Service_Type_Map::ENTRY *next_type_entry = 0;
      CORBA::String_var next_type_name (type_struct.super_types[i]);

      if (this->type_map_.find (next_type_name, next_type_entry) != -1)
        {
          CosTradingRepos::ServiceTypeRepository::TypeStruct &next_type_struct =
            next_type_entry->int_id_->type_struct_;

          const char *type_name =
            type_struct.super_types[i];
          target.enqueue_tail (const_cast<char *> (type_name));

          this->collect_inheritance_hierarchy (next_type_struct,
                                               target);
        }
    }
}

void
TAO_Service_Type_Repository::
validate_properties (Prop_Map &prop_map,
                     const CosTradingRepos::ServiceTypeRepository::PropStructSeq &props)
{
  for (CORBA::ULong i = 0;
       i < props.length ();
       i++)
    {
      const char *n = props[i].name;
      if (TAO_Trader_Base::is_valid_property_name (n) == 0)
        throw CosTrading::IllegalPropertyName (n);
      else
        {
          CORBA::String_var prop_name (n);
          CosTradingRepos::ServiceTypeRepository::PropStruct *prop_val =
            const_cast<CosTradingRepos::ServiceTypeRepository::PropStruct *> (&props[i]);

          if (prop_map.bind (prop_name,
                             prop_val) == 1)
            throw CosTrading::DuplicatePropertyName (n);
        }
    }
}

void
TAO_Service_Type_Repository::
validate_supertypes (Service_Type_Map &super_map,
                     const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super_types)
{
  for (CORBA::ULong i = 0;
       i < super_types.length ();
       i++)
    {
      const char *type =
        super_types[i];

      if (TAO_Trader_Base::is_valid_identifier_name (type) == 0)
        throw CosTrading::IllegalServiceType (type);
      else
        {
          CORBA::String_var hash_type (type);
          Service_Type_Map::ENTRY *type_entry = 0;

          if (this->type_map_.find (hash_type,
                                    type_entry) == -1)
            throw CosTrading::UnknownServiceType (type);
          else
            {
              if (super_map.bind (hash_type,
                                  type_entry->int_id_) == 1)
                throw CosTradingRepos::ServiceTypeRepository::DuplicateServiceTypeName (
                  type);
            }
        }
    }
}

void
TAO_Service_Type_Repository::
validate_inheritance (Prop_Map &prop_map,
                      const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super_types)
{
  CORBA::ULong num_super_types = super_types.length ();

  for (CORBA::ULong i = 0;
       i < num_super_types;
       i++)
    {
      Service_Type_Map::ENTRY *super_type_entry = 0;
      CORBA::String_var super_type (super_types[i]);
      CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq place_holder;
      CosTradingRepos::ServiceTypeRepository::PropStructSeq super_props;

      this->type_map_.find (super_type, super_type_entry);

      // Super_type_entry should never be zero.
      if (super_type_entry != 0)
        this->fully_describe_type_i (super_type_entry->int_id_->type_struct_,
                                     super_props,
                                     place_holder);
      else
        continue;

      CORBA::ULong num_props = super_props.length ();

      for (CORBA::ULong j = 0;
           j < num_props;
           j++)
        {
          Prop_Map::ENTRY *existing_prop = 0;
          CORBA::String_var prop_name (super_props[j].name);

          if (prop_map.bind (prop_name,
                             &super_props[j],
                             existing_prop) == 1)
            {
              // If already there, check that it is compatible with
              // properties of other types. Value Types have to be the
              // same.
              const CosTradingRepos::ServiceTypeRepository::PropStruct &property_in_map =
                *existing_prop->int_id_;

              CORBA::TypeCode_ptr prop_type = property_in_map.value_type.in ();
              int compare = 0;
              try
                {
                  compare =
                    super_props[j].value_type->equal (prop_type);
                }
              catch (const CORBA::Exception&)
                {
                throw CosTradingRepos::ServiceTypeRepository::ValueTypeRedefinition(
                  super_props[j].name,
                  super_props[j],
                  property_in_map.name,
                  property_in_map);
                }

              if (compare == 0
                  || super_props[j].mode > property_in_map.mode)
                throw CosTradingRepos::ServiceTypeRepository::ValueTypeRedefinition(
                  super_props[j].name,
                  super_props[j],
                  property_in_map.name,
                  property_in_map);
            }
        }
    }
}

void
TAO_Service_Type_Repository::
update_type_map (const char *name,
                 const char *if_name,
                 const CosTradingRepos::ServiceTypeRepository::PropStructSeq &props,
                 const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super_types,
                 Prop_Map &,
                 Service_Type_Map &super_map)
{
  // Update entries for all supertypes to include this type as a
  // subtype.  we can use the super_types_map we have constructed.

  for (Service_Type_Map_Iterator super_map_iterator (super_map);
       super_map_iterator.done () == 0;
       super_map_iterator++)
    {
      Type_Info *super_type_info =
        (*super_map_iterator).int_id_;
      super_type_info->has_subtypes_ = 0;
    }

  // All parameters are valid, create an entry for this service type
  // in the this->type_map_.
  Type_Info *type = 0;
  ACE_NEW (type,
           Type_Info);

  type->type_struct_.props = props;
  type->type_struct_.if_name = if_name;
  type->type_struct_.super_types = super_types;
  type->type_struct_.incarnation = this->incarnation_;
  type->type_struct_.masked = 0;
  type->has_subtypes_ = 0;

  // Move the prop struct sequences and super type names from the in
  // params to the internal storage.
  /*
  CORBA::ULong pslength = props.length ();
  CosTradingRepos::ServiceTypeRepository::PropStructSeq* pstructs =
    const_cast<CosTradingRepos::ServiceTypeRepository::PropStructSeq*> (&props);
  CosTradingRepos::ServiceTypeRepository::PropStruct* psbuf =
    pstructs->get_buffer (1);
  type->type_struct_.props.replace (pslength,
                                    pslength,
                                    psbuf,
                                    1);
  */
  CORBA::String_var type_name (name);
  this->type_map_.bind (type_name, type);
}

TAO_END_VERSIONED_NAMESPACE_DECL
