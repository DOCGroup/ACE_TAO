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
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class Coherent_Write_Guard : private ACE_Copy_Disabled
      {
        public:
          Coherent_Write_Guard (DDSPublisher* p, bool coherent_write);
          ~Coherent_Write_Guard ();
        private:
          ::DDSPublisher* p_;
          bool const coherent_write_;
      };

      template <typename NDDS_TYPE, typename CCM_TYPE>
      class Writer_T :
        public virtual CCM_TYPE::writer_type,
        public virtual ::CORBA::LocalObject,
        private virtual ACE_Copy_Disabled
      {
      public:
        /// Constructor
        Writer_T (::DDS::DataWriter_ptr dw);

        /// Destructor
        virtual ~Writer_T (void);

        virtual void write (const typename NDDS_TYPE::value_type& an_instance);

        virtual void write (const typename NDDS_TYPE::seq_type& instances, bool coherent_write);
      private:
        typename NDDS_TYPE::data_writer *impl_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Writer_T.cpp"
