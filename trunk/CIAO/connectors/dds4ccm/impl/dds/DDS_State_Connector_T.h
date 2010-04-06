/**
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_STATE_CONNECTOR_T_H_
#define DDS_STATE_CONNECTOR_T_H_

#include "dds4ccm/impl/dds/DDS_TopicBase_Connector_T.h"
#include "dds4ccm/impl/dds/DDS_Get_T.h"
#include "dds4ccm/impl/dds/DDS_Listen_T.h"
#include "dds4ccm/impl/dds/DDS_StateListen_T.h"
#include "dds4ccm/impl/dds/DDS_Update_T.h"
#include "dds4ccm/impl/dds/DDS_Read_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class DDS_State_Connector_T
  : public virtual DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>
{
public:
  DDS_State_Connector_T (void);
  virtual ~DDS_State_Connector_T (void);

  virtual typename CCM_TYPE::updater_type::_ptr_type
  get_observable_data (void);

  virtual ::DDS::CCM_DataWriter_ptr
  get_observable_dds_entity (void);

  virtual typename CCM_TYPE::reader_type::_ptr_type
  get_passive_observer_data (void);

  virtual ::DDS::CCM_DataReader_ptr
  get_passive_observer_dds_entity (void);

  virtual typename CCM_TYPE::reader_type::_ptr_type
  get_pull_observer_data (void);

  virtual typename CCM_TYPE::getter_type::_ptr_type
  get_pull_observer_fresh_data (void);

  virtual ::DDS::CCM_DataReader_ptr
  get_pull_observer_dds_entity (void);

  virtual typename CCM_TYPE::reader_type::_ptr_type
  get_push_observer_data (void);

  virtual ::CCM_DDS::CCM_DataListenerControl_ptr
  get_push_observer_data_control (void);

  virtual ::DDS::CCM_DataReader_ptr
  get_push_observer_dds_entity (void);

  virtual typename CCM_TYPE::reader_type::_ptr_type
  get_push_state_observer_data (void);

  virtual ::CCM_DDS::CCM_StateListenerControl_ptr
  get_push_state_observer_data_control (void);

  virtual ::DDS::CCM_DataReader_ptr
  get_push_state_observer_dds_entity (void);

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
  DDS_Update_T <DDS_TYPE, CCM_TYPE> observable_;
  //@}

  /**
   * DDS_Read passive_observer
   */
  //@{
  DDS_Read_T <DDS_TYPE, CCM_TYPE, FIXED> passive_observer_;
  //@}

  /**
   * DDS_Get pull_observer
   */
  //@{
  DDS_Get_T <DDS_TYPE, CCM_TYPE, FIXED> pull_observer_;
  //@}

  /**
   * DDS_Listen push_observer
   */
  //@{
  DDS_Listen_T <DDS_TYPE, CCM_TYPE, FIXED> push_observer_;
  //@}

  /**
   * DDS_StateListen push_state_observer
   */
  //@{
  DDS_StateListen_T <DDS_TYPE, CCM_TYPE, FIXED> push_state_observer_;
  //@}

typedef DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE> TopicBaseConnector;
};

#include "dds4ccm/impl/dds/DDS_State_Connector_T.cpp"

#endif /* DDS_STATE_CONNECTOR_T_H_ */
