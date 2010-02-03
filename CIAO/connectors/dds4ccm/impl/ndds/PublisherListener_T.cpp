// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DataReaderHandler_T.h"
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

  try
    {
      if (this->reactor_)
        {
          ::CIAO::DDS4CCM::On_Unexpected_Status_Handler* rh =
           new ::CIAO::DDS4CCM::On_Unexpected_Status_Handler (
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

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::on_offered_deadline_missed (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::OfferedDeadlineMissedStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_offered_deadline_missed");

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
  const ::DDS::LivelinessLostStatus &)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_liveliness_lost");

  if (!CORBA::is_nil (this->error_listener_))
    {
      this->on_unexpected_status (the_Writer, ::DDS::LIVELINESS_LOST_STATUS);
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("PublisherListener_T::on_liveliness_lost: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::on_publication_matched (
  ::DDS::DataWriter_ptr the_Writer,
  const ::DDS::PublicationMatchedStatus &)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PublisherListener_T::on_publication_matched");

  if (!CORBA::is_nil (this->error_listener_))
    {
      this->on_unexpected_status (the_Writer, ::DDS::PUBLICATION_MATCHED_STATUS);
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("PublisherListener_T::on_publication_matched: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  return DDS_OFFERED_DEADLINE_MISSED_STATUS |
         DDS_OFFERED_INCOMPATIBLE_QOS_STATUS |
         DDS_LIVELINESS_LOST_STATUS |
         DDS_PUBLICATION_MATCHED_STATUS;
}

