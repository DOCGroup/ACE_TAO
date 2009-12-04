/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DATAREADERLISTENER_T_H
#define DDS4CCM_DATAREADERLISTENER_T_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/ndds/PortStatusListener_T.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE>
      class DataReaderListener_T :
        public PortStatusListener_T <DDS_TYPE, CCM_TYPE>
      {
      public:
        /// Constructor
        DataReaderListener_T (
          typename CCM_TYPE::context_type::_ptr_type context,
          typename CCM_TYPE::listener_type::_ptr_type listener,
          ::CCM_DDS::PortStatusListener_ptr port_status_listener,
          ::CCM_DDS::DataListenerControl_ptr control);

        /// Destructor
        virtual ~DataReaderListener_T (void);

        virtual void on_data_available( ::DDS::DataReader *rdr);

        static ::DDS::StatusMask get_mask (void);
      private:
        typename CCM_TYPE::listener_type::_var_type listener_;
        ::CCM_DDS::DataListenerControl_var control_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/DataReaderListener_T.cpp"

#endif /* DDS4CCM_DATAREADERLISTENER_T_H */
