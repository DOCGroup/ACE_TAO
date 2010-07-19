// $Id$

#include "Subscriber_T.h"
#include "Publisher_T.h"
#include "ContentFilteredTopic_T.h"
#include "TopicDescription_T.h"
#include "Utils.h"
#include "StatusCondition.h"
#include "DDSPublisherListener_T.h"
#include "DDSSubscriberListener_T.h"
#include "Topic_T.h"
#include "TopicListener_T.h"
#include "DomainParticipantListener.h"
#include "DomainParticipantManager.h"

#include "ndds/PublisherQos.h"
#include "ndds/SubscriberQos.h"
#include "ndds/TopicQos.h"
#include "ndds/DomainParticipantQos.h"
#include "ndds/InstanceHandle_t.h"
#include "ndds/Duration_t.h"
#include "ndds/Time_t.h"
#include "ndds/InstanceHandleSeq.h"
#include "ndds/TopicBuiltinTopicData.h"
#include "ndds/ParticipantBuiltinTopicData.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/Log_Macros.h"

#include "ace/CORBA_macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::CCM_DDS_DomainParticipant_T (
      DDSDomainParticipant * dp) : impl_ (dp)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::CCM_DDS_DomainParticipant_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~CCM_DDS_DomainParticipant_T (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~CCM_DDS_DomainParticipant_T");
    }

#if (CIAO_DDS4CCM_NDDS==1)
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Publisher_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_publisher_with_profile (
      const char* library_name,
      const char *profile_name,
      ::DDS::PublisherListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_publisher_with_profile");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, CLINFO
                    "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                    "VENDOR_TYPE>::create_publisher_with_profile - "
                    "Start creating Publisher\n"));
      CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> *ccm_dds_pl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_pl,
                            PublisherListener_type (a_listener),
                            ::CORBA::NO_MEMORY ());
        }
      DDSPublisher * ccm_dds_pub =
        this->impl ()->create_publisher_with_profile (library_name,
                                                      profile_name,
                                                      ccm_dds_pl,
                                                      mask);

      if (!ccm_dds_pub)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::create_publisher_with_profile - "
                        "Error: Unable to create Publisher\n"));
          delete ccm_dds_pl;
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::Publisher_var retval = ::DDS::Publisher::_nil ();
      ACE_NEW_THROW_EX (retval,
                        Publisher_type (ccm_dds_pub),
                        ::CORBA::NO_MEMORY ());


      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                    "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                    "VENDOR_TYPE>::create_publisher_with_profile - "
                    "Successfully created a DDSPublisher\n"));

      ccm_dds_pub->enable ();
      return retval._retn ();
    }
#endif

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Publisher_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_publisher (
      const ::DDS::PublisherQos & qos,
      ::DDS::PublisherListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_publisher");
      ACE_UNUSED_ARG (qos);
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, CLINFO
                    "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                    "VENDOR_TYPE>::create_publisher - "
                    "Start creating Publisher\n"));

      DDS_PublisherQos ccm_dds_qos = DDS_PUBLISHER_QOS_DEFAULT;

      CCM_DDS_PublisherListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> *ccm_dds_pl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_pl,
                            PublisherListener_type (a_listener),
                            ::CORBA::NO_MEMORY ());
        }

      DDSPublisher * ccm_dds_pub =
        this->impl ()->create_publisher (ccm_dds_qos,
                                         ccm_dds_pl,
                                         mask);

      if (!ccm_dds_pub)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                       "CCM_DDS_DomainParticipant_T"
                       "<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_publisher - "
                       "Error: Unable to create Publisher\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::Publisher_var retval = ::DDS::Publisher::_nil ();
      ACE_NEW_THROW_EX (retval,
                        Publisher_type (ccm_dds_pub),
                        ::CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                    "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                    "VENDOR_TYPE>::create_publisher - "
                    "Successfully created a DDSPublisher\n"));

      ccm_dds_pub->enable ();
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_publisher (
      ::DDS::Publisher_ptr p)
    {
      Publisher_type *ccm_dds_pub =
        dynamic_cast < Publisher_type * > (p);

      if (!ccm_dds_pub)
        {
          DDS4CCM_ERROR (3, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T"
                        "<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_publisher - "
                        "Unable to cast provided object reference to servant pointer.\n"));
          return ::DDS::RETCODE_ERROR;
        }

      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_SUCCESFUL, (LM_TRACE, CLINFO
                   "CCM_DDS_DomainParticipant_T"
                   "<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_publisher - "
                   "Successfully casted provided object reference to "
                   "CCM_DDS_Publisher_T\n"));

      DDS_ReturnCode_t const retval = this->impl ()->delete_publisher (ccm_dds_pub->get_impl ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T"
                        "<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_publisher - "
                        "Error: RTI delete_publisher returned non-ok error code %C\n",
                        translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::delete_publisher - "
                        "Provided publisher successfully deleted\n"));
        }

      return retval;
    }

#if (CIAO_DDS4CCM_NDDS==1)
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Subscriber_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_subscriber_with_profile (
      const char* library_name,
      const char *profile_name,
      ::DDS::SubscriberListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_subscriber_with_profile");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, CLINFO
                   "CCM_DDS_DomainParticipant_T"
                   "<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_subscriber_with_profile - "
                   "Creating Subscriber\n"));

      SubscriberListener_type *ccm_dds_sl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_sl,
                            SubscriberListener_type (a_listener),
                            ::CORBA::NO_MEMORY ());
        }
      DDSSubscriber * ccm_dds_sub =
        this->impl ()->create_subscriber_with_profile (
          library_name,
          profile_name,
          ccm_dds_sl,
          mask);

      if (!ccm_dds_sub)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::create_subscriber_with_profile - "
                        "Error: Unable to create Subscriber\n"));
          delete ccm_dds_sl;
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::Subscriber_var retval = ::DDS::Subscriber::_nil ();
      ACE_NEW_THROW_EX (retval,
                        Subscriber_type (ccm_dds_sub),
                        ::CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                    "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                    "VENDOR_TYPE>::create_subscriber_with_profile - "
                    "Successfully created a DDSSubscriber\n"));

      ccm_dds_sub->enable ();
      return retval._retn ();
    }
#endif

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Subscriber_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_subscriber (
                                       const ::DDS::SubscriberQos & qos,
                                       ::DDS::SubscriberListener_ptr a_listener,
                                       ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_subscriber");

      ACE_UNUSED_ARG (qos);

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, CLINFO
                    "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                    "VENDOR_TYPE>::create_subscriber - "
                    "Creating Subscriber\n"));

      SubscriberListener_type *ccm_dds_sl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_sl,
                            SubscriberListener_type (a_listener),
                            ::CORBA::NO_MEMORY ());
        }

      DDS_SubscriberQos ccm_dds_qos = DDS_SUBSCRIBER_QOS_DEFAULT;
      DDSSubscriber * ccm_dds_sub =
      this->impl ()->create_subscriber (ccm_dds_qos,
                                        ccm_dds_sl,
                                        mask);

      if (!ccm_dds_sub)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::create_subscriber - "
                        "Error: Unable to create Subscriber\n"));
          delete ccm_dds_sl;
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::Subscriber_var retval = ::DDS::Subscriber::_nil ();
      ACE_NEW_THROW_EX (retval,
                        Subscriber_type (ccm_dds_sub),
                        ::CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                    "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                    "VENDOR_TYPE>::create_subscriber - "
                    "Successfully created a DDSSubscriber\n"));

      ccm_dds_sub->enable ();
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_subscriber (
      ::DDS::Subscriber_ptr s)
    {
      Subscriber_type *ccm_dds_sub = dynamic_cast < Subscriber_type * > (s);

      if (!ccm_dds_sub)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::delete_subscriber - Unable to cast "
                        "provided object reference to servant pointer.\n"));
          return ::DDS::RETCODE_ERROR;
        }

      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_SUCCESFUL, (LM_TRACE, CLINFO
                    "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                    "VENDOR_TYPE>::delete_subscriber - Successfully casted "
                    "provided object reference to Subscriber_type\n"));

      DDS_ReturnCode_t const retval = this->impl ()->delete_subscriber (ccm_dds_sub->get_impl ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::delete_subscriber - Error: RTI "
                        "delete_subscriber returned non-ok error code %C\n",
                        translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::delete_subscriber - "
                        "Provided subscriber successfully deleted\n"));
        }

      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Subscriber_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_builtin_subscriber (void)
    {
      ::DDS::Subscriber_var retval = ::DDS::Subscriber::_nil ();
      DDSSubscriber* sub = this->impl ()->get_builtin_subscriber ();
      if (sub)
        {
          ACE_NEW_THROW_EX (retval,
                            Subscriber_type (sub),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Topic_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_topic (
                                            const char * impl_name,
                                            const char * type_name,
                                            const ::DDS::TopicQos & qos,
                                            ::DDS::TopicListener_ptr a_listener,
                                            ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_topic");

      ACE_UNUSED_ARG (qos);

      if (impl_name == 0)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "DDS_DomainParticipant_i::create_topic - "
                        "Error: provided nil topic name\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER,
                                        0);
        }

      if (type_name == 0)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "DDS_DomainParticipant_i::create_topic - "
                        "Error: provided nil type name\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER,
                                        0);
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_DEBUG, CLINFO
                    "DDS_DomainParticipant_i::create_topic - "
                    "Attempting to create topic with name %C and type %C\n",
                    impl_name, type_name));

      TopicListener_type *ccm_dds_tl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_tl,
                            TopicListener_type (a_listener),
                            ::CORBA::NO_MEMORY ());
        }

      DDS_TopicQos ccm_dds_qos = DDS_TOPIC_QOS_DEFAULT;

      DDSTopicDescription * dds_td = 
        this->impl ()->lookup_topicdescription (impl_name);
      DDSTopic * dds_tp = 0;
      if (dds_td)
        {
          dds_tp = DDSTopic::narrow (dds_td);
        }

      if (!dds_tp)
        {
          dds_tp = this->impl ()->create_topic (impl_name,
                                                type_name,
                                                ccm_dds_qos,
                                                ccm_dds_tl,
                                                mask);

          if (!dds_tp)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, CLINFO
                            "DDS_DomainParticipant_i::create_topic - "
                            "Error: RTI DDS returned a nil topic\n"));
              delete ccm_dds_tl;
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
            }

          ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
          ACE_NEW_THROW_EX (retval,
                            Topic_type (dds_tp),
                            ::CORBA::NO_MEMORY ());

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                        "DDS_DomainParticipant_i::create_topic - "
                        "Successfully created topic with name %C and type %C\n",
                        impl_name, type_name));

          Topic_type * ccm_dds_tp =
            dynamic_cast < Topic_type *> (retval.in ());
          ccm_dds_tp->set_impl (dds_tp);

          DPMANAGER->add_topic (this->impl (), dds_tp);

          return retval._retn ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, CLINFO
                        "DDS_DomainParticipant_i::create_topic_with_profile - "
                        "Re-using topic with name %C and type %C.\n",
                        impl_name, type_name));

          DPMANAGER->_inc_ref (this->impl (), dds_tp);

          ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
          ACE_NEW_THROW_EX (retval,
                            Topic_type (dds_tp),
                            ::CORBA::NO_MEMORY ());

          return retval._retn ();
        }
    }

#if (CIAO_DDS4CCM_NDDS==1)
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Topic_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_topic_with_profile (
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
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "DDS_DomainParticipant_i::create_topic_with_profile - "
                        "Error: provided nil topic name\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
        }

      if (type_name == 0)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "DDS_DomainParticipant_i::create_topic_with_profile - "
                        "Error: provided nil type name\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_DEBUG, CLINFO
                    "DDS_DomainParticipant_i::create_topic_with_profile - "
                    "Attempting to create topic with name %C and type %C\n",
                    impl_name, type_name));

      TopicListener_type *ccm_dds_tl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_tl,
                            TopicListener_type (a_listener),
                            ::CORBA::NO_MEMORY ());
        }

      DDSTopicDescription * dds_td = 
        this->impl ()->lookup_topicdescription (impl_name);
      DDSTopic * dds_tp = 0;
      if (dds_td)
        {
          dds_tp = DDSTopic::narrow (dds_td);
        }

      if (!dds_tp)
        {
          dds_tp = this->impl ()->create_topic_with_profile (
                                                        impl_name,
                                                        type_name,
                                                        library_name,
                                                        profile_name,
                                                        ccm_dds_tl,
                                                        mask);

          if (!dds_tp)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, CLINFO
                            "DDS_DomainParticipant_i::create_topic_with_profile - "
                            "Error: RTI DDS returned a nil topic\n"));
              delete ccm_dds_tl;
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
            }

          ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
          ACE_NEW_THROW_EX (retval,
                            Topic_type (dds_tp),
                            ::CORBA::NO_MEMORY ());

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                        "DDS_DomainParticipant_i::create_topic_with_profile - "
                        "Successfully created topic with name %C and type %C\n",
                        impl_name, type_name));

          Topic_type * ccm_dds_tp =
            dynamic_cast < Topic_type *> (retval.in ());
          ccm_dds_tp->set_impl (dds_tp);

          DPMANAGER->add_topic (this->impl (), dds_tp);

          return retval._retn ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, CLINFO
                        "DDS_DomainParticipant_i::create_topic_with_profile - "
                        "Re-using topic with name %C and type %C.\n",
                        impl_name, type_name));
          DPMANAGER->_inc_ref (this->impl (), dds_tp);

          ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
          ACE_NEW_THROW_EX (retval,
                            Topic_type (dds_tp),
                            ::CORBA::NO_MEMORY ());
          return retval._retn ();
        }
    }
#endif

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_topic (::DDS::Topic_ptr a_topic)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::delete_topic");
      const char * topic_name = a_topic->get_name ();
      DDSTopicDescription * dds_td =
        this->impl ()->lookup_topicdescription (topic_name);
      DDSTopic * tp = 0;
      if (dds_td)
        {
          tp = DDSTopic::narrow (dds_td);
        }

      if (!tp)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::delete_topic <%C> - Unable to cast "
                        "provided object reference to servant.\n",
                        topic_name));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }

      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_SUCCESFUL, (LM_TRACE, CLINFO
                    "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                    "VENDOR_TYPE>::delete_topic <%C> - "
                    "Successfully casted provided object reference to servant.\n",
                    topic_name));

      ::DDS::ReturnCode_t retval = DDS::RETCODE_OK;
      if (DPMANAGER->remove_topic (this->impl (), tp))
        {
          retval = this->impl ()->delete_topic (tp);

          if (retval != DDS_RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                            "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                            "VENDOR_TYPE>::delete_topic <%C> - "
                            "Error: RTI delete_topic returned non-ok error code %C\n",
                            topic_name,
                            translate_retcode (retval)));
            }
          else
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                            "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                            "VENDOR_TYPE>::delete_topic <%C> - "
                            "Provided topic successfully deleted\n",
                            topic_name));
            }
        }
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Topic_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::find_topic (const char * impl_name,
                                         const ::DDS::Duration_t & timeout)
    {
      ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
      ::DDS_Duration_t ddstimeout;
      ddstimeout <<= timeout;
      ::DDSTopic* topic = this->impl ()->find_topic (impl_name, ddstimeout);
      if (topic)
        {
          ACE_NEW_RETURN (retval,
                          Topic_type (topic),
                          ::DDS::Topic::_nil ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::TopicDescription_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::lookup_topicdescription (const char * name)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::lookup_topicdescription");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_UNIMP_ACTION, (LM_DEBUG, CLINFO
                    "Looking up topic: name <%C>\n",
                    name));

      ::DDS::TopicDescription_var retval = ::DDS::TopicDescription::_nil ();
      ::DDSTopicDescription* topic = this->impl ()->lookup_topicdescription (name);
      if (topic)
        {
          // Check the entity: is it a Topic or a ContentFilteredTopic
          ::DDSTopic * tp = dynamic_cast < ::DDSTopic *> (topic);
          if (tp)
            {
              ACE_NEW_THROW_EX (retval,
                                Topic_type (tp),
                                ::CORBA::NO_MEMORY ());
            }
          else
            {
              ::DDSContentFilteredTopic * cftp =
                dynamic_cast < ::DDSContentFilteredTopic *>(topic);
              if (cftp)
                {
                  ACE_NEW_THROW_EX (retval,
                                    ContentFilteredTopic_type (cftp),
                                    ::CORBA::NO_MEMORY ());
                }
            }
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ContentFilteredTopic_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_contentfilteredtopic (const char * name,
                                                          ::DDS::Topic_ptr related_topic,
                                                          const char * filter_expression,
                                                          const ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_contentfilteredtopic");

#if (CIAO_DDS4CCM_NDDS==1)
      Topic_type *top = dynamic_cast< Topic_type *> (related_topic);
      if (!top)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::create_contentfilteredtopic - "
                        "Unable to cast provided topic.\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
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

      DDSContentFilteredTopic * ccm_dds_cft = this->impl ()->create_contentfilteredtopic (
                                                                  name,
                                                                  top->get_impl (),
                                                                  filter_expression,
                                                                  parameters);
      delete [] parameterlist;
      if (!ccm_dds_cft)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::create_contentfilteredtopic - "
                        "RTI DDS returned a nil ContentFilteredTopic for "
                        "name <%C> and filter expression <%C>.\n",
                       name, filter_expression));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::ContentFilteredTopic_var retval = ::DDS::ContentFilteredTopic::_nil ();
      ACE_NEW_THROW_EX (retval,
                        ContentFilteredTopic_type (ccm_dds_cft),
                        ::CORBA::NO_MEMORY ());

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                    "DDS_DomainParticipant_i::create_contentfilteredtopic - "
                    "Successfully created contentfilteredtopic with name <%C> and "
                    "filter expression <%C>\n",
                    name, filter_expression));

      return retval._retn ();
#else
      return this->impl ()->create_contentfilteredtopic (name,
                                                         related_topic,
                                                         filter_expression,
                                                         expression_parameters);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_contentfilteredtopic (
      ::DDS::ContentFilteredTopic_ptr a_contentfilteredtopic)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::delete_contentfilteredtopic");
      ContentFilteredTopic_type *ccm_dds_cft =
          dynamic_cast < ContentFilteredTopic_type *> (a_contentfilteredtopic);
      if (!ccm_dds_cft)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::delete_contentfilteredtopic - "
                        "Unable to cast provided ContentFilteredTopic.\n"));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }
      return this->impl ()->delete_contentfilteredtopic (ccm_dds_cft->get_impl ());
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::MultiTopic_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_multitopic (
      const char * name,
      const char * type_name,
      const char * subscription_expression,
      const ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::create_multitopic");
#if (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (name);
      ACE_UNUSED_ARG (type_name);
      ACE_UNUSED_ARG (subscription_expression);
      ACE_UNUSED_ARG (expression_parameters);
      //Not implemented in version ndds.4.5b.rev01 of RTI DDS.
      throw ::CORBA::NO_IMPLEMENT ();
#else
      return this->impl ()->create_multitopic (name,
                                               type_name,
                                               subscription_expression,
                                               expression_parameters);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_multitopic (
      ::DDS::MultiTopic_ptr a_multitopic)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::delete_multitopic");
#if (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (a_multitopic);
      //Not implemented in version ndds.4.5b.rev01 of RTI DDS.
      throw ::CORBA::NO_IMPLEMENT ();
#else
      return this->impl ()->delete_multitopic (a_multitopic);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_contained_entities (void)
    {
      return this->impl ()->delete_contained_entities ();

    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_qos (
      const ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DomainParticipantQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl()->set_qos (ccm_dds_qos);
#else
      return this->impl()->set_qos (qos);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_qos (
      ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_qos");
      ::DDS_DomainParticipantQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode = this->impl()-> get_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_listener (
                ::DDS::DomainParticipantListener_ptr a_listener,
                ::DDS::StatusMask mask)
    {
      CCM_DDS_DomainParticipantListener_i * ccm_dds_impl_list = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            CCM_DDS_DomainParticipantListener_i (a_listener),
                            ::CORBA::NO_MEMORY ());
        }
      return this->impl_->set_listener (ccm_dds_impl_list, mask);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::DomainParticipantListener_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_listener (void)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_listener");

      DDSDomainParticipantListener *ccm_dds_dp_list = this->impl ()->get_listener ();
      CCM_DDS_DomainParticipantListener_i * list_proxy =
        dynamic_cast <CCM_DDS_DomainParticipantListener_i *> (ccm_dds_dp_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, CLINFO
                        "CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::get_listener - "
                        "DDS returned a NIL listener.\n"));
          return ::DDS::DomainParticipantListener::_nil ();
        }
      return list_proxy->get_domainparticipantlistener ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ignore_participant (
      DDS_INSTANCE_HANDLE_T_IN handle)
    {
      ::DDS_InstanceHandle_t ccm_dds_handle;
      ccm_dds_handle <<= handle;
      return this->impl ()->ignore_participant (ccm_dds_handle);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ignore_topic (
      DDS_INSTANCE_HANDLE_T_IN handle)
    {
      ::DDS_InstanceHandle_t ccm_dds_handle;
      ccm_dds_handle <<= handle;
      return this->impl ()->ignore_topic (ccm_dds_handle);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ignore_publication (
      DDS_INSTANCE_HANDLE_T_IN handle)
    {
      ::DDS_InstanceHandle_t ccm_dds_handle;
      ccm_dds_handle <<= handle;
      return this->impl ()->ignore_publication (ccm_dds_handle);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ignore_subscription (
      DDS_INSTANCE_HANDLE_T_IN handle)
    {
      ::DDS_InstanceHandle_t ccm_dds_handle;
      ccm_dds_handle <<= handle;
      return this->impl ()->ignore_subscription (ccm_dds_handle);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::DomainId_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_domain_id (void)
    {
      return this->impl ()->get_domain_id ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::assert_liveliness (void)
    {
      return this->impl ()->assert_liveliness ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_default_publisher_qos (
      const ::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_default_publisher_qos");
      ::DDS_PublisherQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl()->set_default_publisher_qos (ccm_dds_qos);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_default_publisher_qos (
      ::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_default_publisher_qos");
      ::DDS_PublisherQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode = this->impl()-> get_default_publisher_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_default_subscriber_qos (
      const ::DDS::SubscriberQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_default_subscriber_qos");
      ::DDS_SubscriberQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl()->set_default_subscriber_qos (ccm_dds_qos);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_default_subscriber_qos (
      ::DDS::SubscriberQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_default_subscriber_qos");
      ::DDS_SubscriberQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
            this->impl()-> get_default_subscriber_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_default_topic_qos (
      const ::DDS::TopicQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::set_default_topic_qos");
      ::DDS_TopicQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl()->set_default_topic_qos (ccm_dds_qos);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_default_topic_qos (
      ::DDS::TopicQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_default_topic_qos");
      ::DDS_TopicQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
            this->impl()-> get_default_topic_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_discovered_participants (
      ::DDS::InstanceHandleSeq & impl_handles)
    {
      ::DDS_InstanceHandleSeq rtiseq;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_discovered_participants (rtiseq);
      impl_handles <<= rtiseq;
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_discovered_participant_data (
      ::DDS::ParticipantBuiltinTopicData & impl_data,
      DDS_INSTANCE_HANDLE_T_IN impl_handle)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_discovered_participant_data");
      DDS_ParticipantBuiltinTopicData dds_part_data;
      dds_part_data <<= impl_data;
      ::DDS_InstanceHandle_t dds_hnd;
      dds_hnd <<= impl_handle;
      ::DDS_ReturnCode_t retcode =
        this->impl ()->get_discovered_participant_data (dds_part_data,
                                                        dds_hnd);
      impl_data <<= dds_part_data;
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_discovered_topics (
      ::DDS::InstanceHandleSeq & impl_handles)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandleSeq rtiseq;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_discovered_topics (rtiseq);
      impl_handles <<= rtiseq;
      return retval;
#else
      return this->impl ()->get_discovered_topics (impl_handles);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_discovered_topic_data (
      ::DDS::TopicBuiltinTopicData & impl_data,
      DDS_INSTANCE_HANDLE_T_IN impl_handle)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipant_i::get_discovered_topic_data");
      DDS_TopicBuiltinTopicData dds_tp_data;
      dds_tp_data <<= impl_data;
      ::DDS_InstanceHandle_t dds_hnd;
      dds_hnd <<= impl_handle;
      ::DDS_ReturnCode_t retcode =
        this->impl ()->get_discovered_topic_data (dds_tp_data,
                                                  dds_hnd);
      impl_data <<= dds_tp_data;
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::CORBA::Boolean
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::contains_entity (
      DDS_INSTANCE_HANDLE_T_IN a_handle)
    {
      ::DDS_InstanceHandle_t ccm_dds_handle;
      ccm_dds_handle <<= a_handle;
      return this->impl ()->contains_entity (ccm_dds_handle);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_current_time (
      ::DDS::Time_t & current_time)
    {
      DDS_Time_t ccm_dds_time;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_current_time (ccm_dds_time);
      current_time <<= ccm_dds_time;
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::enable (void)
    {
      return this->impl ()->enable ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::StatusCondition_ptr
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval = ::DDS::StatusCondition::_nil ();
      DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
      if (sc)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_StatusCondition_i (sc),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::StatusMask
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDS_INSTANCE_HANDLE_T_RETN
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_instance_handle (void)
    {
      ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSDomainParticipant *
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_impl (
      DDSDomainParticipant * dp)
    {
      this->impl_ = dp;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSDomainParticipant *
    CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}
