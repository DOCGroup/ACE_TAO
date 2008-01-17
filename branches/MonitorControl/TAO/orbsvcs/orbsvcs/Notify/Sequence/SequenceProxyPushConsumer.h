/* -*- C++ -*- */
/**
 *  @file SequenceProxyPushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_SEQUENCEPROXYPUSHCONSUMER_H
#define TAO_Notify_SEQUENCEPROXYPUSHCONSUMER_H

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
 * @class TAO_Notify_SequenceProxyPushConsumer
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_SequenceProxyPushConsumer
  : public virtual TAO_Notify_ProxyConsumer_T <POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>
{
  typedef TAO_Notify_ProxyConsumer_T <POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer> SuperClass;
  friend class TAO_Notify_Builder;
public:
  /// Constuctor
  TAO_Notify_SequenceProxyPushConsumer (void);

  /// Destructor
  virtual ~TAO_Notify_SequenceProxyPushConsumer ();

  virtual const char * get_proxy_type_name (void) const;

  virtual void load_attrs (const TAO_Notify::NVPList& attrs);

protected:
  ///= Data Members
  TAO_Notify_Property_Time pacing_interval_;

  ///= Protected Methods

  //= interface methods
  virtual CosNotifyChannelAdmin::ProxyType MyType (void);

  virtual void connect_sequence_push_supplier (CosNotifyComm::SequencePushSupplier_ptr push_supplier);

  virtual void push_structured_events (const CosNotification::EventBatch & notifications);

  virtual void disconnect_sequence_push_consumer (void);

private:
  /// TAO_Notify_Destroy_Callback methods
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_SEQUENCEPROXYPUSHCONSUMER_H */
