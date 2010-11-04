// $Id$

#include "dds4ccm/impl/ndds/DataWriter_T.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::InstanceHandleManager_T (void)
  : writer_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::~InstanceHandleManager_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::~InstanceHandleManager_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DDS_DataWriter_T <DDS_TYPE> *
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::impl (void)
{
  if (this->writer_)
    {
      return this->writer_;
    }
  else
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::InstanceHandle_t
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::
  register_instance (const typename DDS_TYPE::value_type & datum)
{
  return this->impl ()->register_instance (datum);
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::unregister_instance (
  const typename DDS_TYPE::value_type & datum,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  this->impl ()->unregister_instance (datum, instance_handle);
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::set_impl (
  DDS_DataWriter_T<DDS_TYPE> *writer)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::set_impl");

 this->writer_ = writer;
}

