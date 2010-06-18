//$Id$

#ifndef USER_DATA_QOS_TEST_H_
#define USER_DATA_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/UserDataQosPolicy.h"

class UserDataPolicyTest
{
public:
  UserDataPolicyTest ();

  static bool check (const ::DDS_UserDataQosPolicy & dds_qos,
                     const ::DDS::UserDataQosPolicy & qos);
};

#endif /* USER_DATA_QOS_TEST_H_ */
