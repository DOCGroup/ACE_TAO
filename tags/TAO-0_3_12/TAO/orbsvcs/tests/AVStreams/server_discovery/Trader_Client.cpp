
// $Id$
// ========================================================================
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
  for (Server_Map::ITERATOR mmdevice_map_iter (this->mmdevice_map_);
       ! mmdevice_map_iter.done ();
       mmdevice_map_iter.advance ())
    {
      Server_Map::ENTRY* mmdevice_map_entry = 0;
      mmdevice_map_iter.next (mmdevice_map_entry);
      delete mmdevice_map_entry->int_id_;
    }

}

void
Trader_Client::init (int argc, char** argv)
{

  cout<<"Within the Init method of Trader_Client"<<endl;
  TAO_TRY
    {
      // Initialize the orb.
      this->orb_manager_.init (argc, argv, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      // Bootstrap to the Trading Service.
      //ACE_DEBUG ((LM_DEBUG, "Bootstrapping to the trading service"));
      CORBA::ORB_var orb = this->orb_manager_.orb ();
      CORBA::Object_var obj =
        orb->resolve_initial_references ("TradingService");

      // Narrow the lookup interface.
      //ACE_DEBUG ((LM_DEBUG, "Narrowing the lookup interface.\n"));
      this->lookup_ =  CosTrading::Lookup::_narrow (obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      // Lookup the Receiver interface from a file.

      char receiver_file[BUFSIZ];
      ACE_OS::sprintf (receiver_file,
                       "/tmp/receiver_ior.%d",
                       getuid ());
      FILE *fp = ACE_OS::fopen (receiver_file,"r");
      if (fp == 0)
        ACE_ERROR ((LM_ERROR,"Unable to read receiver ior\n"));
      char ior[BUFSIZ];
      ACE_OS::fgets (ior,BUFSIZ,fp);
      ACE_DEBUG ((LM_DEBUG,"receiver ior is :%s\n",ior));
      ACE_OS::fclose (fp);
      CORBA::Object_var receiver_obj = orb->string_to_object (ior,
                                                              TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->receiver_ = Receiver::_narrow (receiver_obj,TAO_TRY_ENV);
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
      CosTrading::OfferSeq_ptr mmdevice_offer_seq_ptr = 0;
      CosTrading::OfferIterator_ptr mmdevice_offer_iterator_ptr = 0;
      CosTrading::PolicyNameSeq_ptr mmdevice_limits_applied_ptr = 0;
      CosTrading::Lookup::SpecifiedProps desired_props;


      // Set the query-specific policies.
      TAO_Policy_Manager policies;
      policies.exact_type_match (0);
      policies.search_card (16);
      policies.match_card (16);
      policies.return_card (16);
      policies.link_follow_rule (CosTrading::always);
      desired_props.all_ (1);

      CosTrading::OfferSeq_out mmdevice_offer_seq_out (mmdevice_offer_seq_ptr);
      CosTrading::OfferIterator_out mmdevice_offer_iterator_out (mmdevice_offer_iterator_ptr);
      CosTrading::PolicyNameSeq_out mmdevice_limits_applied_out (mmdevice_limits_applied_ptr);

      //ACE_DEBUG ((LM_DEBUG, "Querying trader...\n"));
      this->lookup_->query ("MMDevice",
                            "Number_Of_Connections < Max_Connections",
                            "min Number_Of_Connections",
                            policies.policy_seq (),
                            desired_props,
                            8,
                            mmdevice_offer_seq_out,
                            mmdevice_offer_iterator_out,
                            mmdevice_limits_applied_out,
                            TAO_TRY_ENV);
      TAO_CHECK_ENV;

      
      CosTrading::OfferSeq_var mmdevice_offer_seq (mmdevice_offer_seq_ptr);
      CosTrading::OfferIterator_var mmdevice_offer_iterator (mmdevice_offer_iterator_ptr);
      CosTrading::PolicyNameSeq_var mmdevice_limits_applied (mmdevice_limits_applied_ptr);

      if (mmdevice_offer_seq_ptr != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Hashing the returned offers.\n"));
          CORBA::ULong length = 0, i = 0;
          for (length = mmdevice_offer_seq->length (), i = 0; i < length; i++)
            {
              this->create_hash_table_entry (mmdevice_offer_seq[i]);
            }
        } 

      if (! CORBA::is_nil (mmdevice_offer_iterator_ptr))
        {
          // Hash those offers obtained from the Offer_Iterator.
          CORBA::Boolean any_left = 0;

          do
            {
              CosTrading::OfferSeq_ptr iter_offers_ptr;
              CosTrading::OfferSeq_out iter_offers_out (iter_offers_ptr);

              any_left = mmdevice_offer_iterator->next_n (8,
                                                 iter_offers_out,
                                                 TAO_TRY_ENV);
              TAO_CHECK_ENV;

              CosTrading::OfferSeq_var iter_offers (iter_offers_ptr);
              for (CORBA::ULong length = iter_offers->length (), i = 0; i < length; i++)
                this->create_hash_table_entry (iter_offers[i]);
            }
          while (any_left);

          mmdevice_offer_iterator->destroy (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }

            
    }

  TAO_CATCHANY {}
  TAO_ENDTRY;
}

void
Trader_Client::load_movie (const char* mmdevice_server_name, 
                           TAO_VR::Movie* selected_movie_info)
{
  // Open a unix socket to the controller process of the AV client,
  // and pass in the IOR of the selected server and the movie_name.

  // Of course, we aren't actually saving the object reference
  // portion of the offer at this point. But genius that you are,
  // you'll figure out some way to retrofit this code with the
  // required elements. I believe in you.

  ACE_DEBUG ((LM_DEBUG,"Trader_Client::load_movie"));
  TAO_TRY
    {
      // Get the server mmdevice object from the hash table.
      Reference_Map::ENTRY* mmdevice_entry = 0;
      TAO_String_Hash_Key server_key (mmdevice_server_name);
      if (this->mmdevice_reference_map_.find (server_key,mmdevice_entry) == 0)
        {
          // ACE_DEBUG ((LM_DEBUG,"object reference for %s found",server_name));

          CORBA::Object_ptr mmdevice_reference = mmdevice_entry->int_id_;
          CORBA::String_var server_ior = this->orb_manager_.orb ()->object_to_string (mmdevice_reference,
                                                                                      TAO_TRY_ENV); 
          cout<<"The audio file name "<<selected_movie_info->audio_filename_<<endl;
          cout<<"The video file name "<<selected_movie_info->video_filename_<<endl;
          ACE_DEBUG ((LM_DEBUG,"mmdevice ior is :%s\n",server_ior.in ()));
          // CHANGE THIS TO push_mmdevice
          MMDevice_Exporter_ptr mmdevice_exporter_reference  = MMDevice_Exporter::_narrow (mmdevice_reference,TAO_TRY_ENV);
          TAO_CHECK_ENV;
          CORBA::Object_ptr audio_object_ptr = mmdevice_exporter_reference->get_audio_mmdevice (TAO_TRY_ENV);
          TAO_CHECK_ENV;
          CORBA::Object_ptr video_object_ptr = mmdevice_exporter_reference->get_video_mmdevice (TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CORBA::String_var audio_ior = this->orb_manager_.orb ()->object_to_string (audio_object_ptr,
                                                                                      TAO_TRY_ENV); 
          TAO_CHECK_ENV;
          ACE_DEBUG ((LM_DEBUG,"AUDIO mmdevice ior is :%s\n",audio_ior.in ()));
          CORBA::String_var video_ior = this->orb_manager_.orb ()->object_to_string (video_object_ptr,
                                                                                      TAO_TRY_ENV); 
          TAO_CHECK_ENV;
          ACE_DEBUG ((LM_DEBUG,"VIDEO mmdevice ior is :%s\n",video_ior.in ()));

          this->receiver_->push_mmdevice (audio_object_ptr,
                                          selected_movie_info->audio_filename_,
                                          video_object_ptr,
                                          selected_movie_info->video_filename_,
                                          TAO_TRY_ENV);
         
          TAO_CHECK_ENV;
        }

      
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,"push_audio_mmdevice failed\n"));
    }
  TAO_ENDTRY;
  return;
}

void
Trader_Client::get_servers (int& length, char**& server_names)
{
  ACE_DEBUG ((LM_DEBUG, "Trader_Client::get_servers.\n"));

  length = this->mmdevice_map_.current_size ();
  server_names = new char* [length];

  if (server_names != 0)
    {
      CORBA::ULong i = 0;
      for (Server_Map::ITERATOR server_iter (this->mmdevice_map_);
           ! server_iter.done ();
           server_iter.advance (), i++)
        {
          //ACE_DEBUG ((LM_DEBUG, " adding server: %s\n", (*server_iter).ext_id_.in ()));
          server_names[i] =
            ACE_const_cast (char*, (*server_iter).ext_id_.in ());
        }
    }
}

Movie_Iterator*
Trader_Client::get_movie_info (const char* server_name)
{
  ACE_DEBUG ((LM_DEBUG, "Trader_Client: Get Movie Info Outside\n"));
  Movie_Iterator* movie_iter = 0;
  TAO_String_Hash_Key server_name_key (server_name);

  cout<<"Within Tader Client Movie Info Out side"<<endl;

  // Find the entry for the designated server.
  Server_Map::ENTRY* server_entry = 0;
  if (this->mmdevice_map_.find (server_name_key, server_entry) == 0)
    {
      cout<<"Within Tader Client Movie Info"<<endl;
      ACE_DEBUG ((LM_DEBUG,"%s:find successful\n",server_name));
      // Locate the movie_info property.
      Property_Map::ENTRY* prop_entry = 0;
      Property_Map* prop_map = server_entry->int_id_;
      if (prop_map->find ("Movie_Info", prop_entry) == 0)
        {
	  ACE_DEBUG ((LM_DEBUG,"Movie_Info find successful\n"));
          // Make a fake sequence out of the property for use in the
          // TAO_Property_Evaluator utility class.
          CosTrading::Property& property = prop_entry->int_id_;
	  //ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
          CosTrading::PropertySeq property_seq (1, 1, &property, 0);
	  //ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
          TAO_Property_Evaluator prop_eval (property_seq);
	  //ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
          TAO_TRY
            {
              // Extact the Movie_Info sequence.
              TAO_VR::Movie_Info* movie_info = 0;
	      //ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
              CORBA::Any* movie_prop = prop_eval.property_value (0, TAO_TRY_ENV);
	      //ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
              TAO_CHECK_ENV;

	      if (movie_prop == 0)
		ACE_ERROR_RETURN ((LM_ERROR,"Movie_property is null\n"),0);
	      //ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
              // Create the movie iterator.
              if (((*movie_prop) >>= movie_info) == 1)
		{
		  ACE_DEBUG ((LM_DEBUG,"%s,%d\n",__FILE__,__LINE__));
		  movie_iter = new Movie_Iterator (*movie_info);
		}
            }
          TAO_CATCHANY {
	    ACE_ERROR_RETURN ((LM_ERROR,"property_value failed\n"),0);
	  }
          TAO_ENDTRY;
        }
      else
	ACE_DEBUG ((LM_DEBUG,"Movie_Info find unsuccessful\n"));
    }
  else
    {
      cout<<"Find unsuccesful"<<endl;
      ACE_DEBUG ((LM_DEBUG,"%s:find successful\n",server_name));
    }
  if(movie_iter!=0) cout<<"The movie iterator is not null"<<endl;
  else  cout<<"The movie iterator is null"<<endl;
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
  if (this->mmdevice_map_.find (server_name_key, server_entry) == 0)
    {
      cout<<"Within Trader Client evaluuate performance"<<endl;
      Property_Map* prop_map = server_entry->int_id_;
      TAO_String_Hash_Key prop_name_key (property_name);
      Property_Map::ENTRY* prop_entry = 0;

      if (prop_map->find (prop_name_key, prop_entry) == 0)
        {
          CosTrading::Property& property = prop_entry->int_id_;
          CosTrading::PropertySeq property_seq (1, 1, &property, 0);
          TAO_Property_Evaluator prop_eval (property_seq);

          TAO_TRY
            {
              CORBA::Any* value = prop_eval.property_value (0, TAO_TRY_ENV);
              TAO_CHECK_ENV;

              CORBA::Float prop_value = 0.0;
              (*value) >>= prop_value;
              (*value) >>= return_value;
              cout<<"The property Value is"<<property_name<<"  "<<return_value<<endl;
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
//   //  CosTrading::Offer offer(offer_src);
//   CosTrading::Offer *offer;
//   ACE_NEW (offer,
//            CosTrading::Offer (offer_src));
      
  Property_Map* prop_map = 0;
  ACE_NEW (prop_map, Property_Map);
  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG, "Hashing an offer.\n"));
      for (CORBA::ULong length = offer.properties.length (), i = 0;
           i < length; i++)
        {
          TAO_String_Hash_Key prop_name (offer.properties[i].name);
          ACE_DEBUG ((LM_DEBUG, "Hashing property %s.\n", prop_name.in ()));
          prop_map->bind (prop_name, offer.properties[i]);
          /*
          TAO_VR::Movie_Info* movie_info=0;
          const char* property_name=(const char*)prop_name;
          if(strcmp(property_name,"Movie_Info")==0)
          {
            //cout<<"The property is "<<property_name<<endl;
            if ((offer.properties[i].value >>= movie_info) == 1)
            {

              Movie_Iterator* movie=new Movie_Iterator(*movie_info);
              cout<<"The movie name is"<<movie->name()<<endl;
            }
            else cout<<"Movie Info not found"<<endl;
          }
          */  
        }

      Property_Map::ENTRY* entry = 0;
      TAO_String_Hash_Key server_name ("Server_Name");
      char* server_name_string = 0;
      if (prop_map->find (server_name, entry) == 0)
        {
          entry->int_id_.value >>= server_name_string;
          this->mmdevice_map_.bind (server_name_string, prop_map);
          
          TAO_String_Hash_Key server_reference_key (server_name_string);
          CORBA::String_var server_ior = this->orb_manager_.orb ()->object_to_string (offer.reference,
                                                                                      TAO_TRY_ENV);
          TAO_CHECK_ENV;
         
          this->mmdevice_reference_map_.bind (server_reference_key,
                                              CORBA::Object::_duplicate (offer.reference));
        }
      else
        delete prop_map;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("object_to_String");
      return;
    }
  TAO_ENDTRY;
}

TAO_VR::Movie_Info*
Trader_Client::fetch_movie_array (const char* server_name)
{
  TAO_VR::Movie_Info* movie_info = 0;
  Server_Map::ENTRY* server_entry = 0;
  TAO_String_Hash_Key server_name_str (server_name);
  if (this->mmdevice_map_.find (server_name, server_entry) == 0)
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
