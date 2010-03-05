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

#include "dds4ccm/impl/dds/dds4ccm_dds_impl_export.h"

#include "tao/LocalObject.h"

#include "ace/Singleton.h"

#include <map>

namespace CIAO
{
  namespace DDS4CCM
  {
    class CCM_DDS_DomainParticipant_i;

    class DDS4CCM_DDS_IMPL_Export CCM_DDS_DomainParticipantFactory_i :
      public virtual ::DDS::CCM_DomainParticipantFactory,
      public virtual ::CORBA::LocalObject
    {
    friend class ACE_Singleton<CCM_DDS_DomainParticipantFactory_i, TAO_SYNCH_MUTEX>;

    private:
      // Construtor
      CCM_DDS_DomainParticipantFactory_i (void);

      /// Destructor
      virtual ~CCM_DDS_DomainParticipantFactory_i (void);

    public:
#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
      virtual ::DDS::DomainParticipantFactory_ptr get_instance (void);
#endif

      virtual
        ::DDS::DomainParticipant_ptr create_participant (::DDS::DomainId_t domain_id,
                                                         const ::DDS::DomainParticipantQos & qos,
                                                         ::DDS::DomainParticipantListener_ptr a_listener,
                                                         ::DDS::StatusMask mask);

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      virtual
        ::DDS::DomainParticipant_ptr create_participant_with_profile (::DDS::DomainId_t domain_id,
                                                        const char * library_name,
                                                        const char * profile_name,
                                                         ::DDS::DomainParticipantListener_ptr a_listener,
                                                         ::DDS::StatusMask mask);
#endif

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

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      virtual ::DDS::ReturnCode_t set_default_participant_qos_with_profile(
                                                        const char * library_name,
                                                        const char * profile_name);
#endif

    private:
      void remove_participant (CCM_DDS_DomainParticipant_i * part);

      TAO_SYNCH_MUTEX dps_mutex_;
      typedef std::map<ACE_CString, CCM_DDS_DomainParticipant_i *> DomainParticipants;
      DomainParticipants dps_;
    private:
      ACE_UNIMPLEMENTED_FUNC (void operator= (const CCM_DDS_DomainParticipantFactory_i &))
      ACE_UNIMPLEMENTED_FUNC (CCM_DDS_DomainParticipantFactory_i (const CCM_DDS_DomainParticipantFactory_i &))
    };

    typedef ACE_Singleton<CCM_DDS_DomainParticipantFactory_i,
              TAO_SYNCH_MUTEX> Domain_Participant_Factory;
  }
}

#define DPFACTORY ::CIAO::DDS4CCM::Domain_Participant_Factory::instance ()

/// Declare a process wide singleton
DDS4CCM_DDS_IMPL_SINGLETON_DECLARE (ACE_Singleton,
                                      ::CIAO::DDS4CCM::CCM_DDS_DomainParticipantFactory_i,
                                      TAO_SYNCH_MUTEX)

#endif
