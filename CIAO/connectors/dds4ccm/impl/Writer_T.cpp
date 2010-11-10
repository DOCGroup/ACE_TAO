// $Id$

#if (CIAO_DDS4CCM_NDDS==1)
#include "dds4ccm/impl/ndds/DataWriter_T.h"
#endif

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/Coherent_Changes_Guard.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::Writer_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::Writer_T (void)
  : InstanceHandleManager_T<DDS_TYPE, CCM_TYPE, typename CCM_TYPE::writer_type, VENDOR_TYPE> (),
    is_coherent_write_ (false)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::Writer_T::Writer_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::Writer_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~Writer_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::Writer_T::~Writer_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::Writer_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::write_i (
  const typename DDS_TYPE::value_type& datum,
  const ::DDS::InstanceHandle_t& instance_handle,
  ::CCM_DDS::DataNumber_t index)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::Writer_T::write_i");

  ::DDS::ReturnCode_t const retval = this->dds_writer ()->write (datum, instance_handle);

  if (retval != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "Writer_T::write_i - "
                    "Write unsuccessful, received error code %C\n",
                    translate_retcode (retval)));
      throw ::CCM_DDS::InternalError (retval, index);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::Writer_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::write_one (
  const typename DDS_TYPE::value_type & an_instance,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::Writer_T::write_one");

  this->write_i (an_instance, instance_handle, 0);

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_UNIMP_ACTION, (LM_TRACE, DDS4CCM_INFO
                "Writer_T::write_one - "
                "Write successful\n"));
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::Writer_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::write_many (
  const typename CCM_TYPE::seq_type& instances)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::Writer_T::write_many");

  Coherent_Changes_Guard guard (this->dds_writer ()->get_publisher(),
                                this->is_coherent_write_);

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                "Writer_T::write_many - "
                "Preparing to write to DDS\n"));

  for (typename CCM_TYPE::seq_type::size_type index = 0;
       index < instances.length();
       index++)
    {
      this->write_i (instances[index], ::DDS::HANDLE_NIL, index);
    }

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_UNIMP_ACTION, (LM_TRACE, DDS4CCM_INFO
                "Writer_T::write_many - "
                "Write successful\n"));
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::CORBA::Boolean
CIAO::DDS4CCM::Writer_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::is_coherent_write (void)
{
  return this->is_coherent_write_;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::Writer_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::is_coherent_write (
  ::CORBA::Boolean value)
{
  this->is_coherent_write_ = value;
}
