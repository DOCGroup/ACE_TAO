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
//
// ============================================================================

#ifndef TAO_IIOP_FACTORY_H
#define TAO_IIOP_FACTORY_H

#include "tao/Protocol_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Acceptor;
class TAO_Connector;

class TAO_Export TAO_IIOP_Protocol_Factory : public TAO_Protocol_Factory
{
public:
  TAO_IIOP_Protocol_Factory (void);
  virtual ~TAO_IIOP_Protocol_Factory (void);

  // = Service Configurator hooks.
  virtual int init (int argc, char* argv[]);
  // Dynamic linking hook

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
};

ACE_STATIC_SVC_DECLARE (TAO_IIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_IIOP_Protocol_Factory)

#endif /* TAO_IIOP_FACTORY_H */
