/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_READ_T_H_
#define DDS_READ_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class DDS_Read_T
{
public:
  DDS_Read_T (void);
  ~DDS_Read_T (void);

  /**
   * @name DDS_Read
   * DDS_Read operations
   */
  //@{
  typename CCM_TYPE::reader_type::_ptr_type get_data (void);

  ::DDS::CCM_DataReader_ptr get_dds_entity (void);
  //@}

  void configuration_complete (
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* library_name,
    const char* profile_name);

  void activate (
    ::CCM_DDS::PortStatusListener_ptr listener,
    ACE_Reactor* reactor);

  void passivate (void);

  void remove (
    ::DDS::Subscriber_ptr subscriber);

private:
  /**
   * DDS_Read
   */
  //@{
  ::DDS::DataReaderListener_var status_;
  ::CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED> dds_read_;
  ::CIAO::DDS4CCM::CCM_DDS_DataReader_i ccm_dds_reader_;
  //@}
  typedef ::CIAO::DDS4CCM::PortStatusListener_T <DDS_TYPE, CCM_TYPE> PortStatusListener;
};

#include "dds4ccm/impl/dds/DDS_Read_T.cpp"

#endif /* DDS_READ_T_H_ */
