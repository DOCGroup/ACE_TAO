// $Id$

#include "SubscriberListener.h"
#include "Subscriber.h"
#include "DataReader.h"

#include "ndds/SampleLostStatus.h"
#include "ndds/SubscriptionMatchedStatus.h"
#include "ndds/RequestedDeadlineMissedStatus.h"
#include "ndds/SampleRejectedStatus.h"
#include "ndds/LivelinessChangedStatus.h"
#include "ndds/RequestedIncompatibleQosStatus.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_SubscriberListener_i::CCM_DDS_SubscriberListener_i (::DDS::SubscriberListener_ptr p)
      : impl_ ( ::DDS::SubscriberListener::_duplicate (p))
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_i::CCM_DDS_SubscriberListener_i");
    }

    CCM_DDS_SubscriberListener_i::~CCM_DDS_SubscriberListener_i (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_i::~CCM_DDS_SubscriberListener_i");
    }

    void
    CCM_DDS_SubscriberListener_i::on_data_on_readers (
        ::DDSSubscriber* the_subscriber)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_i::on_data_on_readers");

      ::DDS::Subscriber_var sub = ::DDS::Subscriber::_nil ();
      ACE_NEW (sub,
               CCM_DDS_Subscriber_i (the_subscriber));
      this->impl_->on_data_on_readers (sub.in ());
      the_subscriber->notify_datareaders ();
    }

    void
    CCM_DDS_SubscriberListener_i::on_requested_deadline_missed (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_i::on_requested_deadline_missed");

      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
      ACE_NEW (dds_reader,
               CCM_DDS_DataReader_i (the_reader));
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::RequestedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_requested_deadline_missed (dds_reader.in (), ddsstatus);
#else
      this->impl_->on_requested_deadline_missed (dds_reader.in (), status);
#endif
    }

    void
    CCM_DDS_SubscriberListener_i::on_requested_incompatible_qos (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_i::on_requested_incompatible_qos");

      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
      ACE_NEW (dds_reader,
               CCM_DDS_DataReader_i (the_reader));
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::RequestedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_requested_incompatible_qos (dds_reader.in (), ddsstatus);
#else
      this->impl_->on_requested_incompatible_qos (dds_reader.in (), status);
#endif
    }

    void
    CCM_DDS_SubscriberListener_i::on_sample_rejected (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleRejectedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_i::on_sample_rejected");

      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
      ACE_NEW (dds_reader,
               CCM_DDS_DataReader_i (the_reader));
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::SampleRejectedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_sample_rejected (dds_reader.in (), ddsstatus);
#else
      this->impl_->on_sample_rejected (dds_reader.in (), status);
#endif
    }

    void
    CCM_DDS_SubscriberListener_i::on_liveliness_changed (
      ::DDSDataReader* the_reader,
      const ::DDS_LivelinessChangedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_i::on_liveliness_changed");

      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
      ACE_NEW (dds_reader,
               CCM_DDS_DataReader_i (the_reader));
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::LivelinessChangedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_liveliness_changed (dds_reader.in (), ddsstatus);
#else
      this->impl_->on_liveliness_changed (dds_reader.in (), status);
#endif
    }

    void
    CCM_DDS_SubscriberListener_i::on_data_available (
      ::DDSDataReader* the_reader)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_i::on_data_available");

      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
      ACE_NEW (dds_reader,
               CCM_DDS_DataReader_i (the_reader));
      this->impl_->on_data_available (dds_reader.in ());
    }

    void
    CCM_DDS_SubscriberListener_i::on_subscription_matched (
      ::DDSDataReader* the_reader,
      const ::DDS_SubscriptionMatchedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_i::on_subscription_matched");

      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
      ACE_NEW (dds_reader,
               CCM_DDS_DataReader_i (the_reader));
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::SubscriptionMatchedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_subscription_matched (dds_reader.in (), ddsstatus);
#else
      this->impl_->on_subscription_matched (dds_reader.in (), status);
#endif
    }

    void
    CCM_DDS_SubscriberListener_i::on_sample_lost (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_i::on_sample_lost");

      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
      ACE_NEW (dds_reader,
               CCM_DDS_DataReader_i (the_reader));
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::SampleLostStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_sample_lost (dds_reader.in (), ddsstatus);
#else
      this->impl_->on_sample_lost (dds_reader.in (), status);
#endif
    }

    ::DDS::SubscriberListener_ptr
    CCM_DDS_SubscriberListener_i::get_subscriber_listener (void)
    {
      return ::DDS::SubscriberListener::_duplicate (this->impl_.in ());
    }
  }
}


