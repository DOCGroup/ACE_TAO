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
#include "NDDS_Traits.h"
#include "DomainParticipant.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_Topic_i :
        public virtual DDS::CCM_Topic
      {
      public:
        // Constructor 
        RTI_Topic_i (DDSTopic *);
  
        // Destructor 
        virtual ~RTI_Topic_i (void);
  
        virtual
          ::DDS::ReturnCode_t set_qos (const ::DDS::TopicQos & qos);
  
        virtual
          ::DDS::ReturnCode_t get_qos (::DDS::TopicQos & qos);
  
        virtual
          ::DDS::ReturnCode_t set_listener (::DDS::TopicListener_ptr a_listener,
                                            ::DDS::StatusMask mask);
  
        virtual
          ::DDS::TopicListener_ptr get_listener (void);
  
        virtual
          ::DDS::ReturnCode_t get_inconsistent_topic_status (::DDS::InconsistentTopicStatus & a_status);
  
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
        
        DDSTopic * get_topic (void);
        
      private:
        DDSTopic * topic_;
      };
    }
  }
}

#endif
