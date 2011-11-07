/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * DDS Event Connector implementation
 */
#ifndef DDS_EVENT_CONNECTOR_T_H_
#define DDS_EVENT_CONNECTOR_T_H_

#include "dds4ccm/impl/DDS_TopicBase_Connector_T.h"
#include "dds4ccm/impl/DDS_Get_T.h"
#include "dds4ccm/impl/DDS_Write_T.h"
#include "dds4ccm/impl/DDS_Listen_T.h"

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
class DDS_Event_Connector_T
  : public virtual DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>
{
public:
  DDS_Event_Connector_T (void);
  virtual ~DDS_Event_Connector_T (void);

  /**
   * @name DDS_Write
   * DDS_Write operations
   */
  //@{
  virtual typename CCM_TYPE::supplier_traits::data_type::_ptr_type get_supplier_data (void);

  virtual typename CCM_TYPE::supplier_traits::dds_entity_type::_ptr_type get_supplier_dds_entity (void);
  //@}

  /**
   * @name DDS_Get
   * DDS_Get operations
   */
  //@{
  virtual typename CCM_TYPE::pull_consumer_traits::data_type::_ptr_type
  get_pull_consumer_data (void);

  virtual typename CCM_TYPE::pull_consumer_traits::fresh_data_type::_ptr_type
  get_pull_consumer_fresh_data (void);

  virtual ::DDS::CCM_DataReader_ptr get_pull_consumer_dds_entity (void);

  virtual ::CCM_DDS::CCM_ContentFilterSetting_ptr
  get_pull_consumer_filter_config (void);

  virtual ::CCM_DDS::QueryFilter *pull_consumer_filter (void);

  virtual void pull_consumer_filter (const ::CCM_DDS::QueryFilter & filter);
  //@}

  /**
   * @name DDS_Listen
   * DDS_Listen operations
   */
  //@{
  virtual typename CCM_TYPE::push_consumer_traits::data_type::_ptr_type get_push_consumer_data (void);

  virtual ::CCM_DDS::CCM_DataListenerControl_ptr get_push_consumer_data_control (void);

  virtual ::DDS::CCM_DataReader_ptr get_push_consumer_dds_entity (void);

  virtual ::CCM_DDS::CCM_ContentFilterSetting_ptr
  get_push_consumer_filter_config (void);

  virtual ::CCM_DDS::QueryFilter *push_consumer_filter (void);

  virtual void push_consumer_filter (const ::CCM_DDS::QueryFilter & filter);
  //@}

  virtual void topic_name (const char * topic_name);
  virtual char *topic_name (void);

  virtual void configuration_complete (void);
  virtual void ccm_activate (void);
  virtual void ccm_passivate (void);
  virtual void ccm_remove (void);

private:
  bool supplier_obtained_;
  bool push_consumer_obtained_;
  bool pull_consumer_obtained_;

  /**
   * DDS_Write supplier
   */
  //@{
  DDS_Write_T <
      typename CCM_TYPE::supplier_traits,
      typename DDS_TYPE::typed_writer_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE>
    supplier_;
  //@}

  /**
   * DDS_Listen push_consumer
   */
  //@{
  DDS_Listen_T <
      typename CCM_TYPE::push_consumer_traits,
      typename DDS_TYPE::typed_reader_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE>
    push_consumer_;
  //@}

  /**
   * DDS_Get pull_consumer
   */
  //@{
  DDS_Get_T <
      typename CCM_TYPE::pull_consumer_traits,
      typename DDS_TYPE::typed_reader_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE,
      FIXED>
    pull_consumer_;
  //@}

  void do_configuration_complete (void);
  void do_ccm_activate (void);
  void do_ccm_remove (void);

  typedef DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE> TopicBaseConnector;
};

#include "dds4ccm/impl/DDS_Event_Connector_T.cpp"

#endif /* DDS_EVENT_CONNECTOR_T_H_ */
