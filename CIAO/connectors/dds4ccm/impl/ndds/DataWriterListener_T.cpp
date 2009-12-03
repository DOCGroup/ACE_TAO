// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataWriter.h"
#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::DataWriterListener_T (
      typename CCM_TYPE::context_type::_ptr_type context,
      CCM_DDS::ConnectorStatusListener_ptr error_listener)
      : context_ (CCM_TYPE::context_type::_duplicate (context)),
        error_listener_ (CCM_DDS::ConnectorStatusListener::_duplicate (error_listener))
{
  CIAO_TRACE ("CIAO::DDS4CCM::DataWriterListener_T::DataWriterListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::~DataWriterListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::DataWriterListener_T::~DataWriterListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  return DDS_STATUS_MASK_NONE;
}

