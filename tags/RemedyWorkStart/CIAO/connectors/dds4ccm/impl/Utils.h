/**
 * @file Utils.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Misc utility classes for DDS4CCM.
 */

#ifndef DDS4CCM_UTILS_H
#define DDS4CCM_UTILS_H

#if (CIAO_DDS4CCM_NDDS==1)
# include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#else
# include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#endif

#include "dds4ccm/impl/dds4ccm_conf.h"
#include "ace/String_Base.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    inline const char * translate_retcode (::DDS::ReturnCode_t ret)
    {
#define RTI_DDS_RETCODE(X) case X: return #X
      switch (ret)
        {
          RTI_DDS_RETCODE (DDS::RETCODE_OK);
          RTI_DDS_RETCODE (DDS::RETCODE_ERROR);
          RTI_DDS_RETCODE (DDS::RETCODE_UNSUPPORTED);
          RTI_DDS_RETCODE (DDS::RETCODE_BAD_PARAMETER);
          RTI_DDS_RETCODE (DDS::RETCODE_PRECONDITION_NOT_MET);
          RTI_DDS_RETCODE (DDS::RETCODE_OUT_OF_RESOURCES);
          RTI_DDS_RETCODE (DDS::RETCODE_NOT_ENABLED);
          RTI_DDS_RETCODE (DDS::RETCODE_IMMUTABLE_POLICY);
          RTI_DDS_RETCODE (DDS::RETCODE_INCONSISTENT_POLICY);
          RTI_DDS_RETCODE (DDS::RETCODE_ALREADY_DELETED);
          RTI_DDS_RETCODE (DDS::RETCODE_TIMEOUT);
          RTI_DDS_RETCODE (DDS::RETCODE_NO_DATA);
          RTI_DDS_RETCODE (DDS::RETCODE_ILLEGAL_OPERATION);
        }
      return "***Unknown enum value, update RTI::translate_retcode()";
#undef RTI_DDS_RETCODE
    }

    inline const char * translate_statuskind (DDS::StatusKind ret)
    {
#define RTI_DDS_RETCODE(X) case X: return #X
      switch (ret)
        {
          RTI_DDS_RETCODE (DDS::INCONSISTENT_TOPIC_STATUS);
          RTI_DDS_RETCODE (DDS::OFFERED_DEADLINE_MISSED_STATUS);
          RTI_DDS_RETCODE (DDS::REQUESTED_DEADLINE_MISSED_STATUS);
          RTI_DDS_RETCODE (DDS::OFFERED_INCOMPATIBLE_QOS_STATUS);
          RTI_DDS_RETCODE (DDS::REQUESTED_INCOMPATIBLE_QOS_STATUS);
          RTI_DDS_RETCODE (DDS::SAMPLE_LOST_STATUS);
          RTI_DDS_RETCODE (DDS::SAMPLE_REJECTED_STATUS);
          RTI_DDS_RETCODE (DDS::DATA_ON_READERS_STATUS);
          RTI_DDS_RETCODE (DDS::DATA_AVAILABLE_STATUS);
          RTI_DDS_RETCODE (DDS::LIVELINESS_LOST_STATUS);
          RTI_DDS_RETCODE (DDS::LIVELINESS_CHANGED_STATUS);
          RTI_DDS_RETCODE (DDS::PUBLICATION_MATCHED_STATUS);
          RTI_DDS_RETCODE (DDS::SUBSCRIPTION_MATCHED_STATUS);
#if (CIAO_DDS4CCM_NDDS==1)
          RTI_DDS_RETCODE (DDS::RELIABLE_WRITER_CACHE_CHANGED_STATUS);
          RTI_DDS_RETCODE (DDS::RELIABLE_READER_ACTIVITY_CHANGED_STATUS);
#endif
          //RTI_DDS_RETCODE (DDS::DATA_WRITER_CACHE_STATUS);
          //RTI_DDS_RETCODE (DDS::DATA_WRITER_PROTOCOL_STATUS);
          //RTI_DDS_RETCODE (DDS::DATA_READER_CACHE_STATUS);
          //RTI_DDS_RETCODE (DDS::DATA_READER_PROTOCOL_STATUS);
        }
      return "***Unknown enum value, update RTI::translate_statuskind()";
#undef RTI_DDS_RETCODE
    }

    inline const char * translate_rejectedstatuskind (DDS::SampleRejectedStatusKind ret)
    {
#define RTI_DDS_RETCODE(X) case X: return #X
      switch (ret)
        {
          RTI_DDS_RETCODE (DDS::NOT_REJECTED);
          RTI_DDS_RETCODE (DDS::REJECTED_BY_INSTANCES_LIMIT);
          RTI_DDS_RETCODE (DDS::REJECTED_BY_SAMPLES_LIMIT);
          RTI_DDS_RETCODE (DDS::REJECTED_BY_SAMPLES_PER_INSTANCE_LIMIT);
        }
      return "***Unknown enum value, update RTI::translate_rejectedstatuskind()";
#undef RTI_DDS_RETCODE
    }

    inline void translate_statusmask (ACE_CString &ret, ::DDS::StatusMask mask)
    {
#define RTI_DDS_CHECK_MASK(X, Y, Z) \
        if (X & Y) { \
          if (Z.length () != 0) \
              Z += " | "; \
          Z += translate_statuskind (Y); \
        }
      RTI_DDS_CHECK_MASK (mask, ::DDS::INCONSISTENT_TOPIC_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::OFFERED_DEADLINE_MISSED_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::REQUESTED_DEADLINE_MISSED_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::OFFERED_INCOMPATIBLE_QOS_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::REQUESTED_INCOMPATIBLE_QOS_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::SAMPLE_LOST_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::SAMPLE_REJECTED_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::DATA_ON_READERS_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::DATA_AVAILABLE_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::LIVELINESS_LOST_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::LIVELINESS_CHANGED_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::PUBLICATION_MATCHED_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::SUBSCRIPTION_MATCHED_STATUS, ret);
#if (CIAO_DDS4CCM_NDDS==1)
      RTI_DDS_CHECK_MASK (mask, ::DDS::RELIABLE_WRITER_CACHE_CHANGED_STATUS, ret);
      RTI_DDS_CHECK_MASK (mask, ::DDS::RELIABLE_READER_ACTIVITY_CHANGED_STATUS, ret);
#endif

#undef RTI_DDS_CHECK_MASK
    }

    inline const char * translate_qospolicyid_t (DDS::QosPolicyId_t id)
    {
#define RTI_DDS_POLICYID_T(X) case X: return #X
      switch (id)
        {
          RTI_DDS_POLICYID_T (DDS::INVALID_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::USERDATA_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::DURABILITY_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::PRESENTATION_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::DEADLINE_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::LATENCYBUDGET_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::OWNERSHIP_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::OWNERSHIPSTRENGTH_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::LIVELINESS_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::TIMEBASEDFILTER_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::PARTITION_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::RELIABILITY_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::DESTINATIONORDER_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::HISTORY_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::RESOURCELIMITS_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::ENTITYFACTORY_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::WRITERDATALIFECYCLE_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::READERDATALIFECYCLE_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::TOPICDATA_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::GROUPDATA_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::TRANSPORTPRIORITY_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::LIFESPAN_QOS_POLICY_ID);
          RTI_DDS_POLICYID_T (DDS::DURABILITYSERVICE_QOS_POLICY_ID);
        }
      return "***Unknown enum value, update RTI::translate_qospolicyid_t()";
  #undef RTI_DDS_POLICYID_T
    }
  }
}

#endif
