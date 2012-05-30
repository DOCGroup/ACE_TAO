//==============================================================
/**
 *  @file  SubscriberQos_Handler.h
 *
 *  $Id$
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
//================================================================

#ifndef SUBSCRIBER_QOS_HANDLER_H
#define SUBSCRIBER_QOS_HANDLER_H

#include /**/ "ace/pre.h"

#include "dds4ccm/tools/QOS_XML_Handler/XML_QOS_Handler_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dds_qos.hpp"

namespace DDS
{
  struct SubscriberQos;
}

class XML_QOS_Handler_Export SubscriberQos_Handler
{
public:
  static bool get_subscriber_qos (::DDS::SubscriberQos& sub_qos,
                                  ::dds::qosProfile * profile,
                                  const ACE_TCHAR * name = 0);
private:
  static bool get_subscriber_qos (DDS::SubscriberQos& sub_qos,
                                  dds::subscriberQos * sub);
};

#include /**/ "ace/post.h"
#endif /* SUBSCRIBER_QOS_HANDLER_H */
