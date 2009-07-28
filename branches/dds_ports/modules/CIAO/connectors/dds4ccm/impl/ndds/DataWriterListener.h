/**
 * @file DataWriterListener.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_DATAWRITERLISTENER_H
#define CIAO_RTI_DATAWRITERLISTENER_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_DataWriterListener_i :
        public virtual ::DDS::CCM_DataWriterListener,
        public virtual ::DDSDataWriterListener
      {
      public:
        // Constructor @todo zap default argument
        RTI_DataWriterListener_i (DDSDataWriterListener *p = 0);

        // Destructor
        virtual ~RTI_DataWriterListener_i (void);

        virtual void
        on_offered_deadline_missed (
          ::DDS::DataWriter_ptr the_writer,
          const ::DDS::OfferedDeadlineMissedStatus & status);

        virtual void
        on_offered_incompatible_qos (
          ::DDS::DataWriter_ptr the_writer,
          const ::DDS::OfferedIncompatibleQosStatus & status);

        virtual void
        on_liveliness_lost (
          ::DDS::DataWriter_ptr the_writer,
          const ::DDS::LivelinessLostStatus & status);

        virtual void
        on_publication_matched (
          ::DDS::DataWriter_ptr the_writer,
          const ::DDS::PublicationMatchedStatus & status);

        DDSDataWriterListener * get_datawriterlistener (void);
      private:
        DDSDataWriterListener *drl_;
      };
    }
  }
}

#endif
