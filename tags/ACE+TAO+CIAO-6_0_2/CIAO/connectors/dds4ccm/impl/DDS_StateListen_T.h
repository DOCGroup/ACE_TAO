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

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
class DDS_StateListen_T
  : public DDS_Subscriber_Base_T<CCM_TYPE,
                                 TYPED_DDS_READER,
                                 VALUE_TYPE,
                                 SEQ_VALUE_TYPE>
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

  void set_component (::CORBA::Object_ptr component);

  void activate (
    typename CCM_TYPE::data_listener_type::_ptr_type listener,
    ::CCM_DDS::PortStatusListener_ptr status,
    ACE_Reactor* reactor);

  void remove (::DDS::Subscriber_ptr subscriber);

private:
  typedef ::CIAO::DDS4CCM::DataReaderStateListener_T<CCM_TYPE, TYPED_DDS_READER, SEQ_VALUE_TYPE>
    DataReaderStateListener_type;
  typedef CCM_DDS_StateListenerControl_T< ::CCM_DDS::CCM_StateListenerControl>
    StateListenerControl_type;
  typedef DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>
    SubscriberBase_type;

  /**
   * DDS_StateListen
   */
  //@{
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T<StateListenerControl_type> data_control_;
  //@}
};

#include "dds4ccm/impl/DDS_StateListen_T.cpp"

#endif /* DDS_STATELISTEN_T_H_ */
