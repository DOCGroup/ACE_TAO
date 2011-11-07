/* -*- C++ -*- */
/**
 *  @file CosEC_ProxyPushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_COSEC_PROXYPUSHCONSUMER_H
#define TAO_Notify_COSEC_PROXYPUSHCONSUMER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEventChannelAdminS.h"

#include "orbsvcs/Notify/ProxyConsumer_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_CosEC_ProxyPushConsumer
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_CosEC_ProxyPushConsumer
  : public virtual TAO_Notify_ProxyConsumer_T <POA_CosEventChannelAdmin::ProxyPushConsumer>
{
 friend class TAO_Notify_Builder;
public:
  /// Constructor
  TAO_Notify_CosEC_ProxyPushConsumer (void);

  /// Destructor
  virtual ~TAO_Notify_CosEC_ProxyPushConsumer ();


  virtual const char * get_proxy_type_name (void) const;
  virtual void validate ();
  virtual void configure(TAO_Notify_SupplierAdmin & admin, CosNotifyChannelAdmin::ProxyID_out proxy_id);

protected:
  ///= CosNotifyChannelAdmin::ProxyPushConsumer methods

  virtual void push (const CORBA::Any & data);

  virtual void connect_push_supplier (CosEventComm::PushSupplier_ptr push_supplier);

  virtual void disconnect_push_consumer (void);
private:
  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_COSEC_PROXYPUSHCONSUMER_H */
