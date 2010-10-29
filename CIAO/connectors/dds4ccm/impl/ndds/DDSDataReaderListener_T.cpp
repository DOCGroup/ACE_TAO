// $Id$

#include "DDSDataReaderListener_T.h"
#include "dds4ccm/impl/ndds/DataReader_T.h"

#include "dds4ccm/impl/ndds/convertors/SampleLostStatus.h"
#include "dds4ccm/impl/ndds/convertors/SubscriptionMatchedStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedDeadlineMissedStatus.h"
#include "dds4ccm/impl/ndds/convertors/SampleRejectedStatus.h"
#include "dds4ccm/impl/ndds/convertors/LivelinessChangedStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedIncompatibleQosStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE>
    DDS_DataReaderListener_T<DDS_TYPE>::DDS_DataReaderListener_T (
          ::DDS::DataReaderListener_ptr p,
          DataReader_type *typed_dr)
      : impl_ (::DDS::DataReaderListener::_duplicate (p)),
        typed_dr_ (typed_dr)
    {
    }

    template <typename DDS_TYPE>
    DDS_DataReaderListener_T<DDS_TYPE>::~DDS_DataReaderListener_T (void)
    {
    }

    template <typename DDS_TYPE>
    void
    DDS_DataReaderListener_T<DDS_TYPE>::on_requested_deadline_missed (
      ::DDSDataReader* ,
      const ::DDS_RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_T<DDS_TYPE>::on_requested_deadline_missed");
      ::DDS::RequestedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_requested_deadline_missed (
        this->typed_dr_,
        ddsstatus);
    }

    template <typename DDS_TYPE>
    void
    DDS_DataReaderListener_T<DDS_TYPE>::on_requested_incompatible_qos (
      ::DDSDataReader* ,
      const ::DDS_RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_T<DDS_TYPE>::on_requested_incompatible_qos");
      ::DDS::RequestedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_requested_incompatible_qos (
        this->typed_dr_,
        ddsstatus);
    }

    template <typename DDS_TYPE>
    void
    DDS_DataReaderListener_T<DDS_TYPE>::on_sample_rejected (
      ::DDSDataReader* ,
      const ::DDS_SampleRejectedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_T<DDS_TYPE>::on_sample_rejected");
      ::DDS::SampleRejectedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_sample_rejected (
        this->typed_dr_,
        ddsstatus);
    }

    template <typename DDS_TYPE>
    void
    DDS_DataReaderListener_T<DDS_TYPE>::on_liveliness_changed (
      ::DDSDataReader* ,
      const ::DDS_LivelinessChangedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_T<DDS_TYPE>::on_liveliness_changed");
      ::DDS::LivelinessChangedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_liveliness_changed (
        this->typed_dr_,
        ddsstatus);
    }

    template <typename DDS_TYPE>
    void
    DDS_DataReaderListener_T<DDS_TYPE>::on_data_available(::DDSDataReader *)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_T<DDS_TYPE>::on_data_available");
      this->impl_->on_data_available (
        this->typed_dr_);
    }

    template <typename DDS_TYPE>
    void
    DDS_DataReaderListener_T<DDS_TYPE>::on_subscription_matched (
      ::DDSDataReader* ,
      const ::DDS_SubscriptionMatchedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_T<DDS_TYPE>::on_subscription_matched");
      ::DDS::SubscriptionMatchedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_subscription_matched (
        this->typed_dr_,
        ddsstatus);
    }

    template <typename DDS_TYPE>
    void
    DDS_DataReaderListener_T<DDS_TYPE>::on_sample_lost (
      ::DDSDataReader* ,
      const ::DDS_SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataReaderListener_T<DDS_TYPE>::on_sample_lost");
      ::DDS::SampleLostStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_sample_lost (
        this->typed_dr_,
        ddsstatus);
    }

    template <typename DDS_TYPE>
    ::DDS::DataReaderListener_ptr
    DDS_DataReaderListener_T<DDS_TYPE>::get_datareaderlistener (void)
    {
      return ::DDS::DataReaderListener::_duplicate (this->impl_.in ());
    }
  }
}

