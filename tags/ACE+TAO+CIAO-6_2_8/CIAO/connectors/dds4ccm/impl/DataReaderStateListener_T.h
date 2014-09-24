/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
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
    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    class DataReaderStateListener_T;

    /**
     * Template implementing the DDS4CCM DataReaderStateListener basic port
     * @tparam CCM_TYPE Set of type traits for this basic port
     * @tparam TYPED_DDS_READER The typed DDS DataReader type
     * @tparam SEQ_TYPE The type of sequence
     * @tparam LRT An enum indicating the semantics of this port
     */
    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    class DataReaderStateListenerBase_T :
      public PortStatusListener
    {
    typedef DataReaderStateListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>
      DataReaderStateListener_type;
    typedef ::CIAO::DDS4CCM::DataReaderHandler_T<DataReaderStateListener_type>
      DataReaderStateHandler_type;

    public:
      /// Constructor
      DataReaderStateListenerBase_T (
        typename CCM_TYPE::data_listener_type::_ptr_type listener,
        ::CCM_DDS::PortStatusListener_ptr port_status_listener,
        ::CCM_DDS::StateListenerControl_ptr control,
        ACE_Reactor* reactor,
        ConditionManager& condition_manager);

      /// Destructor
      virtual ~DataReaderStateListenerBase_T (void);

      virtual void on_data_available (::DDS::DataReader_ptr rdr);

      static ::DDS::StatusMask get_mask (
        typename CCM_TYPE::data_listener_type::_ptr_type listener);

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
      ::CCM_DDS::StateListenerControl_var control_;
      ConditionManager& condition_manager_;
    };

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE>
    class DataReaderStateListener_T <CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_TAKE> :
      public DataReaderStateListenerBase_T <CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_TAKE>
    {
    public:
      /// Constructor
      DataReaderStateListener_T (
        typename CCM_TYPE::data_listener_type::_ptr_type listener,
        ::CCM_DDS::PortStatusListener_ptr port_status_listener,
        ::CCM_DDS::StateListenerControl_ptr control,
        ACE_Reactor* reactor,
        ConditionManager& condition_manager) :
        DataReaderStateListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_TAKE> (
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
    class DataReaderStateListener_T <CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_READ> :
      public DataReaderStateListenerBase_T <CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_READ>
    {
    public:
      /// Constructor
      DataReaderStateListener_T (
        typename CCM_TYPE::data_listener_type::_ptr_type listener,
        ::CCM_DDS::PortStatusListener_ptr port_status_listener,
        ::CCM_DDS::StateListenerControl_ptr control,
        ACE_Reactor* reactor,
        ConditionManager& condition_manager) :
        DataReaderStateListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_READ> (
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

#include "dds4ccm/impl/DataReaderStateListener_T.cpp"

#endif /* DATAREADERSTATELISTENER_T_H_ */
