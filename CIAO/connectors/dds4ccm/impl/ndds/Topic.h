/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef NDDS_TOPIC_H
#define NDDS_TOPIC_H

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "tao/LocalObject.h"

class DDSTopic;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_Topic_i :
      public virtual ::DDS::Topic,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      DDS_Topic_i (::DDSTopic* topic,
                 ::DDS::DomainParticipant_ptr dp);

      /// Destructor
      virtual ~DDS_Topic_i (void);

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

      ::DDSTopic * get_rti_entity (void);

      void set_rti_entity (::DDSTopic* topic);

    private:
      ::DDSTopic * rti_entity_;
      ::DDS::DomainParticipant_var dp_;
      ::DDS::TopicListener_var topic_listener_;

      DDSTopic * rti_entity (void);
    };
  }
}

#endif
