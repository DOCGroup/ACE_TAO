/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Module_Factory.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 *
 * The has strategies to handle several phases of the Event
 * processing, for instance dispatching can be implemented using
 * queues and different threads or just FIFO.
 * The choice of the strategies is controlled by a Factory class
 * that builds the right modules for the EC.
 *
 * @todo
 * Some of the strategies are still hard-coded.
 *
 */
//=============================================================================


#ifndef TAO_MODULE_FACTORY_H
#define TAO_MODULE_FACTORY_H
#include "ace/pre.h"

#include "old_event_export.h"

class ACE_EventChannel;

class ACE_ES_Dispatching_Base;
class ACE_ES_Consumer_Module;
class ACE_ES_Correlation_Module;
class ACE_ES_Subscription_Module;
class ACE_ES_Supplier_Module;
class ACE_ES_Priority_Timer;
class TAO_EC_Timer_Module;

/**
 * @class TAO_Module_Factory
 *
 * @brief Factory class for the Event Channel "modules".
 *
 * The Event Channel "modules" represent the different phases and
 * strategies in event processing.  Event Channel configuration is
 * achieved by constructing modules with different dynamic
 * types.  This class encapsulates all the strategies creation in
 * a single point.
 */
class TAO_RTOLDEvent_Export TAO_Module_Factory
{
public:
  virtual ~TAO_Module_Factory (void);

  virtual ACE_ES_Dispatching_Base*
    create_dispatching_module (ACE_EventChannel*) = 0;
  virtual void destroy_dispatching_module (ACE_ES_Dispatching_Base*) = 0;

  virtual ACE_ES_Consumer_Module*
    create_consumer_module (ACE_EventChannel*) = 0;
  virtual void destroy_consumer_module (ACE_ES_Consumer_Module*) = 0;

  virtual ACE_ES_Correlation_Module*
    create_correlation_module (ACE_EventChannel*) = 0;
  virtual void destroy_correlation_module (ACE_ES_Correlation_Module*) = 0;

  virtual ACE_ES_Subscription_Module*
    create_subscription_module (ACE_EventChannel*) = 0;
  virtual void destroy_subscription_module (ACE_ES_Subscription_Module*) = 0;

  virtual ACE_ES_Supplier_Module*
    create_supplier_module (ACE_EventChannel*) = 0;
  virtual void destroy_supplier_module (ACE_ES_Supplier_Module*) = 0;

  virtual TAO_EC_Timer_Module*
    create_timer_module (ACE_EventChannel*) = 0;
  virtual void destroy_timer_module (TAO_EC_Timer_Module*) = 0;

};

// ****************************************************************

/**
 * @class TAO_Default_Module_Factory
 *
 * @brief Default factory class for the Event Channel.
 *
 * The Event Channel "modules" represent the different phases and
 * strategies in event processing.  Event Channel configuration is
 * achieved by constructing modules with different dynamic
 * types.  This class encapsulates all the strategies creation in
 * a single point.
 */
class TAO_RTOLDEvent_Export TAO_Default_Module_Factory : public TAO_Module_Factory
{
public:
  TAO_Default_Module_Factory (void);
  virtual ~TAO_Default_Module_Factory (void);

  virtual ACE_ES_Dispatching_Base*
    create_dispatching_module (ACE_EventChannel*);
  virtual void destroy_dispatching_module (ACE_ES_Dispatching_Base*);

  virtual ACE_ES_Consumer_Module*
    create_consumer_module (ACE_EventChannel*);
  virtual void destroy_consumer_module (ACE_ES_Consumer_Module*);

  virtual ACE_ES_Correlation_Module*
    create_correlation_module (ACE_EventChannel*);
  virtual void destroy_correlation_module (ACE_ES_Correlation_Module*);

  virtual ACE_ES_Subscription_Module*
    create_subscription_module (ACE_EventChannel*);
  virtual void destroy_subscription_module (ACE_ES_Subscription_Module*);

  virtual ACE_ES_Supplier_Module*
    create_supplier_module (ACE_EventChannel*);
  virtual void destroy_supplier_module (ACE_ES_Supplier_Module*);

  virtual TAO_EC_Timer_Module*
    create_timer_module (ACE_EventChannel*);
  virtual void destroy_timer_module (TAO_EC_Timer_Module*);

};

// ****************************************************************

/**
 * @class TAO_Reactive_Module_Factory
 *
 * @brief Factory class for the Event Channel "modules".
 *
 * This class factors out modules that require no extra threads
 * for event or timer dispatching.
 * An Event Channel configured with this factory can exhibit high
 * priority inversion, but it should provide high-performance due
 * to the elimination of context switching.
 */
class TAO_RTOLDEvent_Export TAO_Reactive_Module_Factory : public TAO_Module_Factory
{
public:
  TAO_Reactive_Module_Factory (void);
  virtual ~TAO_Reactive_Module_Factory (void);

  virtual ACE_ES_Dispatching_Base*
    create_dispatching_module (ACE_EventChannel*);
  virtual void destroy_dispatching_module (ACE_ES_Dispatching_Base*);

  virtual ACE_ES_Consumer_Module*
    create_consumer_module (ACE_EventChannel*);
  virtual void destroy_consumer_module (ACE_ES_Consumer_Module*);

  virtual ACE_ES_Correlation_Module*
    create_correlation_module (ACE_EventChannel*);
  virtual void destroy_correlation_module (ACE_ES_Correlation_Module*);

  virtual ACE_ES_Subscription_Module*
    create_subscription_module (ACE_EventChannel*);
  virtual void destroy_subscription_module (ACE_ES_Subscription_Module*);

  virtual ACE_ES_Supplier_Module*
    create_supplier_module (ACE_EventChannel*);
  virtual void destroy_supplier_module (ACE_ES_Supplier_Module*);

  virtual TAO_EC_Timer_Module*
    create_timer_module (ACE_EventChannel*);
  virtual void destroy_timer_module (TAO_EC_Timer_Module*);

};

#include "ace/post.h"
#endif /* ACE_EVENT_CHANNEL_H */
