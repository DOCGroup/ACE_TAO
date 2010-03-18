/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_GET_T_H_
#define DDS_GET_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds/Reader_T.h"
#include "dds4ccm/impl/dds/Getter_T.h"
#include "dds4ccm/impl/dds/PortStatusListener_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class DDS_Get_T
{
public:
  DDS_Get_T (void);
  ~DDS_Get_T (void);

  /**
   * @name DDS_Get
   * DDS_Get operations
   */
  //@{
  typename CCM_TYPE::reader_type::_ptr_type get_data (void);

  typename CCM_TYPE::getter_type::_ptr_type get_fresh_data (void);

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

  void remove (::DDS::Subscriber_ptr subscriber);

private:
  /**
   * DDS_Get
   */
  //@{
  ::DDS::DataReaderListener_var status_;
  ::CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, FIXED> dds_get_;
  ::CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED> dds_read_;
  ::CIAO::DDS4CCM::CCM_DDS_DataReader_i ccm_dds_reader_;
  //@}

  typedef ::CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE> PortStatusListener;
};

#include "dds4ccm/impl/dds/DDS_Get_T.cpp"

#endif /* DDS_GET_T_H_ */
