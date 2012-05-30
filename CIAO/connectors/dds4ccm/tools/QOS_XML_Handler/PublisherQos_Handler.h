//==============================================================
/**
 *  @file  PublisherQos_Handler.h
 *
 *  $Id$
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
//================================================================

#ifndef PUBLISHER_QOS_HANDLER_H
#define PUBLISHER_QOS_HANDLER_H

#include /**/ "ace/pre.h"

#include "dds4ccm/tools/QOS_XML_Handler/XML_QOS_Handler_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dds_qos.hpp"

namespace DDS
{
  struct PublisherQos;
}

class XML_QOS_Handler_Export PublisherQos_Handler
{
public:
  static bool get_publisher_qos (::DDS::PublisherQos& pub_qos,
                                  ::dds::qosProfile * profile,
                                  const ACE_TCHAR * name = 0);
private:
  static bool get_publisher_qos (DDS::PublisherQos& pub_qos,
                                 dds::publisherQos * pub);
};

#include /**/ "ace/post.h"
#endif /* PUBLISHER_QOS_HANDLER_H */
