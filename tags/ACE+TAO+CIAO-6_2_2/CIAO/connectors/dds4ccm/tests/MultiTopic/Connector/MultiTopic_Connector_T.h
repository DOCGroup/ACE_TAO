// $Id$

#ifndef DDS_MT_CONNECTOR_T_H_
#define DDS_MT_CONNECTOR_T_H_

#include "connectors/dds4ccm/impl/DDS_TopicBase_Connector_T.h"
#include "connectors/dds4ccm/impl/DDS_Write_T.h"
#include "connectors/dds4ccm/impl/DDS_Get_T.h"
#include "connectors/dds4ccm/impl/DDS_Listen_T.h"
#include "connectors/dds4ccm/impl/DataListenerControl_T.h"

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
class DDS_MT_Event_Connector_T
  : public virtual DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>
{

typedef DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE> TopicBaseConnector;

public:
  DDS_MT_Event_Connector_T (void);

  /**
    * Square connector
    **/
  virtual
  typename CCM_TYPE::supplier_sq_traits::data_type::_ptr_type
  get_supplier_sq_data (void);

  virtual
  typename CCM_TYPE::supplier_sq_traits::dds_entity_type::_ptr_type
  get_supplier_sq_dds_entity (void);

  virtual
  typename CCM_TYPE::pull_consumer_sq_traits::fresh_data_type::_ptr_type
  get_pull_consumer_sq_fresh_data (void);

  virtual
  typename CCM_TYPE::pull_consumer_sq_traits::data_type::_ptr_type
  get_pull_consumer_sq_data (void);

  virtual
  typename CCM_TYPE::pull_consumer_sq_traits::dds_entity_type::_ptr_type
  get_pull_consumer_sq_dds_entity (void);

  virtual
  CCM_DDS::QueryFilter*
  pull_consumer_sq_filter (void);

  virtual
  void
  pull_consumer_sq_filter (const CCM_DDS::QueryFilter&);

  virtual
  ::CCM_DDS::CCM_ContentFilterSetting_ptr
  get_pull_consumer_sq_filter_config(void);

  virtual
  CCM_DDS::CCM_ContentFilterSetting*
  get_push_consumer_sq_filter_config(void);

  virtual
  CCM_DDS::QueryFilter *
  push_consumer_sq_filter (void);

  virtual
  void
  push_consumer_sq_filter (const ::CCM_DDS::QueryFilter &);

  virtual
  typename CCM_TYPE::push_consumer_sq_traits::data_type::_ptr_type
  get_push_consumer_sq_data (void);

  virtual
  typename CCM_TYPE::push_consumer_sq_traits::data_control_type::_ptr_type
  get_push_consumer_sq_data_control (void);

  virtual
  typename CCM_TYPE::push_consumer_sq_traits::dds_entity_type::_ptr_type
  get_push_consumer_sq_dds_entity (void);

  /**
    * Triangle connector
    **/
  virtual
  typename CCM_TYPE::supplier_tr_traits::data_type::_ptr_type
  get_supplier_tr_data (void);

  virtual
  typename CCM_TYPE::supplier_tr_traits::dds_entity_type::_ptr_type
  get_supplier_tr_dds_entity (void);

  virtual
  typename CCM_TYPE::pull_consumer_tr_traits::fresh_data_type::_ptr_type
  get_pull_consumer_tr_fresh_data (void);

  virtual
  typename CCM_TYPE::pull_consumer_tr_traits::data_type::_ptr_type
  get_pull_consumer_tr_data (void);

  virtual
  typename CCM_TYPE::pull_consumer_tr_traits::dds_entity_type::_ptr_type
  get_pull_consumer_tr_dds_entity (void);

  virtual
  CCM_DDS::QueryFilter*
  pull_consumer_tr_filter (void);

  virtual
  void
  pull_consumer_tr_filter (const CCM_DDS::QueryFilter&);

  virtual
  CCM_DDS::CCM_ContentFilterSetting*
  get_pull_consumer_tr_filter_config(void);

  virtual
  CCM_DDS::CCM_ContentFilterSetting*
  get_push_consumer_tr_filter_config(void);

  virtual
  CCM_DDS::QueryFilter *
  push_consumer_tr_filter (void);

  virtual
  void
  push_consumer_tr_filter (const ::CCM_DDS::QueryFilter &);

  virtual
  typename CCM_TYPE::push_consumer_tr_traits::data_type::_ptr_type
  get_push_consumer_tr_data (void);

  virtual
  typename CCM_TYPE::push_consumer_tr_traits::data_control_type::_ptr_type
  get_push_consumer_tr_data_control (void);

  virtual
  typename CCM_TYPE::push_consumer_tr_traits::dds_entity_type::_ptr_type
  get_push_consumer_tr_dds_entity (void);

  /**
    * Circle connector
    **/
  virtual
  typename CCM_TYPE::supplier_cl_traits::data_type::_ptr_type
  get_supplier_cl_data (void);

  virtual
  typename CCM_TYPE::supplier_cl_traits::dds_entity_type::_ptr_type
  get_supplier_cl_dds_entity (void);

  virtual
  typename CCM_TYPE::pull_consumer_cl_traits::fresh_data_type::_ptr_type
  get_pull_consumer_cl_fresh_data (void);

  virtual
  typename CCM_TYPE::pull_consumer_cl_traits::data_type::_ptr_type
  get_pull_consumer_cl_data (void);

  virtual
  typename CCM_TYPE::pull_consumer_cl_traits::dds_entity_type::_ptr_type
  get_pull_consumer_cl_dds_entity (void);

  virtual
  CCM_DDS::QueryFilter*
  pull_consumer_cl_filter (void);

  virtual
  void
  pull_consumer_cl_filter (const CCM_DDS::QueryFilter&);

  virtual
  CCM_DDS::CCM_ContentFilterSetting*
  get_pull_consumer_cl_filter_config(void);

  virtual
  CCM_DDS::CCM_ContentFilterSetting*
  get_push_consumer_cl_filter_config(void);

  virtual
  CCM_DDS::QueryFilter *
  push_consumer_cl_filter (void);

  virtual
  void
  push_consumer_cl_filter (const ::CCM_DDS::QueryFilter &);

  virtual
  typename CCM_TYPE::push_consumer_cl_traits::data_type::_ptr_type
  get_push_consumer_cl_data (void);

  virtual
  typename CCM_TYPE::push_consumer_cl_traits::data_control_type::_ptr_type
  get_push_consumer_cl_data_control (void);

  virtual
  typename CCM_TYPE::push_consumer_cl_traits::dds_entity_type::_ptr_type
  get_push_consumer_cl_dds_entity (void);

  /**
    * Topic names.
    **/
  virtual char * topic_name_sq (void);
  virtual void topic_name_sq (const char * topic_name_sq);
  virtual char * topic_name_tr (void);
  virtual void topic_name_tr (const char * topic_name_tr);
  virtual char * topic_name_cl (void);
  virtual void topic_name_cl (const char * topic_name_cl);

  /**
    * Deployment methods.
    **/
  virtual void configuration_complete (void);
  virtual void ccm_remove (void);
  virtual void ccm_activate (void);
  virtual void ccm_passivate (void);

private:
  DDS_Write_T <
      typename CCM_TYPE::supplier_sq_traits,
      typename DDS_TYPE::typed_writer_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE>
    sq_supplier_;

  DDS_Write_T <
      typename CCM_TYPE::supplier_tr_traits,
      typename DDS_TYPE::typed_writer_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE>
    tr_supplier_;

  DDS_Write_T <
      typename CCM_TYPE::supplier_cl_traits,
      typename DDS_TYPE::typed_writer_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE>
    cl_supplier_;

  DDS_Get_T <
      typename CCM_TYPE::pull_consumer_sq_traits,
      typename DDS_TYPE::typed_reader_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE,
      FIXED>
    pull_consumer_sq_;

  DDS_Get_T <
      typename CCM_TYPE::pull_consumer_tr_traits,
      typename DDS_TYPE::typed_reader_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE,
      FIXED>
    pull_consumer_tr_;

  DDS_Listen_T<
      typename CCM_TYPE::push_consumer_cl_traits,
      typename DDS_TYPE::typed_reader_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE,
      CIAO::DDS4CCM::DDS4CCM_TAKE>
    push_consumer_cl_;

  /// Connection to the receiver implementation
  typename CCM_TYPE::push_consumer_cl_traits::data_listener_type::_var_type dl_;

  CORBA::String_var topic_name_sq_;
  CORBA::String_var topic_name_tr_;
  CORBA::String_var topic_name_cl_;

  ::DDS::Topic_var topic_sq_;
  ::DDS::Topic_var topic_tr_;
  ::DDS::Topic_var topic_cl_;

  ::DDS::TopicListener_var topiclistener_sq_;
  ::DDS::TopicListener_var topiclistener_tr_;
  ::DDS::TopicListener_var topiclistener_cl_;

  ACE_Reactor * reactor_;

  void create_topics (const char * typesupport_name);

  void activate_topics (void);
  void passivate_topics (void);
  void remove_topics (void);
};

#include "MultiTopic_Connector_T.cpp"

#endif
