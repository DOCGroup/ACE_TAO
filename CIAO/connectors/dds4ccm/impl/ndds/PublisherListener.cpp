// $Id$

#include "PublisherListener.h"
#include "Publisher.h"
#include "DataWriter.h"
#include "PublicationMatchedStatus.h"
#include "LivelinessLostStatus.h"
#include "OfferedIncompatibleQosStatus.h"
#include "OfferedDeadlineMissedStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_PublisherListener_i::RTI_PublisherListener_i ( ::DDS::PublisherListener_ptr p)
        : impl_ (::DDS::PublisherListener::_duplicate (p))
      {
      }

      // Implementation skeleton destructor
      RTI_PublisherListener_i::~RTI_PublisherListener_i (void)
      {
      }

      void
      RTI_PublisherListener_i::on_offered_deadline_missed (
        ::DDSDataWriter* writer,
        const ::DDS_OfferedDeadlineMissedStatus & status)
      {
        ::DDS::OfferedDeadlineMissedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataWriter_var dds_writer = new RTI_DataWriter_i (writer);
        this->impl_->on_offered_deadline_missed (dds_writer.in (), ddsstatus);
      }

      void
      RTI_PublisherListener_i::on_offered_incompatible_qos (
        ::DDSDataWriter* writer,
        const ::DDS_OfferedIncompatibleQosStatus & status)
      {
        ::DDS::OfferedIncompatibleQosStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataWriter_var dds_writer = new RTI_DataWriter_i (writer);
        this->impl_->on_offered_incompatible_qos (dds_writer.in (), ddsstatus);
      }

      void
      RTI_PublisherListener_i::on_liveliness_lost (
        ::DDSDataWriter* writer,
        const ::DDS_LivelinessLostStatus & status)
      {
        ::DDS::LivelinessLostStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataWriter_var dds_writer = new RTI_DataWriter_i (writer);
        this->impl_->on_liveliness_lost (dds_writer.in (), ddsstatus);
      }

      void
      RTI_PublisherListener_i::on_publication_matched (
        ::DDSDataWriter* writer,
        const ::DDS_PublicationMatchedStatus & status)
      {
        ::DDS::PublicationMatchedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataWriter_var dds_writer = new RTI_DataWriter_i (writer);
        this->impl_->on_publication_matched (dds_writer.in (), ddsstatus);
      }

      ::DDS::PublisherListener_ptr
      RTI_PublisherListener_i::get_publisher_listener (void)
      {
        return ::DDS::PublisherListener::_duplicate (this->impl_.in ());
      }
    }
  }
}

