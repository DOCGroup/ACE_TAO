/* -*- C++ -*- */
/**
 *  @file CosEC_ProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_COSEC_PROXYPUSHSUPPLIER_H
#define TAO_Notify_COSEC_PROXYPUSHSUPPLIER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEventChannelAdminS.h"

#include "orbsvcs/Notify/ProxySupplier_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Notify_Serv_Export
TAO_Notify_ProxySupplier_T<POA_CosEventChannelAdmin::ProxyPushSupplier>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class TAO_Notify_CosEC_ProxyPushSupplier
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_CosEC_ProxyPushSupplier
  : public virtual TAO_Notify_ProxySupplier_T <POA_CosEventChannelAdmin::ProxyPushSupplier>
{
  friend class TAO_Notify_Builder;
public:
  /// Constructor
  TAO_Notify_CosEC_ProxyPushSupplier (void);

  /// Destructor
  virtual ~TAO_Notify_CosEC_ProxyPushSupplier ();

  virtual const char * get_proxy_type_name (void) const;

  // = Interface methods
  virtual void connect_push_consumer (
    CosEventComm::PushConsumer_ptr push_consumer
  );

  virtual void disconnect_push_supplier (
  );

  virtual void validate ();
  virtual void configure(TAO_Notify_ConsumerAdmin & admin, CosNotifyChannelAdmin::ProxyID_out proxy_id);

private:
  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_COSEC_PROXYPUSHSUPPLIER_H */
