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
#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#include <map>

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class CCM_DDS_DomainParticipant_T;

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class CCM_DDS_DomainParticipantFactory_T :
      public virtual ::DDS::CCM_DomainParticipantFactory,
      public virtual ::CORBA::LocalObject
    {

    public:
      typedef CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> DomainParticipant_type;
      typedef CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> DomainParticipantFactory_type;
      friend class ACE_Singleton<DomainParticipantFactory_type, TAO_SYNCH_MUTEX>;

      // Construtor
      CCM_DDS_DomainParticipantFactory_T (void);

      /// Destructor
      virtual ~CCM_DDS_DomainParticipantFactory_T (void);

#if (CIAO_DDS4CCM_OPENDDS==1)
      virtual ::DDS::DomainParticipantFactory_ptr get_instance (void);
#endif

      virtual ::DDS::DomainParticipant_ptr create_participant (
          ::DDS::DomainId_t domain_id,
          const ::DDS::DomainParticipantQos & qos,
          ::DDS::DomainParticipantListener_ptr a_listener,
          ::DDS::StatusMask mask);

#if (CIAO_DDS4CCM_NDDS==1)
      virtual ::DDS::DomainParticipant_ptr create_participant_with_profile (
          ::DDS::DomainId_t domain_id,
          const char * library_name,
          const char * profile_name,
          ::DDS::DomainParticipantListener_ptr a_listener,
          ::DDS::StatusMask mask);
#endif

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

#if (CIAO_DDS4CCM_NDDS==1)
      virtual ::DDS::ReturnCode_t set_default_participant_qos_with_profile(
                                                        const char * library_name,
                                                        const char * profile_name);
#endif
    };
  }
}

#include "dds4ccm/impl/DomainParticipantFactory_T.cpp"

#endif /* DOMAINPARTICIPANTFACTORY_T_H_ */
