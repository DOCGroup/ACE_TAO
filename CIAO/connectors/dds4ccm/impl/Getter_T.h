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

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::GuardCondition DDSGuardCondition;
typedef ::DDS::WaitSet DDSWaitSet;
typedef ::DDS::ReadCondition DDSReadCondition;
typedef ::DDS::ConditionSeq DDSConditionSeq;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace DDS_CCM
    {

      /**
      * @class Getter_Base_T<DDS_TYPE,CCM_TYPE>
      *
      * @brief Templated base class for the Getter port.
      *
      *        Spec:  Get operations are performed with the following parameters
      *                · SampleStateMask: NO_READ,
      *                · ViewStateMask: NEW or NOT_NEW,
      *                · InstanceStateMask: ALIVE or NOT_ALIVE,
      *               · Through the query (if any) of the Reader associated to the port,
      *                · Within the time limit specified in time_out.
      *
      *               All methods return a boolean as result indicating whether actual data
      *               are provided (TRUE) or if the time-out occurred (FALSE).
      */
      template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
      class Getter_Base_T :
         public virtual CCM_TYPE::getter_type,
         public virtual ::CORBA::LocalObject,
         private virtual ACE_Copy_Disabled
      {
      typedef CIAO::DDS4CCM::CCM_DDS_ReadCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
        ReadCondition_type;
      typedef CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
        DataReader_type;
      typedef CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
        ConditionManager_type;

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
          typename CCM_TYPE::seq_type& instances,
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
        void set_impl (DataReader_type * reader,
                       ConditionManager_type * condition_manager);

        void _set_component (typename CCM_TYPE::base_type::_ptr_type component);
        virtual ::CORBA::Object_ptr _get_component (void);

      protected:
        DataReader_type * reader_;
        ConditionManager_type * condition_manager_;


        ::DDS_Duration_t time_out_;
        ::CCM_DDS::DataNumber_t max_delivered_data_;

        DataReader_type * impl (void);

        ::DDS::ReturnCode_t get (typename DDS_TYPE::dds_seq_type & data,
                                 DDS_SampleInfoSeq & sample_info,
                                 const DDS_Long & max_samples);

      private:
        typename CCM_TYPE::base_type::_var_type component_;

      };

      template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
      class Getter_T;

      /**
      * @brief Implementation of the Getter port for variable sized data types.
      *
      */
      template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
      class Getter_T <DDS_TYPE, CCM_TYPE, false, VENDOR_TYPE> :
        public Getter_Base_T <DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
      {
      public:
        /**
        *
        * @brief  get_one implementation for variable sized datatypes.
        *
        *         Spec : get_one returns the next sample to be gotten.
        */
        virtual bool get_one (
          typename DDS_TYPE::value_type::_out_type an_instance,
          ::CCM_DDS::ReadInfo_out info);
      };

      /**
      * @brief Implementation of the Getter port for fixed sized data types.
      *
      */
      template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
      class Getter_T <DDS_TYPE, CCM_TYPE, true, VENDOR_TYPE> :
        public Getter_Base_T <DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
      {
      public:
        /**
        * @brief  get_one implementation for fixed sized datatypes.
        *
        *         Spec : get_one returns the next sample to be gotten.
        */
        virtual bool get_one (
          typename DDS_TYPE::value_type::_out_type an_instance,
          ::CCM_DDS::ReadInfo_out info);
      };
    }
  }
}

#include "dds4ccm/impl/Getter_T.cpp"

#endif /* GETTER_T_H_ */
