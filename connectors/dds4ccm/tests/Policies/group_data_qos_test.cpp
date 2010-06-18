// $Id$

#include "group_data_qos_test.h"

#include "octet_seq.h"

GroupDataPolicyTest::GroupDataPolicyTest ()
{
}

bool
GroupDataPolicyTest::check (const ::DDS_GroupDataQosPolicy & dds_qos,
                            const ::DDS::GroupDataQosPolicy & qos)
{
  return OctetSequence::check (dds_qos.value,
                               qos.value,
                               "GroupDataPolicyTest");
}
