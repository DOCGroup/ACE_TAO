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
#include "dds4ccm/impl/CCM_DataWriter.h"

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

  typename CCM_TYPE::datawriter_type::_ptr_type get_dds_entity (void);
  //@}

  void set_component (::CORBA::Object_ptr component);

  void configuration_complete (
    ::DDS::Topic_ptr topic,
    ::DDS::Publisher_ptr publisher,
    const char* library_name,
    const char* profile_name);

  void activate (void);

  void passivate (void);

  void remove (::DDS::Publisher_ptr publisher);

private:
  typedef ::CIAO::DDS4CCM::Writer_T<
    typename CCM_TYPE::writer_type,
    typename DDS_TYPE::typed_writer_type,
    typename CCM_TYPE::value_type,
    typename CCM_TYPE::seq_type>
      Writer_type;
  typedef ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>
    DataWriterListener_type;

  /**
   * DDS_Write
   */
  //@{
  ::DDS::DataWriterListener_var data_listener_;
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T < Writer_type > dds_write_;
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T < ::CIAO::DDS4CCM::CCM_DataWriter > ccm_data_writer_;
  //@}

};

#include "dds4ccm/impl/DDS_Write_T.cpp"

#endif /* DDS_WRITE_T_H_ */
