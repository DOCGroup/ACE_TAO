/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_NDDS_TOPIC_H
#define CIAO_NDDS_TOPIC_H

#include "DomainParticipant.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"
#include "tao/LocalObject.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::Topic DDSTopic;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_NDDS_IMPL_Export CCM_DDS_Topic_i :
      public virtual ::DDS::CCM_Topic,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      CCM_DDS_Topic_i (DDSTopic* topic);

      /// Destructor
      virtual ~CCM_DDS_Topic_i (void);

      virtual ::DDS::ReturnCode_t set_qos (const ::DDS::TopicQos & qos);

      virtual ::DDS::ReturnCode_t get_qos (::DDS::TopicQos & qos);

      virtual ::DDS::ReturnCode_t set_listener (
        ::DDS::TopicListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::TopicListener_ptr get_listener (void);

      virtual ::DDS::ReturnCode_t get_inconsistent_topic_status (
        ::DDS::InconsistentTopicStatus & a_status);

      virtual
        ::DDS::ReturnCode_t enable (void);

      virtual
        ::DDS::StatusCondition_ptr get_statuscondition (void);

      virtual
        ::DDS::StatusMask get_status_changes (void);

      virtual
        ::DDS::InstanceHandle_t get_instance_handle (void);

      virtual
        char * get_type_name (void);

      virtual
        char * get_name (void);

      virtual
        ::DDS::DomainParticipant_ptr get_participant (void);

      DDSTopic * get_impl (void);

      void set_impl (DDSTopic* topic);

    private:
      DDSTopic * impl_;

      DDSTopic * impl (void);
    };
  }
}

#endif
