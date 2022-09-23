// -*- C++ -*-

//=============================================================================
/**
 *  @file   DIOP_Factory.h
 *
 *  @author  Michael Kircher
 */
//=============================================================================


#ifndef TAO_DIOP_FACTORY_H
#define TAO_DIOP_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Strategies/strategies_export.h"
#include "tao/Protocol_Factory.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Acceptor;
class TAO_Connector;

class TAO_Strategies_Export TAO_DIOP_Protocol_Factory
  : public TAO_Protocol_Factory
{
public:
  /// Constructor.
  TAO_DIOP_Protocol_Factory ();

  /// Destructor.
  ~TAO_DIOP_Protocol_Factory (void) override = default;

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

  /**
   * @name Protocol factory methods
   *
   * Check Protocol_Factory.h for a description of these methods.
   */
  //@{
  TAO_Acceptor  *make_acceptor (void) override;
  TAO_Connector *make_connector  (void) override;
  int requires_explicit_endpoint () const override;
  //@}
};


ACE_STATIC_SVC_DECLARE (TAO_DIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO_Strategies, TAO_DIOP_Protocol_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_DIOP_FACTORY_H */
