/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    ORB Services
// 
// = FILENAME
//    Local_ESTypes.h
//
// = AUTHOR
//    Tim Harrison (harrison@cs.wustl.edu)
//
// = DESCRIPTION
//    Manual types that would otherwise be defined/implemented via an
//    IDL compiler.
//    NOTE: the file is obsolecent, we have TAO now, but we keep it
//    to speed up the porting.
//
// ============================================================================

#ifndef ACE_LOCAL_ESTYPES_H
#define ACE_LOCAL_ESTYPES_H

#include "tao/corba.h"

#include "orbsvcs/Event_Service_Constants.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/RtecSchedulerS.h"
#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventChannelAdminS.h"

#define ACE_DEFAULT_EVENT_CHANNEL_TYPE 0

// These are to help MSVC++ 4.2 deal with inheritence of nested types.
// Not needed for Sun C++ or MSVC++ 5.0.

// @@ NOTE: TAO uses the POA mapping instead of the old BOA, but all the
// code still uses the BOA name for the skeleton classes, to speed up
// porting we keep to old names.

typedef RtecEventComm::Event RtecEventComm_Event;
typedef POA_RtecScheduler::Scheduler RtecScheduler_SchedulerBOAImpl;
typedef POA_RtecEventChannelAdmin::EventChannel RtecEventChannelAdmin_EventChannelBOAImpl;
typedef POA_RtecEventComm::PushSupplier RtecEventComm_PushSupplierBOAImpl;
typedef POA_RtecEventChannelAdmin::ConsumerAdmin RtecEventChannelAdmin_ConsumerAdminBOAImpl;
typedef POA_RtecEventChannelAdmin::SupplierAdmin RtecEventChannelAdmin_SupplierAdminBOAImpl;
typedef POA_RtecEventChannelAdmin::ProxyPushConsumer RtecEventChannelAdmin_ProxyPushConsumerBOAImpl;
typedef POA_RtecEventChannelAdmin::ProxyPushSupplier RtecEventChannelAdmin_ProxyPushSupplierBOAImpl;
typedef POA_RtecEventComm::PushConsumer RtecEventComm_PushConsumerBOAImpl;
typedef POA_RtecEventComm::PushSupplier RtecEventComm_PushSupplierBOAImpl;

#if defined (__ACE_INLINE__)
#include "Local_ESTypes.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_LOCAL_ESTYPES_H */
