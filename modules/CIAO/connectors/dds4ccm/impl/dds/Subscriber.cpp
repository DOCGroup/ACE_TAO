// $Id$

#include "Subscriber.h"
#include "SubscriberListener.h"
#include "Topic.h"
#include "ContentFilteredTopic.h"
#include "DataReader.h"
#include "DataReaderListener.h"
#include "Utils.h"
#include "StatusCondition.h"

#include "ndds/InstanceHandle_t.h"
#include "ndds/DataReaderQos.h"
#include "ndds/SubscriberQos.h"
#include "ndds/TopicQos.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_Subscriber_i::CCM_DDS_Subscriber_i (DDSSubscriber * sub)
      : impl_ (sub)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_i::CCM_DDS_Subscriber_i");
    }

    CCM_DDS_Subscriber_i::~CCM_DDS_Subscriber_i (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_i::~CCM_DDS_Subscriber_i");
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::enable (void)
    {
      return this->impl ()->enable ();
    }

    ::DDS::StatusCondition_ptr
    CCM_DDS_Subscriber_i::get_statuscondition (void)
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
      if (! ::CORBA::is_nil (sc.in ()))
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_StatusCondition_i (sc.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return retval._retn ();
    }

    ::DDS::StatusMask
    CCM_DDS_Subscriber_i::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    ::DDS::InstanceHandle_t
    CCM_DDS_Subscriber_i::get_instance_handle (void)
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

    DDSDataReader *
    CCM_DDS_Subscriber_i::create_datareader (
                  DDSContentFilteredTopic * topic,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask,
                   const ::DDS::DataReaderQos & qos)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (qos);
      DDS_DataReaderQos ccm_dds_qos = DDS_DATAREADER_QOS_DEFAULT;
      return this->impl ()->create_datareader (topic,
                                               ccm_dds_qos,
                                               ccm_dds_drl,
                                               mask);
#else
      return this->impl ()->create_datareader (topic,
                                               qos,
                                               ccm_dds_drl,
                                               mask);
#endif
    }

    DDSDataReader *
    CCM_DDS_Subscriber_i::create_datareader (
                  DDSTopic * topic,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask,
                   const ::DDS::DataReaderQos & qos)
    {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (qos);
      DDS_DataReaderQos ccm_dds_qos = DDS_DATAREADER_QOS_DEFAULT;
      return this->impl ()->create_datareader (topic,
                                               ccm_dds_qos,
                                               ccm_dds_drl,
                                               mask);
#else
      return this->impl ()->create_datareader (topic,
                                               qos,
                                               ccm_dds_drl,
                                               mask);
#endif
    }

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
    DDSDataReader *
    CCM_DDS_Subscriber_i::create_datareader_with_profile (
                  DDSContentFilteredTopic * topic,
                  const char * library_name,
                  const char * profile_name,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask)
    {
      return this->impl ()->create_datareader_with_profile (topic,
                                                            library_name,
                                                            profile_name,
                                                            ccm_dds_drl,
                                                            mask);
    }
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
    DDSDataReader *
    CCM_DDS_Subscriber_i::create_datareader_with_profile (
                  DDSTopic * topic,
                  const char * library_name,
                  const char * profile_name,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask)
    {
      return this->impl ()->create_datareader_with_profile (topic,
                                                            library_name,
                                                            profile_name,
                                                            ccm_dds_drl,
                                                            mask);
    }
#endif

    ::DDS::DataReader_ptr
    CCM_DDS_Subscriber_i::create_datareader (
      ::DDS::TopicDescription_ptr a_topic,
      const ::DDS::DataReaderQos & qos,
      ::DDS::DataReaderListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_i::create_datareader");
      ::DDS::DataReader_var retval = ::DDS::DataReader::_nil ();
      DDSDataReaderListener *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            CCM_DDS_DataReaderListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }

      DDSDataReader * ccm_dds_dr = 0;
      CCM_DDS_Topic_i * topic = dynamic_cast < CCM_DDS_Topic_i * > (a_topic);

      if (!topic)
        {
          CCM_DDS_ContentFilteredTopic_i * cf_topic =
            dynamic_cast < CCM_DDS_ContentFilteredTopic_i * > (a_topic);
          if (!cf_topic)
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Subscriber_i::create_datareader - "
                          "Error: Unable to cast provided topic to one of its servant.\n"));
              delete ccm_dds_drl;
              throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
            }
          else
            ccm_dds_dr = this->create_datareader (cf_topic->get_impl (), ccm_dds_drl, mask, qos);
        }
      else
        ccm_dds_dr = this->create_datareader (topic->get_impl (), ccm_dds_drl, mask, qos);

      if (!ccm_dds_dr)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Subscriber_i::create_datareader - "
                       "Error: RTI Topic returned a nil datareader.\n"));
          delete ccm_dds_drl;
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CCM_DDS_Subscriber_i::create_datareader_with_profile - "
                       "Successfully created datareader.\n"));
        }

      ccm_dds_dr->enable ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DataReader_i (ccm_dds_dr),
                        CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    ::DDS::DataReader_ptr
    CCM_DDS_Subscriber_i::create_datareader_with_profile (
      ::DDS::TopicDescription_ptr a_topic,
      const char * library_name,
      const char * profile_name,
      ::DDS::DataReaderListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_i::create_datareader_with_profile");
      DDSDataReaderListener *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            CCM_DDS_DataReaderListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }

      DDSDataReader * ccm_dds_dr = 0;
      CCM_DDS_Topic_i * topic = dynamic_cast < CCM_DDS_Topic_i * > (a_topic);

      if (!topic)
        {
          CCM_DDS_ContentFilteredTopic_i * cf_topic =
            dynamic_cast < CCM_DDS_ContentFilteredTopic_i * > (a_topic);
          if (!cf_topic)
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Subscriber_i::create_datareader_with_profile - "
                          "Error: Unable to cast provided topic to one of its servant.\n"));
              delete ccm_dds_drl;
              throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
            }
          else
            {
              ccm_dds_dr = this->create_datareader_with_profile (cf_topic->get_impl (),
                                                           library_name,
                                                           profile_name,
                                                           ccm_dds_drl,
                                                           mask);
            }
        }
      else
        {
          ccm_dds_dr = this->create_datareader_with_profile (topic->get_impl (),
                                                             library_name,
                                                             profile_name,
                                                             ccm_dds_drl,
                                                             mask);
        }

      if (!ccm_dds_dr)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Subscriber_i::create_datareader_with_profile - "
                       "Error: RTI Topic returned a nil datareader.\n"));
          delete ccm_dds_drl;
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CCM_DDS_Subscriber_i::create_datareader_with_profile - "
                       "Successfully created datareader with profile <%C#%C>.\n",
                       library_name,
                       profile_name));
        }

      ::DDS::DataReader_var retval = ::DDS::DataReader::_nil ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DataReader_i (ccm_dds_dr),
                        CORBA::NO_MEMORY ());

      ccm_dds_dr->enable ();

      return retval._retn ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::delete_datareader (
      ::DDS::DataReader_ptr a_datareader)
    {
      CCM_DDS_DataReader_i *dr = dynamic_cast< CCM_DDS_DataReader_i *> (a_datareader);
      if (!dr)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Subscriber_i::delete_datareader - "
                       "Unable to cast provided object reference to servant.\n"));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_Subscriber_i::delete_datareader - "
                   "Successfully casted provided object reference to servant.\n"));

      DDS_ReturnCode_t const retval = this->impl ()->delete_datareader (dr->get_impl ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Subscriber_i::delete_datareader - "
                       "Error: Returned non-ok error code %C\n",
                       translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_Subscriber_i::delete_datareader - "
                        "Datareader successfully  deleted\n"));
        }

      return retval;
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::delete_contained_entities (void)
    {
      return this->impl ()->delete_contained_entities ();
    }

    ::DDS::DataReader_ptr
    CCM_DDS_Subscriber_i::lookup_datareader (
      const char * impl_name)
    {
      ::DDS::DataReader_var retval = ::DDS::DataReader::_nil ();
      DDSDataReader* dr = this->impl ()->lookup_datareader (impl_name);
      if (dr)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_DataReader_i (dr),
                            CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::get_datareaders (
      ::DDS::DataReaderSeq & readers,
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states)
    {
      CIAO_TRACE ("CCM_DDS_Subscriber_i::set_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDSDataReaderSeq dds_readers;

      ::DDS_ReturnCode_t retcode =
        this->impl ()->get_datareaders (dds_readers,
                                        sample_states,
                                        view_states,
                                        instance_states);
      if (retcode == DDS_RETCODE_OK)
        {
          readers.length (dds_readers.length ());
          for (::DDS_Long i = 0; i < dds_readers.length (); ++i)
            {
              ::DDS::DataReader_var rdr;
              ACE_NEW_THROW_EX (rdr,
                                CCM_DDS_DataReader_i (dds_readers[i]),
                                CORBA::NO_MEMORY ());
              readers [i] = rdr._retn ();
            }
        }
      return retcode;
#else
      return this->impl ()->get_datareaders (readers,
                                             sample_states,
                                             view_states,
                                             instance_states);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::notify_datareaders (void)
    {
      return this->impl ()->notify_datareaders ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::set_qos (
      const ::DDS::SubscriberQos & qos)
    {
      CIAO_TRACE ("CCM_DDS_Subscriber_i::set_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_SubscriberQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl ()->get_qos (ccm_dds_qos);
#else
      return this->impl ()->set_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::get_qos (
      ::DDS::SubscriberQos & qos)
    {
      CIAO_TRACE ("CCM_DDS_Subscriber_i::get_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_SubscriberQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode = this->impl ()->get_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
#else
      return this->impl ()->get_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::set_listener (
      ::DDS::SubscriberListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_i::set_listener");

      CCM_DDS_SubscriberListener_i* ccm_dds_impl_list = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            CCM_DDS_SubscriberListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }
      return this->impl ()->set_listener (ccm_dds_impl_list, mask);
    }

    ::DDS::SubscriberListener_ptr
    CCM_DDS_Subscriber_i::get_listener (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_i::get_listener");

      DDSSubscriberListener *ccm_dds_impl_list = this->impl ()->get_listener ();
      CCM_DDS_SubscriberListener_i *list_proxy = dynamic_cast <CCM_DDS_SubscriberListener_i *> (ccm_dds_impl_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, "CCM_DDS_Subscriber_i::get_listener - "
                                    "DDS returned a NIL listener.\n"));
          return ::DDS::SubscriberListener::_nil ();
        }
      return list_proxy->get_subscriber_listener ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::begin_access (void)
    {
      return this->impl ()->begin_access ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::end_access (void)
    {
      return this->impl ()->end_access ();
    }

    ::DDS::DomainParticipant_ptr
    CCM_DDS_Subscriber_i::get_participant (void)
    {
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipant* p = this->impl ()->get_participant ();
      if (p)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_DomainParticipant_i (p),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDS::DomainParticipant_var p = this->impl ()->get_participant ();
      if (! ::CORBA::is_nil (p.in))
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_DomainParticipant_i (p.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return retval._retn ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::set_default_datareader_qos (
      const ::DDS::DataReaderQos & qos)
    {
      CIAO_TRACE ("CCM_DDS_Subscriber_i::set_default_datareader_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataReaderQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl ()->set_default_datareader_qos (ccm_dds_qos);
#else
      return this->impl ()->set_default_datareader_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::get_default_datareader_qos (
      ::DDS::DataReaderQos & qos)
    {
      CIAO_TRACE ("CCM_DDS_Subscriber_i::get_default_datareader_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataReaderQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode = this->impl ()->get_default_datareader_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
#else
      return this->impl ()->get_default_datareader_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_i::copy_from_topic_qos (
      ::DDS::DataReaderQos & a_datareader_qos,
      const ::DDS::TopicQos & a_impl_qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_i::copy_from_topic_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataReaderQos ccm_dds_qos;
      ::DDS_TopicQos ccm_dds_topic_qos;

      ccm_dds_qos <<= a_datareader_qos;
      ccm_dds_topic_qos <<= a_impl_qos;
      ::DDS::ReturnCode_t retcode =
          this->impl()->copy_from_topic_qos (ccm_dds_qos,
                                             ccm_dds_topic_qos);
      a_datareader_qos <<= ccm_dds_qos;
      return retcode;
#else
      return this->impl()->copy_from_topic_qos (a_datareader_qos,
                                                a_impl_qos);
#endif
    }

    DDSSubscriber *
    CCM_DDS_Subscriber_i::get_impl (void)
    {
      return this->impl_;
    }

    void
    CCM_DDS_Subscriber_i::set_impl (DDSSubscriber * sub)
    {
      this->impl_ = sub;
    }

    DDSSubscriber *
    CCM_DDS_Subscriber_i::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

