/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_PORTSTATUSLISTENER_T_H
#define DDS4CCM_PORTSTATUSLISTENER_T_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE>
      class PortStatusListener_T :
        public ::DDS::DataReaderListener,
        private ACE_Copy_Disabled
      {
      public:
        /// Constructor
        PortStatusListener_T (
          typename CCM_TYPE::context_type::_ptr_type context,
          ::CCM_DDS::PortStatusListener_ptr port_status_listener);

        /// Destructor
        virtual ~PortStatusListener_T (void);

        virtual void on_requested_deadline_missed (
          ::DDS::DataReader_ptr the_reader,
          const ::DDS::RequestedDeadlineMissedStatus & status);

        virtual void on_sample_lost (
          ::DDS::DataReader_ptr the_reader,
          const ::DDS::SampleLostStatus & status);

        static ::DDS::StatusMask get_mask (void);
      private:
        typename CCM_TYPE::context_type::_var_type context_;
        ::CCM_DDS::PortStatusListener_var port_status_listener_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/PortStatusListener_T.cpp"

#endif /* DDS4CCM_PORTSTATUSLISTENER_T_H */
