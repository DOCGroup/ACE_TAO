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

#ifndef TAO_PROTOCOL_FACTORY_H
#define TAO_PROTOCOL_FACTORY_H
#include "ace/pre.h"

#include "ace/Service_Object.h"
#include "ace/Strategies_T.h"
#include "ace/SString.h"
#include "tao/Pluggable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_Export TAO_Protocol_Factory : public ACE_Service_Object
{
public:
  TAO_Protocol_Factory (void);
  virtual ~TAO_Protocol_Factory (void);

  virtual int init (int argc, char *argv[]);
  // Initialization hook.

  virtual int match_prefix (const ACE_CString &prefix);
  // Verify prefix is a match

  virtual const char *prefix (void) const;
  // Returns the prefix used by the protocol.

  virtual char options_delimiter (void) const;
  // Return the character used to mark where an endpoint ends and
  // where its options begin.

  // Factory methods
  virtual TAO_Acceptor  *make_acceptor (void);
  // Create an acceptor

  virtual TAO_Connector *make_connector  (void);
  // Create a connector

  virtual int requires_explicit_endpoint (void) const = 0;
  // Some protocols should not create a default endpoint unless the
  // user specifies a -ORBEndpoint option. For example, local IPC
  // (aka UNIX domain sockets) is unable to remove the rendesvouz
  // point if the server crashes.  For those protocols is better to
  // create the endpoint only if the user requests one.
};

#include "ace/post.h"
#endif /* TAO_PROTOCOL_FACTORY_H */
