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
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class Updater_T
      : public InstanceHandleManager_T <DDS_TYPE, CCM_TYPE, typename CCM_TYPE::updater_type, VENDOR_TYPE>
    {
    public:
      /// Constructor
      Updater_T (void);

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

      virtual void create_many (const typename CCM_TYPE::seq_type& data);

      virtual void update_many (const typename CCM_TYPE::seq_type& data);

      virtual void delete_many (const typename CCM_TYPE::seq_type& data);

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
        const typename DDS_TYPE::value_type& an_instance,
        ::CCM_DDS::DataNumber_t index);

      void update_i (
        const typename DDS_TYPE::value_type& an_instance,
        const ::DDS::InstanceHandle_t & instance_handle,
        ::CCM_DDS::DataNumber_t index);

      void delete_i (
        const typename DDS_TYPE::value_type& an_instance,
        const ::DDS::InstanceHandle_t & instance_handle,
        ::CCM_DDS::DataNumber_t index);

      void check_existent (const typename CCM_TYPE::seq_type& data);

      void check_already_created (const typename CCM_TYPE::seq_type& data);

      bool is_global_scope_;
      bool is_coherent_write_;
    };
  }
}

#include "dds4ccm/impl/Updater_T.cpp"

#endif /* UPDATER_T_H_ */
