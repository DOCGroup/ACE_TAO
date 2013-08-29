/**
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * DDS State Connector implementation
 */
#ifndef DDS_STATE_CONNECTOR_T_H_
#define DDS_STATE_CONNECTOR_T_H_

#include "dds4ccm/impl/DDS_TopicBase_Connector_T.h"
#include "dds4ccm/impl/DDS_Get_T.h"
#include "dds4ccm/impl/DDS_Listen_T.h"
#include "dds4ccm/impl/DDS_StateListen_T.h"
#include "dds4ccm/impl/DDS_Update_T.h"
#include "dds4ccm/impl/DDS_Read_T.h"

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
class DDS_State_Connector_T
  : public virtual DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>
{
public:
  DDS_State_Connector_T (void);
  virtual ~DDS_State_Connector_T (void);

  /**
   * @name DDS_Update
   * DDS_Update operations
   */
  //@{
  virtual typename CCM_TYPE::observable_traits::data_type::_ptr_type
  get_observable_data (void);

  virtual typename CCM_TYPE::observable_traits::dds_entity_type::_ptr_type
  get_observable_dds_entity (void);
  //@}

  /**
   * @name DDS_Read
   * DDS_Read operations
   */
  //@{
  virtual typename CCM_TYPE::passive_observer_traits::data_type::_ptr_type
  get_passive_observer_data (void);

  virtual typename CCM_TYPE::passive_observer_traits::dds_entity_type::_ptr_type
  get_passive_observer_dds_entity (void);

  virtual typename CCM_TYPE::passive_observer_traits::filter_config_type::_ptr_type
  get_passive_observer_filter_config (void);

  virtual ::CCM_DDS::QueryFilter *passive_observer_filter (void);

  virtual void passive_observer_filter (const ::CCM_DDS::QueryFilter & filter);
//@}

  /**
   * @name DDS_Get
   * DDS_Get operations
   */
  //@{
  virtual typename CCM_TYPE::pull_observer_traits::data_type::_ptr_type
  get_pull_observer_data (void);

  virtual typename CCM_TYPE::pull_observer_traits::fresh_data_type::_ptr_type
  get_pull_observer_fresh_data (void);

  virtual typename CCM_TYPE::pull_observer_traits::dds_entity_type::_ptr_type
  get_pull_observer_dds_entity (void);

  virtual typename CCM_TYPE::pull_observer_traits::filter_config_type::_ptr_type
  get_pull_observer_filter_config (void);

  virtual ::CCM_DDS::QueryFilter *pull_observer_filter (void);

  virtual void pull_observer_filter (const ::CCM_DDS::QueryFilter & filter);
  //@}

  /**
   * @name DDS_Listen
   * DDS_Listen operations
   */
  //@{
  virtual typename CCM_TYPE::push_observer_traits::data_type::_ptr_type
  get_push_observer_data (void);

  virtual typename CCM_TYPE::push_observer_traits::data_control_type::_ptr_type
  get_push_observer_data_control (void);

  virtual typename CCM_TYPE::push_observer_traits::dds_entity_type::_ptr_type
  get_push_observer_dds_entity (void);

  virtual typename CCM_TYPE::push_observer_traits::filter_config_type::_ptr_type
  get_push_observer_filter_config (void);

  virtual ::CCM_DDS::QueryFilter *push_observer_filter (void);

  virtual void push_observer_filter (const ::CCM_DDS::QueryFilter & filter);
  //@}

  /**
   * @name DDS_StateListen
   * DDS_StateListen operations
   */
  //@{
  virtual typename CCM_TYPE::push_state_observer_traits::data_type::_ptr_type
  get_push_state_observer_data (void);

  virtual typename CCM_TYPE::push_state_observer_traits::data_control_type::_ptr_type
  get_push_state_observer_data_control (void);

  virtual typename CCM_TYPE::push_state_observer_traits::dds_entity_type::_ptr_type
  get_push_state_observer_dds_entity (void);

  virtual typename CCM_TYPE::push_state_observer_traits::filter_config_type::_ptr_type
  get_push_state_observer_filter_config (void);

  virtual ::CCM_DDS::QueryFilter *push_state_observer_filter (void);

  virtual void
  push_state_observer_filter (const ::CCM_DDS::QueryFilter & filter);
  //@}

  void topic_name (const char * topic_name);
  virtual char *topic_name (void);

  virtual void configuration_complete (void);
  virtual void ccm_activate (void);
  virtual void ccm_passivate (void);
  virtual void ccm_remove (void);

private:
  bool observable_obtained_;
  bool passive_observer_obtained_;
  bool pull_observer_obtained_;
  bool push_observer_obtained_;
  bool push_state_observer_obtained_;

  /**
   * DDS_Update observable
   */
  //@{
  DDS_Update_T <
      typename CCM_TYPE::observable_traits,
      typename DDS_TYPE::typed_writer_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE>
    observable_;
  //@}

  /**
   * DDS_Read passive_observer
   */
  //@{
  DDS_Read_T <
      typename CCM_TYPE::passive_observer_traits,
      typename DDS_TYPE::typed_reader_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE>
    passive_observer_;
  //@}

  /**
   * DDS_Get pull_observer
   */
  //@{
  DDS_Get_T <
      typename CCM_TYPE::pull_observer_traits,
      typename DDS_TYPE::typed_reader_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE,
      FIXED>
    pull_observer_;
  //@}

  /**
   * DDS_Listen push_observer
   */
  //@{
  DDS_Listen_T <
      typename CCM_TYPE::push_observer_traits,
      typename DDS_TYPE::typed_reader_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE,
      CIAO::DDS4CCM::DDS4CCM_READ>
  push_observer_;
  //@}

  /**
   * DDS_StateListen push_state_observer
   */
  //@{
  DDS_StateListen_T <
      typename CCM_TYPE::push_state_observer_traits,
      typename DDS_TYPE::typed_reader_type,
      typename DDS_TYPE::value_type,
      SEQ_TYPE,
      CIAO::DDS4CCM::DDS4CCM_READ>
    push_state_observer_;
  //@}

  void do_configuration_complete (void);
  void do_ccm_activate (void);
  void do_ccm_remove (void);

  typedef DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE> TopicBaseConnector;
};

#include "dds4ccm/impl/DDS_State_Connector_T.cpp"

#endif /* DDS_STATE_CONNECTOR_T_H_ */
