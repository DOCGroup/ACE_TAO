/* -*- C++ -*- */
// $Id$

// ============================================================================
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   EventLog_i.h
//
// = DESCRIPTION
//   Implementation of the DsLogAdmin::EventLog interface.
//
// = AUTHOR
//   Rob Ruff <rruff@scires.com>
//   D A Hanvey <d.hanvey@qub.ac.uk>
//
// ============================================================================

#ifndef TLS_EVENTLOG_I_H
#define TLS_EVENTLOG_I_H

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/DsEventLogAdminS.h"
#include "orbsvcs/Log/Log_i.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"

#include "eventlog_export.h"

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
class EventLog_i;
class EventLogFactory_i;

// Bug in MSVC 5, See KB article Q167350
#if defined (_MSC_VER) && (_MSC_VER == 1100)
using DsLogAdmin::wrap;
#endif /* (_MSC_VER) && (_MSC_VER == 1100) */


class LogConsumer : public virtual POA_CosEventComm::PushConsumer
{
  public:
    LogConsumer (EventLog_i *log);
    ~LogConsumer (void);

    void
      connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);

  private:
    void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
	ACE_THROW_SPEC ((
		CORBA::SystemException
	));
    void push (const CORBA::Any& data ACE_ENV_ARG_DECL)
	ACE_THROW_SPEC ((
		CORBA::SystemException,
		CosEventComm::Disconnected
	));

    CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

    EventLog_i *log_;
};


class EventLog_i : public Log_i,    
                   public POA_DsEventLogAdmin::EventLog,
                   public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   EventLog_i
  // = DESCRIPTION
  //   The class supports the <destroy> method to destroy the Log.
  //
public:
  // = Initialization and Termination.
  EventLog_i (LogMgr_i &logmgr_i,
              DsLogAdmin::LogMgr_ptr factory,
              EventLogFactory_i *event_log_factory,
              LogNotification *log_notifier,
              DsLogAdmin::LogId id,
              DsLogAdmin::LogFullActionType log_full_action = DsLogAdmin::wrap,
              CORBA::ULongLong max_size = 0,
              ACE_Reactor *reactor = ACE_Reactor::instance ());
  // Constructor

  ~EventLog_i ();
  // Destructor.

  virtual DsLogAdmin::Log_ptr copy (DsLogAdmin::LogId &id)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Duplicate the log.

  virtual DsLogAdmin::Log_ptr copy_with_id (DsLogAdmin::LogId id)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Duplicate the log specifying an id.

  void
  destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Destroy the log object and all contained records.

  void
  activate (void);

  CosEventChannelAdmin::ConsumerAdmin_ptr 
  for_consumers (ACE_ENV_SINGLE_ARG_DECL)
	ACE_THROW_SPEC ((
		CORBA::SystemException
	));
  // The CosEventChannelAdmin::EventChannel interface.

  CosEventChannelAdmin::SupplierAdmin_ptr 
  for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
	ACE_THROW_SPEC ((
		CORBA::SystemException
	));
  // The CosEventChannelAdmin::EventChannel interface.

  void write_recordlist (const DsLogAdmin::RecordList & list
    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DsLogAdmin::LogFull,
        DsLogAdmin::LogLocked
      ));
  // Used to write records to the log.


 protected:
  LogMgr_i &logmgr_i_;
  // Used to access the hash map that holds all the Logs created.

 private:
  TAO_CEC_EventChannel *event_channel_;
  // The Event Channel that the log uses.

  LogConsumer *my_log_consumer_;
  // The PushConsumer that consumes the events and stores them
  // in the log.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TLS_EVENTLOG_I_H */
