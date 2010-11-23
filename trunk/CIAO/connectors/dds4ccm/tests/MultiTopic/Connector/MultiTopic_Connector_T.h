// $Id$

#ifndef DDS_MT_CONNECTOR_T_H_
#define DDS_MT_CONNECTOR_T_H_

#include "connectors/dds4ccm/impl/DDS_TopicBase_Connector_T.h"
#include "connectors/dds4ccm/impl/DDS_Write_T.h"
#include "connectors/dds4ccm/impl/DDS_Get_T.h"
#include "connectors/dds4ccm/impl/DDS_Listen_T.h"

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
class DDS_MT_Event_Connector_T
  : public virtual DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>
{

typedef DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE> TopicBaseConnector;

public:
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
    * Deployment methods.
    **/
  virtual void configuration_complete (void);
  virtual void ccm_remove (void);
  virtual void ccm_activate (ACE_Reactor* reactor);
  virtual void ccm_passivate (void);

private:
  DDS_Write_T <typename CCM_TYPE::supplier_sq_traits, DDS_TYPE> sq_supplier_;
  DDS_Write_T <typename CCM_TYPE::supplier_sq_traits, DDS_TYPE> tr_supplier_;
  DDS_Write_T <typename CCM_TYPE::supplier_sq_traits, DDS_TYPE> cl_supplier_;

  DDS_Get_T <typename CCM_TYPE::pull_consumer_sq_traits, DDS_TYPE, FIXED> pull_consumer_sq_;
  DDS_Get_T <typename CCM_TYPE::pull_consumer_tr_traits, DDS_TYPE, FIXED> pull_consumer_tr_;
  DDS_Get_T <typename CCM_TYPE::pull_consumer_cl_traits, DDS_TYPE, FIXED> pull_consumer_cl_;

  DDS_Listen_T <typename CCM_TYPE::push_consumer_sq_traits, DDS_TYPE> push_consumer_sq_;
  DDS_Listen_T <typename CCM_TYPE::push_consumer_tr_traits, DDS_TYPE> push_consumer_tr_;
  DDS_Listen_T <typename CCM_TYPE::push_consumer_cl_traits, DDS_TYPE> push_consumer_cl_;
};

#include "/home/marcel/Develop/ACE/latest/ACE_wrappers/TAO/CIAO/connectors/dds4ccm/tests/MultiTopic/Connector/MultiTopic_Connector_T.cpp"

#endif
