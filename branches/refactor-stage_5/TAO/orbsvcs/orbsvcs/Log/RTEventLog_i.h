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
#include "orbsvcs/orbsvcs/Event/EC_Factory.h"
#include "orbsvcs/orbsvcs/Event/EC_Defaults.h"
#include "orbsvcs/Log/RTEventLogConsumer.h"

#include "rteventlog_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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
class TAO_RTEventLog_Export TAO_RTEventLog_i : 
  public TAO_Log_i,
  public POA_RTEventLogAdmin::EventLog,
  public virtual PortableServer::RefCountServantBase
{
public:
  // = Initialization and Termination methods.

  /// Constructor.
  TAO_RTEventLog_i (TAO_LogMgr_i &logmgr_i,
                    DsLogAdmin::LogMgr_ptr factory,
                    TAO_RTEventLogFactory_i *event_log_factory,
                    TAO_LogNotification *log_notifier,
                    DsLogAdmin::LogId id,
                    DsLogAdmin::LogFullActionType log_full_action = DsLogAdmin::wrap,
                    CORBA::ULongLong max_size = 0,
                    ACE_Reactor *reactor = ACE_Reactor::instance ());

  /// Destructor.
  ~TAO_RTEventLog_i ();

  /// Duplicate the log.
  virtual DsLogAdmin::Log_ptr copy (DsLogAdmin::LogId &id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Duplicate the log specifying an id.
  virtual DsLogAdmin::Log_ptr copy_with_id (DsLogAdmin::LogId id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((DsLogAdmin::LogIdAlreadyExists, CORBA::SystemException));

  /// Destroy the log object and all contained records.
  void
  destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Activate the RTEventLog.
  void
  activate (ACE_ENV_SINGLE_ARG_DECL);

  // = The RtecEventChannelAdmin::EventChannel interface methods.
  RtecEventChannelAdmin::ConsumerAdmin_ptr
  for_consumers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  RtecEventChannelAdmin::SupplierAdmin_ptr
  for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr
                       ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
          RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER));
  virtual void
      remove_observer (RtecEventChannelAdmin::Observer_Handle
                       ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
          RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER));

  /// Used to write records to the log.
  void write_recordlist (const DsLogAdmin::RecordList & list
    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DsLogAdmin::LogFull,
        DsLogAdmin::LogLocked
      ));

 protected:
  /// Used to access the hash map that holds all the Logs created.
  TAO_LogMgr_i &logmgr_i_;

 private:
  /// The EventChannel used.
  TAO_EC_Event_Channel *event_channel_;

  /// The LogConsumer which logs the events the EventChannel above
  /// receives.
  TAO_Rtec_LogConsumer *my_log_consumer_;

  /// The observer strategy
  TAO_EC_ObserverStrategy *observer_strategy_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_RTEVENTLOG_I_H */
