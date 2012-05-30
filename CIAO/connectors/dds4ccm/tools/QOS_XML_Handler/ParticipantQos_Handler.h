//==============================================================
/**
 *  @file  ParticipantQos_Handler.h
 *
 *  $Id$
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
//================================================================

#ifndef PARTICIPANT_QOS_HANDLER_H
#define PARTICIPANT_QOS_HANDLER_H

#include /**/ "ace/pre.h"

#include "dds4ccm/tools/QOS_XML_Handler/XML_QOS_Handler_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dds_qos.hpp"

namespace DDS
{
  class DomainParticipantQos;
}

class XML_QOS_Handler_Export ParticipantQos_Handler
{
public:
  static bool get_participant_qos (::DDS::DomainParticipantQos& dp_qos,
                                  ::dds::qosProfile * profile,
                                  const ACE_TCHAR * name = 0);
private:
  static bool get_participant_qos (DDS::DomainParticipantQos& dp_qos,
                                  dds::domainparticipantQos * dp);
};

#include /**/ "ace/post.h"
#endif /* PARTICIPANT_QOS_HANDLER_H */
