//==============================================================
/**
 *  @file  QOS_Common.h
 *
 *  $Id$
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
//================================================================

#ifndef QOS_COMMON_H
#define QOS_COMMON_H

#include /**/ "ace/pre.h"

#include "dds4ccm/tools/QOS_XML_Handler/XML_QOS_Handler_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dds/DdsDcpsInfrastructureC.h"
#include "dds_qos.hpp"

class XML_QOS_Handler_Export QosCommon
{
public:
  static void get_durability_kind (const ::dds::durabilityKind kind,
                                   ::DDS::DurabilityQosPolicyKind& dds_kind);
  static void get_history_kind (const ::dds::historyKind kind,
                                ::DDS::HistoryQosPolicyKind& dds_kind);
  static void get_duration (::DDS::Duration_t& duration, const ACE_TCHAR * sec, const ACE_TCHAR * nsec);
  static ::CORBA::Long get_qos_long (const ACE_TCHAR * value);
  static void get_liveliness_kind (const ::dds::livelinessKind kind,
                                   ::DDS::LivelinessQosPolicyKind& dds_kind);
  static void get_realiability_kind (const dds::reliabilityKind kind,
                                     ::DDS::ReliabilityQosPolicyKind& dds_kind);
  static void get_destination_order_kind (const ::dds::destinationOrderKind kind,
                                          ::DDS::DestinationOrderQosPolicyKind& dds_kind);

  static void get_ownership_kind (::dds::ownershipKind kind,
                                  ::DDS::OwnershipQosPolicyKind& dds_kind);
};

#include /**/ "ace/post.h"
#endif /* QOS_COMMON_H */
