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
//   ORB file headers that use one or more POA_FT namespace
//   types. A portion of the  source files will be in
//   FT_CORBA_ORBS.h and the rest in FT_CORBA_ServiceS.h in
//   $TAO_ROOT/orbsvcs/
//
// = AUTHOR
//   Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_FTPOAFWD_H
#define TAO_FTPOAFWD_H
#include "ace/pre.h"
#include "tao/ftcorbafwd.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_FT_CORBA == 1)



class FT_POA_RequestDurationPolicy;
typedef FT_POA_RequestDurationPolicy *FT_POA_RequestDurationPolicy_ptr;

class FT_POA_tao_thru_poa_collocated_RequestDurationPolicy;

class FT_POA_HeartbeatPolicy;
typedef FT_POA_HeartbeatPolicy *FT_POA_HeartbeatPolicy_ptr;

class FT_POA_tao_thru_poa_collocated_HeartbeatPolicy;

class FT_POA_HeartbeatEnabledPolicy;
typedef FT_POA_HeartbeatEnabledPolicy *FT_POA_HeartbeatEnabledPolicy_ptr;

class FT_POA_tao_thru_poa_collocated_HeartbeatEnabledPolicy;

class FT_POA_PropertyManager;
typedef FT_POA_PropertyManager *FT_POA_PropertyManager_ptr;

class FT_POA_tao_thru_poa_collocated_PropertyManager;

class FT_POA_ObjectGroupManager;
typedef FT_POA_ObjectGroupManager *FT_POA_ObjectGroupManager_ptr;

class FT_POA_tao_thru_poa_collocated_ObjectGroupManager;

class FT_POA_GenericFactory;
typedef FT_POA_GenericFactory *FT_POA_GenericFactory_ptr;

class FT_POA_tao_thru_poa_collocated_GenericFactory;

class FT_POA_ReplicationManager;
typedef FT_POA_ReplicationManager *FT_POA_ReplicationManager_ptr;

class FT_POA_tao_thru_poa_collocated_ReplicationManager;

class FT_POA_PullMonitorable;
typedef FT_POA_PullMonitorable *FT_POA_PullMonitorable_ptr;

class FT_POA_tao_thru_poa_collocated_PullMonitorable;

class FT_POA_FaultNotifier;
typedef FT_POA_FaultNotifier *FT_POA_FaultNotifier_ptr;

class FT_POA_tao_thru_poa_collocated_FaultNotifier;

class FT_POA_Checkpointable;
typedef FT_POA_Checkpointable *FT_POA_Checkpointable_ptr;

class FT_POA_tao_thru_poa_collocated_Checkpointable;

class FT_POA_Updateable;
typedef FT_POA_Updateable *FT_POA_Updateable_ptr;

class FT_POA_tao_thru_poa_collocated_Updateable;

#if defined (ACE_HAS_USING_KEYWORD)

# if 0
 // This wouldn't work. We need to find a way around for these
 // guys. If somebody wants to use it, we may have to do something
 // about it.
class FT_POA_HeartbeatPolicy_tie;
template <class T> class FT_POA_RequestDurationPolicy_tie;
class FT_POA_HeartbeatEnabledPolicy_tie;
class FT_POA_ObjectGroupManager_tie;
class FT_POA_PropertyManager_tie;
class FT_POA_GenericFactory_tie;
class FT_POA_ReplicationManager_tie;
class FT_POA_PullMonitorable_tie;
class FT_POA_FaultNotifier_tie;
class FT_POA_Checkpointable_tie;
class FT_POA_Updateable_tie;

#endif /*if 0 */
#endif /* ACE_HAS_USING_KEYWORD */

TAO_NAMESPACE POA_FT
{
 typedef FT_POA_RequestDurationPolicy RequestDurationPolicy;
 typedef RequestDurationPolicy *RequestDurationPolicy_ptr;

 typedef FT_POA_tao_thru_poa_collocated_RequestDurationPolicy
   _tao_thru_poa_collocated_RequestDurationPolicy;

 typedef FT_POA_HeartbeatPolicy HeartbeatPolicy;
 typedef HeartbeatPolicy *HeartbeatPolicy_ptr;

 typedef FT_POA_tao_thru_poa_collocated_HeartbeatPolicy
   _tao_thru_poa_collocated_HeartbeatPolicy;

 typedef FT_POA_HeartbeatEnabledPolicy HeartbeatEnabledPolicy;
 typedef HeartbeatEnabledPolicy *HeartbeatEnabledPolicy_ptr;

 typedef FT_POA_tao_thru_poa_collocated_HeartbeatEnabledPolicy
   _tao_thru_poa_collocated_HeartbeatEnabledPolicy;

 typedef FT_POA_PropertyManager PropertyManager;
 typedef PropertyManager *PropertyManager_ptr;

 typedef FT_POA_tao_thru_poa_collocated_PropertyManager
   _tao_thru_poa_collocated_PropertyManager;

 typedef FT_POA_ObjectGroupManager ObjectGroupManager;
 typedef ObjectGroupManager *ObjectGroupManager_ptr;

 typedef FT_POA_tao_thru_poa_collocated_ObjectGroupManager
   _tao_thru_poa_collocated_ObjectGroupManager;

 typedef FT_POA_GenericFactory GenericFactory;
 typedef GenericFactory *GenericFactory_ptr;

 typedef FT_POA_tao_thru_poa_collocated_GenericFactory
   _tao_thru_poa_collocated_GenericFactory;

 typedef FT_POA_ReplicationManager ReplicationManager;
 typedef ReplicationManager *ReplicationManager_ptr;

 typedef FT_POA_tao_thru_poa_collocated_ReplicationManager
   _tao_thru_poa_collocated_ReplicationManager;

 typedef FT_POA_PullMonitorable PullMonitorable;
 typedef PullMonitorable *PullMonitorable_ptr;

 typedef FT_POA_tao_thru_poa_collocated_PullMonitorable
   _tao_thru_poa_collocated_PullMonitorable;

 typedef FT_POA_FaultNotifier FaultNotifier;
 typedef FaultNotifier *FaultNotifier_ptr;

 typedef FT_POA_tao_thru_poa_collocated_FaultNotifier
   _tao_thru_poa_collocated_FaultNotifier;

 typedef FT_POA_Checkpointable Checkpointable;
 typedef Checkpointable *Checkpointable_ptr;

 typedef FT_POA_tao_thru_poa_collocated_Checkpointable
   _tao_thru_poa_collocated_Checkpointable;

 typedef FT_POA_Updateable Updateable;
 typedef Updateable *Updateable_ptr;

 typedef FT_POA_tao_thru_poa_collocated_Updateable
   _tao_thru_poa_collocated_Updateable;

#if defined (ACE_HAS_USING_KEYWORD)

# if 0
 // This wouldn't work. We need to find a way around for these
 // guys. If somebody wants to use it, we may have to do something
 // about it.
 typedef FT_POA_RequestDurationPolicy_tie
   RequestDurationPolicy_tie;
 typedef FT_POA_HeartbeatPolicy_tie HeartbeatPolicy_tie;
 typedef FT_POA_HeartbeatEnabledPolicy_tie HeartbeatEnabledPolicy_tie;
 typedef FT_POA_PropertyManager_tie PropertyManager_tie;
 typedef FT_POA_ObjectGroupManager_tie  ObjectGroupManager_tie;
 typedef FT_POA_GenericFactory_tie GenericFactory_tie;
 typedef FT_POA_ReplicationManager_tie ReplicationManager_tie;
 typedef FT_POA_PullMonitorable_tie PullMonitorable_tie;
 typedef FT_POA_FaultNotifier_tie FaultNotifier_tie;
 typedef FT_POA_Checkpointable_tie Checkpointable_tie;
 typedef FT_POA_Updateable_tie Updateable_tie;
#endif /*if 0*/
#endif /*ACE_HAS_USING_KEYWORD */

}
TAO_NAMESPACE_CLOSE // module FT_POA

#endif /*TAO_HAS_FT_CORBA == 1 */

#include "ace/post.h"
#endif /*TAO_FTPOAFWD_H*/
