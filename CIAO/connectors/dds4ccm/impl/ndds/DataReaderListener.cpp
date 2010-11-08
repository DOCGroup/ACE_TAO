// $Id$

#include "dds4ccm/impl/ndds/DataReaderListener.h"
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
    DDS_DataReaderListener_i::DDS_DataReaderListener_i (
      ::DDS::DataReaderListener_ptr p,
      ::DDS::DataReader_ptr dr)
      : impl_ (::DDS::DataReaderListener::_duplicate (p)),
        dr_ (::DDS::DataReader::_duplicate (dr))
    {
    }

    DDS_DataReaderListener_i::~DDS_DataReaderListener_i (void)
    {
    }

    void
    DDS_DataReaderListener_i::on_requested_deadline_missed (
      ::DDSDataReader* ,
      const ::DDS_RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_i::on_requested_deadline_missed");
      ::DDS::RequestedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_requested_deadline_missed (this->dr_, ddsstatus);
    }

    void
    DDS_DataReaderListener_i::on_requested_incompatible_qos (
      ::DDSDataReader* ,
      const ::DDS_RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_i::on_requested_incompatible_qos");
      ::DDS::RequestedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_requested_incompatible_qos (this->dr_, ddsstatus);
    }

    void
    DDS_DataReaderListener_i::on_sample_rejected (
      ::DDSDataReader* ,
      const ::DDS_SampleRejectedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_i::on_sample_rejected");
      ::DDS::SampleRejectedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_sample_rejected (this->dr_, ddsstatus);
    }

    void
    DDS_DataReaderListener_i::on_liveliness_changed (
      ::DDSDataReader* ,
      const ::DDS_LivelinessChangedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_i::on_liveliness_changed");
      ::DDS::LivelinessChangedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_liveliness_changed (this->dr_, ddsstatus);
    }

    void
    DDS_DataReaderListener_i::on_data_available(::DDSDataReader *)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_i::on_data_available");
      this->impl_->on_data_available (this->dr_);
    }

    void
    DDS_DataReaderListener_i::on_subscription_matched (
      ::DDSDataReader* ,
      const ::DDS_SubscriptionMatchedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_i::on_subscription_matched");
      ::DDS::SubscriptionMatchedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_subscription_matched (this->dr_, ddsstatus);
    }

    void
    DDS_DataReaderListener_i::on_sample_lost (
      ::DDSDataReader* ,
      const ::DDS_SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_i::on_sample_lost");
      ::DDS::SampleLostStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_sample_lost (this->dr_, ddsstatus);
    }

    ::DDS::DataReaderListener_ptr
    DDS_DataReaderListener_i::get_datareaderlistener (void)
    {
      return ::DDS::DataReaderListener::_duplicate (this->impl_.in ());
    }

    void
    DDS_DataReaderListener_i::set_dds_entity (::DDS::DataReader_ptr dr)
    {
      this->dr_ = ::DDS::DataReader::_duplicate (dr);
    }
  }
}

