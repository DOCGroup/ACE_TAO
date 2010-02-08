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
    namespace RTI
    {
      RTI_DataWriterListener_i::RTI_DataWriterListener_i (::DDS::DataWriterListener_ptr s)
        : impl_ (::DDS::DataWriterListener::_duplicate (s))
      {
      }

      RTI_DataWriterListener_i::~RTI_DataWriterListener_i (void)
      {
      }

      void
      RTI_DataWriterListener_i::on_offered_deadline_missed (
        ::DDSDataWriter *the_writer,
        const ::DDS_OfferedDeadlineMissedStatus & status)
      {
        ::DDS::OfferedDeadlineMissedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataWriter_var dds_writer = new RTI_DataWriter_i ();
        RTI_DataWriter_i *dw = dynamic_cast< RTI_DataWriter_i * > (dds_writer.in ());
        dw->set_impl (the_writer);
        this->impl_->on_offered_deadline_missed (dds_writer.in (), ddsstatus);
      }

      void
      RTI_DataWriterListener_i::on_offered_incompatible_qos (
        ::DDSDataWriter *the_writer,
        const ::DDS_OfferedIncompatibleQosStatus & status)
      {
        ::DDS::OfferedIncompatibleQosStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataWriter_var dds_writer = new RTI_DataWriter_i ();
        RTI_DataWriter_i *dw = dynamic_cast< RTI_DataWriter_i * > (dds_writer.in ());
        dw->set_impl (the_writer);
        this->impl_->on_offered_incompatible_qos (dds_writer.in (), ddsstatus);
      }

      void
      RTI_DataWriterListener_i::on_liveliness_lost (
        ::DDSDataWriter *the_writer,
        const ::DDS_LivelinessLostStatus & status)
      {
        ::DDS::LivelinessLostStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataWriter_var dds_writer = new RTI_DataWriter_i ();
        RTI_DataWriter_i *dw = dynamic_cast< RTI_DataWriter_i * > (dds_writer.in ());
        dw->set_impl (the_writer);
        this->impl_->on_liveliness_lost (dds_writer.in (), ddsstatus);
      }

      void
      RTI_DataWriterListener_i::on_publication_matched (
        ::DDSDataWriter *the_writer,
        const ::DDS_PublicationMatchedStatus & status)
      {
        ::DDS::PublicationMatchedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataWriter_var dds_writer = new RTI_DataWriter_i ();
        RTI_DataWriter_i *dw = dynamic_cast< RTI_DataWriter_i * > (dds_writer.in ());
        dw->set_impl (the_writer);
        this->impl_->on_publication_matched (dds_writer.in (), ddsstatus);
      }

      ::DDS::DataWriterListener_ptr
      RTI_DataWriterListener_i::get_datawriterlistener (void)
      {
        return ::DDS::DataWriterListener::_duplicate (this->impl_.in ());
      }
    }
  }
}

