// -*- C++ -*-

//=============================================================================
/**
 *  @file   RTEventLogConsumer.h
 *
 *  $Id$
 *
 *  The RTEventLogConsumer connects to the RTEventLog and logs the events
 *  that are pushed to the RTEventLog.
 *
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_RTEVENTLOGCONSUMER_H
#define TAO_TLS_RTEVENTLOGCONSUMER_H

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
 * @class TAO_Rtec_LogConsumer
 *
 * @brief An implementation of the PushConsumer interface.
 *
 * Connects to the RTEventLog and writes LogRecords to the
 * Log when events are 'pushed' to the RTEventLog which is an
 * EventChannel.
 */
class TAO_RTEventLog_Serv_Export TAO_Rtec_LogConsumer :public virtual POA_RtecEventComm::PushConsumer
{
public:
  // = Initialization and Termination methods.

  /// Constructor.
  TAO_Rtec_LogConsumer (TAO_RTEventLog_i *log);

  /// Destructor.
  ~TAO_Rtec_LogConsumer (void);

  /// Connect to RTEventLog.
  void connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);

private:
  /// Disconnect from EventLog.
  void disconnect_push_consumer (void);

  /// This method will call TAO_Log_i::log() to write
  /// the event to the Log.
  void push (const RtecEventComm::EventSet& events);

  /// ProxyPushSupplier used to connect to RTEventLog.
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// The RTEventLog that this consumer connects to
  /// and whose events are to be logged.
  TAO_RTEventLog_i *log_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_RTEVENTLOGCONSUMER_H */
