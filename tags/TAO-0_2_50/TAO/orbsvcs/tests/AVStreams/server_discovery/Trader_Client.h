// ========================================================================
// $Id$
//
// = FILENAME
//    Trader_Client.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#ifndef TRADER_CLIENT_H
#define TRADER_CLIENT_H

#include "tao/TAO.h"
#include "Video_RepositoryC.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Trader_Utils.h"
#include "mpeg_shared/ReceiverC.h"
#include "mpeg_shared/MMDevice_ExporterC.h"

class Movie_Iterator
{
public:

  Movie_Iterator (const TAO_VR::Movie_Info& movie_info)
    : movie_info_ (movie_info), index_ (0) {}

  int has_more_elements (void) const
    { return this->index_ < this->movie_info_.length (); }

  void advance (void)
    { this->index_++; }

  int num_movies (void) const
    { return this->movie_info_.length (); }

  const char* name (void) const
    { return this->movie_info_[this->index_].name_.in (); }

  const char* description (void) const
    { return this->movie_info_[this->index_].description_.in (); }

  const char* audio_filename (void) const
    { return this->movie_info_[this->index_].audio_filename_.in (); }

  const char* video_filename (void) const
    { return this->movie_info_[this->index_].video_filename_.in (); }

/*   const char* category (void) const */
/*     { return this->movie_info_[this->index_].category_.in (); } */

private:

  int index_;

  TAO_VR::Movie_Info movie_info_;
};

class Trader_Client
// = TITLE
//   Performs all the interaction with the Trading Service on behalf
//   of the Java server selection GUI.
{
public:

  ~Trader_Client (void);
  // Destory all that was dynamically allocated.

  void init (int argc, char** argv);
  // Call once, prior to query_trader.

  void query_trader (void);
  // Call n times, after init has been called.

  void load_movie (const char* server_name, TAO_VR::Movie* selected_movie_info);
  // Instruct the A/V client to begin an A/V stream with the
  // designated host.

  void get_servers (int& length, char**& server_names);
  // Return the list of server names whose offers were obtained from
  // the Trader.

  Movie_Iterator* get_movie_info (const char* server_name);
  // Fill in the sequence with movie information for all movies
  // offered by the specified server.

  float evaluate_performance_property (const char* server_name,
                                       const char* property_name);
  // Evaluate the specified property with respect to the offer from
  // the specified server.

  static Trader_Client* instance (void);

protected:

  Trader_Client (void);
  // No-arg, no-op constructor. Necessary since this class is
  // instantiated in a no-arg factory method.

private:

  static Trader_Client* instance_;

  Receiver_var receiver_;
  // Command handler receiver for mmdevice.

  void create_hash_table_entry (CosTrading::Offer& offer);


  TAO_VR::Movie_Info* fetch_movie_array (const char* server_name);

  typedef
  ACE_Hash_Map_Manager <TAO_String_Hash_Key, CosTrading::Property, ACE_Null_Mutex>
  Property_Map;
  // Second-tier map --- maps property names to their values.

  typedef
    ACE_Hash_Map_Manager <TAO_String_Hash_Key, Property_Map*, ACE_Null_Mutex>
  Server_Map;
  // First tier map --- maps server names to their second-tier maps.

  typedef
    ACE_Hash_Map_Manager <TAO_String_Hash_Key, CORBA::Object_ptr, ACE_Null_Mutex> Reference_Map;
  // First tier map --- maps server names to their object reference.

  TAO_ORB_Manager orb_manager_;
  // Standard orb stuff.

  CosTrading::Lookup_var lookup_;
  // A reference to the trader, what else?

  Server_Map mmdevice_map_;
  // The two-tiered map of server names to a map of property names to
  // values.

  Reference_Map mmdevice_reference_map_;
  // Hash Map of mmdevice object references

  //  Server_Map video_map_;
  // The two-tiered map of server names to a map of property names to
  // values.

  //  Reference_Map video_reference_map_;
  // Hash Map of mmdevice object references
};


// Movie_Iterator* movie_iter=0;


#endif /* TRADER_CLIENT_H */




