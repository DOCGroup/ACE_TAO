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
#include "dds4ccm/impl/Writer_T.h"
#include "dds4ccm/impl/CCM_DataWriter.h"

template <typename CCM_TYPE, typename TYPED_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
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
  typename CCM_TYPE::data_type::_ptr_type get_data (void);

  typename CCM_TYPE::dds_entity_type::_ptr_type get_dds_entity (void);
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
    typename CCM_TYPE::data_type,
    TYPED_WRITER,
    VALUE_TYPE,
    SEQ_VALUE_TYPE>
      Writer_type;

  /**
   * DDS_Write
   */
  //@{
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T < Writer_type > dds_write_;
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T < ::CIAO::DDS4CCM::CCM_DataWriter > ccm_data_writer_;
  //@}

};

#include "dds4ccm/impl/DDS_Write_T.cpp"

#endif /* DDS_WRITE_T_H_ */
