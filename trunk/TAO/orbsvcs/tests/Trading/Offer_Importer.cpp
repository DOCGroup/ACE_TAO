// $Id$

#include "Offer_Importer.h"

ACE_RCSID(Trading, Offer_Importer, "$Id$")

TAO_Offer_Importer::TAO_Offer_Importer (CosTrading::Lookup_ptr lookup_if,
                                        CORBA::Boolean verbose)
  : verbose_ (verbose),
    lookup_ (lookup_if)
{
}

void
TAO_Offer_Importer::perform_queries (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
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
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Importer::Federated Query.\n"));

  TAO_Policy_Creator policies;
  policies.exact_type_match (0);
  policies.search_card (16*NUM_OFFERS);
  policies.match_card (16*NUM_OFFERS);
  policies.return_card (16*NUM_OFFERS);
  policies.link_follow_rule (CosTrading::always);

  this->perform_queries_with_policies (policies ACE_ENV_ARG_PARAMETER);
  // ACE_CHECK;
}

void
TAO_Offer_Importer::perform_directed_queries (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
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
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Importer::Directed Query.\n"));

  TAO_Policy_Creator policies;
  policies.exact_type_match (0);
  policies.search_card (16*NUM_OFFERS);
  policies.match_card (16*NUM_OFFERS);
  policies.return_card (16*NUM_OFFERS);
  policies.link_follow_rule (CosTrading::local_only);

  if (this->verbose_)
    ACE_DEBUG ((LM_DEBUG, "Obtaining link interface.\n"));
  CosTrading::Link_var link_if = this->lookup_->link_if (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG, "Obtaining references to traders directly"
                  " linked to the root trader.\n"));
    }
  CosTrading::LinkNameSeq_var link_name_seq = link_if->list_links (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (link_name_seq->length () > 0)
    {
      if (this->verbose_)
        {
          ACE_DEBUG ((LM_DEBUG, "Getting link information for %s\n",
                      static_cast<const char*> (link_name_seq[0u])));
        }

      CosTrading::Link::LinkInfo_var link_info =
        link_if->describe_link (link_name_seq[0u] ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosTrading::Lookup_ptr lookup_if = link_info->target.in ();
      CosTrading::Link_var link_if2 = lookup_if->link_if (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CosTrading::LinkNameSeq_var link_name_seq2 = link_if2->list_links (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (link_name_seq2->length () > 0)
        {
          CORBA::ULong i = 0;
          CORBA::ULong length = link_name_seq2->length ();
          for (i = 0; i < length; i++)
            {
              if (ACE_OS::strcmp (link_name_seq2[i], "Bootstrap") != 0)
                break;
            }

          if (i < length)
            {
              CosTrading::LinkName* trader_name =
                CosTrading::TraderName::allocbuf (2);

              ACE_DEBUG ((LM_DEBUG, "*** Query through %s to destination %s.\n",
                          static_cast<const char*> (link_name_seq[0u]),
                          static_cast<const char*> (link_name_seq2[i])));

              trader_name[0] = CORBA::string_dup (link_name_seq[0u]);
              trader_name[1] = CORBA::string_dup (link_name_seq2[i]);
              policies.starting_trader (new CosTrading::TraderName
                                        (2, 2, trader_name, 1));

              this->perform_queries_with_policies (policies ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "This test requires a complete"
                      " graph of three traders.\n"));
        }
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "This test requires a complete"
                  " graph of three traders.\n"));
    }
}


void
TAO_Offer_Importer::perform_queries_with_policies (
    const TAO_Policy_Creator& policies
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
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
  ACE_TRY
    {
      CosTrading::Lookup::SpecifiedProps desired_props;

      char* props[] =
      {
        (char*) TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::NAME],
        (char*) TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::DESCRIPTION],
        (char*) TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::LOCATION],
        (char*) TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::HOST_NAME]
      };

      CosTrading::PropertyNameSeq prop_name_seq (4, 4, props, 0);
      desired_props.prop_names (prop_name_seq);

      for (int i = 0; i < TT_Info::NUM_QUERIES; i++)
        {
          ACE_DEBUG ((LM_DEBUG, "\n"));
          ACE_DEBUG ((LM_DEBUG, "*** Performing query for %s.\n", TT_Info::QUERIES[i][0]));
          ACE_DEBUG ((LM_DEBUG, "*** Query: %s\n", TT_Info::QUERIES[i][1]));
          ACE_DEBUG ((LM_DEBUG, "*** Preferences: %s\n", TT_Info::QUERIES[i][2]));
          CosTrading::OfferSeq *offer_seq_ptr = 0;
          CosTrading::OfferIterator_ptr offer_iterator_ptr = 0;
          CosTrading::PolicyNameSeq *limits_applied_ptr = 0;

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
                                limits_applied_out
                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CosTrading::OfferSeq_var offer_seq (offer_seq_ptr);
          CosTrading::OfferIterator_var offer_iterator (offer_iterator_ptr);
          CosTrading::PolicyNameSeq_var limits_applied (limits_applied_ptr);

          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "*** Results:\n\n"));
              this->display_results (*offer_seq_ptr,
                                     offer_iterator_ptr
                                     ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (limits_applied_out->length () > 0)
                ACE_DEBUG ((LM_DEBUG, "*** Limits Applied:\n\n"));

              for (int length = limits_applied_out->length (), j = 0; j < length; j++)
                {
                  const char *policy_name = (*limits_applied_ptr)[j];
                  ACE_DEBUG ((LM_DEBUG, "%s\n",
                              static_cast<const char*> (policy_name)));
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Offer_Importer::perform_queries");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

void
TAO_Offer_Importer::display_results (const CosTrading::OfferSeq& offer_seq,
                                     CosTrading::OfferIterator_ptr offer_iterator
                                     ACE_ENV_ARG_DECL) const
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      CORBA::ULong length = 0, i = 0;
      ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
      ACE_DEBUG ((LM_DEBUG, "Offers in the sequence:\n"));
      ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
      for (length = offer_seq.length (), i = 0; i < length; i++)
        {
          // Call back to the exported object.
          TAO_Trader_Test::Remote_Output_var remote_output =
            TAO_Trader_Test::Remote_Output::_narrow (offer_seq[i].reference.in ()
                                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          remote_output->confirm (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          TT_Info::dump_properties (offer_seq[i].properties, 1 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
        }

      ACE_DEBUG ((LM_DEBUG, "  Offers in the iterator:\n"));
      ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
      if (! CORBA::is_nil (offer_iterator))
        {
          length = offer_seq.length ();
          CORBA::Boolean any_left = 0;

          do
            {
              CosTrading::OfferSeq *iter_offers_ptr;
              CosTrading::OfferSeq_out iter_offers_out (iter_offers_ptr);

              any_left = offer_iterator->next_n (length,
                                                 iter_offers_out
                                                 ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              CosTrading::OfferSeq_var iter_offers (iter_offers_ptr);
              for (length = iter_offers->length (), i = 0; i < length; i++)
                {
                  // Call back to the exported object.
                  TAO_Trader_Test::Remote_Output_var remote_output =
                    TAO_Trader_Test::Remote_Output::_narrow (offer_seq[i].reference.in ()
                                                             ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  remote_output->confirm (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  CosTrading::PropertySeq& props = iter_offers[i].properties;
                  TT_Info::dump_properties (props, 1 ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
                }

            } while (any_left);

          offer_iterator->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Offer_Importer::display_results");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}
