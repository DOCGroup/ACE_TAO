// $Id$

#include "DataWriter_T.h"
#include "StatusCondition.h"
#include "Publisher_T.h"
#include "DDSDataWriterListener_T.h"
#include "Topic_T.h"

#include "ndds/Duration_t.h"
#include "ndds/InstanceHandle_t.h"
#include "ndds/PublicationMatchedStatus.h"
#include "ndds/LivelinessLostStatus.h"
#include "ndds/OfferedIncompatibleQosStatus.h"
#include "ndds/OfferedDeadlineMissedStatus.h"
#include "ndds/InstanceHandleSeq.h"
#include "ndds/DataWriterQos.h"
#include "ndds/SubscriptionBuiltinTopicData.h"

#include "dds4ccm/impl/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::CCM_DDS_DataWriter_T (DDSDataWriter * dw)
      : CCM_DDS_DataWriter_Base (dw)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_qos (const ::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_qos");
      ::DDS_DataWriterQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl()->set_qos (ccm_dds_qos);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_qos (::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_qos");
      ::DDS_DataWriterQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      ::DDS::ReturnCode_t retcode = this->impl()->get_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_listener (
      ::DDS::DataWriterListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      // Delete the previously set listener
      DDSDataWriterListener *listener = this->impl ()->get_listener ();
      delete listener;

      DataWriterListener_type * ccm_dds_impl_list = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            DataWriterListener_type (a_listener, this),
                            ::CORBA::NO_MEMORY ());
        }
      return this->impl ()->set_listener (ccm_dds_impl_list, mask);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::DataWriterListener_ptr
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_listener (void)
    {
      DDSDataWriterListener *wr = this->impl ()->get_listener ();
      DataWriterListener_type * list_proxy =
        dynamic_cast <DataWriterListener_type *> (wr);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_listener - "
                        "DDS returned a NIL listener.\n"));
          return ::DDS::DataWriterListener::_nil ();
        }
      return list_proxy->get_datawriterlistener ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Topic_ptr
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_topic (void)
    {
      ::DDS::Topic_var retval;
      DDSTopic* t = this->impl ()->get_topic ();
      ACE_NEW_THROW_EX (retval,
                        Topic_type (t),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Publisher_ptr
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_publisher (void)
    {
      ::DDS::Publisher_var retval;
      DDSPublisher* p = this->impl ()->get_publisher ();
      ACE_NEW_THROW_EX (retval,
                        Publisher_type (p),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
    {
     ::DDS_Duration_t rtiduration;
     rtiduration <<= max_wait;
     return this->impl ()->wait_for_acknowledgments (rtiduration);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_liveliness_lost_status (::DDS::LivelinessLostStatus & status)
    {
      ::DDS_LivelinessLostStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_liveliness_lost_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_offered_deadline_missed_status (
      ::DDS::OfferedDeadlineMissedStatus & status)
    {
      ::DDS_OfferedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_offered_deadline_missed_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_offered_incompatible_qos_status (
      ::DDS::OfferedIncompatibleQosStatus & status)
    {
      ::DDS_OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_offered_incompatible_qos_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_publication_matched_status (
      ::DDS::PublicationMatchedStatus & status)
    {
      ::DDS_PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_publication_matched_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::assert_liveliness (void)
    {
      return this->impl ()->assert_liveliness ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_matched_subscriptions (
      ::DDS::InstanceHandleSeq & subscription_handles)
    {
      ::DDS_InstanceHandleSeq rtiseq;
      rtiseq <<= subscription_handles;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_matched_subscriptions (rtiseq);
      subscription_handles <<= rtiseq;
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_matched_subscription_data (
      ::DDS::SubscriptionBuiltinTopicData & subscription_data,
      DDS_INSTANCE_HANDLE_T_IN subscription_handle)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_matched_subscription_data");
      ::DDS_SubscriptionBuiltinTopicData ccm_dds_sub_data;
      ::DDS_InstanceHandle_t ccm_dds_sub_handle;
      ccm_dds_sub_handle <<= subscription_handle;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_matched_subscription_data (ccm_dds_sub_data, ccm_dds_sub_handle);
      subscription_data <<= ccm_dds_sub_data;
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::enable (void)
    {
      return this->impl ()->enable ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::StatusCondition_ptr
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval;
      DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_StatusCondition_i (sc),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::StatusMask
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDS_INSTANCE_HANDLE_T_RETN
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_instance_handle (void)
    {
      ::DDS_InstanceHandle_t const rtihandle =
        this->impl ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }
  }
}
