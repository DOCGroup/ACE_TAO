/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
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
      class Reader_T :
        public virtual BASE,
        public virtual ::CORBA::LocalObject
      {
      public:
        // Constructor
        Reader_T (::DDS::DataReader_ptr dr);

        // Destructor
        virtual ~Reader_T (void);

        virtual void read_all (
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual void read_all_history (
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual void read_one (
          typename NDDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info);

        virtual void read_one_history (
          const typename NDDS_TYPE::value_type& an_instance,
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual ::CCM_DDS::QueryFilter *filter (void);

        virtual void filter (const ::CCM_DDS::QueryFilter & filter);
      private:
        Reader_T<NDDS_TYPE, BASE> (const Reader_T<NDDS_TYPE, BASE> &);
        Reader_T<NDDS_TYPE, BASE> & operator = (const Reader_T<NDDS_TYPE, BASE> &);
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Reader_T.cpp"
