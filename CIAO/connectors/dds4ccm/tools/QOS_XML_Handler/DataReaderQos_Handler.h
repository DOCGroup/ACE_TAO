//==============================================================
/**
 *  @file  DataReaderQos_Handler.h
 *
 *  $Id$
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
//================================================================

#ifndef DATAREADER_QOS_HANDLER_H
#define DATAREADER_QOS_HANDLER_H

#include /**/ "ace/pre.h"

#include "dds4ccm/tools/QOS_XML_Handler/XML_QOS_Handler_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dds_qos.hpp"

namespace DDS
{
  struct DataReaderQos;
}

class XML_QOS_Handler_Export DataReaderQos_Handler
{
public:
  static bool get_datareader_qos (::DDS::DataReaderQos& dr_qos,
                                  ::dds::qosProfile * profile,
                                  const ACE_TCHAR * name = 0);
private:
  static bool get_datareader_qos (DDS::DataReaderQos& dr_qos,
                                  dds::datareaderQos * dr);
};

#include /**/ "ace/post.h"
#endif /* DATAREADER_QOS_HANDLER_H */
