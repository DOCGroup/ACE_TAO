// $Id$

#include "Topic.h"
#include "ContentFilteredTopic.h"
#include "StatusCondition.h"
#include "TopicListener.h"
#include "InstanceHandle_t.h"
#include "InconsistentTopicStatus.h"
#include "TopicQos.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_Topic_i::RTI_Topic_i (void)
        : impl_ (0)
      {
      }

      RTI_Topic_i::~RTI_Topic_i (void)
      {
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::set_qos (const ::DDS::TopicQos &qos)
      {
        ::DDS_TopicQos ddsqos;
        ddsqos <<= qos;
        return this->impl ()->set_qos (ddsqos);
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::get_qos (::DDS::TopicQos &qos)
      {
        ::DDS_TopicQos ddsqos;
        ::DDS_ReturnCode_t const retval = this->impl ()->get_qos (ddsqos);
        qos <<= ddsqos;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::set_listener (::DDS::TopicListener_ptr a_listener,
                                 ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_Topic_i::set_listener");

        RTI_TopicListener_i *rti_impl_list = 0;
        if (!CORBA::is_nil (a_listener))
          {
            ACE_NEW_THROW_EX (rti_impl_list,
                              RTI_TopicListener_i (a_listener),
                              CORBA::NO_MEMORY ());
          }
        return this->impl ()->set_listener (rti_impl_list, mask);
      }

      ::DDS::TopicListener_ptr
      RTI_Topic_i::get_listener (void)
      {
        CIAO_TRACE ("RTI_Topic_i::get_listener");

        DDSTopicListener *rti_topic_list = this->impl ()->get_listener ();
        RTI_TopicListener_i *list_proxy = dynamic_cast <RTI_TopicListener_i *> (rti_topic_list);
        if (!list_proxy)
          {
            CIAO_DEBUG (6, (LM_DEBUG, "RTI_Topic_i::get_listener - "
                                      "DDS returned a NIL listener.\n"));
            return ::DDS::TopicListener::_nil ();
          }
        return list_proxy->get_topiclistener ();
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::get_inconsistent_topic_status (::DDS::InconsistentTopicStatus & a_status)
      {
        DDS_InconsistentTopicStatus ddsstatus;
        ddsstatus <<= a_status;
        return this->impl ()->get_inconsistent_topic_status (ddsstatus);
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::enable (void)
      {
        return this->impl ()->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_Topic_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i ();
        RTI_StatusCondition_i *rti_sc = dynamic_cast < RTI_StatusCondition_i *> (retval.in ());
        rti_sc->set_impl (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_Topic_i::get_status_changes (void)
      {
        return this->impl ()->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_Topic_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
      }

      char *
      RTI_Topic_i::get_type_name (void)
      {
        return CORBA::string_dup (this->impl ()->get_type_name ());
      }

      char *
      RTI_Topic_i::get_name (void)
      {
        return CORBA::string_dup (this->impl ()->get_name ());
      }

      ::DDS::DomainParticipant_ptr
      RTI_Topic_i::get_participant (void)
      {
        DDSDomainParticipant* p = this->impl ()->get_participant ();
        ::DDS::DomainParticipant_var retval = new RTI_DomainParticipant_i ();
        RTI_DomainParticipant_i *rti_dp = dynamic_cast < RTI_DomainParticipant_i *> (retval.in ());
        rti_dp->set_impl (p);
        return retval._retn ();
      }

      DDSTopic *
      RTI_Topic_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_Topic_i::set_impl (DDSTopic * topic)
      {
        this->impl_ = topic;
      }

      DDSTopic *
      RTI_Topic_i::impl (void)
      {
        if (!this->impl_)
          {
            throw ::CORBA::BAD_INV_ORDER ();
          }
        return this->impl_;
      }

    }
  }
}
