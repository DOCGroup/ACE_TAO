/**
 * @author Marcel Smit <msmit@remedy.nl>
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
      class Updater_T :
        public virtual CCM_TYPE::updater_type,
        public virtual ::CORBA::LocalObject,
        private ACE_Copy_Disabled
      {
      public:
        // Constructor
        Updater_T (::DDS::DataWriter_ptr dw);

        // Destructor
        virtual ~Updater_T (void);

        virtual void create (const typename DDS_TYPE::value_type& an_instance);

        virtual void update (const typename DDS_TYPE::value_type& an_instance);

        virtual void _cxx_delete (const typename DDS_TYPE::value_type& an_instance);

        virtual bool is_lifecycle_checked ();

      private:
        typename DDS_TYPE::data_writer *impl_;
        bool is_lifecycle_checked_;
        bool is_coherent_write_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Updater_T.cpp"
