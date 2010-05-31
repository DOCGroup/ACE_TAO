// $Id$

#include "PublisherListener.h"
#include "Publisher.h"
#include "DataWriter.h"

#include "ndds/PublicationMatchedStatus.h"
#include "ndds/LivelinessLostStatus.h"
#include "ndds/OfferedIncompatibleQosStatus.h"
#include "ndds/OfferedDeadlineMissedStatus.h"
#include "ndds/ReliableReaderActivityChangedStatus.h"
#include "ndds/ReliableWriterCacheChangedStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_PublisherListener_i::CCM_DDS_PublisherListener_i ( ::DDS::PublisherListener_ptr p)
      : impl_ (::DDS::PublisherListener::_duplicate (p))
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::CCM_DDS_PublisherListener_i");
    }

    CCM_DDS_PublisherListener_i::~CCM_DDS_PublisherListener_i (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::~CCM_DDS_PublisherListener_i");
    }

    ::DDS::CCM_DataWriter_ptr
    CCM_DDS_PublisherListener_i::get_datawriter_proxy (::DDSDataWriter * the_writer)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::get_datawriter_proxy");
      //Retrieve the pointer to the proxy from the QoS
      ::DDS_DataWriterQos qos;
      the_writer->get_qos (qos);
      DDS_Property_t * prop =
        DDSPropertyQosPolicyHelper::lookup_property (qos.property,
                                                    "CCM_DataWriterProxy");
      if (!prop)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        "CCM_DDS_PublisherListener_i::get_datareader_proxy -"
                        "Unable to retrieve proxy from PropertyQosProfile\n"));
          return ::DDS::CCM_DataWriter::_nil ();
        }

      CCM_DDS_DataWriter_i* writer =
        reinterpret_cast < CCM_DDS_DataWriter_i* >
          (ACE_OS::atol (prop->value));

      if (!writer)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        "CCM_DDS_PublisherListener_i::get_datareader_proxy -"
                        "Unable to retrieve writer from PropertyQosProfile\n"));
        }

      return writer;
    }

    void
    CCM_DDS_PublisherListener_i::on_offered_deadline_missed (
      ::DDSDataWriter* the_writer,
      const ::DDS_OfferedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::on_offered_deadline_missed");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::OfferedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_offered_deadline_missed (
        this->get_datawriter_proxy (the_writer),
        ddsstatus);
#else
      this->impl_->on_offered_deadline_missed (
        this->get_datawriter_proxy (the_writer),
        status);
#endif
    }

    void
    CCM_DDS_PublisherListener_i::on_offered_incompatible_qos (
      ::DDSDataWriter* the_writer,
      const ::DDS_OfferedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::on_offered_incompatible_qos");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_offered_incompatible_qos (
        this->get_datawriter_proxy (the_writer),
        ddsstatus);
#else
      this->impl_->on_offered_incompatible_qos (
        this->get_datawriter_proxy (the_writer),
        status);
#endif
    }

    void
    CCM_DDS_PublisherListener_i::on_liveliness_lost (
      ::DDSDataWriter* the_writer,
      const ::DDS_LivelinessLostStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::on_liveliness_lost");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::LivelinessLostStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_liveliness_lost (
        this->get_datawriter_proxy (the_writer),
        ddsstatus);
#else
      this->impl_->on_liveliness_lost (
        this->get_datawriter_proxy (the_writer),
        status);
#endif
    }

    void
    CCM_DDS_PublisherListener_i::on_publication_matched (
      ::DDSDataWriter* the_writer,
      const ::DDS_PublicationMatchedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::on_publication_matched");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_publication_matched (
        this->get_datawriter_proxy (the_writer),
        ddsstatus);
#else
      this->impl_->on_publication_matched (
        this->get_datawriter_proxy (the_writer),
        status);
#endif
    }

    void
    CCM_DDS_PublisherListener_i::on_reliable_reader_activity_changed (
      ::DDSDataWriter *the_writer,
      const ::DDS_ReliableReaderActivityChangedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::on_reliable_reader_activity_changed");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::ReliableReaderActivityChangedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_reliable_reader_activity_changed (
        this->get_datawriter_proxy (the_writer),
        ddsstatus);
#else
      this->impl_->on_reliable_reader_activity_changed (
        this->get_datawriter_proxy (the_writer),
        status);
#endif
    }

    void
    CCM_DDS_PublisherListener_i::on_reliable_writer_cache_changed (
      ::DDSDataWriter* the_writer,
      const ::DDS_ReliableWriterCacheChangedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::on_reliable_writer_cache_changed");

#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::ReliableWriterCacheChangedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_reliable_writer_cache_changed (
        this->get_datawriter_proxy (the_writer),
        ddsstatus);
#else
      this->impl_->on_reliable_writer_cache_changed (
        this->get_datawriter_proxy (the_writer),
        status);
#endif
    }

    ::DDS::PublisherListener_ptr
    CCM_DDS_PublisherListener_i::get_publisher_listener (void)
    {
      return ::DDS::PublisherListener::_duplicate (this->impl_.in ());
    }
  }
}

