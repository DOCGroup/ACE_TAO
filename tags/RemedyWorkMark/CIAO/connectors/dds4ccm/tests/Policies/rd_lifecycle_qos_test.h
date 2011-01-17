//$Id$

#ifndef READER_DATA_LIFECYCLE_QOS_TEST_H_
#define READER_DATA_LIFECYCLE_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/convertors/ReaderDataLifecycleQosPolicy.h"

class ReaderDataLifecyclePolicyTest
{
public:
  ReaderDataLifecyclePolicyTest ();

  static bool check (const ::DDS_ReaderDataLifecycleQosPolicy & dds_qos,
                     const ::DDS::ReaderDataLifecycleQosPolicy & qos);
};

#endif /* READER_DATA_LIFECYCLE_QOS_TEST_H_ */
