// -*- C++ -*-

//=============================================================================
/**
 *  @file   DIOP_Factory.h
 *
 *  $Id$
 *
 *  @author  Michael Kircher
 */
//=============================================================================


#ifndef TAO_DIOP_FACTORY_H
#define TAO_DIOP_FACTORY_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Protocol_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "strategies_export.h"

class TAO_Acceptor;
class TAO_Connector;

class TAO_Strategies_Export TAO_DIOP_Protocol_Factory : public TAO_Protocol_Factory
{
public:
  TAO_DIOP_Protocol_Factory (void);
  virtual ~TAO_DIOP_Protocol_Factory (void);

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

  // = Check Protocol_Factory.h for a description of these methods.
  virtual TAO_Acceptor  *make_acceptor (void);
  virtual TAO_Connector *make_connector  (void);
  virtual int requires_explicit_endpoint (void) const;

private:
  /// Changing the version number can be used to provide backwards
  /// compatibility with old clients.
  int major_;
  int minor_;
};

ACE_STATIC_SVC_DECLARE (TAO_DIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO_Strategies, TAO_DIOP_Protocol_Factory)

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#include "ace/post.h"
#endif /* TAO_DIOP_FACTORY_H */
