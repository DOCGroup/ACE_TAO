/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_WRITE_T_H_
#define DDS_WRITE_T_H_

#include "dds4ccm/impl/dds/DDS_TopicBase_Connector_T.h"
#include "dds4ccm/impl/dds/Writer_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
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

  void configuration_complete (
    ::DDS::Topic_ptr topic,
    ::DDS::Publisher_ptr publisher,
    const char* library_name,
    const char* profile_name);

  void activate (void);

  void passivate (void);

  void remove (
    ::DDS::Publisher_ptr publisher);

private:
  /**
   * DDS_Write
   */
  //@{
  ::DDS::DataWriterListener_var data_listener_;
  ::CIAO::DDS4CCM::Writer_T<DDS_TYPE, CCM_TYPE> writer_t_;
  ::CIAO::DDS4CCM::CCM_DDS_DataWriter_i ccm_dds_writer_i;
  //@}

  typedef ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE> DataWriterListener;
};

#include "dds4ccm/impl/dds/DDS_Write_T.cpp"

#endif /* DDS_WRITE_T_H_ */
