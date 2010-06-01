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
#include "dds4ccm/impl/dds/Reader_T.h"
#include "dds4ccm/impl/dds/DataReader_T.h"
#include "dds4ccm/impl/dds/PortStatusListener_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
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
  typename CCM_TYPE::reader_type::_ptr_type get_data (void);

  ::DDS::CCM_DataReader_ptr get_dds_entity (void);

  ::CCM_DDS::CCM_ContentFilterSetting_ptr get_filter_config (void);

  ::CCM_DDS::QueryFilter *filter (void);

  void filter (const ::CCM_DDS::QueryFilter & filter);
  //@}

  bool configuration_complete (
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* library_name,
    const char* profile_name);

  void activate (
    ::CCM_DDS::PortStatusListener_ptr listener,
    ACE_Reactor* reactor);

  void passivate (void);

  void remove (::DDS::Subscriber_ptr subscriber);

protected:
  /**
   * DDS_Subscriber_Base_T
   */
  //@{
  ::DDS::DataReaderListener_var listener_;
  ::CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED> dds_read_;
  ::CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE> data_reader_;
  ::CCM_DDS::QueryFilter filter_;
  //@}
  typedef ::CIAO::DDS4CCM::PortStatusListener_T <DDS_TYPE, CCM_TYPE> PortStatusListener;
};

#include "dds4ccm/impl/dds/DDS_Subscriber_Base_T.cpp"

#endif /* DDS_READ_T_H_ */
