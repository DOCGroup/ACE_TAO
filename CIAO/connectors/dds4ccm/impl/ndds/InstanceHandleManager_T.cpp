// $Id$

#include "dds4ccm/impl/ndds/DataWriter.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/InstanceHandle_t.h"
#include "ciao/Logger/Log_Macros.h"

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
CIAO::DDS4CCM::RTI::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::InstanceHandleManager_T (
  ::DDS::DataWriter_ptr writer)
  : impl_ (0)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::InstanceHandleManager_T::InstanceHandleManager_T");

  RTI_DataWriter_i *rdw = dynamic_cast <RTI_DataWriter_i *> (writer);

  if (rdw == 0)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::InstanceHandleManager_T::InstanceHandleManager_T - "
                   "Unable to cast provided DataWriter to servant\n"));
      throw CORBA::INTERNAL ();
    }

  impl_ =  DDS_TYPE::data_writer::narrow (rdw->get_datawriter ());

  if (!impl_)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::InstanceHandleManager_T::InstanceHandleManager_T - "
                   "Unable to narrow the provided writer entity to the specific "
                   "type necessary to publish messages\n"));
      throw CORBA::INTERNAL ();
    }
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
CIAO::DDS4CCM::RTI::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::~InstanceHandleManager_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::InstanceHandleManager_T::~InstanceHandleManager_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
::DDS::InstanceHandle_t
CIAO::DDS4CCM::RTI::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::
  register_instance (const typename DDS_TYPE::value_type & datum)
{
  ::DDS_InstanceHandle_t const handle = this->impl_->register_instance (datum);
  ::DDS::InstanceHandle_t dds_handle;
  dds_handle <<= handle;
  return dds_handle;
}

template <typename DDS_TYPE, typename CCM_TYPE, typename BASE_TYPE>
void
CIAO::DDS4CCM::RTI::InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, BASE_TYPE>::unregister_instance (
  const typename DDS_TYPE::value_type & datum,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  ::DDS_InstanceHandle_t handle;
  handle <<= instance_handle;
  this->impl_->unregister_instance (datum, handle);
}

