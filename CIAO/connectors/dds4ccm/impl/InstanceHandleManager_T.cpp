// $Id$

#include "dds4ccm/impl/DataWriter_T.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/ndds/InstanceHandle_t.h"
#include "dds4ccm/impl/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::InstanceHandleManager_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::~InstanceHandleManager_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::~InstanceHandleManager_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
typename DDS_TYPE::datawriter_type *
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
  ::DDS::InstanceHandle_t dds_handle;
  ::DDS_InstanceHandle_t const handle = this->impl ()->register_instance (datum);
  dds_handle <<= handle;
  return dds_handle;
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::unregister_instance (
  const typename DDS_TYPE::value_type & datum,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  ::DDS_InstanceHandle_t handle = ::DDS_HANDLE_NIL;
  handle <<= instance_handle;
  this->impl ()->unregister_instance (datum, handle);
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE, VENDOR_TYPE>::set_impl (
  CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> *writer)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::set_impl");

  if (!writer)
    {
      this->writer_ = 0;
    }
  else
    {
      this->writer_ = DDS_TYPE::datawriter_type::narrow (writer->get_impl ());
    }
}

