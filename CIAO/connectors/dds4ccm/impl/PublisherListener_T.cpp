// $Id$
#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/DataReaderHandler_T.h"
#include "dds4ccm/impl/DDSCallbackStatusHandler.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "tao/ORB_Core.h"

template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, VENDOR_TYPE>::PublisherListener_T (
  ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
  ACE_Reactor* reactor)
      : error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener)),
        reactor_ (reactor)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PublisherListener_T::PublisherListener_T");
}

template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, VENDOR_TYPE>::~PublisherListener_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PublisherListener_T::~PublisherListener_T");
}

template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, VENDOR_TYPE>::on_unexpected_status (
  ::DDS::Entity_ptr entity,
  ::DDS::StatusKind status_kind)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_unexpected_status");

  if (! ::CORBA::is_nil (this->error_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnUnexpectedStatusHandler* rh = 0;
              ACE_NEW (rh,
                       ::CIAO::DDS4CCM::OnUnexpectedStatusHandler (
                         this->error_listener_, entity, status_kind));
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("PublisherListener_T::on_unexpected_status: ")
                        ACE_TEXT ("failed to use reactor.\n")));
                }
            }
          else
            {
              this->error_listener_->on_unexpected_status (entity, status_kind);
            }
        }
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "PublisherListener_T::on_unexpected_status");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "PublisherListener_T::on_unexpected_status - "
            "Unexpected exception caught for <%C>\n",
            translate_statuskind (status_kind)));
        }
    }
  else
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    ACE_TEXT ("PublisherListener_T::on_unexpected_status: ")
                    ACE_TEXT ("No error listener connected for <%C>\n"),
                    translate_statuskind (status_kind)));
    }
}

template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, VENDOR_TYPE>::on_offered_deadline_missed (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::OfferedDeadlineMissedStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_offered_deadline_missed");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                ACE_TEXT ("PublisherListener_T::on_offered_deadline_missed: ")
                ACE_TEXT ("total count <%d> - count change <%d> - ")
                ACE_TEXT ("last instance handle length <%d> - isValid <%d>\n"),
                status.total_count, status.total_count_change,
                status.last_instance_handle.length,
                status.last_instance_handle.isValid));

  if (! ::CORBA::is_nil (this->error_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnOfferedDeadlineMissedHandler* rh = 0;
              ACE_NEW (rh,
                       ::CIAO::DDS4CCM::OnOfferedDeadlineMissedHandler (
                         this->error_listener_, the_Writer, status));
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                                ACE_TEXT ("PublisherListener_T::on_offered_deadline_missed - ")
                                ACE_TEXT ("failed to use reactor.\n")));
                }
            }
          else
            {
              this->error_listener_->on_offered_deadline_missed (the_Writer, status);
            }
        }
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "PublisherListener_T::on_offered_deadline_missed");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "PublisherListener_T::on_offered_deadline_missed - "
            "Unexpected exception caught\n"));
        }
    }
  else
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    ACE_TEXT ("PublisherListener_T::on_offered_deadline_missed: ")
                    ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, VENDOR_TYPE>::on_offered_incompatible_qos (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::OfferedIncompatibleQosStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_offered_incompatible_qos");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                ACE_TEXT ("PublisherListener_T::on_offered_incompatible_qos: ")
                ACE_TEXT ("total count <%d> - total change <%d> - ")
                ACE_TEXT ("last policy id <%d> - policies "),
                status.total_count, status.total_count_change,
                status.last_policy_id));
  for (CORBA::ULong i = 0; i < status.policies.length (); ++i)
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG,
                  ACE_TEXT ("\t\tid <%d> - count <%d>\n"),
                  status.policies[i].policy_id,
                  status.policies[i].count));
    }

  if (! ::CORBA::is_nil (this->error_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnOfferedIncompatibleQoSHandler* rh = 0;
              ACE_NEW (rh,
                       ::CIAO::DDS4CCM::OnOfferedIncompatibleQoSHandler (
                         this->error_listener_, the_Writer, status));
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                                ACE_TEXT ("PublisherListener_T::on_offered_incompatible_qos - ")
                                ACE_TEXT ("failed to use reactor.\n")));
                }
            }
          else
            {
              this->error_listener_->on_offered_incompatible_qos (the_Writer, status);
            }
        }
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "PublisherListener_T::on_offered_incompatible_qos");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "PublisherListener_T::on_offered_incompatible_qos - "
            "Unexpected exception caught\n"));
        }
    }
  else
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    ACE_TEXT ("PublisherListener_T::on_offered_incompatible_qos - ")
                    ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, VENDOR_TYPE>::on_liveliness_lost (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::LivelinessLostStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_liveliness_lost");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                ACE_TEXT ("PublisherListener_T::on_liveliness_lost: ")
                ACE_TEXT ("total count <%d> - count change <%d>\n"),
                status.total_count, status.total_count_change));

  this->on_unexpected_status (the_Writer, ::DDS::LIVELINESS_LOST_STATUS);
}

template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, VENDOR_TYPE>::on_publication_matched (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::PublicationMatchedStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_publication_matched");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                ACE_TEXT ("PublisherListener_T::on_publication_matched: ")
                ACE_TEXT ("total count <%d> - count change <%d> - ")
                ACE_TEXT ("current count <%d> - current count change <%d> ")
                ACE_TEXT ("last publication handle length <%d> - isValid <%d>\n"),
                status.total_count, status.total_count_change,
                status.current_count, status.current_count_change,
                status.last_subscription_handle.length,
                status.last_subscription_handle.isValid));

  this->on_unexpected_status (the_Writer, ::DDS::PUBLICATION_MATCHED_STATUS);
}

#if (CIAO_DDS4CCM_NDDS==1)
template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, VENDOR_TYPE>::on_reliable_writer_cache_changed (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::ReliableWriterCacheChangedStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_reliable_writer_cache_changed");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                ACE_TEXT ("PublisherListener_T::on_reliable_writer_cache_changed: ")
                ACE_TEXT ("empty_reliable_writer_cache.total_count <%d> - ")
                ACE_TEXT ("empty_reliable_writer_cache.total_count_change <%d> - ")
                ACE_TEXT ("full_reliable_writer_cache.total_count <%d> - ")
                ACE_TEXT ("full_reliable_writer_cache.total_count_change <%d> - ")
                ACE_TEXT ("low_watermark_reliable_writer_cache.total_count <%d> - ")
                ACE_TEXT ("low_watermark_reliable_writer_cache.total_count_change <%d> - ")
                ACE_TEXT ("high_watermark_reliable_writer_cache.total_count <%d> - ")
                ACE_TEXT ("high_watermark_reliable_writer_cache.total_count_change <%d> - ")
                ACE_TEXT ("unacknowledged_sample_count <%d> - ")
                ACE_TEXT ("unacknowledged_sample_count_peak <%d>\n"),
                status.empty_reliable_writer_cache.total_count,
                status.empty_reliable_writer_cache.total_count_change,
                status.full_reliable_writer_cache.total_count,
                status.full_reliable_writer_cache.total_count_change,
                status.low_watermark_reliable_writer_cache.total_count,
                status.low_watermark_reliable_writer_cache.total_count_change,
                status.high_watermark_reliable_writer_cache.total_count,
                status.high_watermark_reliable_writer_cache.total_count_change,
                status.unacknowledged_sample_count,
                status.unacknowledged_sample_count_peak));

  this->on_unexpected_status (the_Writer, ::DDS::RELIABLE_WRITER_CACHE_CHANGED_STATUS);
}

template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, VENDOR_TYPE>::on_reliable_reader_activity_changed (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::ReliableReaderActivityChangedStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_reliable_reader_activity_changed");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                ACE_TEXT ("PublisherListener_T::on_reliable_reader_activity_changed: ")
                ACE_TEXT ("active count <%d> - active change <%d> - ")
                ACE_TEXT ("not active count <%d> - inactive count change <%d>")
                ACE_TEXT ("last instance handle length <%d> - isValid <%d>\n"),
                status.active_count, status.active_count_change,
                status.inactive_count, status.inactive_count_change,
                status.last_instance_handle.length,
                status.last_instance_handle.isValid));

  this->on_unexpected_status (the_Writer, ::DDS::RELIABLE_READER_ACTIVITY_CHANGED_STATUS);
}
#endif

template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, VENDOR_TYPE>::get_mask (
  ::CCM_DDS::ConnectorStatusListener_ptr error_listener)
{
  ::DDS::StatusMask mask = 0;

   if (! ::CORBA::is_nil (error_listener) ||
      DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
    {
      mask = ::DDS::OFFERED_DEADLINE_MISSED_STATUS |
             ::DDS::OFFERED_INCOMPATIBLE_QOS_STATUS |
#if (CIAO_DDS4CCM_NDDS==1)
             ::DDS::RELIABLE_WRITER_CACHE_CHANGED_STATUS |
             ::DDS::RELIABLE_READER_ACTIVITY_CHANGED_STATUS |
#endif
             ::DDS::LIVELINESS_LOST_STATUS |
             ::DDS::PUBLICATION_MATCHED_STATUS;
    }

  if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
    {
      ACE_CString msk = "";
      translate_statusmask (msk, mask);
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                    "PublisherListener_T::get_mask - "
                    "Mask becomes %C\n",
                    msk.c_str ()));
    }
  return mask;
}

