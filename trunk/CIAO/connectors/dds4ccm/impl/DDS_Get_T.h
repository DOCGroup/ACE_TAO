/**
 * @author Marcel Smit (msmit@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_GET_T_H_
#define DDS_GET_T_H_

#include "dds4ccm/impl/DDS_Subscriber_Base_T.h"
#include "dds4ccm/impl/Getter_T.h"

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE,
          typename SEQ_VALUE_TYPE, bool FIXED>
class DDS_Get_T
  : public DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>
{
public:
  DDS_Get_T (void);
  ~DDS_Get_T (void);

  /**
   * @name DDS_Get
   * DDS_Get operations
   */
  //@{
  typename CCM_TYPE::fresh_data_type::_ptr_type get_fresh_data (void);
  //@}

  void set_component (::CORBA::Object_ptr component);

  void configuration_complete (
    ::DDS::Topic_ptr topic,
    ::DDS::Subscriber_ptr subscriber,
#if (CIAO_DDS4CCM_NDDS==1)
    const char * qos_profile);
#else
    const char * qos_profile,
    OpenDDS::DCPS::QOS_XML_Loader& qos_xml);
#endif

  void remove (::DDS::Subscriber_ptr subscriber);

private:
  typedef DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>
    SubscriberBase_type;

  typedef ::CIAO::DDS4CCM::Getter_T<
      typename CCM_TYPE::fresh_data_type,
      TYPED_DDS_READER ,
      VALUE_TYPE,
      SEQ_VALUE_TYPE,
      FIXED>
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
