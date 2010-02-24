// $Id$

#include "PublisherListener.h"
#include "Publisher.h"
#include "DataWriter.h"
#include "PublicationMatchedStatus.h"
#include "LivelinessLostStatus.h"
#include "OfferedIncompatibleQosStatus.h"
#include "OfferedDeadlineMissedStatus.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

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

    void
    CCM_DDS_PublisherListener_i::on_offered_deadline_missed (
      ::DDSDataWriter* writer,
      const ::DDS_OfferedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::on_offered_deadline_missed");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::OfferedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
      ACE_NEW (dds_writer, CCM_DDS_DataWriter_i ());
      CCM_DDS_DataWriter_i *dw = dynamic_cast< CCM_DDS_DataWriter_i * > (dds_writer.in ());
      dw->set_impl (writer);
      this->impl_->on_offered_deadline_missed (dds_writer.in (), ddsstatus);
#else
      this->impl_->on_offered_deadline_missed (writer, status);
#endif
    }

    void
    CCM_DDS_PublisherListener_i::on_offered_incompatible_qos (
      ::DDSDataWriter* writer,
      const ::DDS_OfferedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::on_offered_incompatible_qos");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
      ACE_NEW (dds_writer, CCM_DDS_DataWriter_i ());
      CCM_DDS_DataWriter_i *dw = dynamic_cast< CCM_DDS_DataWriter_i * > (dds_writer.in ());
      dw->set_impl (writer);
      this->impl_->on_offered_incompatible_qos (dds_writer.in (), ddsstatus);
#else
      this->impl_->on_offered_incompatible_qos (writer, status);
#endif
    }

    void
    CCM_DDS_PublisherListener_i::on_liveliness_lost (
      ::DDSDataWriter* writer,
      const ::DDS_LivelinessLostStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::on_liveliness_lost");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::LivelinessLostStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
      ACE_NEW (dds_writer, CCM_DDS_DataWriter_i ());
      CCM_DDS_DataWriter_i *dw = dynamic_cast< CCM_DDS_DataWriter_i * > (dds_writer.in ());
      dw->set_impl (writer);
      this->impl_->on_liveliness_lost (dds_writer.in (), ddsstatus);
#else
      this->impl_->on_liveliness_lost (writer, status);
#endif
    }

    void
    CCM_DDS_PublisherListener_i::on_publication_matched (
      ::DDSDataWriter* writer,
      const ::DDS_PublicationMatchedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_i::on_publication_matched");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
      ACE_NEW (dds_writer, CCM_DDS_DataWriter_i ());
      CCM_DDS_DataWriter_i *dw = dynamic_cast< CCM_DDS_DataWriter_i * > (dds_writer.in ());
      dw->set_impl (writer);
      this->impl_->on_publication_matched (dds_writer.in (), ddsstatus);
#else
      this->impl_->on_publication_matched (writer, status);
#endif
    }

    ::DDS::PublisherListener_ptr
    CCM_DDS_PublisherListener_i::get_publisher_listener (void)
    {
      return ::DDS::PublisherListener::_duplicate (this->impl_.in ());
    }
  }
}

