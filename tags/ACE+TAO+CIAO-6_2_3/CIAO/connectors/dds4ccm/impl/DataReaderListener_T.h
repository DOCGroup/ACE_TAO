/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DATAREADERLISTENER_T_H_
#define DATAREADERLISTENER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/PortStatusListener.h"
#include "dds4ccm/impl/DataReaderHandler_T.h"
#include "dds4ccm/impl/ConditionManager.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    class DataReaderListener_T;

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    class DataReaderListenerBase_T :
      public PortStatusListener
    {
    typedef DataReaderListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>
      DataReaderListener_type;
    typedef ::CIAO::DDS4CCM::DataReaderHandler_T<DataReaderListener_type>
      DataReaderHandler_type;

    public:
      /// Constructor
      DataReaderListenerBase_T (
        typename CCM_TYPE::data_listener_type::_ptr_type listener,
        ::CCM_DDS::PortStatusListener_ptr port_status_listener,
        ::CCM_DDS::DataListenerControl_ptr control,
        ACE_Reactor * reactor,
        ConditionManager& condition_manager);

      /// Destructor
      virtual ~DataReaderListenerBase_T (void);

      virtual void on_data_available (::DDS::DataReader_ptr rdr);

      static ::DDS::StatusMask get_mask (
        typename CCM_TYPE::data_listener_type::_ptr_type listener,
        ::CCM_DDS::PortStatusListener_ptr status);

      void on_data_available_i (::DDS::DataReader_ptr rdr);

    private:
      /// Helper method to get data from DDS
      virtual ::DDS::ReturnCode_t get_data_i (
        typename TYPED_DDS_READER::_ptr_type reader,
        ::DDS::QueryCondition_ptr qc,
        SEQ_TYPE &data,
        ::DDS::SampleInfoSeq &sample_info,
        ::CORBA::Long max_samples) = 0;

      typename CCM_TYPE::data_listener_type::_var_type listener_;
      ::CCM_DDS::DataListenerControl_var control_;
      ConditionManager& condition_manager_;
    };

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE>
    class DataReaderListener_T <CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_TAKE> :
      public DataReaderListenerBase_T <CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_TAKE>
    {
    public:
      /// Constructor
      DataReaderListener_T (
        typename CCM_TYPE::data_listener_type::_ptr_type listener,
        ::CCM_DDS::PortStatusListener_ptr port_status_listener,
        ::CCM_DDS::DataListenerControl_ptr control,
        ACE_Reactor * reactor,
        ConditionManager& condition_manager) :
        DataReaderListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_TAKE> (
          listener, port_status_listener, control, reactor, condition_manager)
        {
        }
    private:
      /// Helper method to get data from DDS
      virtual ::DDS::ReturnCode_t get_data_i (
        typename TYPED_DDS_READER::_ptr_type reader,
        ::DDS::QueryCondition_ptr qc,
        SEQ_TYPE &data,
        ::DDS::SampleInfoSeq &sample_info,
        ::CORBA::Long max_samples);
    };

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE>
    class DataReaderListener_T <CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_READ> :
      public DataReaderListenerBase_T <CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_READ>
    {
    public:
      /// Constructor
      DataReaderListener_T (
        typename CCM_TYPE::data_listener_type::_ptr_type listener,
        ::CCM_DDS::PortStatusListener_ptr port_status_listener,
        ::CCM_DDS::DataListenerControl_ptr control,
        ACE_Reactor * reactor,
        ConditionManager& condition_manager) :
        DataReaderListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_READ> (
          listener, port_status_listener, control, reactor, condition_manager)
        {
        }
    private:
      /// Helper method to get data from DDS
      virtual ::DDS::ReturnCode_t get_data_i (
        typename TYPED_DDS_READER::_ptr_type reader,
        ::DDS::QueryCondition_ptr qc,
        SEQ_TYPE &data,
        ::DDS::SampleInfoSeq &sample_info,
        ::CORBA::Long max_samples);
    };
  }
}

#include "dds4ccm/impl/DataReaderListener_T.cpp"

#endif /* DATAREADERLISTENER_T_H_ */
