// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//     TAO
// 
// = FILENAME
//     client_factory.h
//
// = AUTHOR
//     Chris Cleeland
//
// = VERSION
//     $Id$
// ============================================================================

#if !defined (TAO_CLIENT_FACTORY_H)
#  define TAO_CLIENT_FACTORY_H

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
};

#endif /* TAO_CLIENT_FACTORY_H */
