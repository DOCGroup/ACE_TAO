// ========================================================================
// $Id$
// 
// = FILENAME
//    Trader_Client.cpp
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#include "Trader_Client.h"

ACE_RCSID(server_discovery, Trader_Client, "$Id$")

Trader_Client* Trader_Client::instance_ = 0;

Trader_Client*
Trader_Client::instance (void)
{
  if (Trader_Client::instance_ == 0)
    Trader_Client::instance_ = new Trader_Client;

  return Trader_Client::instance_;
}

Trader_Client::Trader_Client (void)
{
}

Trader_Client::~Trader_Client (void)
{
  // Iterate and destroy all dynamically allocated second-tier maps.
  for (Server_Map::ITERATOR map_iter (this->map_);
       ! map_iter.done ();
       map_iter.advance ())
    {
      Server_Map::ENTRY* map_entry = 0;
      map_iter.next (map_entry);
      delete map_entry->int_id_;
    }
}
  
void
Trader_Client::init (int argc, char** argv)
{
  TAO_TRY
    {
      // Initialize the orb.
      this->orb_manager_.init (argc, argv, TAO_TRY_ENV);

      // Bootstrap to the Trading Service.
      ACE_DEBUG ((LM_DEBUG, "Bootstrapping to the trading service"));
      CORBA::ORB_var orb = this->orb_manager_.orb ();
      CORBA::Object_var obj =
        orb->resolve_initial_references ("TradingService");
      
      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, "Narrowing the lookup interface.\n"));
      this->lookup_ =  CosTrading::Lookup::_narrow (obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Failed to initialize the ORB and bootstrap to"
                  " the Trading Service. Wah!\n"));
    }
  TAO_ENDTRY;
}

void
Trader_Client::query_trader (void)
{
  // This code is copied nearly verbatim from
  // orbsvcs/tests/Trading/Offer_Importer.cpp, where its not well
  // documented either. 
  TAO_TRY
    {
      CosTrading::OfferSeq_ptr offer_seq_ptr = 0;
      CosTrading::OfferIterator_ptr offer_iterator_ptr = 0;
      CosTrading::PolicyNameSeq_ptr limits_applied_ptr = 0;
      CosTrading::Lookup::SpecifiedProps desired_props;

      // Set the query-specific policies.
      TAO_Policy_Manager policies;
      policies.exact_type_match (CORBA::B_FALSE);
      policies.search_card (16);
      policies.match_card (16);
      policies.return_card (16);
      policies.link_follow_rule (CosTrading::always);
      desired_props.all_ (CORBA::B_TRUE);
      
      CosTrading::OfferSeq_out offer_seq_out (offer_seq_ptr);
      CosTrading::OfferIterator_out offer_iterator_out (offer_iterator_ptr);
      CosTrading::PolicyNameSeq_out limits_applied_out (limits_applied_ptr);

      ACE_DEBUG ((LM_DEBUG, "Querying trader...\n"));
      this->lookup_->query ("MMDevice",
                            "Number_Of_Connections < Max_Connections",
                            "min Number_Of_Connections",
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
      
      if (offer_seq_ptr != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Hashing the returned offers.\n"));
          CORBA::ULong length = 0, i = 0;
          for (length = offer_seq->length (), i = 0; i < length; i++)
            this->create_hash_table_entry (offer_seq[i]);
        }
      
      if (! CORBA::is_nil (offer_iterator_ptr))
        {
          // Hash those offers obtained from the Offer_Iterator. 
          CORBA::Boolean any_left = CORBA::B_FALSE;
          
          do
            {
              CosTrading::OfferSeq_ptr iter_offers_ptr;
              CosTrading::OfferSeq_out iter_offers_out (iter_offers_ptr);
              
              any_left = offer_iterator->next_n (8,
                                                 iter_offers_out,
                                                 TAO_TRY_ENV);
              TAO_CHECK_ENV;
              
              CosTrading::OfferSeq_var iter_offers (iter_offers_ptr);
              for (CORBA::ULong length = iter_offers->length (), i = 0; i < length; i++)
                this->create_hash_table_entry (iter_offers[i]);
            }
          while (any_left);
          
          offer_iterator->destroy (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCHANY {}
  TAO_ENDTRY;
}

void
Trader_Client::load_movie (const char* server_name, const char* movie_name)
{
  // Open a unix socket to the controller process of the AV client,
  // and pass in the IOR of the selected server and the movie_name.

  // Of course, we aren't actually saving the object reference
  // portion of the offer at this point. But genius that you are,
  // you'll figure out some way to retrofit this code with the
  // required elements. I believe in you. 
}
  
void
Trader_Client::get_servers (int& length, char**& server_names)
{
  ACE_DEBUG ((LM_DEBUG, "Trader_Client::get_servers.\n"));
  
  length = this->map_.current_size ();
  server_names = new char* [length];

  if (server_names != 0)
    {
      CORBA::ULong i = 0;
      for (Server_Map::ITERATOR server_iter (this->map_);
           ! server_iter.done ();
           server_iter.advance (), i++)
        {
          ACE_DEBUG ((LM_DEBUG, " adding server: %s\n", (*server_iter).ext_id_.in ()));
          server_names[i] =
            ACE_const_cast (char*, (*server_iter).ext_id_.in ());
        }
    }
}
  
Movie_Iterator*
Trader_Client::get_movie_info (const char* server_name)
{
  ACE_DEBUG ((LM_DEBUG, "Trader_Client: Get Movie Info\n"));
  Movie_Iterator* movie_iter = 0;
  TAO_String_Hash_Key server_name_key (server_name);

  // Find the entry for the designated server.
  Server_Map::ENTRY* server_entry = 0;
  if (this->map_.find (server_name_key, server_entry) == 0)
    {
      // Locate the movie_info property.
      Property_Map::ENTRY* prop_entry = 0;
      Property_Map* prop_map = server_entry->int_id_;
      if (prop_map->find ("Movie_Info", prop_entry) == 0)
        {
          // Make a fake sequence out of the property for use in the
          // TAO_Property_Evaluator utility class. 
          CosTrading::Property& property = prop_entry->int_id_;
          CosTrading::PropertySeq property_seq (1, 1, &property, CORBA::B_FALSE);
          TAO_Property_Evaluator prop_eval (property_seq);

          TAO_TRY
            {
              // Extact the Movie_Info sequence.
              TAO_VR::Movie_Info* movie_info = 0;
              CORBA::Any* movie_prop = prop_eval.property_value (0, TAO_TRY_ENV);
              TAO_CHECK_ENV;

              // Create the movie iterator.
              if (((*movie_prop) >>= movie_info) == CORBA::B_TRUE)
                movie_iter = new Movie_Iterator (*movie_info);
            }
          TAO_CATCHANY {}
          TAO_ENDTRY;
        }
    }

  return movie_iter;
}

float
Trader_Client::evaluate_performance_property (const char* server_name,
                                              const char* property_name)
{
  float return_value = 0.0;
  Server_Map::ENTRY* server_entry = 0;
  TAO_String_Hash_Key server_name_key (server_name);

  // Locate the property and evaluate it. <--- vivid description, eh?
  if (this->map_.find (server_name_key, server_entry) == 0)
    {
      Property_Map* prop_map = server_entry->int_id_;
      TAO_String_Hash_Key prop_name_key (property_name);
      Property_Map::ENTRY* prop_entry = 0;
      
      if (prop_map->find (prop_name_key, prop_entry) == 0)
        {
          CosTrading::Property& property = prop_entry->int_id_;
          CosTrading::PropertySeq property_seq (1, 1, &property, CORBA::B_FALSE);
          TAO_Property_Evaluator prop_eval (property_seq);
          
          TAO_TRY
            {
              CORBA::Any* value = prop_eval.property_value (0, TAO_TRY_ENV);
              TAO_CHECK_ENV;
              
              CORBA::Float prop_value = 0.0;
              (*value) >>= prop_value;
            }
          TAO_CATCHANY {}
          TAO_ENDTRY;
        }
    }

  return return_value;
}

void
Trader_Client::create_hash_table_entry (CosTrading::Offer& offer)
{
  Property_Map* prop_map = 0;
  ACE_NEW (prop_map, Property_Map);

  ACE_DEBUG ((LM_DEBUG, "Hashing an offer.\n"));
  for (CORBA::ULong length = offer.properties.length (), i = 0;
       i < length; i++)
    {
      TAO_String_Hash_Key prop_name (offer.properties[i].name);
      ACE_DEBUG ((LM_DEBUG, "Hashing property %s.\n", prop_name.in ()));
      prop_map->bind (prop_name, offer.properties[i]);
    }

  Property_Map::ENTRY* entry = 0;
  TAO_String_Hash_Key server_name ("Server_Name");
  if (prop_map->find (server_name, entry) == 0)
    {
      char* server_name_string = 0;
      entry->int_id_.value >>= server_name_string;
      this->map_.bind (server_name_string, prop_map);

      ACE_DEBUG ((LM_DEBUG, "Bound hash map entry for %s\n",
                  server_name_string));
    }
  else
    delete prop_map;
}

TAO_VR::Movie_Info*
Trader_Client::fetch_movie_array (const char* server_name)
{
  TAO_VR::Movie_Info* movie_info = 0;
  Server_Map::ENTRY* server_entry = 0;
  TAO_String_Hash_Key server_name_str (server_name);
  if (this->map_.find (server_name, server_entry) == 0)
    {
      Property_Map::ENTRY* prop_entry = 0;
      Property_Map* prop_map = server_entry->int_id_;
      if (prop_map->find ("Movie_Info", prop_entry) == 0)
        {
          TAO_VR::Movie_Info* movie_info = 0;
          prop_entry->int_id_.value >>= movie_info;
        }
    }

  return movie_info;
}
