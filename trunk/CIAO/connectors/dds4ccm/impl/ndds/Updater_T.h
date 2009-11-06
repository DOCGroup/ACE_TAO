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
      template <typename NDDS_TYPE, typename CCM_TYPE>
      class Updater_T : 
        public virtual CCM_TYPE,
        public virtual ::CORBA::LocalObject
      {
      public:
        // Constructor
        Updater_T (::DDS::DataWriter_ptr dw);

        // Destructor
        virtual ~Updater_T (void);

        virtual void create (const typename NDDS_TYPE::value_type& an_instance);

        virtual void update (const typename NDDS_TYPE::value_type& an_instance);

        virtual void _cxx_delete (const typename NDDS_TYPE::value_type& an_instance);

        virtual bool is_lifecycle_checked ();

      private:
        Updater_T<NDDS_TYPE, CCM_TYPE> (const Writer_T<NDDS_TYPE, CCM_TYPE> &);
        Updater_T<NDDS_TYPE, CCM_TYPE> & operator = (const Writer_T<NDDS_TYPE, CCM_TYPE> &);
        typename NDDS_TYPE::data_writer *impl_;
        bool is_lifecycle_checked_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Updater_T.cpp"
