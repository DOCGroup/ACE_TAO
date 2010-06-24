// $Id$

#include "DataWriterListener_T.h"
#include "DataWriter_T.h"
#include "ndds/PublicationMatchedStatus.h"
#include "ndds/LivelinessLostStatus.h"
#include "ndds/OfferedIncompatibleQosStatus.h"
#include "ndds/OfferedDeadlineMissedStatus.h"

#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/impl/dds/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::CCM_DDS_DataWriterListener_T (::DDS::DataWriterListener_ptr s)
      : impl_ (::DDS::DataWriterListener::_duplicate (s))
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::~CCM_DDS_DataWriterListener_T (void)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::CCM_DataWriter_ptr
    CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_datawriter_proxy (::DDSDataWriter * the_writer)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_datawriter_proxy");
      // Retrieve the pointer to the proxy from the QoS
      ::DDS_DataWriterQos qos;
      the_writer->get_qos (qos);
      DDS_Property_t * prop =
        DDSPropertyQosPolicyHelper::lookup_property (qos.property,
                                                    "CCM_DataWriterProxy");
      if (!prop)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DataWriterListener_T"
                            "<DDS_TYPE, CCM_TYPE>::get_datawriter_proxy -"
                            "Unable to retrieve proxy from PropertyQosProfile\n"));
          return ::DDS::CCM_DataWriter::_nil ();
        }

      DataWriter_type* writer =
        reinterpret_cast < DataWriter_type* >
          (ACE_OS::atol (prop->value));

      if (!writer)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DataWriterListener_T"
                            "<DDS_TYPE, CCM_TYPE>::get_datawriter_proxy -"
                            "Unable to get writer from PropertyQosProfile\n"));
        }

      return writer;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_offered_deadline_missed (
      ::DDSDataWriter *the_writer,
      const ::DDS_OfferedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_offered_deadline_missed");
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_offered_incompatible_qos (
      ::DDSDataWriter *the_writer,
      const ::DDS_OfferedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_offered_incompatible_qos");
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_lost (
      ::DDSDataWriter *the_writer,
      const ::DDS_LivelinessLostStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_lost");
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_publication_matched (
      ::DDSDataWriter *the_writer,
      const ::DDS_PublicationMatchedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::on_publication_matched");
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DataWriterListener_ptr
    CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_datawriterlistener (void)
    {
      return ::DDS::DataWriterListener::_duplicate (this->impl_.in ());
    }
  }
}

