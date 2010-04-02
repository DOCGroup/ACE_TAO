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

#include "dds4ccm/impl/dds/PortStatusListener_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataReaderListener_T :
      public PortStatusListener_T <DDS_TYPE, CCM_TYPE>
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

      virtual void on_data_available( ::DDS::DataReader *rdr);

      static ::DDS::StatusMask get_mask (
        typename CCM_TYPE::listener_type::_ptr_type listener);

      void on_data_available_i ( ::DDS::DataReader *rdr);

    private:
      typename CCM_TYPE::listener_type::_var_type listener_;
      ::CCM_DDS::DataListenerControl_var control_;

      typedef ::CIAO::DDS4CCM::DataReaderHandler_T<DDS_TYPE, CCM_TYPE> drh;
    };
  }
}

#include "dds4ccm/impl/dds/DataReaderListener_T.cpp"

#endif /* DDS4CCM_DATAREADERLISTENER_T_H */
