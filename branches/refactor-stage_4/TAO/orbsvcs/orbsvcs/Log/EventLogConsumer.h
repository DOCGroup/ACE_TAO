/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   EventLogConsumer.h
 *
 *  $Id$
 *
 *  The EventLogConsumer connects to the EventLog and logs the events
 *  that are pushed to the EventLog.
 *
 *  @author Rob Ruff <rruff@scires.com>
 *  @D A Hanvey <d.hanvey@qub.ac.uk>
 */
// ============================================================================

#ifndef TAO_TLS_EVENTLOGCONSUMER_H
#define TAO_TLS_EVENTLOGCONSUMER_H

#include "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "orbsvcs/DsEventLogAdminS.h"
#include "orbsvcs/Log/Log_i.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"

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
 * @class TAO_Event_LogConsumer
 *
 * @brief An implementation of the PushConsumer interface.
 * 
 * Connects to the EventLog and writes LogRecords to the
 * Log when events are 'pushed' to the EventLog which is an
 * EventChannel.
 */
class TAO_Event_LogConsumer : public virtual POA_CosEventComm::PushConsumer
{
public:

  // = Initialization and Termination methods.

  /// Constructor.
  TAO_Event_LogConsumer (TAO_EventLog_i *log);

  /// Destructor.
  ~TAO_Event_LogConsumer (void);

  /// Connect to EventLog.
  void
    connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);

private:

  /// Disconnect from EventLog.
  void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
            CORBA::SystemException
    ));

  /// This method will call TAO_Log_i::log() to write
  /// the event to the Log.
  void push (const CORBA::Any& data ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
            CORBA::SystemException,
            CosEventComm::Disconnected
    ));

  /// ProxyPushSupplier used to connect to EventLog.
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// The EventLog that this consumer connects to
  /// and whose events are to be logged.
  TAO_EventLog_i *log_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_TLS_EVENTLOGCONSUMER_H */
