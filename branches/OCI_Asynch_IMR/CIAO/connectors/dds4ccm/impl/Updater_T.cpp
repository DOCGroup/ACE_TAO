// $Id$

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/Coherent_Changes_Guard.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::Updater_T (void)
    : InstanceHandleManager_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE> (),
      is_global_scope_ (false),
      is_coherent_write_ (false)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::Updater_T");
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::~Updater_T (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::~Updater_T");
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    bool
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::is_global_scope (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::is_global_scope");
      return this->is_global_scope_;
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    bool
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::is_coherent_write (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::is_coherent_write");
      return this->is_coherent_write_;
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::is_coherent_write (bool value)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::is_coherent_write");
      this->is_coherent_write_ = value;
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::create_i (
      const VALUE_TYPE& an_instance,
      ::CCM_DDS::DataNumber_t index)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::create_i");
      ::DDS::InstanceHandle_t const hnd =
          this->dds_writer ()->register_instance (an_instance);
      if (hnd == ::DDS::HANDLE_NIL)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
              ACE_TEXT ("Updater_T::create_i: ")
          ACE_TEXT ("Unable to unregister instance, nil handle.\n")));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, index);
        }
      ::DDS::ReturnCode_t const result  = this->dds_writer ()->write (an_instance, hnd);
      if (result != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("Updater_T::create_i: ")
            ACE_TEXT ("Unable to update data, error %C.\n"),
          translate_retcode (result)));
          throw ::CCM_DDS::InternalError (result, index);
        }
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::create_one (
      const VALUE_TYPE& an_instance)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::create_one");
      ::DDS::InstanceHandle_t const hnd = this->dds_writer ()->lookup_instance (an_instance);
      if (hnd != ::DDS::HANDLE_NIL)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
              ACE_TEXT ("Updater_T::create_one: ")
              ACE_TEXT ("Instance already registered with handle")
              DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER
              ACE_TEXT (".\n"),
              DDS_INSTANCE_HANDLE_LOG (hnd)));
          throw CCM_DDS::AlreadyCreated (0);
        }

      this->create_i (an_instance, 0);
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::update_i (
      const VALUE_TYPE& an_instance,
      DDS_INSTANCE_HANDLE_T_IN instance_handle,
      ::CCM_DDS::DataNumber_t index)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::update_i");
      ::DDS::ReturnCode_t const result  =
          this->dds_writer ()->write (an_instance, instance_handle);
      if (result != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
              ACE_TEXT ("Updater_T::update_i: ")
          ACE_TEXT ("Unable to update data, error %C.\n"),
          translate_retcode (result)));
          throw ::CCM_DDS::InternalError (result, index);
        }
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::update_one (
      const VALUE_TYPE& an_instance,
      DDS_INSTANCE_HANDLE_T_IN instance_handle)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::update_one");
      ::DDS::InstanceHandle_t hnd = instance_handle;
      if (hnd == ::DDS::HANDLE_NIL)
        {
          hnd = this->dds_writer ()->lookup_instance (an_instance);
        }
      else
        {
          // Check explicitly if the instance handle matches the instance, this
          // is not checked by RTI DDS
          ::DDS::InstanceHandle_t const instance_handle_l =
              this->dds_writer ()->lookup_instance (an_instance);

          if (hnd != instance_handle_l)
            {
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
            }
        }
      if (hnd == ::DDS::HANDLE_NIL)
        {
          throw CCM_DDS::NonExistent (0);
        }
      this->update_i (an_instance, instance_handle, 0);
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::delete_i (
      const VALUE_TYPE& an_instance,
      DDS_INSTANCE_HANDLE_T_IN instance_handle,
      ::CCM_DDS::DataNumber_t index)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::delete_i");

      ::DDS::InstanceHandle_t hnd = instance_handle;
      if (instance_handle == ::DDS::HANDLE_NIL)
        {
          hnd = this->dds_writer ()->lookup_instance (an_instance);
        }

      ::DDS::ReturnCode_t result =
          this->dds_writer ()->dispose (an_instance, hnd);
      if (result != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
              ACE_TEXT ("Updater_T::delete_i: ")
          ACE_TEXT ("Unable to dispose instance, error %C.\n"),
          translate_retcode (result)));
          throw ::CCM_DDS::InternalError (result, index);
        }
      result = this->dds_writer ()->unregister_instance (an_instance, hnd);
      if (result != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
              ACE_TEXT ("Updater_T::delete_i: ")
          ACE_TEXT ("Unable to unregister instance, error %C.\n"),
          translate_retcode (result)));
          throw ::CCM_DDS::InternalError (result, index);
        }
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::delete_one (
      const VALUE_TYPE& an_instance,
      DDS_INSTANCE_HANDLE_T_IN instance_handle)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::delete_one");
      ::DDS::InstanceHandle_t hnd = instance_handle;
      if (instance_handle == ::DDS::HANDLE_NIL)
        {
          hnd = this->dds_writer ()->lookup_instance (an_instance);
        }
      else
        {
          // Check explicitly if the instance handle matches the instance, this
          // is not checked by RTI DDS
          ::DDS::InstanceHandle_t const l_instance_handle =
              this->dds_writer ()->lookup_instance (an_instance);

          if (hnd != l_instance_handle)
            {
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
            }
        }
      if (hnd == ::DDS::HANDLE_NIL)
        {
          throw CCM_DDS::NonExistent (0);
        }
      this->delete_i (an_instance, hnd, 0);
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::create_many (
      const SEQ_VALUE_TYPE& data)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::create_many");

      // Check for existence of instances
      this->check_already_created (data);

      ::DDS::Publisher_var pub = this->dds_writer ()->get_publisher();
      Coherent_Changes_Guard guard (pub.in (), this->is_coherent_write_);

      for (::CORBA::ULong index = 0; index < data.length (); index++)
        {
          this->create_i (data[index], index);
        }
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::check_existent (
        const SEQ_VALUE_TYPE& data)
    {
      ::CCM_DDS::NonExistent exception;
      for (::CORBA::ULong index = 0; index < data.length (); index++)
        {
          ::DDS::InstanceHandle_t const hnd =
              this->dds_writer ()->lookup_instance (data[index]);
          if (hnd == ::DDS::HANDLE_NIL)
            {
              CORBA::ULong const length = exception.indexes.length ();
              exception.indexes.length (length + 1);
              exception.indexes[length] = index;
            }
        }

      if (exception.indexes.length () > 0)
        {
          throw exception;
        }
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::check_already_created (
      const SEQ_VALUE_TYPE& data)
    {
      ::CCM_DDS::AlreadyCreated exception;
      for (::CORBA::ULong index = 0; index < data.length (); index++)
        {
          ::DDS::InstanceHandle_t const hnd =
              this->dds_writer ()->lookup_instance (data[index]);
          if (hnd != ::DDS::HANDLE_NIL)
            {
              ::CORBA::ULong const length = exception.indexes.length ();
              exception.indexes.length (length + 1);
              exception.indexes[length] = index;
            }
        }

      if (exception.indexes.length () > 0)
        {
          throw exception;
        }
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::update_many (
        const SEQ_VALUE_TYPE& data)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::update_many");

      // Check for existence of instances
      this->check_existent (data);

      ::DDS::Publisher_var pub = this->dds_writer ()->get_publisher();
      Coherent_Changes_Guard guard (pub.in (), this->is_coherent_write_);

      for (::CORBA::ULong index = 0; index < data.length (); index++)
        {
          this->update_i (data[index], ::DDS::HANDLE_NIL, index);
        }
    }

    template <typename UPDATER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Updater_T<UPDATER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE, SEQ_VALUE_TYPE>::delete_many (
      const SEQ_VALUE_TYPE& data)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::Updater_T::delete_many");

      // Check for existence of instances
      this->check_existent (data);

      ::DDS::Publisher_var pub = this->dds_writer ()->get_publisher();
      Coherent_Changes_Guard guard (pub.in (), this->is_coherent_write_);

      for (::CORBA::ULong index = 0; index < data.length (); index++)
        {
          this->delete_i (data[index], ::DDS::HANDLE_NIL, index);
        }
    }
  }
}
