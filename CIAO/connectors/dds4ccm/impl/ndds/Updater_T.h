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
      class Updater_T : 
        public virtual BASE,
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
        Updater_T<NDDS_TYPE, BASE> (const Writer_T<NDDS_TYPE, BASE> &);
        Updater_T<NDDS_TYPE, BASE> & operator = (const Writer_T<NDDS_TYPE, BASE> &);
        typename NDDS_TYPE::data_writer *impl_;
        bool is_lifecycle_checked_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Updater_T.cpp"
