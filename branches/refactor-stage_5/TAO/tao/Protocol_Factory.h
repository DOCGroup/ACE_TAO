// -*- C++ -*-

//=============================================================================
/**
 *  @file   Protocol_Factory.h
 *
 *  $Id$
 *
 *  @author Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_PROTOCOL_FACTORY_H
#define TAO_PROTOCOL_FACTORY_H
#include /**/ "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/SStringfwd.h"

class TAO_Acceptor;
class TAO_Connector;

class TAO_Export TAO_Protocol_Factory : public ACE_Service_Object
{
public:
  TAO_Protocol_Factory (CORBA::ULong tag);
  virtual ~TAO_Protocol_Factory (void);

  /// Initialization hook.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// The protocol tag, each concrete class will have a specific tag
  /// value.
  CORBA::ULong tag (void) const;

  /// Verify prefix is a match
  virtual int match_prefix (const ACE_CString &prefix);

  /// Returns the prefix used by the protocol.
  virtual const char *prefix (void) const;

  /// Return the character used to mark where an endpoint ends and
  /// where its options begin.
  virtual char options_delimiter (void) const;

  // Factory methods
  /// Create an acceptor
  virtual TAO_Acceptor  *make_acceptor (void);

  /// Create a connector
  virtual TAO_Connector *make_connector  (void);

  /**
   * Some protocols should not create a default endpoint unless the
   * user specifies a -ORBEndpoint option. For example, local IPC
   * (aka UNIX domain sockets) is unable to remove the rendesvouz
   * point if the server crashes.  For those protocols it is better to
   * create the endpoint only if the user requests one.
   */
  virtual int requires_explicit_endpoint (void) const = 0;

private:
  /// IOP protocol tag.
  CORBA::ULong tag_;
};

#include /**/ "ace/post.h"
#endif /* TAO_PROTOCOL_FACTORY_H */
