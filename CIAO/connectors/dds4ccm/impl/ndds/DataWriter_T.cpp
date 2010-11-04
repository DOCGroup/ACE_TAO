// $Id$

#include "dds4ccm/impl/ndds/DataWriter_T.h"
#include "dds4ccm/impl/ndds/DataWriter.h"
#include "dds4ccm/impl/ndds/StatusCondition.h"
#include "dds4ccm/impl/ndds/Publisher_T.h"
#include "dds4ccm/impl/ndds/DDSDataWriterListener_T.h"
#include "dds4ccm/impl/ndds/Topic_T.h"

#include "dds4ccm/impl/ndds/convertors/Duration_t.h"
#include "dds4ccm/impl/ndds/convertors/InstanceHandle_t.h"
#include "dds4ccm/impl/ndds/convertors/PublicationMatchedStatus.h"
#include "dds4ccm/impl/ndds/convertors/LivelinessLostStatus.h"
#include "dds4ccm/impl/ndds/convertors/OfferedIncompatibleQosStatus.h"
#include "dds4ccm/impl/ndds/convertors/OfferedDeadlineMissedStatus.h"
#include "dds4ccm/impl/ndds/convertors/InstanceHandleSeq.h"
#include "dds4ccm/impl/ndds/convertors/DataWriterQos.h"
#include "dds4ccm/impl/ndds/convertors/SubscriptionBuiltinTopicData.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE>
    DDS_DataWriter_T<DDS_TYPE>::DDS_DataWriter_T (DDSDataWriter * dw)
      : DDS_DataWriter_Base (dw),
        writer_ (0)
    {
      if (dw)
        {
          this->writer_ = DDS_TYPE::datawriter_type::narrow (dw);
        }
    }

    template <typename DDS_TYPE>
    void
    DDS_DataWriter_T<DDS_TYPE>::set_impl (DDSDataWriter * dw)
    {
      if (dw)
        {
          this->writer_ = DDS_TYPE::datawriter_type::narrow (dw);
        }
      DDS_DataWriter_Base::set_impl (dw);
    }

    template <typename DDS_TYPE>
    typename DDS_TYPE::datawriter_type*
    DDS_DataWriter_T<DDS_TYPE>::typed_impl (void)
    {
      if (!this->writer_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->writer_;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::set_qos (const ::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DataWriter_T<DDS_TYPE>::set_qos");
      ::DDS_DataWriterQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->typed_impl()->set_qos (ccm_dds_qos);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::get_qos (::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DataWriter_T<DDS_TYPE>::get_qos");
      ::DDS_DataWriterQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      ::DDS::ReturnCode_t retcode = this->typed_impl ()->get_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::set_listener (
      ::DDS::DataWriterListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      // Delete the previously set listener
      DDSDataWriterListener *listener = this->typed_impl ()->get_listener ();
      delete listener;

      DataWriterListener_type * ccm_dds_impl_list = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            DataWriterListener_type (a_listener, this),
                            ::CORBA::NO_MEMORY ());
        }
      return this->typed_impl ()->set_listener (ccm_dds_impl_list, mask);
    }

    template <typename DDS_TYPE>
    ::DDS::DataWriterListener_ptr
    DDS_DataWriter_T<DDS_TYPE>::get_listener (void)
    {
      DDSDataWriterListener *wr = this->typed_impl ()->get_listener ();
      DataWriterListener_type * list_proxy =
        dynamic_cast <DataWriterListener_type *> (wr);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DDS_DataWriter_T<DDS_TYPE>::get_listener - "
                        "DDS returned a NIL listener.\n"));
          return ::DDS::DataWriterListener::_nil ();
        }
      return list_proxy->get_datawriterlistener ();
    }

    template <typename DDS_TYPE>
    ::DDS::Topic_ptr
    DDS_DataWriter_T<DDS_TYPE>::get_topic (void)
    {
      ::DDS::Topic_var retval;
      DDSTopic* t = this->typed_impl ()->get_topic ();
      ACE_NEW_THROW_EX (retval,
                        Topic_type (t),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE>
    ::DDS::Publisher_ptr
    DDS_DataWriter_T<DDS_TYPE>::get_publisher (void)
    {
      ::DDS::Publisher_var retval;
      DDSPublisher* p = this->typed_impl ()->get_publisher ();
      ACE_NEW_THROW_EX (retval,
                        Publisher_type (p),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
    {
     ::DDS_Duration_t rtiduration;
     rtiduration <<= max_wait;
     return this->typed_impl ()->wait_for_acknowledgments (rtiduration);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::get_liveliness_lost_status (::DDS::LivelinessLostStatus & status)
    {
      ::DDS_LivelinessLostStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval =
        this->typed_impl ()->get_liveliness_lost_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::get_offered_deadline_missed_status (
      ::DDS::OfferedDeadlineMissedStatus & status)
    {
      ::DDS_OfferedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval =
        this->typed_impl ()->get_offered_deadline_missed_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::get_offered_incompatible_qos_status (
      ::DDS::OfferedIncompatibleQosStatus & status)
    {
      ::DDS_OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval =
        this->typed_impl ()->get_offered_incompatible_qos_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::get_publication_matched_status (
      ::DDS::PublicationMatchedStatus & status)
    {
      ::DDS_PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval =
        this->typed_impl ()->get_publication_matched_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::assert_liveliness (void)
    {
      return this->typed_impl ()->assert_liveliness ();
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::get_matched_subscriptions (
      ::DDS::InstanceHandleSeq & subscription_handles)
    {
      ::DDS_InstanceHandleSeq rtiseq;
      rtiseq <<= subscription_handles;
      ::DDS::ReturnCode_t const retval =
        this->typed_impl ()->get_matched_subscriptions (rtiseq);
      subscription_handles <<= rtiseq;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::get_matched_subscription_data (
      ::DDS::SubscriptionBuiltinTopicData & subscription_data,
      DDS_INSTANCE_HANDLE_T_IN subscription_handle)
    {
      DDS4CCM_TRACE ("DDS_DataWriter_T<DDS_TYPE>::get_matched_subscription_data");
      ::DDS_SubscriptionBuiltinTopicData ccm_dds_sub_data;
      ::DDS_InstanceHandle_t ccm_dds_sub_handle;
      ccm_dds_sub_handle <<= subscription_handle;
      ::DDS::ReturnCode_t const retval =
        this->typed_impl ()->get_matched_subscription_data (ccm_dds_sub_data, ccm_dds_sub_handle);
      subscription_data <<= ccm_dds_sub_data;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::enable (void)
    {
      return this->typed_impl ()->enable ();
    }

    template <typename DDS_TYPE>
    ::DDS::StatusCondition_ptr
    DDS_DataWriter_T<DDS_TYPE>::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval;
      DDSStatusCondition* sc = this->typed_impl ()->get_statuscondition ();
      ACE_NEW_THROW_EX (retval,
                        DDS_StatusCondition_i (sc),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE>
    ::DDS::StatusMask
    DDS_DataWriter_T<DDS_TYPE>::get_status_changes (void)
    {
      return this->typed_impl ()->get_status_changes ();
    }

    template <typename DDS_TYPE>
    DDS_INSTANCE_HANDLE_T_RETN
    DDS_DataWriter_T<DDS_TYPE>::get_instance_handle (void)
    {
      ::DDS_InstanceHandle_t const rtihandle =
        this->typed_impl ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }

    template <typename DDS_TYPE>
    ::DDS::InstanceHandle_t
    DDS_DataWriter_T<DDS_TYPE>::register_instance (
        const typename DDS_TYPE::value_type& instance_data)
    {
      ::DDS::InstanceHandle_t dds_handle;
      ::DDS_InstanceHandle_t const handle = this->typed_impl ()->register_instance (instance_data);
      dds_handle <<= handle;
      return dds_handle;
    }

    template <typename DDS_TYPE>
    ::DDS::InstanceHandle_t
    DDS_DataWriter_T<DDS_TYPE>::register_instance_w_timestamp (
        const typename  DDS_TYPE::value_type & instance_data,
        const ::DDS::Time_t & source_timestamp)
    {
      ::DDS_Time_t time;
      time <<= source_timestamp;
      ::DDS_InstanceHandle_t const rtihandle =
        this->typed_impl ()->register_instance_w_timestamp (instance_data, time);
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::unregister_instance (
        const typename  DDS_TYPE::value_type & instance_data,
        const ::DDS::InstanceHandle_t & handle)
    {
      ::DDS_InstanceHandle_t instance_handle = ::DDS_HANDLE_NIL;
      instance_handle <<= handle;
      return this->typed_impl ()->unregister_instance (instance_data, instance_handle);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::unregister_instance_w_timestamp (
        const typename  DDS_TYPE::value_type & instance_data,
        const ::DDS::InstanceHandle_t & handle,
        const ::DDS::Time_t & source_timestamp)
    {
      ::DDS_InstanceHandle_t instance_handle = ::DDS_HANDLE_NIL;
      instance_handle <<= handle;
      ::DDS_Time_t time;
      time <<= source_timestamp;
      return this->typed_impl ()->unregister_instance_w_timestamp (instance_data, instance_handle, time);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::write (
        const typename DDS_TYPE::value_type & instance_data,
        const ::DDS::InstanceHandle_t & handle)
    {
      ::DDS_InstanceHandle_t instance_handle = ::DDS_HANDLE_NIL;
      instance_handle <<= handle;
      return this->typed_impl ()->write (instance_data, instance_handle);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::write_w_timestamp (
        const typename DDS_TYPE::value_type & instance_data,
        const ::DDS::InstanceHandle_t & handle,
        const ::DDS::Time_t & source_timestamp)
    {
      ::DDS_InstanceHandle_t instance_handle = ::DDS_HANDLE_NIL;
      instance_handle <<= handle;
      ::DDS_Time_t time;
      time <<= source_timestamp;
      return this->typed_impl ()->write_w_timestamp (instance_data, instance_handle, time);
    }

    template <typename DDS_TYPE>
     ::DDS::ReturnCode_t
     DDS_DataWriter_T<DDS_TYPE>::dispose (
        const typename DDS_TYPE::value_type& instance_data,
        const ::DDS::InstanceHandle_t & handle)
    {
      ::DDS_InstanceHandle_t instance_handle = ::DDS_HANDLE_NIL;
      instance_handle <<= handle;
      return this->typed_impl ()->dispose (instance_data, instance_handle);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::dispose_w_timestamp (
        const typename DDS_TYPE::value_type& instance_data,
        const ::DDS::InstanceHandle_t & handle,
        const ::DDS::Time_t & source_timestamp)
    {
      ::DDS_InstanceHandle_t instance_handle = ::DDS_HANDLE_NIL;
      instance_handle <<= handle;
      ::DDS_Time_t time;
      time <<= source_timestamp;
      return this->typed_impl ()->dispose_w_timestamp (instance_data, instance_handle, time);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DataWriter_T<DDS_TYPE>::get_key_value (
        typename DDS_TYPE::value_type & key_holder,
        const ::DDS::InstanceHandle_t & handle)
    {
      ::DDS_InstanceHandle_t instance_handle = ::DDS_HANDLE_NIL;
      instance_handle <<= handle;
      return this->typed_impl ()->get_key_value (key_holder, instance_handle);
    }

    template <typename DDS_TYPE>
    ::DDS::InstanceHandle_t
    DDS_DataWriter_T<DDS_TYPE>::lookup_instance (
        const typename DDS_TYPE::value_type& instance_data)
    {
      ::DDS_InstanceHandle_t const rtihandle =
        this->typed_impl ()->lookup_instance (instance_data);
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }
  }
}
