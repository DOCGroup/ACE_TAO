// $Id$

#include "Subscriber.h"
#include "SubscriberListener.h"
#include "Topic.h"
#include "ContentFilteredTopic.h"
#include "DataReader.h"
#include "DataReaderListener.h"
#include "Utils.h"
#include "StatusCondition.h"
#include "InstanceHandle_t.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_Subscriber_i::RTI_Subscriber_i (void)
        : impl_ (0)
      {
        CIAO_TRACE ("RTI_Subscriber_i::RTI_Subscriber_i");
      }

      RTI_Subscriber_i::~RTI_Subscriber_i (void)
      {
        CIAO_TRACE ("RTI_Subscriber_i::~RTI_Subscriber_i");
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::enable (void)
      {
        return this->impl ()->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_Subscriber_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i ();
        RTI_StatusCondition_i *rti_sc = dynamic_cast < RTI_StatusCondition_i *> (retval.in ());
        rti_sc->set_impl (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_Subscriber_i::get_status_changes (void)
      {
        return this->impl ()->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_Subscriber_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
      }

      DDSDataReader *
      RTI_Subscriber_i::create_datareader (
                    DDSContentFilteredTopic * topic,
                    DDSDataReaderListener * rti_drl,
                    ::DDS::StatusMask mask)
      {
        DDS_DataReaderQos rti_qos = DDS_DATAREADER_QOS_DEFAULT;
        return this->impl ()->create_datareader (topic,
                                                 rti_qos,
                                                 rti_drl,
                                                 mask);
      }

      DDSDataReader *
      RTI_Subscriber_i::create_datareader (
                    DDSTopic * topic,
                    DDSDataReaderListener * rti_drl,
                    ::DDS::StatusMask mask)
      {
        DDS_DataReaderQos rti_qos = DDS_DATAREADER_QOS_DEFAULT;
        return this->impl ()->create_datareader (topic,
                                                 rti_qos,
                                                 rti_drl,
                                                 mask);
      }

      DDSDataReader *
      RTI_Subscriber_i::create_datareader_with_profile (
                    DDSContentFilteredTopic * topic,
                    const char * library_name,
                    const char * profile_name,
                    DDSDataReaderListener * rti_drl,
                    ::DDS::StatusMask mask)
      {
        return this->impl ()->create_datareader_with_profile (topic,
                                                              library_name,
                                                              profile_name,
                                                              rti_drl,
                                                              mask);
      }

      DDSDataReader *
      RTI_Subscriber_i::create_datareader_with_profile (
                    DDSTopic * topic,
                    const char * library_name,
                    const char * profile_name,
                    DDSDataReaderListener * rti_drl,
                    ::DDS::StatusMask mask)
      {
        return this->impl ()->create_datareader_with_profile (topic,
                                                              library_name,
                                                              profile_name,
                                                              rti_drl,
                                                              mask);
      }

      ::DDS::DataReader_ptr
      RTI_Subscriber_i::create_datareader (
        ::DDS::TopicDescription_ptr a_topic,
        const ::DDS::DataReaderQos & /*qos*/,
        ::DDS::DataReaderListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_Subscriber_i::create_datareader");
        DDSDataReaderListener *rti_drl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            ACE_NEW_THROW_EX (rti_drl,
                              RTI_DataReaderListener_i (a_listener),
                              CORBA::NO_MEMORY ());
          }

        DDSDataReader * rti_dr = 0;
        RTI_Topic_i * topic = dynamic_cast < RTI_Topic_i * > (a_topic);

        if (!topic)
          {
            RTI_ContentFilteredTopic_i * cf_topic =
              dynamic_cast < RTI_ContentFilteredTopic_i * > (a_topic);
            if (!cf_topic)
              {
                CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Subscriber_i::create_datareader - "
                            "Error: Unable to cast provided topic to one of its servant.\n"));
                throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
              }
            else
              rti_dr = this->create_datareader (cf_topic->get_impl (), rti_drl, mask);
          }
        else
          rti_dr = this->create_datareader (topic->get_impl (), rti_drl, mask);

        if (!rti_dr)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Subscriber_i::create_datareader - "
                         "Error: RTI Topic returned a nil datareader.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }
        else
          {
            CIAO_DEBUG (6, (LM_DEBUG, CLINFO "RTI_Subscriber_i::create_datareader_with_profile - "
                         "Successfully created datareader.\n"));
          }

        rti_dr->enable ();
        ::DDS::DataReader_var retval = new RTI_DataReader_i ();
        RTI_DataReader_i *dr = dynamic_cast < RTI_DataReader_i *>(retval.in ());
        dr->set_impl (rti_dr);

        return retval._retn ();
      }

      ::DDS::DataReader_ptr
      RTI_Subscriber_i::create_datareader_with_profile (
        ::DDS::TopicDescription_ptr a_topic,
        const char * library_name,
        const char * profile_name,
        ::DDS::DataReaderListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_Subscriber_i::create_datareader_with_profile");
        DDSDataReaderListener *rti_drl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            ACE_NEW_THROW_EX (rti_drl,
                              RTI_DataReaderListener_i (a_listener),
                              CORBA::NO_MEMORY ());
          }

        DDSDataReader * rti_dr = 0;
        RTI_Topic_i * topic = dynamic_cast < RTI_Topic_i * > (a_topic);

        if (!topic)
          {
            RTI_ContentFilteredTopic_i * cf_topic =
              dynamic_cast < RTI_ContentFilteredTopic_i * > (a_topic);
            if (!cf_topic)
              {
                CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Subscriber_i::create_datareader_with_profile - "
                            "Error: Unable to cast provided topic to one of its servant.\n"));
                throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
              }
            else
              rti_dr = this->create_datareader_with_profile (cf_topic->get_impl (),
                                                             library_name,
                                                             profile_name,
                                                             rti_drl,
                                                             mask);
          }
        else
          rti_dr = this->create_datareader_with_profile (topic->get_impl (),
                                                             library_name,
                                                             profile_name,
                                                             rti_drl,
                                                             mask);

        if (!rti_dr)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Subscriber_i::create_datareader_with_profile - "
                         "Error: RTI Topic returned a nil datareader.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }
        else
          {
            CIAO_DEBUG (6, (LM_DEBUG, CLINFO "RTI_Subscriber_i::create_datareader_with_profile - "
                         "Successfully created datareader with profile <%C#%C>.\n",
                         library_name,
                         profile_name));
          }

        rti_dr->enable ();
        ::DDS::DataReader_var retval = new RTI_DataReader_i ();
        RTI_DataReader_i *dr = dynamic_cast < RTI_DataReader_i *>(retval.in ());
        dr->set_impl (rti_dr);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::delete_datareader (
        ::DDS::DataReader_ptr a_datareader)
      {
        RTI_DataReader_i *dr = dynamic_cast< RTI_DataReader_i *> (a_datareader);
        if (!dr)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Subscriber_i::delete_datareader - "
                         "Unable to cast provided object reference to servant.\n"));
            return ::DDS::RETCODE_BAD_PARAMETER;
          }

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTI_Subscriber_i::delete_datareader - "
                     "Successfully casted provided object reference to servant.\n"));

        DDS_ReturnCode_t const retval = this->impl ()->delete_datareader (dr->get_impl ());

        if (retval != DDS_RETCODE_OK)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Subscriber_i::delete_datareader - "
                         "Error: Returned non-ok error code %C\n",
                         translate_retcode (retval)));
          }
        else CIAO_DEBUG (6, (LM_INFO, CLINFO "RTI_Subscriber_i::delete_datareader - "
                          "Datareader successfully  deleted\n"));

        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::delete_contained_entities (void)
      {
        return this->impl ()->delete_contained_entities ();
      }

      ::DDS::DataReader_ptr
      RTI_Subscriber_i::lookup_datareader (
        const char * impl_name)
      {
        DDSDataReader* dr = this->impl ()->lookup_datareader (impl_name);
        ::DDS::DataReader_var retval = new RTI_DataReader_i ();
        RTI_DataReader_i *rti_dr = dynamic_cast < RTI_DataReader_i *>(retval.in ());
        rti_dr->set_impl (dr);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::get_datareaders (
        ::DDS::DataReaderSeq & /*readers*/,
        ::DDS::SampleStateMask /*sample_states*/,
        ::DDS::ViewStateMask /*view_states*/,
        ::DDS::InstanceStateMask /*instance_states*/)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::notify_datareaders (void)
      {
        return this->impl ()->notify_datareaders ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::set_qos (
        const ::DDS::SubscriberQos & /*qos*/)
      {
        DDS_SubscriberQos rti_impl_qos;
/*        rti_impl_qos.presentation = qos.presentation;
        rti_impl_qos.partition = qos.partition;
        rti_impl_qos.group_data = qos.group_data;
        rti_impl_qos.entity_factory = qos.entity_factory;*/
        return this->impl ()->set_qos (rti_impl_qos);
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::get_qos (
        ::DDS::SubscriberQos & /*qos*/)
      {
        DDS_SubscriberQos rti_impl_qos;
        DDS_ReturnCode_t const rti_retcode = this->impl ()->get_qos (rti_impl_qos);
        /*qos.presentation = rti_impl_qos.presentation;
        qos.partition = rti_impl_qos.partition;
        qos.group_data = rti_impl_qos.group_data;
        qos.entity_factory = rti_impl_qos.entity_factory;*/
        return rti_retcode;
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::set_listener (
        ::DDS::SubscriberListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_Subscriber_i::set_listener");

        RTI_SubscriberListener_i* rti_impl_list = 0;
        if (!CORBA::is_nil (a_listener))
          {
            ACE_NEW_THROW_EX (rti_impl_list,
                              RTI_SubscriberListener_i (a_listener),
                              CORBA::NO_MEMORY ());
          }
        return this->impl ()->set_listener (rti_impl_list, mask);
      }

      ::DDS::SubscriberListener_ptr
      RTI_Subscriber_i::get_listener (void)
      {
        CIAO_TRACE ("RTI_Subscriber_i::get_listener");

        DDSSubscriberListener *rti_impl_list = this->impl ()->get_listener ();
        RTI_SubscriberListener_i *list_proxy = dynamic_cast <RTI_SubscriberListener_i *> (rti_impl_list);
        if (!list_proxy)
          {
            CIAO_DEBUG (6, (LM_DEBUG, "RTI_Subscriber_i::get_listener - "
                                      "DDS returned a NIL listener.\n"));
            return ::DDS::SubscriberListener::_nil ();
          }
        return list_proxy->get_subscriber_listener ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::begin_access (void)
      {
        return this->impl ()->begin_access ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::end_access (void)
      {
        return this->impl ()->end_access ();
      }

      ::DDS::DomainParticipant_ptr
      RTI_Subscriber_i::get_participant (void)
      {
        DDSDomainParticipant* p = this->impl ()->get_participant ();
        ::DDS::DomainParticipant_var retval = new RTI_DomainParticipant_i ();
        RTI_DomainParticipant_i *rti_dp = dynamic_cast < RTI_DomainParticipant_i *> (retval.in ());
        rti_dp->set_impl (p);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::set_default_datareader_qos (
        const ::DDS::DataReaderQos & /*qos*/)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::get_default_datareader_qos (
        ::DDS::DataReaderQos & /*qos*/)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::copy_from_topic_qos (
        ::DDS::DataReaderQos & /*a_datareader_qos*/,
        const ::DDS::TopicQos & /*a_impl_qos*/)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      DDSSubscriber *
      RTI_Subscriber_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_Subscriber_i::set_impl (DDSSubscriber * sub)
      {
        this->impl_ = sub;
      }

      DDSSubscriber *
      RTI_Subscriber_i::impl (void)
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

