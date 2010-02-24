/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_PUBLISHERLISTENER_H
#define CIAO_RTI_PUBLISHERLISTENER_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
# include "ndds/ndds_cpp.h"
#endif

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::PublisherListener DDSPublisherListener;
typedef ::DDS::DataWriter DDSDataWriter;
typedef ::DDS::OfferedDeadlineMissedStatus DDS_OfferedDeadlineMissedStatus;
typedef ::DDS::OfferedIncompatibleQosStatus DDS_OfferedIncompatibleQosStatus;
typedef ::DDS::LivelinessLostStatus DDS_LivelinessLostStatus;
typedef ::DDS::PublicationMatchedStatus DDS_PublicationMatchedStatus;
#endif

#include "tao/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_PublisherListener_i :
        public virtual ::DDSPublisherListener
      {
      public:
        // Constructor
        RTI_PublisherListener_i (::DDS::PublisherListener_ptr p);

        // Destructor
        virtual ~RTI_PublisherListener_i (void);

        virtual void
        on_offered_deadline_missed (
          ::DDSDataWriter *writer,
          const ::DDS_OfferedDeadlineMissedStatus & status);

        virtual void
        on_offered_incompatible_qos (
          ::DDSDataWriter *writer,
          const ::DDS_OfferedIncompatibleQosStatus & status);

        virtual void
        on_liveliness_lost (
          ::DDSDataWriter* writer,
          const ::DDS_LivelinessLostStatus & status);

        virtual void
        on_publication_matched (
          ::DDSDataWriter *writer,
          const ::DDS_PublicationMatchedStatus & status);

        ::DDS::PublisherListener_ptr get_publisher_listener (void);
      private:
        ::DDS::PublisherListener_var impl_;
      };
    }
  }
}

#endif
