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
  this->incarnation_.low = 0;
  this->incarnation_.high = 0;

  // If a lock wasn't provided, let's assume the user doesn't want any
  // kind of lock at all.
  if (this->lock_ == 0)
    ACE_NEW (this->lock_, ACE_Lock_Adapter<ACE_Null_Mutex> ());
}


TAO_Service_Type_Repository::~TAO_Service_Type_Repository (void)
{
  {
    ACE_WRITE_GUARD (ACE_Lock, ace_mon, *this->lock_);
    
    for (Service_Type_Map::iterator service_map_iterator (this->type_map_);
         ! service_map_iterator.done ();
         service_map_iterator++)
      {
        Type_Info* type_info = (*service_map_iterator).int_id_;
        delete type_info;
      }
  }

  delete this->lock_;
}


CosTradingRepos::ServiceTypeRepository::IncarnationNumber 
TAO_Service_Type_Repository::
incarnation (CORBA::Environment& _env)
{
  CosTradingRepos::ServiceTypeRepository::IncarnationNumber inc_num;
  TAO_READ_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, inc_num);
  return this->incarnation_;
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
  Service_Type_Map super_map;
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
TAO_Service_Type_Repository::remove_type (const char * name,
                                          CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
                   CosTrading::IllegalServiceType, 
                   CosTrading::UnknownServiceType, 
                   CosTradingRepos::ServiceTypeRepository::HasSubTypes))
{
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::IllegalServiceType (name));

  TAO_WRITE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  // Check if the type exists.
  Service_Type_Map::ENTRY* type_entry = 0; ;
  if (this->type_map_.find (name, type_entry) == -1)
    TAO_THROW (CosTrading::UnknownServiceType (name));

  // Check if it has any subtypes.
  Type_Info* type_info = type_entry->int_id_;
  if (type_info->has_subtypes_)
    TAO_THROW (CosTradingRepos::ServiceTypeRepository::HasSubTypes (name, ""));

  // remove the type from the map.
  this->type_map_.unbind (type_entry);
  delete type_info;
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
      Type_Info* type_info = (*itr).int_id_;
      const char* type_name = (const char*) (*itr).ext_id_;

      if (num < type_info->type_struct_.incarnation)
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
  CosTradingRepos::ServiceTypeRepository::TypeStruct* descr = 0;
  ACE_NEW_RETURN (descr, CosTradingRepos::ServiceTypeRepository::TypeStruct, 0);
  CosTradingRepos::ServiceTypeRepository::TypeStruct & s =
    type_entry->int_id_->type_struct_;

  descr->if_name = s.if_name;
  descr->masked = s.masked;
  descr->incarnation = s.incarnation;
  descr->super_types = s.super_types;

  CORBA::ULong length = s.props.length ();
  CosTradingRepos::ServiceTypeRepository::PropStruct* pstructs =
    s.props.get_buffer (CORBA::B_FALSE);
  descr->props.replace (length, length, pstructs, CORBA::B_FALSE);
  
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
  CosTradingRepos::ServiceTypeRepository::TypeStruct* descr = 0;
  ACE_NEW_RETURN (descr, CosTradingRepos::ServiceTypeRepository::TypeStruct, 0);
  CosTradingRepos::ServiceTypeRepository::TypeStruct & s =
    type_entry->int_id_->type_struct_;
  
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
  CORBA::Boolean& mask = type_entry->int_id_->type_struct_.masked;
  
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
  CORBA::Boolean& mask = type_entry->int_id_->type_struct_.masked;  
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
  Service_Type_Queue service_type_queue;
  this->collect_inheritance_hierarchy (type_struct, service_type_queue);

  // Count the total number of properties.
  CORBA::ULong num_props = 0,
    num_types = service_type_queue.size ();
  Service_Type_Queue::ITERATOR iterator (service_type_queue);
  for (; ! iterator.done (); iterator.advance ())
    {
      const char** next_type_name = 0;
      Service_Type_Map::ENTRY* type_entry = 0;

      iterator.next (next_type_name);
      TAO_String_Hash_Key hash_key (*next_type_name);
      this->type_map_.find (*next_type_name, type_entry);

      CosTradingRepos::ServiceTypeRepository::TypeStruct& tstruct =
        type_entry->int_id_->type_struct_;
      num_props += tstruct.props.length ();
    }

  num_props += type_struct.props.length ();
  CosTradingRepos::ServiceTypeRepository::PropStruct* pstructs =
    CosTradingRepos::ServiceTypeRepository::PropStructSeq::allocbuf (num_props);
  super_types.length (num_types);

  if (pstructs != 0)
    {
      // Copy in all properties.
      int i = 0;
      CORBA::ULong prop_index = 0,
        type_index = 0;  
      for (i = type_struct.props.length () - 1; i >= 0; i--)
        pstructs[prop_index++] = type_struct.props[i];
      
      iterator.first ();
      for (; ! iterator.done (); iterator.advance ())
        {
          const char** next_type_name = 0;
          Service_Type_Map::ENTRY* type_entry = 0;

          iterator.next (next_type_name);
          TAO_String_Hash_Key hash_key (*next_type_name);
          this->type_map_.find (*next_type_name, type_entry);
          
          CosTradingRepos::ServiceTypeRepository::TypeStruct& tstruct =
            type_entry->int_id_->type_struct_;
          for (i = tstruct.props.length () - 1; i >= 0; i--)
            pstructs[prop_index++] = tstruct.props[i];
          
          super_types[type_index++] = *next_type_name;
        }

      props.replace (num_props, num_props, pstructs, CORBA::B_TRUE);
    }
}

void
TAO_Service_Type_Repository::
collect_inheritance_hierarchy (const CosTradingRepos::ServiceTypeRepository::TypeStruct& type_struct,
                               TAO_Service_Type_Repository::Service_Type_Queue& target)
{
  // Recurse for each super_type
  for (int i = type_struct.super_types.length () - 1; i >= 0; i--)
    {
      Service_Type_Map::ENTRY* next_type_entry = 0;
      TAO_String_Hash_Key next_type_name (type_struct.super_types[i]);
      this->type_map_.find (next_type_name, next_type_entry);
      
      CosTradingRepos::ServiceTypeRepository::TypeStruct&
	next_type_struct = next_type_entry->int_id_->type_struct_;
      target.enqueue_tail (type_struct.super_types[i]);
      
      this->collect_inheritance_hierarchy (next_type_struct, target);
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
	  CosTradingRepos::ServiceTypeRepository::PropStruct* prop_val =
	    (CosTradingRepos::ServiceTypeRepository::PropStruct*) &props[i];

	  if (prop_map.bind (prop_name, prop_val) == 1)
	    TAO_THROW (CosTrading::DuplicatePropertyName (n));
	}
    }
}

void
TAO_Service_Type_Repository::
validate_supertypes (Service_Type_Map& super_map,
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
	    if (super_map.bind (s_type, type_entry->int_id_) == 1)
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
  
  for (CORBA::ULong i = 0; i < num_super_types; i++)
    {
      Service_Type_Map::ENTRY* super_type_entry = 0;
      TAO_String_Hash_Key super_type (super_types[i]);
      CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq place_holder;
      CosTradingRepos::ServiceTypeRepository::PropStructSeq super_props;

      this->type_map_.find (super_type, super_type_entry);      
      this->fully_describe_type_i (super_type_entry->int_id_->type_struct_,
				   super_props,
				   place_holder);

      CORBA::ULong num_props = super_props.length ();
      for (CORBA::ULong j = 0; j < num_props; j++)
	{
	  Prop_Map::ENTRY* existing_prop = 0;
	  TAO_String_Hash_Key prop_name (super_props[j].name);
	  
	  if (prop_map.bind (prop_name, &super_props[j], existing_prop) == 1)
	    {
	      // if already there, check that it is compatible with
	      // properties of other types. Value Types have to be the
	      // same. 
	      const CosTradingRepos::ServiceTypeRepository::PropStruct&
		property_in_map = *existing_prop->int_id_;
	      
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
		 Service_Type_Map& super_map)
{
  Type_Info* type = 0;
  ACE_NEW (type, Type_Info);
  // update entries for all supertypes to include this type as a subtype. 
  // we can use the super_types_map we have constructed.

  TAO_String_Hash_Key type_name (name);
  for (Service_Type_Map::iterator super_map_iterator (super_map);
       ! super_map_iterator.done ();
       super_map_iterator++)
    {
      Type_Info* super_type_info = (*super_map_iterator).int_id_;
      super_type_info->has_subtypes_ = CORBA::B_FALSE;
    }

  // all parameters are valid, create an entry for this service type
  // in the this->type_map_. 
  type->type_struct_.if_name = if_name;
  type->type_struct_.masked = CORBA::B_FALSE;
  type->type_struct_.incarnation = this->incarnation_;
  type->has_subtypes_ = CORBA::B_FALSE;
  type->type_struct_.super_types = super_types;
  
  // Move the prop struct sequences and super type names from the in
  // params to the internal storage.  
  CORBA::ULong pslength = props.length ();
  CosTradingRepos::ServiceTypeRepository::PropStructSeq* pstructs = 
    ACE_const_cast (CosTradingRepos::ServiceTypeRepository::PropStructSeq*,
                    &props);
  CosTradingRepos::ServiceTypeRepository::PropStruct* psbuf =
    pstructs->get_buffer (CORBA::B_TRUE);
  type->type_struct_.props.replace (pslength,
                                    pslength,
                                    psbuf,
                                    CORBA::B_TRUE);;
  
  this->type_map_.bind (type_name, type);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key, CosTradingRepos::ServiceTypeRepository::PropStruct*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key, TAO_Service_Type_Repository::Type_Info*, ACE_Null_Mutex>;
template class ACE_Unbounded_Queue<char*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key, CosTradingRepos::ServiceTypeRepository::PropStruct*, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<TAO_String_Hash_Key, TAO_Service_Type_Repository::Type_Info*, ACE_Null_Mutex>
#pragma instantiate ACE_Unbounded_Queue<char*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
