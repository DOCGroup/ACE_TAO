// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataWriter.h"
#include "ciao/Logger/Log_Macros.h"

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::DataWriterListener_T (
      typename CCM_TYPE::context_type::_ptr_type context,
      ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enabled)
      : context_ (CCM_TYPE::context_type::_duplicate (context)),
        enable_ (enabled)
{
  CIAO_TRACE ("CIAO::DDS4CCM::DataWriterListener_T::DataWriterListener_T");
  this->info_out_connector_status_ = this->context_->get_connection_error_listener ();
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::~DataWriterListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::DataWriterListener_T::~DataWriterListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_offered_deadline_missed (
                                              ::DDS::DataWriter_ptr the_Writer,
                                               const ::DDS::OfferedDeadlineMissedStatus & status)
{
  this->info_out_connector_status_->on_offered_deadline_missed (the_Writer, status);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_offered_incompatible_qos (
                                  ::DDS::DataWriter_ptr the_Writer,
                                  const ::DDS::OfferedIncompatibleQosStatus & status)
{
  this->info_out_connector_status_->on_offered_incompatible_qos (the_Writer, status);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_lost (
                                ::DDS::DataWriter_ptr the_Writer,
                                const ::DDS::LivelinessLostStatus &)
{
  this->info_out_connector_status_->on_unexpected_status (the_Writer, ::DDS::LIVELINESS_LOST_STATUS);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_publication_matched (
                                ::DDS::DataWriter_ptr the_Writer,
                                const ::DDS::PublicationMatchedStatus &)
{
  this->info_out_connector_status_->on_unexpected_status (the_Writer, ::DDS::PUBLICATION_MATCHED_STATUS);
}

