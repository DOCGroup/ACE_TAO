// $Id$

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "CCM_DataReader.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::InstanceHandleManager_T (void)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::~InstanceHandleManager_T (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::~InstanceHandleManager_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    typename DDS_TYPE::typed_writer_type::_ptr_type
    InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::dds_writer (void)
    {
      if (!::CORBA::is_nil (this->dds_writer_.in ()))
        {
          return this->dds_writer_.in ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "CIAO::NDDS::DataWriter_T <DDS_TYPE>::dds_writer - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
    }

    template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::InstanceHandle_t
    InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::
      register_instance (const typename DDS_TYPE::value_type & datum)
    {
      return this->dds_writer ()->register_instance (datum);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::unregister_instance (
      const typename DDS_TYPE::value_type & datum,
      const ::DDS::InstanceHandle_t & instance_handle)
    {
      this->dds_writer ()->unregister_instance (datum, instance_handle);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::set_dds_writer (
      ::DDS::DataWriter_ptr dds_writer)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::set_dds_writer");

      this->dds_writer_ = DDS_TYPE::typed_writer_type::_narrow (dds_writer);
    }
  }
}
