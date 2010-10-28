/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_WRITE_T_H_
#define DDS_WRITE_T_H_

#include "dds4ccm/impl/DDS_TopicBase_Connector_T.h"
#include "dds4ccm/impl/DataWriterListener_T.h"
#include "dds4ccm/impl/Writer_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
class DDS_Write_T
{
public:
  DDS_Write_T (void);
  ~DDS_Write_T (void);

  /**
   * @name DDS_Write
   * DDS_Write operations
   */
  //@{
  typename CCM_TYPE::writer_type::_ptr_type get_data (void);

  ::DDS::CCM_DataWriter_ptr get_dds_entity (void);
  //@}

  void set_component (
    typename CCM_TYPE::base_type::_ptr_type component);

  void configuration_complete (
    ::DDS::Topic_ptr topic,
    ::DDS::Publisher_ptr publisher,
    const char* library_name,
    const char* profile_name);

  void activate (void);

  void passivate (void);

  void remove (::DDS::Publisher_ptr publisher);

private:
  typedef ::CIAO::DDS4CCM::Writer_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> Writer_type;
  typedef ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
    DataWriterListener_type;
  typedef ::CIAO::DDS4CCM::CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
    DataWriter_type;

  /**
   * DDS_Write
   */
  //@{
  ::DDS::DataWriterListener_var data_listener_;
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T < Writer_type > writer_t_;
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T < DataWriter_type > ccm_dds_writer_;
  //@}

};

#include "dds4ccm/impl/DDS_Write_T.cpp"

#endif /* DDS_WRITE_T_H_ */
