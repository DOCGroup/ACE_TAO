// -*- C++ -*-

//=============================================================================
/**
 *  @file   UIOP_Factory.h
 *
 *  $Id$
 *
 *  @author Fred Kuhns <fredk@cs.wustl.edu>Ossama Othman <othman@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_UIOP_FACTORY_H
#define TAO_UIOP_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "tao/Protocol_Factory.h"
#include "tao/Strategies/strategies_export.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Acceptor;
class TAO_Connector;

class TAO_Strategies_Export TAO_UIOP_Protocol_Factory : public TAO_Protocol_Factory
{
public:
  /// Constructor.
  TAO_UIOP_Protocol_Factory (void);

  /// Destructor.
  virtual ~TAO_UIOP_Protocol_Factory (void);

  // = Service Configurator hooks.
  /// Dynamic linking hook
  virtual int init (int argc, ACE_TCHAR* argv[]);

  /// Verify prefix is a match
  virtual int match_prefix (const ACE_CString &prefix);

  /// Returns the prefix used by the protocol.
  virtual const char *prefix (void) const;

  /// Return the character used to mark where an endpoint ends and
  /// where its options begin.
  virtual char options_delimiter (void) const;

  /**
   * @name Protocol factory methods
   *
   * Check Protocol_Factory.h for a description of these methods.
   */
  //@{
  virtual TAO_Acceptor  *make_acceptor (void);
  virtual TAO_Connector *make_connector  (void);
  virtual int requires_explicit_endpoint (void) const;
  //@}
};


ACE_STATIC_SVC_DECLARE (TAO_UIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO_Strategies, TAO_UIOP_Protocol_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

# endif  /* TAO_HAS_UIOP == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_UIOP_FACTORY_H */
