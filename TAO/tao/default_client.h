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
#if 0
#  include "ace/SOCK_Acceptor.h"
#  include "ace/SOCK_Connector.h"
#  include "ace/Strategies_T.h"
#  include "ace/Connector.h"
#  include "ace/Synch.h"

#  include "tao/params.h"
#  include "tao/connect.h"
#  include "tao/objtable.h"
#endif /* 0 */

#include "tao/corba.h"

class TAO_Default_Client_Strategy_Factory : public TAO_Client_Strategy_Factory
  // = TITLE
  //   This is the "default" client strategy factor for
  //   TAO...basically, it includes strategies that are configured
  //   through command-line options so that everything operates as if
  //   there were no dynamically-linkable strategies.
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
