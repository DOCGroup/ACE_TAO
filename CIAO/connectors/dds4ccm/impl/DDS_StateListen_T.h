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

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class DDS_StateListen_T
  : public DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>
{
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

  void activate (
    typename CCM_TYPE::statelistener_type::_ptr_type listener,
    ::CCM_DDS::PortStatusListener_ptr status,
    ACE_Reactor* reactor);

private:
  /**
   * DDS_Listen
   */
  //@{
  ::CCM_DDS::CCM_StateListenerControl_var data_control_;
  //@}
  typedef ::CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE> DataReaderStateListener;
};

#include "dds4ccm/impl/DDS_StateListen_T.cpp"

#endif /* DDS_STATELISTEN_T_H_ */
