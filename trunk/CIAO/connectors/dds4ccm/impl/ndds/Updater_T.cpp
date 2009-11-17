// $Id$
#include "dds4ccm/impl/ndds/DataWriter.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/Coherent_Changes_Guard.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::Updater_T (::DDS::DataWriter_ptr dw)
  : InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, typename CCM_TYPE::updater_type> (dw),
    is_global_scope_ (false),
    is_coherent_write_ (false)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Updater_T::Updater_T");
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::~Updater_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Updater_T::~Updater_T");
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::is_global_scope () const
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Updater_T::is_global_scope_");
  return this->is_global_scope_;
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::is_coherent_write () const
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Updater_T::is_coherent_write");
  return this->is_coherent_write_;
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::is_coherent_write (bool value)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Updater_T::is_coherent_write");
  this->is_coherent_write_ = value;
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::create_one (
  const typename DDS_TYPE::value_type & an_instance)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Updater_T::create_one");

  DDS_InstanceHandle_t const hnd = this->impl_->lookup_instance (an_instance);
  if (!DDS_InstanceHandle_equals (&hnd, & ::DDS_HANDLE_NIL))
    {
      throw CCM_DDS::AlreadyCreated (0);
    }
  this->impl_->register_instance (an_instance);
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::update_one (
  const typename DDS_TYPE::value_type & an_instance,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Updater_T::update");

  DDS_InstanceHandle_t hnd = this->impl_->lookup_instance (an_instance);
  if (DDS_InstanceHandle_equals (&hnd, &::DDS_HANDLE_NIL))
    throw CCM_DDS::NonExistent (0);
  ::DDS::ReturnCode_t const result  = this->impl_->write (an_instance, hnd);
  if (result != DDS_RETCODE_OK)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::RTI::Updater_T::update_one : ")
                            ACE_TEXT ("Unable to update data, error %d.\n"), result));
      throw CCM_DDS::InternalError (result, 0);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::delete_one (
  const typename DDS_TYPE::value_type & an_instance,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Updater_T::delete_one");

  DDS_InstanceHandle_t hnd = this->impl_->lookup_instance (an_instance);
  if (DDS_InstanceHandle_equals (&hnd, & ::DDS_HANDLE_NIL))
    throw CCM_DDS::NonExistent (0);
  ::DDS::ReturnCode_t const result  = this->impl_->unregister_instance (an_instance, hnd);
  if (result != DDS_RETCODE_OK)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::RTI::Updater_T::delete_one ")
                            ACE_TEXT ("Unable to unregister instance, error %d.\n"), result));
      throw CCM_DDS::InternalError (result, 0);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::create_many (
  const typename CCM_TYPE::seq_type& data)
{
  Coherent_Changes_Guard guard (this->impl_->get_publisher(), this->is_coherent_write_);

  for (typename CCM_TYPE::seq_type::size_type index = 0; index < data.length (); index++)
    {
      this->create_one (data[index]);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::update_many (
  const typename CCM_TYPE::seq_type& data)
{
  Coherent_Changes_Guard guard (this->impl_->get_publisher(), this->is_coherent_write_);

  for (typename CCM_TYPE::seq_type::size_type index = 0; index < data.length (); index++)
    {
      this->update_one (data[index], ::DDS::HANDLE_NIL);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>::delete_many (
  const typename CCM_TYPE::seq_type& data)
{
  Coherent_Changes_Guard guard (this->impl_->get_publisher(), this->is_coherent_write_);

  for (typename CCM_TYPE::seq_type::size_type index = 0; index < data.length (); index++)
    {
      this->delete_one (data[index], ::DDS::HANDLE_NIL);
    }
}

