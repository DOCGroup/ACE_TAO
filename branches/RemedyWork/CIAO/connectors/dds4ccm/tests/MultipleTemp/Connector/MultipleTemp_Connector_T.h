// $Id$

#ifndef DDS_MLT_CONNECTOR_T_H_
#define DDS_MLT_CONNECTOR_T_H_

#include "connectors/dds4ccm/impl/DDS_Base_Connector_T.h"
#include "connectors/dds4ccm/impl/DDS_Write_T.h"
#include "connectors/dds4ccm/impl/DDS_Get_T.h"
#include "connectors/dds4ccm/impl/DDS_Listen_T.h"
#include "connectors/dds4ccm/impl/DDS_Update_T.h"
#include "connectors/dds4ccm/impl/DataListenerControl_T.h"

template <typename CCM_TYPE,
          typename DDS_TYPE_T, bool FIXED_T,
          typename SEQ_TYPE_T,
          typename DDS_TYPE_Y, bool FIXED_Y,
          typename SEQ_TYPE_Y>
class FooConnector_Connector_T
  : public virtual DDS_Base_Connector_T<CCM_TYPE>
{

  typedef DDS_Base_Connector_T<CCM_TYPE> BaseConnector;

public:
  FooConnector_Connector_T (void);

  virtual
  void
  register_type (
  ::DDS::DomainParticipant_ptr participant,
  const char * typesupport_name);

  virtual
  void
  unregister_type (
  ::DDS::DomainParticipant_ptr participant,
  const char * typesupport_name);

  /**
   * T type connector
   **/
   // DDS_Listen operations
   virtual
  CCM_DDS::CCM_ContentFilterSetting*
  get_push_consumer_t_filter_config(void);

  virtual
  CCM_DDS::QueryFilter *
  push_consumer_t_filter (void);

  virtual
  void
  push_consumer_t_filter (const ::CCM_DDS::QueryFilter &);

  virtual
  typename CCM_TYPE::push_consumer_t_traits::data_type::_ptr_type
  get_push_consumer_t_data (void);

  virtual
  typename CCM_TYPE::push_consumer_t_traits::data_control_type::_ptr_type
  get_push_consumer_t_data_control (void);

  virtual
  typename CCM_TYPE::push_consumer_t_traits::dds_entity_type::_ptr_type
  get_push_consumer_t_dds_entity (void);

  // DDS_Update operations
  virtual
  typename CCM_TYPE::observable_t_traits::data_type::_ptr_type
    get_observable_t_data (void);

  virtual
  typename CCM_TYPE::observable_t_traits::dds_entity_type::_ptr_type
   get_observable_t_dds_entity (void);



  /**
    * Y type connector
   **/
  // DDS_Listen operations
  virtual
   CCM_DDS::CCM_ContentFilterSetting*
   get_push_consumer_y_filter_config(void);

   virtual
   CCM_DDS::QueryFilter *
   push_consumer_y_filter (void);

   virtual
   void
   push_consumer_y_filter (const ::CCM_DDS::QueryFilter &);

   virtual
   typename CCM_TYPE::push_consumer_y_traits::data_type::_ptr_type
   get_push_consumer_y_data (void);

   virtual
   typename CCM_TYPE::push_consumer_y_traits::data_control_type::_ptr_type
   get_push_consumer_y_data_control (void);

   virtual
   typename CCM_TYPE::push_consumer_y_traits::dds_entity_type::_ptr_type
   get_push_consumer_y_dds_entity (void);

   // DDS_Update operations
    virtual
   typename CCM_TYPE::observable_y_traits::data_type::_ptr_type
   get_observable_y_data (void);

   virtual
   typename CCM_TYPE::observable_y_traits::dds_entity_type::_ptr_type
    get_observable_y_dds_entity (void);



  /**
    * Topic names.
    **/
  virtual char * topic_name_t (void);
  virtual void topic_name_t (const char * topic_name_t);
  virtual char * topic_name_y (void);
  virtual void topic_name_y (const char * topic_name_y);

  /**
    * Deployment methods.
    **/
  virtual void configuration_complete (void);
  virtual void ccm_remove (void);
  virtual void ccm_activate (void);
  virtual void ccm_passivate (void);

private:
  DDS_Listen_T<
      typename CCM_TYPE::push_consumer_t_traits,
      typename DDS_TYPE_T::typed_reader_type,
      typename DDS_TYPE_T::value_type,
      SEQ_TYPE_T>
    push_consumer_t_;
  DDS_Listen_T<
      typename CCM_TYPE::push_consumer_y_traits,
      typename DDS_TYPE_Y::typed_reader_type,
      typename DDS_TYPE_Y::value_type,
      SEQ_TYPE_Y>
    push_consumer_y_;

  DDS_Update_T<
      typename CCM_TYPE::observable_t_traits,
      typename DDS_TYPE_T::typed_writer_type,
      typename DDS_TYPE_T::value_type,
      SEQ_TYPE_T>
    observable_t_;

  DDS_Update_T<
      typename CCM_TYPE::observable_y_traits,
      typename DDS_TYPE_Y::typed_writer_type,
      typename DDS_TYPE_Y::value_type,
      SEQ_TYPE_Y>
    observable_y_;

  //connection to the receiver implementation
  typename CCM_TYPE::push_consumer_t_traits::data_listener_type::_var_type dl_t_;
  typename CCM_TYPE::push_consumer_y_traits::data_listener_type::_var_type dl_y_;

  ::CCM_DDS::PortStatusListener_var push_consumer_t_psl;
  ::CCM_DDS::PortStatusListener_var push_consumer_y_psl;

  ::DDS::Publisher_var publisher_t_;
  ::DDS::PublisherListener_var publisher_t_listener_;
  ::DDS::Subscriber_var subscriber_t_;
  ::DDS::SubscriberListener_var subscriber_t_listener_;

  ::DDS::Publisher_var publisher_y_;
  ::DDS::PublisherListener_var publisher_y_listener_;
  ::DDS::Subscriber_var subscriber_y_;
  ::DDS::SubscriberListener_var subscriber_y_listener_;

  CORBA::String_var topic_name_t_;
  CORBA::String_var topic_name_y_;

  ::DDS::Topic_var topic_t_;
  ::DDS::Topic_var topic_y_;

  ::DDS::TopicListener_var topiclistener_t_;
  ::DDS::TopicListener_var topiclistener_y_;

  ACE_Reactor * reactor_;

  void create_topics (const char * typesupport_name);

};

#include "MultipleTemp_Connector_T.cpp"

#endif
