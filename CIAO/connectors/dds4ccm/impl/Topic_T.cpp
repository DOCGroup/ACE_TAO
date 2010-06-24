// $Id$

#include "Topic_T.h"
#include "ContentFilteredTopic_T.h"
#include "StatusCondition.h"
#include "DDSTopicListener_T.h"

#include "ndds/InstanceHandle_t.h"
#include "ndds/InconsistentTopicStatus.h"
#include "ndds/TopicQos.h"

#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::CCM_DDS_Topic_T (DDSTopic* topic)
      : impl_ (topic)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::~CCM_DDS_Topic_T (void)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::set_qos (const ::DDS::TopicQos &qos)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_TopicQos ddsqos;
      ddsqos <<= qos;
      return this->impl ()->set_qos (ddsqos);
#else
      return this->impl ()->set_qos (qos);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_qos (::DDS::TopicQos &qos)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_TopicQos ddsqos;
      ::DDS_ReturnCode_t const retval = this->impl ()->get_qos (ddsqos);
      qos <<= ddsqos;
      return retval;
#else
      return this->impl ()->get_qos (qos);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::set_listener (
      ::DDS::TopicListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::set_listener");

#if (CIAO_DDS4CCM_NDDS==1)
      TopicListener_type *ccm_dds_impl_list = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            TopicListener_type (a_listener),
                            CORBA::NO_MEMORY ());
        }
      return this->impl ()->set_listener (ccm_dds_impl_list, mask);
#else
      return this->impl ()->set_listener (a_listener, mask);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::TopicListener_ptr
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_listener (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_listener");

#if (CIAO_DDS4CCM_NDDS==1)
      DDSTopicListener *ccm_dds_topic_list = this->impl ()->get_listener ();
      TopicListener_type * list_proxy =
        dynamic_cast <TopicListener_type *> (ccm_dds_topic_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, "CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_listener - "
                                    "DDS returned a NIL listener.\n"));
          return ::DDS::TopicListener::_nil ();
        }
      return list_proxy->get_topiclistener ();
#else
      return this->impl ()->get_listener ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_inconsistent_topic_status (
      ::DDS::InconsistentTopicStatus & a_status)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_InconsistentTopicStatus ddsstatus;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_inconsistent_topic_status (ddsstatus);
      a_status <<= ddsstatus;;
      return retval;
#else
      return this->impl ()->get_inconsistent_topic_status (a_status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::enable (void)
    {
      return this->impl ()->enable ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::StatusCondition_ptr
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_statuscondition (void)
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
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::InstanceHandle_t
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_instance_handle (void)
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    char *
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_type_name (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      return CORBA::string_dup (this->impl ()->get_type_name ());
#else
      return this->impl ()->get_type_name ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    char *
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_name (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      return CORBA::string_dup (this->impl ()->get_name ());
#else
      return this->impl ()->get_name ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DomainParticipant_ptr
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_participant (void)
    {
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipant * p = this->impl ()->get_participant ();
      if (p)
        {
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (p),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDS::DomainParticipant_var p = this->impl ()->get_participant ();
      if (! ::CORBA::is_nil (p.in ()))
        {
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (p.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSTopic *
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::set_impl (DDSTopic * topic)
    {
      this->impl_ = topic;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSTopic *
    CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
      }
  }
}
