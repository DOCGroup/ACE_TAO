/* -*- C++ -*- */
/**
 *  @file StructuredProxyPushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_STRUCTUREDPROXYPUSHCONSUMER_H
#define TAO_Notify_STRUCTUREDPROXYPUSHCONSUMER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"

#include "orbsvcs/Notify/ProxyConsumer_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_StructuredProxyPushConsumer
 *
 * @brief CosNotifyChannelAdmin::StructuredProxyPushConsumer implementation.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_StructuredProxyPushConsumer
  : public virtual TAO_Notify_ProxyConsumer_T <POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>
{
  typedef TAO_Notify_ProxyConsumer_T <POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer> SuperClass;
  friend class TAO_Notify_Builder;

public:
  /// Constuctor
  TAO_Notify_StructuredProxyPushConsumer (void);

  /// Destructor
  virtual ~TAO_Notify_StructuredProxyPushConsumer ();


  virtual void load_attrs (const TAO_Notify::NVPList& attrs);

protected:

  // = interface methods
  virtual CosNotifyChannelAdmin::ProxyType MyType (void);

  virtual void connect_structured_push_supplier (
    CosNotifyComm::StructuredPushSupplier_ptr push_supplier
  );

  virtual void push_structured_event (
      const CosNotification::StructuredEvent & notification
    );

  virtual void disconnect_structured_push_consumer (
  );

  virtual const char * get_proxy_type_name (void) const;

private:

  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_STRUCTUREDPROXYPUSHCONSUMER_H */
