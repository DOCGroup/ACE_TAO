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
                      ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::ListenerMode> &mode,
                      ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::DataNumber_t> &max_delivered_data);

        /// Destructor
        virtual ~DataReaderListener_T (void);

        virtual void on_data_available( ::DDS::DataReader *rdr);

        virtual void on_requested_deadline_missed (::DDS::DataReader_ptr the_reader,
                                 const ::DDS::RequestedDeadlineMissedStatus & status);

        virtual void on_sample_lost (::DDS::DataReader_ptr the_reader,
                                 const ::DDS::SampleLostStatus & status);

        virtual void on_requested_incompatible_qos (::DDS::DataReader_ptr the_reader,
                                 const ::DDS::RequestedIncompatibleQosStatus & status);

        virtual void on_liveliness_changed(
          ::DDS::DataReader* reader,
          const ::DDS::LivelinessChangedStatus& status);

        virtual void on_sample_rejected(
          ::DDS::DataReader* reader,
          const ::DDS::SampleRejectedStatus& status);

        virtual void on_subscription_matched(
          ::DDS::DataReader* reader,
          const ::DDS::SubscriptionMatchedStatus& status);

        static ::DDS::StatusMask get_mask (void);
      private:
        typename CCM_TYPE::context_type::_var_type context_;
        ::CCM_DDS::ConnectorStatusListener_var  error_listener_;
        ::CCM_DDS::PortStatusListener_var       info_out_portstatus_;
        typename CCM_TYPE::listener_type::_var_type listener_;
        ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::ListenerMode> &mode_;
        ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::DataNumber_t> &max_delivered_data_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/DataReaderListener_T.cpp"
