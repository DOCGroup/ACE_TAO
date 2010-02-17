// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DataReaderHandler_T.h"
#include "dds4ccm/impl/ndds/DDSCallbackStatusHandler.h"
#include "tao/ORB_Core.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::PublisherListener_T (
  ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
  ACE_Reactor* reactor)
      : error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener)),
        reactor_ (reactor)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::PublisherListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::~PublisherListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::~PublisherListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::on_unexpected_status (
  ::DDS::Entity_ptr entity,
  ::DDS::StatusKind status_kind)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_unexpected_status");

  if (!CORBA::is_nil (this->error_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnUnexpectedStatusHandler* rh =
              new ::CIAO::DDS4CCM::OnUnexpectedStatusHandler (
                this->error_listener_, entity, status_kind);
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("PublisherListener_T::failed to use reactor.\n")));
                }
            }
          else
            {
              this->error_listener_->on_unexpected_status (entity, status_kind);
            }
        }
      catch (...)
        {
          CIAO_DEBUG (6, (LM_DEBUG,
              ACE_TEXT ("PublisherListener_T::on_unexpected_status: ")
              ACE_TEXT ("DDS Exception caught\n")));
        }
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("PublisherListener_T::on_offered_deadline_missed: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::on_offered_deadline_missed (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::OfferedDeadlineMissedStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_offered_deadline_missed");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("PublisherListener_T::on_offered_deadline_missed: ")
              ACE_TEXT ("total count <%d> - count change <%d> - ")
              ACE_TEXT ("last instance handle <length <%d> - isValid <%d>\n"),
              status.total_count, status.total_count_change,
              status.last_instance_handle.length,
              status.last_instance_handle.isValid));

  if (!CORBA::is_nil (this->error_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnOfferedDeadlineMissedHandler* rh =
               new ::CIAO::DDS4CCM::OnOfferedDeadlineMissedHandler (
                this->error_listener_, the_Writer, status);
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("PublisherListener_T::failed to use reactor.\n")));
                }
            }
          else
            {
              this->error_listener_->on_offered_deadline_missed (the_Writer, status);
            }
        }
      catch (...)
        {
          CIAO_DEBUG (6, (LM_DEBUG, ACE_TEXT ("PublisherListener_T::on_offered_deadline_missed: ")
                                 ACE_TEXT ("DDS Exception caught\n")));
        }
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("PublisherListener_T::on_offered_deadline_missed: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::on_offered_incompatible_qos (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::OfferedIncompatibleQosStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_offered_incompatible_qos");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("PublisherListener_T::on_offered_incompatible_qos: ")
              ACE_TEXT ("total count <%d> - total change <%d> - ")
              ACE_TEXT ("last policy id <%d> - policies "),
              status.total_count, status.total_count_change,
              status.last_policy_id));
  for (CORBA::ULong i = 0; i < status.policies.length (); ++i)
    {
      CIAO_DEBUG (10, (LM_DEBUG,
                  ACE_TEXT ("\t\tid <%d> - count <%d>\n"),
                  status.policies[i].policy_id,
                  status.policies[i].count));
    }

  if (!CORBA::is_nil (this->error_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnOfferedIncompatibleQoSHandler* rh =
               new ::CIAO::DDS4CCM::OnOfferedIncompatibleQoSHandler (
                this->error_listener_, the_Writer, status);
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("PublisherListener_T::failed to use reactor.\n")));
                }
            }
          else
            {
              this->error_listener_->on_offered_incompatible_qos (the_Writer, status);
            }
        }
      catch (...)
        {
          CIAO_DEBUG (6, (LM_DEBUG, ACE_TEXT ("PublisherListener_T::on_offered_incompatible_qos: ")
                                 ACE_TEXT ("DDS Exception caught\n")));
        }
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("PublisherListener_T::on_offered_incompatible_qos: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_lost (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::LivelinessLostStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_liveliness_lost");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
            ACE_TEXT ("PublisherListener_T::on_liveliness_lost: ")
            ACE_TEXT ("total count <%d> - count change <%d>\n"),
            status.total_count, status.total_count_change));

  this->on_unexpected_status (the_Writer, ::DDS::LIVELINESS_LOST_STATUS);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::on_publication_matched (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::PublicationMatchedStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_publication_matched");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("PublisherListener_T::on_publication_matched: ")
              ACE_TEXT ("total count <%d> - count change <%d> - ")
              ACE_TEXT ("current count <%d> - current count change <%d> ")
              ACE_TEXT ("last publication handle <length <%d> - isValid <%d>\n"),
              status.total_count, status.total_count_change,
              status.current_count, status.current_count_change,
              status.last_subscription_handle.length,
              status.last_subscription_handle.isValid));

  this->on_unexpected_status (the_Writer, ::DDS::PUBLICATION_MATCHED_STATUS);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::on_reliable_reader_activity_changed (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::ReliableReaderActivityChangedStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_reliable_reader_activity_changed");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("PublisherListener_T::on_reliable_reader_activity_changed: ")
              ACE_TEXT ("active count <%d> - active change <%d> - ")
              ACE_TEXT ("not active count <%d> - inactive count change <%d>")
              ACE_TEXT ("last instance handle <length <%d> - isValid <%d>\n"),
              status.active_count, status.active_count_change,
              status.not_active_count, status.inactive_count_change,
              status.last_instance_handle.length,
              status.last_instance_handle.isValid));

  this->on_unexpected_status (the_Writer, ::DDS::RELIABLE_READER_ACTIVITY_CHANGED_STATUS);
}


template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::get_mask (
  ::CCM_DDS::ConnectorStatusListener_ptr error_listener)
{
  if (!CORBA::is_nil (error_listener) || CIAO_debug_level >= 10)
    {
      return ::DDS::OFFERED_DEADLINE_MISSED_STATUS |
             ::DDS::OFFERED_INCOMPATIBLE_QOS_STATUS |
             ::DDS::LIVELINESS_LOST_STATUS |
             ::DDS::PUBLICATION_MATCHED_STATUS |
             ::DDS::RELIABLE_READER_ACTIVITY_CHANGED_STATUS;
    }
  else
    {
      return 0;
    }
}

