/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"
#include "InstanceHandleManager_T.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE>
      class Updater_T :
        public InstanceHandleManager_T <DDS_TYPE, CCM_TYPE, typename CCM_TYPE::updater_type>
      {
      public:
        /// Constructor
        Updater_T (::DDS::DataWriter_ptr dw);

        /// Destructor
        virtual ~Updater_T (void);

        virtual void create_one (
          const typename DDS_TYPE::value_type& an_instance);

        virtual void update_one (
          const typename DDS_TYPE::value_type& an_instance,
          const ::DDS::InstanceHandle_t & instance_handle);

        virtual void delete_one (
          const typename DDS_TYPE::value_type& an_instance,
          const ::DDS::InstanceHandle_t & instance_handle);

        virtual void create_many (
          const typename CCM_TYPE::seq_type& data);

        virtual void update_many (
          const typename CCM_TYPE::seq_type& data);

        virtual void delete_many (
          const typename CCM_TYPE::seq_type& data);

        virtual bool is_global_scope () const;

        virtual bool is_coherent_write () const;

        virtual void is_coherent_write (bool value_);

      private:
        bool is_global_scope_;
        bool is_coherent_write_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Updater_T.cpp"
