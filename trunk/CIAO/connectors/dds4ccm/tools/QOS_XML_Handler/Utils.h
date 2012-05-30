/**
 * @file Utils.h
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 */

#ifndef QOS_XML_UTILS_H_
#define QOS_XML_UTILS_H_

#include "ace/SString.h"

#include "XML_QOS_Handler_Export.h"

namespace DDS4CCM
{
  XML_QOS_Handler_Export ACE_CString get_xml_file_name (const char * qos_profile);
  XML_QOS_Handler_Export ACE_CString get_profile_name (const char * qos_profile);
}

#endif /* QOS_XML_UTILS_H_ */
