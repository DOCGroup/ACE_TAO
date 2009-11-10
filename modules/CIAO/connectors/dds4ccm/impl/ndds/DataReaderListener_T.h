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
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE>
      class DataReaderListener_T :
        public ::DDS::DataReaderListener,
        private ACE_Copy_Disabled
      {
      public:
        /// Constructor
        DataReaderListener_T (
                      typename CCM_TYPE::context_type::_ptr_type context,
                      ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enabled);

        /// Destructor
        virtual ~DataReaderListener_T (void);

        virtual void on_data_available( ::DDS::DataReader *rdr);

        virtual void on_requested_deadline_missed (::DDS::DataReader_ptr the_reader,
                                 const ::DDS::RequestedDeadlineMissedStatus & status);

        virtual void on_sample_lost (::DDS::DataReader_ptr the_reader,
                                 const ::DDS::SampleLostStatus & status);

        virtual void on_requested_incompatible_qos (::DDS::DataReader_ptr the_reader,
                                 const ::DDS::RequestedIncompatibleQosStatus & status);

        bool enabled () const;
        void enabled (bool enable);

      private:
        typename CCM_TYPE::context_type::_var_type context_;

        CCM_DDS::ConnectorStatusListener_var  info_out_connector_status_;
        CCM_DDS::PortStatusListener_var       info_out_portstatus_;
        ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enabled_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/DataReaderListener_T.cpp"
