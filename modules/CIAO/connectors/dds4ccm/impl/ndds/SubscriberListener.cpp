// $Id$

#include "SubscriberListener.h"
#include "Subscriber.h"
#include "SubscriptionMatchedStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_SubscriberListener_i::RTI_SubscriberListener_i (DDSSubscriberListener *s)
        : sub_list_ (s)
      {
      }

      // Implementation skeleton destructor
      RTI_SubscriberListener_i::~RTI_SubscriberListener_i (void)
      {
      }

      void
      RTI_SubscriberListener_i::on_data_on_readers (
          ::DDS::Subscriber_ptr the_subscriber)
      {
        RTI_Subscriber_i* rti_sub = dynamic_cast <RTI_Subscriber_i*>(the_subscriber);
        if (rti_sub)
          {
            this->sub_list_->on_data_on_readers (rti_sub->get_subscriber ());
          }
      }

      void
      RTI_SubscriberListener_i::on_requested_deadline_missed (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::RequestedDeadlineMissedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      void
      RTI_SubscriberListener_i::on_requested_incompatible_qos (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::RequestedIncompatibleQosStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      void
      RTI_SubscriberListener_i::on_sample_rejected (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::SampleRejectedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      void
      RTI_SubscriberListener_i::on_liveliness_changed (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::LivelinessChangedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      void
      RTI_SubscriberListener_i::on_data_available (
        ::DDS::DataReader_ptr the_reader)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      void
      RTI_SubscriberListener_i::on_subscription_matched (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::SubscriptionMatchedStatus & status)
      {
        ::DDS_SubscriptionMatchedStatus ddsstatus;
        status >>= ddsstatus;
        //return this->dr_->get_subscription_matched_status (ddsstatus);
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_SubscriberListener_i::on_sample_lost (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::SampleLostStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      DDSSubscriberListener *
      RTI_SubscriberListener_i::get_subscriber_listener (void)
      {
        return this->sub_list_;
      }
    }
  }
}

