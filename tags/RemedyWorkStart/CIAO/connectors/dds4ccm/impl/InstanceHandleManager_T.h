/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef INSTANCEHANDLEMANGER_T_H_
#define INSTANCEHANDLEMANGER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/LocalObject.h"
#include "dds4ccm/impl/LocalObject_T.h"
#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename BASE_TYPE, typename WRITER_TYPE, typename VALUE_TYPE>
    class InstanceHandleManager_T :
      public virtual BASE_TYPE,
      public virtual LocalObject,
      private virtual ACE_Copy_Disabled
    {
    public:
      /// Constructor
      InstanceHandleManager_T (void);

      /// Destructor
      virtual ~InstanceHandleManager_T (void);

      virtual ::DDS::InstanceHandle_t register_instance (
        const VALUE_TYPE& datum);

      virtual void unregister_instance (
        const VALUE_TYPE& datum,
        DDS_INSTANCE_HANDLE_T_IN instance_handle);

      void set_dds_writer (::DDS::DataWriter_ptr dds_writer);

      ::DDS::DataWriter_ptr get_dds_writer (void);

    protected:
      /// This method doesn't increment the refcount, only for internal
      /// usage
      typename WRITER_TYPE::_ptr_type dds_writer (void);

    private:
      typename WRITER_TYPE::_var_type dds_writer_;
    };
  }
}

#include "dds4ccm/impl/InstanceHandleManager_T.cpp"

#endif /* INSTANCEHANDLEMANGER_T_H_ */
