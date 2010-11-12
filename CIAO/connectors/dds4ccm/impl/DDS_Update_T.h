/**
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_UPDATE_T_H_
#define DDS_UPDATE_T_H_

#include "dds4ccm/impl/Updater_T.h"
#include "dds4ccm/impl/CCM_DataWriter.h"

template <typename DDS_TYPE, typename CCM_TYPE>
class DDS_Update_T
{
public:
  DDS_Update_T (void);
  ~DDS_Update_T (void);

  typename CCM_TYPE::updater_type::_ptr_type get_data (void);

  typename CCM_TYPE::datawriter_type::_ptr_type get_dds_entity (void);

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
  typedef ::CIAO::DDS4CCM::Updater_T<
    typename CCM_TYPE::updater_type,
    typename DDS_TYPE::typed_writer_type,
    typename CCM_TYPE::value_type,
    typename CCM_TYPE::seq_type>
      Updater_type;

    /**
   * DDS_Update
   */
  //@{
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T< Updater_type > dds_update_;
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T< ::CIAO::DDS4CCM::CCM_DataWriter > ccm_data_writer_;
  //@}
};

#include "dds4ccm/impl/DDS_Update_T.cpp"

#endif /* DDS_UPDATE_T_H_ */
