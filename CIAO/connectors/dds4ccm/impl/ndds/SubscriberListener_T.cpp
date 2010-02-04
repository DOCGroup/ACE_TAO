// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/OnUnexpectedStatusHandler.h"
#include "dds4ccm/impl/ndds/Utils.h"
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
  ::DDS::DataReader_ptr reader,
  const ::DDS::RequestedIncompatibleQosStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_requested_incompatible_qos");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("SubscriberListener_T::on_requested_incompatible_qos: ")
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
              ::CIAO::DDS4CCM::OnRequestedOncompatibleQosHandler* rh =
               new ::CIAO::DDS4CCM::OnRequestedOncompatibleQosHandler (
                this->error_listener_, reader, status);
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  ACE_ERROR ((LM_ERROR, CLINFO
                              ACE_TEXT ("SubscriberListener_T::on_requested_incompatible_qos: ")
                              ACE_TEXT ("failed to use reactor.\n")));
                }
            }
          else
            {
              this->error_listener_->on_requested_incompatible_qos (reader, status);
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
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_unexpected_status (
  ::DDS::Entity_ptr entity,
  ::DDS::StatusKind status_kind)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_unexpected_status");

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
                  ACE_ERROR ((LM_ERROR, CLINFO
                              ACE_TEXT ("SubscriberListener_T::on_unexpected_status: ")
                              ACE_TEXT ("failed to use reactor.\n")));
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
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  ACE_TEXT ("SubscriberListener_T::on_requested_incompatible_qos: ")
                  ACE_TEXT ("No error listener connected\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_changed (
  ::DDS::DataReader* reader,
  const ::DDS::LivelinessChangedStatus& status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_liveliness_changed");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("SubscriberListener_T::on_liveliness_changed: ")
              ACE_TEXT ("alive count <%d> - not alive count <%d> - ")
              ACE_TEXT ("alive count change <%d> - not alive count change <%d> "),
              ACE_TEXT ("last publication handle <%C>\n"),
              status.alive_count, status.not_alive_count,
              status.alive_count_change, status.not_alive_count_change,
              translate_instancehandle (status.last_publication_handle)));

  this->on_unexpected_status (reader, ::DDS::LIVELINESS_CHANGED_STATUS);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_sample_rejected (
  ::DDS::DataReader_ptr reader,
  const ::DDS::SampleRejectedStatus& status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_sample_rejected");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("SubscriberListener_T::on_sample_rejected: ")
              ACE_TEXT ("total count <%d> - count change <%d> - ")
              ACE_TEXT ("last reason <%C> - last instance handle <%C>\n"),
              status.total_count, status.total_count_change,
              translate_rejectedstatuskind (status.last_reason),
              translate_instancehandle (status.last_instance_handle)));
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
                  ACE_ERROR ((LM_ERROR, CLINFO
                              ACE_TEXT ("SubscriberListener_T::on_sample_rejected: ")
                              ACE_TEXT ("failed to use reactor.\n")));
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
  const ::DDS::SubscriptionMatchedStatus& status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_subscription_matched");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("SubscriberListener_T::on_subscription_matched: ")
              ACE_TEXT ("total count <%d> - count change <%d> - ")
              ACE_TEXT ("current count <%d> - current count change <%d>"),
              ACE_TEXT ("last publication handle <%C>\n"),
              status.total_count, status.total_count_change,
              status.current_count, status.current_count_change,
              translate_instancehandle (status.last_publication_handle)));

  this->on_unexpected_status (reader, ::DDS::SUBSCRIPTION_MATCHED_STATUS);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_data_on_readers(
  ::DDS::Subscriber* sub)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_data_on_readers");
  this->on_unexpected_status (sub, ::DDS::DATA_ON_READERS_STATUS);
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

