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

#ifndef TLS_RTEVENTLOG_I_H
#define TLS_RTEVENTLOG_I_H

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/RTEventLogAdminS.h"
#include "orbsvcs/Log/Log_i.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/orbsvcs/Event/EC_Factory.h"
#include "orbsvcs/orbsvcs/Event/EC_Defaults.h"

#include "rteventlog_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class LogMgr_i;
class RTEventLog_i;
class RTEventLogFactory_i;

// Bug in MSVC 5, See KB article Q167350
#if defined (_MSC_VER) && (_MSC_VER == 1100)
using DsLogAdmin::wrap;
#endif /* (_MSC_VER) && (_MSC_VER == 1100) */

class LogConsumer : public virtual POA_RtecEventComm::PushConsumer
{
public:
  LogConsumer (RTEventLog_i *log);
  ~LogConsumer (void);

  void
  connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);

private:
  void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void push (const RtecEventComm::EventSet& events ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  RTEventLog_i *log_;
};

class TAO_RTEventLog_Export RTEventLog_i : public Log_i,
                                           public POA_RTEventLogAdmin::EventLog,
                                           public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   EventLog_i
  // = DESCRIPTION
  //   The class supports the <destroy> method to destroy the Log.
  //
public:
  // = Initialization and Termination.
  RTEventLog_i (LogMgr_i &logmgr_i,
              DsLogAdmin::LogMgr_ptr factory,
              RTEventLogFactory_i *event_log_factory,
              LogNotification *log_notifier,
              DsLogAdmin::LogId id,
              DsLogAdmin::LogFullActionType log_full_action = DsLogAdmin::wrap,
              CORBA::ULongLong max_size = 0,
              ACE_Reactor *reactor = ACE_Reactor::instance ());
  // Constructor

  ~RTEventLog_i ();
  // Destructor.

  virtual DsLogAdmin::Log_ptr copy (DsLogAdmin::LogId &id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual DsLogAdmin::Log_ptr copy_with_id (DsLogAdmin::LogId id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((DsLogAdmin::LogIdAlreadyExists, CORBA::SystemException));

  void
  destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Destroy the log object and all contained records.

  void
  activate (ACE_ENV_SINGLE_ARG_DECL);

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


  void write_recordlist (const DsLogAdmin::RecordList & list
    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DsLogAdmin::LogFull,
        DsLogAdmin::LogLocked
      ));

 protected:
  LogMgr_i &logmgr_i_;
  // Used to access the hash map that holds all the Logs created.
 private:
  TAO_EC_Event_Channel *event_channel_;

  LogConsumer *my_log_consumer_;

  /// The observer strategy
  TAO_EC_ObserverStrategy *observer_strategy_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TLS_RTEVENTLOG_I_H */
