// -*- C++ -*-

/**
 *  @file RT_Factory.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_RT_FACTORY_H
#define TAO_Notify_RT_FACTORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Default_Factory.h"

#if defined (__BORLANDC__) && (__BORLANDC__ >= 0x660) && (__BORLANDC__ <= 0x730)
#  pragma option push -w-8022
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_RT_Factory
 *
 * @brief The Factory for the RT aware Notify classes.
 */
class TAO_RT_Notify_Export TAO_Notify_RT_Factory : public TAO_Notify_Default_Factory
{
public:
  /// Constructor
  TAO_Notify_RT_Factory (void);

  /// Destructor
  virtual ~TAO_Notify_RT_Factory ();

#if !defined (__GNUC__) || (__GNUC__ >= 3)
  /// Instruct the compiler that we want the create
  /// methods from the base class as well
  using TAO_Notify_Default_Factory::create;
#endif /* !__GNUC__ || __GNUC__ >= 3 */

  /// Create StructuredProxyPushSupplier
  virtual void create (TAO_Notify_StructuredProxyPushSupplier*& proxy);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__BORLANDC__) && (__BORLANDC__ >= 0x660) && (__BORLANDC__ <= 0x730)
# pragma option pop
#endif

ACE_FACTORY_DECLARE (TAO_RT_Notify, TAO_Notify_RT_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_Notify_RT_FACTORY_H */
