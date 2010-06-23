// $Id$
#include "dds4ccm/impl/dds/Utils.h"

#include "dds4ccm/impl/dds/Log_Macros.h"
#include "dds4ccm/impl/dds/DataReaderHandler_T.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#include "tao/ORB_Core.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::DomainParticipantListener_T (
      ::CCM_DDS::ConnectorStatusListener_ptr error_listener)
      : error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener))
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DomainParticipantListener_T::DomainParticipantListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::~DomainParticipantListener_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DomainParticipantListener_T::~DomainParticipantListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_inconsistent_topic (
  ::DDS::Topic_ptr,
  const ::DDS::InconsistentTopicStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_requested_deadline_missed (
  ::DDS::DataReader_ptr ,
  const ::DDS::RequestedDeadlineMissedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_requested_incompatible_qos (
  ::DDS::DataReader_ptr ,
  const ::DDS::RequestedIncompatibleQosStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_sample_rejected (
  ::DDS::DataReader_ptr ,
  const ::DDS::SampleRejectedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_changed (
  ::DDS::DataReader_ptr ,
  const ::DDS::LivelinessChangedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_data_available (
  ::DDS::DataReader_ptr )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_subscription_matched (
  ::DDS::DataReader_ptr ,
  const ::DDS::SubscriptionMatchedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_sample_lost (
  ::DDS::DataReader_ptr ,
  const ::DDS::SampleLostStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_offered_deadline_missed (
  ::DDS::DataWriter_ptr ,
  const ::DDS::OfferedDeadlineMissedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_offered_incompatible_qos (
  ::DDS::DataWriter_ptr ,
  const ::DDS::OfferedIncompatibleQosStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_lost (
  ::DDS::DataWriter_ptr ,
  const ::DDS::LivelinessLostStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_publication_matched (
  ::DDS::DataWriter_ptr ,
  const ::DDS::PublicationMatchedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_data_on_readers (
  ::DDS::Subscriber_ptr )
{
}

#if (CIAO_DDS4CCM_NDDS==1)
template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_reliable_writer_cache_changed (
  ::DDS::DataWriter_ptr ,
  const ::DDS::ReliableWriterCacheChangedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::on_reliable_reader_activity_changed (
  ::DDS::DataWriter_ptr ,
  const ::DDS::ReliableReaderActivityChangedStatus & )
{
}
#endif

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DomainParticipantListener_T::get_mask");
  return 0;
}

