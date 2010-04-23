// $Id$

#include "DataWriterListener.h"
#include "DataWriter.h"
#include "ndds/PublicationMatchedStatus.h"
#include "ndds/LivelinessLostStatus.h"
#include "ndds/OfferedIncompatibleQosStatus.h"
#include "ndds/OfferedDeadlineMissedStatus.h"

#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

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

    ::DDS::DataWriter_ptr
    CCM_DDS_DataWriterListener_i::get_datawriter_proxy (::DDSDataWriter * the_writer)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriterListener_i::get_datawriter_proxy");
      //Retrieve the pointer to the proxy from the QoS
      ::DDS_DataWriterQos qos;
      the_writer->get_qos (qos);
      DDS_Property_t * prop =
        DDSPropertyQosPolicyHelper::lookup_property (qos.property,
                                                    "CCM_DataWriterProxy");
      if (!prop)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DataReaderListener_i::get_datareader_proxy -"
                                              "Unable to retrieve proxy from PropertyQosProfile\n"));
          return ::DDS::DataWriter::_nil ();
        }
      ::DDS::CCM_DataWriter_ptr writer =
        reinterpret_cast < ::DDS::CCM_DataWriter_ptr >
          (ACE_OS::atol (prop->value));

      return reinterpret_cast < ::DDS::DataWriter_ptr >
          (writer);
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

