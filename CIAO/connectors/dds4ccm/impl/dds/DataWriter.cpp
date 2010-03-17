// $Id$

#include "DataWriter.h"
#include "StatusCondition.h"
#include "Publisher.h"
#include "DataWriterListener.h"
#include "Topic.h"

#include "ndds/Duration_t.h"
#include "ndds/InstanceHandle_t.h"
#include "ndds/PublicationMatchedStatus.h"
#include "ndds/LivelinessLostStatus.h"
#include "ndds/OfferedIncompatibleQosStatus.h"
#include "ndds/OfferedDeadlineMissedStatus.h"
#include "ndds/InstanceHandleSeq.h"
#include "ndds/DataWriterQos.h"
#include "ndds/SubscriptionBuiltinTopicData.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_DataWriter_i::CCM_DDS_DataWriter_i (DDSDataWriter * dw)
      : impl_ (dw)
    {
    }

    CCM_DDS_DataWriter_i::~CCM_DDS_DataWriter_i (void)
    {
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::set_qos (const ::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriter_i::set_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataWriterQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl()->set_qos (ccm_dds_qos);
#else
      return this->impl ()->set_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::get_qos (::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriter_i::get_qos");
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

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::set_listener (::DDS::DataWriterListener_ptr a_listener,
                                    ::DDS::StatusMask mask)
    {
      CCM_DDS_DataWriterListener_i* ccm_dds_impl_list = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            CCM_DDS_DataWriterListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }
      return this->impl ()->set_listener (ccm_dds_impl_list, mask);
    }

    ::DDS::DataWriterListener_ptr
    CCM_DDS_DataWriter_i::get_listener (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDataWriterListener *wr = this->impl ()->get_listener ();
      CCM_DDS_DataWriterListener_i *list_proxy = dynamic_cast <CCM_DDS_DataWriterListener_i *> (wr);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, "CCM_DDS_DataWriter_i::get_listener - "
                                    "DDS returned a NIL listener.\n"));
          return ::DDS::DataWriterListener::_nil ();
        }
      return list_proxy->get_datawriterlistener ();
#else
      return this->impl ()->get_listener ();
#endif
    }

    ::DDS::Topic_ptr
    CCM_DDS_DataWriter_i::get_topic (void)
    {
      ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSTopic* t = this->impl ()->get_topic ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Topic_i (t),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::Topic_var t = this->impl ()->get_topic ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Topic_i (t.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    ::DDS::Publisher_ptr
    CCM_DDS_DataWriter_i::get_publisher (void)
    {
      ::DDS::Publisher_var retval = ::DDS::Publisher::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSPublisher* p = this->impl ()->get_publisher ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Publisher_i (p),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::Publisher_var p = this->impl ()->get_publisher ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Publisher_i (p.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
    {
#if (CIAO_DDS4CCM_NDDS==1)
     ::DDS_Duration_t rtiduration;
     rtiduration <<= max_wait;
     return this->impl ()->wait_for_acknowledgments (rtiduration);
#else
     return this->impl ()->wait_for_acknowledgments (max_wait);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::get_liveliness_lost_status (::DDS::LivelinessLostStatus & status)
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

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_OfferedDeadlineMissedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_offered_deadline_missed_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_offered_deadline_missed_status (status);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_OfferedIncompatibleQosStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_offered_incompatible_qos_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_offered_incompatible_qos_status (status);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::get_publication_matched_status (::DDS::PublicationMatchedStatus & status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_PublicationMatchedStatus ddsstatus;
      ddsstatus <<= status;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_publication_matched_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
#else
      return this->impl ()->get_publication_matched_status (status);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::assert_liveliness (void)
    {
      return this->impl ()->assert_liveliness ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::get_matched_subscriptions (::DDS::InstanceHandleSeq & subscription_handles)
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

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::get_matched_subscription_data (::DDS::SubscriptionBuiltinTopicData & subscription_data,
                                                         DDS_INSTANCE_HANDLE_T_IN subscription_handle)
    {
      DDS4CCM_TRACE ("CCM_DDS_DataWriter_i::get_matched_subscription_data");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_SubscriptionBuiltinTopicData ccm_dds_sub_data;
      ::DDS_InstanceHandle_t ccm_dds_sub_handle;
      ccm_dds_sub_data <<= subscription_data;
      ccm_dds_sub_handle <<= subscription_handle;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_matched_subscription_data (ccm_dds_sub_data,
                                                                                       ccm_dds_sub_handle);
      subscription_data <<= ccm_dds_sub_data;
      return retval;
#else
      return this->impl ()->get_matched_subscription_data (subscription_data, subscription_handle);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DataWriter_i::enable (void)
    {
      return this->impl ()->enable ();
    }

    ::DDS::StatusCondition_ptr
    CCM_DDS_DataWriter_i::get_statuscondition (void)
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

    ::DDS::StatusMask
    CCM_DDS_DataWriter_i::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    DDS_INSTANCE_HANDLE_T_RETN
    CCM_DDS_DataWriter_i::get_instance_handle (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
#else
      return this->impl ()->get_instance_handle ();
#endif
    }

    DDSDataWriter *
    CCM_DDS_DataWriter_i::get_impl (void)
    {
      return this->impl_;
    }

    void
    CCM_DDS_DataWriter_i::set_impl (DDSDataWriter * dw)
    {
      this->impl_ = dw;
    }

    DDSDataWriter *
    CCM_DDS_DataWriter_i::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}
