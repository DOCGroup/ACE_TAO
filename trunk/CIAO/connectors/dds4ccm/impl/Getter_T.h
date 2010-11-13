/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef GETTER_T_H_
#define GETTER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

#include "dds4ccm/impl/ndds/Condition.h"
#include "dds4ccm/impl/ndds/ReadCondition.h"
#include "dds4ccm/impl/ndds/QueryCondition.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    /**
    * @class Getter_Base_T<DDS_TYPE,CCM_TYPE>
    *
    * @brief Templated base class for the Getter port.
    *
    *        Spec:  Get operations are performed with the following parameters
    *                . SampleStateMask: NO_READ,
    *                . ViewStateMask: NEW or NOT_NEW,
    *                . InstanceStateMask: ALIVE or NOT_ALIVE,
    *                . Through the query (if any) of the Reader associated to the port,
    *                . Within the time limit specified in time_out.
    *
    *               All methods return a boolean as result indicating whether actual data
    *               are provided (TRUE) or if the time-out occurred (FALSE).
    */
    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    class Getter_Base_T :
        public virtual GETTER_TYPE,
        public virtual LocalObject,
        private virtual ACE_Copy_Disabled
    {
    public:
      /// Constructor
      Getter_Base_T (void);

      /// Destructor
      virtual ~Getter_Base_T (void);

      /**
        *  Spec : get_many returns all the available samples
        *         in the limits set by the attribute max_delivered_data.
        *         In case there are more available samples, the first
        *         max_delivered_data are returned. The default value for
        *         that attribute is UNLIMITED (0)
        */
      virtual bool get_many (
        SEQ_VALUE_TYPE& instances,
        ::CCM_DDS::ReadInfoSeq& infos);

      /**
        * @name time_out
        * Accessors for time_out
        */
      //@{
      virtual ::DDS::Duration_t time_out (void);
      virtual void time_out (const ::DDS::Duration_t & time_out);
      //@}

      /**
        * @name max_delivered_data
        * Accessors for max_delivered_data
        */
      //@{
      virtual ::CCM_DDS::DataNumber_t max_delivered_data (void);
      virtual void max_delivered_data (
        ::CCM_DDS::DataNumber_t max_delivered_data);
      //@}

      /**
        * Set the actual pointer to DDS Datareader
        */
      void set_dds_reader (::DDS::DataReader_ptr reader,
                            ::CIAO::DDS4CCM::ConditionManager * condition_manager);
      ::DDS::DataReader_ptr get_dds_reader (void);
    protected:
        typename DDS_READER_TYPE::_var_type dds_reader_;
      ::CIAO::DDS4CCM::ConditionManager * condition_manager_;

      ::DDS::Duration_t time_out_;
      ::CCM_DDS::DataNumber_t max_delivered_data_;

      typename DDS_READER_TYPE::_ptr_type dds_reader (void);

      ::DDS::ReturnCode_t get (SEQ_VALUE_TYPE& data,
                                ::DDS::SampleInfoSeq & sample_info,
                                const ::CORBA::Long & max_samples);
      ::DDS::ReturnCode_t get (
        SEQ_VALUE_TYPE& data,
        ::DDS::SampleInfoSeq & sample_info,
        const ::CORBA::Long & max_samples,
        ::DDS::QueryCondition_ptr qc);

      ::DDS::ReturnCode_t get (
        SEQ_VALUE_TYPE & data,
        ::DDS::SampleInfoSeq & sample_info,
        const ::CORBA::Long & max_samples,
        ::DDS::ReadCondition_ptr rd);
    };

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, bool FIXED>
    class Getter_T;

    /**
    * @brief Implementation of the Getter port for variable sized data types.
    *
    */
    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    class Getter_T <GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE, false> :
      public Getter_Base_T <GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>
    {
    public:
      /**
      *
      * @brief  get_one implementation for variable sized datatypes.
      *
      *         Spec : get_one returns the next sample to be gotten.
      */
      virtual bool get_one (
        typename VALUE_TYPE::_out_type an_instance,
        ::CCM_DDS::ReadInfo_out info);
    };

    /**
    * @brief Implementation of the Getter port for fixed sized data types.
    *
    */
    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    class Getter_T <GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE, true> :
      public Getter_Base_T <GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>
    {
    public:
      /**
      * @brief  get_one implementation for fixed sized datatypes.
      *
      *         Spec : get_one returns the next sample to be gotten.
      */
      virtual bool get_one (
        typename VALUE_TYPE::_out_type an_instance,
        ::CCM_DDS::ReadInfo_out info);
    };
  }
}

#include "dds4ccm/impl/Getter_T.cpp"

#endif /* GETTER_T_H_ */
