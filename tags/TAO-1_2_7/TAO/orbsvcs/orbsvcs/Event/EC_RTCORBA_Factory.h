/* -*- C++ -*- */
/**
 *  @file EC_RTCORBA_Factory.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_RTCORBA_FACTORY_H
#define TAO_EC_RTCORBA_FACTORY_H
#include "ace/pre.h"

#include "rtcorba_event_export.h"
#include "EC_Factory.h"
#include "tao/RTCORBA/RTCORBA.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_EC_RTCORBA_Factory
 *
 * @brief Decorate an EC_Factory to use the EC_RTCORBA_Dispatching
 * module.
 */
class TAO_RTCORBAEvent_Export TAO_EC_RTCORBA_Factory : public TAO_EC_Factory
{
public:
  /// Constructor
  /**
   * @param body The decorated factory.
   */
  TAO_EC_RTCORBA_Factory (TAO_EC_Factory *body,
                          const RTCORBA::ThreadpoolLanes &lanes);

  /// destructor...
  virtual ~TAO_EC_RTCORBA_Factory (void);

  // Decorated methods...
  virtual int init (int argc, char* argv[]);
  virtual int fini (void);
  virtual TAO_EC_Dispatching*
      create_dispatching (TAO_EC_Event_Channel*);
  virtual void
      destroy_dispatching (TAO_EC_Dispatching*) ;
  virtual TAO_EC_Filter_Builder*
      create_filter_builder (TAO_EC_Event_Channel*)  ;
  virtual void
      destroy_filter_builder (TAO_EC_Filter_Builder*);
  virtual TAO_EC_Supplier_Filter_Builder*
      create_supplier_filter_builder (TAO_EC_Event_Channel*)  ;
  virtual void
      destroy_supplier_filter_builder (TAO_EC_Supplier_Filter_Builder*);
  virtual TAO_EC_ConsumerAdmin*
      create_consumer_admin (TAO_EC_Event_Channel*) ;
  virtual void
      destroy_consumer_admin (TAO_EC_ConsumerAdmin*);
  virtual TAO_EC_SupplierAdmin*
      create_supplier_admin (TAO_EC_Event_Channel*) ;
  virtual void
      destroy_supplier_admin (TAO_EC_SupplierAdmin*);
  virtual TAO_EC_ProxyPushSupplier*
      create_proxy_push_supplier (TAO_EC_Event_Channel*);
  virtual void
      destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier*);
  virtual TAO_EC_ProxyPushConsumer*
      create_proxy_push_consumer (TAO_EC_Event_Channel*);
  virtual void
      destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer*);
  virtual TAO_EC_Timeout_Generator*
      create_timeout_generator (TAO_EC_Event_Channel*) ;
  virtual void
      destroy_timeout_generator (TAO_EC_Timeout_Generator*);
  virtual TAO_EC_ObserverStrategy*
      create_observer_strategy (TAO_EC_Event_Channel*);
  virtual void
      destroy_observer_strategy (TAO_EC_ObserverStrategy*);
  virtual TAO_EC_Scheduling_Strategy*
      create_scheduling_strategy (TAO_EC_Event_Channel*);
  virtual void
      destroy_scheduling_strategy (TAO_EC_Scheduling_Strategy*);
  virtual TAO_EC_ProxyPushConsumer_Collection*
      create_proxy_push_consumer_collection (TAO_EC_Event_Channel*);
  virtual void
      destroy_proxy_push_consumer_collection (TAO_EC_ProxyPushConsumer_Collection*);
  virtual TAO_EC_ProxyPushSupplier_Collection*
      create_proxy_push_supplier_collection (TAO_EC_Event_Channel*);
  virtual void
      destroy_proxy_push_supplier_collection (TAO_EC_ProxyPushSupplier_Collection*);
  virtual ACE_Lock* create_consumer_lock (void);
  virtual void destroy_consumer_lock (ACE_Lock*);
  virtual ACE_Lock* create_supplier_lock (void);
  virtual void destroy_supplier_lock (ACE_Lock*);
  virtual TAO_EC_ConsumerControl*
      create_consumer_control (TAO_EC_Event_Channel*);
  virtual void
      destroy_consumer_control (TAO_EC_ConsumerControl*);
  virtual TAO_EC_SupplierControl*
      create_supplier_control (TAO_EC_Event_Channel*);
  virtual void
      destroy_supplier_control (TAO_EC_SupplierControl*);

protected:
  /// The decorated factory
  TAO_EC_Factory *body_;

  /// Configure the thread pool lanes.
  RTCORBA::ThreadpoolLanes lanes_;
};

#if defined (__ACE_INLINE__)
#include "EC_RTCORBA_Factory.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_RTCORBA_FACTORY_H */
