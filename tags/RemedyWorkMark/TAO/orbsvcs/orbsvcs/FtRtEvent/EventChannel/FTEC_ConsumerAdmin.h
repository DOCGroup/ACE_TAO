// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_ConsumerAdmin.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef TAO_FTEC_CONSUMERADMIN_H
#define TAO_FTEC_CONSUMERADMIN_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/FtRtEvent/EventChannel/FT_ProxyAdmin_T.h"
#include "orbsvcs/FtRtEvent/EventChannel/ProxySupplierStateWorker.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace FtRtecEventChannelAdmin {
  struct ConsumerAdminState;
}


class TAO_FTEC_Event_Channel_Impl;
class TAO_FTEC_ProxyPushSupplier;
/**
 * @class TAO_FTEC_ConsumerAdmin
 *
 * @brief Implements the ConsumerAdmin interface, i.e. the factory for
 * TAO_FTEC_ProxyPushSupplier objects.
 *
 */
class TAO_FTEC_ConsumerAdmin
  : public TAO_EC_ConsumerAdmin
  , public FT_ProxyAdmin<TAO_FTEC_ConsumerAdmin,
                         TAO_FTEC_ProxyPushSupplier,
             RtecEventChannelAdmin::ProxyPushSupplier,
             FtRtecEventChannelAdmin::ConsumerAdminState>
{
public:
  static const FtRtecEventChannelAdmin::OperationType OBTAIN_ID ;

  typedef FT_ProxyAdmin<TAO_FTEC_ConsumerAdmin,
                         TAO_FTEC_ProxyPushSupplier,
             RtecEventChannelAdmin::ProxyPushSupplier,
             FtRtecEventChannelAdmin::ConsumerAdminState>
          FT_Aspect;

  typedef ProxySupplierStateWorker  StateWorker;
  TAO_FTEC_ConsumerAdmin (TAO_EC_Event_Channel_Base* event_channel);

  /// destructor...
  virtual ~TAO_FTEC_ConsumerAdmin (void);

  // = The RtecEventChannelAdmin::ConsumerAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr
      obtain_push_supplier (void);

  void disconnect(RtecEventChannelAdmin::ProxyPushSupplier_ptr obj);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_EC_CONSUMERADMIN_H */
