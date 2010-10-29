// $Id$

#include "dds4ccm/impl/ndds/Topic_T.h"
#include "dds4ccm/impl/ndds/ContentFilteredTopic_T.h"
#include "dds4ccm/impl/ndds/StatusCondition.h"
#include "dds4ccm/impl/ndds/DDSTopicListener_T.h"

#include "convertors/InstanceHandle_t.h"
#include "convertors/InconsistentTopicStatus.h"
#include "convertors/TopicQos.h"

#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE>
    DDS_Topic_T<DDS_TYPE>::DDS_Topic_T (DDSTopic* topic)
      : impl_ (topic)
    {
    }

    template <typename DDS_TYPE>
    DDS_Topic_T<DDS_TYPE>::~DDS_Topic_T (void)
    {
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_Topic_T<DDS_TYPE>::set_qos (const ::DDS::TopicQos &qos)
    {
      ::DDS_TopicQos ddsqos;
      ddsqos <<= qos;
      return this->impl ()->set_qos (ddsqos);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_Topic_T<DDS_TYPE>::get_qos (::DDS::TopicQos &qos)
    {
      ::DDS_TopicQos ddsqos;
      ::DDS_ReturnCode_t const retval = this->impl ()->get_qos (ddsqos);
      qos <<= ddsqos;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_Topic_T<DDS_TYPE>::set_listener (
      ::DDS::TopicListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_Topic_T<DDS_TYPE>::set_listener");

      // Delete the previously set listener
      DDSTopicListener *listener = this->impl ()->get_listener ();
      delete listener;

      TopicListener_type *ccm_dds_impl_list = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            TopicListener_type (a_listener),
                            ::CORBA::NO_MEMORY ());
        }
      return this->impl ()->set_listener (ccm_dds_impl_list, mask);
    }

    template <typename DDS_TYPE>
    ::DDS::TopicListener_ptr
    DDS_Topic_T<DDS_TYPE>::get_listener (void)
    {
      DDS4CCM_TRACE ("DDS_Topic_T<DDS_TYPE>::get_listener");

      DDSTopicListener *ccm_dds_topic_list = this->impl ()->get_listener ();
      TopicListener_type * list_proxy =
        dynamic_cast <TopicListener_type *> (ccm_dds_topic_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_Topic_T<DDS_TYPE>::"
                        "get_listener - DDS returned a NIL listener.\n"));
          return ::DDS::TopicListener::_nil ();
        }
      return list_proxy->get_topiclistener ();
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_Topic_T<DDS_TYPE>::get_inconsistent_topic_status (
      ::DDS::InconsistentTopicStatus & a_status)
    {
      DDS_InconsistentTopicStatus ddsstatus;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_inconsistent_topic_status (ddsstatus);
      a_status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_Topic_T<DDS_TYPE>::enable (void)
    {
      return this->impl ()->enable ();
    }

    template <typename DDS_TYPE>
    ::DDS::StatusCondition_ptr
    DDS_Topic_T<DDS_TYPE>::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval;
      DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
      ACE_NEW_THROW_EX (retval,
                        DDS_StatusCondition_i (sc),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE>
    ::DDS::StatusMask
    DDS_Topic_T<DDS_TYPE>::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    template <typename DDS_TYPE>
    ::DDS::InstanceHandle_t
    DDS_Topic_T<DDS_TYPE>::get_instance_handle (void)
    {
      ::DDS_InstanceHandle_t const rtihandle =
        this->impl ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }

    template <typename DDS_TYPE>
    char *
    DDS_Topic_T<DDS_TYPE>::get_type_name (void)
    {
      return CORBA::string_dup (this->impl ()->get_type_name ());
    }

    template <typename DDS_TYPE>
    char *
    DDS_Topic_T<DDS_TYPE>::get_name (void)
    {
      return CORBA::string_dup (this->impl ()->get_name ());
    }

    template <typename DDS_TYPE>
    ::DDS::DomainParticipant_ptr
    DDS_Topic_T<DDS_TYPE>::get_participant (void)
    {
      ::DDS::DomainParticipant_var retval;
      DDSDomainParticipant * p = this->impl ()->get_participant ();
      if (p)
        {
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (p),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE>
    DDSTopic *
    DDS_Topic_T<DDS_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE>
    void
    DDS_Topic_T<DDS_TYPE>::set_impl (DDSTopic * topic)
    {
      this->impl_ = topic;
    }

    template <typename DDS_TYPE>
    DDSTopic *
    DDS_Topic_T<DDS_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
      }
  }
}
