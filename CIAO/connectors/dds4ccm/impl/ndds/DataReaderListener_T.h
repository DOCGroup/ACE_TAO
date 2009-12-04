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
                      ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
                      typename CCM_TYPE::listener_type::_ptr_type listener,
                      ::CCM_DDS::PortStatusListener_ptr port_status_listener,
                      ::CCM_DDS::DataListenerControl_ptr control);

        /// Destructor
        virtual ~DataReaderListener_T (void);

        virtual void on_data_available( ::DDS::DataReader *rdr);

        virtual void on_requested_deadline_missed (::DDS::DataReader_ptr the_reader,
                                 const ::DDS::RequestedDeadlineMissedStatus & status);

        virtual void on_sample_lost (::DDS::DataReader_ptr the_reader,
                                 const ::DDS::SampleLostStatus & status);

        static ::DDS::StatusMask get_mask (void);
      private:
        typename CCM_TYPE::context_type::_var_type context_;
        ::CCM_DDS::ConnectorStatusListener_var error_listener_;
        typename CCM_TYPE::listener_type::_var_type listener_;
        ::CCM_DDS::PortStatusListener_var port_status_listener_;
        ::CCM_DDS::DataListenerControl_var control_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/DataReaderListener_T.cpp"
