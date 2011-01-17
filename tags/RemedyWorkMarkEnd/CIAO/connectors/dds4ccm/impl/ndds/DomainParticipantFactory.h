/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DOMAINPARTICIPANTFACTORY_T_H_
#define DOMAINPARTICIPANTFACTORY_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "tao/LocalObject.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_DomainParticipantFactory_i :
      public virtual ::DDS::DomainParticipantFactory,
      public virtual ::CORBA::LocalObject
    {

    public:
      friend class ACE_Singleton<DDS_DomainParticipantFactory_i, TAO_SYNCH_MUTEX>;

      /// Constructor
      DDS_DomainParticipantFactory_i (void);

      /// Destructor
      virtual ~DDS_DomainParticipantFactory_i (void);

      virtual ::DDS::DomainParticipant_ptr create_participant (
          ::DDS::DomainId_t domain_id,
          const ::DDS::DomainParticipantQos & qos,
          ::DDS::DomainParticipantListener_ptr a_listener,
          ::DDS::StatusMask mask);

      virtual ::DDS::DomainParticipant_ptr create_participant_with_profile (
          ::DDS::DomainId_t domain_id,
          const char * library_name,
          const char * profile_name,
          ::DDS::DomainParticipantListener_ptr a_listener,
          ::DDS::StatusMask mask);

      virtual ::DDS::ReturnCode_t delete_participant (
        ::DDS::DomainParticipant_ptr a_participant);

      virtual ::DDS::DomainParticipant_ptr lookup_participant (
        ::DDS::DomainId_t domain_id);

      virtual
        ::DDS::ReturnCode_t set_default_participant_qos (const ::DDS::DomainParticipantQos & qos);

      virtual
        ::DDS::ReturnCode_t get_default_participant_qos (::DDS::DomainParticipantQos & qos);

      virtual
        ::DDS::ReturnCode_t set_qos (const ::DDS::DomainParticipantFactoryQos & qos);

      virtual
        ::DDS::ReturnCode_t get_qos (::DDS::DomainParticipantFactoryQos & qos);

      virtual ::DDS::ReturnCode_t set_default_participant_qos_with_profile(
                                                        const char * library_name,
                                                        const char * profile_name);
    };
  }
}

#endif /* DOMAINPARTICIPANTFACTORY_T_H_ */
