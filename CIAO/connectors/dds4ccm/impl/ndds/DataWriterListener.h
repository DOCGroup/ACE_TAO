/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDSDATAWRITERLISTENER_T_H_
#define DDSDATAWRITERLISTENER_T_H_

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_DataWriterListener_i :
      public ::DDSDataWriterListener
    {
    public:
      /// Constructor
      DDS_DataWriterListener_i (::DDS::DataWriterListener_ptr p,
                                ::DDS::DataWriter_ptr dw);

      /// Destructor
      virtual ~DDS_DataWriterListener_i (void);

      virtual void on_offered_deadline_missed (
        ::DDSDataWriter *the_writer,
        const ::DDS_OfferedDeadlineMissedStatus & status);

      virtual void on_offered_incompatible_qos (
        ::DDSDataWriter *the_writer,
        const ::DDS_OfferedIncompatibleQosStatus & status);

      virtual void on_liveliness_lost (
        ::DDSDataWriter *the_writer,
        const ::DDS_LivelinessLostStatus & status);

      virtual void on_publication_matched (
        ::DDSDataWriter *the_writer,
        const ::DDS_PublicationMatchedStatus & status);

      ::DDS::DataWriterListener_ptr get_datawriterlistener (void);

      void set_dds_entity (::DDS::DataWriter_ptr dw);

    private:
      ::DDS::DataWriterListener_var impl_;
      ::DDS::DataWriter_var dw_;
    };
  }
}

#endif /* DDSDATAWRITERLISTENER_T_H_ */
