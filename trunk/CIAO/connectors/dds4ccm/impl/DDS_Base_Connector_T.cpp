// -*- C++ -*-
// $Id$

#include "ace/Env_Value_T.h"
#include "tao/ORB_Core.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

#include "dds4ccm/impl/TopicListener.h"
#include "dds4ccm/impl/PublisherListener.h"
#include "dds4ccm/impl/SubscriberListener.h"

#include "dds4ccm/impl/DomainParticipantManager.h"

#if (CIAO_DDS4CCM_NDDS==1)
# include "dds4ccm/impl/ndds/TypeSupport.h"
# include "dds4ccm/impl/ndds/DomainParticipant.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
# include "dds/DCPS/Marked_Default_Qos.h"
# include "dds/DCPS/RcHandle_T.h"
# include "dds/DCPS/RTPS/RtpsDiscovery.h"
# include "dds/DCPS/Discovery.h"
# include "dds/DCPS/transport/framework/TransportType_rch.h"
# include "dds/DCPS/transport/rtps_udp/RtpsUdpInst_rch.h"
# include "dds/DCPS/transport/rtps_udp/RtpsUdpInst.h"
# include "dds/DCPS/transport/framework/TransportRegistry.h"
# include "dds/DCPS/transport/framework/TransportConfig_rch.h"
#endif

#if defined (GEN_OSTREAM_OPS)
#include <sstream>
#endif

template <typename CCM_TYPE>
DDS_Base_Connector_T<CCM_TYPE>::DDS_Base_Connector_T (void)
  : domain_id_ (0)
  , configuration_complete_ (false)
  , dlf_ (0)
  , qos_xml_ (0)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::DDS_Base_Connector_T");

  ACE_Env_Value<int> id (ACE_TEXT("DDS4CCM_DEFAULT_DOMAIN_ID"), this->domain_id_);
  this->domain_id_ = id;
#if (CIAO_DDS4CCM_OPENDDS==1)
  ACE_Env_Value<int> oid (ACE_TEXT("OPENDDS_RTPS_DEFAULT_D0"), this->domain_id_);
  this->domain_id_ = oid;
#endif
  this->dlf_ =
    ACE_Dynamic_Service<CIAO::DDS4CCM::Logger_Service>::instance ("DDS4CCM_Logger");
  if (this->dlf_)
    {
      this->dlf_->init ();
    }
  qos_xml_ = new DDS_XML_QOS_PARSER_TYPE;
  this->create_dds_participant_factory ();
}

template <typename CCM_TYPE>
DDS_Base_Connector_T<CCM_TYPE>::~DDS_Base_Connector_T (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::~DDS_Base_Connector_T");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
                "DDS_Base_Connector_T::~DDS_Base_Connector_T - "
                "Connector has been destructed\n"));
  delete this->qos_xml_;
  this->qos_xml_ = 0;
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::create_dds_participant_factory (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::create_dds_participant_factory");
  try
    {
      if (::CORBA::is_nil (this->participant_factory_.in ()))
        {
#if (CIAO_DDS4CCM_OPENDDS==1)
          this->participant_factory_ = TheParticipantFactory;

          OpenDDS::DCPS::TransportConfig_rch config =
            OpenDDS::DCPS::TransportRegistry::instance()->get_config("dds4ccm_rtps");

          if (config.is_nil())
            {
              config =
                OpenDDS::DCPS::TransportRegistry::instance()->create_config("dds4ccm_rtps");
            }

          OpenDDS::DCPS::TransportInst_rch inst =
            OpenDDS::DCPS::TransportRegistry::instance()->get_inst("the_rtps_transport");

          if (inst.is_nil())
            {
              inst =
                OpenDDS::DCPS::TransportRegistry::instance()->create_inst("the_rtps_transport",
                                                                    "rtps_udp");
              OpenDDS::DCPS::RtpsUdpInst_rch rui =
                OpenDDS::DCPS::static_rchandle_cast<OpenDDS::DCPS::RtpsUdpInst>(inst);

              config->instances_.push_back(inst);

              OpenDDS::DCPS::TransportRegistry::instance()->global_config(config);
            }

          OpenDDS::RTPS::RtpsDiscovery_rch disc =
            new OpenDDS::RTPS::RtpsDiscovery(OpenDDS::DCPS::Discovery::DEFAULT_RTPS);

          TheServiceParticipant->add_discovery(OpenDDS::DCPS::static_rchandle_cast<OpenDDS::DCPS::Discovery>(disc));
          TheServiceParticipant->set_repo_domain(this->domain_id (), disc->key());
          //TheServiceParticipant->set_default_discovery (OpenDDS::DCPS::Discovery::DEFAULT_RTPS);
#else
          this->participant_factory_ =
            new ::CIAO::NDDS::DDS_DomainParticipantFactory_i;
#endif
        }
    }
  catch (const CORBA::Exception& e)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (DDS4CCM_LOG_LEVEL_ERROR, e,
          "DDS_Base_Connector_T::create_dds_participant_factory - "
          "Exception caught whilst setting up dds participant factory");
      throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }
}

template <typename CCM_TYPE>
::DDS::DomainId_t
DDS_Base_Connector_T<CCM_TYPE>::domain_id (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::domain_id");

  return this->domain_id_;
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::domain_id (
  ::DDS::DomainId_t domain_id)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::domain_id");

  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->domain_id_ = domain_id;

#if (CIAO_DDS4CCM_OPENDDS==1)
      OpenDDS::RTPS::RtpsDiscovery_rch disc =
        new OpenDDS::RTPS::RtpsDiscovery(OpenDDS::DCPS::Discovery::DEFAULT_RTPS);

      TheServiceParticipant->add_discovery(OpenDDS::DCPS::static_rchandle_cast<OpenDDS::DCPS::Discovery>(disc));
      TheServiceParticipant->set_repo_domain(this->domain_id (), disc->key());
#endif
    }
}

template <typename CCM_TYPE>
char *
DDS_Base_Connector_T<CCM_TYPE>::qos_profile (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::qos_profile");

  return CORBA::string_dup (this->qos_profile_.in ());
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::qos_profile (
  const char * qos_profile)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::qos_profile");

  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->qos_profile_ = ::CORBA::string_dup (qos_profile);

      if (this->qos_xml_)
        {
          DDS::ReturnCode_t const retcode = this->qos_xml_->init (qos_profile);
          if (retcode != ::DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "DDS_Base_Connector_T::qos_profile - "
                  "Error while initializing QOS Handler <%C>\n",
                  ::CIAO::DDS4CCM::translate_retcode (retcode)));
              throw ::CCM_DDS::InternalError (retcode, 0);
            }
        }
    }
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::init_domain (
  ::DDS::DomainParticipant_ptr & participant)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::init_domain");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                "DDS_Base_Connector_T::init_domain - "
                "Start configuring default domain <%d>\n",
                this->domain_id_));

#if (CIAO_DDS4CCM_NDDS==1)
  if (!::CORBA::is_nil (this->qos_profile_.in ()))
    {
      this->participant_factory_->set_default_participant_qos_with_profile (
                                      this->qos_profile_.in ());

      DDS::DomainParticipant_var dds_dp =
        DPMANAGER->get_participant (this->domain_id_, this->qos_profile_.in ());

      if (::CORBA::is_nil (dds_dp.in ()))
        {
          // Create a new participant for this qos profile and domain ID.
          participant = this->participant_factory_->create_participant_with_profile (
                                          this->domain_id_,
                                          this->qos_profile_.in (),
                                          ::DDS::DomainParticipantListener::_nil (),
                                          0);
          if (!DPMANAGER->register_participant (
            this->domain_id_, this->qos_profile_.in (), participant))
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                          "DDS_Base_Connector_T::init_domain - "
                          "Error: Unable to register DomainParticipant for domain <%d>\n",
                          this->domain_id_));
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
            }
        }
      else
        {
          // Re-use the domain_participant for this QOS profile and domain ID
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                        "DDS_Base_Connector_T::init_domain - "
                        "Re-using domainparticipant "
                        DDS_PARTICIPANT_FORMAT_SPECIFIER
                        " domain <%d> - "
                        "qos_profile <%C>\n",
                        DDS_PARTICIPANT_LOG (dds_dp.in ()),
                        this->domain_id_, this->qos_profile_.in ()));

          participant = ::DDS::DomainParticipant::_duplicate (dds_dp.in ());
        }
    }
  else
#endif
    {
      ::DDS::DomainParticipantQos qos;
      DDS::ReturnCode_t const retcode =
        this->participant_factory_->get_default_participant_qos (qos);

      if (retcode != DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
              "DDS_Base_Connector_T::init_domain - "
              "Error: Unable to retrieve default_participant_qos: <%C>\n",
              ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CCM_DDS::InternalError (retcode, 0);
        }

      if (!::CORBA::is_nil (this->qos_profile_.in ()) && this->qos_xml_)
        {
          DDS::ReturnCode_t const retcode_dp_qos =
            this->qos_xml_->get_participant_qos (
              qos,
              this->qos_profile_.in ());

          if (retcode_dp_qos != DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "DDS_Base_Connector_T::init_domain - "
                  "Error: Unable to retrieve participant QOS from XML: <%C>\n",
                  ::CIAO::DDS4CCM::translate_retcode (retcode_dp_qos)));
              throw ::CCM_DDS::InternalError (retcode_dp_qos, 0);
            }
        }

#if defined GEN_OSTREAM_OPS
      if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          std::stringstream output;
          output << qos;
          std::string message = output.str();
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_INFO, DDS4CCM_INFO
                        ACE_TEXT ("DDS_Base_Connector_T::init_domain - ")
                        ACE_TEXT ("Using participant QOS <%C>\n"), message.c_str()));
        }
#endif

      DDS::DomainParticipant_var dds_dp =
        DPMANAGER->get_participant (this->domain_id_, this->qos_profile_.in ());
      if (::CORBA::is_nil (dds_dp.in ()))
        {
          // Create a new participant for this qos profile and domain ID.
          participant = this->participant_factory_->create_participant (
                                          this->domain_id_,
                                          qos,
                                          ::DDS::DomainParticipantListener::_nil (),
                                          0);

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                        "DDS_Base_Connector_T::init_domain - "
                        "Created domainparticipant "
                        DDS_PARTICIPANT_FORMAT_SPECIFIER
                        ". domain <%d> - "
                        "qos_profile <%C>\n",
                        DDS_PARTICIPANT_LOG (participant),
                        this->domain_id_, this->qos_profile_.in ()));

          if (!DPMANAGER->register_participant (
                this->domain_id_, this->qos_profile_.in (), participant))
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                          "DDS_Base_Connector_T::init_domain - "
                          "Error: Unable to register DomainParticipant for domain <%d>\n",
                          this->domain_id_));
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
            }
        }
      else
        {
          // Re-use the domain_participant for this QOS profile and domain ID
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                        "DDS_Base_Connector_T::init_domain - "
                        "Re-using domainparticipant "
                        DDS_PARTICIPANT_FORMAT_SPECIFIER
                        ". domain <%d> - "
                        "qos_profile <%C>\n",
                        DDS_PARTICIPANT_LOG (dds_dp.in ()),
                        this->domain_id_, this->qos_profile_.in ()));
          participant = ::DDS::DomainParticipant::_duplicate (dds_dp.in ());
        }
    }

  if (::CORBA::is_nil (participant))
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "DDS_Base_Connector_T::init_domain - "
                  "Error: Unable to create DomainParticipant for domain <%d>\n",
                  this->domain_id_));
      throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::set_session_context (
  ::Components::SessionContext_ptr ctx)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::set_session_context");

  typename CCM_TYPE::context_type::_var_type lctx =
    CCM_TYPE::context_type::_narrow (ctx);

  if (::CORBA::is_nil (lctx.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }

  this->context_ = lctx;
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::configuration_complete");
  this->init_domain (this->domain_participant_.inout ());
  this->configuration_complete_ = true;
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::ccm_activate (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::ccm_activate");
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::ccm_passivate");
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::ccm_remove");

  ::DDS::DomainParticipant_var dp = this->domain_participant_._retn ();
  if (!::CORBA::is_nil (dp.in ()))
    {
      this->remove_domain (dp.in ());
    }
}

template <typename CCM_TYPE>
ACE_Reactor*
DDS_Base_Connector_T<CCM_TYPE>::reactor (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE>::reactor");

  ACE_Reactor* reactor = 0;

#if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
  ::CORBA::Object_var ccm_object = this->context_->get_CCM_object();
  if (!::CORBA::is_nil (ccm_object.in ()))
    {
      ::CORBA::ORB_var orb = ccm_object->_get_orb ();
      if (!::CORBA::is_nil (orb.in ()))
        {
          reactor = orb->orb_core ()->reactor ();
          if (!reactor)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_Event_Connector_T::ccm_activate - "
                            "No reactor to perform required context switch.\n"));
              throw ::CORBA::INTERNAL ();
            }
        }
    }
#endif
  return reactor;
}

/**
  * Initialization
 **/
template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::init_topic (
  ::DDS::DomainParticipant_ptr participant,
  ::DDS::Topic_ptr & topic,
  const char * topic_name,
  const char * typesupport_name)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::init_topic");

  ::DDS::Duration_t timeout;
  timeout.sec = 0;
  timeout.nanosec = 0;

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                "DDS_Base_Connector_T::init_topic - "
                "Finding existing topic "
                "name <%C> for profile <%C> in participant "
                DDS_PARTICIPANT_FORMAT_SPECIFIER "\n",
                topic_name, this->qos_profile_.in (),
                DDS_PARTICIPANT_LOG (participant)));

  ::DDS::Topic_var dds_tp =
    participant->find_topic (topic_name, timeout);

  if (!CORBA::is_nil (dds_tp.in ()))
    {
      // Re-use topic.
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                    "DDS_Base_Connector_T::init_topic - "
                    "Re using topic <%C> for profile <%C>\n",
                    topic_name, this->qos_profile_.in ()));

      topic = ::DDS::Topic::_duplicate (dds_tp);
      return;
    }
#if (CIAO_DDS4CCM_NDDS==1)
  if (!::CORBA::is_nil (this->qos_profile_.in ()))
    {
      // Create a new topic
      topic = participant->create_topic_with_profile (topic_name,
                                          typesupport_name,
                                          this->qos_profile_.in (),
                                          ::DDS::TopicListener::_nil (),
                                          0);
    }
  else
#endif
    {
      ::DDS::TopicQos tqos;
      DDS::ReturnCode_t const retcode =
        participant->get_default_topic_qos (tqos);

      if (retcode != DDS::RETCODE_OK)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "DDS_Base_Connector_T::init_topic - "
            "Error: Unable to retrieve default_topic_qos: <%C>\n",
            ::CIAO::DDS4CCM::translate_retcode (retcode)));
        throw ::CCM_DDS::InternalError (retcode, 0);
      }

      if (!::CORBA::is_nil (this->qos_profile_.in ()) && this->qos_xml_)
        {
          DDS::ReturnCode_t const retcode_tp_qos =
            this->qos_xml_->get_topic_qos (
                        tqos,
                        this->qos_profile_.in (),
                        topic_name);

          if (retcode_tp_qos != DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "DDS_Base_Connector_T::init_topic - "
                  "Error: Unable to retrieve topic QOS from XML: <%C>\n",
                  ::CIAO::DDS4CCM::translate_retcode (retcode_tp_qos)));
              throw ::CCM_DDS::InternalError (retcode_tp_qos, 0);
            }
        }

#if defined GEN_OSTREAM_OPS
      if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          std::stringstream output;
          output << tqos;
          std::string message = output.str();
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_INFO, DDS4CCM_INFO
                        ACE_TEXT ("DDS_Base_Connector_T::init_topic - ")
                        ACE_TEXT ("Using topic QOS <%C>\n"), message.c_str()));
        }
#endif

      // Create a new topic
      topic = participant->create_topic (topic_name,
                            typesupport_name,
                            tqos,
                            ::DDS::TopicListener::_nil (),
                            0);
    }

  if (!::CORBA::is_nil (topic))
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                    "DDS_Base_Connector_T::init_topic - "
                    "Created new topic "
                    DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER
                    " name <%C> for profile <%C>\n",
                    DDS_INSTANCE_HANDLE_LOG (topic->get_instance_handle ()),
                    topic_name, this->qos_profile_.in ()));
    }
  else
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                    "DDS_Base_Connector_T::init_topic - "
                    "Error: unable to create new topic <%C> for profile <%C>\n",
                    topic_name, this->qos_profile_.in ()));
      throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::init_publisher (
  ::DDS::DomainParticipant_ptr participant,
  ::DDS::Publisher_ptr & publisher)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::init_publisher");

  if (::CORBA::is_nil (publisher))
    {
#if (CIAO_DDS4CCM_NDDS==1)
      if (!::CORBA::is_nil (this->qos_profile_.in ()))
        {
          publisher = participant->create_publisher_with_profile (
                                              this->qos_profile_.in (),
                                              ::DDS::PublisherListener::_nil (),
                                              0);
        }
      else
#endif
        {
          ::DDS::PublisherQos pqos;
          DDS::ReturnCode_t const retcode =
            participant->get_default_publisher_qos (pqos);

          if (retcode != DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "DDS_Base_Connector_T::init_publisher - "
                  "Error: Unable to retrieve get_default_publisher_qos: <%C>\n",
                  ::CIAO::DDS4CCM::translate_retcode (retcode)));
              throw ::CCM_DDS::InternalError (retcode, 0);
            }

          if (!::CORBA::is_nil (this->qos_profile_.in ()) && this->qos_xml_)
            {
              DDS::ReturnCode_t const retcode_pub_qos =
                this->qos_xml_->get_publisher_qos (
                  pqos,
                  this->qos_profile_.in ());

              if (retcode_pub_qos != DDS::RETCODE_OK)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                      "DDS_Base_Connector_T::init_publisher - "
                      "Error: Unable to retrieve publisher QOS from XML: <%C>\n",
                      ::CIAO::DDS4CCM::translate_retcode (retcode_pub_qos)));
                  throw ::CCM_DDS::InternalError (retcode_pub_qos, 0);
                }
            }

#if defined GEN_OSTREAM_OPS
          if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
            {
              std::stringstream output;
              output << pqos;
              std::string message = output.str();
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_INFO, DDS4CCM_INFO
                            ACE_TEXT ("DDS_Base_Connector_T::init_publisher - ")
                            ACE_TEXT ("Using publisher QOS <%C>\n"), message.c_str()));
            }
#endif

          publisher = participant->create_publisher (pqos,
                                            ::DDS::PublisherListener::_nil (),
                                            0);
        }
      if (::CORBA::is_nil (publisher))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Base_Connector_T::init_publisher - "
                        "Error: Proxy returned a nil publisher.\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
    }
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::init_subscriber (
  ::DDS::DomainParticipant_ptr participant,
  ::DDS::Subscriber_ptr & subscriber)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::init_subscriber");

  if (::CORBA::is_nil (subscriber))
    {
#if (CIAO_DDS4CCM_NDDS==1)
      if (!::CORBA::is_nil (this->qos_profile_.in ()))
        {
          subscriber = participant->create_subscriber_with_profile (
                                              this->qos_profile_.in (),
                                              ::DDS::SubscriberListener::_nil (),
                                              0);
        }
      else
#endif
        {
          ::DDS::SubscriberQos sqos;
          DDS::ReturnCode_t const retcode =
            participant->get_default_subscriber_qos (sqos);

          if (retcode != DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "DDS_Base_Connector_T::init_subscriber - "
                  "Error: Unable to retrieve get_default_subscriber_qos: <%C>\n",
                  ::CIAO::DDS4CCM::translate_retcode (retcode)));
              throw ::CCM_DDS::InternalError (retcode, 0);
            }

          if (!::CORBA::is_nil (this->qos_profile_.in ()) && this->qos_xml_)
            {
              DDS::ReturnCode_t const retcode_sub_qos =
                this->qos_xml_->get_subscriber_qos (
                  sqos,
                  this->qos_profile_.in ());

              if (retcode_sub_qos != DDS::RETCODE_OK)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                      "DDS_Base_Connector_T::init_subscriber - "
                      "Error: Unable to retrieve subscriber QOS from XML: <%C>\n",
                      ::CIAO::DDS4CCM::translate_retcode (retcode_sub_qos)));
                  throw ::CCM_DDS::InternalError (retcode_sub_qos, 0);
                }
            }

#if defined GEN_OSTREAM_OPS
          if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
            {
              std::stringstream output;
              output << sqos;
              std::string message = output.str();
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_INFO, DDS4CCM_INFO
                            ACE_TEXT ("DDS_Base_Connector_T::init_subscriber - ")
                            ACE_TEXT ("Using subscriber QOS <%C>\n"), message.c_str()));
            }
#endif

          subscriber = participant->create_subscriber (sqos,
                                              ::DDS::SubscriberListener::_nil (),
                                              0);
        }
      if (::CORBA::is_nil (subscriber))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Base_Connector_T::init_subscriber - "
                        "Error: Proxy returned a nil subscriber.\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
    }
}

/**
  * Activation
 **/
template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::activate_topic (
  ACE_Reactor* reactor,
  ::DDS::Topic_ptr topic,
  ::DDS::TopicListener_ptr & listener)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::activate_topic");

  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();

  ::DDS::StatusMask const mask =
    ::CIAO::DDS4CCM::TopicListener::get_mask (error_listener.in ());

  if (mask != 0)
    {
      if (::CORBA::is_nil (listener))
        {
          ACE_NEW_THROW_EX (listener,
                            ::CIAO::DDS4CCM::TopicListener (
                              error_listener.in (),
                              reactor),
                            ::CORBA::NO_MEMORY ());

          DDS::ReturnCode_t const retcode = topic->set_listener (listener,
                                                                   mask);

          if (retcode != ::DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_Base_Connector_T::activate_topic - "
                            "Error while setting the listener on the topic - <%C>\n",
                            ::CIAO::DDS4CCM::translate_retcode (retcode)));
              throw ::CORBA::INTERNAL ();
            }
        }
    }
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::activate_publisher (
  ACE_Reactor* reactor,
  ::DDS::Publisher_ptr publisher,
  ::DDS::PublisherListener_ptr & publisher_listener)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::activate_publisher");

  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();

  ::DDS::StatusMask const mask =
      ::CIAO::DDS4CCM::PublisherListener::get_mask (error_listener.in ());

  if (mask != 0)
    {
      if (::CORBA::is_nil (publisher_listener))
        {
          ACE_NEW_THROW_EX (publisher_listener,
                            ::CIAO::DDS4CCM::PublisherListener (
                              error_listener.in (),
                              reactor),
                            ::CORBA::NO_MEMORY ());
        }

      DDS::ReturnCode_t const retcode = publisher->set_listener (publisher_listener,
                                                                   mask);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Base_Connector_T::activate_publisher - "
                        "Error while setting the listener on the publisher - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::activate_subscriber (
  ACE_Reactor* reactor,
  ::DDS::Subscriber_ptr subscriber,
  ::DDS::SubscriberListener_ptr & subscriber_listener)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::activate_subscriber");

  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();

  ::DDS::StatusMask const mask =
    ::CIAO::DDS4CCM::SubscriberListener::get_mask (error_listener.in ());

  if (mask != 0)
    {
      if (::CORBA::is_nil (subscriber_listener))
        {
          ACE_NEW_THROW_EX (subscriber_listener,
                            ::CIAO::DDS4CCM::SubscriberListener (
                              error_listener.in (),
                              reactor),
                            ::CORBA::NO_MEMORY ());
        }

      DDS::ReturnCode_t const retcode = subscriber->set_listener (subscriber_listener,
                                                                    mask);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Base_Connector_T::activate_subscriber - "
                        "Error while setting the listener on the subscriber - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

/**
  * Passivation
 **/
template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::passivate_topic (
  ::DDS::Topic_ptr topic,
  ::DDS::TopicListener_ptr topic_listener)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::passivate_topic");

  if (!::CORBA::is_nil (topic_listener))
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_Base_Connector_T::passivate_topic - "
                    "Setting the listener on the topic to nil\n"));

      DDS::ReturnCode_t const retcode =
        topic->set_listener (::DDS::TopicListener::_nil (), 0);
      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Base_Connector_T::passivate_topic - "
                        "Error while setting the listener on the topic - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::passivate_publisher (
  ::DDS::Publisher_ptr publisher,
  ::DDS::PublisherListener_ptr publisher_listener)

{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::passivate_publisher");

  if (!::CORBA::is_nil (publisher_listener))
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_Base_Connector_T::passivate_publisher - "
                    "Setting the listener on the publisher to nil\n"));

      DDS::ReturnCode_t const retcode =
        publisher->set_listener (::DDS::PublisherListener::_nil (), 0);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Base_Connector_T::passivate_publisher - "
                        "Error while setting the listener on the publisher - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::passivate_subscriber (
  ::DDS::Subscriber_ptr subscriber,
  ::DDS::SubscriberListener_ptr subscriber_listener)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::passivate_subscriber");

  if (!::CORBA::is_nil (subscriber_listener))
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_Base_Connector_T::passivate_subscriber - "
                    "Setting the listener on the subscriber to nil\n"));

      DDS::ReturnCode_t const retcode =
        subscriber->set_listener (::DDS::SubscriberListener::_nil (), 0);
      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Base_Connector_T::passivate_subscriber - "
                        "Error while setting the listener on the subscriber - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

/**
  * Removal
 **/
template <typename CCM_TYPE>
void DDS_Base_Connector_T<CCM_TYPE>::remove_topic (
  ::DDS::DomainParticipant_ptr participant,
  ::DDS::Topic_ptr topic)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::remove_topic");

  CORBA::String_var name = topic->get_name ();

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                "DDS_Base_Connector_T::remove_topic - "
                "Going to delete topic <%C> from participant"
                DDS_PARTICIPANT_FORMAT_SPECIFIER
                "\n",
                name.in (),
                DDS_PARTICIPANT_LOG (participant)));

  DDS::ReturnCode_t retcode = participant->delete_topic (topic);

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                "DDS_Base_Connector_T::remove_topic - "
                "Deleted topic <%C> from "
                DDS_PARTICIPANT_FORMAT_SPECIFIER
                " return code <%C>\n",
                name.in (),
                DDS_PARTICIPANT_LOG (participant),
                ::CIAO::DDS4CCM::translate_retcode (retcode)));

  if (retcode != ::DDS::RETCODE_OK && retcode != ::DDS::RETCODE_PRECONDITION_NOT_MET)
    {
      throw ::CCM_DDS::InternalError (retcode, 0);
    }
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::remove_publisher (
  ::DDS::DomainParticipant_ptr participant,
  ::DDS::Publisher_ptr publisher)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::remove_publisher");

  participant->delete_publisher (publisher);
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::remove_subscriber (
  ::DDS::DomainParticipant_ptr participant,
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::remove_subscriber");

  participant->delete_subscriber (subscriber);
}

template <typename CCM_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE>::remove_domain (
  ::DDS::DomainParticipant_ptr participant)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T::remove_domain");

  DDS::ReturnCode_t retcode = DDS::RETCODE_OK;

  if (DPMANAGER->unregister_participant (
      this->domain_id_, this->qos_profile_.in (), participant))
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
                "DDS_Base_Connector_T"
                "::remove_domain - "
                "Going to delete participant "
                DDS_PARTICIPANT_FORMAT_SPECIFIER
                "for domain <%d> with qos <%C>.\n",
                DDS_PARTICIPANT_LOG (participant),
                this->domain_id_, this->qos_profile_.in ()));

      retcode = this->participant_factory_->delete_participant (participant);

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
                "DDS_Base_Connector_T"
                "::remove_domain - "
                "Deleted participant "
                DDS_PARTICIPANT_FORMAT_SPECIFIER
                "for domain <%d> with qos <%C> "
                "return code <%C>\n",
                DDS_PARTICIPANT_LOG (participant),
                this->domain_id_, this->qos_profile_.in (),
                ::CIAO::DDS4CCM::translate_retcode (retcode)));
    }

  if (retcode != ::DDS::RETCODE_OK)
    {
      throw ::CCM_DDS::InternalError (retcode, 0);
    }
}
