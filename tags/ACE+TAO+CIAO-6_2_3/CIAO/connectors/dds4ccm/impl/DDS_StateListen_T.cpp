// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/DataReaderStateListener_T.h"
#include "dds4ccm/impl/StateListenerControl_T.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, CIAO::DDS4CCM::DDS4CCM_LISTENER_READ_TAKE LRT>
DDS_StateListen_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, LRT>::DDS_StateListen_T (void)
{
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, CIAO::DDS4CCM::DDS4CCM_LISTENER_READ_TAKE LRT>
DDS_StateListen_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, LRT>::~DDS_StateListen_T (void)
{
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, CIAO::DDS4CCM::DDS4CCM_LISTENER_READ_TAKE LRT>
void
DDS_StateListen_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, LRT>::set_component (
    ::CORBA::Object_ptr component)
{
  DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::set_component (component);
  this->data_control_->_set_component (component);
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, CIAO::DDS4CCM::DDS4CCM_LISTENER_READ_TAKE LRT>
void
DDS_StateListen_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, LRT>::activate (
  typename CCM_TYPE::data_listener_type::_ptr_type listener,
  ::CCM_DDS::PortStatusListener_ptr status,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_StateListen_T::activate");

  ::DDS::StatusMask const mask =
    DataReaderStateListener_type::get_mask (listener);

  if (mask != 0)
    {
      if (::CORBA::is_nil (this->listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->listener_,
                            DataReaderStateListener_type (
                              listener,
                              status,
                              this->data_control_,
                              reactor,
                              this->condition_manager_),
                            ::CORBA::NO_MEMORY ());
        }

      ::DDS::DataReader_var dr = this->dds_read_->get_dds_reader ();
      if (!::CORBA::is_nil (dr.in ()))
        {
          ::DDS::ReturnCode_t const retcode =
            dr->set_listener (this->listener_.in (), mask);

          if (retcode != ::DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_StateListen_T::activate - "
                            "Error setting the listener on the DataReader - <%C>\n",
                            ::CIAO::DDS4CCM::translate_retcode (retcode)));
              throw ::CORBA::INTERNAL ();
            }
        }
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_StateListen_T::activate - "
                        "Error while retrieving the DataReader\n"));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, CIAO::DDS4CCM::DDS4CCM_LISTENER_READ_TAKE LRT>
void
DDS_StateListen_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, LRT>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_StateListen_T::remove");

  SubscriberBase_type::remove (subscriber);
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, CIAO::DDS4CCM::DDS4CCM_LISTENER_READ_TAKE LRT>
::CCM_DDS::CCM_StateListenerControl_ptr
DDS_StateListen_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, LRT>::get_data_control (void)
{
  DDS4CCM_TRACE ("DDS_StateListen_T::get_data_control");

  return ::CCM_DDS::CCM_StateListenerControl::_duplicate (this->data_control_);
}


