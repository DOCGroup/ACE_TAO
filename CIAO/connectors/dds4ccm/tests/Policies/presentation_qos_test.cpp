// $Id$

#include "presentation_qos_test.h"

PresentationPolicyTest::PresentationPolicyTest ()
{
}

bool
PresentationPolicyTest::check (const ::DDS_PresentationQosPolicy & dds_qos,
                               const ::DDS::PresentationQosPolicy & qos)
{
  return dds_qos.access_scope ==
              static_cast <DDS_PresentationQosPolicyAccessScopeKind>(qos.access_scope) &&
         dds_qos.coherent_access == qos.coherent_access &&
         dds_qos.ordered_access == qos.ordered_access;
}
