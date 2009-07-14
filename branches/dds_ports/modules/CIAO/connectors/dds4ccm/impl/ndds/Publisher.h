/**
 * @file Publisher.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 * 
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_PUBLISHER_H
#define CIAO_RTI_PUBLISHER_H

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
      class DDS4CCM_NDDS_IMPL_Export RTI_Publisher_i :
        public virtual DDS::CCM_Publisher
      {
      public:
        // Constructor 
        RTI_Publisher_i (DDSPublisher *p);
  
        // Destructor 
        virtual ~RTI_Publisher_i (void);
  
        virtual
          ::DDS::DataWriter_ptr create_datawriter (::DDS::Topic_ptr a_topic,
                                                   const ::DDS::DataWriterQos & qos,
                                                   ::DDS::DataWriterListener_ptr a_listener,
                                                   ::DDS::StatusMask mask);
  
        virtual
          ::DDS::ReturnCode_t delete_datawriter (::DDS::DataWriter_ptr a_datawriter);
  
        virtual
          ::DDS::DataWriter_ptr lookup_datawriter (const char * topic_name);
  
        virtual
          ::DDS::ReturnCode_t delete_contained_entities (void);
  
        virtual
          ::DDS::ReturnCode_t set_qos (const ::DDS::PublisherQos & qos);
  
        virtual
          ::DDS::ReturnCode_t get_qos (::DDS::PublisherQos & qos);
  
        virtual
          ::DDS::ReturnCode_t set_listener (::DDS::PublisherListener_ptr a_listener,
                                            ::DDS::StatusMask mask);
  
        virtual
          ::DDS::PublisherListener_ptr get_listener (void);
  
        virtual
          ::DDS::ReturnCode_t suspend_publications (void);
  
        virtual
          ::DDS::ReturnCode_t resume_publications (void);
  
        virtual
          ::DDS::ReturnCode_t begin_coherent_changes (void);
  
        virtual
          ::DDS::ReturnCode_t end_coherent_changes (void);
  
        virtual
          ::DDS::ReturnCode_t wait_for_acknowledgments (const ::DDS::Duration_t & max_wait);
  
        virtual
          ::DDS::DomainParticipant_ptr get_participant (void);
  
        virtual
          ::DDS::ReturnCode_t set_default_datawriter_qos (const ::DDS::DataWriterQos & qos);
  
        virtual
          ::DDS::ReturnCode_t get_default_datawriter_qos (::DDS::DataWriterQos & qos);
  
        virtual
          ::DDS::ReturnCode_t copy_from_topic_qos (::DDS::DataWriterQos & a_datawriter_qos,
                                                   const ::DDS::TopicQos & a_topic_qos);
  
        virtual
          ::DDS::ReturnCode_t enable (void);
  
        virtual
          ::DDS::StatusCondition_ptr get_statuscondition (void);
  
        virtual
          ::DDS::StatusMask get_status_changes (void);
  
        virtual
          ::DDS::InstanceHandle_t get_instance_handle (void);
        
        DDSPublisher * get_publisher (void);
      private:
        DDSPublisher *pub_;
      };
    }
  }
}

#endif
