/* -*- C++ -*- */

// ============================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   Register.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ============================================================================

#if !defined (TAO_REGISTER_C)
#define TAO_REGISTER_C

#include "Register.h"
#include "Property_Evaluator.h"
#include "Trader.h"
#include <iostream.h>
#include <algorithm>

template <class TRADER>
const char* TAO_Register<TRADER>::NAME = "Trader: Register";

template <class TRADER>
TAO_Register<TRADER>::TAO_Register (TRADER &trader)
  : trader_ (trader),
    TAO_Trader_Components<POA_CosTrading::Register> (trader.trading_components ()),
    TAO_Support_Attributes<POA_CosTrading::Register> (trader.support_attributes ())
{
}

template <class TRADER>
TAO_Register<TRADER>::~TAO_Register (void)
{
}

template <class TRADER> CosTrading::OfferId 
TAO_Register<TRADER>::export (CORBA::Object_ptr reference, 
			      const char *type, 
			      const CosTrading::PropertySeq &properties,
			      CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::Register::InvalidObjectRef, 
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  CosTrading::Register::InterfaceTypeMismatch, 
		  CosTrading::IllegalPropertyName, 
		  CosTrading::PropertyTypeMismatch, 
		  CosTrading::ReadonlyDynamicProperty, 
		  CosTrading::MissingMandatoryProperty, 
		  CosTrading::DuplicatePropertyName))
{
  // Get service type map
  TRADER::SERVICE_TYPE_MAP &service_type_map = 
    this->trader_.service_type_map ();

  CosTrading::Offer offer;
  TAO_Support_Attributes_Impl& support_attrs =
    this->trader_.support_attributes ();  
  CosTrading::TypeRepository_ptr type_repos =
    support_attrs.type_repos ();
  CosTradingRepos::ServiceTypeRepository_ptr rep = 
    CosTradingRepos::ServiceTypeRepository::_narrow (type_repos, _env);
  TAO_CHECK_ENV_RETURN (_env, 0);
  
  // Yank our friend, the type struct, and confirm that the given
  // properties match the type definition.
  TYPE_STRUCT* type_struct = rep->fully_describe_type (type, _env);
  TAO_CHECK_ENV_RETURN (_env, 0);
  
  // Oops the type is masked, we shouldn't let exporters know the type 
  // exists.
  if (type_struct->masked)
    TAO_THROW_RETURN (CosTrading::UnknownServiceType (type), 0);

  // TAO-specific way to determine if an object is derived from or is
  // an interface type.
  if (! reference->_is_a (type_struct->if_name, _env))
    TAO_THROW_RETURN (CosTrading::Register::
		      InterfaceTypeMismatch (type, reference), 0);
  
  // Validate that the properties defined for this offer are correct
  // to their types and strength.
  this->validate_properties (type, type_struct,
			     (CosTrading::PropertySeq&) properties, _env);
  TAO_CHECK_ENV_RETURN (_env, 0);
  
  offer.reference = (reference->_duplicate (reference));
  offer.properties = properties;
  
  // Insert the offer into the underlying type map.
  CosTrading::OfferId id = service_type_map.insert_offer (type, offer);
  
  if (id == 0)
    {
      // Add type, if it's already been added in that split second
      // since we've released the lock, nothing bad will happen.
      service_type_map.add_type (type);      
      id = service_type_map.insert_offer (type, offer);
    }
  
  return id;
}

template <class TRADER> void 
TAO_Register<TRADER>::withdraw (const char *id,
				CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException, 
		   CosTrading::IllegalOfferId, 
		   CosTrading::UnknownOfferId, 
		   CosTrading::Register::ProxyOfferId))
{
  // Get service type map.
  TRADER::SERVICE_TYPE_MAP &service_type_map = 
    this->trader_.service_type_map ();

  service_type_map.remove_offer ((CosTrading::OfferId) id, _env);
}

template <class TRADER> CosTrading::Register::OfferInfo * 
TAO_Register<TRADER>::describe (const char *id,
				CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::IllegalOfferId, 
		  CosTrading::UnknownOfferId, 
		  CosTrading::Register::ProxyOfferId))
{
  // Get service type map.
  char* type = 0;
  TRADER::SERVICE_TYPE_MAP &service_type_map = 
    this->trader_.service_type_map ();

  // Perform a lookup to find the offer.
  CosTrading::Offer* offer =
    service_type_map.lookup_offer ((CosTrading::OfferId) id, type, _env);
  TAO_CHECK_ENV_RETURN (_env, (CosTrading::Register::OfferInfo *) 0);
      
  CosTrading::Register::OfferInfo *offer_info = 
    new CosTrading::Register::OfferInfo ();

  offer_info->reference = offer->reference->_duplicate (offer->reference);
  offer_info->type = CORBA::string_dup (type);
  offer_info->properties = offer->properties;

  return offer_info;
}

template <class TRADER> void 
TAO_Register<TRADER>::modify (const char *id, 
			      const CosTrading::PropertyNameSeq& del_list, 
			      const CosTrading::PropertySeq& modify_list,
			      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::NotImplemented, 
		  CosTrading::IllegalOfferId, 
		  CosTrading::UnknownOfferId, 
		  CosTrading::Register::ProxyOfferId, 
		  CosTrading::IllegalPropertyName, 
		  CosTrading::Register::UnknownPropertyName, 
		  CosTrading::PropertyTypeMismatch, 
		  CosTrading::ReadonlyDynamicProperty, 
		  CosTrading::Register::MandatoryProperty, 
		  CosTrading::Register::ReadonlyProperty, 
		  CosTrading::DuplicatePropertyName))
{
  // Throw an exception if the trader is not configured
  // to support properties modification.
  if (! this->supports_modifiable_properties (_env))
    TAO_THROW (CosTrading::NotImplemented ());
  else
    {
      char* type = 0;
      TAO_Support_Attributes_Impl& support_attrs =
	this->trader_.support_attributes ();  
      CosTrading::TypeRepository_ptr type_repos =
	support_attrs.type_repos ();
      CosTradingRepos::ServiceTypeRepository_ptr rep = 
	CosTradingRepos::ServiceTypeRepository::_narrow (type_repos, _env);
      TAO_CHECK_ENV_RETURN (_env,);
      TRADER::SERVICE_TYPE_MAP &service_type_map = 
	this->trader_.service_type_map ();
      
      CosTrading::Offer* offer =
	service_type_map.lookup_offer ((CosTrading::OfferId) id, type, _env);
      TAO_CHECK_ENV_RETURN (_env,);
      
      if (offer != 0)
	{
	  // Yank our friend, the type struct.
	  TYPE_STRUCT* type_struct = rep->describe_type (type, _env);
	  TAO_CHECK_ENV_RETURN (_env,);
	  TAO_Offer_Modifier offer_mod (type, type_struct, *offer);

	  // Delete, add, and change properties of the offer.
	  this->validate_properties (type, type_struct,
				     (CosTrading::PropertySeq) modify_list, _env);
	  TAO_CHECK_ENV_RETURN (_env,);
	  offer_mod.delete_properties (del_list, _env);
	  TAO_CHECK_ENV_RETURN (_env,);
	  offer_mod.merge_properties (modify_list, _env);
	  TAO_CHECK_ENV_RETURN (_env,);

	  // Alter our reference to the offer. 
	  offer_mod.affect_change ();
	}
    }
}

template <class TRADER> void 
TAO_Register<TRADER>::withdraw_using_constraint (const char *type, 
						 const char *constr,
						 CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  CosTrading::IllegalConstraint, 
		  CosTrading::Register::NoMatchingOffers))
{
  int num_removed = 0;
  deque<CosTrading::OfferId_var> ids;
  TAO_Support_Attributes_Impl& support_attrs =
    this->trader_.support_attributes ();  
  CosTrading::TypeRepository_ptr type_repos =
    support_attrs.type_repos ();
  CosTradingRepos::ServiceTypeRepository_ptr rep = 
    CosTradingRepos::ServiceTypeRepository::_narrow (type_repos, _env);
  TRADER::SERVICE_TYPE_MAP &service_type_map = 
    this->trader_.service_type_map ();
  CORBA::Boolean dp_support =
    support_attrs.supports_dynamic_properties ();
  
  // Retrieve the type struct
  TYPE_STRUCT* type_struct = rep->describe_type (type, _env);
  TAO_CHECK_ENV_RETURN (_env,);

  // Try to find the map of offers of desired service type.
  TRADER::SERVICE_TYPE_MAP::Local_Offer_Iterator*
    offer_iter = service_type_map.get_offers (type);

  if (offer_iter != 0)
    {
      TAO_Constraint_Validator validator (type_struct);
      TAO_Constraint_Interpreter constr_inter (validator, constr, _env);
      TAO_CHECK_ENV_RETURN (_env,);
      
      while (offer_iter->has_more_offers ())
	{
	  CosTrading::Offer* offer = offer_iter->get_offer ();
	  // Add offer if it matches the constraints
	  
	  TAO_Constraint_Evaluator evaluator (offer, dp_support);
	  if (constr_inter.evaluate (evaluator))
	    ids.push_back (offer_iter->get_id ());
	  
	  offer_iter->next_offer ();
	}
      
      // Must delete the iterator, so we can write to the service type map.
      delete offer_iter;
    }
  
  if (ids.size () == 0)
    TAO_THROW (CosTrading::Register::NoMatchingOffers (constr));
  else
    {      
      for (deque<CosTrading::OfferId_var>::iterator id_iter = ids.begin ();
	   id_iter != ids.end ();
	   id_iter++)
	{
	  service_type_map.remove_offer
	    ((CosTrading::OfferId) ids.front ().in (), _env);
	}
    }
}
           
template <class TRADER> CosTrading::Register_ptr 
TAO_Register<TRADER>::resolve (const CosTrading::TraderName &name,
			       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::Register::IllegalTraderName, 
		  CosTrading::Register::UnknownTraderName, 
		  CosTrading::Register::RegisterNotSupported))
{
  // Determine if the first link is a legal link name.
  if (! TAO_Trader_Base::is_valid_identifier_name (name[0]))
    TAO_THROW_RETURN (CosTrading::Register::IllegalTraderName (name),
		      CosTrading::Register::_nil ());

  // Grab a reference to the link interface, and get a link description.
  CosTrading::Link_var link (this->link_if (_env));
  TAO_CHECK_ENV_RETURN (_env, CosTrading::Register::_nil ());
  CosTrading::Link::LinkInfo_var link_info;
  
  TAO_TRY
    {
      // Ensure that the link to the next trader exists.
      link_info = link->describe_link (name[0], TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_THROW_RETURN (CosTrading::Register::UnknownTraderName (name),
			CosTrading::Register::_nil ());
    }
  TAO_ENDTRY;
  
  // Ensure that the register pointer isn't nil.
  if (link_info->target_reg == CosTrading::Register::_nil ())
    TAO_THROW_RETURN (CosTrading::Register::RegisterNotSupported (name),
		      CosTrading::Register::_nil ());

  CosTrading::Register_ptr return_value = link_info->target_reg;
  
  if (name.length () > 1)
    {
      // Create a new Trader Name with the first link removed.
      CosTrading::TraderName trader_name (name.length () - 1);
      for (int i = trader_name.length () - 1; i >= 0; i--)
	trader_name[i] = name[i + 1];

      return_value = link_info->target_reg->resolve (trader_name, _env);
      TAO_CHECK_ENV_RETURN (_env, CosTrading::Register::_nil ());
    }

  return return_value;
}

template <class TRADER> void
TAO_Register<TRADER>::
validate_properties (const char* type, 
		     TYPE_STRUCT* type_struct,
		     CosTrading::PropertySeq& properties,
		     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalPropertyName, 
		  CosTrading::PropertyTypeMismatch, 
		  CosTrading::ReadonlyDynamicProperty, 
		  CosTrading::MissingMandatoryProperty, 
		  CosTrading::DuplicatePropertyName))
{
  typedef CosTradingRepos::ServiceTypeRepository SERVICE_TYPE_REPOS;
  
  int length = properties.length ();
  SERVICE_TYPE_REPOS::PropStructSeq& prop_types = type_struct->props;
  TAO_Property_Evaluator_By_Name prop_eval (properties, _env);
  TAO_CHECK_ENV_RETURN (_env,);
  
  // Perform property validation
  length = prop_types.length ();
  for (int i = 0; i < length; i++)
    {
      SERVICE_TYPE_REPOS::PropStruct& prop_struct = prop_types[i]; 
      const char* prop_name  = prop_struct.name;
      
      // Obtain the type of the exported property.
      CORBA::TypeCode* prop_type = prop_eval.property_type (prop_name);
      
      if (prop_type == CORBA::TypeCode::_nil ())
	{
	  // Offer cannot have a missing mandatory property.
	  if (prop_types[i].mode == SERVICE_TYPE_REPOS::PROP_MANDATORY)
	    TAO_THROW (CosTrading::MissingMandatoryProperty (type, prop_name));
	}
      else
	{
	  if (! prop_type->equal (prop_struct.value_type, _env))
	    {
	      // Offer cannot redefine the type of an property. 
	      const CosTrading::Property* prop = prop_eval.get_property (prop_name);
	      TAO_THROW (CosTrading::PropertyTypeMismatch (type, *prop));
	    }
	  else if (prop_struct.mode == SERVICE_TYPE_REPOS::PROP_READONLY &&
		   prop_eval.is_dynamic_property (prop_name))
	    TAO_THROW (CosTrading::ReadonlyDynamicProperty (type, prop_name));
	}
    }
}

#endif /* TAO_REGISTER_C */
