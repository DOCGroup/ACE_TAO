// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
// 
// = FILENAME
//     Client_Strategy_Factory.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#if !defined (TAO_CLIENT_STRATEGY_FACTORY_H)
#  define TAO_CLIENT_STRATEGY_FACTORY_H

class TAO_Client_Strategy_Factory : public ACE_Service_Object
  // = TITLE
  //    Abstract factory used by the client to manufacture various
  //    strategies used on the client-side.
{
public:
  // = Intialization and termination methods.
  TAO_Client_Strategy_Factory (void);
  // Constructor.
  
  virtual ~TAO_Client_Strategy_Factory (void);
  // Destructor

  virtual ACE_Lock* create_iiop_profile_lock (void);
  // create the lock for the forwarding IIOP Profile used by 
  // the TAO_GIOP_Invocation::location_forward and the 
  // TAO_GIOP_Invocation::start
};

#endif /* TAO_CLIENT_STRATEGY_FACTORY_H */
