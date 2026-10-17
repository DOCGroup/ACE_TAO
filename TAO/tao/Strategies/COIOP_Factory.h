// -*- C++ -*-

//=============================================================================
/**
 *  @file   COIOP_Factory.h
 *
 *  @author  Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_COIOP_FACTORY_H
#define TAO_COIOP_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "tao/Strategies/strategies_export.h"
#include "tao/Protocol_Factory.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Acceptor;
class TAO_Connector;

class TAO_Strategies_Export TAO_COIOP_Protocol_Factory
  : public TAO_Protocol_Factory
{
public:
  /// Constructor.
  TAO_COIOP_Protocol_Factory ();

  /// Destructor.
  ~TAO_COIOP_Protocol_Factory () override = default;

  // = Service Configurator hooks.
  /// Dynamic linking hook
  int init (int argc, ACE_TCHAR* argv[]) override;

  /// Verify prefix is a match
  int match_prefix (ACE_CString const &prefix) override;

  /// Returns the prefix used by the protocol.
  char const *prefix () const override;

  /// Return the character used to mark where an endpoint ends and
  /// where its options begin.
  char options_delimiter () const override;

  /**
   * @name Protocol factory methods
   *
   * Check Protocol_Factory.h for a description of these methods.
   */
  //@{
  TAO_Acceptor  *make_acceptor () override;
  TAO_Connector *make_connector () override;
  int requires_explicit_endpoint () const override;
  //@}

private:
  TAO_COIOP_Protocol_Factory (TAO_COIOP_Protocol_Factory const &) = delete;
  TAO_COIOP_Protocol_Factory &operator= (TAO_COIOP_Protocol_Factory const &) = delete;
  TAO_COIOP_Protocol_Factory (TAO_COIOP_Protocol_Factory &&) = delete;
  TAO_COIOP_Protocol_Factory &operator= (TAO_COIOP_Protocol_Factory &&) = delete;
};


ACE_STATIC_SVC_DECLARE (TAO_COIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO_Strategies, TAO_COIOP_Protocol_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_COIOP_FACTORY_H */
