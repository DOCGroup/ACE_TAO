/* -*- C++ -*- */
/**
 *  @file Peer_T.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_PEER_T_H
#define TAO_Notify_Tests_PEER_T_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Peer.h"
#include "tao/PortableServer/Servant_Base.h"

/**
 * @class TAO_Notify_Tests_Peer_T
 *
 * @brief Base implementation for all Supplier and Consumer Classes.
 */
template <class Peer_Traits>
class TAO_Notify_Tests_Peer_T : public TAO_Notify_Tests_Peer, public Peer_Traits::SKELETON
{
  typedef typename Peer_Traits::Admin_Traits Admin_Traits;
  typedef typename Peer_Traits::Admin_Ext_Traits Admin_Ext_Traits;
  typedef typename Peer_Traits::Proxy_Traits Proxy_Traits;

  typedef typename Peer_Traits::PTR Peer_Traits_PTR;

  typedef typename Proxy_Traits::INTERFACE Proxy_Traits_INTERFACE;
  typedef typename Proxy_Traits::PTR Proxy_Traits_PTR;
  typedef typename Proxy_Traits::ID Proxy_Traits_ID;

  typedef typename Admin_Traits::PTR Admin_Traits_PTR;

  typedef typename Admin_Ext_Traits::INTERFACE Admin_Ext_Traits_INTERFACE;
  typedef typename Admin_Ext_Traits::PTR Admin_Ext_Traits_PTR;

public:
  /// Constructor
  TAO_Notify_Tests_Peer_T ();

  /// Destructor
  virtual ~TAO_Notify_Tests_Peer_T ();

  // Activates this servant with the POA supplied in init.
  // Connects to given proxy.
  void connect (Proxy_Traits_PTR proxy, Proxy_Traits_ID proxy_id);

  // Activates this servant with the POA supplied in init.
  // Creates a new proxy supplier and connects to it.
  void connect (Admin_Traits_PTR admin_ptr);

  /// Connect using options parsed and set initial QoS.
  virtual void connect ();

  // Disconnect from the Peer.
  void disconnect ();

  /// Set Qos
  void set_qos (CosNotification::QoSProperties& qos);

  /// Dump status
  void status ();

  // Accessor for <proxy_>.
  Proxy_Traits_PTR get_proxy ();

  /// Deactivate the object.
  void deactivate ();

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA ();

protected:
  /// Activate this object in the default POA.
  virtual Peer_Traits_PTR activate ();

  ///= To be implemented by specializations.

  /// Connect to Peer.
  virtual void connect_to_peer (Proxy_Traits_PTR proxy_ptr, Peer_Traits_PTR peer_ptr) = 0;

  /// Obtain Proxy.
  virtual Proxy_Traits_PTR obtain_proxy (Admin_Traits_PTR admin_ptr) = 0;

  /// Obtain Proxy with QoS.
  virtual Proxy_Traits_PTR obtain_proxy (Admin_Ext_Traits_PTR admin_ptr
                                         , CosNotification::QoSProperties& qos) = 0;

  /// Disconnect from proxy.
  virtual void disconnect_from_proxy () = 0;

  // = Data Members

  /// The proxy that we are connected to.
   typename Proxy_Traits::VAR proxy_;

  /// This <proxy_> id.
   typename Proxy_Traits::ID proxy_id_;
};

#include "Peer_T.cpp"

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_PEER_T_H */
