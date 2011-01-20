/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef PUBLISHER_T_H_
#define PUBLISHER_T_H_

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "tao/LocalObject.h"

class DDSPublisher;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_Publisher_i :
      public virtual ::DDS::Publisher,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      DDS_Publisher_i (DDSPublisher * p,
                     ::DDS::DomainParticipant_ptr dp);

      /// Destructor
      virtual ~DDS_Publisher_i (void);

      virtual
        ::DDS::DataWriter_ptr create_datawriter (::DDS::Topic_ptr a_topic,
                                                 const ::DDS::DataWriterQos & qos,
                                                 ::DDS::DataWriterListener_ptr a_listener,
                                                 ::DDS::StatusMask mask);

      virtual ::DDS::DataWriter_ptr create_datawriter_with_profile (
          ::DDS::Topic_ptr a_topic,
          const char* qos_profile,
          ::DDS::DataWriterListener_ptr a_listener,
          ::DDS::StatusMask mask);

      virtual
        ::DDS::ReturnCode_t delete_datawriter (::DDS::DataWriter_ptr a_datawriter);

      virtual
        ::DDS::DataWriter_ptr lookup_datawriter (const char * impl_name);

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
        ::DDS::ReturnCode_t copy_from_topic_qos (::DDS::DataWriterQos & a_dataimpl_qos,
                                                 const ::DDS::TopicQos & a_impl_qos);

      virtual
        ::DDS::ReturnCode_t enable (void);

      virtual
        ::DDS::StatusCondition_ptr get_statuscondition (void);

      virtual
        ::DDS::StatusMask get_status_changes (void);

      virtual
        ::DDS::InstanceHandle_t get_instance_handle (void);

      DDSPublisher * get_rti_entity (void);

    private:
      DDSPublisher *rti_entity_;
      ::DDS::DomainParticipant_var dp_;

      DDSPublisher *rti_entity (void);
    };
  }
}

#endif /* PUBLISHER_T_H_ */
