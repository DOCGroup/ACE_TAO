/* -*- C++ -*- */
/**
 *  @file RT_Factory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RT_FACTORY_H
#define TAO_NS_RT_FACTORY_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Default_Factory.h"

/**
 * @class TAO_NS_RT_Factory
 *
 * @brief The Factory for the RT aware Notify classes.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RT_Factory : public TAO_NS_Default_Factory
{
public:
  /// Constuctor
  TAO_NS_RT_Factory (void);

  /// Destructor
  virtual ~TAO_NS_RT_Factory ();

  /// Create StructuredProxyPushSupplier
  virtual void create (TAO_NS_StructuredProxyPushSupplier*& proxy ACE_ENV_ARG_DECL);
};

ACE_FACTORY_DECLARE (TAO_Notify, TAO_NS_RT_Factory)

#if defined (__ACE_INLINE__)
#include "RT_Factory.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RT_FACTORY_H */
