/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
 
#ifndef CIAO_RTI_SUBSCRIBER_H
#define CIAO_RTI_SUBSCRIBER_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_Subscriber_i :
        public virtual DDS::CCM_Subscriber
      {
      public:
        // Constructor
        RTI_Subscriber_i (DDSSubscriber *p);

        // Destructor
        virtual ~RTI_Subscriber_i (void);

        virtual ::DDS::ReturnCode_t
        enable (void);

        virtual ::DDS::StatusCondition_ptr
        get_statuscondition (void);

        virtual ::DDS::StatusMask
        get_status_changes (void);

        virtual ::DDS::InstanceHandle_t
        get_instance_handle (void);

        virtual ::DDS::DataReader_ptr
        create_datareader (
          ::DDS::TopicDescription_ptr a_topic,
          const ::DDS::DataReaderQos & qos,
          ::DDS::DataReaderListener_ptr a_listener,
          ::DDS::StatusMask mask);

        virtual ::DDS::ReturnCode_t
        delete_datareader (
          ::DDS::DataReader_ptr a_datareader);

        virtual ::DDS::ReturnCode_t
        delete_contained_entities (void);

        virtual ::DDS::DataReader_ptr
        lookup_datareader (
          const char * topic_name);

        virtual ::DDS::ReturnCode_t
        get_datareaders (
          ::DDS::DataReaderSeq & readers,
          ::DDS::SampleStateMask sample_states,
          ::DDS::ViewStateMask view_states,
          ::DDS::InstanceStateMask instance_states);

        virtual ::DDS::ReturnCode_t
        notify_datareaders (void);

        virtual ::DDS::ReturnCode_t
        set_qos (
          const ::DDS::SubscriberQos & qos);

        virtual ::DDS::ReturnCode_t
        get_qos (
          ::DDS::SubscriberQos & qos);

        virtual ::DDS::ReturnCode_t
        set_listener (
          ::DDS::SubscriberListener_ptr a_listener,
          ::DDS::StatusMask mask);

        virtual ::DDS::SubscriberListener_ptr
        get_listener (void);

        virtual ::DDS::ReturnCode_t
        begin_access (void);

        virtual ::DDS::ReturnCode_t
        end_access (void);

        virtual ::DDS::DomainParticipant_ptr
        get_participant (void);

        virtual ::DDS::ReturnCode_t
        set_default_datareader_qos (
          const ::DDS::DataReaderQos & qos);

        virtual ::DDS::ReturnCode_t
        get_default_datareader_qos (
          ::DDS::DataReaderQos & qos);

        virtual ::DDS::ReturnCode_t
        copy_from_topic_qos (
          ::DDS::DataReaderQos & a_datareader_qos,
          const ::DDS::TopicQos & a_topic_qos);

        DDSSubscriber * get_subscriber (void);
      private:
        DDSSubscriber *sub_;
      };
    }
  }
}

#endif
