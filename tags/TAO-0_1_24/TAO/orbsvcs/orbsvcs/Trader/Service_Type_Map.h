/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Service_Type_Map.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_SERVICE_TYPE_MAP_H
#define TAO_SERVICE_TYPE_MAP_H

#include "Trader.h"
#include "Monitor.h"
#include "stl.h"

class TAO_Offer_Id_Iterator;

template <class LOCK_TYPE>
class TAO_Service_Type_Map
// = DESCRIPTION
//   The TAO_Service_Type_Map encapsulates the mapping of service
//   types to those offers exported with that service types. The
//   underlying structure is a map of maps. The first maps maps the
//   service type name to a map of exported offers. The second map
//   maps the identifying index for that offer within the service
//   types. So a service type name and an index uniquely identifies an 
//   exported offer. In fact, when the register export interface
//   returns a CosTrading::OfferId, it's returning no more than a
//   simple string concatenation of these two values. In addition to
//   all these wonderful things, the TAO_Service_Type_Map has built-in 
//   locking, one reader/writer-style lock for modifying the top-level 
//   map and a reader/writer-style for each of the offer
//   maps. Needless to say the locks are acquired when the
//   TAO_Service_Type_Map performs operations on the structures they
//   guard.
//
//   NOTE: TAO_Service_Type_Map needs to be parameterized by a
//   READER/WRITER LOCK, a RECURSIVE MUTEX, or a NULL MUTEX, not a
//   simple binary mutex! Mutexes will cause deadlock when you try to
//   contruct an iterator (which acquires a read lock on the map under
//   an existing read lock). Just don't do it, ok?
{
public:

  typedef int HUGE_NUMBER;
  typedef map<HUGE_NUMBER, CosTrading::Offer, less<HUGE_NUMBER> > OFFERS;
  typedef TAO_Monitor<OFFERS, LOCK_TYPE> OFFER_MAP;
  typedef pair<OFFER_MAP, HUGE_NUMBER> OFFER_MAP_PLUS_COUNTER;
  typedef TAO_Monitor <map <string, OFFER_MAP_PLUS_COUNTER, less<string> >, LOCK_TYPE> SERVICE_TYPE_MAP;
  
  class Local_Offer_Iterator;
  class Random_Offer_Locator;
  
  TAO_Service_Type_Map (void);
  // No arg constructor.
  
  Local_Offer_Iterator* get_offers (const char* type);
  // Return an iterator for traversing the list of offers whose type
  // is <type>, or 0 if the type doesn't exist.
  
  int add_type (const char* type);
  // Add a new service type. Returns 0 on success (that is, the type
  // was found), -1 on failure (the type wasn't found).
  
  int remove_type (const char* type);
  // Remove a service type. returns 0 on success (the type was found
  // and removed), and -1 on failure (the type wasn't found).
  
  CosTrading::OfferId insert_offer (const char* type,
				    const CosTrading::Offer& offer);
  // Add an offer of type <type> and generate a CosTrading::OfferId
  // for it. Returns 0 on failure.
  
  int remove_offer (const CosTrading::OfferId offer_id,
		    CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::IllegalOfferId,
		    CosTrading::UnknownOfferId));

  int remove_offer (const char* type, HUGE_NUMBER id);
  // Remove an offers whose id is <offer_id>. Returns 0 on success, -1 
  // on failure, and throws a CosTrading::IllegalOfferId if it can't
  // parse the CosTrading::OfferId.

  
  CosTrading::Offer* lookup_offer (const CosTrading::OfferId offer_id,
				   CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::IllegalOfferId,
		    CosTrading::UnknownOfferId));
  // Lookup an offer whose offer_id is <offer_id>, and return
  // it. Otherwise, throw the appropriate exception.

  CosTrading::Offer* lookup_offer (const CosTrading::OfferId offer_id,
				   char*& type_name,
				   CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::IllegalOfferId,
		    CosTrading::UnknownOfferId));
  // Lookup an offer whose OfferId is <offer_id> and return in
  // <type_name> the type name of the object. Type name is just a
  // pointer to a location in offer_id, so DON'T DELETE IT.

  CosTrading::Offer* lookup_offer (const char* type, HUGE_NUMBER id);
  // Lookup an offer whose type is <type> and id, <id>. Return 0 on
  // failure. 

  Random_Offer_Locator* lookup_offers (const char* type);
  // Return a random access iterator to the set of offers of type
  // <type>.

  TAO_Offer_Id_Iterator* retrieve_all_offer_ids (void);
  // Return an iterator that will traverse and return all the offer
  // ids in the service type map.

  typedef CosTrading::ServiceTypeName TYPE_NAME;
  typedef CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq TYPE_NAME_SEQ;
  TYPE_NAME_SEQ* list_all_types (void);
  
  class Local_Offer_Iterator
    // = TITLE
    //   Local_Offer_Iterator iterates over the set of exported
    //   offers for a given type. Handily, it takes care of all the
    //   necessary locking, acquiring them in the constructor, and
    //   releasing them in the destructor. Since Local_Offer_Iterator
    //   is dynamically allocated by TAO_Offer_Id_Iterator, make sure
    //   to EXPLICITLY CALL DELETE when you're done with it.
  {
    friend TAO_Service_Type_Map<LOCK_TYPE>;
  public:
    
    ~Local_Offer_Iterator (void);
    // Release all the locks acquired.
    
    int has_more_offers (void);
    // Returns 1 if there are more offers, 0 otherwise.
    
    CosTrading::OfferId get_id (void);
    // Get the id for the current offer.
    
    CosTrading::Offer* get_offer (void);
    // Returns the next offer in the series.
    
    void next_offer (void);
    // Advances the iterator 1.
    
  private:

    Local_Offer_Iterator (const char* type,
			  SERVICE_TYPE_MAP& stm,
			  OFFER_MAP& offer_map);
    // Protected constructor.
    
    OFFER_MAP& offer_map_;
    // The underlying map of offers.
    
    SERVICE_TYPE_MAP& stm_;
    // The top level map on which we're going to acquire a read lock.
    
    OFFER_MAP::iterator offer_iter_;
    //

    const char* type_;
  };

  class Random_Offer_Locator
    // = TITLE
    //   Random_Offer_Locator locates offers in a set of exported
    //   offers for a given type. Handily, it takes care of all the
    //   necessary locking, acquiring them in the constructor, and
    //   releasing them in the destructor. Since Random_Offer_Locator
    //   is dynamically allocated by TAO_Offer_Id_Iterator, make sure
    //   to EXPLICITLY CALL DELETE when you're done with it.

  {
    friend TAO_Service_Type_Map<LOCK_TYPE>;
  public:
    
    ~Random_Offer_Locator (void);
    // Release all the read locks acquire by the constructor.
    
    CosTrading::Offer* get_offer (HUGE_NUMBER id);
    // Returns the offer in the series of id <id>
    
  private:
    
    Random_Offer_Locator (SERVICE_TYPE_MAP& stm,
			  OFFER_MAP& offer_map);
    
    OFFER_MAP& offer_map_;
    SERVICE_TYPE_MAP& stm_;
    OFFER_MAP::iterator offer_iter_;
  };

  // Should be private:
  
  static char* generate_offer_id (const char *service_type_name,
				  HUGE_NUMBER id);
  // Take in a service type name for the offer the current value of 
  // of the counter and generate an offer id.
  
private:
  
  static void parse_offer_id (const char *offer_id,
			      char* &service_type,
			      HUGE_NUMBER& id,
			      CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::IllegalOfferId));
  // Take in a previously generated offer id and return the type
  // and id that were used to generate the offer id.  

  SERVICE_TYPE_MAP type_map_;
  // The protected data structure.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Service_Type_Map.cpp"
#endif  /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_SERVICE_TYPE_MAP_H */
