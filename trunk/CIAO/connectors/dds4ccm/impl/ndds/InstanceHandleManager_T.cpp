// $Id$

#include "dds4ccm/impl/ndds/DataWriter.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/InstanceHandle_t.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::InstanceHandleManager_T (void)
  : impl_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::~InstanceHandleManager_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::~InstanceHandleManager_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
typename DDS_TYPE::data_writer *
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::impl (void)
{
  if (this->impl_)
    {
      return this->impl_;
    }
  else
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
::DDS::InstanceHandle_t
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::
  register_instance (const typename DDS_TYPE::value_type & datum)
{
  ::DDS::InstanceHandle_t dds_handle;
  ::DDS_InstanceHandle_t const handle = this->impl ()->register_instance (datum);
  dds_handle <<= handle;
  return dds_handle;
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
void
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::unregister_instance (
  const typename DDS_TYPE::value_type & datum,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  ::DDS_InstanceHandle_t handle = ::DDS_HANDLE_NIL;
  handle <<= instance_handle;
  this->impl ()->unregister_instance (datum, handle);
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
void
CIAO::DDS4CCM::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::set_impl (
  ::DDS::DataWriter_ptr writer)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::set_impl");

  if (::CORBA::is_nil (writer))
    {
      impl_ = 0;
    }
  else
    {
      CCM_DDS_DataWriter_i *rdw = dynamic_cast <CCM_DDS_DataWriter_i *> (writer);

      if (rdw == 0)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::InstanceHandleManager_T::data_writer - "
                       "Unable to cast provided DataWriter to servant\n"));
          throw ::CORBA::INTERNAL ();
        }

      this->impl_ =  DDS_TYPE::data_writer::narrow (rdw->get_impl ());

      if (!this->impl_)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::InstanceHandleManager_T::data_writer - "
                       "Unable to narrow the provided writer entity to the specific "
                       "type necessary to publish messages\n"));
          throw ::CORBA::INTERNAL ();
        }
    }
}

