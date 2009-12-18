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
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "dds4ccm/impl/ndds/Getter_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
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

  void init (
    ::CCM_DDS::PortStatusListener_ptr listener,
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* library_name,
    const char* profile_name);

private:
  /**
   * DDS_Get
   */
  //@{
  ::DDS::CCM_DataReader_var data_;
  ::DDS::DataReaderListener_var status_;
  ::CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE> dds_get_;
  ::CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> dds_read_;
  ::CIAO::DDS4CCM::RTI::RTI_DataReader_i rti_reader_;
  //@}
};

#include "dds4ccm/impl/ndds/DDS_Get_T.cpp"

#endif /* DDS_GET_T_H_ */
