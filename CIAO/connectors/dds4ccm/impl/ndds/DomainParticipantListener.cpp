// $Id$

#include "DomainParticipantListener.h"
#include "DomainParticipant.h"
#include "SampleLostStatus.h"
#include "SubscriptionMatchedStatus.h"
#include "RequestedDeadlineMissedStatus.h"
#include "SampleRejectedStatus.h"
#include "LivelinessChangedStatus.h"
#include "RequestedIncompatibleQosStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_DomainParticipantListener_i::RTI_DomainParticipantListener_i (
        ::DDS::DomainParticipantListener_ptr p)
          : impl_ (::DDS::DomainParticipantListener::_duplicate (p))
      {
      }

      RTI_DomainParticipantListener_i::~RTI_DomainParticipantListener_i (void)
      {
      }

      void
      RTI_DomainParticipantListener_i::on_inconsistent_topic (
          ::DDS::Topic_ptr,
          const ::DDS::InconsistentTopicStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_requested_deadline_missed (
          ::DDS::DataReader_ptr ,
          const ::DDS::RequestedDeadlineMissedStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_requested_incompatible_qos (
          ::DDS::DataReader_ptr ,
          const ::DDS::RequestedIncompatibleQosStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_sample_rejected (
          ::DDS::DataReader_ptr ,
          const ::DDS::SampleRejectedStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_liveliness_changed (
          ::DDS::DataReader_ptr ,
          const ::DDS::LivelinessChangedStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_data_available (
          ::DDS::DataReader_ptr )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_subscription_matched (
          ::DDS::DataReader_ptr ,
          const ::DDS::SubscriptionMatchedStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_sample_lost (
          ::DDS::DataReader_ptr ,
          const ::DDS::SampleLostStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_offered_deadline_missed (
          ::DDS::DataWriter_ptr ,
          const ::DDS::OfferedDeadlineMissedStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_offered_incompatible_qos (
          ::DDS::DataWriter_ptr ,
          const ::DDS::OfferedIncompatibleQosStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_liveliness_lost (
          ::DDS::DataWriter_ptr ,
          const ::DDS::LivelinessLostStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_publication_matched (
          ::DDS::DataWriter_ptr ,
          const ::DDS::PublicationMatchedStatus & )
      {
      }

      void
      RTI_DomainParticipantListener_i::on_data_on_readers (
          ::DDS::Subscriber_ptr )
      {
      }

      ::DDS::DomainParticipantListener_ptr
      RTI_DomainParticipantListener_i::get_domainparticipantlistener (void)
      {
        return ::DDS::DomainParticipantListener::_duplicate (this->impl_.in ());
      }
    }
  }
}

