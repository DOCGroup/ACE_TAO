/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Base class for all ports, containing a reader
 */
#ifndef DDS_SUBSCRIBER_BASE_T_H_
#define DDS_SUBSCRIBER_BASE_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/impl/Reader_T.h"
#include "dds4ccm/impl/PortStatusListener.h"
#include "dds4ccm/impl/ContentFilterSetting.h"
#include "dds4ccm/impl/ConditionManager.h"
#include "dds4ccm/impl/CCM_DataReader.h"
#include "dds4ccm/impl/LocalObject_T.h"

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
class DDS_Subscriber_Base_T
{
public:
  DDS_Subscriber_Base_T (void);
  ~DDS_Subscriber_Base_T (void);

  /**
   * @name DDS_Read
   * DDS_Subscriber_Base_T operations
   */
  //@{
  typename CCM_TYPE::data_type::_ptr_type get_data (void);

  typename CCM_TYPE::dds_entity_type::_ptr_type get_dds_entity (void);

  typename CCM_TYPE::filter_config_type::_ptr_type get_filter_config (void);

  ::CCM_DDS::QueryFilter *filter (void);

  void filter (const ::CCM_DDS::QueryFilter & filter);
  //@}

  void set_component (::CORBA::Object_ptr component);

  void configuration_complete (
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* qos_profile);

  void activate (
    ::CCM_DDS::PortStatusListener_ptr listener,
    ACE_Reactor* reactor);

  void passivate (void);

  void remove (::DDS::Subscriber_ptr subscriber);

protected:
  typedef ::CIAO::DDS4CCM::Reader_T<
      typename CCM_TYPE::data_type,
      TYPED_DDS_READER,
      VALUE_TYPE,
      SEQ_VALUE_TYPE>
    Reader_type;
  typedef ::CIAO::DDS4CCM::CCM_DDS_ContentFilterSetting_i
    ContentFilterSetting_type;

  /**
   * DDS_Subscriber_Base_T
   */
  //@{
  ::DDS::DataReaderListener_var listener_;
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T<ContentFilterSetting_type> cft_setting_;
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T<Reader_type> dds_read_;
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T< ::CIAO::DDS4CCM::CCM_DataReader > ccm_data_reader_;

  bool configuration_complete_;
  //@}
  ::CIAO::DDS4CCM::ConditionManager condition_manager_;
};

#include "dds4ccm/impl/DDS_Subscriber_Base_T.cpp"

#endif /* DDS_SUBSCRIBER_BASE_T_H_ */
