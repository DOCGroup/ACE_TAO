// $Id$

#include "dds4ccm/impl/ndds/Topic.h"
#include "dds4ccm/impl/ndds/StatusCondition.h"
#include "dds4ccm/impl/ndds/TopicListener.h"

#include "dds4ccm/impl/ndds/convertors/InstanceHandle_t.h"
#include "dds4ccm/impl/ndds/convertors/InconsistentTopicStatus.h"
#include "dds4ccm/impl/ndds/convertors/TopicQos.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_Topic_i::DDS_Topic_i (::DDSTopic* topic,
                          ::DDS::DomainParticipant_ptr dp)
      : rti_entity_ (topic),
        dp_ (::DDS::DomainParticipant::_duplicate (dp))
    {
    }

    DDS_Topic_i::~DDS_Topic_i (void)
    {
    }

    ::DDS::ReturnCode_t
    DDS_Topic_i::set_qos (const ::DDS::TopicQos &qos)
    {
      ::DDS_TopicQos ddsqos;
      ddsqos <<= qos;
      return this->rti_entity ()->set_qos (ddsqos);
    }

    ::DDS::ReturnCode_t
    DDS_Topic_i::get_qos (::DDS::TopicQos &qos)
    {
      ::DDS_TopicQos ddsqos;
      ::DDS_ReturnCode_t const retval = this->rti_entity ()->get_qos (ddsqos);
      qos <<= ddsqos;
      return retval;
    }

    ::DDS::ReturnCode_t
    DDS_Topic_i::set_listener (
      ::DDS::TopicListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_Topic_i::set_listener");

      // Delete the previously set listener
      DDSTopicListener *listener = this->rti_entity ()->get_listener ();
      delete listener;
      listener = 0;

      this->topic_listener_ = ::DDS::TopicListener::_duplicate (a_listener);

      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (listener,
                            DDS_TopicListener_i (this, a_listener),
                            ::CORBA::NO_MEMORY ());

        }
      return this->rti_entity ()->set_listener (listener, mask);
    }

    ::DDS::TopicListener_ptr
    DDS_Topic_i::get_listener (void)
    {
      DDS4CCM_TRACE ("DDS_Topic_i::get_listener");

      return ::DDS::TopicListener::_duplicate (this->topic_listener_.in ());
    }

    ::DDS::ReturnCode_t
    DDS_Topic_i::get_inconsistent_topic_status (
      ::DDS::InconsistentTopicStatus & a_status)
    {
      DDS_InconsistentTopicStatus ddsstatus;
      ::DDS::ReturnCode_t const retval =
        this->rti_entity ()->get_inconsistent_topic_status (ddsstatus);
      a_status <<= ddsstatus;
      return retval;
    }

    ::DDS::ReturnCode_t
    DDS_Topic_i::enable (void)
    {
      return this->rti_entity ()->enable ();
    }

    ::DDS::StatusCondition_ptr
    DDS_Topic_i::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval;
      DDSStatusCondition* sc = this->rti_entity ()->get_statuscondition ();
      ACE_NEW_THROW_EX (retval,
                        DDS_StatusCondition_i (sc, this->dp_.in ()),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    ::DDS::StatusMask
    DDS_Topic_i::get_status_changes (void)
    {
      return this->rti_entity ()->get_status_changes ();
    }

    ::DDS::InstanceHandle_t
    DDS_Topic_i::get_instance_handle (void)
    {
      ::DDS_InstanceHandle_t const rtihandle =
        this->rti_entity ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }

    char *
    DDS_Topic_i::get_type_name (void)
    {
      return CORBA::string_dup (this->rti_entity ()->get_type_name ());
    }

    char *
    DDS_Topic_i::get_name (void)
    {
      return CORBA::string_dup (this->rti_entity ()->get_name ());
    }

    ::DDS::DomainParticipant_ptr
    DDS_Topic_i::get_participant (void)
    {
      return ::DDS::DomainParticipant::_duplicate (this->dp_.in ());
    }

    DDSTopic *
    DDS_Topic_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }

    void
    DDS_Topic_i::set_rti_entity (::DDSTopic* topic)
    {
      this->rti_entity_ = topic;
    }


    DDSTopic *
    DDS_Topic_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DDS_Topic_i::rti_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
      }
  }
}
