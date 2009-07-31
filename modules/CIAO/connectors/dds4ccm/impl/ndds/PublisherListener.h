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

#include "ndds/ndds_cpp.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_PublisherListener_i :
        public virtual ::DDS::CCM_PublisherListener,
        public virtual ::CORBA::LocalObject
      {
      public:
        // Constructor
        RTI_PublisherListener_i (DDSPublisherListener *p);

        // Destructor
        virtual ~RTI_PublisherListener_i (void);

        virtual void
        on_offered_deadline_missed (
          ::DDS::DataWriter_ptr writer,
          const ::DDS::OfferedDeadlineMissedStatus & status);

        virtual void
        on_offered_incompatible_qos (
          ::DDS::DataWriter_ptr writer,
          const ::DDS::OfferedIncompatibleQosStatus & status);

        virtual void
        on_liveliness_lost (
          ::DDS::DataWriter_ptr writer,
          const ::DDS::LivelinessLostStatus & status);

        virtual void
        on_publication_matched (
          ::DDS::DataWriter_ptr writer,
          const ::DDS::PublicationMatchedStatus & status);

        DDSPublisherListener * get_publisher_listener (void);
      private:
        DDSPublisherListener *pub_list_;
      };
    }
  }
}

#endif
