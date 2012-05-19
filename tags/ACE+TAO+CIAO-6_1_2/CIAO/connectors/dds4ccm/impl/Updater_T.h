/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef UPDATER_T_H_
#define UPDATER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"
#include "InstanceHandleManager_T.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    class Updater_T
      : public InstanceHandleManager_T <UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE>
    {
    public:
      /// Constructor
      Updater_T (void);

      /// Destructor
      virtual ~Updater_T (void);

      virtual void create_one (
        const VALUE_TYPE& an_instance);

      virtual void update_one (
        const VALUE_TYPE& an_instance,
        DDS_INSTANCE_HANDLE_T_IN instance_handle);

      virtual void delete_one (
        const VALUE_TYPE& an_instance,
        DDS_INSTANCE_HANDLE_T_IN instance_handle);

      virtual void create_many (const SEQ_VALUE_TYPE& data);

      virtual void update_many (const SEQ_VALUE_TYPE& data);

      virtual void delete_many (const SEQ_VALUE_TYPE& data);

      virtual bool is_global_scope ();

      /**
       * @name is_coherent_write
       * Accessors for is_coherent_write
       */
      //@{
      virtual bool is_coherent_write ();

      virtual void is_coherent_write (bool value_);
      //@}

    private:
      void create_i (
        const VALUE_TYPE& an_instance,
        ::CCM_DDS::DataNumber_t index);

      void update_i (
        const VALUE_TYPE& an_instance,
        DDS_INSTANCE_HANDLE_T_IN instance_handle,
        ::CCM_DDS::DataNumber_t index);

      void delete_i (
        const VALUE_TYPE& an_instance,
        DDS_INSTANCE_HANDLE_T_IN instance_handle,
        ::CCM_DDS::DataNumber_t index);

      void check_existent (const SEQ_VALUE_TYPE& data);

      void check_already_created (const SEQ_VALUE_TYPE& data);

      bool is_global_scope_;
      bool is_coherent_write_;
    };
  }
}

#include "dds4ccm/impl/Updater_T.cpp"

#endif /* UPDATER_T_H_ */
