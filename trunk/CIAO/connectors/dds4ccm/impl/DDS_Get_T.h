/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_GET_T_H_
#define DDS_GET_T_H_

#include "dds4ccm/impl/DDS_Subscriber_Base_T.h"
#include "dds4ccm/impl/Getter_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
class DDS_Get_T
  : public DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>
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

  void set_component (
    typename CCM_TYPE::base_type::_ptr_type component);

  void configuration_complete (
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
    const char* library_name,
    const char* profile_name);

  void remove (
    ::DDS::Subscriber_ptr subscriber);

private:
  typedef CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
    ConditionManager_type;
  typedef DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>
    DDSSubscriberBase_type;
  typedef ::CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>
    Getter_type;

  /**
   * DDS_Get
   */
  //@{
  ::DDS::DataReaderListener_var listener_;
  ::CIAO::DDS4CCM::LocalObject_Auto_Ptr_T<Getter_type> dds_get_;
  //@}
};

#include "dds4ccm/impl/DDS_Get_T.cpp"

#endif /* DDS_GET_T_H_ */
