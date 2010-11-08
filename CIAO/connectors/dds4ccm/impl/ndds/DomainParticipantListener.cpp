// $Id$

#include "dds4ccm/impl/ndds/DomainParticipantListener.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_DomainParticipantListener_i::DDS_DomainParticipantListener_i (
      ::DDS::DomainParticipantListener_ptr p)
        : impl_ (::DDS::DomainParticipantListener::_duplicate (p))
    {
    }

    DDS_DomainParticipantListener_i::~DDS_DomainParticipantListener_i (void)
    {
    }

    void
    DDS_DomainParticipantListener_i::on_inconsistent_topic (
        ::DDSTopic*,
        const ::DDS_InconsistentTopicStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_requested_deadline_missed (
        ::DDSDataReader* ,
        const ::DDS_RequestedDeadlineMissedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_requested_incompatible_qos (
        ::DDSDataReader* ,
        const ::DDS_RequestedIncompatibleQosStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_sample_rejected (
        ::DDSDataReader* ,
        const ::DDS_SampleRejectedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_liveliness_changed (
        ::DDSDataReader* ,
        const ::DDS_LivelinessChangedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_data_available (
        ::DDSDataReader* )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_subscription_matched (
        ::DDSDataReader* ,
        const ::DDS_SubscriptionMatchedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_sample_lost (
        ::DDSDataReader* ,
        const ::DDS_SampleLostStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_offered_deadline_missed (
        ::DDSDataWriter* ,
        const ::DDS_OfferedDeadlineMissedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_offered_incompatible_qos (
        ::DDSDataWriter* ,
        const ::DDS_OfferedIncompatibleQosStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_liveliness_lost (
        ::DDSDataWriter* ,
        const ::DDS_LivelinessLostStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_publication_matched (
        ::DDSDataWriter* ,
        const ::DDS_PublicationMatchedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_data_on_readers (
        ::DDSSubscriber* )
    {
    }

    ::DDS::DomainParticipantListener_ptr
    DDS_DomainParticipantListener_i::get_domainparticipantlistener (void)
    {
      return ::DDS::DomainParticipantListener::_duplicate (this->impl_.in ());
    }
  }
}

