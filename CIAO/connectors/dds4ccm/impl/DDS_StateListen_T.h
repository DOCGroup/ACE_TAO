/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_STATELISTEN_T_H_
#define DDS_STATELISTEN_T_H_

#include "dds4ccm/impl/DDS_Subscriber_Base_T.h"
#include "dds4ccm/impl/StateListenerControl_T.h"
#include "dds4ccm/impl/ConditionManager_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
class DDS_StateListen_T
  : public DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>
{
typedef CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
  ConditionManager_type;
typedef ::CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
  DataReaderStateListener_type;
typedef CCM_DDS_StateListenerControl_T< ::CCM_DDS::CCM_StateListenerControl, CCM_TYPE>
  StateListenerControl_type;
typedef DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>
  DDSSubscriberBase_type;

public:
  DDS_StateListen_T (void);
  ~DDS_StateListen_T (void);

  /**
   * @name DDS_Listen
   * DDS_Listen operations
   */
  //@{
  ::CCM_DDS::CCM_StateListenerControl_ptr get_data_control (void);
  //@}

  bool configuration_complete (
    typename CCM_TYPE::base_type::_ptr_type component,
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* library_name,
    const char* profile_name);

  void activate (
    typename CCM_TYPE::statelistener_type::_ptr_type listener,
    ::CCM_DDS::PortStatusListener_ptr status,
    ACE_Reactor* reactor);

  void remove (::DDS::Subscriber_ptr subscriber);

private:
  /**
   * DDS_StateListen
   */
  //@{
  StateListenerControl_type data_control_;
  //@}
};

#include "dds4ccm/impl/DDS_StateListen_T.cpp"

#endif /* DDS_STATELISTEN_T_H_ */
