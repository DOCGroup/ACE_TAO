//==============================================================
/**
 *  @file  TopicQos_Handler.h
 *
 *  $Id$
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
//================================================================

#ifndef TOPIC_QOS_HANDLER_H
#define TOPIC_QOS_HANDLER_H

#include /**/ "ace/pre.h"

#include "dds4ccm/tools/QOS_XML_Handler/XML_QOS_Handler_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dds_qos.hpp"

namespace DDS
{
  struct TopicQos;
}

class XML_QOS_Handler_Export TopicQos_Handler
{
public:
  static bool get_topic_qos (::DDS::TopicQos& tp_qos,
                                  ::dds::qosProfile * profile,
                                  const ACE_TCHAR * name = 0);
private:
  static bool get_topic_qos (DDS::TopicQos& tp_qos,
                                  dds::topicQos * tp);
};

#include /**/ "ace/post.h"
#endif /* TOPIC_QOS_HANDLER_H */
