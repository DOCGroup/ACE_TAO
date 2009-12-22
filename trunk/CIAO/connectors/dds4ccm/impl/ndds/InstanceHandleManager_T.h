/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef INSTANCE_HANDLEMANGER_T_H
#define INSTANCE_HANDLEMANGER_T_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
      class InstanceHandleManager_T :
        public virtual BASE_TYPE,
        public virtual ::CORBA::LocalObject,
        private virtual ACE_Copy_Disabled
      {
      public:
        /// Constructor
        InstanceHandleManager_T (void);

        /// Destructor
        virtual ~InstanceHandleManager_T (void);

        virtual ::DDS::InstanceHandle_t register_instance (
          const typename DDS_TYPE::value_type & datum);

        virtual void unregister_instance (
          const typename DDS_TYPE::value_type & datum,
          const ::DDS::InstanceHandle_t & instance_handle);

        void set_impl (::DDS::DataWriter_ptr writer);

        typename DDS_TYPE::data_writer * impl (void);

      private:
        typename DDS_TYPE::data_writer *impl_;

      };
    }
  }
}

#include "dds4ccm/impl/ndds/InstanceHandleManager_T.cpp"

#endif /* INSTANCE_HANDLEMANGER_T_H */
