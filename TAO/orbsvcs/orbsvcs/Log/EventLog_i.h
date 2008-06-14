// -*- C++ -*-

// ============================================================================
/**
 *  @file   EventLog_i.h
 *
 *  $Id$
 *
 *  Implementation of the DsLogAdmin::EventLog interface.
 *  File also conatins TAO_Event_LogConsumer which is used
 *  to write events to the Log.
 *
 *  @author Rob Ruff <rruff@scires.com>
 *  @author D A Hanvey <d.hanvey@qub.ac.uk>
 */
// ============================================================================

#ifndef TAO_TLS_EVENTLOG_I_H
#define TAO_TLS_EVENTLOG_I_H

#include /**/ "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "orbsvcs/DsEventLogAdminS.h"
#include "orbsvcs/Log/Log_i.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/Log/EventLogConsumer.h"

#include "orbsvcs/Log/eventlog_serv_export.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LogMgr_i;
class TAO_EventLog_i;
class TAO_EventLogFactory_i;

/**
 * @class TAO_EventLog_i
 *
 * @brief The EventLog is an EventChannel and a Log.
 *
 * It is used to log events that pass through the EventChannel.
 * The class supports the @c destroy> method to destroy the Log.
 */
class TAO_EventLog_i :
  public TAO_Log_i,
  public POA_DsEventLogAdmin::EventLog
{
public:

  // = Initialization and Termination.

  /// Constructor.
  TAO_EventLog_i (CORBA::ORB_ptr orb,
                  PortableServer::POA_ptr poa,
                  PortableServer::POA_ptr log_poa,
                  TAO_LogMgr_i &logmgr_i,
                  DsLogAdmin::LogMgr_ptr factory,
                  TAO_LogNotification *log_notifier,
                  DsLogAdmin::LogId id);

  /// Duplicate the log.
  virtual DsLogAdmin::Log_ptr copy (DsLogAdmin::LogId &id);

  /// Duplicate the log specifying an id.
  virtual DsLogAdmin::Log_ptr copy_with_id (DsLogAdmin::LogId id);

  /// Destroy the log object and all contained records.
  void
  destroy (void);

  /// Activate the EventLog.
  void
  activate (void);

  // = The CosEventChannelAdmin::EventChannel interface methods.
  CosEventChannelAdmin::ConsumerAdmin_ptr
  for_consumers (void);

  CosEventChannelAdmin::SupplierAdmin_ptr
  for_suppliers (void);

protected:

  /// Destructor
  /**
   * Protected destructor to enforce proper memory management through
   * reference counting.
   */
  ~TAO_EventLog_i ();

private:

  /// The Event Channel that the log uses.
  TAO_CEC_EventChannel *event_channel_;

  /// The PushConsumer that consumes the events and stores them
  /// in the log.
  TAO_Event_LogConsumer *my_log_consumer_;

  PortableServer::POA_var	poa_;

  PortableServer::POA_var	log_poa_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_EVENTLOG_I_H */
