// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_SupplierAdmin.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef TAO_FTEC_SUPPLIERADMIN_H
#define TAO_FTEC_SUPPLIERADMIN_H
#include /**/ "ace/pre.h"

#include "FTEC_ProxyConsumer.h"
#include "FT_ProxyAdmin_T.h"
#include "ProxyConsumerStateWorker.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_SupplierAdmin.h"
//#include "event_export.h"

class TAO_FTEC_Event_Channel_Impl;

namespace FtRtecEventChannelAdmin {
  struct SupplierAdminState;
};
/**
 * @class TAO_FTEC_SupplierAdmin
 *
 * @brief Implement the RtecEventChannelAdmin::SupplierAdmin interface.
 *
 *
 */
class TAO_FTEC_SupplierAdmin
  : public TAO_EC_SupplierAdmin
  , public FT_ProxyAdmin<TAO_FTEC_SupplierAdmin,
                         TAO_FTEC_ProxyPushConsumer,
             RtecEventChannelAdmin::ProxyPushConsumer,
             FtRtecEventChannelAdmin::SupplierAdminState>
{
public:

  static const FtRtecEventChannelAdmin::OperationType OBTAIN_ID;

  typedef FT_ProxyAdmin<TAO_FTEC_SupplierAdmin,
                         TAO_FTEC_ProxyPushConsumer,
             RtecEventChannelAdmin::ProxyPushConsumer,
             FtRtecEventChannelAdmin::SupplierAdminState>
          FT_Aspect;

  typedef ProxyConsumerStateWorker  StateWorker;

  /// constructor...
  TAO_FTEC_SupplierAdmin (TAO_EC_Event_Channel_Base* event_channel);

  /// destructor...
  virtual ~TAO_FTEC_SupplierAdmin (void);


  // = The RtecEventChannelAdmin::SupplierAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushConsumer_ptr
      obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException));

  void disconnect(RtecEventChannelAdmin::ProxyPushConsumer_ptr obj);
};

#include /**/ "ace/post.h"
#endif /* TAO_FTEC_SUPPLIERADMIN_H */
