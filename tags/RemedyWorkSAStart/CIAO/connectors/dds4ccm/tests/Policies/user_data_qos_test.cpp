// $Id$

#include "user_data_qos_test.h"

#include "octet_seq.h"

UserDataPolicyTest::UserDataPolicyTest ()
{
}

bool
UserDataPolicyTest::check (const ::DDS_UserDataQosPolicy & dds_qos,
                           const ::DDS::UserDataQosPolicy & qos)
{
  return OctetSequence::check (dds_qos.value,
                               qos.value,
                               "UserDataPolicyTest");
}
