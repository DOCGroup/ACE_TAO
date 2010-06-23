/**
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_UPDATE_T_H
#define DDS_UPDATE_T_H

#include "dds4ccm/impl/dds/Updater_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
class DDS_Update_T
{
public:
  DDS_Update_T (void);
  ~DDS_Update_T (void);

  typename CCM_TYPE::updater_type::_ptr_type get_data (void);

  ::DDS::CCM_DataWriter_ptr  get_dds_entity (void);

  void configuration_complete (
    typename CCM_TYPE::base_type::_ptr_type component, 
    ::DDS::Topic_ptr topic,
    ::DDS::Publisher_ptr publisher,
    const char* library_name,
    const char* profile_name);

  void activate (void);

  void passivate (void);

  void remove (::DDS::Publisher_ptr publisher);

private:
  /**
   * DDS_Update
   */
  //@{
  ::DDS::DataWriterListener_var data_listener_;
  ::CIAO::DDS4CCM::Updater_T<DDS_TYPE, CCM_TYPE> dds_update_;
  ::CIAO::DDS4CCM::CCM_DDS_DataWriter_T<DDS_TYPE, CCM_TYPE> ccm_dds_writer_;
  //@}
  typedef ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE> DataWriterListener;
};

#include "dds4ccm/impl/dds/DDS_Update_T.cpp"

#endif /* DDS_UPDATE_T_H */
