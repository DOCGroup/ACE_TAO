// $Id$

#include "hist_qos_test.h"

HistoryPolicyTest::HistoryPolicyTest ()
{
}

bool
HistoryPolicyTest::check (const ::DDS_HistoryQosPolicy & dds_qos,
                          const ::DDS::HistoryQosPolicy & qos)
{

  return dds_qos.kind == static_cast <DDS_HistoryQosPolicyKind> (qos.kind) &&
         dds_qos.depth == qos.depth;
}
