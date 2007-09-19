// -*- C++ -*-
/**
 *  @file   EC_TPC_Factory.h
 *
 *  $Id$
 *
 */

#ifndef TAO_EC_TPC_FACTORY_H
#define TAO_EC_TPC_FACTORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Default_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_EC_TPC_Factory
 *
 * @brief A generic factory for EC experimentation.
 *
 * This class allows the user to experiment with different EC
 * configurations.  Using a command-line like interface the user
 * can specify which strategies will this factory generate.
 * Since the class can be dynamically loaded the strategies can be
 * set in the service configurator file.
 */
class TAO_RTEvent_Serv_Export TAO_EC_TPC_Factory : public TAO_EC_Default_Factory
{
public:
  /// Constructor
  TAO_EC_TPC_Factory (void);

  /// destructor...
  virtual ~TAO_EC_TPC_Factory (void);

  /// Helper function to register the default factory into the service
  /// configurator.
  static int init_svcs (void);

  // = The Service_Object entry points
  virtual int init (int argc, char* argv[]);

  // = The EC_Factory methods
  virtual TAO_EC_Dispatching*
      create_dispatching (TAO_EC_Event_Channel_Base*);

  virtual TAO_EC_ProxyPushSupplier*
      create_proxy_push_supplier (TAO_EC_Event_Channel_Base*);

  virtual TAO_EC_ProxyPushConsumer*
      create_proxy_push_consumer (TAO_EC_Event_Channel_Base*);
};

extern unsigned long TAO_EC_TPC_debug_level;

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_EC_TPC_Factory)
ACE_FACTORY_DECLARE (TAO_RTEvent_Serv, TAO_EC_TPC_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_EC_TPC_FACTORY_H */
