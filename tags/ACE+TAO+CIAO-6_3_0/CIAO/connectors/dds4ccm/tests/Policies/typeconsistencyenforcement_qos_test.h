//$Id$

#ifndef TYPECONSISTENCYENFORCEMENT_QOS_TEST_H_
#define TYPECONSISTENCYENFORCEMENT_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/convertors/TypeConsistencyEnforcementQosPolicy.h"

#if defined (RTI_DDS_VERSION_MAJOR) && (RTI_DDS_VERSION_MAJOR >= 5)

class TypeConsistencyEnforcementPolicyPolicyTest
{
public:
  TypeConsistencyEnforcementPolicyPolicyTest ();

  static bool check (const ::DDS_TypeConsistencyEnforcementQosPolicy & dds_qos,
                     const ::DDS::TypeConsistencyEnforcementQosPolicy & qos);
};

#endif

#endif /* TYPECONSISTENCYENFORCEMENT_QOS_TEST_H_ */
