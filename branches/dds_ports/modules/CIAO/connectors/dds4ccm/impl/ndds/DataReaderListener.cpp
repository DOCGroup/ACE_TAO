// $Id$

#include "DataReaderListener.h"
#include "DataReader.h"
#include "SampleLostStatus.h"
#include "SubscriptionMatchedStatus.h"
#include "RequestedDeadlineMissedStatus.h"
#include "SampleRejectedStatus.h"
#include "LivelinessChangedStatus.h"
#include "RequestedIncompatibleQosStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_DataReaderListener_i::RTI_DataReaderListener_i (::DDS::DataReaderListener_ptr p)
        : impl_ (::DDS::DataReaderListener::_duplicate (p))
      {
      }

      // Implementation skeleton destructor
      RTI_DataReaderListener_i::~RTI_DataReaderListener_i (void)
      {
      }

      void
      RTI_DataReaderListener_i::on_requested_deadline_missed (
        ::DDSDataReader* the_reader,
        const ::DDS_RequestedDeadlineMissedStatus & status)
      {
        ::DDS::RequestedDeadlineMissedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_requested_deadline_missed (dds_reader.in (), ddsstatus);
      }

      void
      RTI_DataReaderListener_i::on_requested_incompatible_qos (
        ::DDSDataReader* the_reader,
        const ::DDS_RequestedIncompatibleQosStatus & status)
      {
        ::DDS::RequestedIncompatibleQosStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_requested_incompatible_qos (dds_reader.in (), ddsstatus);
      }

      void
      RTI_DataReaderListener_i::on_sample_rejected (
        ::DDSDataReader* the_reader,
        const ::DDS_SampleRejectedStatus & status)
      {
        ::DDS::SampleRejectedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_sample_rejected (dds_reader.in (), ddsstatus);
      }

      void
      RTI_DataReaderListener_i::on_liveliness_changed (
        ::DDSDataReader* the_reader,
        const ::DDS_LivelinessChangedStatus & status)
      {
        ::DDS::LivelinessChangedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_liveliness_changed (dds_reader.in (), ddsstatus);
      }

      void
      RTI_DataReaderListener_i::on_data_available(::DDSDataReader *reader)
      {
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (reader);
        this->impl_->on_data_available (dds_reader.in ());
      }

      void
      RTI_DataReaderListener_i::on_subscription_matched (
        ::DDSDataReader* the_reader,
        const ::DDS_SubscriptionMatchedStatus & status)
      {
        ::DDS::SubscriptionMatchedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_subscription_matched (dds_reader.in (), ddsstatus);
      }

      void
      RTI_DataReaderListener_i::on_sample_lost (
        ::DDSDataReader* the_reader,
        const ::DDS_SampleLostStatus & status)
      {
        ::DDS::SampleLostStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_sample_lost (dds_reader.in (), ddsstatus);
      }

      ::DDS::DataReaderListener_ptr
      RTI_DataReaderListener_i::get_datareaderlistener (void)
      {
        return ::DDS::DataReaderListener::_duplicate (this->impl_.in ());
      }
    }
  }
}

