/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef PUBLISHERLISTENER_T_H_
#define PUBLISHERLISTENER_T_H_

#include "dds4ccm/idl/dds4ccm_ConnectorStatusListenerC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export PublisherListener :
      public ::DDS::PublisherListener
    {
    public:
      /// Constructor
      PublisherListener (
        ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
        ACE_Reactor* reactor);

      /// Destructor
      virtual ~PublisherListener (void);

      virtual void on_offered_deadline_missed (
        ::DDS::DataWriter_ptr the_Writer,
        const ::DDS::OfferedDeadlineMissedStatus & status);

      virtual void on_liveliness_lost (
        ::DDS::DataWriter_ptr the_Writer,
        const ::DDS::LivelinessLostStatus & status);

      virtual void on_offered_incompatible_qos (
        ::DDS::DataWriter_ptr the_Writer,
        const ::DDS::OfferedIncompatibleQosStatus & status);

      virtual void on_publication_matched (
        ::DDS::DataWriter_ptr the_Writer,
        const ::DDS::PublicationMatchedStatus & status);

#if (CIAO_DDS4CCM_NDDS==1)
      virtual void on_reliable_writer_cache_changed (
        ::DDS::DataWriter_ptr the_writer,
        const ::DDS::ReliableWriterCacheChangedStatus & status);

      virtual void on_reliable_reader_activity_changed (
        ::DDS::DataWriter_ptr the_Writer,
        const ::DDS::ReliableReaderActivityChangedStatus & status);
#endif

      static ::DDS::StatusMask get_mask (
        ::CCM_DDS::ConnectorStatusListener_ptr error_listener);

    private:
      virtual void on_unexpected_status (
        ::DDS::Entity_ptr entity,
        ::DDS::StatusKind status_kind);

      ::CCM_DDS::ConnectorStatusListener_var error_listener_;

      ACE_Reactor* reactor_;
    };
  }
}

#endif /* PUBLISHERLISTENER_T_H_ */

