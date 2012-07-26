/**
 * @author Marcel Smit (msmit@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef SAMPLE_INFO_H
#define SAMPLE_INFO_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"


inline void
operator<<= (::CCM_DDS::ReadInfo& ccm_dds_readinfo, const ::DDS::SampleInfo& sample_info)
{
  ccm_dds_readinfo.source_timestamp = sample_info.source_timestamp;
  if (sample_info.sample_state == ::DDS::NOT_READ_SAMPLE_STATE)
    {
      ccm_dds_readinfo.access_status = ::CCM_DDS::FRESH_INFO;
    }
  else if (sample_info.sample_state == ::DDS::READ_SAMPLE_STATE)
    {
      ccm_dds_readinfo.access_status = ::CCM_DDS::ALREADY_SEEN;
    }

  ccm_dds_readinfo.instance_handle = sample_info.instance_handle;

  if (sample_info.instance_state == ::DDS::ALIVE_INSTANCE_STATE &&
      sample_info.view_state == ::DDS::NEW_VIEW_STATE)
    {
      // Instance_status new
      ccm_dds_readinfo.instance_status = ::CCM_DDS::INSTANCE_CREATED;
    }
  else if (sample_info.instance_state == ::DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE)
    {
      // Instance_status deleted
      ccm_dds_readinfo.instance_status = ::CCM_DDS::INSTANCE_DELETED;
    }
  else if ((sample_info.instance_state == ::DDS::ALIVE_INSTANCE_STATE &&
            sample_info.view_state == ::DDS::NOT_NEW_VIEW_STATE) ||
            sample_info.instance_state == ::DDS::NOT_ALIVE_NO_WRITERS_INSTANCE_STATE)
    {
      // Instance_status updated
      ccm_dds_readinfo.instance_status = ::CCM_DDS::INSTANCE_UPDATED;
    }
}

#endif
