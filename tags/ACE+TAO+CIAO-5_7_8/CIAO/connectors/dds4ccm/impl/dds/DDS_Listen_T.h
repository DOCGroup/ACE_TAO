/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_LISTEN_T_H_
#define DDS_LISTEN_T_H_

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class DDS_Listen_T
{
public:
  DDS_Listen_T (void);
  ~DDS_Listen_T (void);

  /**
   * @name DDS_Listen
   * DDS_Listen operations
   */
  //@{
  typename CCM_TYPE::reader_type::_ptr_type get_data (void);

  ::CCM_DDS::CCM_DataListenerControl_ptr get_data_control (void);

  ::DDS::CCM_DataReader_ptr get_dds_entity (void);
  //@}

  void configuration_complete (
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* library_name,
    const char* profile_name);

  void activate (
    typename CCM_TYPE::listener_type::_ptr_type listener,
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
  ::CCM_DDS::CCM_DataListenerControl_var data_control_;
  ::DDS::DataReaderListener_var data_listener_;
  ::CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED> dds_read_;
  ::CIAO::DDS4CCM::CCM_DDS_DataReader_i ccm_dds_reader_;
  //@}
  typedef ::CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE> DataReaderListener;
};

#include "dds4ccm/impl/dds/DDS_Listen_T.cpp"

#endif /* DDS_LISTEN_T_H_ */
