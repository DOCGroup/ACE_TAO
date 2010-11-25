// $Id$

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/Coherent_Changes_Guard.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename WRITER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    Writer_T<WRITER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::Writer_T (void)
    : InstanceHandleManager_T<WRITER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE> (),
      is_coherent_write_ (false)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Writer_T::Writer_T");
    }

    template <typename WRITER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    Writer_T<WRITER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::~Writer_T (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Writer_T::~Writer_T");
    }

    template <typename WRITER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Writer_T<WRITER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::write_i (
      const VALUE_TYPE& datum,
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

    template <typename WRITER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Writer_T<WRITER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::write_one (
      const VALUE_TYPE & an_instance,
      const ::DDS::InstanceHandle_t & instance_handle)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Writer_T::write_one");

      this->write_i (an_instance, instance_handle, 0);

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_UNIMP_ACTION, (LM_TRACE, DDS4CCM_INFO
          "Writer_T::write_one - "
          "Write successful\n"));
    }

    template <typename WRITER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Writer_T<WRITER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::write_many (
      const SEQ_VALUE_TYPE& instances)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Writer_T::write_many");

      ::DDS::Publisher_var pub = this->dds_writer ()->get_publisher ();
      if (CORBA::is_nil (pub.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("Writer_T::write_many - ")
                        ACE_TEXT ("Publisher on DataWriter seems to be NIL\n")));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
      Coherent_Changes_Guard guard (
        pub.in (),
        this->is_coherent_write_);

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
          "Writer_T::write_many - "
          "Preparing to write to DDS\n"));

      for (::CORBA::ULong index = 0; index < instances.length(); index++)
        {
          this->write_i (instances[index], ::DDS::HANDLE_NIL, index);
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_UNIMP_ACTION, (LM_TRACE, DDS4CCM_INFO
          "Writer_T::write_many - "
          "Write successful\n"));
    }

    template <typename WRITER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    ::CORBA::Boolean
    Writer_T<WRITER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::is_coherent_write (void)
    {
      return this->is_coherent_write_;
    }

    template <typename WRITER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Writer_T<WRITER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::is_coherent_write (
        ::CORBA::Boolean value)
    {
      this->is_coherent_write_ = value;
    }
  }
}
