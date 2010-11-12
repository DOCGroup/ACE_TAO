/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_TOPICBASE_CONNECTOR_H
#define DDS_TOPICBASE_CONNECTOR_H

#include "ace/Copy_Disabled.h"

#include "dds4ccm/impl/TopicListener_T.h"
#include "dds4ccm/impl/DDS_Base_Connector_T.h"
#include "dds4ccm/impl/PublisherListener_T.h"
#include "dds4ccm/impl/SubscriberListener_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
class DDS_TopicBase_Connector_T
  : public virtual DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>
{
public:
  DDS_TopicBase_Connector_T (void);
  virtual ~DDS_TopicBase_Connector_T (void);

  virtual ::DDS::StringSeq *key_fields (void);

  virtual void topic_name (const char * topic_name);

  virtual char *topic_name (void);

  virtual void key_fields (const ::DDS::StringSeq & key_fields);

  virtual void configuration_complete (void);
  virtual void ccm_remove (void);
  virtual void ccm_activate (ACE_Reactor* reactor);
  virtual void ccm_passivate (void);

protected:
  void init_default_topic (void);
  void init_publisher (void);
  void init_subscriber (void);

  void activate_default_topic (ACE_Reactor* reactor);
  void activate_publisher (ACE_Reactor* reactor);
  void activate_subscriber (ACE_Reactor* reactor);

  void passivate_default_topic (void);
  void passivate_publisher (void);
  void passivate_subscriber (void);

  void remove_default_topic (void);
  void remove_publisher (void);
  void remove_subscriber (void);

  bool late_binding (void);
  void late_binding (bool late_binding);
  bool late_binded (const char * topic_name);

  CORBA::String_var topic_name_;
  DDS::StringSeq key_fields_;

  ::DDS::Topic_var topic_;
  ::DDS::TopicListener_var topiclistener_;

  ::DDS::Publisher_var publisher_;
  ::DDS::PublisherListener_var publisher_listener_;
  ::DDS::Subscriber_var subscriber_;
  ::DDS::SubscriberListener_var subscriber_listener_;

  typedef ::CIAO::DDS4CCM::TopicListener_T<DDS_TYPE> TopicListener;
  typedef ::CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE> SubscriberListener;
  typedef ::CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE> PublisherListener;
  typedef DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE> BaseConnector;
private:
  bool late_binding_;
};

#include "dds4ccm/impl/DDS_TopicBase_Connector_T.cpp"

#endif /* DDS_TOPICBASE_CONNECTOR_H */
