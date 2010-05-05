// $Id$

#include "DataReaderListener.h"
#include "DataReader.h"

#include "ndds/SampleLostStatus.h"
#include "ndds/SubscriptionMatchedStatus.h"
#include "ndds/RequestedDeadlineMissedStatus.h"
#include "ndds/SampleRejectedStatus.h"
#include "ndds/LivelinessChangedStatus.h"
#include "ndds/RequestedIncompatibleQosStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_DataReaderListener_i::CCM_DDS_DataReaderListener_i (::DDS::DataReaderListener_ptr p)
      : impl_ (::DDS::DataReaderListener::_duplicate (p))
    {
    }

    CCM_DDS_DataReaderListener_i::~CCM_DDS_DataReaderListener_i (void)
    {
    }

    ::DDS::CCM_DataReader_ptr
    CCM_DDS_DataReaderListener_i::get_datareader_proxy (
      ::DDSDataReader * the_reader)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::get_datareader_proxy");
      // Retrieve the pointer to the proxy from the QoS
      ::DDS_DataReaderQos qos;
      the_reader->get_qos (qos);
      DDS_Property_t * prop =
        DDSPropertyQosPolicyHelper::lookup_property (qos.property,
                                                    "CCM_DataReaderProxy");
      if (!prop)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DataReaderListener_i::get_datareader_proxy -"
                                              "Unable to retrieve proxy from PropertyQosProfile\n"));
          return ::DDS::CCM_DataReader::_nil ();
        }
      ::DDS::CCM_DataReader_ptr reader =
        reinterpret_cast < ::DDS::CCM_DataReader_ptr >
          (ACE_OS::atol (prop->value));
      if (::CORBA::is_nil (reader))
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DataReaderListener_i::get_datareader_proxy -"
                                              "Unable to retrieve reader from PropertyQosProfile\n"));
        }

      return reader;
    }

    void
    CCM_DDS_DataReaderListener_i::on_requested_deadline_missed (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_requested_deadline_missed");
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

    void
    CCM_DDS_DataReaderListener_i::on_requested_incompatible_qos (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_requested_incompatible_qos");
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

    void
    CCM_DDS_DataReaderListener_i::on_sample_rejected (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleRejectedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_sample_rejected");
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

    void
    CCM_DDS_DataReaderListener_i::on_liveliness_changed (
      ::DDSDataReader* the_reader,
      const ::DDS_LivelinessChangedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_liveliness_changed");
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

    void
    CCM_DDS_DataReaderListener_i::on_data_available(::DDSDataReader *the_reader)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_data_available");
#if (CIAO_DDS4CCM_NDDS==1)
      this->impl_->on_data_available (
        this->get_datareader_proxy (the_reader));
#else
      this->impl_->on_data_available (
        this->get_datareader_proxy (the_reader));
#endif
    }

    void
    CCM_DDS_DataReaderListener_i::on_subscription_matched (
      ::DDSDataReader* the_reader,
      const ::DDS_SubscriptionMatchedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_subscription_matched");
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

    void
    CCM_DDS_DataReaderListener_i::on_sample_lost (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_sample_lost");
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

    ::DDS::DataReaderListener_ptr
    CCM_DDS_DataReaderListener_i::get_datareaderlistener (void)
    {
      return ::DDS::DataReaderListener::_duplicate (this->impl_.in ());
    }
  }
}

