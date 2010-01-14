// $Id$

#include "DomainParticipant.h"
#include "Subscriber.h"
#include "Publisher.h"
#include "ContentFilteredTopic.h"
#include "Topic.h"
#include "TopicDescription.h"
#include "Utils.h"
#include "StatusCondition.h"
#include "InstanceHandle_t.h"
#include "Duration_t.h"
#include "Time_t.h"
#include "InstanceHandleSeq.h"
#include "PublisherListener.h"
#include "SubscriberListener.h"
#include "TopicListener.h"
#include "DomainParticipantListener.h"
#include "PublisherQos.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_DomainParticipant_i::RTI_DomainParticipant_i (void)
        : impl_ (0)
      {
        CIAO_TRACE ("RTI_DomainParticipant_i::RTI_DomainParticipant_i");
      }

      // Implementation skeleton destructor
      RTI_DomainParticipant_i::~RTI_DomainParticipant_i (void)
      {
        CIAO_TRACE ("RTI_DomainParticipant_i::~RTI_DomainParticipant_i");
      }

      ::DDS::Publisher_ptr
      RTI_DomainParticipant_i::create_publisher_with_profile (
        const char* library_name,
        const char *profile_name,
        ::DDS::PublisherListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_publisher_with_profile");

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTI_DomainParticipant_i::create_publisher_with_profile - "
                     "Creating Publisher\n"));

        RTI_PublisherListener_i *rti_pl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            rti_pl = new RTI_PublisherListener_i (a_listener);
          }
        DDSPublisher * rti_pub =
          this->impl ()->create_publisher_with_profile (library_name,
                                                        profile_name,
                                                        rti_pl,
                                                        mask);

        if (!rti_pub)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::create_publisher_with_profile - "
                         "Error: Unable to create Publisher\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        CIAO_DEBUG (6, (LM_INFO, CLINFO "RTI_DomainParticipant_i::create_publisher_with_profile - "
                     "Successfully created a DDSPublisher\n"));

        rti_pub->enable ();
        ::DDS::Publisher_var retval = new RTI_Publisher_i ();
        RTI_Publisher_i *rti_p = dynamic_cast < RTI_Publisher_i *> (retval.in ());
        rti_p->set_impl (rti_pub);

        return retval._retn ();
      }

      ::DDS::Publisher_ptr
      RTI_DomainParticipant_i::create_publisher (
        const ::DDS::PublisherQos & /*qos*/,
        ::DDS::PublisherListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_publisher");

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTI_DomainParticipant_i::create_publisher - "
                     "Creating Publisher\n"));

        DDS_PublisherQos rti_qos = DDS_PUBLISHER_QOS_DEFAULT;

        RTI_PublisherListener_i *rti_pl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            rti_pl = new RTI_PublisherListener_i (a_listener);
          }

        DDSPublisher * rti_pub =
          this->impl ()->create_publisher (rti_qos,
                                           rti_pl,
                                           mask);

        if (!rti_pub)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::create_publisher - "
                         "Error: Unable to create Publisher\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        CIAO_DEBUG (6, (LM_INFO, CLINFO "RTI_DomainParticipant_i::create_publisher - "
                     "Successfully created a DDSPublisher\n"));

        rti_pub->enable ();
        ::DDS::Publisher_var retval = new RTI_Publisher_i ();
        RTI_Publisher_i *rti_p = dynamic_cast < RTI_Publisher_i *> (retval.in ());
        rti_p->set_impl (rti_pub);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::delete_publisher (::DDS::Publisher_ptr p)
      {
        RTI_Publisher_i *rti_pub = dynamic_cast < RTI_Publisher_i * > (p);

        if (!rti_pub)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::delete_publisher - "
                         "Unable to cast provided object refence to servant pointer.\n"));
            return ::DDS::RETCODE_ERROR;
          }

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTI_DomainParticipant_i::delete_publisher - "
                     "Successfully casted provided object refence to RTI_Publisher_i\n"));

        DDS_ReturnCode_t retval = this->impl ()->delete_publisher (rti_pub->get_impl ());

        if (retval != DDS_RETCODE_OK)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::delete_publisher - "
                         "Error: RTI delete_publisher returned non-ok error code %C\n",
                         translate_retcode (retval)));
          }
        else CIAO_DEBUG (6, (LM_INFO, CLINFO "RTI_DomainParticipant_i::delete_publisher - "
                          "Provided publisher successfully deleted\n"));

        return retval;
      }

      ::DDS::Subscriber_ptr
      RTI_DomainParticipant_i::create_subscriber_with_profile (
        const char* library_name,
        const char *profile_name,
        ::DDS::SubscriberListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_subscriber_with_profile");

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTI_DomainParticipant_i::create_subscriber_with_profile - "
                     "Creating Subscriber\n"));

        RTI_SubscriberListener_i *rti_sl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            rti_sl = new RTI_SubscriberListener_i (a_listener);
          }
        DDSSubscriber * rti_sub =
          this->impl ()->create_subscriber_with_profile (
            library_name,
            profile_name,
            rti_sl,
            mask);

        if (!rti_sub)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::create_subscriber_with_profile - "
                         "Error: Unable to create Subscriber\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        CIAO_DEBUG (6, (LM_INFO, CLINFO "RTI_DomainParticipant_i::create_subscriber_with_profile - "
                     "Successfully created a DDSSubscriber\n"));

        rti_sub->enable ();
        ::DDS::Subscriber_var retval = new RTI_Subscriber_i ();
        RTI_Subscriber_i *rti_s = dynamic_cast < RTI_Subscriber_i *> (retval.in ());
        rti_s->set_impl (rti_sub);

        return retval._retn ();
      }

      ::DDS::Subscriber_ptr
      RTI_DomainParticipant_i::create_subscriber (const ::DDS::SubscriberQos & /*qos*/,
                                                  ::DDS::SubscriberListener_ptr a_listener,
                                                  ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_subscriber");

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTI_DomainParticipant_i::create_subscriber - "
                     "Creating Subscriber\n"));

        RTI_SubscriberListener_i *rti_sl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            rti_sl = new RTI_SubscriberListener_i (a_listener);
          }
        DDSSubscriber * rti_sub =
          this->impl ()->create_subscriber (DDS_SUBSCRIBER_QOS_DEFAULT,
                                            rti_sl,
                                            mask);

        if (!rti_sub)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::create_subscriber - "
                         "Error: Unable to create Subscriber\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        CIAO_DEBUG (6, (LM_INFO, CLINFO "RTI_DomainParticipant_i::create_subscriber - "
                     "Successfully created a DDSSubscriber\n"));

        rti_sub->enable ();
        ::DDS::Subscriber_var retval = new RTI_Subscriber_i ();
        RTI_Subscriber_i *rti_s = dynamic_cast < RTI_Subscriber_i *> (retval.in ());
        rti_s->set_impl (rti_sub);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::delete_subscriber (::DDS::Subscriber_ptr s)
      {
        RTI_Subscriber_i *rti_sub = dynamic_cast < RTI_Subscriber_i * > (s);

        if (!rti_sub)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::delete_subscriber - "
                         "Unable to cast provided object refence to servant pointer.\n"));
            return ::DDS::RETCODE_ERROR;
          }

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTI_DomainParticipant_i::delete_subscriber - "
                     "Successfully casted provided object refence to RTI_Subscriber_i\n"));

        DDS_ReturnCode_t retval = this->impl ()->delete_subscriber (rti_sub->get_impl ());

        if (retval != DDS_RETCODE_OK)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::delete_subscriber - "
                         "Error: RTI delete_subscriber returned non-ok error code %C\n",
                         translate_retcode (retval)));
          }
        else CIAO_DEBUG (6, (LM_INFO, CLINFO "RTI_DomainParticipant_i::delete_subscriber - "
                          "Provided subscriber successfully deleted\n"));

        return retval;
      }

      ::DDS::Subscriber_ptr
      RTI_DomainParticipant_i::get_builtin_subscriber (void)
      {
        DDSSubscriber* rti_sub = this->impl ()->get_builtin_subscriber ();
        ::DDS::Subscriber_var retval = new RTI_Subscriber_i ();
        RTI_Subscriber_i *rti_s = dynamic_cast < RTI_Subscriber_i *> (retval.in ());
        rti_s->set_impl (rti_sub);
        return retval._retn ();
      }

      ::DDS::Topic_ptr
      RTI_DomainParticipant_i::create_topic (const char * impl_name,
                                             const char * type_name,
                                             const ::DDS::TopicQos & /*qos*/,
                                             ::DDS::TopicListener_ptr a_listener,
                                             ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_topic");

        if (impl_name == 0)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic - "
                         "Error: provided nil topic name\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER,
                                          0);
          }

        if (type_name == 0)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic - "
                         "Error: provided nil type name\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER,
                                          0);
          }

        CIAO_DEBUG (6, (LM_DEBUG, CLINFO "DDS_DomainParticipant_i::create_topic - "
                     "Attempting to create topic with name %C and type %C\n",
                     impl_name, type_name));

        RTI_TopicListener_i *rti_tl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            rti_tl = new RTI_TopicListener_i (a_listener);
          }

        DDSTopic *rti_topic = this->impl ()->create_topic (impl_name,
                                                         type_name,
                                                         DDS_TOPIC_QOS_DEFAULT,
                                                         rti_tl,
                                                         mask);
        if (rti_topic == 0)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic - "
                         "Error: RTI DDS returned a nil topic\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        CIAO_DEBUG (6, (LM_INFO, CLINFO "DDS_DomainParticipant_i::create_topic - "
                     "Successfully created topic with name %C and type %C\n",
                     impl_name, type_name));

        ::DDS::Topic_var retval = new RTI_Topic_i ();
        RTI_Topic_i *tp = dynamic_cast < RTI_Topic_i *> (retval.in ());
        tp->set_impl (rti_topic);

        return retval._retn ();
      }

      ::DDS::Topic_ptr
      RTI_DomainParticipant_i::create_topic_with_profile (
          const char *impl_name,
          const char *type_name,
          const char *library_name,
          const char *profile_name,
          ::DDS::TopicListener_ptr a_listener,
          ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_topic_with_profile");

        if (impl_name == 0)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic_with_profile - "
                         "Error: provided nil topic name\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER,
                                          0);
          }

        if (type_name == 0)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic_with_profile - "
                         "Error: provided nil type name\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER,
                                          0);
          }

        CIAO_DEBUG (6, (LM_DEBUG, CLINFO "DDS_DomainParticipant_i::create_topic_with_profile - "
                     "Attempting to create topic with name %C and type %C\n",
                     impl_name, type_name));

        RTI_TopicListener_i *rti_tl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            rti_tl = new RTI_TopicListener_i (a_listener);
          }
        DDSTopic *rti_topic = this->impl ()->create_topic_with_profile (impl_name,
                                                         type_name,
                                                         library_name,
                                                         profile_name,
                                                         rti_tl,
                                                         mask);

        if (rti_topic == 0)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic - "
                         "Error: RTI DDS returned a nil topic\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        CIAO_DEBUG (6, (LM_INFO, CLINFO "DDS_DomainParticipant_i::create_topic - "
                     "Successfully created topic with name %C and type %C\n",
                     impl_name, type_name));

        ::DDS::Topic_var retval = new RTI_Topic_i ();
        RTI_Topic_i *tp = dynamic_cast < RTI_Topic_i *> (retval.in ());
        tp->set_impl (rti_topic);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::delete_topic (::DDS::Topic_ptr a_topic)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::delete_topic");

        RTI_Topic_i *top = dynamic_cast< RTI_Topic_i *> (a_topic);

        if (top == 0)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::delete_topic - "
                         "Unable to cast provided object reference to servant.\n"));
            return ::DDS::RETCODE_BAD_PARAMETER;
          }

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTI_DomainParticipant_i::delete_topic - "
                     "Successfully casted provided object reference to servant.\n"));

        DDS_ReturnCode_t retval = this->impl ()->delete_topic (top->get_impl ());

        if (retval != DDS_RETCODE_OK)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::delete_topic - "
                         "Error: RTI delete_topic returned non-ok error code %C\n",
                         translate_retcode (retval)));
          }
        else CIAO_DEBUG (6, (LM_INFO, CLINFO "RTI_DomainParticipant_i::delete_topic - "
                          "Provided topic successfully deleted\n"));

        return retval;
      }

      ::DDS::Topic_ptr
      RTI_DomainParticipant_i::find_topic (const char * impl_name,
                                           const ::DDS::Duration_t & timeout)
      {
        ::DDS_Duration_t ddstimeout;
        ddstimeout <<= timeout;
        ::DDSTopic* rti_topic = this->impl ()->find_topic (impl_name, ddstimeout);
        ::DDS::Topic_var retval = new RTI_Topic_i ();
        RTI_Topic_i *tp = dynamic_cast < RTI_Topic_i *> (retval.in ());
        tp->set_impl (rti_topic);
        return retval._retn ();
      }

      ::DDS::TopicDescription_ptr
      RTI_DomainParticipant_i::lookup_topicdescription (const char * name)
      {
        ::DDSTopicDescription* rti_topic = this->impl ()->lookup_topicdescription (name);
        ::DDS::TopicDescription_var retval = new RTI_TopicDescription_i ();
        RTI_TopicDescription_i *rti_td = dynamic_cast < RTI_TopicDescription_i *>(retval.in ());
        rti_td->set_impl (rti_topic);
        return retval._retn ();
      }

      ::DDS::ContentFilteredTopic_ptr
      RTI_DomainParticipant_i::create_contentfilteredtopic (const char * name,
                                                            ::DDS::Topic_ptr related_topic,
                                                            const char * filter_expression,
                                                            const ::DDS::StringSeq & expression_parameters)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_contentfilteredtopic");

        RTI_Topic_i *top = dynamic_cast< RTI_Topic_i *> (related_topic);
        if (!top)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::create_contentfilteredtopic - "
                         "Unable to cast provided topic.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }
        const char* parameterlist[expression_parameters.length ()];
        for (CORBA::ULong i = 0; i < expression_parameters.length (); ++i)
          {
            parameterlist[i] = expression_parameters[i].in ();
          }
        DDS_StringSeq parameters (expression_parameters.length ());
        parameters.from_array(parameterlist, expression_parameters.length ());

        DDSContentFilteredTopic * rti_cft = this->impl ()->create_contentfilteredtopic (
                                                                    name,
                                                                    top->get_impl (),
                                                                    filter_expression,
                                                                    parameters);
        if (!rti_cft)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_DomainParticipant_i::create_contentfilteredtopic - "
                         "RTI DDS returned a nil ContentFilteredTopic.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }
        CIAO_DEBUG (6, (LM_INFO, CLINFO "DDS_DomainParticipant_i::create_contentfilteredtopic - "
                     "Successfully created topic with name %C and expresion %C\n",
                     name, filter_expression));

        ::DDS::ContentFilteredTopic_var retval = new RTI_ContentFilteredTopic_i ();
        RTI_ContentFilteredTopic_i *tp = dynamic_cast < RTI_ContentFilteredTopic_i *> (retval.in ());
        tp->set_impl (rti_cft);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::delete_contentfilteredtopic (::DDS::ContentFilteredTopic_ptr /*a_contentfilteredtopic*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::delete_contentfilteredtopic");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::MultiTopic_ptr
      RTI_DomainParticipant_i::create_multitopic (const char * /*name*/,
                                                  const char * /*type_name*/,
                                                  const char * /*subscription_expression*/,
                                                  const ::DDS::StringSeq & /*expression_parameters*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_multitopic");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::delete_multitopic (::DDS::MultiTopic_ptr /*a_multitopic*/)
      {
        //this->impl ()->delete_multitopic (
        CIAO_TRACE ("DDS_DomainParticipant_i::delete_multitopic");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::delete_contained_entities (void)
      {
        return this->impl ()->delete_contained_entities ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::set_qos (const ::DDS::DomainParticipantQos & /*qos*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::set_qos");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::get_qos (::DDS::DomainParticipantQos & /*qos*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_qos");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::set_listener (::DDS::DomainParticipantListener_ptr a_listener,
                                             ::DDS::StatusMask mask)
      {
        RTI_DomainParticipantListener_i* rti_impl_list = new RTI_DomainParticipantListener_i (a_listener);
        return this->impl_->set_listener (rti_impl_list, mask);
      }

      ::DDS::DomainParticipantListener_ptr
      RTI_DomainParticipant_i::get_listener (void)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_listener");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::ignore_participant (const ::DDS::InstanceHandle_t & handle)
      {
        ::DDS_InstanceHandle_t rti_handle;
        rti_handle <<= handle;
        return this->impl ()->ignore_participant (rti_handle);

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::ignore_topic (const ::DDS::InstanceHandle_t & handle)
      {
        ::DDS_InstanceHandle_t rti_handle;
        rti_handle <<= handle;
        return this->impl ()->ignore_topic (rti_handle);
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::ignore_publication (const ::DDS::InstanceHandle_t & handle)
      {
        ::DDS_InstanceHandle_t rti_handle;
        rti_handle <<= handle;
        return this->impl ()->ignore_publication (rti_handle);
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::ignore_subscription (const ::DDS::InstanceHandle_t & handle)
      {
        ::DDS_InstanceHandle_t rti_handle;
        rti_handle <<= handle;
        return this->impl ()->ignore_subscription (rti_handle);
      }

      ::DDS::DomainId_t
      RTI_DomainParticipant_i::get_domain_id (void)
      {
        return this->impl ()->get_domain_id ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::assert_liveliness (void)
      {
        return this->impl ()->assert_liveliness ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::set_default_publisher_qos (const ::DDS::PublisherQos & /*qos*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::set_default_publisher_qos");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::get_default_publisher_qos (::DDS::PublisherQos & /*qos*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_default_publisher_qos");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::set_default_subscriber_qos (const ::DDS::SubscriberQos & /*qos*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::set_default_subscriber_qos");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::get_default_subscriber_qos (::DDS::SubscriberQos & /*qos*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_default_subscriber_qos");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::set_default_topic_qos (const ::DDS::TopicQos & /*qos*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::set_default_topic_qos");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::get_default_topic_qos (::DDS::TopicQos & /*qos*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_default_topic_qos");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::get_discovered_participants (::DDS::InstanceHandleSeq & impl_handles)
      {
        ::DDS_InstanceHandleSeq rtiseq;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_discovered_participants (rtiseq);
        rtiseq >>= impl_handles;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::get_discovered_participant_data (::DDS::ParticipantBuiltinTopicData & /*impl_data*/,
                                                                const ::DDS::InstanceHandle_t & /*impl_handle*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_discovered_participant_data");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::get_discovered_topics (::DDS::InstanceHandleSeq & impl_handles)
      {
        ::DDS_InstanceHandleSeq rtiseq;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_discovered_topics (rtiseq);
        rtiseq >>= impl_handles;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::get_discovered_topic_data (::DDS::TopicBuiltinTopicData & /*impl_data*/,
                                                          const ::DDS::InstanceHandle_t & /*impl_handle*/)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_discovered_topic_data");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::CORBA::Boolean
      RTI_DomainParticipant_i::contains_entity (const ::DDS::InstanceHandle_t & a_handle)
      {
        ::DDS_InstanceHandle_t rti_handle;
        rti_handle <<= a_handle;
        return this->impl ()->contains_entity (rti_handle);

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::get_current_time (::DDS::Time_t & current_time)
      {
        DDS_Time_t rti_time;
        ::DDS::ReturnCode_t const retval = this->impl ()->get_current_time (rti_time);
        rti_time >>= current_time;
        return retval;

      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::enable (void)
      {
        return this->impl ()->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_DomainParticipant_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i ();
        RTI_StatusCondition_i *rti_sc = dynamic_cast < RTI_StatusCondition_i *> (retval.in ());
        rti_sc->set_impl (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_DomainParticipant_i::get_status_changes (void)
      {
        return this->impl ()->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_DomainParticipant_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
      }

      DDSDomainParticipant *
      RTI_DomainParticipant_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_DomainParticipant_i::set_impl (DDSDomainParticipant * dw)
      {
        this->impl_ = dw;
      }

      DDSDomainParticipant *
      RTI_DomainParticipant_i::impl (void)
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
