/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataWriterListener_T :
      public ::DDS::DataWriterListener,
      private ACE_Copy_Disabled
    {
    public:
      // Constructor
      DataWriterListener_T (
                    typename CCM_TYPE::context_type::_ptr_type context,
                    ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enabled);

      // Destructor
      virtual ~DataWriterListener_T (void);

      virtual void on_offered_deadline_missed (
        ::DDS::DataWriter *the_writer,
        const ::DDS::OfferedDeadlineMissedStatus & status);

      virtual void on_offered_incompatible_qos (
        ::DDS::DataWriter *the_writer,
        const ::DDS::OfferedIncompatibleQosStatus & status);

      virtual void on_liveliness_lost (
        ::DDS::DataWriter *the_writer,
        const ::DDS::LivelinessLostStatus & status);

      virtual void on_publication_matched (
        ::DDS::DataWriter *the_writer,
        const ::DDS::PublicationMatchedStatus & status);

      bool enabled () const;
      void enabled (bool enable);

    private:
      typename CCM_TYPE::context_type::_var_type context_;

      CCM_DDS::ConnectorStatusListener_var  info_out_connector_status_;
      ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enable_;
    };
  }
}

#include "dds4ccm/impl/ndds/DataWriterListener_T.cpp"
