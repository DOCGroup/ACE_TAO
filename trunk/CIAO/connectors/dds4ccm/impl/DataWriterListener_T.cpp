// $Id$

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::DataWriterListener_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataWriterListener_T::DataWriterListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::~DataWriterListener_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataWriterListener_T::~DataWriterListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_offered_deadline_missed (
  ::DDS::DataWriter_ptr ,
  const ::DDS::OfferedDeadlineMissedStatus &)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_offered_incompatible_qos (
  ::DDS::DataWriter_ptr ,
  const ::DDS::OfferedIncompatibleQosStatus &)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_lost (
  ::DDS::DataWriter_ptr ,
  const ::DDS::LivelinessLostStatus &)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_publication_matched (
  ::DDS::DataWriter_ptr ,
  const ::DDS::PublicationMatchedStatus &)
{
}

#if (CIAO_DDS4CCM_NDDS==1)
template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_reliable_writer_cache_changed (
  ::DDS::DataWriter_ptr ,
  const ::DDS::ReliableWriterCacheChangedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_reliable_reader_activity_changed (
  ::DDS::DataWriter_ptr ,
  const ::DDS::ReliableReaderActivityChangedStatus &)
{
}
#endif

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataWriterListener_T::get_mask");
  return 0;
}

