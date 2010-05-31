//$Id$

#ifndef PRESENTATION_QOS_TEST_H_
#define PRESENTATION_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/PresentationQosPolicy.h"

class PresentationPolicyTest
{
public:
  PresentationPolicyTest ();

  static bool check (const ::DDS_PresentationQosPolicy & dds_qos,
                     const ::DDS::PresentationQosPolicy & qos);
};

#endif /* PRESENTATION_QOS_TEST_H_ */
