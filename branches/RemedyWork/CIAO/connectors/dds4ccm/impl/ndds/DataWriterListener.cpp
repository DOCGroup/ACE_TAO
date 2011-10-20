// $Id$

#include "dds4ccm/impl/ndds/DataWriterListener.h"
#include "dds4ccm/impl/ndds/convertors/PublicationMatchedStatus.h"
#include "dds4ccm/impl/ndds/convertors/LivelinessLostStatus.h"
#include "dds4ccm/impl/ndds/convertors/OfferedIncompatibleQosStatus.h"
#include "dds4ccm/impl/ndds/convertors/OfferedDeadlineMissedStatus.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {

    DDS_DataWriterListener_i::DDS_DataWriterListener_i (
      ::DDS::DataWriterListener_ptr s,
      ::DDS::DataWriter_ptr dw)
      : impl_ (::DDS::DataWriterListener::_duplicate (s)),
        dw_ (::DDS::DataWriter::_duplicate (dw))
    {
    }

    DDS_DataWriterListener_i::~DDS_DataWriterListener_i (void)
    {
    }

    void
    DDS_DataWriterListener_i::on_offered_deadline_missed (
      ::DDSDataWriter *,
      const ::DDS_OfferedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataWriterListener_i::on_offered_deadline_missed");
      ::DDS::OfferedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_offered_deadline_missed (this->dw_, ddsstatus);
    }


    void
    DDS_DataWriterListener_i::on_offered_incompatible_qos (
      ::DDSDataWriter *,
      const ::DDS_OfferedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataWriterListener_i::on_offered_incompatible_qos");
      ::DDS::OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_offered_incompatible_qos (this->dw_, ddsstatus);
    }

    void
    DDS_DataWriterListener_i::on_liveliness_lost (
      ::DDSDataWriter *,
      const ::DDS_LivelinessLostStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataWriterListener_i::on_liveliness_lost");
      ::DDS::LivelinessLostStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_liveliness_lost (this->dw_, ddsstatus);
    }

    void
    DDS_DataWriterListener_i::on_publication_matched (
      ::DDSDataWriter *,
      const ::DDS_PublicationMatchedStatus & status)
    {
      DDS4CCM_TRACE ("DDS_DataWriterListener_i::on_publication_matched");
      ::DDS::PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_publication_matched (this->dw_, ddsstatus);
    }

    ::DDS::DataWriterListener_ptr
    DDS_DataWriterListener_i::get_datawriterlistener (void)
    {
      return ::DDS::DataWriterListener::_duplicate (this->impl_.in ());
    }

    void
    DDS_DataWriterListener_i::set_dds_entity (::DDS::DataWriter_ptr dw)
    {
      this->dw_ = ::DDS::DataWriter::_duplicate (dw);
    }
  }
}

