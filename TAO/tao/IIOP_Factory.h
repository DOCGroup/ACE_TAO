// -*- C++ -*-

//=============================================================================
/**
 *  @file   IIOP_Factory.h
 *
 *  @author Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_FACTORY_H
#define TAO_IIOP_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/Protocol_Factory.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Acceptor;
class TAO_Connector;

class TAO_Export TAO_IIOP_Protocol_Factory : public TAO_Protocol_Factory
{
public:
  TAO_IIOP_Protocol_Factory ();
  virtual ~TAO_IIOP_Protocol_Factory ();

  // = Service Configurator hooks.
  /// Dynamic linking hook
  int init (int argc, ACE_TCHAR* argv[]) override;

  /// Verify prefix is a match
  int match_prefix (const ACE_CString &prefix) override;

  /// Returns the prefix used by the protocol.
  const char *prefix () const override;

  /// Return the character used to mark where an endpoint ends and
  /// where its options begin.
  char options_delimiter () const override;

  // = Check Protocol_Factory.h for a description of these methods.
  TAO_Acceptor  *make_acceptor () override;
  TAO_Connector *make_connector  () override;
  int requires_explicit_endpoint () const override;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_IIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_IIOP_Protocol_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_IIOP_FACTORY_H */
