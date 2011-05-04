// $Id$

#include "dds4ccm/impl/ndds/Subscriber.h"
#include "dds4ccm/impl/ndds/Topic.h"
#include "dds4ccm/impl/ndds/ContentFilteredTopic.h"
#include "dds4ccm/impl/ndds/DataReader_T.h"
#include "dds4ccm/impl/ndds/DataReaderListener.h"
#include "dds4ccm/impl/ndds/StatusCondition.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/TypeSupport.h"
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/SubscriberListener.h"

#include "dds4ccm/impl/ndds/convertors/InstanceHandle_t.h"
#include "dds4ccm/impl/ndds/convertors/DataReaderQos.h"
#include "dds4ccm/impl/ndds/convertors/SubscriberQos.h"
#include "dds4ccm/impl/ndds/convertors/TopicQos.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_Subscriber_i::DDS_Subscriber_i (::DDSSubscriber * sub,
                                    ::DDS::DomainParticipant_ptr dp)
      : rti_entity_ (sub),
        dp_ (::DDS::DomainParticipant::_duplicate (dp))
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::DDS_Subscriber_i");
    }

    DDS_Subscriber_i::~DDS_Subscriber_i (void)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::~DDS_Subscriber_i");
    }

    ::DDS::ReturnCode_t
    DDS_Subscriber_i::enable (void)
    {
      return this->rti_entity ()->enable ();
    }

    ::DDS::StatusCondition_ptr
    DDS_Subscriber_i::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval;
      ::DDSStatusCondition* sc = this->rti_entity ()->get_statuscondition ();
      if (sc)
        {
          ACE_NEW_THROW_EX (retval,
                            DDS_StatusCondition_i (sc, this->dp_.in ()),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    ::DDS::StatusMask
    DDS_Subscriber_i::get_status_changes (void)
    {
      return this->rti_entity ()->get_status_changes ();
    }

    ::DDS::InstanceHandle_t
    DDS_Subscriber_i::get_instance_handle (void)
    {
      ::DDS_InstanceHandle_t const rtihandle =
        this->rti_entity ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }

    DDSDataReader *
    DDS_Subscriber_i::create_datareader (
                  DDSContentFilteredTopic * topic,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask,
                   const ::DDS::DataReaderQos & qos)
    {
      ACE_UNUSED_ARG (qos);
      DDS_DataReaderQos ccm_dds_qos = DDS_DATAREADER_QOS_DEFAULT;
      return this->rti_entity ()->create_datareader (topic,
                                               ccm_dds_qos,
                                               ccm_dds_drl,
                                               mask);
    }

    DDSDataReader *
    DDS_Subscriber_i::create_datareader (
                  DDSTopic * topic,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask,
                   const ::DDS::DataReaderQos & qos)
    {
      ACE_UNUSED_ARG (qos);
      DDS_DataReaderQos ccm_dds_qos = DDS_DATAREADER_QOS_DEFAULT;
      return this->rti_entity ()->create_datareader (topic,
                                               ccm_dds_qos,
                                               ccm_dds_drl,
                                               mask);
    }

    DDSDataReader *
    DDS_Subscriber_i::create_datareader_with_profile (
                  DDSContentFilteredTopic * topic,
                  const char * qos_profile,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask)
    {
      char * lib_name = get_library_name(qos_profile);
      char * prof_name = get_profile_name(qos_profile);

      if (lib_name == 0 || prof_name == 0)
        {
          ACE_OS::free (lib_name);
          ACE_OS::free (prof_name);
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_i::create_datareader_with_profile <%C> - "
                        "Malformed qos_profile. Expected format: "
                        "<library_name>#<profile_name>\n",
                        qos_profile));
          throw ::CORBA::INTERNAL ();
        }

      DDSDataReader * dr = this->rti_entity ()->create_datareader_with_profile (
                                                            topic,
                                                            lib_name,
                                                            prof_name,
                                                            ccm_dds_drl,
                                                            mask);
      ACE_OS::free (lib_name);
      ACE_OS::free (prof_name);

      return dr;
    }

    DDSDataReader *
    DDS_Subscriber_i::create_datareader_with_profile (
                  DDSTopic * topic,
                  const char * qos_profile,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask)
    {
      char * lib_name = get_library_name (qos_profile);
      char * prof_name = get_profile_name (qos_profile);

      if (lib_name == 0 || prof_name == 0)
        {
          ACE_OS::free (lib_name);
          ACE_OS::free (prof_name);
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_i::create_datareader_with_profile <%C> - "
                        "Malformed qos_profile. Expected format: "
                        "<library_name>#<profile_name>\n",
                        qos_profile));
          throw ::CORBA::INTERNAL ();
        }

      DDSDataReader * dr = this->rti_entity ()->create_datareader_with_profile (topic,
                                                            lib_name,
                                                            prof_name,
                                                            ccm_dds_drl,
                                                            mask);
      ACE_OS::free (lib_name);
      ACE_OS::free (prof_name);
      return dr;
    }

    ::DDS::DataReader_ptr
    DDS_Subscriber_i::create_datareader (
      ::DDS::TopicDescription_ptr a_topic,
      const ::DDS::DataReaderQos & qos,
      ::DDS::DataReaderListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::create_datareader");
      DDS_DataReaderListener_i *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            DDS_DataReaderListener_i (a_listener,
                                                      ::DDS::DataReader::_nil ()),
                            ::CORBA::NO_MEMORY ());
        }

      DDSDataReader * ccm_dds_dr = 0;
      DDS_Topic_i* topic = dynamic_cast < DDS_Topic_i * > (a_topic);

      if (!topic)
        {
          DDS_ContentFilteredTopic_i * cf_topic =
            dynamic_cast < DDS_ContentFilteredTopic_i * > (a_topic);
          if (!cf_topic)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                          "DDS_Subscriber_i::create_datareader - Error: Unable to "
                          "cast provided topic to one of its servant.\n"));
              delete ccm_dds_drl;
              return ::DDS::DataReader::_nil ();
            }
          else
            ccm_dds_dr = this->create_datareader (cf_topic->get_rti_entity (),
                                                  ccm_dds_drl,
                                                  mask,
                                                  qos);
        }
      else
        ccm_dds_dr = this->create_datareader (topic->get_rti_entity (),
                                              ccm_dds_drl,
                                              mask,
                                              qos);

      if (!ccm_dds_dr)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO "DDS_Subscriber_i"
                       "::create_datareader - "
                       "Error: RTI Topic returned a nil datareader.\n"));
          delete ccm_dds_drl;
          return ::DDS::DataReader::_nil ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_Subscriber_i::create_datareader - "
                        "Successfully created datareader.\n"));
        }

      ::DDS::DataReader_var reader = DDS_TypeSupport_i::create_datareader (
                                                                ccm_dds_dr,
                                                                this->dp_.in (),
                                                                this);
      if (ccm_dds_drl)
        {
          ccm_dds_drl->set_dds_entity (reader.in ());
        }
      return reader._retn ();
    }


    ::DDS::DataReader_ptr
    DDS_Subscriber_i::create_datareader_with_profile (
      ::DDS::TopicDescription_ptr a_topic,
      const char * qos_profile,
      ::DDS::DataReaderListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::create_datareader_with_profile");
      DDS_DataReaderListener_i *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            DDS_DataReaderListener_i (a_listener, ::DDS::DataReader::_nil ()),
                            ::CORBA::NO_MEMORY ());
        }

      DDSDataReader * ccm_dds_dr = 0;
      DDS_Topic_i * topic = dynamic_cast < DDS_Topic_i * > (a_topic);

      if (!topic)
        {
          DDS_ContentFilteredTopic_i * cf_topic =
            dynamic_cast < DDS_ContentFilteredTopic_i * > (a_topic);
          if (!cf_topic)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_Subscriber_i::create_datareader_with_profile <%C> - "
                            "Error: Unable to cast provided topic to one "
                            "of its servant.\n",
                            qos_profile));
              delete ccm_dds_drl;
              return ::DDS::DataReader::_nil ();
            }
          else
            {
              ccm_dds_dr = this->create_datareader_with_profile (cf_topic->get_rti_entity (),
                                                           qos_profile,
                                                           ccm_dds_drl,
                                                           mask);
            }
        }
      else
        {
          ccm_dds_dr = this->create_datareader_with_profile (topic->get_rti_entity (),
                                                             qos_profile,
                                                             ccm_dds_drl,
                                                             mask);
        }

      if (!ccm_dds_dr)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_i::create_datareader_with_profile <%C>- "
                        "Error: RTI Topic returned a nil datareader.\n",
                        qos_profile));
          delete ccm_dds_drl;
          return ::DDS::DataReader::_nil ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_Subscriber_i::create_datareader_with_profile - "
                        "Successfully created datareader with profile <%C>.\n",
                        qos_profile));
        }

      ::DDS::DataReader_var reader = DDS_TypeSupport_i::create_datareader (ccm_dds_dr,
                                                                           this->dp_.in (),
                                                                           this);
      if (ccm_dds_drl)
        {
          ccm_dds_drl->set_dds_entity (reader.in ());
        }
      return reader._retn ();
    }

    ::DDS::ReturnCode_t
    DDS_Subscriber_i::delete_datareader (
      ::DDS::DataReader_ptr a_datareader)
    {
      DDS_DataReader_Base *dr = dynamic_cast< DDS_DataReader_Base *> (a_datareader);
      if (!dr)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_i::delete_datareader - Unable to cast "
                        "provided object reference to servant.\n"));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_SUCCESSFUL, (LM_TRACE, DDS4CCM_INFO
                    "DDS_Subscriber_i::delete_datareader - "
                    "Successfully casted provided object reference "
                    "to servant.\n"));

      DDS_ReturnCode_t const retval = this->rti_entity ()->delete_datareader (dr->get_rti_entity ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_i::delete_datareader - "
                        "Error: Returned non-ok error code %C\n",
                        ::CIAO::DDS4CCM::translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_Subscriber_i::delete_datareader - "
                        "Datareader successfully deleted\n"));
        }

      return retval;
    }

    ::DDS::ReturnCode_t
    DDS_Subscriber_i::delete_contained_entities (void)
    {
      return this->rti_entity ()->delete_contained_entities ();
    }

    ::DDS::DataReader_ptr
    DDS_Subscriber_i::lookup_datareader (
      const char * impl_name)
    {
      ::DDS::DataReader_var retval;
      DDSDataReader* dr = this->rti_entity ()->lookup_datareader (impl_name);
      if (dr)
        {
          retval = DDS_TypeSupport_i::create_datareader (dr,
                                                         this->dp_.in (),
                                                         this);
        }
      return retval._retn();
    }


    ::DDS::ReturnCode_t
    DDS_Subscriber_i::get_datareaders (
      ::DDS::DataReaderSeq & readers,
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::set_qos");
      ::DDSDataReaderSeq dds_readers;

      ::DDS_ReturnCode_t retcode =
        this->rti_entity ()->get_datareaders (dds_readers,
                                        sample_states,
                                        view_states,
                                        instance_states);
      if (retcode == DDS_RETCODE_OK)
        {
          readers.length (dds_readers.length ());
          for (::DDS_Long i = 0; i < dds_readers.length (); ++i)
            {
              ::DDS::DataReader_var rdr = DDS_TypeSupport_i::create_datareader (
                                                                dds_readers[i],
                                                                this->dp_.in (),
                                                                this);
              readers [i] = rdr._retn ();
            }
        }
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_Subscriber_i::notify_datareaders (void)
    {
      return this->rti_entity ()->notify_datareaders ();
    }


    ::DDS::ReturnCode_t
    DDS_Subscriber_i::set_qos (
      const ::DDS::SubscriberQos & qos)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::set_qos");
      ::DDS_SubscriberQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->rti_entity ()->get_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_Subscriber_i::get_qos (
      ::DDS::SubscriberQos & qos)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::get_qos");
      ::DDS_SubscriberQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode = this->rti_entity ()->get_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_Subscriber_i::set_listener (
      ::DDS::SubscriberListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::set_listener");

      // Delete the previously set listener
      DDSSubscriberListener *listener = this->rti_entity ()->get_listener ();
      delete listener;
      listener = 0;

      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (listener,
                            DDS_SubscriberListener_i (a_listener, this->dp_.in ()),
                            ::CORBA::NO_MEMORY ());
        }
      return this->rti_entity ()->set_listener (listener, mask);
    }


    ::DDS::SubscriberListener_ptr
    DDS_Subscriber_i::get_listener (void)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::get_listener");

      DDSSubscriberListener *ccm_dds_impl_list = this->rti_entity ()->get_listener ();
      DDS_SubscriberListener_i *list_proxy =
        dynamic_cast <DDS_SubscriberListener_i *> (ccm_dds_impl_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_Subscriber_i::get_listener - "
                        "DDS returned a NIL listener.\n"));
          return ::DDS::SubscriberListener::_nil ();
        }
      return list_proxy->get_subscriber_listener ();
    }


    ::DDS::ReturnCode_t
    DDS_Subscriber_i::begin_access (void)
    {
      return this->rti_entity ()->begin_access ();
    }


    ::DDS::ReturnCode_t
    DDS_Subscriber_i::end_access (void)
    {
      return this->rti_entity ()->end_access ();
    }


    ::DDS::DomainParticipant_ptr
    DDS_Subscriber_i::get_participant (void)
    {
      return DDS::DomainParticipant::_duplicate (this->dp_.in ());
    }


    ::DDS::ReturnCode_t
    DDS_Subscriber_i::set_default_datareader_qos (
      const ::DDS::DataReaderQos & qos)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::set_default_datareader_qos");
      ::DDS_DataReaderQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->rti_entity ()->set_default_datareader_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_Subscriber_i::get_default_datareader_qos (
      ::DDS::DataReaderQos & qos)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::get_default_datareader_qos");
      ::DDS_DataReaderQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode = this->rti_entity ()->get_default_datareader_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_Subscriber_i::copy_from_topic_qos (
      ::DDS::DataReaderQos & a_datareader_qos,
      const ::DDS::TopicQos & a_impl_qos)
    {
      DDS4CCM_TRACE ("DDS_Subscriber_i::copy_from_topic_qos");
      ::DDS_DataReaderQos ccm_dds_qos;
      ::DDS_TopicQos ccm_dds_topic_qos;

      ccm_dds_qos <<= a_datareader_qos;
      ccm_dds_topic_qos <<= a_impl_qos;
      ::DDS::ReturnCode_t retcode =
          this->rti_entity ()->copy_from_topic_qos (ccm_dds_qos,
                                             ccm_dds_topic_qos);
      a_datareader_qos <<= ccm_dds_qos;
      return retcode;
    }

    DDSSubscriber *
    DDS_Subscriber_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }

    void
    DDS_Subscriber_i::set_rti_entity (DDSSubscriber * sub)
    {
      this->rti_entity_ = sub;
    }

    DDSSubscriber *
    DDS_Subscriber_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DDS_Subscriber_i::rti_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
    }
  }
}

