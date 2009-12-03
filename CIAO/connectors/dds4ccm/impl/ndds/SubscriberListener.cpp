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
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_SubscriberListener_i::RTI_SubscriberListener_i (::DDS::SubscriberListener_ptr p)
        : impl_ ( ::DDS::SubscriberListener::_duplicate (p))
      {
        CIAO_TRACE ("RTI_SubscriberListener_i::RTI_SubscriberListener_i");
      }

      // Implementation skeleton destructor
      RTI_SubscriberListener_i::~RTI_SubscriberListener_i (void)
      {
        CIAO_TRACE ("RTI_SubscriberListener_i::~RTI_SubscriberListener_i");
      }

      void
      RTI_SubscriberListener_i::on_data_on_readers (
          ::DDSSubscriber* the_subscriber)
      {
        CIAO_TRACE ("RTI_SubscriberListener_i::on_data_on_readers");

        ::DDS::Subscriber_var sub = new RTI_Subscriber_i (the_subscriber);
        this->impl_->on_data_on_readers (sub.in ());
        the_subscriber->notify_datareaders ();
      }

      void
      RTI_SubscriberListener_i::on_requested_deadline_missed (
        ::DDSDataReader* the_reader,
        const ::DDS_RequestedDeadlineMissedStatus & status)
      {
        CIAO_TRACE ("RTI_SubscriberListener_i::on_requested_deadline_missed");

        ::DDS::RequestedDeadlineMissedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_requested_deadline_missed (dds_reader.in (), ddsstatus);
      }

      void
      RTI_SubscriberListener_i::on_requested_incompatible_qos (
        ::DDSDataReader* the_reader,
        const ::DDS_RequestedIncompatibleQosStatus & status)
      {
        CIAO_TRACE ("RTI_SubscriberListener_i::on_requested_incompatible_qos");

        ::DDS::RequestedIncompatibleQosStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_requested_incompatible_qos (dds_reader.in (), ddsstatus);
      }

      void
      RTI_SubscriberListener_i::on_sample_rejected (
        ::DDSDataReader* the_reader,
        const ::DDS_SampleRejectedStatus & status)
      {
        CIAO_TRACE ("RTI_SubscriberListener_i::on_sample_rejected");

        ::DDS::SampleRejectedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_sample_rejected (dds_reader.in (), ddsstatus);
      }

      void
      RTI_SubscriberListener_i::on_liveliness_changed (
        ::DDSDataReader* the_reader,
        const ::DDS_LivelinessChangedStatus & status)
      {
        CIAO_TRACE ("RTI_SubscriberListener_i::on_liveliness_changed");

        ::DDS::LivelinessChangedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_liveliness_changed (dds_reader.in (), ddsstatus);
      }

      void
      RTI_SubscriberListener_i::on_data_available (
        ::DDSDataReader* the_reader)
      {
        CIAO_TRACE ("RTI_SubscriberListener_i::on_data_available");

        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_data_available (dds_reader.in ());
      }

      void
      RTI_SubscriberListener_i::on_subscription_matched (
        ::DDSDataReader* the_reader,
        const ::DDS_SubscriptionMatchedStatus & status)
      {
        CIAO_TRACE ("RTI_SubscriberListener_i::on_subscription_matched");

        ::DDS::SubscriptionMatchedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_subscription_matched (dds_reader.in (), ddsstatus);
      }

      void
      RTI_SubscriberListener_i::on_sample_lost (
        ::DDSDataReader* the_reader,
        const ::DDS_SampleLostStatus & status)
      {
        CIAO_TRACE ("RTI_SubscriberListener_i::on_sample_lost");

        ::DDS::SampleLostStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->impl_->on_sample_lost (dds_reader.in (), ddsstatus);
      }

      ::DDS::SubscriberListener_ptr
      RTI_SubscriberListener_i::get_subscriber_listener (void)
      {
        return ::DDS::SubscriberListener::_duplicate (this->impl_.in ());
      }
    }
  }
}


