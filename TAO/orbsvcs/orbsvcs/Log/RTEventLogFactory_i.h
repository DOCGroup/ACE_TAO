/* -*- C++ -*- */
// $Id$

// ============================================================================
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   RTEventLogFactory_i.h
//
// = DESCRIPTION
//   Implements the Factory for RTEventLog Objects.
//
// = AUTHOR
//   D A Hanvey <d.hanvey@qub.ac.uk>
//
// ============================================================================

#ifndef TLS_RTEVENTLOGFACTORY_I_H
#define TLS_RTEVENTLOGFACTORY_I_H

#include "orbsvcs/RTEventLogAdminS.h"
#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/RtecSchedulerS.h"
#include "RTEventLog_i.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "rteventlog_export.h"

class RTEventLog_i;
class RTEventLogNotification;

class TAO_RTEventLog_Export RTEventLogFactory_i :
  public POA_RTEventLogAdmin::EventLogFactory,
  public LogMgr_i
{
  // = TITLE
  //   EventLogFactory_i
  // = DESCRIPTION
  //   Implementation of the EventLogFactory interface.
  //
public:
  //= Initialization and termination code.
  RTEventLogFactory_i (void);
  // Ctor

  ~RTEventLogFactory_i ();
  // Dtor

  int
  init (PortableServer::POA_ptr poa,
          const char* child_poa_name,
          CosNaming::NamingContext_ptr naming = CosNaming::NamingContext::_nil ()
          ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  RTEventLogAdmin::EventLogFactory_ptr
    activate (PortableServer::POA_ptr poa
              ACE_ENV_ARG_DECL);
  // Activate this servant with the POA passed in.

  RTEventLogAdmin::EventLog_ptr create (
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
        DsLogAdmin::LogId_out id
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DsLogAdmin::InvalidLogFullAction,
        DsLogAdmin::InvalidThreshold
      ));

  RTEventLogAdmin::EventLog_ptr create_with_id (
        DsLogAdmin::LogId id,
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DsLogAdmin::LogIdAlreadyExists,
        DsLogAdmin::InvalidLogFullAction,
        DsLogAdmin::InvalidThreshold
      ));
  // Same as create (), but allows clients to specify the id.

  RtecEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL 
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

protected:
  DsLogAdmin::LogMgr_var log_mgr_;

  TAO_EC_Event_Channel *impl;
  // The event channel used for log generated events.

  RTEventLogNotification* notifier_;

  RtecEventChannelAdmin::EventChannel_var event_channel_;

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;

  PortableServer::POA_var poa_;
  // The POA with which we activate all the EventLogs.

  CosNaming::NamingContext_var naming_;
  // The naming context to use.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TLS_RTEVENTLOGFACTORY_I_H */
