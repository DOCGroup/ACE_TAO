/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef GETTER_T_H
#define GETTER_T_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE>
      class Getter_T :
         public virtual CCM_TYPE::getter_type,
         public virtual ::CORBA::LocalObject,
         private virtual ACE_Copy_Disabled
      {
      public:
        /// Constructor
        Getter_T (::DDS::DataReader_ptr dr);

        /// Destructor
        virtual ~Getter_T (void);

        virtual bool get_one (
          typename DDS_TYPE::value_type::_out_type an_instance,
          ::CCM_DDS::ReadInfo_out info);

        virtual bool get_many (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual ::DDS::Duration_t time_out (void);
        virtual void time_out (const ::DDS::Duration_t & time_out);

        virtual ::CCM_DDS::DataNumber_t max_delivered_data (void) const;
        virtual void max_delivered_data (const ::CCM_DDS::DataNumber_t & max_delivered_data);
      private:
        bool wait (DDSConditionSeq& active_conditions);

        typename DDS_TYPE::data_reader *impl_;
        DDSQueryCondition* condition_;
        ::DDS::Duration_t time_out_;
        ::CCM_DDS::DataNumber_t max_delivered_data_;
        DDSGuardCondition*  gd_;
        DDSWaitSet* ws_;
        DDSReadCondition* rd_condition_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Getter_T.cpp"

#endif /* GETTER_T_H */
