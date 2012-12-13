// $Id$

#include "typeconsistencyenforcement_qos_test.h"

TypeConsistencyEnforcementPolicyPolicyTest::TypeConsistencyEnforcementPolicyPolicyTest ()
{
}

bool
TypeConsistencyEnforcementPolicyPolicyTest::check (const ::DDS_TypeConsistencyEnforcementQosPolicy & dds_qos,
                              const ::DDS::TypeConsistencyEnforcementQosPolicy & qos)
{
#if defined (RTI_DDS_VERSION_MAJOR) && (RTI_DDS_VERSION_MAJOR >= 5)
  return dds_qos.kind == static_cast <DDS_TypeConsistencyKind> (qos.kind);
#else
  return true;
#endif
}
