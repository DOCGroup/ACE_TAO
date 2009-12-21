// $Id$

#include "dds4ccm/impl/ndds/DataWriter.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/InstanceHandle_t.h"
#include "ciao/Logger/Log_Macros.h"

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
CIAO::DDS4CCM::RTI::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::InstanceHandleManager_T (void)
  : impl_ (0)
{
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
CIAO::DDS4CCM::RTI::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::~InstanceHandleManager_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::InstanceHandleManager_T::~InstanceHandleManager_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
typename DDS_TYPE::data_writer *
CIAO::DDS4CCM::RTI::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::impl (void)
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
CIAO::DDS4CCM::RTI::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::
  register_instance (const typename DDS_TYPE::value_type & datum)
{
  ::DDS::InstanceHandle_t dds_handle;
  ::DDS_InstanceHandle_t const handle = this->impl ()->register_instance (datum);
  dds_handle <<= handle;
  return dds_handle;
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
void
CIAO::DDS4CCM::RTI::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::unregister_instance (
  const typename DDS_TYPE::value_type & datum,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  ::DDS_InstanceHandle_t handle = ::DDS_HANDLE_NIL;
  handle <<= instance_handle;
  this->impl ()->unregister_instance (datum, handle);
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
void
CIAO::DDS4CCM::RTI::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::data_writer (
  ::DDS::DataWriter_ptr writer)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::InstanceHandleManager_T::data_writer");

  if (::CORBA::is_nil (writer))
    {
      impl_ = 0;
    }
  else
    {
      RTI_DataWriter_i *rdw = dynamic_cast <RTI_DataWriter_i *> (writer);

      if (rdw == 0)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::InstanceHandleManager_T::data_writer - "
                       "Unable to cast provided DataWriter to servant\n"));
          throw ::CORBA::INTERNAL ();
        }

      impl_ =  DDS_TYPE::data_writer::narrow (rdw->get_impl ());

      if (!impl_)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::InstanceHandleManager_T::data_writer - "
                       "Unable to narrow the provided writer entity to the specific "
                       "type necessary to publish messages\n"));
          throw ::CORBA::INTERNAL ();
        }
    }
}

