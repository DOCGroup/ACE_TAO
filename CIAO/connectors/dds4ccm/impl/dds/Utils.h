/**
 * @file Utils.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Misc utility classes for NDDS.
 */

#ifndef NDDS_UTILS_H
#define NDDS_UTILS_H

#include "dds4ccm_dds_impl_export.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

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
          //RTI_DDS_RETCODE (DDS::RELIABLE_WRITER_CACHE_CHANGED_STATUS);
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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
  }
}

#endif
