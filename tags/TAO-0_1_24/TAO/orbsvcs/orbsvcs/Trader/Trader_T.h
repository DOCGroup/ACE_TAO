/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Trader.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
//   
// ========================================================================

#ifndef TAO_TRADER_H
#define TAO_TRADER_H

// ACE includes
#include "Trader.h"
#include "Monitor.h"
#include "Service_Type_Map.h"

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
class TAO_Trader : public TAO_Trader_Base
  //
  // = TITLE
  //     This class packages together all the various pieces that
  //     provide functionality specified in COS Trading specification.
  //
  // = DESCRIPTION
  //     TAO_Trader contains all the components that together represent
  //     a single trader.  Based on its constructor arguments, 
  //     TAO_Trader creates instances of appropriate interface 
  //     implementations as well as instances of objects common to
  //     more than one interface (offers, attributes, etc.).
  //     TAO_Trader also enforces the proper order on all
  //     initializations.  TAO_Trader acts like a "glue" class that
  //     creates appropriate components, holds everything together,
  //     and enforces order. TAO_Trader is parameterized by two types
  //     of locks: one for its service service offers, one for its
  //     state (configuration). 
{    
public:

  enum Trader_Components
  { 
    LOOKUP = 0x001,  
    REGISTER = 0x002,
    LINK = 0x004,
    PROXY = 0x008,
    ADMIN = 0x010
  };
  // The desired combination of interfaces to be passed to the 
  // TAO_Trader constructor.

  typedef TAO_Service_Type_Map<MAP_LOCK_TYPE> SERVICE_TYPE_MAP;
  typedef SERVICE_TYPE_MAP::Local_Offer_Iterator LOCAL_OFFER_ITER;
  typedef MAP_LOCK_TYPE LOCK_TYPE;

  TAO_Trader (Trader_Components components = LOOKUP);
  // Constructor which based on its arguments will create 
  // a particular type of trader (e.g. Query trader, Simple trader, etc.)
  // The argument is a bitwise OR of desired Trader_Components as listed
  // in enumerated type above.

  ~TAO_Trader (void);
  // destructor.
  
  SERVICE_TYPE_MAP& service_type_map (void);
  // Accessor for the structure with all the service offers.

  ACE_Lock &lock (void);
  // returns the trader
  
protected:

  typedef TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE> TRADER_SELF;
  
  SERVICE_TYPE_MAP service_type_map_; 
  // A monitor (i.e. an STL map + a lock) serving as a storage for
  // all the service offers of a trader.
  // Structure: a map (actually a monitor) of service type names to  
  // a counter + a map (a monitor) of offers for that service type.  A map of offers for 
  // a service type (an internal map) is  a mapping from a number ids to offers.

  ACE_Lock_Adapter<TRADER_LOCK_TYPE> lock_;
  // lock that guards the state of the trader (its configuration).

  enum { LOOKUP_IF, REGISTER_IF, ADMIN_IF, PROXY_IF, LINK_IF };
  
  PortableServer::ServantBase* ifs_[5];
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Trader_T.cpp"
#endif  /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* ACE_TRADER_H */
	
