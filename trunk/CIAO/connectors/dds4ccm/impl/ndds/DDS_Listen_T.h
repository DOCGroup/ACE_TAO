/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_LISTEN_T_H_
#define DDS_LISTEN_T_H_

template <typename DDS_TYPE, typename CCM_TYPE>
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

  void init (
    typename CCM_TYPE::listener_type::_ptr_type listener,
    ::CCM_DDS::PortStatusListener_ptr status,
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* library_name,
    const char* profile_name);

private:
  /**
   * DDS_Listen
   */
  //@{
  ::DDS::CCM_DataReader_var data_reader_;
  ::CCM_DDS::CCM_DataListenerControl_var data_control_;
  ::DDS::DataReaderListener_var data_listener_;
  ::CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> dds_read_;
  ::CIAO::DDS4CCM::RTI::RTI_DataReader_i rti_reader_;
  //@}
};

#include "dds4ccm/impl/ndds/DDS_Listen_T.cpp"

#endif /* DDS_LISTEN_T_H_ */
