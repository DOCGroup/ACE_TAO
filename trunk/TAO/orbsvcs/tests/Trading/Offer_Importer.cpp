// $Id$
#include "Offer_Importer.h"
#include "orbsvcs/Trader/Policy_Manager.h"

TAO_Offer_Importer::TAO_Offer_Importer (CosTrading::Lookup_ptr lookup_if)
  : lookup_ (lookup_if)
{
}

void
TAO_Offer_Importer::perform_queries (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType,
		   CosTrading::UnknownServiceType,
		   CosTrading::IllegalConstraint,
		   CosTrading::Lookup::IllegalPreference,
		   CosTrading::Lookup::IllegalPolicyName,
		   CosTrading::Lookup::PolicyTypeMismatch,
		   CosTrading::Lookup::InvalidPolicyValue,
		   CosTrading::IllegalPropertyName,
		   CosTrading::DuplicatePropertyName,
		   CosTrading::DuplicatePolicyName))
{
  TAO_TRY
    {
      TAO_Policy_Manager policies;
      CosTrading::Lookup::SpecifiedProps desired_props;

      
      //      CosTrading::PropertyNameSeq prop_names (3);
      const char* prop_names[] =
      {
	TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::NAME],
	TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::NAME],
      	TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::NAME]
      };
      
      //prop_names.length (3);
      //prop_names[0] = ;
      //      prop_names[1] = ;
      //      prop_names[2] = ;

      //      desired_props.prop_names (CosTrading::PropertyNameSeq (3, 3, (char **) prop_names, CORBA::B_FALSE));
      desired_props._d (CosTrading::Lookup::all);
      policies.exact_type_match (CORBA::B_FALSE);
      policies.search_card (4*NUM_OFFERS);
      policies.match_card (4*NUM_OFFERS);
      policies.return_card (4*NUM_OFFERS);
      for (int i = 0; i < TT_Info::NUM_QUERIES; i++)
	{
	  ACE_DEBUG ((LM_DEBUG, "Performing query for %s.\n", TT_Info::QUERIES[i][0]));
	  ACE_DEBUG ((LM_DEBUG, "Query: %s\n", TT_Info::QUERIES[i][1]));
	  ACE_DEBUG ((LM_DEBUG, "Preferences: %s\n", TT_Info::QUERIES[i][2]));     

	  CosTrading::OfferSeq_ptr offer_seq_ptr = 0;
	  CosTrading::OfferIterator_ptr offer_iterator_ptr = 0;
	  CosTrading::PolicyNameSeq_ptr limits_applied_ptr = 0;

	  CosTrading::OfferSeq_out offer_seq_out (offer_seq_ptr);
	  CosTrading::OfferIterator_out offer_iterator_out (offer_iterator_ptr);
	  CosTrading::PolicyNameSeq_out limits_applied_out (limits_applied_ptr);
	  
	  this->lookup_->query (TT_Info::QUERIES[i][0],
				TT_Info::QUERIES[i][1],
				TT_Info::QUERIES[i][2],
				policies.policy_seq (),
				desired_props,
				8,
				offer_seq_out,
				offer_iterator_out,
				limits_applied_out,
				TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  CosTrading::OfferSeq_var offer_seq (offer_seq_ptr);
	  CosTrading::OfferIterator_var offer_iterator (offer_iterator_ptr);
	  CosTrading::PolicyNameSeq_var limits_applied (limits_applied_ptr);
	  ACE_DEBUG ((LM_DEBUG, "Results:\n\n"));

	  
	  this->display_results (*offer_seq_ptr,
				 offer_iterator_ptr,
				 TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  
	  ACE_DEBUG ((LM_DEBUG, "Limits Applied:\n\n"));
	  for (int length = limits_applied_out->length (), j = 0; j < length; j++)
	    {
	      ACE_DEBUG ((LM_DEBUG, "%s\n", (const char *)(*limits_applied_out)[j]));
	    }
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Offer_Importer::perform_queries");
      TAO_RETHROW;
    }
  TAO_ENDTRY;    
}

void
TAO_Offer_Importer::perform_federated_queries (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType,
		   CosTrading::UnknownServiceType,
		   CosTrading::IllegalConstraint,
		   CosTrading::Lookup::IllegalPreference,
		   CosTrading::Lookup::IllegalPolicyName,
		   CosTrading::Lookup::PolicyTypeMismatch,
		   CosTrading::Lookup::InvalidPolicyValue,
		   CosTrading::IllegalPropertyName,
		   CosTrading::DuplicatePropertyName,
		   CosTrading::DuplicatePolicyName))
{
}

void
TAO_Offer_Importer::display_results (const CosTrading::OfferSeq& offer_seq,
				     CosTrading::OfferIterator_ptr offer_iterator,
				     CORBA::Environment& _env) const
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  for (int length = offer_seq.length (), i = 0; i < length; i++)
    {
      TT_Info::dump_properties (offer_seq[i].properties);
      //      TT_Info::serialize_offer (offer_seq[i]);
      //      CosTrading::Offer_var offer = TT_Info::deserialize_offer ();
      //      TT_Info::dump_properties (offer->properties);
      ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
    }

  TAO_TRY
    {  
      if (! CORBA::is_nil (offer_iterator))
	{
	  CORBA::Boolean any_left = CORBA::B_FALSE;

	  do
	    {
	      CosTrading::OfferSeq_ptr iter_offers_ptr;
	      CosTrading::OfferSeq_out iter_offers_out (iter_offers_ptr);

	      any_left = offer_iterator->next_n (length,
						 iter_offers_out,
						 TAO_TRY_ENV);
	      TAO_CHECK_ENV;

	      CosTrading::OfferSeq_var iter_offers (iter_offers_ptr);
	      for (length = iter_offers->length (), i = 0; i < length; i++)
		{
		  CosTrading::PropertySeq& props = iter_offers[i].properties;
		  TT_Info::dump_properties (props);
		  ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
		}
	      
	    } while (any_left); 	
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Offer_Importer::display_results");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}
