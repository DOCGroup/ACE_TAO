/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   Service_Type_Repository.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#include "Locking.h"
#include "Service_Type_Repository.h"

TAO_Service_Type_Repository::
TAO_Service_Type_Repository (ACE_Lock* lock)
  : lock_ (lock)
{
  incarnation_.low = 0;
  incarnation_.high = 0;

  // If a lock wasn't provided, let's assume the user doesn't want any
  // kind of lock at all.
  if (this->lock_ == 0)
    ACE_NEW (this->lock_, ACE_Lock_Adapter<ACE_Null_Mutex> ());
}


TAO_Service_Type_Repository::
~TAO_Service_Type_Repository (void)
{
  delete this->lock_;
}


CosTradingRepos::ServiceTypeRepository::IncarnationNumber 
TAO_Service_Type_Repository::
incarnation (CORBA::Environment& _env)
{
  CosTradingRepos::ServiceTypeRepository::IncarnationNumber inc_num;
  TAO_READ_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, inc_num);
  return incarnation_;
}


CosTradingRepos::ServiceTypeRepository::IncarnationNumber 
TAO_Service_Type_Repository::
add_type (const char * name, 
	  const char * if_name, 
	  const CosTradingRepos::ServiceTypeRepository::PropStructSeq& props, 
	  const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq& super_types,
	  CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType, 
		   CosTradingRepos::ServiceTypeRepository::ServiceTypeExists, 
		   CosTradingRepos::ServiceTypeRepository::InterfaceTypeMismatch, 
		   CosTrading::IllegalPropertyName, 
		   CosTrading::DuplicatePropertyName, 
		   CosTradingRepos::ServiceTypeRepository::ValueTypeRedefinition, 
		   CosTrading::UnknownServiceType, 
		   CosTradingRepos::ServiceTypeRepository::DuplicateServiceTypeName))
{
  Prop_Map prop_map;
  Super_Type_Map super_map;
  CosTradingRepos::ServiceTypeRepository::IncarnationNumber inc_num;
  
  TAO_WRITE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, inc_num);

  // make sure Type name is valid
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW_RETURN (CosTrading::IllegalServiceType (name),
		      this->incarnation_);

  // check if the service type already exists.
  TAO_String_Hash_Key type_name (name);
  if (this->type_map_.find (type_name) == 0)
    TAO_THROW_RETURN (CosTradingRepos::ServiceTypeRepository::ServiceTypeExists (),
		      this->incarnation_);
  
  // make sure all property names are valid and appear only once.
  this->validate_properties (prop_map, props, _env);
  TAO_CHECK_ENV_RETURN (_env, this->incarnation_);
  
  // check that all super_types exist, and none are duplicated.
  this->validate_supertypes (super_map, super_types, _env);
  TAO_CHECK_ENV_RETURN (_env, this->incarnation_);

  // NOTE: I don't really know a way to do this without an Interface
  // Repository, since the Interface Repository IDs don't contain
  // information about supertypes.
  // 
  // make sure interface name is legal.
  //  this->validate_interface (if_name, super_types, _env);
  //  TAO_CHECK_ENV_RETURN(_env, this->incarnation);
  //
  // Instead, we do this:
  // 
  if (if_name == 0)
    TAO_THROW_RETURN (CosTradingRepos::ServiceTypeRepository::InterfaceTypeMismatch (),
		      this->incarnation_);
  
  // collect and make sure that properties of all supertypes and this type
  // are compatible.  We can use prop_map and super_types_map for the job.
  this->validate_inheritance (prop_map, super_types, _env);
  TAO_CHECK_ENV_RETURN (_env, this->incarnation_);
  
  // we can now use prop_map to construct a sequence of all properties the
  // this type.
  this->update_type_map (name,
			 if_name,
			 props,
			 super_types,
			 prop_map,
			 super_map);

  CosTradingRepos::ServiceTypeRepository::IncarnationNumber return_value =
    this->incarnation_;
  
  // increment incarnation number
  this->incarnation_.low++;
  // if we wrapped around in lows...
  if (this->incarnation_.low == 0)
    this->incarnation_.high++;

  return return_value;
}


void 
TAO_Service_Type_Repository::
remove_type (const char * name,
	     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  CosTradingRepos::ServiceTypeRepository::HasSubTypes))
{
  ACE_DEBUG ((LM_DEBUG, "TAO_Service_Type_Repository::remove_type (%s)\n",
	      name));
	      
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::IllegalServiceType (name));

  TAO_WRITE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  // Check if the type exists.
  Service_Type_Map::ENTRY* type_entry = 0; ;
  if (this->type_map_.find (name, type_entry) == -1)
    TAO_THROW (CosTrading::UnknownServiceType (name));

  // Check if it has any subtypes.
  Type_Info& type_info = type_entry->int_id_;
  if (type_info.sub_types_.is_empty () == 0)
    {
      Type_Info::Type_List::iterator type_iter = type_info.sub_types_.begin ();
      TAO_String_Hash_Key type_name = *type_iter;
      
      TAO_THROW (CosTradingRepos::ServiceTypeRepository::HasSubTypes
		 (name, (const char*) type_name));
    }

  // remove this type from an entry of each of its supertypes.
  CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super =
    type_info.type_struct_.super_types;

  for (CORBA::ULong i = 0; i < super.length (); i++)
    {
      // find the super type
      Service_Type_Map::ENTRY* super_entry = 0;
      TAO_String_Hash_Key super_type (super[i]);
      this->type_map_.find (super_type, super_entry);
      
      // remove this type from its list of subtypes
      TAO_String_Hash_Key type_name (name);
      Type_Info& type_info = super_entry->int_id_;
      type_info.sub_types_.remove (type_name);
    }

  // remove the type from the map.
  this->type_map_.unbind (type_entry);
}
           
CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq* 
TAO_Service_Type_Repository::
list_types (const CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes& which_types,
	    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  TAO_READ_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  CORBA::ULong i = 0;
  CORBA::ULong length = this->type_map_.current_size ();
  CosTrading::ServiceTypeName* types =
    CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq::allocbuf (length);
  CosTradingRepos::ServiceTypeRepository::IncarnationNumber num;
  
  if (types == 0)
    return 0;
  
  if (which_types._d () == CosTradingRepos::ServiceTypeRepository::all)
    {
      num.high = 0;
      num.low = 0;
    }
  else
    num = which_types.incarnation ();

  for (Service_Type_Map::iterator itr (this->type_map_);
       ! itr.done ();
       itr++)
    {
      Type_Info& type_info = (*itr).int_id_;
      const char* type_name = (const char*) (*itr).ext_id_;

      if (num < type_info.type_struct_.incarnation)
	types[i++] = CORBA::string_dup (type_name);
    }
  
  return new CosTradingRepos::ServiceTypeRepository::
    ServiceTypeNameSeq (length, i, types, CORBA::B_TRUE);
}



CosTradingRepos::ServiceTypeRepository::TypeStruct*
TAO_Service_Type_Repository::
describe_type (const char * name,
	       CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType, 
		   CosTrading::UnknownServiceType))
{
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW_RETURN
      (CosTrading::IllegalServiceType (name),
       (CosTradingRepos::ServiceTypeRepository::TypeStruct*) 0);
  
  TAO_READ_GUARD_RETURN
    (ACE_Lock, 
     ace_mon, 
     *this->lock_,
     (CosTradingRepos::ServiceTypeRepository::TypeStruct*) 0);
  
  // make sure the type exists.
  TAO_String_Hash_Key type_name (name);
  Service_Type_Map::ENTRY* type_entry = 0;
  if (this->type_map_.find (type_name, type_entry) == -1)
    TAO_THROW_RETURN (CosTrading::UnknownServiceType (name),
		      (CosTradingRepos::ServiceTypeRepository::TypeStruct*) 0);
  
  // return appropriate information about the type.
  CosTradingRepos::ServiceTypeRepository::TypeStruct* descr =
    new CosTradingRepos::ServiceTypeRepository::TypeStruct;
  CosTradingRepos::ServiceTypeRepository::TypeStruct & s =
    type_entry->int_id_.type_struct_;
  
  (*descr) = s;
  
  return descr;
}


CosTradingRepos::ServiceTypeRepository::TypeStruct* 
TAO_Service_Type_Repository::
fully_describe_type (const char * name,
		     CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType, 
		   CosTrading::UnknownServiceType))
{
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW_RETURN
      (CosTrading::IllegalServiceType (name),
       (CosTradingRepos::ServiceTypeRepository::TypeStruct*) 0);
  
  TAO_READ_GUARD_RETURN (ACE_Lock, 
			 ace_mon, 
			 *this->lock_,
			 (CosTradingRepos::ServiceTypeRepository::TypeStruct*) 0);
  
  // make sure the type exists.
  TAO_String_Hash_Key type_name (name);
  Service_Type_Map::ENTRY* type_entry = 0;
  if (this->type_map_.find (type_name, type_entry) == -1)
    TAO_THROW_RETURN (CosTrading::UnknownServiceType (name),
		      (CosTradingRepos::ServiceTypeRepository::TypeStruct*) 0);
  
  // return appropriate information about the type.
  CosTradingRepos::ServiceTypeRepository::TypeStruct* descr =
    new CosTradingRepos::ServiceTypeRepository::TypeStruct;
  CosTradingRepos::ServiceTypeRepository::TypeStruct & s =
    type_entry->int_id_.type_struct_;
  
  // Aggregate the Properties of this type and all its supertypes.
  // Computer the transitive closure of all supertypes.
  this->fully_describe_type_i (s, descr->props, descr->super_types);
  
  // We do the explicit copy, since otherwise we'd have excessive
  // properties copying.
  descr->if_name = s.if_name;
  descr->masked = s.masked; 
  descr->incarnation = s.incarnation; 
  
  return descr;  
}


void 
TAO_Service_Type_Repository::
mask_type (const char * name,
	   CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		   CosTrading::IllegalServiceType, 
		   CosTrading::UnknownServiceType, 
		   CosTradingRepos::ServiceTypeRepository::AlreadyMasked))
{
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::IllegalServiceType (name));

  TAO_WRITE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  // make sure the type exists.
  TAO_String_Hash_Key type_name (name);
  Service_Type_Map::ENTRY* type_entry = 0;
  if (this->type_map_.find (type_name, type_entry) != -1)
    TAO_THROW (CosTrading::UnknownServiceType (name));

  // make sure the type is unmasked.
  CORBA::Boolean& mask = type_entry->int_id_.type_struct_.masked;
  
  if (mask == CORBA::B_TRUE)
    TAO_THROW (CosTradingRepos::ServiceTypeRepository::AlreadyMasked (name));
  else
    mask = CORBA::B_TRUE;
}
       

void 
TAO_Service_Type_Repository::
unmask_type (const char * name,
	     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  CosTradingRepos::ServiceTypeRepository::NotMasked))
{
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::IllegalServiceType (name));

  TAO_WRITE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  // make sure the type exists.
  TAO_String_Hash_Key type_name (name);
  Service_Type_Map::ENTRY* type_entry = 0;
  if (this->type_map_.find (type_name, type_entry) != -1)
    TAO_THROW (CosTrading::UnknownServiceType (name));
  
  // make sure the type is masked.
  CORBA::Boolean& mask = type_entry->int_id_.type_struct_.masked;  
  if (mask == CORBA::B_FALSE)
    TAO_THROW (CosTradingRepos::ServiceTypeRepository::AlreadyMasked (name));
  else
    mask = CORBA::B_FALSE;
}

void
TAO_Service_Type_Repository::
fully_describe_type_i (const CosTradingRepos::ServiceTypeRepository::TypeStruct& type_struct,
		       CosTradingRepos::ServiceTypeRepository::PropStructSeq& props,
		       CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq& super_types)
{   
  // Insert our own properties first.
  CORBA::ULong prop_index_begin = props.length ();
  CORBA::ULong num_properties = type_struct.props.length ();

  props.length (prop_index_begin + num_properties);
  for (int i = 0; i < num_properties; i++)
    props[prop_index_begin++] = type_struct.props[i];

  // Insert the names of our own supertypes next
  CORBA::ULong super_types_index_begin = super_types.length ();
  CORBA::ULong num_super_types = type_struct.super_types.length ();

  super_types.length (super_types_index_begin + num_super_types);
  for (i = 0; i < num_super_types; i++)
    super_types[super_types_index_begin++] = type_struct.super_types[i];

  // Recurse for each super_type
  for (i = 0; i < num_super_types; i++)
    {
      Service_Type_Map::ENTRY* next_type_entry = 0;
      TAO_String_Hash_Key next_type_name (type_struct.super_types[i]);
      this->type_map_.find (next_type_name, next_type_entry);
      
      CosTradingRepos::ServiceTypeRepository::TypeStruct&
	next_type_struct = next_type_entry->int_id_.type_struct_;
      
      this->fully_describe_type_i (next_type_struct, props, super_types);
    }
}


void
TAO_Service_Type_Repository::
validate_properties (Prop_Map& prop_map,
		     const CosTradingRepos::ServiceTypeRepository::PropStructSeq& props,
		     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalPropertyName,
		   CosTrading::DuplicatePropertyName))
{
  for (CORBA::ULong i = 0; i < props.length (); i++)
    {
      const char* n = props[i].name;
      if (! TAO_Trader_Base::is_valid_identifier_name (n))
	TAO_THROW (CosTrading::IllegalPropertyName (n));
      else
	{
	  TAO_String_Hash_Key prop_name (n);
	  const CosTradingRepos::ServiceTypeRepository::PropStruct&
	    prop_struct = props[i];

	  if (prop_map.bind (prop_name, prop_struct) == 1)
	    TAO_THROW (CosTrading::DuplicatePropertyName (n));
	}
    }
}

void
TAO_Service_Type_Repository::
validate_supertypes (Super_Type_Map& super_map,
		     const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq& super_types,
		     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalServiceType,
		  CosTrading::UnknownServiceType,
		  CosTrading::DuplicatePropertyName))
{
  for (CORBA::ULong i = 0; i < super_types.length (); i++)
    {
      const char* type = (const char*) super_types[i];
      
      if (! TAO_Trader_Base::is_valid_identifier_name (type))
	TAO_THROW (CosTrading::IllegalServiceType (type));
      else
	{
	  TAO_String_Hash_Key s_type (type);
	  Service_Type_Map::ENTRY* type_entry = 0;
	  if (this->type_map_.find (s_type, type_entry) == -1)
	    TAO_THROW (CosTrading::UnknownServiceType (type));
	  else
	    if (super_map.bind (s_type, &(type_entry->int_id_)) == 1)
	      TAO_THROW (CosTradingRepos::ServiceTypeRepository::DuplicateServiceTypeName (type));
	}
    }
}

void
TAO_Service_Type_Repository::
validate_inheritance (Prop_Map& prop_map,
		      const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq& super_types,
		      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTradingRepos::ServiceTypeRepository::ValueTypeRedefinition))
{
  CORBA::ULong num_super_types = super_types.length ();
  
  for (int i = 0; i < num_super_types; i++)
    {
      Service_Type_Map::ENTRY* super_type_entry = 0;
      TAO_String_Hash_Key super_type (super_types[i]);
      CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq place_holder;
      CosTradingRepos::ServiceTypeRepository::PropStructSeq super_props;

      this->type_map_.find (super_type, super_type_entry);      
      this->fully_describe_type_i (super_type_entry->int_id_.type_struct_,
				   super_props,
				   place_holder);

      CORBA::ULong num_props = super_props.length ();
      for (int j = 0; j < num_props; j++)
	{
	  Prop_Map::ENTRY* existing_prop = 0;
	  TAO_String_Hash_Key prop_name (super_props[j].name);
	  
	  if (prop_map.bind (prop_name, super_props[j], existing_prop) == 1)
	    {
	      // if already there, check that it is compatible with
	      // properties of other types. Value Types have to be the
	      // same. 
	      const CosTradingRepos::ServiceTypeRepository::PropStruct&
		property_in_map = existing_prop->int_id_;
	      
	      if (! super_props[j].value_type->equal (property_in_map.value_type, _env))
		{
		  TAO_THROW (CosTradingRepos::ServiceTypeRepository::ValueTypeRedefinition
			     (super_props[j].name,
			      super_props[j],
			      property_in_map.name,
			      property_in_map));
		}	      	      
	    }	  
	}
    }
}
      
void
TAO_Service_Type_Repository::
update_type_map (const char* name,
		 const char * if_name, 
		 const CosTradingRepos::ServiceTypeRepository::PropStructSeq& props,
		 const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq& super_types,
		 Prop_Map& prop_map,
		 Super_Type_Map& super_map)
{
  Type_Info type;
  // update entries for all supertypes to include this type as a subtype. 
  // we can use the super_types_map we have constructed.

  TAO_String_Hash_Key type_name (name);
  for (Super_Type_Map::iterator super_map_iterator (super_map);
       ! super_map_iterator.done ();
       super_map_iterator++)
    {
      Type_Info* super_type_info = (*super_map_iterator).int_id_;
      super_type_info->sub_types_.insert (type_name);
    }

  // all parameters are valid, create an entry for this service type
  // in the this->type_map_. 
  type.type_struct_.if_name = if_name;
  type.type_struct_.props = props;
  type.type_struct_.super_types = super_types;
  type.type_struct_.masked = CORBA::B_FALSE;
  type.type_struct_.incarnation = this->incarnation_;
  
  this->type_map_.bind (type_name, type);
}
