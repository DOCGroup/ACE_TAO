// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//     default_client.h
//
// = AUTHOR
//     Chris Cleeland
//
// = VERSION
//     $Id$
// ============================================================================

#if !defined (TAO_DEFAULT_CLIENT_H)
#  define TAO_DEFAULT_CLIENT_H

#include "tao/corba.h"

class TAO_Default_Client_Strategy_Factory : public TAO_Client_Strategy_Factory
  // = TITLE
  //   This is the "default" client strategy factor for TAO.  It
  //   includes strategies that are configured through command-line
  //   options so that everything operates as if there were no
  //   dynamically-linkable strategies.
{
public:
  // = Initialization and termination methods.
  TAO_Default_Client_Strategy_Factory (void);
  // Constructor.

  virtual ~TAO_Default_Client_Strategy_Factory (void);
  // Destructor.
  
  // = Service Configurator hooks.
  virtual int init (int argc, char* argv[]);
  // Dynamic linking hook

  int parse_args(int argc, char* argv[]);
  // Parse svc.conf arguments
};

ACE_SVC_FACTORY_DECLARE (TAO_Default_Client_Strategy_Factory)

#endif /* TAO_DEFAULT_CLIENT_H */
