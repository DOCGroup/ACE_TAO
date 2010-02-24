// $Id$

#include "Topic.h"
#include "ContentFilteredTopic.h"
#include "StatusCondition.h"
#include "TopicListener.h"
#include "InstanceHandle_t.h"
#include "InconsistentTopicStatus.h"
#include "TopicQos.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_Topic_i::CCM_DDS_Topic_i (DDSTopic* topic)
      : impl_ (topic)
    {
    }

    CCM_DDS_Topic_i::~CCM_DDS_Topic_i (void)
    {
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Topic_i::set_qos (const ::DDS::TopicQos &qos)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_TopicQos ddsqos;
      ddsqos <<= qos;
      return this->impl ()->set_qos (ddsqos);
#else
      return this->impl ()->set_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Topic_i::get_qos (::DDS::TopicQos &qos)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_TopicQos ddsqos;
      ::DDS_ReturnCode_t const retval = this->impl ()->get_qos (ddsqos);
      qos <<= ddsqos;
      return retval;
#else
      return this->impl ()->get_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Topic_i::set_listener (
      ::DDS::TopicListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Topic_i::set_listener");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_TopicListener_i *rti_impl_list = 0;
      if (!CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (rti_impl_list,
                            CCM_DDS_TopicListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }
      return this->impl ()->set_listener (rti_impl_list, mask);
#else
      return this->impl ()->set_listener (a_listener, mask);
#endif
    }

    ::DDS::TopicListener_ptr
    CCM_DDS_Topic_i::get_listener (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Topic_i::get_listener");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDSTopicListener *rti_topic_list = this->impl ()->get_listener ();
      CCM_DDS_TopicListener_i *list_proxy = dynamic_cast <CCM_DDS_TopicListener_i *> (rti_topic_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, "CCM_DDS_Topic_i::get_listener - "
                                    "DDS returned a NIL listener.\n"));
          return ::DDS::TopicListener::_nil ();
        }
      return list_proxy->get_topiclistener ();
#else
      return this->impl ()->get_listener ();
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Topic_i::get_inconsistent_topic_status (
      ::DDS::InconsistentTopicStatus & a_status)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDS_InconsistentTopicStatus ddsstatus;
      ddsstatus <<= a_status;
      return this->impl ()->get_inconsistent_topic_status (ddsstatus);
#else
      return this->impl ()->get_inconsistent_topic_status (a_status);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Topic_i::enable (void)
    {
      return this->impl ()->enable ();
    }

    ::DDS::StatusCondition_ptr
    CCM_DDS_Topic_i::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval = ::DDS::StatusCondition::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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
    CCM_DDS_Topic_i::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    ::DDS::InstanceHandle_t
    CCM_DDS_Topic_i::get_instance_handle (void)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
#else
      return this->impl ()->get_instance_handle ();
#endif
    }

    char *
    CCM_DDS_Topic_i::get_type_name (void)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      return CORBA::string_dup (this->impl ()->get_type_name ());
#else
      return this->impl ()->get_type_name ();
#endif
    }

    char *
    CCM_DDS_Topic_i::get_name (void)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      return CORBA::string_dup (this->impl ()->get_name ());
#else
      return this->impl ()->get_name ();
#endif
    }

    ::DDS::DomainParticipant_ptr
    CCM_DDS_Topic_i::get_participant (void)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
      ACE_NEW_THROW_EX (retval,
                        ::CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i (),
                        CORBA::NO_MEMORY ());

      DDSDomainParticipant* p = this->impl ()->get_participant ();
      ::CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i *rti_dp = dynamic_cast < ::CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i *> (retval.in ());
      rti_dp->set_impl (p);
      return retval._retn ();
#else
      return this->impl ()->get_participant ();
#endif
    }

    DDSTopic *
    CCM_DDS_Topic_i::get_impl (void)
    {
      return this->impl_;
    }

    void
    CCM_DDS_Topic_i::set_impl (DDSTopic * topic)
    {
      this->impl_ = topic;
    }

    DDSTopic *
    CCM_DDS_Topic_i::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
      }
  }
}
