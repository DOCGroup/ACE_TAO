// -*- C++ -*-

//=============================================================================
/**
 *  @file     UIPMC_Factory.h
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_UIPMC_FACTORY_H
#define TAO_UIPMC_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/Protocol_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Acceptor;
class TAO_Connector;

class TAO_PortableGroup_Export TAO_UIPMC_Protocol_Factory : public TAO_Protocol_Factory
{
public:
  TAO_UIPMC_Protocol_Factory ();
  ~TAO_UIPMC_Protocol_Factory () override;

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

private:
  /// Changing the version number can be used to provide backwards
  /// compatibility with old clients.
  // int major_;
  // int minor_;

  /// Should we override the default behaviour (whatever it is) to listen on
  /// all interfaces
  bool listen_on_all_;

  /// Any specified and valid -ORBListenerInterfaces option string
  ACE_CString listener_interfaces_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_UIPMC_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO_PortableGroup, TAO_UIPMC_Protocol_Factory)

#include /**/ "ace/post.h"

#endif /* TAO_UIPMC_FACTORY_H */
