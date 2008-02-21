// -*- C++ -*-

//=============================================================================
/**
 *  @file   RTEventLog_i.h
 *
 *  $Id$
 *
 *  Implementation of the RTEventLogAdmin::NotifyLog interface.
 *
 *
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_RTEVENTLOG_I_H
#define TAO_TLS_RTEVENTLOG_I_H

#include /**/ "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RTEventLogAdminS.h"
#include "orbsvcs/Log/Log_i.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Factory.h"
#include "orbsvcs/Event/EC_Defaults.h"
#include "orbsvcs/Log/RTEventLogConsumer.h"

#include "orbsvcs/Log/rteventlog_serv_export.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LogMgr_i;
class TAO_RTEventLog_i;
class TAO_RTEventLogFactory_i;

/**
 * @class TAO_RTEventLog_i
 *
 * @brief The RTEventLog is an EventChannel and a Log.
 *
 * It is used to log events that pass through the EventChannel.
 * The class supports the @c destroy> method to destroy the Log.
 */
class TAO_RTEventLog_Serv_Export TAO_RTEventLog_i :
  public TAO_Log_i,
  public POA_RTEventLogAdmin::EventLog
{
public:
  // = Initialization and Termination methods.

  /// Constructor.
  TAO_RTEventLog_i (CORBA::ORB_ptr orb,
		    PortableServer::POA_ptr poa,
		    PortableServer::POA_ptr log_poa,
                    TAO_LogMgr_i &logmgr_i,
                    DsLogAdmin::LogMgr_ptr factory,
                    TAO_LogNotification *log_notifier,
                    DsLogAdmin::LogId id);

  /// Destructor.
  ~TAO_RTEventLog_i ();

  /// Duplicate the log.
  virtual DsLogAdmin::Log_ptr copy (DsLogAdmin::LogId &id);

  /// Duplicate the log specifying an id.
  virtual DsLogAdmin::Log_ptr copy_with_id (DsLogAdmin::LogId id);

  /// Destroy the log object and all contained records.
  void destroy (void);

  /// Activate the RTEventLog.
  void activate (void);

  // = The RtecEventChannelAdmin::EventChannel interface methods.
  RtecEventChannelAdmin::ConsumerAdmin_ptr for_consumers (void);

  RtecEventChannelAdmin::SupplierAdmin_ptr for_suppliers (void);

  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr);
  virtual void
      remove_observer (RtecEventChannelAdmin::Observer_Handle);

 private:
  /// The EventChannel used.
  TAO_EC_Event_Channel *event_channel_;

  /// The LogConsumer which logs the events the EventChannel above
  /// receives.
  TAO_Rtec_LogConsumer *my_log_consumer_;

  /// The observer strategy
  TAO_EC_ObserverStrategy *observer_strategy_;

  PortableServer::POA_var	poa_;

  PortableServer::POA_var	log_poa_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_RTEVENTLOG_I_H */
