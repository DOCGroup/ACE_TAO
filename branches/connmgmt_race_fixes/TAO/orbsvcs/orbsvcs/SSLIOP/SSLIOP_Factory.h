// -*- C++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//     TAO_SSLIOP
//
// = FILENAME
//     SSLIOP_Factory.h
//
// = AUTHOR
//     Carlos O'Ryan <coryan@ece.uci.edu>
//     Ossama Othman <ossama@ece.uci.edu>
//
// ============================================================================

#ifndef TAO_SSLIOP_FACTORY_H
#define TAO_SSLIOP_FACTORY_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Protocol_Factory.h"

#include "ace/Service_Config.h"

class TAO_Acceptor;
class TAO_Connector;

class TAO_SSLIOP_Export TAO_SSLIOP_Protocol_Factory : public TAO_Protocol_Factory
{
public:
  TAO_SSLIOP_Protocol_Factory (void);
  virtual ~TAO_SSLIOP_Protocol_Factory (void);

  // = Service Configurator hooks.
  virtual int init (int argc, char* argv[]);
  // Dynamic linking hook

  /// Create and register the SSLIOP ORB initializer.
  int register_orb_initializer (void);

  virtual int match_prefix (const ACE_CString &prefix);
  // Verify prefix is a match

  virtual const char *prefix (void) const;
  // Returns the prefix used by the protocol.

  virtual char options_delimiter (void) const;
  // Return the character used to mark where an endpoint ends and
  // where its options begin.

  // = Check Protocol_Factory.h for a description of these methods.
  virtual TAO_Acceptor  *make_acceptor (void);
  virtual TAO_Connector *make_connector  (void);
  virtual int requires_explicit_endpoint (void) const;

private:
  int major_;
  int minor_;
  // Changing the version number can be used to provide backwards
  // compatibility with old clients.

  int no_protection_;
  // In the case of the server side, allow connections to the insecure
  // IIOP port.
  // In the case of the client side, connect to the insecure IIOP port
  // instead of the secure IIOP over SSL port.
};

ACE_STATIC_SVC_DECLARE (TAO_SSLIOP_Protocol_Factory)
ACE_STATIC_SVC_REQUIRE (TAO_SSLIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO_SSLIOP, TAO_SSLIOP_Protocol_Factory)

#include "ace/post.h"

#endif /* TAO_SSLIOP_FACTORY_H */
