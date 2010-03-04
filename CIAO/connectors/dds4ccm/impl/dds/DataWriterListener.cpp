// $Id$

#include "DataWriterListener.h"
#include "DataWriter.h"
#include "ndds/PublicationMatchedStatus.h"
#include "ndds/LivelinessLostStatus.h"
#include "ndds/OfferedIncompatibleQosStatus.h"
#include "ndds/OfferedDeadlineMissedStatus.h"

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
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::OfferedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      ACE_NEW (dds_writer, 
               CCM_DDS_DataWriter_i (the_writer));
      this->impl_->on_offered_deadline_missed (dds_writer.in (), ddsstatus);
#else
      ACE_NEW (dds_writer, 
               CCM_DDS_DataWriter_i (the_writer));
      this->impl_->on_offered_deadline_missed (dds_writer.in (), status);
#endif
    }

    void
    CCM_DDS_DataWriterListener_i::on_offered_incompatible_qos (
      ::DDSDataWriter *the_writer,
      const ::DDS_OfferedIncompatibleQosStatus & status)
    {
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      ACE_NEW (dds_writer, 
               CCM_DDS_DataWriter_i (the_writer));
      this->impl_->on_offered_incompatible_qos (dds_writer.in (), ddsstatus);
#else
      ACE_NEW (dds_writer, 
               CCM_DDS_DataWriter_i (the_writer));
      this->impl_->on_offered_incompatible_qos (dds_writer.in (), status);
#endif
    }

    void
    CCM_DDS_DataWriterListener_i::on_liveliness_lost (
      ::DDSDataWriter *the_writer,
      const ::DDS_LivelinessLostStatus & status)
    {
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::LivelinessLostStatus ddsstatus;
      ddsstatus <<= status;
      ACE_NEW (dds_writer, 
               CCM_DDS_DataWriter_i (the_writer));
      this->impl_->on_liveliness_lost (dds_writer.in (), ddsstatus);
#else
      ACE_NEW (dds_writer, 
               CCM_DDS_DataWriter_i (the_writer));
      this->impl_->on_liveliness_lost (dds_writer.in (), status);
#endif
    }

    void
    CCM_DDS_DataWriterListener_i::on_publication_matched (
      ::DDSDataWriter *the_writer,
      const ::DDS_PublicationMatchedStatus & status)
    {
      ::DDS::DataWriter_var dds_writer = ::DDS::DataWriter::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;
      ACE_NEW (dds_writer, 
               CCM_DDS_DataWriter_i (the_writer));
      this->impl_->on_publication_matched (dds_writer.in (), ddsstatus);
#else
      ACE_NEW (dds_writer, 
               CCM_DDS_DataWriter_i (the_writer));
      this->impl_->on_publication_matched (dds_writer.in (), status);
#endif
    }

    ::DDS::DataWriterListener_ptr
    CCM_DDS_DataWriterListener_i::get_datawriterlistener (void)
    {
      return ::DDS::DataWriterListener::_duplicate (this->impl_.in ());
    }
  }
}

