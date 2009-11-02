/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename NDDS_TYPE, typename BASE>
      class Getter_T :
         public virtual BASE,
         public virtual ::CORBA::LocalObject
      {
      public:
        // Constructor
        Getter_T (::DDS::DataReader_ptr dr);

        // Destructor
        virtual ~Getter_T (void);

        virtual bool get_all (
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual bool get_all_history (
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual bool get_one (
          typename NDDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info);

        virtual bool get_one_history (
          const typename NDDS_TYPE::value_type& an_instance,
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual bool get_next (
          typename NDDS_TYPE::value_type::_out_type  an_instance,
          ::CCM_DDS::ReadInfo_out info);

        virtual ::CCM_DDS::QueryFilter *filter (void);
        virtual void filter (const ::CCM_DDS::QueryFilter & filter);

        virtual ::DDS::Duration_t time_out (void);
        virtual void time_out (const ::DDS::Duration_t & time_out);
      private:
        Getter_T<NDDS_TYPE, BASE> (const Getter_T<NDDS_TYPE, BASE> &);
        Getter_T<NDDS_TYPE, BASE> & operator = (const Getter_T<NDDS_TYPE, BASE> &);
        typename NDDS_TYPE::data_reader *impl_;
        DDSQueryCondition* condition_;
        ::DDS::Duration_t time_out_;
        DDSGuardCondition*  gd_;
        DDSWaitSet* ws_;
        DDSReadCondition* rd_condition_;

        bool
        wait (
          DDSConditionSeq& active_conditions);
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Getter_T.cpp"
