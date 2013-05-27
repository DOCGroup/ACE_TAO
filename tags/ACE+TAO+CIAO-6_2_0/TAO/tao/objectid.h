// -*- C++ -*-

//=============================================================================
/**
 *  @file     objectid.h
 *
 *  $Id$
 *
 *  ObjectIds redefined for TAO and recognized by CORBA
 *
 *  @author  Chris Cleeland
 *  @author  Douglas C. Schmidt
 */
//=============================================================================

#ifndef TAO_OBJECTID_H
#define TAO_OBJECTID_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/*
 *
 * This file is a break away from orbconf.h. Basically orbconf.h is
 * going the OS.h way. Too many things cluttered in other than the
 * configuration details. Hence this breakway group.
 *
 * This file has the TAO specific objectid definitions for the
 * services that are recognized by resolve_initial_references (),
 * list_initiial_services () etc. They have no place in the ORB
 * configuration.
 */

// ObjectIds recognized by CORBA::ORB::resolve_initial_references()...
// of course, no guarantees are made that the call will return
// something useful.
#define TAO_OBJID_NAMESERVICE         "NameService"
#define TAO_OBJID_TRADINGSERVICE      "TradingService"
#define TAO_OBJID_IMPLREPOSERVICE     "ImplRepoService"
#define TAO_OBJID_ROOTPOA             "RootPOA"
#define TAO_OBJID_POACURRENT          "POACurrent"
#define TAO_OBJID_INTERFACEREP        "InterfaceRepository"
#define TAO_OBJID_POLICYMANAGER       "ORBPolicyManager"
#define TAO_OBJID_POLICYCURRENT       "PolicyCurrent"
#define TAO_OBJID_IORMANIPULATION     "IORManipulation"
#define TAO_OBJID_IORTABLE            "IORTable"
#define TAO_OBJID_ASYNCIORTABLE       "AsyncIORTable"
#define TAO_OBJID_DYNANYFACTORY       "DynAnyFactory"
#define TAO_OBJID_TYPECODEFACTORY     "TypeCodeFactory"
#define TAO_OBJID_RTORB               "RTORB"
#define TAO_OBJID_RTCURRENT               "RTCurrent"
#define TAO_OBJID_PRIORITYMAPPINGMANAGER  "PriorityMappingManager"
#define TAO_OBJID_NETWORKPRIORITYMAPPINGMANAGER  "NetworkPriorityMappingManager"
#define TAO_OBJID_SECURITYCURRENT     "SecurityCurrent"
#define TAO_OBJID_SECURITYMANAGER     "SecurityManager"
#define TAO_OBJID_TRANSACTIONCURRENT  "TransactionCurrent"
#define TAO_OBJID_NOTIFICATIONSERVICE "NotificationService"
#define TAO_OBJID_TYPEDNOTIFICATIONSERVICE "TypedNotificationService"
#define TAO_OBJID_COMPONENTHOMEFINDER "ComponentHomeFinder"
#define TAO_OBJID_PSS                 "PSS"
#define TAO_OBJID_CODECFACTORY        "CodecFactory"
#define TAO_OBJID_PICurrent           "PICurrent"
#define TAO_OBJID_POAMANAGERFACTORY   "POAManagerFactory"
#define TAO_OBJID_COMPRESSIONMANAGER  "CompressionManager"
#define TAO_OBJID_MONITOR             "Monitor"

// Comma separated list of the above ObjectIDs.
// DO NOT include unimplemented services!
// The CORBA::ORB::list_initial_services () method iterates through
// this list to determine which initial services are available.
#define TAO_LIST_OF_INITIAL_SERVICES \
        TAO_OBJID_NAMESERVICE, \
        TAO_OBJID_TRADINGSERVICE, \
        TAO_OBJID_IMPLREPOSERVICE, \
        TAO_OBJID_ROOTPOA, \
        TAO_OBJID_POACURRENT, \
        TAO_OBJID_INTERFACEREP, \
        TAO_OBJID_POLICYMANAGER, \
        TAO_OBJID_POLICYCURRENT, \
        TAO_OBJID_IORMANIPULATION, \
        TAO_OBJID_IORTABLE, \
        TAO_OBJID_DYNANYFACTORY, \
        TAO_OBJID_TYPECODEFACTORY, \
        TAO_OBJID_COMPRESSIONMANAGER, \
        TAO_OBJID_MONITOR
// @@ Some initial references are added via other means, such as
//    ORBInitInfo::register_initial_references().  Those should not be
//    placed in the above list.  Ideally, we should no longer need the
//    above list once the above services register their references
//    dynamically.

/// Service IDs for the services that are located through Multicast.
namespace TAO
{
  enum MCAST_SERVICEID
    {
      MCAST_NAMESERVICE,
      MCAST_TRADINGSERVICE,
      MCAST_IMPLREPOSERVICE,
      MCAST_INTERFACEREPOSERVICE
    };
}

/// No. of services locatable through multicast.
#define TAO_NO_OF_MCAST_SERVICES 4

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_OBJECTID_H*/
