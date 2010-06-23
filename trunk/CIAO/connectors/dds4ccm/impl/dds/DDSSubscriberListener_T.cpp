// $Id$

#include "DDSSubscriberListener_T.h"
#include "Subscriber_T.h"

#include "ndds/SampleLostStatus.h"
#include "ndds/SubscriptionMatchedStatus.h"
#include "ndds/RequestedDeadlineMissedStatus.h"
#include "ndds/SampleRejectedStatus.h"
#include "ndds/LivelinessChangedStatus.h"
#include "ndds/RequestedIncompatibleQosStatus.h"

#include "dds4ccm/impl/dds/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::CCM_DDS_SubscriberListener_T (::DDS::SubscriberListener_ptr p)
      : impl_ ( ::DDS::SubscriberListener::_duplicate (p))
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::CCM_DDS_SubscriberListener_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::~CCM_DDS_SubscriberListener_T (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::~CCM_DDS_SubscriberListener_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_data_on_readers (
        ::DDSSubscriber* the_subscriber)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_data_on_readers");

      ::DDS::Subscriber_var sub = ::DDS::Subscriber::_nil ();
      ACE_NEW (sub,
               Subscriber_type (the_subscriber));
      this->impl_->on_data_on_readers (sub.in ());
      the_subscriber->notify_datareaders ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::CCM_DataReader_ptr
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::get_datareader_proxy (
      ::DDSDataReader * the_reader)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::get_datareader_proxy");
      ::DDS::CCM_DataReader_var dds_reader = ::DDS::CCM_DataReader::_nil ();
      ACE_NEW_NORETURN (dds_reader,
                        DataReader_type (the_reader));
      return dds_reader._retn ();
/*
      // Retrieve the pointer to the proxy from the QoS
      ::DDS_DataReaderQos qos;
      the_reader->get_qos (qos);
      DDS_Property_t * prop =
        DDSPropertyQosPolicyHelper::lookup_property (qos.property,
                                                    "CCM_DataReaderProxy");
      if (!prop)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::get_datareader_proxy -"
                                              "Unable to retrieve proxy from PropertyQosProfile\n"));
          return ::DDS::CCM_DataReader::_nil ();
        }

      ::DDS::CCM_DataReader_ptr reader =
        reinterpret_cast < ::DDS::CCM_DataReader_ptr >
          (ACE_OS::atol (prop->value));
          
      if (::CORBA::is_nil (reader))
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::get_datareader_proxy -"
                                              "Unable to retrieve reader from PropertyQosProfile\n"));
        }

      return reader;
*/
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_requested_deadline_missed (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_requested_deadline_missed");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::RequestedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_requested_deadline_missed (
        this->get_datareader_proxy (the_reader),
        ddsstatus);
#else
      this->impl_->on_requested_deadline_missed (
        this->get_datareader_proxy (the_reader),
        status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_requested_incompatible_qos (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_requested_incompatible_qos");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::RequestedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_requested_incompatible_qos (
        this->get_datareader_proxy (the_reader),
        ddsstatus);
#else
      this->impl_->on_requested_incompatible_qos (
        this->get_datareader_proxy (the_reader),
        status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_sample_rejected (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleRejectedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_sample_rejected");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::SampleRejectedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_sample_rejected (
        this->get_datareader_proxy (the_reader),
        ddsstatus);
#else
      this->impl_->on_sample_rejected (
        this->get_datareader_proxy (the_reader),
        status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_changed (
      ::DDSDataReader* the_reader,
      const ::DDS_LivelinessChangedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_changed");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::LivelinessChangedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_liveliness_changed (
        this->get_datareader_proxy (the_reader),
        ddsstatus);
#else
      this->impl_->on_liveliness_changed (
        this->get_datareader_proxy (the_reader),
        status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_data_available (
      ::DDSDataReader* the_reader)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_data_available");

      this->impl_->on_data_available (
        this->get_datareader_proxy (the_reader));
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_subscription_matched (
      ::DDSDataReader* the_reader,
      const ::DDS_SubscriptionMatchedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_subscription_matched");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::SubscriptionMatchedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_subscription_matched (
        this->get_datareader_proxy (the_reader),
        ddsstatus);
#else
      this->impl_->on_subscription_matched (
        this->get_datareader_proxy (the_reader),
        status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_sample_lost (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::on_sample_lost");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::SampleLostStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_sample_lost (
        this->get_datareader_proxy (the_reader),
        ddsstatus);
#else
      this->impl_->on_sample_lost (
        this->get_datareader_proxy (the_reader),
        status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::SubscriberListener_ptr
    CCM_DDS_SubscriberListener_T<DDS_TYPE, CCM_TYPE>::get_subscriber_listener (void)
    {
      return ::DDS::SubscriberListener::_duplicate (this->impl_.in ());
    }
  }
}


