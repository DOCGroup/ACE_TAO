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

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

#include "ace/Service_Object.h"
#include "ace/SStringfwd.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Acceptor;
class TAO_Connector;

class TAO_Export TAO_Protocol_Factory : public ACE_Service_Object
{
public:
  TAO_Protocol_Factory (CORBA::ULong tag);
  virtual ~TAO_Protocol_Factory (void);

  /// Initialization hook.
  virtual int init (int argc, ACE_TCHAR *argv[]) = 0;

  /// The protocol tag, each concrete class will have a specific tag
  /// value.
  CORBA::ULong tag (void) const;

  /// Verify prefix is a match
  virtual int match_prefix (const ACE_CString &prefix) = 0;

  /// Returns the prefix used by the protocol.
  virtual const char *prefix (void) const = 0;

  /// Return the character used to mark where an endpoint ends and
  /// where its options begin.
  virtual char options_delimiter (void) const = 0;

  // Factory methods
  /// Create an acceptor
  virtual TAO_Acceptor *make_acceptor (void) = 0;

  /// Create a connector
  virtual TAO_Connector *make_connector  (void) = 0;

  /**
   * Some protocols should not create a default endpoint unless the
   * user specifies a -ORBEndpoint option. For example, local IPC
   * (aka UNIX domain sockets) is unable to remove the rendezvous
   * point if the server crashes.  For those protocols it is better to
   * create the endpoint only if the user requests one.
   */
  virtual int requires_explicit_endpoint (void) const = 0;

private:

  /// IOP protocol tag.
  CORBA::ULong const tag_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PROTOCOL_FACTORY_H */
