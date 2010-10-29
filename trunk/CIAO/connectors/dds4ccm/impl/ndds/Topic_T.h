/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef NDDS_TOPIC_H
#define NDDS_TOPIC_H

#include "dds4ccm/impl/ndds/DomainParticipant_T.h"
#include "dds4ccm/impl/ndds/DDSTopicListener_T.h"

#include "tao/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_Topic_T :
      public virtual ::DDS::Topic,
      public virtual ::CORBA::LocalObject
    {
    typedef DDS_TopicListener_T<DDS_TYPE, VENDOR_TYPE> TopicListener_type;
    typedef DDS_DomainParticipant_T<DDS_TYPE, VENDOR_TYPE> DomainParticipant_type;

    public:
      /// Constructor
      DDS_Topic_T (DDSTopic* topic);

      /// Destructor
      virtual ~DDS_Topic_T (void);

      virtual ::DDS::ReturnCode_t set_qos (const ::DDS::TopicQos & qos);

      virtual ::DDS::ReturnCode_t get_qos (::DDS::TopicQos & qos);

      virtual ::DDS::ReturnCode_t set_listener (
        ::DDS::TopicListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::TopicListener_ptr get_listener (void);

      virtual ::DDS::ReturnCode_t get_inconsistent_topic_status (
        ::DDS::InconsistentTopicStatus & a_status);

      virtual ::DDS::ReturnCode_t enable (void);

      virtual ::DDS::StatusCondition_ptr get_statuscondition (void);

      virtual ::DDS::StatusMask get_status_changes (void);

      virtual ::DDS::InstanceHandle_t get_instance_handle (void);

      virtual char * get_type_name (void);

      virtual char * get_name (void);

      virtual ::DDS::DomainParticipant_ptr get_participant (void);

      DDSTopic * get_impl (void);

      void set_impl (DDSTopic* topic);

    private:
      DDSTopic * impl_;

      DDSTopic * impl (void);
    };
  }
}

#include "dds4ccm/impl/ndds/Topic_T.cpp"

#endif
