// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//   ftcorbafwd.h
//
// = DESCRIPTION
//   Forward declare the basic types used in the FT
//   implementation. This file should be included in
//   ORB file headers that use one or more FT namespace
//   types. A portion of the  source files will be in
//   FT_CORBA_ORBC.h and the rest in FT_CORBA_ServiceC.h in
//   $TAO_ROOT/orbsvcs/
//
// = AUTHOR
//   Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_FTCORBAFWD_H
#define TAO_FTCORBAFWD_H
#include "ace/pre.h"
#include "tao/TimeBaseC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_FT_CORBA == 1)

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Export
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

struct FT_TagFTGroupTaggedComponent;
class  FT_TagFTGroupTaggedComponent_var;
class  FT_TagFTGroupTaggedComponent_out;

struct FT_TagFTPrimaryTaggedComponent;
class  FT_TagFTPrimaryTaggedComponent_var;


struct FT_FTGroupVersionServiceContext;
class  FT_FTGroupVersionServiceContext_var;


struct FT_FTRequestServiceContext;
class  FT_FTRequestServiceContext_var;
class  FT_FTRequestServiceContext_out;

class   FT_RequestDurationPolicy;
class   FT_RequestDurationPolicy_var;
class   FT_RequestDurationPolicy_out;
typedef FT_RequestDurationPolicy *FT_RequestDurationPolicy_ptr;

struct FT_TagFTHeartbeatEnabledTaggedComponent;
class  FT_TagFTHeartbeatEnabledTaggedComponent_var;


struct FT_HeartbeatPolicyValue;
class  FT_HeartbeatPolicyValue_var;

class FT_Name;
class FT_Name_var;
class FT_Name_out;

class   FT_HeartbeatPolicy;
class   FT_HeartbeatPolicy_var;
class   FT_HeartbeatPolicy_out;
typedef FT_HeartbeatPolicy *FT_HeartbeatPolicy_ptr;

class   FT_HeartbeatEnabledPolicy;
class   FT_HeartbeatEnabledPolicy_var;
class   FT_HeartbeatEnabledPolicy_out;
typedef FT_HeartbeatEnabledPolicy *FT_HeartbeatEnabledPolicy_ptr;

class   FT_GenericFactory;
class   FT_GenericFactory_var;
class   FT_GenericFactory_out;
typedef FT_GenericFactory *FT_GenericFactory_ptr;

class FT_FaultNotifier;
class FT_FaultNotifier_var;
class FT_FaultNotifier_out;
typedef FT_FaultNotifier *FT_FaultNotifier_ptr;

struct FT_Property;
class  FT_Property_var;
class  FT_Property_out;

class FT_Properties;
class FT_Properties_var;
class FT_Properties_out;

//class FT_Location;
//class FT_Location_var;
//class FT_Location_out;

class FT_Locations;
class FT_Locations_var;
class FT_Locations_out;

typedef FT_Properties FT_Criteria;
typedef FT_Properties_var FT_Criteria_var;
typedef FT_Properties_out FT_Criteria_out;

struct FT_FactoryInfo;
class  FT_FactoryInfo_var;
class  FT_FactoryInfo_out;

class FT_FactoryInfos;
class FT_FactoryInfos_var;
class FT_FactoryInfos_out;

struct FT_FaultMonitoringIntervalAndTimeoutValue;
class  FT_FaultMonitoringIntervalAndTimeoutValue_var;
struct FT_FaultMonitoringIntervalAndTimeoutValue_out;

class FT_InterfaceNotFound;
class FT_MemberNotFound;
class FT_ObjectNotFound;
class FT_MemberAlreadyPresent;
class FT_BadReplicationStyle;
class FT_ObjectNotCreated;
class FT_ObjectNotAdded;
class FT_PrimaryNotSet;
class FT_UnsupportedProperty;
class FT_InvalidProperty;
class FT_NoFactory;
class FT_InvalidCriteria;
class FT_CannotMeetCriteria;
class FT_ObjectGroupNotFound;
class FT_PropertyManager;
class FT_PropertyManager_var;
class FT_PropertyManager_out;
typedef FT_PropertyManager *FT_PropertyManager_ptr;

class FT_ObjectGroupManager;
class FT_ObjectGroupManager_var;
class FT_ObjectGroupManager_out;
typedef FT_ObjectGroupManager *FT_ObjectGroupManager_ptr;

class FT_GenericFactory;
class FT_GenericFactory_var;
class FT_GenericFactory_out;
typedef FT_GenericFactory *FT_GenericFactory_ptr;

class FT_ReplicationManager;
class FT_ReplicationManager_var;
class FT_ReplicationManager_out;
typedef FT_ReplicationManager *FT_ReplicationManager_ptr;

class FT_PullMonitorable;
class FT_PullMonitorable_var;
class FT_PullMonitorable_out;
typedef FT_PullMonitorable *FT_PullMonitorable_ptr;

class FT_FaultNotifier;
class FT_FaultNotifier_var;
class FT_FaultNotifier_out;
typedef FT_FaultNotifier *FT_FaultNotifier_ptr;

class FT_State;
class FT_State_var;
class FT_State_out;

class FT_NoStateAvailable;
class FT_InvalidState;
class FT_NoUpdateAvailable;
class FT_InvalidUpdate;

class FT_Checkpointable;
class FT_Checkpointable_var;
class FT_Checkpointable_out;
typedef FT_Checkpointable *FT_Checkpointable_ptr;

class FT_Updateable;
class FT_Updateable_var;
class FT_Updateable_out;
typedef FT_Updateable *FT_Updateable_ptr;

TAO_NAMESPACE FT
{
  typedef char * FTDomainId;
  typedef CORBA::String_var FTDomainId_var;
  typedef CORBA::String_out FTDomainId_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FTDomainId;

  typedef CORBA::ULongLong ObjectGroupId;
  typedef CORBA::ULongLong_out ObjectGroupId_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ObjectGroupId;

  typedef CORBA::ULong ObjectGroupRefVersion;
  typedef CORBA::ULong_out ObjectGroupRefVersion_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ObjectGroupRefVersion;

  typedef FT_TagFTGroupTaggedComponent TagFTGroupTaggedComponent;
  typedef FT_TagFTGroupTaggedComponent_var TagFTGroupTaggedComponent_var;
  typedef FT_TagFTGroupTaggedComponent_out TagFTGroupTaggedComponent_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_TagFTGroupTaggedComponent;

  typedef FT_TagFTPrimaryTaggedComponent TagFTPrimaryTaggedComponent;
  typedef FT_TagFTPrimaryTaggedComponent_var TagFTPrimaryTaggedComponent_var;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_TagFTPrimaryTaggedComponent;

  typedef FT_FTGroupVersionServiceContext FTGroupVersionServiceContext;
  typedef FT_FTGroupVersionServiceContext_var FTGroupVersionServiceContext_var;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FTGroupVersionServiceContext;

  typedef FT_FTRequestServiceContext FTRequestServiceContext;
  typedef FT_FTRequestServiceContext_var FTRequestServiceContext_var;
  typedef FT_FTRequestServiceContext_out FTRequestServiceContext_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FTRequestServiceContext;

  TAO_NAMESPACE_STORAGE_CLASS const CORBA::ULong REQUEST_DURATION_POLICY;

  typedef FT_RequestDurationPolicy RequestDurationPolicy;
  typedef FT_RequestDurationPolicy_var RequestDurationPolicy_var;
  typedef FT_RequestDurationPolicy_out RequestDurationPolicy_out;
  typedef RequestDurationPolicy *RequestDurationPolicy_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_RequestDurationPolicy;

  typedef FT_TagFTHeartbeatEnabledTaggedComponent TagFTHeartbeatEnabledTaggedComponent;
  typedef FT_TagFTHeartbeatEnabledTaggedComponent_var TagFTHeartbeatEnabledTaggedComponent_var;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_TagFTHeartbeatEnabledTaggedComponent;

  TAO_NAMESPACE_STORAGE_CLASS const CORBA::ULong HEARTBEAT_POLICY;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::ULong HEARTBEAT_ENABLED_POLICY;

  typedef FT_HeartbeatPolicyValue HeartbeatPolicyValue;
  typedef FT_HeartbeatPolicyValue_var HeartbeatPolicyValue_var;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_HeartbeatPolicyValue;

  typedef FT_HeartbeatPolicy HeartbeatPolicy;
  typedef FT_HeartbeatPolicy_var HeartbeatPolicy_var;
  typedef FT_HeartbeatPolicy_out HeartbeatPolicy_out;
  typedef HeartbeatPolicy *HeartbeatPolicy_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_HeartbeatPolicy;

  typedef FT_HeartbeatEnabledPolicy HeartbeatEnabledPolicy;
  typedef FT_HeartbeatEnabledPolicy_var HeartbeatEnabledPolicy_var;
  typedef FT_HeartbeatEnabledPolicy_out HeartbeatEnabledPolicy_out;
  typedef HeartbeatEnabledPolicy *HeartbeatEnabledPolicy_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr  _tc_HeartbeatEnabledPolicy;

  typedef FT_GenericFactory GenericFactory;
  typedef FT_GenericFactory_out GenericFactory_out;
  typedef FT_GenericFactory_var GenericFactory_var;
  typedef GenericFactory *GenericFactory_ptr;

  typedef FT_FaultNotifier FaultNotifier;
  typedef FT_FaultNotifier_var FaultNotifier_var;
  typedef FT_FaultNotifier_out FaultNotifier_out;
  typedef FaultNotifier *FaultNotifier_ptr;

  typedef FT_Name Name;
  typedef FT_Name_var Name_var;
  typedef FT_Name_out Name_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Name;

  typedef Name Location;
  typedef Name_var Location_var;
  typedef Name_out Location_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Location;

  typedef char * TypeId;
  typedef CORBA::String_var TypeId_var;
  typedef CORBA::String_out TypeId_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_TypeId;

  typedef CORBA::Object ObjectGroup;
  typedef CORBA::Object_ptr ObjectGroup_ptr;
  typedef CORBA::Object_var ObjectGroup_var;
  typedef CORBA::Object_out ObjectGroup_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ObjectGroup;

  typedef CORBA::Any Value;
  typedef CORBA::Any_ptr Value_ptr;
  typedef CORBA::Any_var Value_var;
  typedef CORBA::Any_out Value_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Value;

  typedef FT_Property Property;
  typedef FT_Property_var Property_var;
  typedef FT_Property_out Property_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Property;

  typedef FT_Properties Properties;
  typedef FT_Properties_var Properties_var;
  typedef FT_Properties_out Properties_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Properties;

  typedef FT_Locations Locations;
  typedef FT_Locations_var Locations_var;
  typedef FT_Locations_out Locations_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Locations;

  typedef Properties Criteria;
  typedef Properties_var Criteria_var;
  typedef Properties_out Criteria_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Criteria;

  typedef FT_FactoryInfo FactoryInfo;
  typedef FT_FactoryInfo_var FactoryInfo_var;
  typedef FT_FactoryInfo_out FactoryInfo_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FactoryInfo;

  typedef FT_FactoryInfos FactoryInfos;
  typedef FT_FactoryInfos_var FactoryInfos_var;
  typedef FT_FactoryInfos_out FactoryInfos_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FactoryInfos;

  typedef FactoryInfos FactoriesValue;
  typedef FactoryInfos_var FactoriesValue_var;
  typedef FactoryInfos_out FactoriesValue_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FactoriesValue;

  typedef CORBA::Long ReplicationStyleValue;
  typedef CORBA::Long_out ReplicationStyleValue_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ReplicationStyleValue;

  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long STATELESS;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long COLD_PASSIVE;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long WARM_PASSIVE;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long ACTIVE;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long ACTIVE_WITH_VOTING;

  typedef CORBA::Long MembershipStyleValue;
  typedef CORBA::Long_out MembershipStyleValue_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_MembershipStyleValue;

  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long MEMB_APP_CTRL;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long MEMB_INF_CTRL;

  typedef CORBA::Long ConsistencyStyleValue;
  typedef CORBA::Long_out ConsistencyStyleValue_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ConsistencyStyleValue;

  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long CONS_APP_CTRL;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long CONS_INF_CTRL;

  typedef CORBA::Long FaultMonitoringStyleValue;
  typedef CORBA::Long_out FaultMonitoringStyleValue_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FaultMonitoringStyleValue;

  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long PULL;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long PUSH;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long NOT_MONITORED;

  typedef CORBA::Long FaultMonitoringGranularityValue;
  typedef CORBA::Long_out FaultMonitoringGranularityValue_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FaultMonitoringGranularityValue;

  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long MEMB;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long LOC;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Long LOC_AND_TYPE;

  typedef CORBA::UShort InitialNumberReplicasValue;
  typedef CORBA::UShort_out InitialNumberReplicasValue_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InitialNumberReplicasValue;

  typedef CORBA::UShort MinimumNumberReplicasValue;
  typedef CORBA::UShort_out MinimumNumberReplicasValue_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_MinimumNumberReplicasValue;

  typedef FT_FaultMonitoringIntervalAndTimeoutValue FaultMonitoringIntervalAndTimeoutValue;
  typedef FT_FaultMonitoringIntervalAndTimeoutValue_var FaultMonitoringIntervalAndTimeoutValue_var;
  typedef FT_FaultMonitoringIntervalAndTimeoutValue_out FaultMonitoringIntervalAndTimeoutValue_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FaultMonitoringIntervalAndTimeoutValue;

  typedef TimeBase::TimeT CheckpointIntervalValue;
  typedef TimeBase::TimeT_out CheckpointIntervalValue_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_CheckpointIntervalValue;

  typedef FT_InterfaceNotFound InterfaceNotFound;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InterfaceNotFound;

  typedef FT_ObjectGroupNotFound ObjectGroupNotFound;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ObjectGroupNotFound;

  typedef FT_MemberNotFound MemberNotFound;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_MemberNotFound;

  typedef FT_ObjectNotFound ObjectNotFound;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ObjectNotFound;

  typedef FT_MemberAlreadyPresent MemberAlreadyPresent;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_MemberAlreadyPresent;

  typedef FT_BadReplicationStyle BadReplicationStyle;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_BadReplicationStyle;

  typedef FT_ObjectNotCreated ObjectNotCreated;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ObjectNotCreated;

  typedef FT_ObjectNotAdded ObjectNotAdded;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ObjectNotAdded;

  typedef FT_PrimaryNotSet PrimaryNotSet;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PrimaryNotSet;

  typedef FT_UnsupportedProperty UnsupportedProperty;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_UnsupportedProperty;

  typedef FT_InvalidProperty InvalidProperty;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InvalidProperty;

  typedef FT_NoFactory NoFactory;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_NoFactory;

  typedef FT_InvalidCriteria InvalidCriteria;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InvalidCriteria;

  typedef FT_CannotMeetCriteria CannotMeetCriteria;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_CannotMeetCriteria;

  typedef FT_PropertyManager PropertyManager;
  typedef FT_PropertyManager_var PropertyManager_var;
  typedef FT_PropertyManager_out PropertyManager_out;
  typedef PropertyManager *PropertyManager_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PropertyManager;

  typedef FT_ObjectGroupManager ObjectGroupManager;
  typedef FT_ObjectGroupManager_var ObjectGroupManager_var;
  typedef FT_ObjectGroupManager_out ObjectGroupManager_out;
  typedef ObjectGroupManager *ObjectGroupManager_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ObjectGroupManager;

  typedef FT_GenericFactory GenericFactory;
  typedef FT_GenericFactory_var GenericFactory_var;
  typedef FT_GenericFactory_out GenericFactory_out;
  typedef GenericFactory *GenericFactory_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_GenericFactory;

  typedef FT_ReplicationManager ReplicationManager;
  typedef FT_ReplicationManager_var ReplicationManager_var;
  typedef FT_ReplicationManager_out ReplicationManager_out;
  typedef ReplicationManager *ReplicationManager_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ReplicationManager;

  typedef FT_PullMonitorable PullMonitorable;
  typedef FT_PullMonitorable_var PullMonitorable_var;
  typedef FT_PullMonitorable_out PullMonitorable_out;
  typedef PullMonitorable *PullMonitorable_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PullMonitorable;

  typedef FT_FaultNotifier FaultNotifier;
  typedef FT_FaultNotifier_var FaultNotifier_var;
  typedef FT_FaultNotifier_out FaultNotifier_out;
  typedef FaultNotifier *FaultNotifier_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FaultNotifier;

  typedef FT_State State;
  typedef FT_State_var State_var;
  typedef FT_State_out State_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_State;

  typedef FT_NoStateAvailable NoStateAvailable;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_NoStateAvailable;

  typedef FT_InvalidState InvalidState;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InvalidState;

  typedef FT_NoUpdateAvailable NoUpdateAvailable;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_NoUpdateAvailable;

  typedef FT_InvalidUpdate InvalidUpdate;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InvalidUpdate;

  typedef FT_Checkpointable Checkpointable;
  typedef FT_Checkpointable_var Checkpointable_var;
  typedef FT_Checkpointable_out Checkpointable_out;
  typedef Checkpointable *Checkpointable_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Checkpointable;

  typedef FT_Updateable Updateable;
  typedef FT_Updateable_var Updateable_var;
  typedef FT_Updateable_out Updateable_out;
  typedef Updateable *Updateable_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Updateable;
}
TAO_NAMESPACE_CLOSE // module FT



#endif /*TAO_HAS_FT_CORBA == 1 */

#include "ace/post.h"
#endif /*TAO_FTCORBAFWD_H*/
