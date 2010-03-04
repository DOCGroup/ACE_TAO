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

    void
    CCM_DDS_DataReaderListener_i::on_requested_deadline_missed (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_requested_deadline_missed");
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
    CCM_DDS_DataReaderListener_i::on_requested_incompatible_qos (
      ::DDSDataReader* the_reader,
      const ::DDS_RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_requested_incompatible_qos");
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
    CCM_DDS_DataReaderListener_i::on_sample_rejected (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleRejectedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_sample_rejected");
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
    CCM_DDS_DataReaderListener_i::on_liveliness_changed (
      ::DDSDataReader* the_reader,
      const ::DDS_LivelinessChangedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_liveliness_changed");
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
    CCM_DDS_DataReaderListener_i::on_data_available(::DDSDataReader *the_reader)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_data_available");
      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
      ACE_NEW (dds_reader, 
               CCM_DDS_DataReader_i (the_reader));
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      this->impl_->on_data_available (dds_reader.in ());
#else
      this->impl_->on_data_available (dds_reader.in ());
#endif
    }

    void
    CCM_DDS_DataReaderListener_i::on_subscription_matched (
      ::DDSDataReader* the_reader,
      const ::DDS_SubscriptionMatchedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_subscription_matched");
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
    CCM_DDS_DataReaderListener_i::on_sample_lost (
      ::DDSDataReader* the_reader,
      const ::DDS_SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataReaderListener_i::on_sample_lost");
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

    ::DDS::DataReaderListener_ptr
    CCM_DDS_DataReaderListener_i::get_datareaderlistener (void)
    {
      return ::DDS::DataReaderListener::_duplicate (this->impl_.in ());
    }
  }
}

