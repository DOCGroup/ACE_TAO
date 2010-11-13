/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DATAREADERSTATELISTENER_T_H_
#define DATAREADERSTATELISTENER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/PortStatusListener.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataReaderStateListener_T :
      public PortStatusListener
    {
    typedef ::CIAO::DDS4CCM::DataReaderStateHandler_T<DDS_TYPE, CCM_TYPE>
      DataReaderStateHandler_type;

    public:
      /// Constructor
      DataReaderStateListener_T (
                    typename CCM_TYPE::data_listener_type::_ptr_type listener,
                    ::CCM_DDS::PortStatusListener_ptr port_status_listener,
                    ::CCM_DDS::StateListenerControl_ptr control,
                    ACE_Reactor* reactor,
                    ConditionManager& condition_manager);

      /// Destructor
      virtual ~DataReaderStateListener_T (void);

      virtual void on_data_available (::DDS::DataReader_ptr rdr);

      static ::DDS::StatusMask get_mask (
        typename CCM_TYPE::data_listener_type::_ptr_type listener);

      void on_data_available_i (::DDS::DataReader_ptr rdr);

    private:
      typename CCM_TYPE::data_listener_type::_var_type listener_;
      ::CCM_DDS::StateListenerControl_var control_;
      ConditionManager& condition_manager_;
    };
  }
}

#include "dds4ccm/impl/DataReaderStateListener_T.cpp"

#endif /* DATAREADERSTATELISTENER_T_H_ */
