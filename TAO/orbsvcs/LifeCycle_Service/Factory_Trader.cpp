
//=============================================================================
/**
 *  @file   Factory_Trader.cpp
 *
 * A colocated instance of the Trading Service, only part of
 * the functionality provided is used. This class serves
 * as Wrapper around the Trading Service and provides
 * smaller interfaces.
 * TRADER_AVAILABLE is defined via compiler switch in the Makefile
 *
 *  @author Michael Kircher  (mk1@cs.wustl.edu)
 */
//=============================================================================

#include "orbsvcs/Log_Macros.h"
#include "Factory_Trader.h"
#include "orbsvcs/CosTradingC.h"

Factory_Trader::Factory_Trader (int debug_level)
  : debug_level_ (debug_level)
{
  try
    {
      int argc {};
      // create the trader
      this->trader_ptr_.reset (TAO_Trader_Factory::create_trader (argc, nullptr));
      this->support_Attributes_ptr_ = std::addressof (this->trader_ptr_->support_attributes ());
      // this pointer is deleted when the trader_ptr is deleted
      this->trading_Components_ptr_ = std::addressof (this->trader_ptr_->trading_components ());
      // this pointer is deleted when the trader_ptr is deleted

      // Set the service type repository
      support_Attributes_ptr_->type_repos
                     (this->repository_._this ());

      // Add the "Factory" type to the repository
      this->add_type ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "LifeCycle Server: (Factory_Trader::Factory_Trader) Failed adding a new type.\n");
    }
  // @@ ACE_CHECK?  No way to pass back any exceptions.
}

void
Factory_Trader::add_type ()
{
  try
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
      this->repository_.add_type (CORBA::string_dup("GenericFactory"),
                                  GENERIC_FACTORY_INTERFACE_REPOSITORY_ID,
                                  propStructSeq,
                                  superTypeSeq);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("LifeCycle Server: (Factory_Trader::init).\n");
    }
  // @@ ACE_CHECK
}

void
Factory_Trader::_cxx_export (char const * name,
                             char const * location,
                             char const * description,
                             CORBA::Object_ptr object_ptr)
{
  try
    {
      if (CORBA::is_nil(object_ptr))
        {
          ORBSVCS_ERROR ((LM_ERROR, "LifeCycle Server (Factory_Trader::export): "
                                "Object pointer is nil, cannot export!\n"));
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
      register_ptr->_cxx_export (CORBA::Object::_duplicate (object_ptr),
                                 CORBA::string_dup("GenericFactory"),
                                 propertySeq);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "LifeCycle Server (Factory_Trader::export): ""Failed to export factory.\n");
    }
  // @@ ACE_CHECK*
}

CORBA::Object_ptr
Factory_Trader::query (std::string const & constraint)
{
  try
    {
      CosTrading::Lookup::SpecifiedProps specifiedProps;
      specifiedProps._d(CosTrading::Lookup::all);

      CosTrading::OfferSeq_var offer_seq;
      CosTrading::OfferIterator_var offer_iterator;
      CosTrading::PolicyNameSeq_var policy_names;

      // An empty policy sequence
      CosTrading::PolicySeq policySeq;

      // Get a reference to the lookup interface
      CosTrading::Lookup_ptr lookup_ptr = this->trading_Components_ptr_->lookup_if ();
      // this pointer is deleted when the trader_ptr is deleted

      // Invoke the query method on the Lookup Interface.
      lookup_ptr->query ("GenericFactory",  // Type name
                         constraint.c_str (), // Constraint, very important
                         "",                // Preferences
                         policySeq,         // Policy
                         specifiedProps,    // Specified Properties
                         1,                 // Number of wanted results
                         offer_seq.out (),
                         offer_iterator.out (),
                         policy_names.out ()
                         );

      // Check if an offer was made
      if (offer_seq.ptr () != nullptr)
        {
          // We need at least one offer.
          if (offer_seq->length () >= 1)
            {
              // now we are all set to read from the sequence the result
              CORBA::Object_var object =
                CORBA::Object::_duplicate (offer_seq[0u].reference.in ());

              if (CORBA::is_nil (object.in ()))
                ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                       "Factory_Trader::query: Object reference is nil.\n"),
                                      0);
              else
                if (this->debug_level_ >= 2)
                  ORBSVCS_DEBUG ((LM_DEBUG, "Factory_Trader::query: Received a proper object reference.\n"));
              return object._retn ();
            }
          else
            ORBSVCS_ERROR ((LM_ERROR, "Factory_Trader::query: OfferSequence.length is smaller than 1.\n"));
        }
      return {};
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Factory_Trader::query: Failed.\n");
    }
  // @@ ACE_CHECK_RETURN (?)
  return {};
}
