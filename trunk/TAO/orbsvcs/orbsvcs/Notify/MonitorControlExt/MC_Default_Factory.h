/**
 *  @file MC_Default_Factory.h
 *
 *  $Id$
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 *
 *
 */

#ifndef TAO_MC_DEFAULT_FACTORY_H
#define TAO_MC_DEFAULT_FACTORY_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Default_Factory.h"

// The using in the class below is apparently ignored by the
// Borland compiler.  Having the using below should take care
// of the warning about hiding virtual functions.
#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x590)
# pragma warning (push)
# pragma warning (disable : 8022)
#endif /* __BORLANDC__ &&  && __BORLANDC__ <= 0x590 */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_MC_Default_Factory
 *
 * @brief Implemetation of the TAO_Default_Factory interface for Monitoring
 *
 */
class TAO_Notify_MC_Ext_Export TAO_MC_Default_Factory:
                                 public TAO_Notify_Default_Factory
{
public:
#if !defined (__GNUC__) || (__GNUC__ >= 3)
  /// Instruct the compiler that we want the create
  /// methods from the base class as well
  using TAO_Notify_Default_Factory::create;
#endif /* !__GNUC__ || __GNUC__ >= 3 */

  /// Create the channel factory.
  virtual void create (TAO_Notify_EventChannelFactory*& factory,
                       const char* name);

  /// Create the event channel
  virtual void create (TAO_Notify_EventChannel*& ec,
                       const char* name);

  /// Create the supplier admin
  virtual void create (TAO_Notify_SupplierAdmin*& admin);

  /// Create the consumer admin
  virtual void create (TAO_Notify_ConsumerAdmin*& admin);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x590)
# pragma warning (pop)
#endif /* __BORLANDC__  && __BORLANDC__ <= 0x590 */

#include /**/ "ace/post.h"
#endif /* TAO_MC_DEFAULT_FACTORY_H */
