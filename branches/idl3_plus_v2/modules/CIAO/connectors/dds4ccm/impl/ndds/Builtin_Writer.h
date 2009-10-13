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
      class Builtin_Writer_T 
      {
      public:
        // Constructor
        Builtin_Writer_T (::DDS::DataWriter_ptr dw);

        // Destructor
        virtual ~Builtin_Writer_T (void);

        virtual void write (const typename NDDS_TYPE::value_type& an_instance);

        virtual void write (const typename NDDS_TYPE::seq_type& instances, bool coherent_write);
      private:
        Builtin_Writer_T<NDDS_TYPE, BASE> (const Builtin_Writer_T<NDDS_TYPE, BASE> &);
        Builtin_Writer_T<NDDS_TYPE, BASE> & operator = (const Builtin_Writer_T<NDDS_TYPE, BASE> &);
        typename NDDS_TYPE::data_writer *impl_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Builtin_Writer.tpp"
