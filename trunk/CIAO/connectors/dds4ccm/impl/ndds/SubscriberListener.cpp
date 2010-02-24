// $Id$

#include "SubscriberListener.h"
#include "Subscriber.h"
#include "DataReader.h"
#include "SampleLostStatus.h"
#include "SubscriptionMatchedStatus.h"
#include "RequestedDeadlineMissedStatus.h"
#include "SampleRejectedStatus.h"
#include "LivelinessChangedStatus.h"
#include "RequestedIncompatibleQosStatus.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_SubscriberListener_i::RTI_SubscriberListener_i (::DDS::SubscriberListener_ptr p)
        : impl_ ( ::DDS::SubscriberListener::_duplicate (p))
      {
        DDS4CCM_TRACE ("RTI_SubscriberListener_i::RTI_SubscriberListener_i");
      }

      RTI_SubscriberListener_i::~RTI_SubscriberListener_i (void)
      {
        DDS4CCM_TRACE ("RTI_SubscriberListener_i::~RTI_SubscriberListener_i");
      }

      void
      RTI_SubscriberListener_i::on_data_on_readers (
          ::DDSSubscriber* the_subscriber)
      {
        DDS4CCM_TRACE ("RTI_SubscriberListener_i::on_data_on_readers");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::Subscriber_var sub = ::DDS::Subscriber::_nil ();
        ACE_NEW (sub, RTI_Subscriber_i ());
        RTI_Subscriber_i *s = dynamic_cast < RTI_Subscriber_i *> (sub.in ());
        s->set_impl (the_subscriber);
        this->impl_->on_data_on_readers (sub.in ());
#else
        this->impl_->on_data_on_readers (the_subscriber);
#endif        
        the_subscriber->notify_datareaders ();
      }

      void
      RTI_SubscriberListener_i::on_requested_deadline_missed (
        ::DDSDataReader* the_reader,
        const ::DDS_RequestedDeadlineMissedStatus & status)
      {
        DDS4CCM_TRACE ("RTI_SubscriberListener_i::on_requested_deadline_missed");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::RequestedDeadlineMissedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
        ACE_NEW (dds_reader, RTI_DataReader_i ());
        RTI_DataReader_i *rti_dr = dynamic_cast < RTI_DataReader_i *>(dds_reader.in ());
        rti_dr->set_impl (the_reader);
        this->impl_->on_requested_deadline_missed (dds_reader.in (), ddsstatus);
#else
        this->impl_->on_requested_deadline_missed (the_reader, status);
#endif        
      }

      void
      RTI_SubscriberListener_i::on_requested_incompatible_qos (
        ::DDSDataReader* the_reader,
        const ::DDS_RequestedIncompatibleQosStatus & status)
      {
        DDS4CCM_TRACE ("RTI_SubscriberListener_i::on_requested_incompatible_qos");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::RequestedIncompatibleQosStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
        ACE_NEW (dds_reader, RTI_DataReader_i ());
        RTI_DataReader_i *rti_dr = dynamic_cast < RTI_DataReader_i *>(dds_reader.in ());
        rti_dr->set_impl (the_reader);
        this->impl_->on_requested_incompatible_qos (dds_reader.in (), ddsstatus);
#else
        this->impl_->on_requested_incompatible_qos (the_reader, status);
#endif        
      }

      void
      RTI_SubscriberListener_i::on_sample_rejected (
        ::DDSDataReader* the_reader,
        const ::DDS_SampleRejectedStatus & status)
      {
        DDS4CCM_TRACE ("RTI_SubscriberListener_i::on_sample_rejected");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::SampleRejectedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
        ACE_NEW (dds_reader, RTI_DataReader_i ());
        RTI_DataReader_i *rti_dr = dynamic_cast < RTI_DataReader_i *>(dds_reader.in ());
        rti_dr->set_impl (the_reader);
        this->impl_->on_sample_rejected (dds_reader.in (), ddsstatus);
#else
        this->impl_->on_sample_rejected (the_reader, status);
#endif        
      }

      void
      RTI_SubscriberListener_i::on_liveliness_changed (
        ::DDSDataReader* the_reader,
        const ::DDS_LivelinessChangedStatus & status)
      {
        DDS4CCM_TRACE ("RTI_SubscriberListener_i::on_liveliness_changed");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::LivelinessChangedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
        ACE_NEW (dds_reader, RTI_DataReader_i ());
        RTI_DataReader_i *rti_dr = dynamic_cast < RTI_DataReader_i *>(dds_reader.in ());
        rti_dr->set_impl (the_reader);
        this->impl_->on_liveliness_changed (dds_reader.in (), ddsstatus);
#else
        this->impl_->on_liveliness_changed (the_reader, status);
#endif        
      }

      void
      RTI_SubscriberListener_i::on_data_available (
        ::DDSDataReader* the_reader)
      {
        DDS4CCM_TRACE ("RTI_SubscriberListener_i::on_data_available");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
        ACE_NEW (dds_reader, RTI_DataReader_i ());
        RTI_DataReader_i *rti_dr = dynamic_cast < RTI_DataReader_i *>(dds_reader.in ());
        rti_dr->set_impl (the_reader);
        this->impl_->on_data_available (dds_reader.in ());
#else
        this->impl_->on_data_available (the_reader);
#endif        
      }

      void
      RTI_SubscriberListener_i::on_subscription_matched (
        ::DDSDataReader* the_reader,
        const ::DDS_SubscriptionMatchedStatus & status)
      {
        DDS4CCM_TRACE ("RTI_SubscriberListener_i::on_subscription_matched");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::SubscriptionMatchedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
        ACE_NEW (dds_reader, RTI_DataReader_i ());
        RTI_DataReader_i *rti_dr = dynamic_cast < RTI_DataReader_i *>(dds_reader.in ());
        rti_dr->set_impl (the_reader);
        this->impl_->on_subscription_matched (dds_reader.in (), ddsstatus);
#else
        this->impl_->on_subscription_matched (the_reader, status);
#endif        
      }

      void
      RTI_SubscriberListener_i::on_sample_lost (
        ::DDSDataReader* the_reader,
        const ::DDS_SampleLostStatus & status)
      {
        DDS4CCM_TRACE ("RTI_SubscriberListener_i::on_sample_lost");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::SampleLostStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
        ACE_NEW (dds_reader, RTI_DataReader_i ());
        RTI_DataReader_i *rti_dr = dynamic_cast < RTI_DataReader_i *>(dds_reader.in ());
        rti_dr->set_impl (the_reader);
        this->impl_->on_sample_lost (dds_reader.in (), ddsstatus);
#else
        this->impl_->on_sample_lost (the_reader, status);
#endif        
      }

      ::DDS::SubscriberListener_ptr
      RTI_SubscriberListener_i::get_subscriber_listener (void)
      {
        return ::DDS::SubscriberListener::_duplicate (this->impl_.in ());
      }
    }
  }
}


