/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE>
      class DataReaderListener_T :
        public virtual ::DDS::DataReaderListener
      {
      public:
        // Constructor
        DataReaderListener_T (
                      typename CCM_TYPE::context_type::_ptr_type context,
                      ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enabled);

        // Destructor
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
        DataReaderListener_T<DDS_TYPE, CCM_TYPE> (const DataReaderListener_T<DDS_TYPE, CCM_TYPE> &);
        DataReaderListener_T<DDS_TYPE, CCM_TYPE> & operator = (const DataReaderListener_T<DDS_TYPE, CCM_TYPE> &);
  
        typename CCM_TYPE::context_type::_var_type context_;

        CCM_DDS::ConnectorStatusListener_var  info_out_connector_status_;
        CCM_DDS::PortStatusListener_var       info_out_portstatus_;
        typename CCM_TYPE::rawlistener_type::_var_type listener_;
        ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enable_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/DataReaderListener_T.cpp"
