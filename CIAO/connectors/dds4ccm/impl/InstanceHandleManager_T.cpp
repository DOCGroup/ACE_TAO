// $Id$

#if (CIAO_DDS4CCM_NDDS==1)
# include "dds4ccm/impl/ndds/DataWriter_T.h"
#endif
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::InstanceHandleManager_T (void)
  : rti_entity_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::~InstanceHandleManager_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::~InstanceHandleManager_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::NDDS::DataWriter_T <DDS_TYPE> *
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::rti_entity (void)
{
  if (this->rti_entity_)
    {
      return this->rti_entity_;
    }
  else
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                    "CIAO::NDDS::DataWriter_T <DDS_TYPE>::impl - "
                    "Throwing BAD_INV_ORDER.\n"));
      throw ::CORBA::BAD_INV_ORDER ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::InstanceHandle_t
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::
  register_instance (const typename DDS_TYPE::value_type & datum)
{
  return this->rti_entity ()->register_instance (datum);
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::unregister_instance (
  const typename DDS_TYPE::value_type & datum,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  this->rti_entity ()->unregister_instance (datum, instance_handle);
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::set_rti_entity (
  ::CIAO::NDDS::DataWriter_T<DDS_TYPE> *rti_entity)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::set_impl");

 this->rti_entity_ = rti_entity;
}

