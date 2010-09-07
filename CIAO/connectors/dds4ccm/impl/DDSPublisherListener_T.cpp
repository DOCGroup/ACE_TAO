// $Id$

#include "DDSPublisherListener_T.h"
#include "Publisher_T.h"
#include "DataWriter_T.h"

#include "ndds/PublicationMatchedStatus.h"
#include "ndds/LivelinessLostStatus.h"
#include "ndds/OfferedIncompatibleQosStatus.h"
#include "ndds/OfferedDeadlineMissedStatus.h"
#include "ndds/ReliableReaderActivityChangedStatus.h"
#include "ndds/ReliableWriterCacheChangedStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::CCM_DDS_PublisherListener_T ( ::DDS::PublisherListener_ptr p)
      : impl_ (::DDS::PublisherListener::_duplicate (p))
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::CCM_DDS_PublisherListener_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~CCM_DDS_PublisherListener_T (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~CCM_DDS_PublisherListener_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::CCM_DataWriter_ptr
    CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_datawriter_proxy (::DDSDataWriter * the_writer)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_datawriter_proxy");
      ::DDS::CCM_DataWriter_var dds_writer;
      ACE_NEW_NORETURN (dds_writer,
                        DataWriter_type (the_writer));
      return dds_writer._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_offered_deadline_missed (
      ::DDSDataWriter* the_writer,
      const ::DDS_OfferedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_offered_deadline_missed");

      ::DDS::OfferedDeadlineMissedStatus ddsstatus;
      ::DDS::CCM_DataWriter_var proxy = this->get_datawriter_proxy (the_writer);
      ddsstatus <<= status;
      this->impl_->on_offered_deadline_missed (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_offered_incompatible_qos (
      ::DDSDataWriter* the_writer,
      const ::DDS_OfferedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_offered_incompatible_qos");

      ::DDS::OfferedIncompatibleQosStatus ddsstatus;
      ::DDS::CCM_DataWriter_var proxy = this->get_datawriter_proxy (the_writer);
      ddsstatus <<= status;
      this->impl_->on_offered_incompatible_qos (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_liveliness_lost (
      ::DDSDataWriter* the_writer,
      const ::DDS_LivelinessLostStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_liveliness_lost");

      ::DDS::LivelinessLostStatus ddsstatus;
      ::DDS::CCM_DataWriter_var proxy = this->get_datawriter_proxy (the_writer);
      ddsstatus <<= status;
      this->impl_->on_liveliness_lost (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_publication_matched (
      ::DDSDataWriter* the_writer,
      const ::DDS_PublicationMatchedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_publication_matched");

      ::DDS::PublicationMatchedStatus ddsstatus;
      ::DDS::CCM_DataWriter_var proxy = this->get_datawriter_proxy (the_writer);
      ddsstatus <<= status;
      this->impl_->on_publication_matched (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_reliable_reader_activity_changed (
      ::DDSDataWriter *the_writer,
      const ::DDS_ReliableReaderActivityChangedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_reliable_reader_activity_changed");

      ::DDS::ReliableReaderActivityChangedStatus ddsstatus;
      ::DDS::CCM_DataWriter_var proxy = this->get_datawriter_proxy (the_writer);
      ddsstatus <<= status;
      this->impl_->on_reliable_reader_activity_changed (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_reliable_writer_cache_changed (
      ::DDSDataWriter* the_writer,
      const ::DDS_ReliableWriterCacheChangedStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_reliable_writer_cache_changed");

      ::DDS::ReliableWriterCacheChangedStatus ddsstatus;
      ::DDS::CCM_DataWriter_var proxy = this->get_datawriter_proxy (the_writer);
      ddsstatus <<= status;
      this->impl_->on_reliable_writer_cache_changed (proxy.in (), ddsstatus);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::PublisherListener_ptr
    CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_publisher_listener (void)
    {
      return ::DDS::PublisherListener::_duplicate (this->impl_.in ());
    }
  }
}

