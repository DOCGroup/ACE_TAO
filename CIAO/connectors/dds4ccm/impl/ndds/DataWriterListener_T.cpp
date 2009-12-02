// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataWriter.h"
#include "ciao/Logger/Log_Macros.h"

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::DataWriterListener_T (
      typename CCM_TYPE::context_type::_ptr_type context,
      ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::ListenerMode> &mode,
      ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::DataNumber_t> &max_delivered_data)
      : context_ (CCM_TYPE::context_type::_duplicate (context)),
        info_out_connector_status_ (CCM_DDS::ConnectorStatusListener::_duplicate (context->get_connection_error_listener ())),
        mode_ (mode),
        max_delivered_data_ (max_delivered_data)
{
  CIAO_TRACE ("CIAO::DDS4CCM::DataWriterListener_T::DataWriterListener_T");
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
  try
    {
      if (!CORBA::is_nil (this->info_out_connector_status_))
        {
          this->info_out_connector_status_->on_offered_deadline_missed (the_Writer, status);
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataWriterListener_T::on_offered_deadline_missed: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_offered_incompatible_qos (
                                  ::DDS::DataWriter_ptr the_Writer,
                                  const ::DDS::OfferedIncompatibleQosStatus & status)
{
  try
    {
      if (!CORBA::is_nil (this->info_out_connector_status_))
        {
          this->info_out_connector_status_->on_offered_incompatible_qos (the_Writer, status);
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataWriterListener_T::on_offered_incompatible_qos: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_lost (
                                ::DDS::DataWriter_ptr the_Writer,
                                const ::DDS::LivelinessLostStatus &)
{
  try
    {
      if (!CORBA::is_nil (this->info_out_connector_status_))
        {
          this->info_out_connector_status_->on_unexpected_status (the_Writer, ::DDS::LIVELINESS_LOST_STATUS);
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataWriterListener_T::on_liveliness_lost: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_publication_matched (
                                ::DDS::DataWriter_ptr the_Writer,
                                const ::DDS::PublicationMatchedStatus &)
{
  try
    {
      if (!CORBA::is_nil (this->info_out_connector_status_))
        {
          this->info_out_connector_status_->on_unexpected_status (the_Writer, ::DDS::PUBLICATION_MATCHED_STATUS);
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataWriterListener_T::on_publication_matched: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  return DDS_OFFERED_DEADLINE_MISSED_STATUS | DDS_OFFERED_INCOMPATIBLE_QOS_STATUS | DDS_LIVELINESS_LOST_STATUS | DDS_PUBLICATION_MATCHED_STATUS;
}

