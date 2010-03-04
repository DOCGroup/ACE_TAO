// $Id$

#include "wr_lifecycle_qos_test.h"

WriterDataLifecyclePolicyTest::WriterDataLifecyclePolicyTest ()
{
}

bool
WriterDataLifecyclePolicyTest::check (const ::DDS_WriterDataLifecycleQosPolicy & dds_qos,
                                      const ::DDS::WriterDataLifecycleQosPolicy & qos)
{
  return dds_qos.autodispose_unregistered_instances ==
         qos.autodispose_unregistered_instances;
}
