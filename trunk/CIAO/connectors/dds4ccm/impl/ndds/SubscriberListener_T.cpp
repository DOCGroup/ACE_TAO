// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/OnUnexpectedStatusHandler.h"
#include "tao/ORB_Core.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::SubscriberListener_T (
  ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
  ACE_Reactor* reactor)
    : error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener)),
      reactor_ (reactor)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::SubscriberListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::~SubscriberListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::~SubscriberListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_requested_incompatible_qos (
  ::DDS::DataReader_ptr the_reader,
  const ::DDS::RequestedIncompatibleQosStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_requested_incompatible_qos");

  if (!CORBA::is_nil (this->error_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnRequestedOncompatibleQosHandler* rh =
               new ::CIAO::DDS4CCM::OnRequestedOncompatibleQosHandler (
                this->error_listener_, the_reader, status);
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("SubscriberListener_T::failed to use reactor.\n")));
                }
            }
          else
            {
              this->error_listener_->on_requested_incompatible_qos (the_reader, status);
            }
        }
      catch (...)
        {
          CIAO_DEBUG (6, (LM_DEBUG, ACE_TEXT ("SubscriberListener_T::on_requested_incompatible_qos: ")
                                 ACE_TEXT ("DDS Exception caught\n")));
        }
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("SubscriberListener_T::on_requested_incompatible_qos: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_changed(
  ::DDS::DataReader* reader,
  const ::DDS::LivelinessChangedStatus& /*status*/)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_liveliness_changed");

  if (!CORBA::is_nil (this->error_listener_))
    {
      this->on_unexpected_status (reader, ::DDS::LIVELINESS_CHANGED_STATUS);
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("SubscriberListener_T::on_liveliness_changed: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_unexpected_status (
  ::DDS::Entity* entity,
  ::DDS::StatusKind status_kind)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_unexpected_status");

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
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("SubscriberListener_T::failed to use reactor.\n")));
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
          ACE_TEXT ("SubscriberListener_T::on_unexpected_status: ")
          ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_sample_rejected(
  ::DDS::DataReader_ptr reader,
  const ::DDS::SampleRejectedStatus& status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_sample_rejected");

  if (!CORBA::is_nil (this->error_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnSampleRejectedHandler* rh =
               new ::CIAO::DDS4CCM::OnSampleRejectedHandler (
                this->error_listener_, reader, status);
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("SubscriberListener_T::failed to use reactor.\n")));
                }
            }
          else
            {
              this->error_listener_->on_sample_rejected (reader, status);
            }
        }
      catch (...)
        {
          CIAO_DEBUG (6, (LM_DEBUG, ACE_TEXT ("SubscriberListener_T::on_sample_rejected: ")
                                 ACE_TEXT ("DDS Exception caught\n")));
        }
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("SubscriberListener_T::on_sample_rejected: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_subscription_matched(
  ::DDS::DataReader* reader,
  const ::DDS::SubscriptionMatchedStatus& /*status*/)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_subscription_matched");

  if (!CORBA::is_nil (this->error_listener_))
    {
      this->on_unexpected_status (reader, ::DDS::SUBSCRIPTION_MATCHED_STATUS);
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("SubscriberListener_T::on_subscription_matched: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_data_on_readers(
  ::DDS::Subscriber* sub)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_data_on_readers");

  if (!CORBA::is_nil (this->error_listener_))
    {
      this->on_unexpected_status (sub, ::DDS::DATA_ON_READERS_STATUS);
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("SubscriberListener_T::on_data_on_readers: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  return DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS |
         DDS_SAMPLE_REJECTED_STATUS |
         DDS_LIVELINESS_CHANGED_STATUS |
         DDS_SUBSCRIPTION_MATCHED_STATUS |
         DDS_DATA_ON_READERS_STATUS;
}

