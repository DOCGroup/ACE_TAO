// $Id$

#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/Subscriber.h"
#include "dds4ccm/impl/ndds/Publisher.h"
#include "dds4ccm/impl/ndds/ContentFilteredTopic.h"
#include "dds4ccm/impl/ndds/TopicDescription.h"
#include "dds4ccm/impl/ndds/Topic.h"
#include "dds4ccm/impl/ndds/DomainParticipantListener.h"
#include "dds4ccm/impl/ndds/PublisherListener.h"
#include "dds4ccm/impl/ndds/SubscriberListener.h"
#include "dds4ccm/impl/ndds/StatusCondition.h"

#include "dds4ccm/impl/ndds/TopicListener.h"
#include "dds4ccm/impl/ndds/DomainParticipantManager.h"
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/convertors/PublisherQos.h"
#include "dds4ccm/impl/ndds/convertors/SubscriberQos.h"
#include "dds4ccm/impl/ndds/convertors/TopicQos.h"
#include "dds4ccm/impl/ndds/convertors/DomainParticipantQos.h"
#include "dds4ccm/impl/ndds/convertors/InstanceHandle_t.h"
#include "dds4ccm/impl/ndds/convertors/Duration_t.h"
#include "dds4ccm/impl/ndds/convertors/Time_t.h"
#include "dds4ccm/impl/ndds/convertors/InstanceHandleSeq.h"
#include "dds4ccm/impl/ndds/convertors/TopicBuiltinTopicData.h"
#include "dds4ccm/impl/ndds/convertors/ParticipantBuiltinTopicData.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

#include "ace/CORBA_macros.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_DomainParticipant_i::DDS_DomainParticipant_i (
      ::DDSDomainParticipant * dp) : rti_entity_ (dp)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::DDS_DomainParticipant_i");
    }

    DDS_DomainParticipant_i::~DDS_DomainParticipant_i (void)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::~DDS_DomainParticipant_i");
    }

    ::DDS::Publisher_ptr
    DDS_DomainParticipant_i::create_publisher_with_profile (
      const char * qos_profile,
      ::DDS::PublisherListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_publisher_with_profile");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::create_publisher_with_profile <%C> - "
                    "Start creating Publisher\n",
                    qos_profile));
      DDS_PublisherListener_i *ccm_dds_pl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_pl,
                            DDS_PublisherListener_i (a_listener, this),
                            ::CORBA::NO_MEMORY ());
        }
      char * lib_name = get_library_name(qos_profile);
      char * prof_name = get_profile_name(qos_profile);

      DDSPublisher * ccm_dds_pub = 0;

      if (lib_name != 0 && prof_name != 0)
        {
          ccm_dds_pub = this->rti_entity ()->create_publisher_with_profile (
                                                        lib_name,
                                                        prof_name,
                                                        ccm_dds_pl,
                                                        mask);
        }

      ACE_OS::free (lib_name);
      ACE_OS::free (prof_name);

      if (!ccm_dds_pub)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_publisher_with_profile <%C> - "
                        "Error: Unable to create Publisher\n",
                        qos_profile));
          delete ccm_dds_pl;
          return ::DDS::Publisher::_nil ();
        }

      ::DDS::Publisher_var retval;
      ACE_NEW_THROW_EX (retval,
                        DDS_Publisher_i (ccm_dds_pub, this),
                        ::CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::create_publisher_with_profile <%C> - "
                    "Successfully created a DDSPublisher\n",
                    qos_profile));

      DDS_ReturnCode_t retcode = ccm_dds_pub->enable ();
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                       "DDS_DomainParticipant_i"
                       "::create_publisher_with_profile <%C> - "
                       "Error: Unable to create Publisher: <%C>\n",
                       qos_profile,
                       ::CIAO::DDS4CCM::translate_retcode (retcode)));
          delete ccm_dds_pl;
          throw ::CORBA::INTERNAL ();
        }
      return retval._retn ();
    }


    ::DDS::Publisher_ptr
    DDS_DomainParticipant_i::create_publisher (
      const ::DDS::PublisherQos & qos,
      ::DDS::PublisherListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_publisher");
      ACE_UNUSED_ARG (qos);
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::create_publisher - "
                    "Start creating Publisher\n"));

      DDS_PublisherQos ccm_dds_qos = DDS_PUBLISHER_QOS_DEFAULT;

      DDS_PublisherListener_i *ccm_dds_pl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_pl,
                            DDS_PublisherListener_i (a_listener, this),
                            ::CORBA::NO_MEMORY ());
        }

      DDSPublisher * ccm_dds_pub =
        this->rti_entity ()->create_publisher (ccm_dds_qos,
                                         ccm_dds_pl,
                                         mask);

      if (!ccm_dds_pub)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                       "DDS_DomainParticipant_i"
                       "::create_publisher - "
                       "Error: Unable to create Publisher\n"));
          delete ccm_dds_pl;
          return ::DDS::Publisher::_nil ();
        }

      ::DDS::Publisher_var retval;
      ACE_NEW_THROW_EX (retval,
                        DDS_Publisher_i (ccm_dds_pub, this),
                        ::CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::create_publisher - "
                    "Successfully created a DDSPublisher\n"));

      DDS_ReturnCode_t retcode = ccm_dds_pub->enable ();
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                       "DDS_DomainParticipant_i"
                       "::create_publisher - "
                       "Error: Unable to enable the Publisher: <%C>\n",
                       ::CIAO::DDS4CCM::translate_retcode (retcode)));
          delete ccm_dds_pl;
          throw ::CORBA::INTERNAL ();
        }
      return retval._retn ();
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::delete_publisher (
      ::DDS::Publisher_ptr p)
    {
      DDS_Publisher_i *ccm_dds_pub =
        dynamic_cast < DDS_Publisher_i * > (p);

      if (!ccm_dds_pub)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i"
                        "::delete_publisher - "
                        "Unable to cast provided object reference to servant pointer.\n"));
          return ::DDS::RETCODE_ERROR;
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_SUCCESSFUL, (LM_TRACE, DDS4CCM_INFO
                   "DDS_DomainParticipant_i"
                   "::delete_publisher - "
                   "Successfully casted provided object reference to "
                   "DDS_Publisher_i\n"));

      DDS_ReturnCode_t const retval = this->rti_entity ()->delete_publisher (ccm_dds_pub->get_rti_entity ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i"
                        "::delete_publisher - "
                        "Error: RTI delete_publisher returned non-ok error code %C\n",
                        ::CIAO::DDS4CCM::translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::delete_publisher - "
                        "Provided publisher successfully deleted\n"));
        }

      return retval;
    }


    ::DDS::Subscriber_ptr
    DDS_DomainParticipant_i::create_subscriber_with_profile (
      const char * qos_profile,
      ::DDS::SubscriberListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_subscriber_with_profile");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                   "DDS_DomainParticipant_i"
                   "::create_subscriber_with_profile <%C> - "
                   "Creating Subscriber\n",
                   qos_profile));

      DDS_SubscriberListener_i *ccm_dds_sl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_sl,
                            DDS_SubscriberListener_i (a_listener, this),
                            ::CORBA::NO_MEMORY ());
        }
      char * lib_name = get_library_name(qos_profile);
      char * prof_name = get_profile_name(qos_profile);

      DDSSubscriber * ccm_dds_sub = 0;

      if (lib_name != 0 && prof_name != 0)
        {
          ccm_dds_sub = this->rti_entity ()->create_subscriber_with_profile (
                                                        lib_name,
                                                        prof_name,
                                                        ccm_dds_sl,
                                                        mask);
        }

      ACE_OS::free (lib_name);
      ACE_OS::free (prof_name);

      if (!ccm_dds_sub)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::"
                        "create_subscriber_with_profile <%C> - "
                        "Error: RTI DDS returned a nil subscriber.\n",
                        qos_profile));
          delete ccm_dds_sl;
          return ::DDS::Subscriber::_nil ();
        }

      ::DDS::Subscriber_var retval;
      ACE_NEW_THROW_EX (retval,
                        DDS_Subscriber_i (ccm_dds_sub, this),
                        ::CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::create_subscriber_with_profile <%C> - "
                    "Successfully created a DDSSubscriber\n",
                    qos_profile));

      DDS_ReturnCode_t retcode = ccm_dds_sub->enable ();
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_subscriber_with_profile <%C> - "
                        "Error: Unable to enable the DDSSubscriber: <%C>\n",
                        qos_profile,
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          delete ccm_dds_sl;
          throw ::CORBA::INTERNAL ();
        }
      return retval._retn ();
    }


    ::DDS::Subscriber_ptr
    DDS_DomainParticipant_i::create_subscriber (
                                       const ::DDS::SubscriberQos & qos,
                                       ::DDS::SubscriberListener_ptr a_listener,
                                       ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_subscriber");

      ACE_UNUSED_ARG (qos);

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::create_subscriber - "
                    "Creating Subscriber\n"));

      DDS_SubscriberListener_i *ccm_dds_sl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_sl,
                            DDS_SubscriberListener_i (a_listener, this),
                            ::CORBA::NO_MEMORY ());
        }

      DDS_SubscriberQos ccm_dds_qos = DDS_SUBSCRIBER_QOS_DEFAULT;
      DDSSubscriber * ccm_dds_sub =
      this->rti_entity ()->create_subscriber (ccm_dds_qos,
                                        ccm_dds_sl,
                                        mask);

      if (!ccm_dds_sub)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::"
                        "create_subscriber - "
                        "Error: RTI DDS returned a nil subscriber.\n"));
          delete ccm_dds_sl;
          return ::DDS::Subscriber::_nil ();
        }

      ::DDS::Subscriber_var retval;
      ACE_NEW_THROW_EX (retval,
                        DDS_Subscriber_i (ccm_dds_sub, this),
                        ::CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::create_subscriber - "
                    "Successfully created a DDSSubscriber\n"));

      DDS_ReturnCode_t retcode = ccm_dds_sub->enable ();
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_subscriber - "
                        "Error: Unable to enable the DDSSubscriber: <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          delete ccm_dds_sl;
          throw ::CORBA::INTERNAL ();
        }
      return retval._retn ();
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::delete_subscriber (
      ::DDS::Subscriber_ptr s)
    {
      DDS_Subscriber_i *ccm_dds_sub = dynamic_cast < DDS_Subscriber_i * > (s);

      if (!ccm_dds_sub)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::delete_subscriber - Unable to cast "
                        "provided object reference to servant pointer.\n"));
          return ::DDS::RETCODE_ERROR;
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_SUCCESSFUL, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::delete_subscriber - Successfully casted "
                    "provided object reference to Subscriber_type\n"));

      DDS_ReturnCode_t const retval = this->rti_entity ()->delete_subscriber (ccm_dds_sub->get_rti_entity ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::delete_subscriber - Error: RTI "
                        "delete_subscriber returned non-ok error code %C\n",
                        ::CIAO::DDS4CCM::translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::delete_subscriber - "
                        "Provided subscriber successfully deleted\n"));
        }

      return retval;
    }


    ::DDS::Subscriber_ptr
    DDS_DomainParticipant_i::get_builtin_subscriber (void)
    {
      ::DDS::Subscriber_var retval;
      DDSSubscriber* sub = this->rti_entity ()->get_builtin_subscriber ();
      if (sub)
        {
          ACE_NEW_THROW_EX (retval,
                            DDS_Subscriber_i (sub, this),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }


    ::DDS::Topic_ptr
    DDS_DomainParticipant_i::create_topic (
                                            const char * impl_name,
                                            const char * type_name,
                                            const ::DDS::TopicQos & qos,
                                            ::DDS::TopicListener_ptr a_listener,
                                            ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_topic");

      ACE_UNUSED_ARG (qos);

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::create_topic - "
                    "Attempting to create topic with name %C and type %C\n",
                    impl_name, type_name));

      DDS_TopicListener_i *ccm_dds_tl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          // Topic will be set later (using set_dds_topic)
          ACE_NEW_THROW_EX (ccm_dds_tl,
                            DDS_TopicListener_i (::DDS::Topic::_nil (), a_listener),
                            ::CORBA::NO_MEMORY ());
        }

      DDS_TopicQos ccm_dds_qos = DDS_TOPIC_QOS_DEFAULT;

      DDSTopicDescription * dds_td =
        this->rti_entity ()->lookup_topicdescription (impl_name);
      DDSTopic * dds_tp = 0;
      if (dds_td)
        {
          dds_tp = DDSTopic::narrow (dds_td);
        }

      if (!dds_tp)
        {
          dds_tp = this->rti_entity ()->create_topic (impl_name,
                                                type_name,
                                                ccm_dds_qos,
                                                ccm_dds_tl,
                                                mask);

          if (!dds_tp)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                            "DDS_DomainParticipant_i::create_topic - "
                            "Error: RTI DDS returned a nil topic\n"));
              delete ccm_dds_tl;
              return ::DDS::Topic::_nil ();
            }

          ::DDS::Topic_var retval;
          ACE_NEW_THROW_EX (retval,
                            DDS_Topic_i (dds_tp, this),
                            ::CORBA::NO_MEMORY ());

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_topic - "
                        "Successfully created topic with name %C and type %C\n",
                        impl_name, type_name));

          DPMANAGER->add_topic (this->rti_entity (), dds_tp);

          if (ccm_dds_tl)
            {
              ccm_dds_tl->set_dds_topic (retval.in ());
            }

          return retval._retn ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_topic - "
                        "Re-using topic with name %C and type %C.\n",
                        impl_name, type_name));

          DPMANAGER->_inc_ref (this->rti_entity (), dds_tp);

          ::DDS::Topic_var retval;
          ACE_NEW_THROW_EX (retval,
                            DDS_Topic_i (dds_tp, this),
                            ::CORBA::NO_MEMORY ());

          return retval._retn ();
        }
    }


    ::DDS::Topic_ptr
    DDS_DomainParticipant_i::create_topic_with_profile (
        const char *impl_name,
        const char *type_name,
        const char *qos_profile,
        ::DDS::TopicListener_ptr a_listener,
        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_topic_with_profile");

      if (impl_name == 0)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_topic_with_profile <%C> - "
                        "Error: provided nil topic name\n",
                        qos_profile));
          return ::DDS::Topic::_nil ();
        }

      if (type_name == 0)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_topic_with_profile <%C> - "
                        "Error: provided nil type name\n",
                        qos_profile));
          return ::DDS::Topic::_nil ();
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::create_topic_with_profile <%C> - "
                    "Attempting to create topic with name %C and type %C\n",
                    qos_profile,
                    impl_name, type_name));

      DDS_TopicListener_i *ccm_dds_tl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          // Topic will be set later (using set_dds_topic)
          ACE_NEW_THROW_EX (ccm_dds_tl,
                            DDS_TopicListener_i (::DDS::Topic::_nil (), a_listener),
                            ::CORBA::NO_MEMORY ());
        }

      DDSTopicDescription * dds_td =
        this->rti_entity ()->lookup_topicdescription (impl_name);
      DDSTopic * dds_tp = 0;
      if (dds_td)
        {
          dds_tp = DDSTopic::narrow (dds_td);
        }

      if (!dds_tp)
        {
          char * lib_name = get_library_name(qos_profile);
          char * prof_name = get_profile_name(qos_profile);

          if (lib_name != 0 && prof_name != 0)
            {
              dds_tp = this->rti_entity ()->create_topic_with_profile (
                                                            impl_name,
                                                            type_name,
                                                            lib_name,
                                                            prof_name,
                                                            ccm_dds_tl,
                                                            mask);
            }
          ACE_OS::free (lib_name);
          ACE_OS::free (prof_name);

          if (!dds_tp)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                            "DDS_DomainParticipant_i::create_topic_with_profile <%C> - "
                            "Error: RTI DDS returned a nil topic\n",
                            qos_profile));
              delete ccm_dds_tl;
              return ::DDS::Topic::_nil ();
            }

          ::DDS::Topic_var retval;
          ACE_NEW_THROW_EX (retval,
                            DDS_Topic_i (dds_tp, this),
                            ::CORBA::NO_MEMORY ());

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_topic_with_profile <%C> - "
                        "Successfully created topic with name %C and type %C\n",
                        qos_profile,
                        impl_name, type_name));

          if (ccm_dds_tl)
            {
              ccm_dds_tl->set_dds_topic (retval.in ());
            }
          DPMANAGER->add_topic (this->rti_entity (), dds_tp);

          return retval._retn ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_topic_with_profile <%C> - "
                        "Re-using topic with name %C and type %C.\n",
                        qos_profile,
                        impl_name, type_name));
          DPMANAGER->_inc_ref (this->rti_entity (), dds_tp);

          ::DDS::Topic_var retval;
          ACE_NEW_THROW_EX (retval,
                            DDS_Topic_i (dds_tp, this),
                            ::CORBA::NO_MEMORY ());
          if (ccm_dds_tl)
            {
              ccm_dds_tl->set_dds_topic (retval.in ());
            }
          return retval._retn ();
        }
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::delete_topic (::DDS::Topic_ptr a_topic)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::delete_topic");
      CORBA::String_var topic_name = a_topic->get_name ();
      DDSTopicDescription * dds_td =
        this->rti_entity ()->lookup_topicdescription (topic_name.in ());
      DDSTopic * tp = 0;
      if (dds_td)
        {
          tp = DDSTopic::narrow (dds_td);
        }

      if (!tp)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::delete_topic <%C> - Unable to cast "
                        "provided object reference to servant.\n",
                        topic_name.in ()));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_SUCCESSFUL, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::delete_topic <%C> - "
                    "Successfully casted provided object reference to servant.\n",
                    topic_name.in ()));

      ::DDS::ReturnCode_t retval = DDS::RETCODE_OK;
      if (DPMANAGER->remove_topic (this->rti_entity (), tp))
        {
          retval = this->rti_entity ()->delete_topic (tp);

          if (retval != DDS_RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_DomainParticipant_i::delete_topic <%C> - "
                            "Error: RTI delete_topic returned non-ok error code %C\n",
                            topic_name.in (),
                            ::CIAO::DDS4CCM::translate_retcode (retval)));
            }
          else
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                            "DDS_DomainParticipant_i::delete_topic <%C> - "
                            "Provided topic successfully deleted\n",
                            topic_name.in ()));
            }
        }
      return retval;
    }


    ::DDS::Topic_ptr
    DDS_DomainParticipant_i::find_topic (const char * impl_name,
                                         const ::DDS::Duration_t & timeout)
    {
      ::DDS::Topic_var retval;
      ::DDS_Duration_t ddstimeout;
      ddstimeout <<= timeout;
      ::DDSTopic* topic = this->rti_entity ()->find_topic (impl_name, ddstimeout);
      if (topic)
        {
          ACE_NEW_RETURN (retval,
                          DDS_Topic_i (topic, this),
                          ::DDS::Topic::_nil ());
        }
      return retval._retn ();
    }


    ::DDS::TopicDescription_ptr
    DDS_DomainParticipant_i::lookup_topicdescription (const char * name)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::lookup_topicdescription");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_UNIMP_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::lookup_topicdescription - "
                    "Looking up topic: name <%C>\n",
                    name));

      ::DDS::TopicDescription_var retval;
      ::DDSTopicDescription* topic = this->rti_entity ()->lookup_topicdescription (name);
      if (topic)
        {
          // Check the entity: is it a Topic or a ContentFilteredTopic
          ::DDSTopic * tp = dynamic_cast < ::DDSTopic *> (topic);
          if (tp)
            {
              ACE_NEW_THROW_EX (retval,
                                DDS_Topic_i (tp, this),
                                ::CORBA::NO_MEMORY ());
            }
          else
            {
              ::DDSContentFilteredTopic * cftp =
                dynamic_cast < ::DDSContentFilteredTopic *>(topic);
              if (cftp)
                {
                  ACE_NEW_THROW_EX (retval,
                                    DDS_ContentFilteredTopic_i (cftp, this),
                                    ::CORBA::NO_MEMORY ());
                }
            }
        }
      return retval._retn ();
    }


    ::DDS::ContentFilteredTopic_ptr
    DDS_DomainParticipant_i::create_contentfilteredtopic (const char * name,
                                                          ::DDS::Topic_ptr related_topic,
                                                          const char * filter_expression,
                                                          const ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_contentfilteredtopic");

      DDS_Topic_i *top = dynamic_cast< DDS_Topic_i *> (related_topic);
      if (!top)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_contentfilteredtopic - "
                        "Unable to cast provided topic.\n"));
          return ::DDS::ContentFilteredTopic::_nil ();
        }

      const char** parameterlist = 0;
      ACE_NEW_THROW_EX (parameterlist,
                        const char*[expression_parameters.length ()],
                        ::CORBA::NO_MEMORY ());

      for (CORBA::ULong i = 0; i < expression_parameters.length (); ++i)
        {
          parameterlist[i] = expression_parameters[i].in ();
        }
      DDS_StringSeq parameters (expression_parameters.length ());
      parameters.from_array (parameterlist, expression_parameters.length ());

      DDSContentFilteredTopic * ccm_dds_cft = this->rti_entity ()->create_contentfilteredtopic (
                                                                  name,
                                                                  top->get_rti_entity (),
                                                                  filter_expression,
                                                                  parameters);
      delete [] parameterlist;
      if (!ccm_dds_cft)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::create_contentfilteredtopic - "
                        "RTI DDS returned a nil ContentFilteredTopic for "
                        "name <%C> and filter expression <%C>.\n",
                       name, filter_expression));
          return ::DDS::ContentFilteredTopic::_nil ();
        }

      ::DDS::ContentFilteredTopic_var retval;
      ACE_NEW_THROW_EX (retval,
                        DDS_ContentFilteredTopic_i (ccm_dds_cft, this),
                        ::CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_DomainParticipant_i::create_contentfilteredtopic - "
                    "Successfully created contentfilteredtopic with name <%C> and "
                    "filter expression <%C>\n",
                    name, filter_expression));

      return retval._retn ();
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::delete_contentfilteredtopic (
      ::DDS::ContentFilteredTopic_ptr a_contentfilteredtopic)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::delete_contentfilteredtopic");
      DDS_ContentFilteredTopic_i *ccm_dds_cft =
          dynamic_cast < DDS_ContentFilteredTopic_i *> (a_contentfilteredtopic);
      if (!ccm_dds_cft)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::delete_contentfilteredtopic - "
                        "Unable to cast provided ContentFilteredTopic.\n"));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }
      return this->rti_entity ()->delete_contentfilteredtopic (ccm_dds_cft->get_rti_entity ());
    }


    ::DDS::MultiTopic_ptr
    DDS_DomainParticipant_i::create_multitopic (
      const char * name,
      const char * type_name,
      const char * subscription_expression,
      const ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_multitopic");
      ACE_UNUSED_ARG (name);
      ACE_UNUSED_ARG (type_name);
      ACE_UNUSED_ARG (subscription_expression);
      ACE_UNUSED_ARG (expression_parameters);
      //Not implemented in version ndds.4.5b.rev01 of RTI DDS.
      throw ::CORBA::NO_IMPLEMENT ();
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::delete_multitopic (
      ::DDS::MultiTopic_ptr a_multitopic)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::delete_multitopic");
      ACE_UNUSED_ARG (a_multitopic);
      //Not implemented in version ndds.4.5b.rev01 of RTI DDS.
      throw ::CORBA::NO_IMPLEMENT ();
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::delete_contained_entities (void)
    {
      return this->rti_entity ()->delete_contained_entities ();

    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::set_qos (
      const ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_qos");
      ::DDS_DomainParticipantQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->rti_entity ()->set_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::get_qos (
      ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_qos");
      ::DDS_DomainParticipantQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode = this->rti_entity ()-> get_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::set_listener (
                ::DDS::DomainParticipantListener_ptr a_listener,
                ::DDS::StatusMask mask)
    {
      // Delete the previously set listener
      DDSDomainParticipantListener *listener = this->rti_entity ()->get_listener ();
      delete listener;

      ::DDSDomainParticipantListener * ccm_dds_impl_list = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            DDS_DomainParticipantListener_i (a_listener),
                            ::CORBA::NO_MEMORY ());
        }
      return this->rti_entity ()->set_listener (ccm_dds_impl_list, mask);
    }


    ::DDS::DomainParticipantListener_ptr
    DDS_DomainParticipant_i::get_listener (void)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_listener");

      DDSDomainParticipantListener *ccm_dds_dp_list = this->rti_entity ()->get_listener ();
      DDS_DomainParticipantListener_i * list_proxy =
        dynamic_cast <DDS_DomainParticipantListener_i *> (ccm_dds_dp_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_DomainParticipant_i::get_listener - "
                        "DDS returned a NIL listener.\n"));
          return ::DDS::DomainParticipantListener::_nil ();
        }
      return list_proxy->get_domainparticipantlistener ();
    }

    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::ignore_participant (
      DDS_INSTANCE_HANDLE_T_IN handle)
    {
      ::DDS_InstanceHandle_t ccm_dds_handle;
      ccm_dds_handle <<= handle;
      return this->rti_entity ()->ignore_participant (ccm_dds_handle);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::ignore_topic (
      DDS_INSTANCE_HANDLE_T_IN handle)
    {
      ::DDS_InstanceHandle_t ccm_dds_handle;
      ccm_dds_handle <<= handle;
      return this->rti_entity ()->ignore_topic (ccm_dds_handle);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::ignore_publication (
      DDS_INSTANCE_HANDLE_T_IN handle)
    {
      ::DDS_InstanceHandle_t ccm_dds_handle;
      ccm_dds_handle <<= handle;
      return this->rti_entity ()->ignore_publication (ccm_dds_handle);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::ignore_subscription (
      DDS_INSTANCE_HANDLE_T_IN handle)
    {
      ::DDS_InstanceHandle_t ccm_dds_handle;
      ccm_dds_handle <<= handle;
      return this->rti_entity ()->ignore_subscription (ccm_dds_handle);
    }


    ::DDS::DomainId_t
    DDS_DomainParticipant_i::get_domain_id (void)
    {
      return this->rti_entity ()->get_domain_id ();
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::assert_liveliness (void)
    {
      return this->rti_entity ()->assert_liveliness ();
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::set_default_publisher_qos (
      const ::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_default_publisher_qos");
      ::DDS_PublisherQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->rti_entity ()->set_default_publisher_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::get_default_publisher_qos (
      ::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_default_publisher_qos");
      ::DDS_PublisherQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode = this->rti_entity ()-> get_default_publisher_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::set_default_subscriber_qos (
      const ::DDS::SubscriberQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_default_subscriber_qos");
      ::DDS_SubscriberQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->rti_entity ()->set_default_subscriber_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::get_default_subscriber_qos (
      ::DDS::SubscriberQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_default_subscriber_qos");
      ::DDS_SubscriberQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
            this->rti_entity ()-> get_default_subscriber_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::set_default_topic_qos (
      const ::DDS::TopicQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_default_topic_qos");
      ::DDS_TopicQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->rti_entity ()->set_default_topic_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::get_default_topic_qos (
      ::DDS::TopicQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_default_topic_qos");
      ::DDS_TopicQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
            this->rti_entity ()-> get_default_topic_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::get_discovered_participants (
      ::DDS::InstanceHandleSeq & impl_handles)
    {
      ::DDS_InstanceHandleSeq rtiseq;
      ::DDS::ReturnCode_t const retval = this->rti_entity ()->get_discovered_participants (rtiseq);
      impl_handles <<= rtiseq;
      return retval;
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::get_discovered_participant_data (
      ::DDS::ParticipantBuiltinTopicData & impl_data,
      DDS_INSTANCE_HANDLE_T_IN impl_handle)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_discovered_participant_data");
      DDS_ParticipantBuiltinTopicData dds_part_data;
      dds_part_data <<= impl_data;
      ::DDS_InstanceHandle_t dds_hnd;
      dds_hnd <<= impl_handle;
      ::DDS_ReturnCode_t retcode =
        this->rti_entity ()->get_discovered_participant_data (dds_part_data,
                                                        dds_hnd);
      impl_data <<= dds_part_data;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::get_discovered_topics (
      ::DDS::InstanceHandleSeq & impl_handles)
    {
      ::DDS_InstanceHandleSeq rtiseq;
      ::DDS::ReturnCode_t const retval = this->rti_entity ()->get_discovered_topics (rtiseq);
      impl_handles <<= rtiseq;
      return retval;
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::get_discovered_topic_data (
      ::DDS::TopicBuiltinTopicData & impl_data,
      DDS_INSTANCE_HANDLE_T_IN impl_handle)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_discovered_topic_data");
      DDS_TopicBuiltinTopicData dds_tp_data;
      dds_tp_data <<= impl_data;
      ::DDS_InstanceHandle_t dds_hnd;
      dds_hnd <<= impl_handle;
      ::DDS_ReturnCode_t retcode =
        this->rti_entity ()->get_discovered_topic_data (dds_tp_data,
                                                  dds_hnd);
      impl_data <<= dds_tp_data;
      return retcode;
    }


    ::CORBA::Boolean
    DDS_DomainParticipant_i::contains_entity (
      DDS_INSTANCE_HANDLE_T_IN a_handle)
    {
      ::DDS_InstanceHandle_t ccm_dds_handle;
      ccm_dds_handle <<= a_handle;
      return this->rti_entity ()->contains_entity (ccm_dds_handle);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::get_current_time (
      ::DDS::Time_t & current_time)
    {
      DDS_Time_t ccm_dds_time;
      ::DDS::ReturnCode_t const retval = this->rti_entity ()->get_current_time (ccm_dds_time);
      current_time <<= ccm_dds_time;
      return retval;
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipant_i::enable (void)
    {
      return this->rti_entity ()->enable ();
    }


    ::DDS::StatusCondition_ptr
    DDS_DomainParticipant_i::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval;
      DDSStatusCondition* sc = this->rti_entity ()->get_statuscondition ();
      if (sc)
        {
          ACE_NEW_THROW_EX (retval,
                            DDS_StatusCondition_i (sc, this),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }


    ::DDS::StatusMask
    DDS_DomainParticipant_i::get_status_changes (void)
    {
      return this->rti_entity ()->get_status_changes ();
    }


    DDS_INSTANCE_HANDLE_T_RETN
    DDS_DomainParticipant_i::get_instance_handle (void)
    {
      ::DDS_InstanceHandle_t const rtihandle = this->rti_entity ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }


    DDSDomainParticipant *
    DDS_DomainParticipant_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }


    void
    DDS_DomainParticipant_i::set_rti_entity (
      DDSDomainParticipant * dp)
    {
      this->rti_entity_ = dp;
    }


    DDSDomainParticipant *
    DDS_DomainParticipant_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DDS_DomainParticipant_i::rti_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
    }
  }
}
