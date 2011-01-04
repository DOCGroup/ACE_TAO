// $Id$

#ifndef GROUP_DATA_QOS_TEST_H_
#define GROUP_DATA_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/convertors/GroupDataQosPolicy.h"

class GroupDataPolicyTest
{
public:
  GroupDataPolicyTest ();

  static bool check (const ::DDS_GroupDataQosPolicy & dds_qos,
                     const ::DDS::GroupDataQosPolicy & qos);
};

#endif /* GROUP_DATA_QOS_TEST_H_ */
