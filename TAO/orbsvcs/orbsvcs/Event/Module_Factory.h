/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   Module_Factory
//
// = AUTHOR
//   Carlos O'Ryan
//
// = DESCRIPTION
//   The has strategies to handle several phases of the Event
//   processing, for instance dispatching can be implemented using
//   queues and different threads or just FIFO.
//   The choice of the strategies is controlled by a Factory class
//   that builds the right modules for the EC.
//
// = TODO
//   Some of the strategies are still hard-coded.
//
// ============================================================================

#ifndef TAO_MODULE_FACTORY_H
#define TAO_MODULE_FACTORY_H

#include "orbsvcs/orbsvcs_export.h"

class ACE_EventChannel;

class ACE_ES_Dispatching_Base;
class ACE_ES_Consumer_Module;
class ACE_Task_Manager;
class ACE_ES_Correlation_Module;
class ACE_ES_Subscription_Module;
class ACE_ES_Supplier_Modulel;
class ACE_ES_Priority_Timer;

class TAO_ORBSVCS_Export TAO_Module_Factory
{
  // = TITLE
  //   Factory class for the Event Channel "modules".
  //
  // = DESCRIPTION
  //   The Event Channel "modules" represent the different phases and
  //   strategies in event processing.  Event Channel configuration is
  //   achieved by constructing modules with different dynamic
  //   types.  This class encapsulates all the strategies creation in
  //   a single point.
  // 
public:
  virtual ~TAO_Module_Factory (void);

  virtual ACE_ES_Dispatching_Base*
    create_dispatching_module (ACE_EventChannel*) = 0;
  virtual void destroy_dispatching_module (ACE_ES_Dispatching_Base*) = 0;

  virtual ACE_ES_Consumer_Module*
    create_consumer_module (ACE_EventChannel*) = 0;
  virtual void destroy_consumer_module (ACE_ES_Consumer_Module*) = 0;

  virtual ACE_Task_Manager*
    create_task_manager (ACE_EventChannel*) = 0;
  virtual void destroy_task_manager (ACE_Task_Manager*) = 0;

  virtual ACE_ES_Correlation_Module*
    create_correlation_module (ACE_EventChannel*) = 0;
  virtual void destroy_correlation_module (ACE_ES_Correlation_Module*) = 0;

  virtual ACE_ES_Subscription_Module*
    create_subscription_module (ACE_EventChannel*) = 0;
  virtual void destroy_subscription_module (ACE_ES_Subscription_Module*) = 0;

  virtual ACE_ES_Supplier_Module*
    create_supplier_module (ACE_EventChannel*) = 0;
  virtual void destroy_supplier_module (ACE_ES_Supplier_Module*) = 0;

  virtual ACE_ES_Priority_Timer*
    create_timer_module (ACE_EventChannel*) = 0;
  virtual void destroy_timer_module (ACE_ES_Priority_Timer*) = 0;
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_Default_Module_Factory : public TAO_Module_Factory
{
  // = TITLE
  //   Factory class for the Event Channel "modules".
  //
  // = DESCRIPTION
  //   The Event Channel "modules" represent the different phases and
  //   strategies in event processing.  Event Channel configuration is
  //   achieved by constructing modules with different dynamic
  //   types.  This class encapsulates all the strategies creation in
  //   a single point.
  // 
public:
  TAO_Default_Module_Factory (void);
  virtual ~TAO_Default_Module_Factory (void);

  virtual ACE_ES_Dispatching_Base*
    create_dispatching_module (ACE_EventChannel*);
  virtual void destroy_dispatching_module (ACE_ES_Dispatching_Base*);

  virtual ACE_ES_Consumer_Module*
    create_consumer_module (ACE_EventChannel*);
  virtual void destroy_consumer_module (ACE_ES_Consumer_Module*);

  virtual ACE_Task_Manager*
    create_task_manager (ACE_EventChannel*);
  virtual void destroy_task_manager (ACE_Task_Manager*);

  virtual ACE_ES_Correlation_Module*
    create_correlation_module (ACE_EventChannel*);
  virtual void destroy_correlation_module (ACE_ES_Correlation_Module*);

  virtual ACE_ES_Subscription_Module*
    create_subscription_module (ACE_EventChannel*);
  virtual void destroy_subscription_module (ACE_ES_Subscription_Module*);

  virtual ACE_ES_Supplier_Module*
    create_supplier_module (ACE_EventChannel*);
  virtual void destroy_supplier_module (ACE_ES_Supplier_Module*);

  virtual ACE_ES_Priority_Timer*
    create_timer_module (ACE_EventChannel*);
  virtual void destroy_timer_module (ACE_ES_Priority_Timer*);
};

#endif /* ACE_EVENT_CHANNEL_H */
