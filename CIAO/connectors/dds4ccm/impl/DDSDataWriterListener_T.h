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

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_NDDS==1)
# include "ndds/ndds_cpp.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DataWriterListener DDSDataWriterListener;
typedef ::DDS::DataWriter DDSDataWriter;
typedef ::DDS::OfferedDeadlineMissedStatus DDS_OfferedDeadlineMissedStatus;
typedef ::DDS::OfferedIncompatibleQosStatus DDS_OfferedIncompatibleQosStatus;
typedef ::DDS::LivelinessLostStatus DDS_LivelinessLostStatus;
typedef ::DDS::PublicationMatchedStatus DDS_PublicationMatchedStatus;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_DataWriterListener_T :
      public virtual ::DDSDataWriterListener
    {
    typedef CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE> DataWriter_type;
    public:
      /// Constructor
      CCM_DDS_DataWriterListener_T (::DDS::DataWriterListener_ptr p,
                                    DataWriter_type * typed_dw);

      /// Destructor
      virtual ~CCM_DDS_DataWriterListener_T (void);

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

#include "dds4ccm/impl/DDSDataWriterListener_T.cpp"

#endif /* DDSDATAWRITERLISTENER_T_H_ */
