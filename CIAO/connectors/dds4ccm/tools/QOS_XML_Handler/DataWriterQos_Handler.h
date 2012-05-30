//==============================================================
/**
 *  @file  DataWriterQos_Handler.h
 *
 *  $Id$
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
//================================================================

#ifndef DATAWRITER_QOS_HANDLER_H
#define DATAWRITER_QOS_HANDLER_H

#include /**/ "ace/pre.h"

#include "dds4ccm/tools/QOS_XML_Handler/XML_QOS_Handler_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dds_qos.hpp"

namespace DDS
{
  struct DataWriterQos;
}

class XML_QOS_Handler_Export DataWriterQos_Handler
{
public:
  static bool get_datawriter_qos (::DDS::DataWriterQos& dw_qos,
                                  ::dds::qosProfile * profile,
                                  const ACE_TCHAR * name = 0);
private:
  static bool get_datawriter_qos (DDS::DataWriterQos& dw_qos,
                                  dds::datawriterQos * dw);
};

#include /**/ "ace/post.h"
#endif /* DATAWRITER_QOS_HANDLER_H */
