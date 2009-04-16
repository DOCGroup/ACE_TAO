/* -*- C++ -*- */
/**
 *  @file SequenceProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_SEQUENCEPROXYPUSHSUPPLIER_H
#define TAO_Notify_SEQUENCEPROXYPUSHSUPPLIER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"

#include "orbsvcs/Notify/ProxySupplier_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Notify_Serv_Export
TAO_Notify_ProxySupplier_T<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class TAO_Notify_SequenceProxyPushSupplier
 *
 * @brief Implements the CosNotifyChannelAdmin::SequenceProxyPushSupplier methods.
 *
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_SequenceProxyPushSupplier
  : public virtual TAO_Notify_ProxySupplier_T <POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>
{
  typedef TAO_Notify_ProxySupplier_T <POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier> SuperClass;
  friend class TAO_Notify_Builder;

public:
  /// Constructor
  TAO_Notify_SequenceProxyPushSupplier (void);

  /// Destructor
  virtual ~TAO_Notify_SequenceProxyPushSupplier ();


  virtual const char * get_proxy_type_name (void) const;

  virtual void load_attrs (const TAO_Notify::NVPList& attrs);
  virtual void validate ();

  /// = Servant methods
  virtual CosNotifyChannelAdmin::ProxyType MyType (void);

  virtual void connect_sequence_push_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer
  );

  virtual void disconnect_sequence_push_supplier (
  );

  /// TAO_Notify_Destroy_Callback methods
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_SEQUENCEPROXYPUSHSUPPLIER_H */
