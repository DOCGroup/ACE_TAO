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

        virtual void write (const typename DDS_TYPE::value_type& an_instance);

        virtual void write_many (const typename CCM_TYPE::seq_type& instances);
        
        virtual ::DDS::InstanceHandle_t register_instance (const typename DDS_TYPE::value_type & datum);
    
        virtual void unregister_instance (const typename DDS_TYPE::value_type & datum, const ::DDS::InstanceHandle_t & instance_handle);

      private:
        typename DDS_TYPE::data_writer *impl_;
        bool is_coherent_write_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Writer_T.cpp"
