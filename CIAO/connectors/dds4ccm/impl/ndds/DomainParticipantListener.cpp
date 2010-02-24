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
    CCM_DDS_DomainPublisherListener_i::CCM_DDS_DomainPublisherListener_i (
      ::DDS::DomainParticipantListener_ptr p)
        : impl_ (::DDS::DomainParticipantListener::_duplicate (p))
    {
    }

    CCM_DDS_DomainPublisherListener_i::~CCM_DDS_DomainPublisherListener_i (void)
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_inconsistent_topic (
        ::DDS::Topic_ptr,
        const ::DDS::InconsistentTopicStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_requested_deadline_missed (
        ::DDS::DataReader_ptr ,
        const ::DDS::RequestedDeadlineMissedStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_requested_incompatible_qos (
        ::DDS::DataReader_ptr ,
        const ::DDS::RequestedIncompatibleQosStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_sample_rejected (
        ::DDS::DataReader_ptr ,
        const ::DDS::SampleRejectedStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_liveliness_changed (
        ::DDS::DataReader_ptr ,
        const ::DDS::LivelinessChangedStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_data_available (
        ::DDS::DataReader_ptr )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_subscription_matched (
        ::DDS::DataReader_ptr ,
        const ::DDS::SubscriptionMatchedStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_sample_lost (
        ::DDS::DataReader_ptr ,
        const ::DDS::SampleLostStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_offered_deadline_missed (
        ::DDS::DataWriter_ptr ,
        const ::DDS::OfferedDeadlineMissedStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_offered_incompatible_qos (
        ::DDS::DataWriter_ptr ,
        const ::DDS::OfferedIncompatibleQosStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_liveliness_lost (
        ::DDS::DataWriter_ptr ,
        const ::DDS::LivelinessLostStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_publication_matched (
        ::DDS::DataWriter_ptr ,
        const ::DDS::PublicationMatchedStatus & )
    {
    }

    void
    CCM_DDS_DomainPublisherListener_i::on_data_on_readers (
        ::DDS::Subscriber_ptr )
    {
    }

    ::DDS::DomainParticipantListener_ptr
    CCM_DDS_DomainPublisherListener_i::get_domainparticipantlistener (void)
    {
      return ::DDS::DomainParticipantListener::_duplicate (this->impl_.in ());
    }
  }
}

