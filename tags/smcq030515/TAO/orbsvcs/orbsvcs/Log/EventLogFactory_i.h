/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   EventLogFactory_i.h
 *
 *  $Id$
 *
 *  Implementation of the DsEventLogAdmin::EventLogFactory interface.
 *
 *
 *  @author Rob Ruff <rruff@scires.com>
 *  @David A. Hanvey <d.hanvey@qub.ac.uk>
 *  
 */
//=============================================================================

#ifndef TAO_TLS_EVENTLOGFACTORY_I_H
#define TAO_TLS_EVENTLOGFACTORY_I_H

#include "ace/pre.h"

#include "orbsvcs/DsEventLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "EventLog_i.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_EventLog_i;
class TAO_EventLogNotification;

/**
 * @class TAO_EventLogFactory_i
 *
 * @brief The EventLogFactory is a factory that is used to create EventLogs which are event-aware.
 */
class TAO_EventLog_Export TAO_EventLogFactory_i :
  public POA_DsEventLogAdmin::EventLogFactory,
  public TAO_LogMgr_i

{
public:

  //= Initialization and termination code.

  /// Constructor.
  TAO_EventLogFactory_i (void);

  /// Destructor.
  ~TAO_EventLogFactory_i ();

  /// Initialise the EventChannel and obtain a 
  /// pointer to it.
  CosEventChannelAdmin::EventChannel_ptr
    init (PortableServer::POA_ptr poa
              ACE_ENV_ARG_DECL);

  /// Activate this servant with the POA passed in.
  DsEventLogAdmin::EventLogFactory_ptr
    activate (PortableServer::POA_ptr poa
              ACE_ENV_ARG_DECL);

  /// Used to create an EventLog.
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

  /// Same as create (), but allows clients to specify the id.
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

  // = Implementation of the CosEventChannelAdmin::ConsumerAdmin methods.
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

  /// Our object ref. after <active>ation.
  DsLogAdmin::LogMgr_var log_mgr_;
 
  /// The event channel used for log generated events.
  TAO_CEC_EventChannel *impl;

  /// The object used for log generated events.
  TAO_EventLogNotification* notifier_;

  /// EventChannel used to obtain the ConsumerAdmin.
  CosEventChannelAdmin::EventChannel_var event_channel_;
 
  /// The ConsumerAdmin that the EventLogFactory supports.
  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// POA.
  PortableServer::POA_var poa_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TLS_EVENTLOGFACTORY_I_H */
