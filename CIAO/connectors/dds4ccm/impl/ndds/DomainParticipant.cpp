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
#include "SubscriberQos.h"
#include "TopicQos.h"
#include "DomainParticipantQos.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_DomainParticipant_i::CCM_DDS_DomainParticipant_i (
      DDSDomainParticipant * dp) : impl_ (dp)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipant_i::CCM_DDS_DomainParticipant_i");
    }

    CCM_DDS_DomainParticipant_i::~CCM_DDS_DomainParticipant_i (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipant_i::~CCM_DDS_DomainParticipant_i");
    }

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
    ::DDS::Publisher_ptr
    CCM_DDS_DomainParticipant_i::create_publisher_with_profile (
      const char* library_name,
      const char *profile_name,
      ::DDS::PublisherListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_publisher_with_profile");

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipant_i::create_publisher_with_profile - "
                   "Creating Publisher\n"));
      CCM_DDS_PublisherListener_i *rti_pl = 0;
      if (!CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (rti_pl,
                            CCM_DDS_PublisherListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }
      DDSPublisher * rti_pub =
        this->impl ()->create_publisher_with_profile (library_name,
                                                      profile_name,
                                                      rti_pl,
                                                      mask);

      if (!rti_pub)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::create_publisher_with_profile - "
                       "Error: Unable to create Publisher\n"));
          delete rti_pl;
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::Publisher_var retval = ::DDS::Publisher::_nil ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Publisher_i (rti_pub),
                        CORBA::NO_MEMORY ());


      DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_DomainParticipant_i::create_publisher_with_profile - "
                   "Successfully created a DDSPublisher\n"));

      rti_pub->enable ();
      return retval._retn ();
    }
#endif

    ::DDS::Publisher_ptr
    CCM_DDS_DomainParticipant_i::create_publisher (
      const ::DDS::PublisherQos & qos,
      ::DDS::PublisherListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_publisher");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (qos);
      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipant_i::create_publisher - "
                   "Creating Publisher\n"));

      DDS_PublisherQos rti_qos = DDS_PUBLISHER_QOS_DEFAULT;

      CCM_DDS_PublisherListener_i *rti_pl = 0;
      if (!CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (rti_pl,
                            CCM_DDS_PublisherListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }

      DDSPublisher * rti_pub =
        this->impl ()->create_publisher (rti_qos,
                                         rti_pl,
                                         mask);

      if (!rti_pub)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::create_publisher - "
                       "Error: Unable to create Publisher\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::Publisher_var retval = ::DDS::Publisher::_nil ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Publisher_i (rti_pub),
                        CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_DomainParticipant_i::create_publisher - "
                   "Successfully created a DDSPublisher\n"));

      rti_pub->enable ();
      return retval._retn ();
#else
      return this->impl ()->create_publisher (qos, a_listener, mask);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::delete_publisher (::DDS::Publisher_ptr p)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_Publisher_i *rti_pub = dynamic_cast < CCM_DDS_Publisher_i * > (p);

      if (!rti_pub)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::delete_publisher - "
                       "Unable to cast provided object refence to servant pointer.\n"));
          return ::DDS::RETCODE_ERROR;
        }

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipant_i::delete_publisher - "
                   "Successfully casted provided object refence to CCM_DDS_Publisher_i\n"));

      DDS_ReturnCode_t const retval = this->impl ()->delete_publisher (rti_pub->get_impl ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::delete_publisher - "
                       "Error: RTI delete_publisher returned non-ok error code %C\n",
                       translate_retcode (retval)));
        }
      else DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_DomainParticipant_i::delete_publisher - "
                        "Provided publisher successfully deleted\n"));

      return retval;
#else
      return this->impl ()->delete_publisher (p);
#endif
    }

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
    ::DDS::Subscriber_ptr
    CCM_DDS_DomainParticipant_i::create_subscriber_with_profile (
      const char* library_name,
      const char *profile_name,
      ::DDS::SubscriberListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_subscriber_with_profile");

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipant_i::create_subscriber_with_profile - "
                   "Creating Subscriber\n"));

      CCM_DDS_SubscriberListener_i *rti_sl = 0;
      if (!CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (rti_sl,
                            CCM_DDS_SubscriberListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }
      DDSSubscriber * rti_sub =
        this->impl ()->create_subscriber_with_profile (
          library_name,
          profile_name,
          rti_sl,
          mask);

      if (!rti_sub)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::create_subscriber_with_profile - "
                       "Error: Unable to create Subscriber\n"));
          delete rti_sl;
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::Subscriber_var retval = ::DDS::Subscriber::_nil ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Subscriber_i (rti_sub),
                        CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_DomainParticipant_i::create_subscriber_with_profile - "
                   "Successfully created a DDSSubscriber\n"));

      rti_sub->enable ();
      return retval._retn ();
    }
#endif

    ::DDS::Subscriber_ptr
    CCM_DDS_DomainParticipant_i::create_subscriber (const ::DDS::SubscriberQos & qos,
                                                ::DDS::SubscriberListener_ptr a_listener,
                                                ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_subscriber");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (qos);

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipant_i::create_subscriber - "
                   "Creating Subscriber\n"));

      CCM_DDS_SubscriberListener_i *rti_sl = 0;
      if (!CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (rti_sl,
                            CCM_DDS_SubscriberListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }

      DDS_SubscriberQos rti_qos = DDS_SUBSCRIBER_QOS_DEFAULT;
      DDSSubscriber * rti_sub =
        this->impl ()->create_subscriber (rti_qos,
                                          rti_sl,
                                          mask);

      if (!rti_sub)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::create_subscriber - "
                       "Error: Unable to create Subscriber\n"));
          delete rti_sl;
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::Subscriber_var retval = ::DDS::Subscriber::_nil ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Subscriber_i (rti_sub),
                        CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_DomainParticipant_i::create_subscriber - "
                   "Successfully created a DDSSubscriber\n"));

      rti_sub->enable ();
      return retval._retn ();
#else
      return this->impl ()->create_subscriber (qos, a_listener, mask);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::delete_subscriber (::DDS::Subscriber_ptr s)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_Subscriber_i *rti_sub = dynamic_cast < CCM_DDS_Subscriber_i * > (s);

      if (!rti_sub)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::delete_subscriber - "
                       "Unable to cast provided object refence to servant pointer.\n"));
          return ::DDS::RETCODE_ERROR;
        }

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipant_i::delete_subscriber - "
                   "Successfully casted provided object refence to CCM_DDS_Subscriber_i\n"));

      DDS_ReturnCode_t const retval = this->impl ()->delete_subscriber (rti_sub->get_impl ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::delete_subscriber - "
                       "Error: RTI delete_subscriber returned non-ok error code %C\n",
                       translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_DomainParticipant_i::delete_subscriber - "
                        "Provided subscriber successfully deleted\n"));
        }

      return retval;
#else
      return this->impl ()->delete_subscriber (s);
#endif
    }

    ::DDS::Subscriber_ptr
    CCM_DDS_DomainParticipant_i::get_builtin_subscriber (void)
    {
      ::DDS::Subscriber_var retval = ::DDS::Subscriber::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDSSubscriber* sub = this->impl ()->get_builtin_subscriber ();
      if (sub)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_Subscriber_i (sub),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDS::Subscriber_var sub = this->impl ()->get_builtin_subscriber ();
      if (!CORBA::is_nil (sub.in ()))
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_Subscriber_i (sub.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return retval._retn ();
    }

    ::DDS::Topic_ptr
    CCM_DDS_DomainParticipant_i::create_topic (const char * impl_name,
                                           const char * type_name,
                                           const ::DDS::TopicQos & qos,
                                           ::DDS::TopicListener_ptr a_listener,
                                           ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_topic");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (qos);

      if (impl_name == 0)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic - "
                       "Error: provided nil topic name\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER,
                                        0);
        }

      if (type_name == 0)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic - "
                       "Error: provided nil type name\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER,
                                        0);
        }

      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "DDS_DomainParticipant_i::create_topic - "
                   "Attempting to create topic with name %C and type %C\n",
                   impl_name, type_name));

      CCM_DDS_TopicListener_i *rti_tl = 0;
      if (!CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (rti_tl,
                            CCM_DDS_TopicListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }

      DDS_TopicQos rti_qos = DDS_TOPIC_QOS_DEFAULT;

      DDSTopic *rti_topic = this->impl ()->create_topic (impl_name,
                                                      type_name,
                                                      rti_qos,
                                                      rti_tl,
                                                      mask);

      if (rti_topic == 0)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic - "
                      "Error: RTI DDS returned a nil topic\n"));
          delete rti_tl;
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Topic_i (rti_topic),
                        CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "DDS_DomainParticipant_i::create_topic - "
                  "Successfully created topic with name %C and type %C\n",
                  impl_name, type_name));

      return retval._retn ();
#else
      return this->impl ()->create_topic (impl_name,
                                                      type_name,
                                                      qos,
                                                      a_listener,
                                                      mask);
#endif
    }

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
    ::DDS::Topic_ptr
    CCM_DDS_DomainParticipant_i::create_topic_with_profile (
        const char *impl_name,
        const char *type_name,
        const char *library_name,
        const char *profile_name,
        ::DDS::TopicListener_ptr a_listener,
        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_topic_with_profile");

      if (impl_name == 0)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic_with_profile - "
                       "Error: provided nil topic name\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
        }

      if (type_name == 0)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic_with_profile - "
                       "Error: provided nil type name\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
        }

      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "DDS_DomainParticipant_i::create_topic_with_profile - "
                   "Attempting to create topic with name %C and type %C\n",
                   impl_name, type_name));

      ::DDS::Duration_t dur = { 0, 1000000 };
      ::DDS::Topic_var tp = this->find_topic (impl_name, dur);
      if (CORBA::is_nil (tp))
        {
          CCM_DDS_TopicListener_i *rti_tl = 0;
          if (!CORBA::is_nil (a_listener))
            {
              ACE_NEW_THROW_EX (rti_tl,
                                CCM_DDS_TopicListener_i (a_listener),
                                CORBA::NO_MEMORY ());
            }
          DDSTopic *rti_topic = this->impl ()->create_topic_with_profile (impl_name,
                                                          type_name,
                                                          library_name,
                                                          profile_name,
                                                          rti_tl,
                                                          mask);

          if (rti_topic == 0)
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic_with_profile - "
                          "Error: RTI DDS returned a nil topic\n"));
              delete rti_tl;
              throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
            }

          ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_Topic_i (rti_topic),
                            CORBA::NO_MEMORY ());

          DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "DDS_DomainParticipant_i::create_topic_with_profile - "
                      "Successfully created topic with name %C and type %C\n",
                      impl_name, type_name));

          return retval._retn ();
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "DDS_DomainParticipant_i::create_topic_with_profile - "
                      "Re-using topic  with name %C and type %C.\n",
                      impl_name, type_name));
          return ::DDS::Topic::_duplicate (tp.in ());
        }
    }
#endif

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::delete_topic (::DDS::Topic_ptr a_topic)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::delete_topic");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_Topic_i *top = dynamic_cast< CCM_DDS_Topic_i *> (a_topic);

      if (top == 0)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::delete_topic - "
                       "Unable to cast provided object reference to servant.\n"));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipant_i::delete_topic - "
                   "Successfully casted provided object reference to servant.\n"));

      DDS_ReturnCode_t const retval = this->impl ()->delete_topic (top->get_impl ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::delete_topic - "
                       "Error: RTI delete_topic returned non-ok error code %C\n",
                       translate_retcode (retval)));
        }
      else
        {
           DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_DomainParticipant_i::delete_topic - "
                        "Provided topic successfully deleted\n"));
        }

      return retval;
#else
      return this->impl ()->delete_topic (a_topic);
#endif
    }

    ::DDS::Topic_ptr
    CCM_DDS_DomainParticipant_i::find_topic (const char * impl_name,
                                         const ::DDS::Duration_t & timeout)
    {
      ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_Duration_t ddstimeout;
      ddstimeout <<= timeout;
      ::DDSTopic* topic = this->impl ()->find_topic (impl_name, ddstimeout);
      if (topic)
        {
          ACE_NEW_RETURN (retval,
                          CCM_DDS_Topic_i (topic),
                          ::DDS::Topic::_nil ());
        }
#else
      ::DDS::Topic_var topic = this->impl ()->find_topic (impl_name, timeout);
      if (!CORBA::is_nil (topic.in ())
        {
          ACE_NEW_RETURN (retval,
                          CCM_DDS_Topic_i (topic.in ()),
                          ::DDS::Topic::_nil ());
        }
#endif
      return retval._retn ();
    }

    ::DDS::TopicDescription_ptr
    CCM_DDS_DomainParticipant_i::lookup_topicdescription (const char * name)
    {
      ::DDS::TopicDescription_var retval = ::DDS::TopicDescription::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDSTopicDescription* topic = this->impl ()->lookup_topicdescription (name);
      if (topic)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_TopicDescription_i (topic),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDSTopicDescription_var topic = this->impl ()->lookup_topicdescription (name);
      if (!CORBA::is_ni (topic.in ()))
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_TopicDescription_i (topic.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return retval._retn ();
    }

    ::DDS::ContentFilteredTopic_ptr
    CCM_DDS_DomainParticipant_i::create_contentfilteredtopic (const char * name,
                                                          ::DDS::Topic_ptr related_topic,
                                                          const char * filter_expression,
                                                          const ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_contentfilteredtopic");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_Topic_i *top = dynamic_cast< CCM_DDS_Topic_i *> (related_topic);
      if (!top)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::create_contentfilteredtopic - "
                       "Unable to cast provided topic.\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      const char** parameterlist = 0;
      ACE_NEW_THROW_EX (parameterlist,
                        const char*[expression_parameters.length ()],
                        CORBA::NO_MEMORY ());

      for (CORBA::ULong i = 0; i < expression_parameters.length (); ++i)
        {
          parameterlist[i] = expression_parameters[i].in ();
        }
      DDS_StringSeq parameters (expression_parameters.length ());
      parameters.from_array (parameterlist, expression_parameters.length ());

      DDSContentFilteredTopic * rti_cft = this->impl ()->create_contentfilteredtopic (
                                                                  name,
                                                                  top->get_impl (),
                                                                  filter_expression,
                                                                  parameters);
      delete [] parameterlist;
      if (!rti_cft)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipant_i::create_contentfilteredtopic - "
                       "RTI DDS returned a nil ContentFilteredTopic.\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::ContentFilteredTopic_var retval = ::DDS::ContentFilteredTopic::_nil ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_ContentFilteredTopic_i (rti_cft),
                        CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "DDS_DomainParticipant_i::create_contentfilteredtopic - "
                   "Successfully created topic with name <%C> and filter expression <%C>\n",
                   name, filter_expression));

      return retval._retn ();
#else
      return this->impl ()->create_contentfilteredtopic (
                                                                  name,
                                                                  related_topic,
                                                                  filter_expression,
                                                                  expression_parameters);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::delete_contentfilteredtopic (::DDS::ContentFilteredTopic_ptr /*a_contentfilteredtopic*/)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::delete_contentfilteredtopic");
      throw CORBA::NO_IMPLEMENT ();

    }

    ::DDS::MultiTopic_ptr
    CCM_DDS_DomainParticipant_i::create_multitopic (const char * /*name*/,
                                                const char * /*type_name*/,
                                                const char * /*subscription_expression*/,
                                                const ::DDS::StringSeq & /*expression_parameters*/)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_multitopic");
      throw CORBA::NO_IMPLEMENT ();

    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::delete_multitopic (::DDS::MultiTopic_ptr /*a_multitopic*/)
    {
      //this->impl ()->delete_multitopic (
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::delete_multitopic");
      throw CORBA::NO_IMPLEMENT ();

    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::delete_contained_entities (void)
    {
      return this->impl ()->delete_contained_entities ();

    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::set_qos (const ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DomainParticipantQos rti_qos;
      rti_qos <<= qos;
      return this->impl()->set_qos (rti_qos);
#else
      return this->impl()->set_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::get_qos (::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DomainParticipantQos rti_qos;
      ::DDS::ReturnCode_t retcode = this->impl()-> get_qos (rti_qos);
      qos <<= rti_qos;
      return retcode;
#else
      return this->impl()->get_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::set_listener (::DDS::DomainParticipantListener_ptr a_listener,
                                           ::DDS::StatusMask mask)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_DomainPublisherListener_i* rti_impl_list = 0;
      if (!CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (rti_impl_list,
                            CCM_DDS_DomainPublisherListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }
      return this->impl_->set_listener (rti_impl_list, mask);
#else
      return this->impl_->set_listener (a_listener, mask);
#endif
    }

    ::DDS::DomainParticipantListener_ptr
    CCM_DDS_DomainParticipant_i::get_listener (void)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_listener");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipantListener *rti_dp_list = this->impl ()->get_listener ();
      CCM_DDS_DomainPublisherListener_i *list_proxy = dynamic_cast <CCM_DDS_DomainPublisherListener_i *> (rti_dp_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, "CCM_DDS_DomainParticipant_i::get_listener - "
                                    "DDS returned a NIL listener.\n"));
          return ::DDS::DomainParticipantListener::_nil ();
        }
      return list_proxy->get_domainparticipantlistener ();
#else
      return this->impl ()->get_listener ();
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::ignore_participant (DDS_INSTANCE_HANDLE_T_IN handle)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandle_t rti_handle;
      rti_handle <<= handle;
      return this->impl ()->ignore_participant (rti_handle);
#else
      return this->impl ()->ignore_participant (handle);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::ignore_topic (DDS_INSTANCE_HANDLE_T_IN handle)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandle_t rti_handle;
      rti_handle <<= handle;
      return this->impl ()->ignore_topic (rti_handle);
#else
      return this->impl ()->ignore_topic (handle);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::ignore_publication (DDS_INSTANCE_HANDLE_T_IN handle)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandle_t rti_handle;
      rti_handle <<= handle;
      return this->impl ()->ignore_publication (rti_handle);
#else
      return this->impl ()->ignore_publication (handle);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::ignore_subscription (DDS_INSTANCE_HANDLE_T_IN handle)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandle_t rti_handle;
      rti_handle <<= handle;
      return this->impl ()->ignore_subscription (rti_handle);
#else
      return this->impl ()->ignore_subscription (handle);
#endif
    }

    ::DDS::DomainId_t
    CCM_DDS_DomainParticipant_i::get_domain_id (void)
    {
      return this->impl ()->get_domain_id ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::assert_liveliness (void)
    {
      return this->impl ()->assert_liveliness ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::set_default_publisher_qos (const ::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_default_publisher_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_PublisherQos rti_qos;
      rti_qos <<= qos;
      return this->impl()->set_default_publisher_qos (rti_qos);
#else
      return this->impl()->set_default_publisher_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::get_default_publisher_qos (::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_default_publisher_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_PublisherQos rti_qos;
      ::DDS::ReturnCode_t retcode = this->impl()-> get_default_publisher_qos (rti_qos);
      qos <<= rti_qos;
      return retcode;
#else
      return this->impl()->get_default_publisher_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::set_default_subscriber_qos (const ::DDS::SubscriberQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_default_subscriber_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_SubscriberQos rti_qos;
      rti_qos <<= qos;
      return this->impl()->set_default_subscriber_qos (rti_qos);
#else
      return this->impl()->set_default_subscriber_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::get_default_subscriber_qos (::DDS::SubscriberQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_default_subscriber_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_SubscriberQos rti_qos;
      ::DDS::ReturnCode_t retcode =
            this->impl()-> get_default_subscriber_qos (rti_qos);
      qos <<= rti_qos;
      return retcode;
#else
      return this->impl()->get_default_subscriber_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::set_default_topic_qos (const ::DDS::TopicQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_default_topic_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_TopicQos rti_qos;
      rti_qos <<= qos;
      return this->impl()->set_default_topic_qos (rti_qos);
#else
      return this->impl()->set_default_topic_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::get_default_topic_qos (::DDS::TopicQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_default_topic_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_TopicQos rti_qos;
      ::DDS::ReturnCode_t retcode =
            this->impl()-> get_default_topic_qos (rti_qos);
      qos <<= rti_qos;
      return retcode;
#else
      return this->impl()->get_default_topic_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::get_discovered_participants (::DDS::InstanceHandleSeq & impl_handles)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandleSeq rtiseq;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_discovered_participants (rtiseq);
      rtiseq >>= impl_handles;
      return retval;
#else
      return this->impl ()->get_discovered_participants (impl_handles);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::get_discovered_participant_data (::DDS::ParticipantBuiltinTopicData & /*impl_data*/,
                                                              DDS_INSTANCE_HANDLE_T_IN /*impl_handle*/)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_discovered_participant_data");
      throw CORBA::NO_IMPLEMENT ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::get_discovered_topics (::DDS::InstanceHandleSeq & impl_handles)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandleSeq rtiseq;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_discovered_topics (rtiseq);
      rtiseq >>= impl_handles;
      return retval;
#else
      return this->impl ()->get_discovered_topics (impl_handles);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::get_discovered_topic_data (::DDS::TopicBuiltinTopicData & /*impl_data*/,
                                                        DDS_INSTANCE_HANDLE_T_IN /*impl_handle*/)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_discovered_topic_data");
      throw CORBA::NO_IMPLEMENT ();
    }

    ::CORBA::Boolean
    CCM_DDS_DomainParticipant_i::contains_entity (DDS_INSTANCE_HANDLE_T_IN a_handle)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandle_t rti_handle;
      rti_handle <<= a_handle;
      return this->impl ()->contains_entity (rti_handle);
#else
      return this->impl ()->contains_entity (a_handle);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::get_current_time (::DDS::Time_t & current_time)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDS_Time_t rti_time;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_current_time (rti_time);
      rti_time >>= current_time;
      return retval;
#else
      return this->impl ()->get_current_time (current_time);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_i::enable (void)
    {
      return this->impl ()->enable ();
    }

    ::DDS::StatusCondition_ptr
    CCM_DDS_DomainParticipant_i::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval = ::DDS::StatusCondition::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
      if (sc)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_StatusCondition_i (sc),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDS::StatusCondition_var sc = this->impl ()->get_statuscondition ();
      if (!CORBA::is_nil (sc.in ()))
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_StatusCondition_i (sc.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return retval._retn ();
    }

    ::DDS::StatusMask
    CCM_DDS_DomainParticipant_i::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    DDS_INSTANCE_HANDLE_T_RETN
    CCM_DDS_DomainParticipant_i::get_instance_handle (void)
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

    DDSDomainParticipant *
    CCM_DDS_DomainParticipant_i::get_impl (void)
    {
      return this->impl_;
    }

    void
    CCM_DDS_DomainParticipant_i::set_impl (DDSDomainParticipant * dp)
    {
      this->impl_ = dp;
    }

    DDSDomainParticipant *
    CCM_DDS_DomainParticipant_i::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}
