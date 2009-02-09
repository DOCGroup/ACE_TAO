/* -*- C++ -*- */
/**
 *  @file   EC_Default_Factory.h
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

#ifndef TAO_EC_DEFAULT_FACTORY_H
#define TAO_EC_DEFAULT_FACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_Defaults.h"

#include "ace/Service_Config.h"
#include "ace/SString.h"
#include "ace/Time_Value.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_Queue_Full_Service_Object;

/**
 * @class TAO_EC_Default_Factory
 *
 * @brief A generic factory for EC experimentation.
 *
 * This class allows the user to experiment with different EC
 * configurations.  Using a command-line like interface the user
 * can specify which strategies will this factory generate.
 * Since the class can be dynamically loaded the strategies can be
 * set in the service configurator file.
 */
class TAO_RTEvent_Serv_Export TAO_EC_Default_Factory : public TAO_EC_Factory
{
public:
  /// Constructor
  TAO_EC_Default_Factory (void);

  /// destructor...
  virtual ~TAO_EC_Default_Factory (void);

  /// Helper function to register the default factory into the service
  /// configurator.
  static int init_svcs (void);

  // = The Service_Object entry points
  virtual int init (int argc, ACE_TCHAR* argv[]);
  virtual int fini (void);

  // = The EC_Factory methods
  virtual TAO_EC_Dispatching*
      create_dispatching (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_dispatching (TAO_EC_Dispatching*);
  virtual TAO_EC_Filter_Builder*
      create_filter_builder (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_filter_builder (TAO_EC_Filter_Builder*);
  virtual TAO_EC_Supplier_Filter_Builder*
      create_supplier_filter_builder (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_supplier_filter_builder (TAO_EC_Supplier_Filter_Builder*);
  virtual TAO_EC_ConsumerAdmin*
      create_consumer_admin (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_consumer_admin (TAO_EC_ConsumerAdmin*);
  virtual TAO_EC_SupplierAdmin*
      create_supplier_admin (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_supplier_admin (TAO_EC_SupplierAdmin*);
  virtual TAO_EC_ProxyPushSupplier*
      create_proxy_push_supplier (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier*);
  virtual TAO_EC_ProxyPushConsumer*
      create_proxy_push_consumer (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer*);
  virtual TAO_EC_Timeout_Generator*
      create_timeout_generator (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_timeout_generator (TAO_EC_Timeout_Generator*);
  virtual TAO_EC_ObserverStrategy*
      create_observer_strategy (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_observer_strategy (TAO_EC_ObserverStrategy*);
  virtual TAO_EC_Scheduling_Strategy*
      create_scheduling_strategy (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_scheduling_strategy (TAO_EC_Scheduling_Strategy*);
  virtual TAO_EC_ProxyPushConsumer_Collection*
      create_proxy_push_consumer_collection (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_proxy_push_consumer_collection (TAO_EC_ProxyPushConsumer_Collection*);
  virtual TAO_EC_ProxyPushSupplier_Collection*
    create_proxy_push_supplier_collection (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_proxy_push_supplier_collection (TAO_EC_ProxyPushSupplier_Collection*);

  virtual ACE_Lock* create_consumer_lock (void);
  virtual void destroy_consumer_lock (ACE_Lock*);
  virtual ACE_Lock* create_supplier_lock (void);
  virtual void destroy_supplier_lock (ACE_Lock*);

  virtual TAO_EC_ConsumerControl*
      create_consumer_control (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_consumer_control (TAO_EC_ConsumerControl*);
  virtual TAO_EC_SupplierControl*
      create_supplier_control (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_supplier_control (TAO_EC_SupplierControl*);

  /// Accessors to consumer collection flags
  int consumer_collection (void) const;

  /// Accessors to supplier collection flags
  int supplier_collection (void) const;

  /// Accessors to supplier filtering flags
  int supplier_filtering (void) const;

  /// Accessor to ORBid
  const ACE_CString& orb_id (void) const;

protected:
  /// Helper for agrument parsing.  Prints out an error message about
  /// unsupported option value.
  void unsupported_option_value (const ACE_TCHAR * option_name,
                                 const ACE_TCHAR * option_value);

protected:
  /// Several flags to control the kind of object created.
  int dispatching_;
  int filtering_;
  int supplier_filtering_;
  int timeout_;
  int observer_;
  int scheduling_;
  int consumer_collection_;
  int supplier_collection_;
  int consumer_lock_;
  int supplier_lock_;

  /// Flags used by thread-based dispatching strategies.
  int dispatching_threads_;     //! number of threads; may be ignored depending on strategy; default: TAO_EC_DEFAULT_DISPATCHING_THREADS
  int dispatching_threads_flags_; //! flags for thread creation; default: TAO_EC_DEFAULT_DISPATCHING_THREADS_FLAGS
  int dispatching_threads_priority_; //! dispatching thread priority; default: TAO_EC_DEFAULT_DISPATCHING_THREADS_PRIORITY
  int dispatching_threads_force_active_; //! create threads with innocuous default values if creation with requested values fails
  ACE_TString queue_full_service_object_name_; //! name of ACE_Service_Object which should be invoked when output queue becomes full
  TAO_EC_Queue_Full_Service_Object* find_service_object (const ACE_TCHAR* wanted,
                                                         const ACE_TCHAR* fallback);

  /// Use this ORB to locate global resources.
  ACE_CString orbid_;

  /// The consumer and supplier control policies.
  int consumer_control_;
  int supplier_control_;

  /// The consumer and supplier control periods in usecs
  int consumer_control_period_;
  int supplier_control_period_;

  /// The consumer control timeout in usecs
  ACE_Time_Value consumer_control_timeout_;

  /// The supplier control timeout in usecs
  ACE_Time_Value supplier_control_timeout_;

  /// Validate the connection to consumer on connect
  int consumer_validate_connection_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Default_Factory.inl"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_EC_Default_Factory)
ACE_FACTORY_DECLARE (TAO_RTEvent_Serv, TAO_EC_Default_Factory)

#include /**/ "ace/post.h"

#endif /* TAO_EC_DEFAULT_FACTORY_H */
