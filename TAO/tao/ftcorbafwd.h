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
//   FT_CORBA_ORB.h and the rest in FT_CORBA_Service.h in
//   $TAO_ROOT/orbsvcs/ 
//
// = AUTHOR
//   Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_FTCORBAFWD_H
#define TAO_FTCORBAFWD_H
#include "ace/pre.h"

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
class FT_TagFTPrimaryTaggedComponent_var;
class FT_TagFTPrimaryTaggedComponent_out;

struct FT_FTGroupVersionServiceContext;
class FT_FTGroupVersionServiceContext_var;
class FT_FTGroupVersionServiceContext_out;

struct FT_FTRequestServiceContext;
class FT_FTRequestServiceContext_var;
class FT_FTRequestServiceContext_out;

class FT_RequestDurationPolicy;
class FT_RequestDurationPolicy_var;
class FT_RequestDurationPolicy_out;
typedef FT_RequestDurationPolicy *FT_RequestDurationPolicy_ptr;

struct FT_TagFTHeartbeatEnabledTaggedComponent;
class FT_TagFTHeartbeatEnabledTaggedComponent_var;
class FT_TagFTHeartbeatEnabledTaggedComponent_out;

struct FT_HeartbeatPolicyValue;
class FT_HeartbeatPolicyValue_var;
class FT_HeartbeatPolicyValue_out;

class FT_HeartbeatPolicy;
class FT_HeartbeatPolicy_var;
class FT_HeartbeatPolicy_out;
typedef FT_HeartbeatPolicy *FT_HeartbeatPolicy_ptr;

class FT_HeartbeatEnabledPolicy;
class FT_HeartbeatEnabledPolicy_var;
class FT_HeartbeatEnabledPolicy_out;
typedef FT_HeartbeatEnabledPolicy *FT_HeartbeatEnabledPolicy_ptr;

class FT_GenericFactory;
class FT_GenericFactory_var;
class FT_GenericFactory_out;
typedef FT_GenericFactory *FT_GenericFactory_ptr;

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
  typedef FT_TagFTPrimaryTaggedComponent_out TagFTPrimaryTaggedComponent_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_TagFTPrimaryTaggedComponent;
    
  typedef FT_FTGroupVersionServiceContext FTGroupVersionServiceContext;
  typedef FT_FTGroupVersionServiceContext_var FTGroupVersionServiceContext_var;
  typedef FT_FTGroupVersionServiceContext_out FTGroupVersionServiceContext_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FTGroupVersionServiceContext;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::ULong REQUEST_DURATION_POLICY;
  
  typedef FT_FTRequestServiceContext FTRequestServiceContext;
  typedef FT_FTRequestServiceContext_var FTRequestServiceContext_var;
  typedef FT_FTRequestServiceContext_out FTRequestServiceContext_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FTRequestServiceContext;
  
  typedef FT_RequestDurationPolicy RequestDurationPolicy;
  typedef FT_RequestDurationPolicy_var RequestDurationPolicy_var;
  typedef FT_RequestDurationPolicy_out RequestDurationPolicy_out;
  typedef RequestDurationPolicy *RequestDurationPolicy_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_RequestDurationPolicy;
  
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::ULong HEARTBEAT_POLICY;
  
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::ULong HEARTBEAT_ENABLED_POLICY;
  
  typedef FT_TagFTHeartbeatEnabledTaggedComponent TagFTHeartbeatEnabledTaggedComponent;
  typedef FT_TagFTHeartbeatEnabledTaggedComponent_var TagFTHeartbeatEnabledTaggedComponent_var;
  typedef FT_TagFTHeartbeatEnabledTaggedComponent_out TagFTHeartbeatEnabledTaggedComponent_out;
  
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_TagFTHeartbeatEnabledTaggedComponent;

  typedef FT_HeartbeatPolicyValue HeartbeatPolicyValue;
  typedef FT_HeartbeatPolicyValue_var HeartbeatPolicyValue_var;
  typedef FT_HeartbeatPolicyValue_out HeartbeatPolicyValue_out;
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
  
};

#endif /*TAO_HAS_FT_CORBA == 1 */

#include "ace/post.h"
#endif /*TAO_FTCORBAFWD_H*/
