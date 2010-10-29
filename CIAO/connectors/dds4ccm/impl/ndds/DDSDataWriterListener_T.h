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

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_DataWriter_T;

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_DataWriterListener_T :
      public virtual ::DDSDataWriterListener
    {
    typedef DDS_DataWriter_T<DDS_TYPE, VENDOR_TYPE> DataWriter_type;
    public:
      /// Constructor
      DDS_DataWriterListener_T (::DDS::DataWriterListener_ptr p,
                                    DataWriter_type * typed_dw);

      /// Destructor
      virtual ~DDS_DataWriterListener_T (void);

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
    private:
      ::DDS::DataWriterListener_var impl_;
      DataWriter_type * typed_dw_;
    };
  }
}

#include "dds4ccm/impl/ndds/DDSDataWriterListener_T.cpp"

#endif /* DDSDATAWRITERLISTENER_T_H_ */
