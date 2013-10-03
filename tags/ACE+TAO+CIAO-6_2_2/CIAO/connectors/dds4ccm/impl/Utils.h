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
#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

#include "ace/String_Base.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    /**
     * Enum controlling the semantics of the
     * DDS4CCM listeners.
     */
    enum DDS4CCM_LISTENER_READ_TAKE
    {
      /// Listener does a DDS read
      DDS4CCM_READ,
      /// Listener does a DDS take
      DDS4CCM_TAKE
    };

    /// Helper method translating a DDS ReturnCode_t into
    /// a readable string
    inline const char * translate_retcode (::DDS::ReturnCode_t ret)
    {
#define DDS4CCM_RETCODE(X) case X: return #X
      switch (ret)
        {
          DDS4CCM_RETCODE (DDS::RETCODE_OK);
          DDS4CCM_RETCODE (DDS::RETCODE_ERROR);
          DDS4CCM_RETCODE (DDS::RETCODE_UNSUPPORTED);
          DDS4CCM_RETCODE (DDS::RETCODE_BAD_PARAMETER);
          DDS4CCM_RETCODE (DDS::RETCODE_PRECONDITION_NOT_MET);
          DDS4CCM_RETCODE (DDS::RETCODE_OUT_OF_RESOURCES);
          DDS4CCM_RETCODE (DDS::RETCODE_NOT_ENABLED);
          DDS4CCM_RETCODE (DDS::RETCODE_IMMUTABLE_POLICY);
          DDS4CCM_RETCODE (DDS::RETCODE_INCONSISTENT_POLICY);
          DDS4CCM_RETCODE (DDS::RETCODE_ALREADY_DELETED);
          DDS4CCM_RETCODE (DDS::RETCODE_TIMEOUT);
          DDS4CCM_RETCODE (DDS::RETCODE_NO_DATA);
          DDS4CCM_RETCODE (DDS::RETCODE_ILLEGAL_OPERATION);
        }
      return "***Unknown enum value, update CIAO::DDS4CCM::translate_retcode()";
#undef DDS4CCM_RETCODE
    }

    inline const char * translate_statuskind (DDS::StatusKind ret)
    {
#define DDS4CCM_RETCODE(X) case X: return #X
      switch (ret)
        {
          DDS4CCM_RETCODE (DDS::INCONSISTENT_TOPIC_STATUS);
          DDS4CCM_RETCODE (DDS::OFFERED_DEADLINE_MISSED_STATUS);
          DDS4CCM_RETCODE (DDS::REQUESTED_DEADLINE_MISSED_STATUS);
          DDS4CCM_RETCODE (DDS::OFFERED_INCOMPATIBLE_QOS_STATUS);
          DDS4CCM_RETCODE (DDS::REQUESTED_INCOMPATIBLE_QOS_STATUS);
          DDS4CCM_RETCODE (DDS::SAMPLE_LOST_STATUS);
          DDS4CCM_RETCODE (DDS::SAMPLE_REJECTED_STATUS);
          DDS4CCM_RETCODE (DDS::DATA_ON_READERS_STATUS);
          DDS4CCM_RETCODE (DDS::DATA_AVAILABLE_STATUS);
          DDS4CCM_RETCODE (DDS::LIVELINESS_LOST_STATUS);
          DDS4CCM_RETCODE (DDS::LIVELINESS_CHANGED_STATUS);
          DDS4CCM_RETCODE (DDS::PUBLICATION_MATCHED_STATUS);
          DDS4CCM_RETCODE (DDS::SUBSCRIPTION_MATCHED_STATUS);
#if (CIAO_DDS4CCM_NDDS==1)
          DDS4CCM_RETCODE (DDS::RELIABLE_WRITER_CACHE_CHANGED_STATUS);
          DDS4CCM_RETCODE (DDS::RELIABLE_READER_ACTIVITY_CHANGED_STATUS);
#endif
          //DDS4CCM_RETCODE (DDS::DATA_WRITER_CACHE_STATUS);
          //DDS4CCM_RETCODE (DDS::DATA_WRITER_PROTOCOL_STATUS);
          //DDS4CCM_RETCODE (DDS::DATA_READER_CACHE_STATUS);
          //DDS4CCM_RETCODE (DDS::DATA_READER_PROTOCOL_STATUS);
        }
      return "***Unknown enum value, update CIAO::DDS4CCM::translate_statuskind()";
#undef DDS4CCM_RETCODE
    }

    inline const char * translate_rejectedstatuskind (DDS::SampleRejectedStatusKind ret)
    {
#define DDS4CCM_RETCODE(X) case X: return #X
      switch (ret)
        {
          DDS4CCM_RETCODE (DDS::NOT_REJECTED);
          DDS4CCM_RETCODE (DDS::REJECTED_BY_INSTANCES_LIMIT);
          DDS4CCM_RETCODE (DDS::REJECTED_BY_SAMPLES_LIMIT);
          DDS4CCM_RETCODE (DDS::REJECTED_BY_SAMPLES_PER_INSTANCE_LIMIT);
#if (CIAO_DDS4CCM_NDDS==1)
          DDS4CCM_RETCODE (DDS::REJECTED_BY_REMOTE_WRITERS_LIMIT);
          DDS4CCM_RETCODE (DDS::REJECTED_BY_REMOTE_WRITERS_PER_INSTANCE_LIMIT);
          DDS4CCM_RETCODE (DDS::REJECTED_BY_SAMPLES_PER_REMOTE_WRITER_LIMIT);
          DDS4CCM_RETCODE (DDS::REJECTED_BY_VIRTUAL_WRITERS_LIMIT); /* RTI DDS 5.0.0 Extension */
          DDS4CCM_RETCODE (DDS::REJECTED_BY_REMOTE_WRITERS_PER_SAMPLE_LIMIT); /* RTI DDS 5.0.0 Extension */
          DDS4CCM_RETCODE (DDS::REJECTED_BY_REMOTE_WRITER_SAMPLES_PER_VIRTUAL_QUEUE_LIMIT); /* RTI DDS 5.0.0 Extension */
#endif
        }
      return "***Unknown enum value, update CIAO::DDS4CCM::translate_rejectedstatuskind()";
#undef DDS4CCM_RETCODE
    }

    inline void translate_statusmask (ACE_CString &ret, ::DDS::StatusMask mask)
    {
#define DDS4CCM_CHECK_MASK(X, Y, Z) \
        if (X & Y) { \
          if (Z.length () != 0) \
              Z += " | "; \
          Z += translate_statuskind (Y); \
        }
      DDS4CCM_CHECK_MASK (mask, ::DDS::INCONSISTENT_TOPIC_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::OFFERED_DEADLINE_MISSED_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::REQUESTED_DEADLINE_MISSED_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::OFFERED_INCOMPATIBLE_QOS_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::REQUESTED_INCOMPATIBLE_QOS_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::SAMPLE_LOST_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::SAMPLE_REJECTED_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::DATA_ON_READERS_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::DATA_AVAILABLE_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::LIVELINESS_LOST_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::LIVELINESS_CHANGED_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::PUBLICATION_MATCHED_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::SUBSCRIPTION_MATCHED_STATUS, ret);
#if (CIAO_DDS4CCM_NDDS==1)
      DDS4CCM_CHECK_MASK (mask, ::DDS::RELIABLE_WRITER_CACHE_CHANGED_STATUS, ret);
      DDS4CCM_CHECK_MASK (mask, ::DDS::RELIABLE_READER_ACTIVITY_CHANGED_STATUS, ret);
#endif

#undef DDS4CCM_CHECK_MASK
    }

    inline const char * translate_qospolicyid_t (DDS::QosPolicyId_t id)
    {
#define DDS4CCM_DDS_POLICYID_T(X) case X: return #X
      switch (id)
        {
          DDS4CCM_DDS_POLICYID_T (DDS::INVALID_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::USERDATA_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::DURABILITY_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::PRESENTATION_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::DEADLINE_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::LATENCYBUDGET_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::OWNERSHIP_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::OWNERSHIPSTRENGTH_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::LIVELINESS_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::TIMEBASEDFILTER_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::PARTITION_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::RELIABILITY_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::DESTINATIONORDER_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::HISTORY_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::RESOURCELIMITS_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::ENTITYFACTORY_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::WRITERDATALIFECYCLE_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::READERDATALIFECYCLE_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::TOPICDATA_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::GROUPDATA_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::TRANSPORTPRIORITY_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::LIFESPAN_QOS_POLICY_ID);
          DDS4CCM_DDS_POLICYID_T (DDS::DURABILITYSERVICE_QOS_POLICY_ID);
        }
      return "***Unknown enum value, update CIAO::DDS4CCM::translate_qospolicyid_t()";
  #undef DDS4CCM_DDS_POLICYID_T
    }

    class DDS4CCM_DDS_IMPL_Export QOS_XML_Noop
    {
    public:
      QOS_XML_Noop (void);
      ~QOS_XML_Noop (void);

      DDS::ReturnCode_t
      init (const ACE_TCHAR * qos_profile);

      DDS::ReturnCode_t
      get_datawriter_qos (DDS::DataWriterQos& dw_qos,
                          const char *qos_profile,
                          const char* topic_name);

      DDS::ReturnCode_t
      get_datareader_qos (DDS::DataReaderQos& dr_qos,
                          const char *qos_profile,
                          const char* topic_name);

      DDS::ReturnCode_t
      get_publisher_qos (DDS::PublisherQos& pub_qos,
                          const char *qos_profile);

      DDS::ReturnCode_t
      get_subscriber_qos (DDS::SubscriberQos& sub_qos,
                          const char *qos_profile);

      DDS::ReturnCode_t
      get_topic_qos (DDS::TopicQos& topic_qos,
                    const char *qos_profile,
                    const ACE_TCHAR *topic_name);

      DDS::ReturnCode_t
      get_participant_qos (DDS::DomainParticipantQos& part_qos,
                          const char *qos_profile);
    };
  }
}

#endif
