// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//   Factory_Trader.cpp
//
// = DESCRIPTION
//   A colocated instance of the Trading Service, only part of 
//   the functionality provided is used. This class serves
//   as Wrapper around the Trading Service and provides 
//   smaller interfaces.
//   TRADER_AVAILABLE is defined via compiler switch in the Makefile
//
// = AUTHOR
//   Michael Kircher  (mk1@cs.wustl.edu)
//
// ============================================================================

#include "Factory_Trader.h"
#include "orbsvcs/CosTradingC.h"
 
ACE_RCSID(LifeCycle_Service, Factory_Trader, "$Id$")

// This const char * is used for adding a new type to the service repository
// the added types will be subclasses of this.
const char * Factory_Trader::GENERIC_FACTORY_INTERFACE_REPOSITORY_ID = "IDL:CosLifeCycle/GenericFactory:1.0";


Factory_Trader::Factory_Trader ()
  : trader_ptr_(0),
    trading_Components_ptr_ (0), 
    support_Attributes_ptr_(0)
{ 
  TAO_TRY
    {      
      int argc = 0;
      // create the trader 
      this->trader_ptr_ = TAO_Trader_Factory::create_trader (argc, 0);
      this->support_Attributes_ptr_ = &(trader_ptr_->support_attributes ());
      // this pointer is deleted when the trader_ptr is deleted
      this->trading_Components_ptr_ = &(trader_ptr_->trading_components ());
      // this pointer is deleted when the trader_ptr is deleted
            
      // Set the service type repository
      support_Attributes_ptr_->type_repos (this->repository_._this (TAO_TRY_ENV));
      TAO_CHECK_ENV;  
      
      // Add the "Factory" type to the repository
      this->add_type ();    
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Factory_Trader constructor: Failed adding a new type."));
      TAO_TRY_ENV.print_exception ("Factory_Trader constructor: Exception.\n");
    }
  TAO_ENDTRY;  
}

Factory_Trader::~Factory_Trader ()
{ 
  delete this->trader_ptr_;
}


void 
Factory_Trader::add_type ()
{
  TAO_TRY
    {
      // define the new type
      CosTradingRepos::ServiceTypeRepository::PropStruct propStruct_name;
      propStruct_name.name = CORBA::string_dup ("name");
      propStruct_name.value_type = CORBA::_tc_string;
      propStruct_name.mode =  CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
      
      CosTradingRepos::ServiceTypeRepository::PropStruct propStruct_location;
      propStruct_location.name  = CORBA::string_dup ("location");
      propStruct_location.value_type = CORBA::_tc_string;
      propStruct_location.mode =  CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
      
      CosTradingRepos::ServiceTypeRepository::PropStruct propStruct_description;
      propStruct_description.name = CORBA::string_dup ("description");
      propStruct_description.value_type = CORBA::_tc_string;
      propStruct_description.mode =  CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
      
      CosTradingRepos::ServiceTypeRepository::PropStructSeq propStructSeq(3);
      propStructSeq.length (3);
      propStructSeq[0] = propStruct_name;
      propStructSeq[1] = propStruct_location;
      propStructSeq[2] = propStruct_description;
            
      CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq superTypeSeq;

      // Add the new type
      this->repository_.add_type (CORBA::string_dup("Factory"),
                                  GENERIC_FACTORY_INTERFACE_REPOSITORY_ID,
                                  propStructSeq,
                                  superTypeSeq,
                                  TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("Factory_Trader::init: User Exception.\n");
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("Factory_Trader::init: System Exception.\n");
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Factory_Trader::init: Failed adding a new type."));
    }
  TAO_ENDTRY;
}
  

void 
Factory_Trader::export (const char * name, 
	       const char * location, 
	       const char * description,
	       const CORBA::Object_ptr object_ptr)
{
  TAO_TRY
    {
      if (CORBA::is_nil(object_ptr))
	{
	  ACE_DEBUG ((LM_DEBUG,"Factory_Trader::export: Object pointer is nil, cannot export!\n"));
	  return;
	}

      CosTrading::PropertySeq propertySeq(3);
      propertySeq.length (3);
      propertySeq[0].name = CORBA::string_dup("name");
      propertySeq[0].value <<= CORBA::string_dup (name);
      propertySeq[1].name =  CORBA::string_dup("location");
      propertySeq[1].value <<= CORBA::string_dup (location);
      propertySeq[2].name =  CORBA::string_dup("description");
      propertySeq[2].value <<= CORBA::string_dup (description);      
      
      // get the register interface
      CosTrading::Register_ptr register_ptr = this->trading_Components_ptr_->register_if (); 
      // this pointer is deleted when the trader_ptr is deleted
      
      // invoke the export method on the Register interface of the Trading Service
      register_ptr->export (CORBA::Object::_duplicate (object_ptr),
			    CORBA::string_dup("Factory"),
			    propertySeq,
			    TAO_TRY_ENV);
			    
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("Factory_Trader::export: User Exception.\n");
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("Factory_Trader::export: System Exception.\n");
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Factory_Trader::export: Failed to export factory.\n"));
    }
  TAO_ENDTRY;  
}


CORBA::Object_ptr
Factory_Trader::query (const CORBA::String constraint)
{
  TAO_TRY
    {           
      CosTrading::Lookup::SpecifiedProps specifiedProps;
      specifiedProps._d(CosTrading::Lookup::all);
      
      // Get some pointers for the out parameters of the call.
      CosTrading::OfferSeq_ptr offerSeq_ptr =  0;      
      CosTrading::OfferIterator_ptr offerIterator_ptr = 0;      
      CosTrading::PolicyNameSeq_ptr policyNameSeq_ptr = 0;
      
      // An empty policy sequence
      CosTrading::PolicySeq policySeq;           

      // Get a reference to the lookup interface
      CosTrading::Lookup_ptr lookup_ptr = this->trading_Components_ptr_->lookup_if ();
      // this pointer is deleted when the trader_ptr is deleted

      // Invoke the query method on the Lookup Interface.
      lookup_ptr->query ("Factory",         // Type name
			 constraint,        // Constraint, very important
			 "",                // Preferences 
			 policySeq,         // Policy
			 specifiedProps,    // Specified Properties
			 1,                 // Number of wanted results
			 CosTrading::OfferSeq_out(offerSeq_ptr),               // results
			 CosTrading::OfferIterator_out(offerIterator_ptr),     // more results
			 CosTrading::PolicyNameSeq_out(policyNameSeq_ptr),     // Policies
			 TAO_TRY_ENV);
      TAO_CHECK_ENV;      
      
      // Initialize
      CORBA::Object_ptr object_ptr = 0;
      
      // Check if an offer was made
      if (offerSeq_ptr != 0)
	{
	  // Insert the pointer into the out class
	  CosTrading::OfferSeq_var offerSeq_var(offerSeq_ptr);
	  
	  // We need at least one offer.
	  if (offerSeq_var->length() >= 1)
	    {
	      // now we are all set to read from the sequence the result
	      object_ptr = CORBA::Object::_duplicate (offerSeq_var[0].reference.in());

	      if (CORBA::is_nil (object_ptr))
		{
		  ACE_DEBUG ((LM_DEBUG,"Factory_Trader::query: Object reference is nil.\n"));
		  return 0;
		}
	      else
		ACE_DEBUG ((LM_DEBUG,"Factory_Trader::query: Received a proper object reference.\n"));
	    } 
	  else
	    ACE_DEBUG ((LM_DEBUG,"Factory_Trader::query: OfferSequence.length is smaller than 1.\n"));	   
	}
      return object_ptr;      
    } 
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("Factory_Trader::query: User Exception");
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("Factory_Trader::query: System Exception");
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Factory_Trader::query: Failed.\n"));
    }
  TAO_ENDTRY;    
  return 0;
}


