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

template <typename DDS_TYPE, typename CCM_TYPE>
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

  void init (
    ::CCM_DDS::PortStatusListener_ptr listener,
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* library_name,
    const char* profile_name);

private:
  /**
   * DDS_Read
   */
  //@{
  ::DDS::DataReader_var data_;
  ::DDS::DataReaderListener_var status_;
  //@}
};

#include "dds4ccm/impl/ndds/DDS_Read_T.cpp"

#endif /* DDS_READ_T_H_ */
