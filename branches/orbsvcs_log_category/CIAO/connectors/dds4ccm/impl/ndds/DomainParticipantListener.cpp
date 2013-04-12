// $Id$

#include "dds4ccm/impl/ndds/DomainParticipantListener.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/TypeSupport.h"

#include "dds4ccm/impl/ndds/convertors/InconsistentTopicStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedDeadlineMissedStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedIncompatibleQosStatus.h"
#include "dds4ccm/impl/ndds/convertors/SampleRejectedStatus.h"
#include "dds4ccm/impl/ndds/convertors/LivelinessChangedStatus.h"
#include "dds4ccm/impl/ndds/convertors/SubscriptionMatchedStatus.h"
#include "dds4ccm/impl/ndds/convertors/SampleLostStatus.h"
#include "dds4ccm/impl/ndds/convertors/OfferedIncompatibleQosStatus.h"
#include "dds4ccm/impl/ndds/convertors/OfferedDeadlineMissedStatus.h"
#include "dds4ccm/impl/ndds/convertors/LivelinessLostStatus.h"
#include "dds4ccm/impl/ndds/convertors/PublicationMatchedStatus.h"

#include "dds4ccm/impl/ndds/Topic.h"
#include "dds4ccm/impl/ndds/Subscriber.h"
#include "dds4ccm/impl/ndds/Publisher.h"

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

    ::DDS::DataReader_ptr
    DDS_DomainParticipantListener_i::create_datareader (DDSDataReader * reader)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::create_datareader");

      ::DDS::Subscriber_var sub;
      ACE_NEW_THROW_EX (sub,
                        DDS_Subscriber_i (reader->get_subscriber (),
                                          this->dp_.in ()),
                        ::CORBA::NO_MEMORY ());

      ::DDS::DataReader_var dr = DDS_TypeSupport_i::create_datareader (
                                                                reader,
                                                                this->dp_.in (),
                                                                sub.in ());
      return dr.in ();
    }

    ::DDS::DataWriter_ptr
    DDS_DomainParticipantListener_i::create_datawriter (DDSDataWriter * writer)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::create_datawriter");

      ::DDS::Publisher_var pub;
      ACE_NEW_THROW_EX (pub,
                        DDS_Publisher_i (writer->get_publisher (),
                                         this->dp_.in ()),
                        ::CORBA::NO_MEMORY ());

      ::DDS::DataWriter_var dw = DDS_TypeSupport_i::create_datawriter (
                                                                writer,
                                                                this->dp_.in (),
                                                                pub.in ());
      return dw.in ();
    }

    void
    DDS_DomainParticipantListener_i::on_inconsistent_topic (
        ::DDSTopic * tp,
        const ::DDS_InconsistentTopicStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_inconsistent_topic");

      ::DDS::InconsistentTopicStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::Topic_var topic;
      ACE_NEW_THROW_EX (topic,
                        DDS_Topic_i (tp,
                                     this->dp_.in ()),
                        ::CORBA::NO_MEMORY ());
      this->impl_->on_inconsistent_topic (topic.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_requested_deadline_missed (
        ::DDSDataReader * reader,
        const ::DDS_RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_requested_deadline_missed");

      ::DDS::RequestedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::DataReader_var dr = this->create_datareader (reader);
      this->impl_->on_requested_deadline_missed (dr.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_requested_incompatible_qos (
        ::DDSDataReader * reader,
        const ::DDS_RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_requested_incompatible_qos");

      ::DDS::RequestedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::DataReader_var dr = this->create_datareader (reader);
      this->impl_->on_requested_incompatible_qos (dr.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_sample_rejected (
        ::DDSDataReader * reader,
        const ::DDS_SampleRejectedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_sample_rejected");

      ::DDS::SampleRejectedStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::DataReader_var dr = this->create_datareader (reader);
      this->impl_->on_sample_rejected (dr.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_liveliness_changed (
        ::DDSDataReader * reader,
        const ::DDS_LivelinessChangedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_liveliness_changed");

      ::DDS::LivelinessChangedStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::DataReader_var dr = this->create_datareader (reader);
      this->impl_->on_liveliness_changed (dr.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_data_available (
        ::DDSDataReader* reader)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_data_available");

      ::DDS::DataReader_var dr = this->create_datareader (reader);
      this->impl_->on_data_available (dr.in ());
    }

    void
    DDS_DomainParticipantListener_i::on_subscription_matched (
        ::DDSDataReader * reader,
        const ::DDS_SubscriptionMatchedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_subscription_matched");

      ::DDS::SubscriptionMatchedStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::DataReader_var dr = this->create_datareader (reader);
      this->impl_->on_subscription_matched (dr.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_sample_lost (
        ::DDSDataReader * reader,
        const ::DDS_SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_sample_lost");

      ::DDS::SampleLostStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::DataReader_var dr = this->create_datareader (reader);
      this->impl_->on_sample_lost (dr.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_offered_deadline_missed (
        ::DDSDataWriter * writer,
        const ::DDS_OfferedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_offered_deadline_missed");

      ::DDS::OfferedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::DataWriter_var dw = this->create_datawriter (writer);
      this->impl_->on_offered_deadline_missed (dw.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_offered_incompatible_qos (
        ::DDSDataWriter * writer,
        const ::DDS_OfferedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_offered_incompatible_qos");

      ::DDS::OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::DataWriter_var dw = this->create_datawriter (writer);
      this->impl_->on_offered_incompatible_qos (dw.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_liveliness_lost (
        ::DDSDataWriter * writer,
        const ::DDS_LivelinessLostStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_liveliness_lost");

      ::DDS::LivelinessLostStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::DataWriter_var dw = this->create_datawriter (writer);
      this->impl_->on_liveliness_lost (dw.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_publication_matched (
        ::DDSDataWriter * writer,
        const ::DDS_PublicationMatchedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantListener_i::on_publication_matched");

      ::DDS::PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;

      ::DDS::DataWriter_var dw = this->create_datawriter (writer);
      this->impl_->on_publication_matched (dw.in (), ddsstatus);
    }

    void
    DDS_DomainParticipantListener_i::on_data_on_readers (
        ::DDSSubscriber* subscriber)
    {
      ::DDS::Subscriber_var sub;
      ACE_NEW_THROW_EX (sub,
                        DDS_Subscriber_i (subscriber,
                                          this->dp_.in ()),
                        ::CORBA::NO_MEMORY ());
      this->impl_->on_data_on_readers (sub.in ());
    }

    ::DDS::DomainParticipantListener_ptr
    DDS_DomainParticipantListener_i::get_domainparticipantlistener (void)
    {
      return ::DDS::DomainParticipantListener::_duplicate (this->impl_.in ());
    }

    void
    DDS_DomainParticipantListener_i::set_dds_dp (::DDS::DomainParticipant_ptr dp)
    {
      this->dp_ = ::DDS::DomainParticipant::_duplicate (dp);
    }
  }
}

