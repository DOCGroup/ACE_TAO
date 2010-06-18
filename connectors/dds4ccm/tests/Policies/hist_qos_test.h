//$Id$

#ifndef HISTORY_QOS_TEST_H_
#define HISTORY_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/HistoryQosPolicy.h"

class HistoryPolicyTest
{
public:
  HistoryPolicyTest ();

  static bool check (const ::DDS_HistoryQosPolicy & dds_qos,
                     const ::DDS::HistoryQosPolicy & qos);
};

#endif /* HISTORY_QOS_TEST_H_ */
