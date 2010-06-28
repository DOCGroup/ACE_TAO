/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DATAREADERLISTENER_T_H_
#define DATAREADERLISTENER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include "dds4ccm/impl/PortStatusListener_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DataReaderListener_T :
      public PortStatusListener_T <DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
    {
    public:
      /// Constructor
      DataReaderListener_T (
        typename CCM_TYPE::listener_type::_ptr_type listener,
        ::CCM_DDS::PortStatusListener_ptr port_status_listener,
        ::CCM_DDS::DataListenerControl_ptr control,
        ACE_Reactor* reactor);

      /// Destructor
      virtual ~DataReaderListener_T (void);

      virtual void on_data_available (::DDS::DataReader_ptr rdr);

      static ::DDS::StatusMask get_mask (
        ::CCM_DDS::PortStatusListener_ptr listener);

      void on_data_available_i (::DDS::DataReader_ptr rdr);

    private:
      typename CCM_TYPE::listener_type::_var_type listener_;
      ::CCM_DDS::DataListenerControl_var control_;

      typedef ::CIAO::DDS4CCM::DataReaderHandler_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> drh;
      typedef ::CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> PortStatusListener;
    };
  }
}

#include "dds4ccm/impl/DataReaderListener_T.cpp"

#endif /* DATAREADERLISTENER_T_H_ */
