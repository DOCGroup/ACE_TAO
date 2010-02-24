// $Id$

#include "DataWriterListener.h"
#include "DataWriter.h"
#include "PublicationMatchedStatus.h"
#include "LivelinessLostStatus.h"
#include "OfferedIncompatibleQosStatus.h"
#include "OfferedDeadlineMissedStatus.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_DataWriterListener_i::CCM_DDS_DataWriterListener_i (::DDS::DataWriterListener_ptr s)
      : impl_ (::DDS::DataWriterListener::_duplicate (s))
    {
    }

    CCM_DDS_DataWriterListener_i::~CCM_DDS_DataWriterListener_i (void)
    {
    }

    void
    CCM_DDS_DataWriterListener_i::on_offered_deadline_missed (
      ::DDSDataWriter *the_writer,
      const ::DDS_OfferedDeadlineMissedStatus & status)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::OfferedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
      ACE_NEW (dds_writer, CCM_DDS_DataWriter_i ());
      CCM_DDS_DataWriter_i *dw = dynamic_cast< CCM_DDS_DataWriter_i * > (dds_writer.in ());
      dw->set_impl (the_writer);
      this->impl_->on_offered_deadline_missed (dds_writer.in (), ddsstatus);
#else
      this->impl_->on_offered_deadline_missed (the_writer, status);
#endif
    }

    void
    CCM_DDS_DataWriterListener_i::on_offered_incompatible_qos (
      ::DDSDataWriter *the_writer,
      const ::DDS_OfferedIncompatibleQosStatus & status)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
      ACE_NEW (dds_writer, CCM_DDS_DataWriter_i ());
      CCM_DDS_DataWriter_i *dw = dynamic_cast< CCM_DDS_DataWriter_i * > (dds_writer.in ());
      dw->set_impl (the_writer);
      this->impl_->on_offered_incompatible_qos (dds_writer.in (), ddsstatus);
#else
      this->impl_->on_offered_incompatible_qos (the_writer, status);
#endif
    }

    void
    CCM_DDS_DataWriterListener_i::on_liveliness_lost (
      ::DDSDataWriter *the_writer,
      const ::DDS_LivelinessLostStatus & status)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::LivelinessLostStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
      ACE_NEW (dds_writer, CCM_DDS_DataWriter_i ());
      CCM_DDS_DataWriter_i *dw = dynamic_cast< CCM_DDS_DataWriter_i * > (dds_writer.in ());
      dw->set_impl (the_writer);
      this->impl_->on_liveliness_lost (dds_writer.in (), ddsstatus);
#else
      this->impl_->on_liveliness_lost (the_writer, status);
#endif
    }

    void
    CCM_DDS_DataWriterListener_i::on_publication_matched (
      ::DDSDataWriter *the_writer,
      const ::DDS_PublicationMatchedStatus & status)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
      ACE_NEW (dds_writer, CCM_DDS_DataWriter_i ());
      CCM_DDS_DataWriter_i *dw = dynamic_cast< CCM_DDS_DataWriter_i * > (dds_writer.in ());
      dw->set_impl (the_writer);
      this->impl_->on_publication_matched (dds_writer.in (), ddsstatus);
#else
      this->impl_->on_publication_matched (the_writer, status);
#endif
    }

    ::DDS::DataWriterListener_ptr
    CCM_DDS_DataWriterListener_i::get_datawriterlistener (void)
    {
      return ::DDS::DataWriterListener::_duplicate (this->impl_.in ());
    }
  }
}

