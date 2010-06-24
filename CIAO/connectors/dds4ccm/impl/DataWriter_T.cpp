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
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::CCM_DDS_DataWriter_T (DDSDataWriter * dw)
      : CCM_DDS_DataWriter_Base (dw)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::set_qos (const ::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::set_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataWriterQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl()->set_qos (ccm_dds_qos);
#else
      return this->impl ()->set_qos (qos);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_qos (::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataWriterQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      ::DDS::ReturnCode_t retcode = this->impl()->get_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
#else
      return this->impl ()->get_qos (qos);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::set_listener (
      ::DDS::DataWriterListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DataWriterListener_type * ccm_dds_impl_list = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            DataWriterListener_type (a_listener,
                                                     this),
                            CORBA::NO_MEMORY ());
        }
      return this->impl ()->set_listener (ccm_dds_impl_list, mask);
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DataWriterListener_ptr
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_listener (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDataWriterListener *wr = this->impl ()->get_listener ();
      DataWriterListener_type * list_proxy =
        dynamic_cast <DataWriterListener_type *> (wr);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, "CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_listener - "
                                    "DDS returned a NIL listener.\n"));
          return ::DDS::DataWriterListener::_nil ();
        }
      return list_proxy->get_datawriterlistener ();
#else
      return this->impl ()->get_listener ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::Topic_ptr
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_topic (void)
    {
      ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSTopic* t = this->impl ()->get_topic ();
      ACE_NEW_THROW_EX (retval,
                        Topic_type (t),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::Topic_var t = this->impl ()->get_topic ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Topic_T (t.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::Publisher_ptr
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_publisher (void)
    {
      ::DDS::Publisher_var retval = ::DDS::Publisher::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSPublisher* p = this->impl ()->get_publisher ();
      ACE_NEW_THROW_EX (retval,
                        Publisher_type (p),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::Publisher_var p = this->impl ()->get_publisher ();
      ACE_NEW_THROW_EX (retval,
                        Publisher_type (p.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
    {
#if (CIAO_DDS4CCM_NDDS==1)
     ::DDS_Duration_t rtiduration;
     rtiduration <<= max_wait;
     return this->impl ()->wait_for_acknowledgments (rtiduration);
#else
     return this->impl ()->wait_for_acknowledgments (max_wait);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_liveliness_lost_status (::DDS::LivelinessLostStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_LivelinessLostStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_liveliness_lost_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_liveliness_lost_status (status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_offered_deadline_missed_status (
      ::DDS::OfferedDeadlineMissedStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_OfferedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_offered_deadline_missed_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_offered_deadline_missed_status (status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_offered_incompatible_qos_status (
      ::DDS::OfferedIncompatibleQosStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_offered_incompatible_qos_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_offered_incompatible_qos_status (status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_publication_matched_status (
      ::DDS::PublicationMatchedStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_publication_matched_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_publication_matched_status (status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::assert_liveliness (void)
    {
      return this->impl ()->assert_liveliness ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_matched_subscriptions (
      ::DDS::InstanceHandleSeq & subscription_handles)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandleSeq rtiseq;
      rtiseq <<= subscription_handles;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_matched_subscriptions (rtiseq);
      subscription_handles <<= rtiseq;
      return retval;
#else
      return this->impl ()->get_matched_subscriptions (subscription_handles);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_matched_subscription_data (
      ::DDS::SubscriptionBuiltinTopicData & subscription_data,
      DDS_INSTANCE_HANDLE_T_IN subscription_handle)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_matched_subscription_data");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_SubscriptionBuiltinTopicData ccm_dds_sub_data;
      ::DDS_InstanceHandle_t ccm_dds_sub_handle;
      ccm_dds_sub_handle <<= subscription_handle;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_matched_subscription_data (ccm_dds_sub_data, ccm_dds_sub_handle);
      subscription_data <<= ccm_dds_sub_data;
      return retval;
#else
      return this->impl ()->get_matched_subscription_data (subscription_data, subscription_handle);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::enable (void)
    {
      return this->impl ()->enable ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::StatusCondition_ptr
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval = ::DDS::StatusCondition::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_StatusCondition_i (sc),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::StatusCondition_var sc = this->impl ()->get_statuscondition ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_StatusCondition_i (sc.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::StatusMask
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDS_INSTANCE_HANDLE_T_RETN
    CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE>::get_instance_handle (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandle_t const rtihandle =
        this->impl ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
#else
      return this->impl ()->get_instance_handle ();
#endif
    }
  }
}
