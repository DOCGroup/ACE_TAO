/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_STATELISTEN_T_H_
#define DDS_STATELISTEN_T_H_

template <typename DDS_TYPE, typename CCM_TYPE>
class DDS_StateListen_T
{
public:
  DDS_StateListen_T (void);
  ~DDS_StateListen_T (void);

  /**
   * @name DDS_Listen
   * DDS_Listen operations
   */
  //@{
  typename CCM_TYPE::reader_type::_ptr_type get_data (void);

  ::CCM_DDS::CCM_StateListenerControl_ptr get_data_control (void);

  ::DDS::CCM_DataReader_ptr get_dds_entity (void);
  //@}

  void configuration_complete (
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* library_name,
    const char* profile_name);

  void activate (
    typename CCM_TYPE::statelistener_type::_ptr_type listener,
    ::CCM_DDS::PortStatusListener_ptr status,
    ACE_Reactor* reactor);

  void passivate (void);

  void remove (
    ::DDS::Subscriber_ptr subscriber);


private:
  /**
   * DDS_Listen
   */
  //@{
  ::DDS::CCM_DataReader_var data_reader_;
  ::CCM_DDS::CCM_StateListenerControl_var data_control_;
  ::DDS::DataReaderListener_var data_listener_;
  ::CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> dds_read_;
  ::CIAO::DDS4CCM::CCM_DDS_DataReader_i rti_reader_;
  //@}
  typedef ::CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE> DataReaderStateListener;
};

#include "dds4ccm/impl/ndds/DDS_StateListen_T.cpp"

#endif /* DDS_STATELISTEN_T_H_ */
