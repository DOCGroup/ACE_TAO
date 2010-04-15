// $Id$

#include "DataWriterListener.h"
#include "DataWriter.h"
#include "ndds/PublicationMatchedStatus.h"
#include "ndds/LivelinessLostStatus.h"
#include "ndds/OfferedIncompatibleQosStatus.h"
#include "ndds/OfferedDeadlineMissedStatus.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_DataWriterListener_i::CCM_DDS_DataWriterListener_i (::DDS::DataWriterListener_ptr s)
      : impl_ (::DDS::DataWriterListener::_duplicate (s)),
        dds_writer_ (::DDS::DataWriter::_nil ())

    {
    }

    CCM_DDS_DataWriterListener_i::~CCM_DDS_DataWriterListener_i (void)
    {
    }

    ::DDS::DataWriter_ptr
    CCM_DDS_DataWriterListener_i::get_datawriter_proxy (::DDSDataWriter * the_writer)
    {
      if (::CORBA::is_nil (this->dds_writer_.in ()))
        {
          ACE_NEW_NORETURN (this->dds_writer_,
                             CCM_DDS_DataWriter_i (the_writer));
        }
      return this->dds_writer_.in ();
    }


    void
    CCM_DDS_DataWriterListener_i::on_offered_deadline_missed (
      ::DDSDataWriter *the_writer,
      const ::DDS_OfferedDeadlineMissedStatus & status)
    {
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
    CCM_DDS_DataWriterListener_i::on_offered_incompatible_qos (
      ::DDSDataWriter *the_writer,
      const ::DDS_OfferedIncompatibleQosStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_offered_incompatible_qos (
        this->get_datawriter_proxy (the_writer),
        ddsstatus);
#else
      this->impl_->on_offered_incompatible_qos (
        this->get_datawriter_proxy (the_writer),
        ddsstatus);
#endif
    }

    void
    CCM_DDS_DataWriterListener_i::on_liveliness_lost (
      ::DDSDataWriter *the_writer,
      const ::DDS_LivelinessLostStatus & status)
    {
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
    CCM_DDS_DataWriterListener_i::on_publication_matched (
      ::DDSDataWriter *the_writer,
      const ::DDS_PublicationMatchedStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_publication_matched (
        this->get_datawriter_proxy (the_writer),
        ddsstatus);
#else
      this->impl_->on_publication_matched ();
        this->get_datawriter_proxy (the_writer),
        ddsstatus);
#endif
    }

    ::DDS::DataWriterListener_ptr
    CCM_DDS_DataWriterListener_i::get_datawriterlistener (void)
    {
      return ::DDS::DataWriterListener::_duplicate (this->impl_.in ());
    }
  }
}

