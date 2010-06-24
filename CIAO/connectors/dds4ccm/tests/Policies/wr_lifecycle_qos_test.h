//$Id$

#ifndef WRITER_DATA_LIFECYCLE_QOS_TEST_H_
#define WRITER_DATA_LIFECYCLE_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/WriterDataLifecycleQosPolicy.h"

class WriterDataLifecyclePolicyTest
{
public:
  WriterDataLifecyclePolicyTest ();

  static bool check (const ::DDS_WriterDataLifecycleQosPolicy & dds_qos,
                     const ::DDS::WriterDataLifecycleQosPolicy & qos);
};

#endif /* WRITER_DATA_LIFECYCLE_QOS_TEST_H_ */
