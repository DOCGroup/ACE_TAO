// $Id$

#include "dds4ccm/impl/ndds/PublisherListener.h"
#include "dds4ccm/impl/ndds/Publisher.h"
#include "dds4ccm/impl/ndds/DataWriter_T.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/TypeSupport.h"

#include "dds4ccm/impl/ndds/convertors/PublicationMatchedStatus.h"
#include "dds4ccm/impl/ndds/convertors/LivelinessLostStatus.h"
#include "dds4ccm/impl/ndds/convertors/OfferedIncompatibleQosStatus.h"
#include "dds4ccm/impl/ndds/convertors/OfferedDeadlineMissedStatus.h"
#include "dds4ccm/impl/ndds/convertors/ReliableReaderActivityChangedStatus.h"
#include "dds4ccm/impl/ndds/convertors/ReliableWriterCacheChangedStatus.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_PublisherListener_i::DDS_PublisherListener_i (
      ::DDS::PublisherListener_ptr p,
      ::DDS::DomainParticipant_ptr dp)
      : impl_ (::DDS::PublisherListener::_duplicate (p)),
        dp_ (::DDS::DomainParticipant::_duplicate (dp))
    {
      DDS4CCM_TRACE ("DDS_PublisherListener_i::DDS_PublisherListener_i");
    }

    DDS_PublisherListener_i::~DDS_PublisherListener_i (void)
    {
      DDS4CCM_TRACE ("DDS_PublisherListener_i::~DDS_PublisherListener_i");
    }

    void
    DDS_PublisherListener_i::on_offered_deadline_missed (
      ::DDSDataWriter* the_writer,
      const ::DDS_OfferedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_PublisherListener_i::on_offered_deadline_missed");

      ::DDS::OfferedDeadlineMissedStatus ddsstatus;
      ::DDS::DataWriter_var proxy =
        DDS_TypeSupport_i::create_datawriter (the_writer,
                                              this->dp_.in (),
                                              ::DDS::Publisher::_nil ());
      ddsstatus <<= status;
      this->impl_->on_offered_deadline_missed (proxy.in (), ddsstatus);
    }

    void
    DDS_PublisherListener_i::on_offered_incompatible_qos (
      ::DDSDataWriter* the_writer,
      const ::DDS_OfferedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("DDS_PublisherListener_i::on_offered_incompatible_qos");

      ::DDS::OfferedIncompatibleQosStatus ddsstatus;
      ::DDS::DataWriter_var proxy = DDS_TypeSupport_i::create_datawriter (
                                                      the_writer,
                                                      this->dp_.in (),
                                                      ::DDS::Publisher::_nil ());
      ddsstatus <<= status;
      this->impl_->on_offered_incompatible_qos (proxy.in (), ddsstatus);
    }

    void
    DDS_PublisherListener_i::on_liveliness_lost (
      ::DDSDataWriter* the_writer,
      const ::DDS_LivelinessLostStatus & status)
    {
      DDS4CCM_TRACE ("DDS_PublisherListener_i::on_liveliness_lost");

      ::DDS::LivelinessLostStatus ddsstatus;
      ::DDS::DataWriter_var proxy = DDS_TypeSupport_i::create_datawriter (
                                                      the_writer,
                                                      this->dp_.in (),
                                                      ::DDS::Publisher::_nil ());
      ddsstatus <<= status;
      this->impl_->on_liveliness_lost (proxy.in (), ddsstatus);
    }

    void
    DDS_PublisherListener_i::on_publication_matched (
      ::DDSDataWriter* the_writer,
      const ::DDS_PublicationMatchedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_PublisherListener_i::on_publication_matched");

      ::DDS::PublicationMatchedStatus ddsstatus;
      ::DDS::DataWriter_var proxy = DDS_TypeSupport_i::create_datawriter (
                                                      the_writer,
                                                      this->dp_.in (),
                                                      ::DDS::Publisher::_nil ());
      ddsstatus <<= status;
      this->impl_->on_publication_matched (proxy.in (), ddsstatus);
    }

    void
    DDS_PublisherListener_i::on_reliable_reader_activity_changed (
      ::DDSDataWriter *the_writer,
      const ::DDS_ReliableReaderActivityChangedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_PublisherListener_i::on_reliable_reader_activity_changed");

      ::DDS::ReliableReaderActivityChangedStatus ddsstatus;
      ::DDS::DataWriter_var proxy = DDS_TypeSupport_i::create_datawriter (
                                                      the_writer,
                                                      this->dp_.in (),
                                                      ::DDS::Publisher::_nil ());
      ddsstatus <<= status;
      this->impl_->on_reliable_reader_activity_changed (proxy.in (), ddsstatus);
    }

    void
    DDS_PublisherListener_i::on_reliable_writer_cache_changed (
      ::DDSDataWriter* the_writer,
      const ::DDS_ReliableWriterCacheChangedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_PublisherListener_i::on_reliable_writer_cache_changed");

      ::DDS::ReliableWriterCacheChangedStatus ddsstatus;
      ::DDS::DataWriter_var proxy = DDS_TypeSupport_i::create_datawriter (
                                                      the_writer,
                                                      this->dp_.in (),
                                                      ::DDS::Publisher::_nil ());
      ddsstatus <<= status;
      this->impl_->on_reliable_writer_cache_changed (proxy.in (), ddsstatus);
    }

    ::DDS::PublisherListener_ptr
    DDS_PublisherListener_i::get_publisher_listener (void)
    {
      return ::DDS::PublisherListener::_duplicate (this->impl_.in ());
    }
  }
}

