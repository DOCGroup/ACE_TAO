/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
 
#ifndef DDS_RTI_PARTICIPANTFACTORY_H
#define DDS_RTI_PARTICIPANTFACTORY_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"

#include "tao/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_DomainParticipantFactory_i : 
        public virtual DDS::CCM_DomainParticipantFactory
      {
      public:
        // Constructor 
        RTI_DomainParticipantFactory_i (void);
  
        // Destructor 
        virtual ~RTI_DomainParticipantFactory_i (void);
  
        virtual
          ::DDS::DomainParticipant_ptr create_participant (::DDS::DomainId_t domain_id,
                                                           const ::DDS::DomainParticipantQos & qos,
                                                           ::DDS::DomainParticipantListener_ptr a_listener,
                                                           ::DDS::StatusMask mask);
  
        virtual
          ::DDS::ReturnCode_t delete_participant (::DDS::DomainParticipant_ptr a_participant);
        
        virtual
          ::DDS::DomainParticipant_ptr lookup_participant (::DDS::DomainId_t domain_id);
  
        virtual
          ::DDS::ReturnCode_t set_default_participant_qos (const ::DDS::DomainParticipantQos & qos);
  
        virtual
          ::DDS::ReturnCode_t get_default_participant_qos (::DDS::DomainParticipantQos & qos);
  
        virtual
          ::DDS::ReturnCode_t set_qos (const ::DDS::DomainParticipantFactoryQos & qos);
  
        virtual
          ::DDS::ReturnCode_t get_qos (::DDS::DomainParticipantFactoryQos & qos);
      };
    }
  }
}


#endif
