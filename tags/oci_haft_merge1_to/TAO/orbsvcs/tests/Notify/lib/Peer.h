/* -*- C++ -*- */
/**
 *  @file Peer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_PEER_H
#define TAO_Notify_Tests_PEER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminC.h"
#include "tao/PortableServer/PortableServer.h"

class ACE_Arg_Shifter;

/**
 * @class TAO_Notify_Tests_Peer
 *
 * @brief
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Peer
{
public:
  /// Constuctor
  TAO_Notify_Tests_Peer (void);

  /// Destructor
  virtual ~TAO_Notify_Tests_Peer ();

  /// Init
  void init (PortableServer::POA_ptr poa  ACE_ENV_ARG_DECL_NOT_USED);

  /// Init this object.
  virtual int init_state (ACE_Arg_Shifter& arg_shifter);

  /// Set POA
  void set_poa (PortableServer::POA_ptr poa  ACE_ENV_ARG_DECL);

  // Accessor to set/get our name.
  void set_name (ACE_CString& name);
  const ACE_CString& get_name (void);

protected:

  /// My name.
  ACE_CString name_;

  /// Proxy Name.
  ACE_CString proxy_name_;

  ACE_CString admin_name_;

  ACE_CString poa_name_;

  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;

  CosNotification::QoSProperties qos_;

  /// The default POA.
  PortableServer::POA_var default_POA_;
};

#if defined (__ACE_INLINE__)
#include "Peer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_Tests_PEER_H */
