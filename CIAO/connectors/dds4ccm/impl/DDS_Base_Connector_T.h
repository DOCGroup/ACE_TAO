/**
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 */
#ifndef DDS_BASE_CONNECTOR_T_H_
#define DDS_BASE_CONNECTOR_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"
#include "dds4ccm/impl/ndds/DomainParticipantFactory.h"
#include "dds4ccm/impl/logger/Logger_Service.h"

template <typename CCM_TYPE>
class DDS_Base_Connector_T
  : public virtual CCM_TYPE::base_type,
    public virtual ::CORBA::LocalObject,
    private virtual ACE_Copy_Disabled
{
typedef ::CIAO::NDDS::DDS_DomainParticipantFactory_i
  DomainParticipantFactory;

public:
  DDS_Base_Connector_T (void);
  virtual ~DDS_Base_Connector_T (void);

  /**
   * @name domain_id
   * Accessors for domain_id
   */
  //@{
  virtual ::DDS::DomainId_t domain_id (void);

  virtual void domain_id (::DDS::DomainId_t domain_id);
  //@}

  /**
   * @name qos_profile
   * Accessors for qos_profile
   */
  //@{
  virtual char *qos_profile (void);

  virtual void qos_profile (const char * qos_profile);
  //@}

  virtual void set_session_context (::Components::SessionContext_ptr ctx);

  /**
    *
    * Interface methods for the Deployment and Configuration Framework
    */
  //@{
  virtual void configuration_complete (void);
  virtual void ccm_remove (void);
  virtual void ccm_activate (void);
  virtual void ccm_passivate (void);
  //@}

protected:

  /**
   * Initialization of a connector.
   *
   * When one wants to use his own DDS::DomainParticipant, DDS::Topic,
   * DDS::Subscriber and/or DDS::Publisher, these helper methods allows
   * to create these, independent of the DDS vendor.
   *
   * See
   * $CIAO_ROOT/connectors/dds4ccm/tests/MultiTopic/Connector/MultiTopic_Connector_T.cpp
   * on how to use these methods.
   *
   * These methods will create the DDS4CCM Entities (and therefor the DDS
   * entities in DDS itself), like DomainParticipant, Topic, Subscriber and Publisher
   *
   */
  //@{
  void init_domain (::DDS::DomainParticipant_ptr & participant);

  virtual void register_type    (::DDS::DomainParticipant_ptr participant,
                         const char * typesupport_name) = 0;

  void init_topic       (::DDS::DomainParticipant_ptr participant,
                         ::DDS::Topic_ptr & topic,
                         const char * topic_name,
                         const char * typesupport_name);
  void init_publisher   (::DDS::DomainParticipant_ptr participant,
                         ::DDS::Publisher_ptr & publisher);
  void init_subscriber  (::DDS::DomainParticipant_ptr participant,
                         ::DDS::Subscriber_ptr & subscriber);
  //@}

  /**
   * Activation of a connector.
   *
   * Helper methods to create the DDS listeners and attach them to the DDS Entities.
   * This will activate the DDS Entities.
   *
   */
  //@{
  void activate_topic       (ACE_Reactor* reactor,
                             ::DDS::Topic_ptr topic,
                             ::DDS::TopicListener_ptr & listener);
  void activate_publisher   (ACE_Reactor* reactor,
                             ::DDS::Publisher_ptr publisher,
                             ::DDS::PublisherListener_ptr & publisher_listener);
  void activate_subscriber  (ACE_Reactor* reactor,
                             ::DDS::Subscriber_ptr subscriber,
                             ::DDS::SubscriberListener_ptr & subscriber_listener);
  //@}

  /**
   * Passivation of a connector.
   *
   * Helper methods to detach the DDS listeners from the DDS Entities.
   * This'll passivate the DDS Entities.
   *
   * In order to allow a thread safe and memory leak free passivation, one should
   * use a temporary _var variable and assign this by using _retn () in the calling
   * methods.
   *
   * For example:
   *
   * ::DDS::TopicListener_var tmp = this->topic_listener_._retn ();
   * if (!::CORBA::is_nil (tmp.in ())) this->passivate_topi (this->topic_.in (),
   *                                                         tmp.in ());
   *
   * Where topic_listener_ and topic_ are _var class members of a derived, user
   * defined class.
   *
   * Again, see
   * $CIAO_ROOT/connectors/dds4ccm/tests/MultiTopic/Connector/MultiTopic_Connector_T.cpp
   * on how to passivate a connector.
   *
   */
  //@{
  void passivate_topic      (::DDS::Topic_ptr topic,
                             ::DDS::TopicListener_ptr topic_listener);
  void passivate_publisher  (::DDS::Publisher_ptr publisher,
                             ::DDS::PublisherListener_ptr publisher_listener);
  void passivate_subscriber (::DDS::Subscriber_ptr subscriber,
                             ::DDS::SubscriberListener_ptr subscriber_listener);
  //@}

  /**
   * Removal of a connector.
   *
   * Helper methods to remove the DDS Entities from memory.
   *
   * Again, one should use a temporary _var variable and _retn () in order to
   * remove the DDS Entities in a thread safe and memory leak free manner.
   * See 'Passivation of a connector' in this file.
   */
  //@{
  void remove_topic       (::DDS::DomainParticipant_ptr participant,
                           ::DDS::Topic_ptr topic);
  void remove_publisher   (::DDS::DomainParticipant_ptr participant,
                           ::DDS::Publisher_ptr publisher);
  void remove_subscriber  (::DDS::DomainParticipant_ptr participant,
                           ::DDS::Subscriber_ptr subscriber);
  void remove_domain      (::DDS::DomainParticipant_ptr participant);

  virtual
  void unregister_type    (::DDS::DomainParticipant_ptr participant,
                           const char * typesupport_name) = 0;
  //@}

  /// Get the reactor associated with this component
  ACE_Reactor * reactor (void);

  /**
   * Attribute of DDS4CCM Base
   */
  //@{
  ::DDS::DomainId_t domain_id_;
  ::CORBA::String_var qos_profile_;
  //@}

  /**
   * @name configuration_complete_
   * Indicates whether configuration_complete has been invoked. Once invoked,
   * changing a connectors attribute is not allowed anymore. If the user still
   * wants to set an attribute, a NonChangeable exception is thrown.
    */
  bool configuration_complete_;

  ::CIAO::DDS4CCM::Logger_Service * dlf_;

  ::DDS::DomainParticipant_var domain_participant_;

  /**
    * Context of the connector. Used to make connection to the user component
    * like the interface to the data listeners.
    */
  typename CCM_TYPE::context_type::_var_type context_;

  /**
    * DomainParticipantFactory. Administration of Domain Participants
    */
  DomainParticipantFactory participant_factory_;
};

#include "dds4ccm/impl/DDS_Base_Connector_T.cpp"

#endif /* DDS_BASE_CONNECTOR_T_H_ */
