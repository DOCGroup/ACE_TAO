/* -*- C++ -*- */
/**
 *  @file Peer_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PEER_T_H
#define TAO_NS_PEER_T_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Peer.h"
#include "tao/PortableServer/Servant_Base.h"

/**
 * @class TAO_NS_Peer_T
 *
 * @brief
 *
 */
template <class Peer_Traits>
class TAO_NOTIFY_TEST_Export TAO_NS_Peer_T : public TAO_NS_Peer, public Peer_Traits::SKELETON, public PortableServer::RefCountServantBase
{
  typedef ACE_TYPENAME Peer_Traits::Admin_Traits Admin_Traits;
  typedef ACE_TYPENAME Peer_Traits::Admin_Ext_Traits Admin_Ext_Traits;
  typedef ACE_TYPENAME Peer_Traits::Proxy_Traits Proxy_Traits;

  typedef ACE_TYPENAME Peer_Traits::PTR Peer_Traits_PTR;

  typedef ACE_TYPENAME Proxy_Traits::INTERFACE Proxy_Traits_INTERFACE;
  typedef ACE_TYPENAME Proxy_Traits::PTR Proxy_Traits_PTR;
  typedef ACE_TYPENAME Proxy_Traits::ID Proxy_Traits_ID;

  typedef ACE_TYPENAME Admin_Traits::PTR Admin_Traits_PTR;

  typedef ACE_TYPENAME Admin_Ext_Traits::INTERFACE Admin_Ext_Traits_INTERFACE;
  typedef ACE_TYPENAME Admin_Ext_Traits::PTR Admin_Ext_Traits_PTR;

public:
  /// Constuctor
  TAO_NS_Peer_T (void);

  /// Destructor
  virtual ~TAO_NS_Peer_T ();

  // Activates this servant with the POA supplied in init.
  // Connects to given proxy.
  void connect (Proxy_Traits_PTR proxy, Proxy_Traits_ID proxy_id ACE_ENV_ARG_DECL);

  // Activates this servant with the POA supplied in init.
  // Creates a new proxy supplier and connects to it.
  void connect (Admin_Traits_PTR admin_ptr ACE_ENV_ARG_DECL);

  /// Connect using options parsed and set initial QoS.
  virtual void connect (ACE_ENV_SINGLE_ARG_DECL);

  // Disconnect from the Peer.
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  /// Set Qos
  void set_qos (CosNotification::QoSProperties& qos ACE_ENV_ARG_DECL);

  /// Dump status
  void status (ACE_ENV_SINGLE_ARG_DECL);

  // Accessor for <proxy_>.
  Proxy_Traits_PTR get_proxy (void);

  /// Deactivate the object.
  void deactivate (ACE_ENV_SINGLE_ARG_DECL);

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

protected:
  /// Activate this object in the default POA.
  virtual Peer_Traits_PTR activate (ACE_ENV_SINGLE_ARG_DECL);

  ///= To be implemented by specializations.

  /// Connect to Peer.
  virtual void connect_to_peer (Proxy_Traits_PTR proxy_ptr, Peer_Traits_PTR peer_ptr ACE_ENV_ARG_DECL) = 0;

  /// Obtain Proxy.
  virtual Proxy_Traits_PTR obtain_proxy (Admin_Traits_PTR admin_ptr ACE_ENV_ARG_DECL) = 0;

  /// Obtain Proxy with QoS.
  virtual Proxy_Traits_PTR obtain_proxy (Admin_Ext_Traits_PTR admin_ptr
                                         , CosNotification::QoSProperties& qos
                                         ACE_ENV_ARG_DECL) = 0;

  /// Disconnect from proxy.
  virtual void disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL) = 0;

  // = Data Members

  /// The proxy that we are connected to.
   ACE_TYPENAME Proxy_Traits::VAR proxy_;

  /// This <proxy_> id.
   ACE_TYPENAME Proxy_Traits::ID proxy_id_;
};

#if defined (__ACE_INLINE__)
#include "Peer_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Peer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Peer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_PEER_T_H */
