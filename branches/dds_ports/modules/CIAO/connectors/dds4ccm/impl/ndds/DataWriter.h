/**
 * @file DataWriter.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 * 
 * Simple IDL facade on top of NDDS.
 */
#ifndef CIAO_RTI_DATAWRITER_H
#define CIAO_RTI_DATAWRITER_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"
#include "tao/LocalObject.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_DataWriter_i
        : public virtual DDS::CCM_DataWriter,
        public virtual CORBA::LocalObject
      {
      public:
        // Constructor 
        RTI_DataWriter_i (DDSDataWriter *dw);
  
        // Destructor 
        virtual ~RTI_DataWriter_i (void);
  
        virtual
          ::DDS::ReturnCode_t set_qos (const ::DDS::DataWriterQos & qos);
  
        virtual
          ::DDS::ReturnCode_t get_qos (::DDS::DataWriterQos & qos);
  
        virtual
          ::DDS::ReturnCode_t set_listener (::DDS::DataWriterListener_ptr a_listener,
                                            ::DDS::StatusMask mask);
  
        virtual
          ::DDS::DataWriterListener_ptr get_listener (void);
  
        virtual
          ::DDS::Topic_ptr get_topic (void);
  
        virtual
          ::DDS::Publisher_ptr get_publisher (void);
  
        virtual
          ::DDS::ReturnCode_t wait_for_acknowledgments (const ::DDS::Duration_t & max_wait);
  
        virtual
          ::DDS::ReturnCode_t get_liveliness_lost_status (::DDS::LivelinessLostStatus & status);
  
        virtual
          ::DDS::ReturnCode_t get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus & status);
  
        virtual
          ::DDS::ReturnCode_t get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus & status);
  
        virtual
          ::DDS::ReturnCode_t get_publication_matched_status (::DDS::PublicationMatchedStatus & status);
  
        virtual
          ::DDS::ReturnCode_t assert_liveliness (void);
  
        virtual
          ::DDS::ReturnCode_t get_matched_subscriptions (::DDS::InstanceHandleSeq & subscription_handles);
  
        virtual
          ::DDS::ReturnCode_t get_matched_subscription_data (::DDS::SubscriptionBuiltinTopicData & subscription_data,
                                                             ::DDS::InstanceHandle_t subscription_handle);
  
        virtual
          ::DDS::ReturnCode_t enable (void);
  
        virtual
          ::DDS::StatusCondition_ptr get_statuscondition (void);
  
        virtual
          ::DDS::StatusMask get_status_changes (void);
  
        virtual
          ::DDS::InstanceHandle_t get_instance_handle (void);
        
        DDSDataWriter * get_datawriter (void);
        
      private:
        DDSDataWriter * dw_;
      };
    }
  }
}

#endif
