// -*- C++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Protocol_Factory.h
//
// = AUTHOR
//   Fred Kuhns <fredk@cs.wustl.edu>
//   Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_UIOP_FACTORY_H
#define TAO_UIOP_FACTORY_H

# if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "tao/Protocol_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Acceptor;
class TAO_Connector;


class TAO_Export TAO_UIOP_Protocol_Factory : public TAO_Protocol_Factory
{
public:
  TAO_UIOP_Protocol_Factory (void);
  virtual ~TAO_UIOP_Protocol_Factory (void);

  // = Service Configurator hooks.
  virtual int init (int argc, char* argv[]);
  // Dynamic linking hook

  virtual int match_prefix (const ACE_CString &prefix);
  // verify prefix is a match

  // Factory methods 
  virtual TAO_Acceptor  *make_acceptor (void);
  // create an acceptor

  virtual TAO_Connector *make_connector  (void);
  // create a connector
};

ACE_STATIC_SVC_DECLARE (TAO_UIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_UIOP_Protocol_Factory)

# endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#endif /* TAO_UIOP_FACTORY_H */
