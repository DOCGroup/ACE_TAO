// -*- C++ -*-

//=============================================================================
/**
 *  @file   NotifyLog_i.h
 *
 *  $Id$
 *
 *  Implementation of the DsNotifyLogAdmin::NotifyLog interface.
 *  
 *
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TLS_NOTIFYLOG_I_H
#define TLS_NOTIFYLOG_I_H

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/DsNotifyLogAdminS.h"
#include "orbsvcs/Log/Log_i.h"
#include "orbsvcs/Notify/Notify_EventChannel_i.h"

#include "notifylog_export.h"

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
class NotifyLog_i;
class NotifyLogFactory_i;

// Bug in MSVC 5, See KB article Q167350
#if defined (_MSC_VER) && (_MSC_VER == 1100)
using DsLogAdmin::wrap;
#endif /* (_MSC_VER) && (_MSC_VER == 1100) */

class LogConsumer : public virtual POA_CosNotifyComm::StructuredPushConsumer, 
                    public PortableServer::RefCountServantBase
{
public:
  LogConsumer (NotifyLog_i *log);

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin ACE_ENV_ARG_DECL);

  virtual void disconnect (ACE_ENV_SINGLE_ARG_DECL);

protected:
  virtual ~LogConsumer (void);

  // = NotifyPublish method
  virtual void offer_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      CosNotifyComm::InvalidEventType
    ));

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
      const CosNotification::StructuredEvent & notification
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      CosEventComm::Disconnected
     ));

  virtual void disconnect_structured_push_consumer (
      ACE_ENV_SINGLE_ARG_DECL
      )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;

  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;

  NotifyLog_i *log_;
};

class NotifyLog_i : public Log_i,
                    public POA_DsNotifyLogAdmin::NotifyLog,
                    public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   EventLog_i
  // = DESCRIPTION
  //   The class supports the <destroy> method to destroy the Log.
  //
public:
  // = Initialization and Termination.
  NotifyLog_i (LogMgr_i &logmgr_i,
              DsLogAdmin::LogMgr_ptr factory,
              NotifyLogFactory_i *notify_log_factory,
              CosNotifyChannelAdmin::EventChannelFactory_ptr ecf,
              LogNotification *log_notifier,
              DsLogAdmin::LogId id,
              DsLogAdmin::LogFullActionType log_full_action = DsLogAdmin::wrap,
              CORBA::ULongLong max_size = 0,
              ACE_Reactor *reactor = ACE_Reactor::instance ());
  // Constructor

  ~NotifyLog_i ();
  // Destructor.

  virtual DsLogAdmin::Log_ptr copy (DsLogAdmin::LogId &id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual DsLogAdmin::Log_ptr copy_with_id (DsLogAdmin::LogId id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  void
  destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Destroy the log object and all contained records.

  void
  activate (ACE_ENV_SINGLE_ARG_DECL);

  //IDL to C++
  CosNotifyFilter::Filter_ptr
  get_filter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  void set_filter (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::EventChannelFactory_ptr
  MyFactory (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::ConsumerAdmin_ptr
  default_consumer_admin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::SupplierAdmin_ptr
  default_supplier_admin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyFilter::FilterFactory_ptr
  default_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::ConsumerAdmin_ptr
  new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID& id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::SupplierAdmin_ptr
  new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID& id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::ConsumerAdmin_ptr
  get_consumeradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::AdminNotFound, 
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::SupplierAdmin_ptr
  get_supplieradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::AdminNotFound, 
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::AdminIDSeq*
  get_all_consumeradmins (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::AdminIDSeq*
  get_all_supplieradmins (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotification::AdminProperties*
  get_admin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  void set_admin (const CosNotification::AdminProperties& admin ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedAdmin, 
      CORBA::SystemException
    ));

  CosNotification::QoSProperties*
  get_qos (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  void set_qos (const CosNotification::QoSProperties& ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedQoS, 
      CORBA::SystemException
    ));

  void validate_qos (const CosNotification::QoSProperties&,
    CosNotification::NamedPropertyRangeSeq_out ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedQoS, 
      CORBA::SystemException
    ));

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
  LogConsumer *my_log_consumer_;

  CosNotifyChannelAdmin::EventChannel_var event_channel_;

  CosNotifyChannelAdmin::ChannelID channelID_;

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TLS_NOTIFYLOG_I_H */
