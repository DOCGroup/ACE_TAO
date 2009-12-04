// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DataReaderHandler_T.h"
#include "tao/ORB_Core.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::SubscriberListener_T (
  typename CCM_TYPE::context_type::_ptr_type context,
  ::CCM_DDS::ConnectorStatusListener_ptr error_listener)
    : context_ (CCM_TYPE::context_type::_duplicate (context)),
      error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener))
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

  try
    {
      if (!CORBA::is_nil (this->error_listener_))
        {
          this->error_listener_->on_requested_incompatible_qos (the_reader, status);
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("SubscriberListener_T::on_requested_incompatible_qos: ")
                      ACE_TEXT ("No error listener installed\n")));
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("SubscriberListener_T::on_requested_incompatible_qos: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_changed(
  ::DDS::DataReader* reader,
  const ::DDS::LivelinessChangedStatus& /*status*/)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_liveliness_changed");

  try
    {
      if (!CORBA::is_nil (this->error_listener_))
        {
          this->error_listener_->on_unexpected_status (reader, ::DDS::LIVELINESS_CHANGED_STATUS);
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("SubscriberListener_T::on_liveliness_changed: ")
                      ACE_TEXT ("No error listener installed\n")));
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("SubscriberListener_T::on_liveliness_changed: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_sample_rejected(
  ::DDS::DataReader* reader,
  const ::DDS::SampleRejectedStatus& status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_sample_rejected");

  try
    {
      if (!CORBA::is_nil (this->error_listener_))
        {
          this->error_listener_->on_sample_rejected (reader, status);
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("SubscriberListener_T::on_sample_rejected: ")
                      ACE_TEXT ("No error listener installed\n")));
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("SubscriberListener_T::on_sample_rejected: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_subscription_matched(
  ::DDS::DataReader* reader,
  const ::DDS::SubscriptionMatchedStatus& /*status*/)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_subscription_matched");

  try
    {
      if (!CORBA::is_nil (this->error_listener_))
        {
          this->error_listener_->on_unexpected_status (reader, ::DDS::SUBSCRIPTION_MATCHED_STATUS);
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("SubscriberListener_T::on_subscription_matched: ")
                      ACE_TEXT ("No error listener installed\n")));
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("SubscriberListener_T::on_subscription_matched: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_data_on_readers(
  ::DDS::Subscriber* sub)
{
  CIAO_TRACE ("CIAO::DDS4CCM::SubscriberListener_T::on_data_on_readers");

  try
    {
      if (!CORBA::is_nil (this->error_listener_))
        {
          this->error_listener_->on_unexpected_status (sub, ::DDS::DATA_ON_READERS_STATUS);
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("SubscriberListener_T::on_data_on_readers: ")
                      ACE_TEXT ("No error listener installed\n")));
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("SubscriberListener_T::on_data_on_readers: ")
                             ACE_TEXT ("DDS Exception caught\n")));
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

