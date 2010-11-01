// $Id$

#include "dds4ccm/impl/ndds/DDSSubscriberListener_T.h"
#include "dds4ccm/impl/ndds/Subscriber_T.h"

#include "dds4ccm/impl/ndds/convertors/SampleLostStatus.h"
#include "dds4ccm/impl/ndds/convertors/SubscriptionMatchedStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedDeadlineMissedStatus.h"
#include "dds4ccm/impl/ndds/convertors/SampleRejectedStatus.h"
#include "dds4ccm/impl/ndds/convertors/LivelinessChangedStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedIncompatibleQosStatus.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE>
    DDS_SubscriberListener_T<DDS_TYPE>::DDS_SubscriberListener_T (::DDS::SubscriberListener_ptr p)
      : impl_ ( ::DDS::SubscriberListener::_duplicate (p))
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::DDS_SubscriberListener_T");
    }

    template <typename DDS_TYPE>
    DDS_SubscriberListener_T<DDS_TYPE>::~DDS_SubscriberListener_T (void)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::~DDS_SubscriberListener_T");
    }

    template <typename DDS_TYPE>
    void
    DDS_SubscriberListener_T<DDS_TYPE>::on_data_on_readers (
        ::DDSSubscriber* the_subscriber)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::on_data_on_readers");

      ::DDS::Subscriber_var sub;
      ACE_NEW (sub,
               Subscriber_type (the_subscriber));
      this->impl_->on_data_on_readers (sub.in ());
      the_subscriber->notify_datareaders ();
    }

    template <typename DDS_TYPE>
    ::DDS::DataReader_ptr
    DDS_SubscriberListener_T<DDS_TYPE>::get_datareader_proxy (
      ::DDSDataReader * the_reader)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::get_datareader_proxy");
      ::DDS::DataReader_var dds_reader;
      ACE_NEW_NORETURN (dds_reader,
                        DataReader_type (the_reader));
      return dds_reader._retn ();
    }

    template <typename DDS_TYPE>
    void
    DDS_SubscriberListener_T<DDS_TYPE>::on_requested_deadline_missed (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::on_requested_deadline_missed");

      ::DDS::RequestedDeadlineMissedStatus ddsstatus;
      ::DDS::DataReader_var proxy = this->get_datareader_proxy (the_reader);
      ddsstatus <<= status;
      this->impl_->on_requested_deadline_missed (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE>
    void
    DDS_SubscriberListener_T<DDS_TYPE>::on_requested_incompatible_qos (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::on_requested_incompatible_qos");

      ::DDS::RequestedIncompatibleQosStatus ddsstatus;
      ::DDS::DataReader_var proxy = this->get_datareader_proxy (the_reader);
      ddsstatus <<= status;
      this->impl_->on_requested_incompatible_qos (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE>
    void
    DDS_SubscriberListener_T<DDS_TYPE>::on_sample_rejected (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleRejectedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::on_sample_rejected");

      ::DDS::SampleRejectedStatus ddsstatus;
      ::DDS::DataReader_var proxy = this->get_datareader_proxy (the_reader);
      ddsstatus <<= status;
      this->impl_->on_sample_rejected (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE>
    void
    DDS_SubscriberListener_T<DDS_TYPE>::on_liveliness_changed (
      ::DDSDataReader* the_reader,
      const ::DDS_LivelinessChangedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::on_liveliness_changed");

      ::DDS::LivelinessChangedStatus ddsstatus;
      ::DDS::DataReader_var proxy = this->get_datareader_proxy (the_reader);
      ddsstatus <<= status;
      this->impl_->on_liveliness_changed (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE>
    void
    DDS_SubscriberListener_T<DDS_TYPE>::on_data_available (
      ::DDSDataReader* the_reader)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::on_data_available");

      ::DDS::DataReader_var proxy = this->get_datareader_proxy (the_reader);
      this->impl_->on_data_available (proxy.in ());
    }

    template <typename DDS_TYPE>
    void
    DDS_SubscriberListener_T<DDS_TYPE>::on_subscription_matched (
      ::DDSDataReader* the_reader,
      const ::DDS_SubscriptionMatchedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::on_subscription_matched");

      ::DDS::SubscriptionMatchedStatus ddsstatus;
      ::DDS::DataReader_var proxy = this->get_datareader_proxy (the_reader);
      ddsstatus <<= status;
      this->impl_->on_subscription_matched (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE>
    void
    DDS_SubscriberListener_T<DDS_TYPE>::on_sample_lost (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("DDS_SubscriberListener_T<DDS_TYPE>::on_sample_lost");

      ::DDS::SampleLostStatus ddsstatus;
      ::DDS::DataReader_var proxy = this->get_datareader_proxy (the_reader);
      ddsstatus <<= status;
      this->impl_->on_sample_lost (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE>
    ::DDS::SubscriberListener_ptr
    DDS_SubscriberListener_T<DDS_TYPE>::get_subscriber_listener (void)
    {
      return ::DDS::SubscriberListener::_duplicate (this->impl_.in ());
    }
  }
}


