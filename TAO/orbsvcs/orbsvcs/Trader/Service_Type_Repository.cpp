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

#if !defined (TAO_SERVICE_TYPE_REPOSITORY_C)
#define TAO_SERVICE_TYPE_REPOSITORY_C

#include "Locking.h"
#include "Service_Type_Repository.h"

template <class MAP_LOCK_TYPE>
const char* TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
NAME = "Type_Repository";

template <class MAP_LOCK_TYPE>
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
TAO_Service_Type_Repository (void)
{
  incarnation_.low = 0;
  incarnation_.high = 0;
}

template <class MAP_LOCK_TYPE>
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
~TAO_Service_Type_Repository (void)
{
}

template <class MAP_LOCK_TYPE>
SERVICE_TYPE_REPOS::IncarnationNumber 
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
incarnation (CORBA::Environment& _env)
{
  SERVICE_TYPE_REPOS::IncarnationNumber inc_num;
  TAO_READ_GUARD_RETURN (MAP_LOCK_TYPE, 
			 ace_mon, 
			 this->type_map_.lock (),
			 inc_num);
  return incarnation_;
}

template <class MAP_LOCK_TYPE>
SERVICE_TYPE_REPOS::IncarnationNumber 
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
add_type (const char * name, 
	  const char * if_name, 
	  const SERVICE_TYPE_REPOS::PropStructSeq& props, 
	  const SERVICE_TYPE_REPOS::ServiceTypeNameSeq& super_types,
	  CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalServiceType, 
		  SERVICE_TYPE_REPOS::ServiceTypeExists, 
		  SERVICE_TYPE_REPOS::InterfaceTypeMismatch, 
		  CosTrading::IllegalPropertyName, 
		  CosTrading::DuplicatePropertyName, 
		  SERVICE_TYPE_REPOS::ValueTypeRedefinition, 
		  CosTrading::UnknownServiceType, 
		  SERVICE_TYPE_REPOS::DuplicateServiceTypeName))
{
  PROP_MAP prop_map;
  SUPER_TYPE_MAP super_map;
  SERVICE_TYPE_REPOS::TypeStruct info;
  SERVICE_TYPE_MAP::iterator type_iterator;
  SERVICE_TYPE_REPOS::IncarnationNumber inc_num;
  
  TAO_WRITE_GUARD_RETURN (MAP_LOCK_TYPE, 
			  ace_mon, 
			  this->type_map_.lock (),
			  inc_num);

  // make sure Type name is valid
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW_RETURN (CosTrading::IllegalServiceType (name),
		      this->incarnation_);

  // check if the service type already exists.
  type_iterator = this->type_map_.find (name);
  if (type_iterator != this->type_map_.end ())
    TAO_THROW_RETURN (SERVICE_TYPE_REPOS::ServiceTypeExists (name),
		      this->incarnation_);
  
  // make sure all property names are valid and appear only once.
  this->validate_properties (props, prop_map, _env);
  TAO_CHECK_ENV_RETURN (_env, this->incarnation_);
  
  // check that all super_types exist, and none are duplicated.
  this->validate_supertypes (super_types, super_map, _env);
  TAO_CHECK_ENV_RETURN (_env, this->incarnation_);

  // make sure interface name is legal.
  // this->validate_interface (if_name) forthcoming
  
  // collect and make sure that properties of all supertypes and this type
  // are compatible.  We can use prop_map and super_types_map for the job.
  this->validate_inheritance (prop_map, super_map, _env);
  TAO_CHECK_ENV_RETURN (_env, this->incarnation_);
  
  // we can now use prop_map to construct a sequence of all properties the
  // this type.
  info.if_name = if_name;
  info.props = props;
  info.super_types = super_types;
  info.masked = 0;
  info.incarnation = this->incarnation_; 
  this->update_type_map (name, info, prop_map, super_map);

  // increment incarnation number
  this->incarnation_.low++;
  // if we wrapped around in lows...
  if (this->incarnation_.low == 0)
    this->incarnation_.high++;

  return this->type_map_[name].type_info_.incarnation;
}

template <class MAP_LOCK_TYPE>
void 
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
remove_type (const char * name,
	     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  SERVICE_TYPE_REPOS::HasSubTypes))
{
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::IllegalServiceType (name));

  TAO_WRITE_GUARD (MAP_LOCK_TYPE, 
		   ace_mon, 
		   this->type_map_.lock ());

  // check if the type exists.
  SERVICE_TYPE_MAP::iterator type_iterator = this->type_map_.find (name);
  if (type_iterator == this->type_map_.end ())
    TAO_THROW (CosTrading::UnknownServiceType (name));

  // check if has any subtypes.
  Type_Info& type_info = (*type_iterator).second;
  if (type_info.sub_types_.empty () == 0)
    {
      const char* type_name = type_info.sub_types_.front ().data ();
      TAO_THROW (SERVICE_TYPE_REPOS::HasSubTypes (name, type_name));
    }

  // remove this type from an entry of each of its supertypes.
  SERVICE_TYPE_REPOS::ServiceTypeNameSeq &super =
    type_info.type_info_.super_types;

  for (CORBA::ULong i = 0; i < super.length (); i++)
    {
      // find the super type
      const char *n = super[i];
      SERVICE_TYPE_MAP::iterator super_iterator = this->type_map_.find (n);
      
      // remove this type from its list of subtypes
      (*super_iterator).second.sub_types_.remove (name);
    }

  // remove the type from the map.
  this->type_map_.erase (type_iterator);
}
           
template <class MAP_LOCK_TYPE>
SERVICE_TYPE_REPOS::ServiceTypeNameSeq* 
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
list_types (const SERVICE_TYPE_REPOS::SpecifiedServiceTypes& which_types,
	    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  TAO_READ_GUARD_RETURN (MAP_LOCK_TYPE, 
			 ace_mon, 
			 this->type_map_.lock (),
			 (SERVICE_TYPE_REPOS::ServiceTypeNameSeq*) 0);
  
  SERVICE_TYPE_REPOS::ServiceTypeNameSeq_ptr result =
    new SERVICE_TYPE_REPOS::ServiceTypeNameSeq ();
  
  CORBA::ULong i = 0;
  
  if (which_types._d () == SERVICE_TYPE_REPOS::all)
    {
      result->length (this->type_map_.size ());
      for (SERVICE_TYPE_MAP::iterator itr = this->type_map_.begin ();
	   itr != this->type_map_.end ();
	   itr++, i++)
	(*result)[i] = CORBA::string_dup ((*itr).first.c_str ());
    }
  else
    {
      SERVICE_TYPE_REPOS::IncarnationNumber num =
	which_types.incarnation ();

      if (num > incarnation_)
	result->length (0);
      else
	result->length (this->type_map_.size ());
      
      for (SERVICE_TYPE_MAP::iterator itr = this->type_map_.begin ();
	   itr != this->type_map_.end ();
	   itr++, i++)
	{
	  if (num < (*itr).second.type_info_.incarnation)
	    (*result)[i] = CORBA::string_dup ((*itr).first.c_str ());
	}
      
      result->length (i);
    }
  
  return result;
}

           
template <class MAP_LOCK_TYPE>
SERVICE_TYPE_REPOS::TypeStruct*
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
describe_type (const char * name,
	       CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType))
{
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW_RETURN (CosTrading::IllegalServiceType (name),
		      (SERVICE_TYPE_REPOS::TypeStruct*) 0);

  TAO_READ_GUARD_RETURN (MAP_LOCK_TYPE, 
			 ace_mon, 
			 this->type_map_.lock (),
			 (SERVICE_TYPE_REPOS::TypeStruct*) 0);

  // make sure the type exists.
  SERVICE_TYPE_MAP::iterator type_iterator = this->type_map_.find (name);
  if (type_iterator == this->type_map_.end ())
    TAO_THROW_RETURN (CosTrading::UnknownServiceType (name),
		      (SERVICE_TYPE_REPOS::TypeStruct*) 0);
  
  // return appropriate information about the type.
  SERVICE_TYPE_REPOS::TypeStruct* descr = new SERVICE_TYPE_REPOS::TypeStruct;
  SERVICE_TYPE_REPOS::TypeStruct & s = (*type_iterator).second.type_info_;

  (*descr) = s;

  return descr;
}
       
template <class MAP_LOCK_TYPE>
SERVICE_TYPE_REPOS::TypeStruct* 
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
fully_describe_type (const char * name,
		     CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType))
{
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW_RETURN (CosTrading::IllegalServiceType (name),
		      (SERVICE_TYPE_REPOS::TypeStruct*) 0);
  
  TAO_READ_GUARD_RETURN (MAP_LOCK_TYPE, 
			 ace_mon, 
			 this->type_map_.lock (),
			 (SERVICE_TYPE_REPOS::TypeStruct*) 0);

  // make sure the type exists.
  SERVICE_TYPE_MAP::iterator type_iterator = this->type_map_.find (name);
  if (type_iterator == this->type_map_.end ())
    TAO_THROW_RETURN (CosTrading::UnknownServiceType (name),
		      (SERVICE_TYPE_REPOS::TypeStruct*) 0);

  // return appropriate information about the type.
  SERVICE_TYPE_REPOS::TypeStruct* descr =
    new SERVICE_TYPE_REPOS::TypeStruct;
  SERVICE_TYPE_REPOS::TypeStruct & s =
    (*type_iterator).second.type_info_;

  // We do the explicit copy, since otherwise we'd have excessive
  // properties copying.
  descr->if_name = s.if_name;
  descr->props = (*type_iterator).second.all_prop_; 
  descr->super_types = s.super_types; 
  descr->masked = s.masked; 
  descr->incarnation = s.incarnation; 

  return descr;  
}

template <class MAP_LOCK_TYPE>
void 
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
mask_type (const char * name,
	   CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  SERVICE_TYPE_REPOS::AlreadyMasked))
{
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::IllegalServiceType (name));

  TAO_WRITE_GUARD (MAP_LOCK_TYPE, 
		   ace_mon, 
		   this->type_map_.lock ());

  // make sure the type exists.
  SERVICE_TYPE_MAP::iterator type_iterator = this->type_map_.find (name);
  if (type_iterator == this->type_map_.end ())
    TAO_THROW (CosTrading::UnknownServiceType (name));

  // make sure the type is unmasked.
  CORBA::Boolean & mask = (*type_iterator).second.type_info_.masked;
  if (mask == 1)
    TAO_THROW (SERVICE_TYPE_REPOS::AlreadyMasked (name));
  // mask.
  else
    mask = 1;
}
       
template <class MAP_LOCK_TYPE>
void 
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
unmask_type (const char * name,
	     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  SERVICE_TYPE_REPOS::NotMasked))
{
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::IllegalServiceType (name));

  TAO_WRITE_GUARD (MAP_LOCK_TYPE, 
		   ace_mon, 
		   this->type_map_.lock ());

  // make sure the type exists.
  SERVICE_TYPE_MAP::iterator type_iterator = this->type_map_.find (name);
  if (type_iterator == this->type_map_.end ())
    TAO_THROW (CosTrading::UnknownServiceType (name));

  // make sure the type is masked.
  CORBA::Boolean & mask = (*type_iterator).second.type_info_.masked;
  if (mask == 0)
    TAO_THROW (SERVICE_TYPE_REPOS::NotMasked (name));
  // unmask.
  else
    mask = 0;
}

template <class MAP_LOCK_TYPE> void
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
validate_properties (const SERVICE_TYPE_REPOS::PropStructSeq& props,
		     PROP_MAP& prop_map,
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
	  string prop_name (n);
	  SERVICE_TYPE_REPOS::PropStruct* prop_struct =
	    (SERVICE_TYPE_REPOS::PropStruct *) &props[i];

	  if (! prop_map.insert
	      (make_pair (prop_name, prop_struct)).second)
	    TAO_THROW (CosTrading::DuplicatePropertyName (n));
	}
    }
}

template <class MAP_LOCK_TYPE> void
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
validate_supertypes (const SERVICE_TYPE_REPOS::ServiceTypeNameSeq& super_types,
		     SUPER_TYPE_MAP& super_map,
		     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalServiceType,
		  CosTrading::UnknownServiceType,
		  CosTrading::DuplicatePropertyName))
{
  for (CORBA::ULong i = 0; i < super_types.length (); i++)
    {
      char* type = (char*)((const char*)super_types[i]);
      
      if (! TAO_Trader_Base::is_valid_identifier_name (type))
	TAO_THROW (CosTrading::IllegalServiceType (type));
      else
	{
	  string s_type (type);
	  SERVICE_TYPE_MAP::iterator super_iter =
	    this->type_map_.find (s_type);
	  
	  if (super_iter == this->type_map_.end ())
	    TAO_THROW (CosTrading::UnknownServiceType (type));
	  else
	    if (! super_map.insert (make_pair (s_type, super_iter)).second)
	      TAO_THROW (SERVICE_TYPE_REPOS::DuplicateServiceTypeName (type));
	}
    }
}

template <class MAP_LOCK_TYPE> void
TAO_Service_Type_Repository<MAP_LOCK_TYPE>
::validate_inheritance (PROP_MAP& prop_map,
			SUPER_TYPE_MAP& super_map,
			CORBA::Environment& _env)
  TAO_THROW_SPEC ((SERVICE_TYPE_REPOS::ValueTypeRedefinition))
{
  SERVICE_TYPE_REPOS::PropertyMode mode;
  // for each super_type
  for (SUPER_TYPE_MAP::iterator super_map_iterator = super_map.begin ();
       super_map_iterator != super_map.end ();
       super_map_iterator++)
    {
      SERVICE_TYPE_MAP::iterator super_iter = (*super_map_iterator).second;
      
      // for each property in a super_type
      for (int i = 0; i < (*super_iter).second.all_prop_.length (); i++)
	{
	  SERVICE_TYPE_REPOS::PropStruct& property =
	    (*super_iter).second.all_prop_[i];
	  
	  // insert property into the map.
	  string prop_name = (const char*) property.name;
	  if (prop_map.insert (prop_name, &property).second == 0)
	    {
	      // if already there, check that it is compatible with
	      // properties of other types. Value Types have to be the
	      // same. 
	      SERVICE_TYPE_REPOS::PropStruct& property_in_map =
		*(prop_map[prop_name]);
	      
	      if (! property.value_type->equal (property_in_map.value_type, _env))
		{
		  TAO_THROW (SERVICE_TYPE_REPOS::ValueTypeRedefinition
			     (property.name, property,
			      property_in_map.name, property_in_map));
		}
	      
	      // Mode of the parent type has to be the same or less
	      // restrictive. 
	      if (property.mode >= property_in_map.mode)
		{
		  TAO_THROW (SERVICE_TYPE_REPOS::ValueTypeRedefinition
			     (property.name, property,
			      property_in_map.name, property_in_map));
		}
	    }
	}
    }
}

template <class MAP_LOCK_TYPE> void
TAO_Service_Type_Repository<MAP_LOCK_TYPE>::
update_type_map (const char* name,
		 SERVICE_TYPE_REPOS::TypeStruct& info,
		 PROP_MAP& prop_map,
		 SUPER_TYPE_MAP& super_map)
{
  Type_Info type;
  SERVICE_TYPE_REPOS::PropStructSeq all_prop;

  CORBA::ULong i = 0;
  all_prop.length (prop_map.size ());
  for (PROP_MAP::iterator prop_map_iterator = prop_map.begin ();
       prop_map_iterator != prop_map.end ();
       prop_map_iterator++, i++)
    {
      all_prop[i] = *((*prop_map_iterator).second); 
    }

  // update entries for all supertypes to include this type as a subtype. 
  // we can use the super_types_map we have constructed.

  for (SUPER_TYPE_MAP::iterator super_map_iterator = super_map.begin ();
       super_map_iterator != super_map.end ();
       super_map_iterator++)
    {
      SERVICE_TYPE_MAP::iterator super_iter = (*super_map_iterator).second;
      (*super_iter).second.sub_types_.push_back (name);
    }

  // all parameters are valid, create an entry for this service type
  // in the this->type_map_. 
  type.type_info_ = info;
  type.all_prop_ = all_prop;
  this->type_map_[name] = type;

}

int
operator< (const SERVICE_TYPE_REPOS::IncarnationNumber &l,
	   const SERVICE_TYPE_REPOS::IncarnationNumber &r)
{
  if (l.high < r.high)
    return 1;
  else if (l.high == r.high) 
    return (l.low < r.low);
  else
    return 0;
}


int
operator> (const SERVICE_TYPE_REPOS::IncarnationNumber &l,
	   const SERVICE_TYPE_REPOS::IncarnationNumber &r)
{
  return (r < l);
}

#endif /* TAO_SERVICE_TYPE_REPOSITORY_C */
