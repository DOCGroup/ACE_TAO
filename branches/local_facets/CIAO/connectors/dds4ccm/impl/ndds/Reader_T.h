/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE>
      class Reader_T :
          public virtual CCM_TYPE::reader_type,
          public virtual ::CORBA::LocalObject,
          private virtual ACE_Copy_Disabled
      {
      public:
        // Constructor
        Reader_T (::DDS::DataReader_ptr dr);

        // Destructor
        virtual ~Reader_T (void);

        virtual void read_last (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual void read_all (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual void read_one_last (
          typename DDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info,
          const ::DDS::InstanceHandle_t & instance_handle);

        virtual void read_one_all (
          const typename DDS_TYPE::value_type& an_instance,
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos,
          const ::DDS::InstanceHandle_t & instance_handle);

        virtual ::CCM_DDS::QueryFilter *filter (void);

        virtual void filter (const ::CCM_DDS::QueryFilter & filter);
      private:
        typename DDS_TYPE::data_reader *impl_;
        ::DDS::DataReader_ptr reader_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Reader_T.cpp"
