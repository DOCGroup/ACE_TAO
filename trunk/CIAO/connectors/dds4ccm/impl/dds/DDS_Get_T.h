/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_GET_T_H_
#define DDS_GET_T_H_

#include "dds4ccm/impl/dds/DDS_Subscriber_Base_T.h"
#include "dds4ccm/impl/dds/Getter_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class DDS_Get_T
  : public DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>
{
public:
  DDS_Get_T (void);
  ~DDS_Get_T (void);

  /**
   * @name DDS_Get
   * DDS_Get operations
   */
  //@{
  typename CCM_TYPE::getter_type::_ptr_type get_fresh_data (void);
  //@}

  bool configuration_complete (
    typename CCM_TYPE::base_type::_ptr_type component,
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* library_name,
    const char* profile_name);

  void remove (
    ::DDS::Subscriber_ptr subscriber);

private:
  /**
   * DDS_Get
   */
  //@{
  ::DDS::DataReaderListener_var listener_;
  ::CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, FIXED> dds_get_;
  //@}
  typedef DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED> DDSSubscriberBase;
};

#include "dds4ccm/impl/dds/DDS_Get_T.cpp"

#endif /* DDS_GET_T_H_ */
