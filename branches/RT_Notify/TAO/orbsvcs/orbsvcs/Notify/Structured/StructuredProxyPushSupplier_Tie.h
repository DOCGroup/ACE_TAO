/* -*- C++ -*- */
/**
 *  @file StructuredProxyPushSupplier_Tie.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_STRUCTUREDPROXYPUSHSUPPLIER_TIE_H
#define TAO_NS_STRUCTUREDPROXYPUSHSUPPLIER_TIE_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "StructuredProxyPushSupplier.h"
#include "../Destroy_Callback.h"

/**
 * @class TAO_NS_StructuredProxyPushSupplier_Tie
 *
 * @brief Tie implementation that implements CosNotifyChannelAdmin::StructuredProxyPushSupplier.
 *
 */
class TAO_Notify_Export TAO_NS_StructuredProxyPushSupplier_Tie : public POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier_tie <TAO_NS_StructuredProxyPushSupplier>, public TAO_NS_Destroy_Callback
{
  typedef POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier_tie <TAO_NS_StructuredProxyPushSupplier> inherited;

public:
  /// Constuctor
  TAO_NS_StructuredProxyPushSupplier_Tie (void);

  /// Destructor
  ~TAO_NS_StructuredProxyPushSupplier_Tie ();

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

protected:
  /// The Tied object providing the required proxy functionality.
  TAO_NS_StructuredProxyPushSupplier tied_;  
};

#if defined (__ACE_INLINE__)
#include "StructuredProxyPushSupplier_Tie.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_STRUCTUREDPROXYPUSHSUPPLIER_TIE_H */
