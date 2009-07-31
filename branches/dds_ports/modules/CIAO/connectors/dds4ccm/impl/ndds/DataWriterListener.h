/**
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
        public virtual ::DDSDataWriterListener
      {
      public:
        // Constructor
        RTI_DataWriterListener_i (::DDS::DataWriterListener_ptr p);

        // Destructor
        virtual ~RTI_DataWriterListener_i (void);

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

        ::DDS::DataWriterListener_ptr get_datawriterlistener (void);
      private:
        ::DDS::DataWriterListener_var drl_;
      };
    }
  }
}

#endif
