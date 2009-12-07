/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_WRITE_T_H_
#define DDS_WRITE_T_H_

#include "dds4ccm/impl/ndds/DDS_TopicBase_Connector_T.h"
#include "dds4ccm/impl/ndds/DDS_Get_T.h"

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

  void init (
    ::DDS::Topic_ptr topic,
    ::DDS::Publisher_ptr publisher,
    const char* library_name,
    const char* profile_name);

private:
  /**
   * DDS_Write
   */
  //@{
  ::DDS::CCM_DataWriter_var writer_;
  ::DDS::DataWriterListener_var writer_listener_;
  //@}
};

#include "dds4ccm/impl/ndds/DDS_Write_T.cpp"

#endif /* DDS_WRITE_T_H_ */
