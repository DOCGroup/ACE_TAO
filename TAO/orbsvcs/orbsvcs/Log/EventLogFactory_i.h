/* -*- C++ -*- */
// $Id$

// ============================================================================
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   EventLogFactory_i.h
//
// = DESCRIPTION
//   Implements the Factory for EventLog Objects.
//
// = AUTHOR
//   Rob Ruff <rruff@scires.com>
//   D A Hanvey <d.hanvey@qub.ac.uk>
//
// ============================================================================

#ifndef TLS_EVENTLOGFACTORY_I_H
#define TLS_EVENTLOGFACTORY_I_H

#include "orbsvcs/DsEventLogAdminS.h"
#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "EventLog_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


class EventLog_i;
class EventLogNotification;

class TAO_EventLog_Export EventLogFactory_i :
public POA_DsEventLogAdmin::EventLogFactory,
public LogMgr_i

{
  // = TITLE
  //   EventLogFactory_i
  // = DESCRIPTION
  //   Implementation of the EventLogFactory interface.
  //
public:
  //= Initialization and termination code.
  EventLogFactory_i (void);
  // Ctor

  ~EventLogFactory_i ();
  // Dtor

  CosEventChannelAdmin::EventChannel_ptr
    init (PortableServer::POA_ptr poa
              ACE_ENV_ARG_DECL);

  DsEventLogAdmin::EventLogFactory_ptr
    activate (PortableServer::POA_ptr poa
              ACE_ENV_ARG_DECL);
  // Activate this servant with the POA passed in.

  DsEventLogAdmin::EventLog_ptr create (
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

  // Allows clients to create new BasicLog objects. Raises
  // DsLogAdmin::NoResources and DsLogAdmin::InvalidThreshold
  DsEventLogAdmin::EventLog_ptr create_with_id (
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

  CosEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL 
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  CosEventChannelAdmin::ProxyPullSupplier_ptr obtain_pull_supplier (
        ACE_ENV_SINGLE_ARG_DECL 
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

 protected:
  DsLogAdmin::LogMgr_var log_mgr_;
  // Our object ref. after <active>ation.

  TAO_CEC_EventChannel *impl;
  // The event channel used for log generated events.

  EventLogNotification* notifier_;
  // The object used for log generated events.

  CosEventChannelAdmin::EventChannel_var event_channel_;
  // EventChannel used to obtain the ConsumerAdmin.

  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The ConsumerAdmin that the EventLogFactory supports.

  PortableServer::POA_var poa_;
  // POA.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TLS_EVENTLOGFACTORY_I_H */
