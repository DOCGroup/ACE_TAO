// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DataReaderHandler_T.h"
#include "tao/ORB_Core.h"

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::DomainParticipantListener_T (
      ::CCM_DDS::ConnectorStatusListener_ptr error_listener)
      : error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener))
{
  CIAO_TRACE ("CIAO::DDS4CCM::DomainParticipantListener_T::DomainParticipantListener_T");
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::~DomainParticipantListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::DomainParticipantListener_T::~DomainParticipantListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::DomainParticipantListener_T::get_mask");
  return DDS_STATUS_MASK_NONE;
}

