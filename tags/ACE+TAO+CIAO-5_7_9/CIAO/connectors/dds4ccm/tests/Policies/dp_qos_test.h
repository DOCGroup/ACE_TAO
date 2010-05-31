//$Id$

#ifndef DOMAINPARTICIPANT_QOS_TEST_H_
#define DOMAINPARTICIPANT_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/DomainParticipantQos.h"

class DomainParticipantPolicyTest
{
public:
  DomainParticipantPolicyTest ();

  int run ();
};

#endif /* DOMAINPARTICIPANT_QOS_TEST_H_ */
