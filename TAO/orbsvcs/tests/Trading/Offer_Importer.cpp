// $Id$

#include "Offer_Importer.h"

TAO_Offer_Importer::TAO_Offer_Importer (CosTrading::Lookup_ptr lookup_if,
                                        CORBA::Boolean verbose)
  : verbose_ (verbose),
    lookup_ (lookup_if)
{
}

void
TAO_Offer_Importer::perform_queries (void)
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Importer::Federated Query.\n"));

  TAO_Policy_Creator policies;
  policies.exact_type_match (0);
  policies.search_card (16*NUM_OFFERS);
  policies.match_card (16*NUM_OFFERS);
  policies.return_card (16*NUM_OFFERS);
  policies.link_follow_rule (CosTrading::always);
  this->perform_queries_with_policies (policies);
}

void
TAO_Offer_Importer::perform_directed_queries (void)
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
  CosTrading::Link_var link_if = this->lookup_->link_if ();

  if (this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG, "Obtaining references to traders directly"
                  " linked to the root trader.\n"));
    }
  CosTrading::LinkNameSeq_var link_name_seq = link_if->list_links ();

  if (link_name_seq->length () > 0)
    {
      if (this->verbose_)
        {
          ACE_DEBUG ((LM_DEBUG, "Getting link information for %C\n",
                      static_cast<const char*> (link_name_seq[0u])));
        }

      CosTrading::Link::LinkInfo_var link_info =
        link_if->describe_link (link_name_seq[0u]);

      CosTrading::Lookup_ptr lookup_if = link_info->target.in ();
      CosTrading::Link_var link_if2 = lookup_if->link_if ();

      CosTrading::LinkNameSeq_var link_name_seq2 = link_if2->list_links ();

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

              ACE_DEBUG ((LM_DEBUG, "*** Query through %C to destination %C.\n",
                          static_cast<const char*> (link_name_seq[0u]),
                          static_cast<const char*> (link_name_seq2[i])));

              trader_name[0] = CORBA::string_dup (link_name_seq[0u]);
              trader_name[1] = CORBA::string_dup (link_name_seq2[i]);
              policies.starting_trader (new CosTrading::TraderName
                                        (2, 2, trader_name, 1));

              this->perform_queries_with_policies (policies);
            }
        }
      else
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT("ERROR, This test requires a complete graph of three traders!\n")));
          throw CORBA::BAD_INV_ORDER(); // Let outside world know we can't continue this way.
        }
    }
  else
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT("ERROR, This test requires a complete graph of three traders!\n")));
      throw CORBA::BAD_INV_ORDER(); // Let outside world know we can't continue this way.
    }
}


void
TAO_Offer_Importer::perform_queries_with_policies (
    const TAO_Policy_Creator& policies
  )
{
  try
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
          ACE_DEBUG ((LM_DEBUG, "*** Performing query for %C.\n", TT_Info::QUERIES[i][0]));
          ACE_DEBUG ((LM_DEBUG, "*** Query: %C\n", TT_Info::QUERIES[i][1]));
          ACE_DEBUG ((LM_DEBUG, "*** Preferences: %C\n", TT_Info::QUERIES[i][2]));
          CosTrading::OfferSeq_var      offer_seq;
          CosTrading::OfferIterator_var offer_iterator;
          CosTrading::PolicyNameSeq_var limits_applied;

          // Test with different how_many amount, both should work.
          // Initially try to get this amount in the sequence.
          CORBA::ULong how_many = 8;
          if (i&0x0001)
          {
            how_many=0; // 0: Don't retrieve in offers but all in iterator.
          };

          this->lookup_->query (TT_Info::QUERIES[i][0],
                                TT_Info::QUERIES[i][1],
                                TT_Info::QUERIES[i][2],
                                policies.policy_seq (),
                                desired_props,
                                how_many,
                                offer_seq.out(),
                                offer_iterator.out(),
                                limits_applied.out());

          CORBA::ULong seqlen = (offer_seq.ptr() == 0 ? 0 :
                                 offer_seq->length ());
          CORBA::ULong itrlen = (CORBA::is_nil(offer_iterator.in()) ? 0 :
                                 offer_iterator->max_left ());
          CORBA::ULong total = seqlen + itrlen;
          CORBA::ULong expected = ACE_OS::atoi (TT_Info::QUERIES[i][3]);

          if (total != expected)
          {
            ACE_ERROR ((LM_ERROR
                        ,"ERROR: Expected %d for query %d, got seq(%d) + itr(%d) = %d\n"
                        ,expected
                        ,i
                        ,seqlen
                        ,itrlen
                        ,total
                        ));
            throw CORBA::TRANSIENT(); // Try again later?
          }
          else if (this->verbose_)
          {
            ACE_DEBUG ((LM_DEBUG
                        ,"Expected %d for query %d, got seq(%d) + itr(%d) = %d\n"
                        ,expected
                        ,i
                        ,seqlen
                        ,itrlen
                        ,total
                        ));
          };

          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "*** Results:\n\n"));
              this->display_results (*offer_seq.ptr(),
                                     offer_iterator.in());

              if (limits_applied->length () > 0)
                ACE_DEBUG ((LM_DEBUG, "*** Limits Applied:\n\n"));

              for (int length = limits_applied->length (), j = 0; j < length; j++)
                {
                  const char *policy_name = (*limits_applied.ptr())[j];
                  ACE_DEBUG ((LM_DEBUG, "%C\n",
                              static_cast<const char*> (policy_name)));
                }
            }
        }
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("TAO_Offer_Importer::perform_queries_with_policies");
      throw;
    }
}

void
TAO_Offer_Importer::display_results (const CosTrading::OfferSeq& offer_seq,
                                     CosTrading::OfferIterator_ptr offer_iterator) const
{
  try
    {
      ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
      ACE_DEBUG ((LM_DEBUG, "Offers in the sequence:\n"));
      ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
      for (CORBA::ULong i = 0; i < offer_seq.length (); i++)
        {
          // Call back to the exported object.
          // FIXME: shouldn't this be done in a separate 'verify_results' method?
          //        (confirm () now skipped in quiet mode)
          TAO_Trader_Test::Remote_Output_var remote_output =
            TAO_Trader_Test::Remote_Output::_narrow (offer_seq[i].reference.in ());

          remote_output->confirm ();

          TT_Info::dump_properties (offer_seq[i].properties, 1);
          ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
        }

      ACE_DEBUG ((LM_DEBUG, "  Offers in the iterator:\n"));
      ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
      if (! CORBA::is_nil (offer_iterator))
        {
          CORBA::Boolean any_left = 0;
          do
            {
              CosTrading::OfferSeq_var offers;
              // Use of iterator->max_left() to get all iterator results is not
              // recommended? (see p.879 Advanced CORBA programming book)
              // Howmany to process is a choice between next_n call 'dispatch
              // costs' and 'larges results marshalling bandwidth costs'.
              CORBA::ULong how_many = 100;
              any_left = offer_iterator->next_n (how_many, offers.out());

              for (CORBA::ULong i = 0; i < offers->length (); i++)
                {
                  // Call back to the exported object.
                  // FIXME: shouldn't this be done in a separate 'verify_results' method?
                  //        (confirm () now skipped in quiet mode)
                  TAO_Trader_Test::Remote_Output_var remote_output =
                    TAO_Trader_Test::Remote_Output::_narrow ((*offers)[i].reference.in ());

                  remote_output->confirm ();

                  CosTrading::PropertySeq& props = (*offers)[i].properties;
                  TT_Info::dump_properties (props, 1);

                  ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
                }

            } while (any_left);

          offer_iterator->destroy ();
        }
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("TAO_Offer_Importer::display_results");
      throw;
    }
}
