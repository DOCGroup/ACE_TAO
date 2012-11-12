/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDSPUBLISHERLISTENER_T_H_
#define DDSPUBLISHERLISTENER_T_H_

#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_PublisherListener_i :
      public ::DDSPublisherListener
    {
    public:
      /// Constructor
      DDS_PublisherListener_i (
        ::DDS::PublisherListener_ptr p,
        ::DDS::DomainParticipant_ptr dp);

      /// Destructor
      virtual ~DDS_PublisherListener_i (void);

      virtual void
      on_offered_deadline_missed (
        ::DDSDataWriter *the_writer,
        const ::DDS_OfferedDeadlineMissedStatus & status);

      virtual void
      on_offered_incompatible_qos (
        ::DDSDataWriter *the_writer,
        const ::DDS_OfferedIncompatibleQosStatus & status);

      virtual void
      on_liveliness_lost (
        ::DDSDataWriter *the_writer,
        const ::DDS_LivelinessLostStatus & status);

      virtual void
      on_publication_matched (
        ::DDSDataWriter *the_writer,
        const ::DDS_PublicationMatchedStatus & status);

      virtual void
      on_reliable_reader_activity_changed (
        ::DDSDataWriter *the_writer,
        const ::DDS_ReliableReaderActivityChangedStatus & status);

      virtual void
      on_reliable_writer_cache_changed (
        ::DDSDataWriter *the_writer,
        const ::DDS_ReliableWriterCacheChangedStatus & status);

      ::DDS::PublisherListener_ptr get_publisher_listener (void);
    private:
      ::DDS::PublisherListener_var impl_;
      ::DDS::DomainParticipant_var dp_;
    };
  }
}

#endif /* DDSPUBLISHERLISTENER_T_H_ */
