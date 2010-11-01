// $Id$

#include "dds4ccm/impl/DomainParticipantListener_T.h"

#include "dds4ccm/impl/ndds/DomainParticipant_T.h"

#include "dds4ccm/impl/ndds/convertors/SampleLostStatus.h"
#include "dds4ccm/impl/ndds/convertors/SubscriptionMatchedStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedDeadlineMissedStatus.h"
#include "dds4ccm/impl/ndds/convertors/SampleRejectedStatus.h"
#include "dds4ccm/impl/ndds/convertors/LivelinessChangedStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedIncompatibleQosStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
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
        ::DDS::Topic_ptr,
        const ::DDS::InconsistentTopicStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_requested_deadline_missed (
        ::DDS::DataReader_ptr ,
        const ::DDS::RequestedDeadlineMissedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_requested_incompatible_qos (
        ::DDS::DataReader_ptr ,
        const ::DDS::RequestedIncompatibleQosStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_sample_rejected (
        ::DDS::DataReader_ptr ,
        const ::DDS::SampleRejectedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_liveliness_changed (
        ::DDS::DataReader_ptr ,
        const ::DDS::LivelinessChangedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_data_available (
        ::DDS::DataReader_ptr )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_subscription_matched (
        ::DDS::DataReader_ptr ,
        const ::DDS::SubscriptionMatchedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_sample_lost (
        ::DDS::DataReader_ptr ,
        const ::DDS::SampleLostStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_offered_deadline_missed (
        ::DDS::DataWriter_ptr ,
        const ::DDS::OfferedDeadlineMissedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_offered_incompatible_qos (
        ::DDS::DataWriter_ptr ,
        const ::DDS::OfferedIncompatibleQosStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_liveliness_lost (
        ::DDS::DataWriter_ptr ,
        const ::DDS::LivelinessLostStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_publication_matched (
        ::DDS::DataWriter_ptr ,
        const ::DDS::PublicationMatchedStatus & )
    {
    }

    void
    DDS_DomainParticipantListener_i::on_data_on_readers (
        ::DDS::Subscriber_ptr )
    {
    }

    ::DDS::DomainParticipantListener_ptr
    DDS_DomainParticipantListener_i::get_domainparticipantlistener (void)
    {
      return ::DDS::DomainParticipantListener::_duplicate (this->impl_.in ());
    }
  }
}

