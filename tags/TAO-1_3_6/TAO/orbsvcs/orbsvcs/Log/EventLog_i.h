/* -*- C++ -*- */

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
 *  @D A Hanvey <d.hanvey@qub.ac.uk>
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

#include "eventlog_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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
  public POA_DsEventLogAdmin::EventLog,
  public virtual PortableServer::RefCountServantBase
{
public:

  // = Initialization and Termination.

  /// Constructor.
  TAO_EventLog_i (TAO_LogMgr_i &logmgr_i,
                  DsLogAdmin::LogMgr_ptr factory,
                  TAO_EventLogFactory_i *event_log_factory,
                  TAO_LogNotification *log_notifier,
                  DsLogAdmin::LogId id,
                  DsLogAdmin::LogFullActionType log_full_action = DsLogAdmin::wrap,
                  CORBA::ULongLong max_size = 0,
                  ACE_Reactor *reactor = ACE_Reactor::instance ());

  /// Duplicate the log. 
  virtual DsLogAdmin::Log_ptr copy (DsLogAdmin::LogId &id
                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Duplicate the log specifying an id.
  virtual DsLogAdmin::Log_ptr copy_with_id (DsLogAdmin::LogId id
                                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Destroy the log object and all contained records.
  void
  destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Activate the EventLog.
  void
  activate (ACE_ENV_SINGLE_ARG_DECL);

  // = The CosEventChannelAdmin::EventChannel interface methods.
  CosEventChannelAdmin::ConsumerAdmin_ptr
  for_consumers (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
                CORBA::SystemException
        ));

  CosEventChannelAdmin::SupplierAdmin_ptr
  for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
                CORBA::SystemException
        ));

  /// Used to write records to the log.
  void write_recordlist (const DsLogAdmin::RecordList & list
    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DsLogAdmin::LogFull,
        DsLogAdmin::LogLocked,
        DsLogAdmin::LogDisabled
      ));

protected:

  /// Destructor
  /**
   * Protected destructor to enforce proper memory management through
   * reference counting.
   */
  ~TAO_EventLog_i ();

protected:

  /// Used to access the hash map that holds all the Logs created.
  TAO_LogMgr_i &logmgr_i_;

private:

  /// The Event Channel that the log uses.
  TAO_CEC_EventChannel *event_channel_;

  /// The PushConsumer that consumes the events and stores them
  /// in the log.
  TAO_Event_LogConsumer *my_log_consumer_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_EVENTLOG_I_H */
