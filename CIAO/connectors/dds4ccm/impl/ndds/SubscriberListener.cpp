// $Id$

#include "dds4ccm/impl/ndds/SubscriberListener.h"
#include "dds4ccm/impl/ndds/Subscriber.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/TypeSupport.h"

#include "dds4ccm/impl/ndds/convertors/SampleLostStatus.h"
#include "dds4ccm/impl/ndds/convertors/SubscriptionMatchedStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedDeadlineMissedStatus.h"
#include "dds4ccm/impl/ndds/convertors/SampleRejectedStatus.h"
#include "dds4ccm/impl/ndds/convertors/LivelinessChangedStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedIncompatibleQosStatus.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_SubscriberListener_i::DDS_SubscriberListener_i (
      ::DDS::SubscriberListener_ptr p,
      ::DDS::DomainParticipant_ptr dp)
      : impl_ ( ::DDS::SubscriberListener::_duplicate (p)),
        dp_ (::DDS::DomainParticipant::_duplicate (dp))
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_i::DDS_SubscriberListener_i");
    }

    DDS_SubscriberListener_i::~DDS_SubscriberListener_i (void)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_i::~DDS_SubscriberListener_i");
    }

    void
    DDS_SubscriberListener_i::on_data_on_readers (
        ::DDSSubscriber* the_subscriber)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_i::on_data_on_readers");

      ::DDS::Subscriber_var sub;
      ACE_NEW (sub,
               DDS_Subscriber_i (the_subscriber, dp_.in ()));
      this->impl_->on_data_on_readers (sub.in ());
      the_subscriber->notify_datareaders ();
    }

    void
    DDS_SubscriberListener_i::on_requested_deadline_missed (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_i::on_requested_deadline_missed");

      ::DDS::RequestedDeadlineMissedStatus ddsstatus;
      ::DDS::DataReader_var proxy = DDS_TypeSupport_i::create_datareader (the_reader, this->dp_.in ());
      ddsstatus <<= status;
      this->impl_->on_requested_deadline_missed (proxy.in (), ddsstatus);
    }

    void
    DDS_SubscriberListener_i::on_requested_incompatible_qos (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_i::on_requested_incompatible_qos");

      ::DDS::RequestedIncompatibleQosStatus ddsstatus;
      ::DDS::DataReader_var proxy = DDS_TypeSupport_i::create_datareader (the_reader, this->dp_.in ());
      ddsstatus <<= status;
      this->impl_->on_requested_incompatible_qos (proxy.in (), ddsstatus);
    }

    void
    DDS_SubscriberListener_i::on_sample_rejected (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleRejectedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_i::on_sample_rejected");

      ::DDS::SampleRejectedStatus ddsstatus;
      ::DDS::DataReader_var proxy = DDS_TypeSupport_i::create_datareader (the_reader, this->dp_.in ());
      ddsstatus <<= status;
      this->impl_->on_sample_rejected (proxy.in (), ddsstatus);
    }

    void
    DDS_SubscriberListener_i::on_liveliness_changed (
      ::DDSDataReader* the_reader,
      const ::DDS_LivelinessChangedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_i::on_liveliness_changed");

      ::DDS::LivelinessChangedStatus ddsstatus;
      ::DDS::DataReader_var proxy = DDS_TypeSupport_i::create_datareader (the_reader, this->dp_.in ());
      ddsstatus <<= status;
      this->impl_->on_liveliness_changed (proxy.in (), ddsstatus);
    }

    void
    DDS_SubscriberListener_i::on_data_available (
      ::DDSDataReader* the_reader)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_i::on_data_available");

      ::DDS::DataReader_var proxy = DDS_TypeSupport_i::create_datareader (the_reader, this->dp_.in ());
      this->impl_->on_data_available (proxy.in ());
    }

    void
    DDS_SubscriberListener_i::on_subscription_matched (
      ::DDSDataReader* the_reader,
      const ::DDS_SubscriptionMatchedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_i::on_subscription_matched");

      ::DDS::SubscriptionMatchedStatus ddsstatus;
      ::DDS::DataReader_var proxy = DDS_TypeSupport_i::create_datareader (the_reader, this->dp_.in ());
      ddsstatus <<= status;
      this->impl_->on_subscription_matched (proxy.in (), ddsstatus);
    }

    void
    DDS_SubscriberListener_i::on_sample_lost (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_i::on_sample_lost");

      ::DDS::SampleLostStatus ddsstatus;
      ::DDS::DataReader_var proxy = DDS_TypeSupport_i::create_datareader (the_reader, this->dp_.in ());
      ddsstatus <<= status;
      this->impl_->on_sample_lost (proxy.in (), ddsstatus);
    }

    ::DDS::SubscriberListener_ptr
    DDS_SubscriberListener_i::get_subscriber_listener (void)
    {
      return ::DDS::SubscriberListener::_duplicate (this->impl_.in ());
    }
  }
}


