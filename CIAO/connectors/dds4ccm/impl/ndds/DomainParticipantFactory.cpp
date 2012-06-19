// $Id$

#include "dds4ccm/impl/ndds/DomainParticipantFactory.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/DomainParticipantListener.h"
#include "dds4ccm/impl/ndds/convertors/DomainParticipantFactoryQos.h"
#include "dds4ccm/impl/ndds/convertors/DomainParticipantQos.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_DomainParticipantFactory_i::DDS_DomainParticipantFactory_i (void)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::DDS_DomainParticipantFactory_i");
    }

    DDS_DomainParticipantFactory_i::~DDS_DomainParticipantFactory_i (void)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::~DDS_DomainParticipantFactory_i");
    }

    ::DDS::DomainParticipant_ptr
    DDS_DomainParticipantFactory_i::create_participant (::DDS::DomainId_t domain_id,
                                                        const ::DDS::DomainParticipantQos & qos,
                                                        ::DDS::DomainParticipantListener_ptr a_listener,
                                                        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::create_participant");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_i::create_participant - "
                    "Start creating domain participant for domain <%d>\n",
                    domain_id));

      DDS_DomainParticipantListener_i *ccm_dds_dpl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_dpl,
                            DDS_DomainParticipantListener_i (a_listener),
                            ::CORBA::NO_MEMORY ());
        }

      ACE_CString qos_profile = "default";

      DDS_DomainParticipantQos ccm_dds_qos;
      DDS_ReturnCode_t retcode = DDSDomainParticipantFactory::get_instance()->get_default_participant_qos (ccm_dds_qos);
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                      "DDS_DomainParticipantFactory_i"
                      "::create_participant - "
                      "Error: Unable to retrieve default participant qos\n"));
          return ::DDS::DomainParticipant::_nil ();
        }
      ccm_dds_qos <<= qos;
      DDSDomainParticipant * dds_dp = DDSDomainParticipantFactory::get_instance ()->
                  create_participant (domain_id,
                                      ccm_dds_qos,
                                      ccm_dds_dpl,
                                      mask);

      if (!dds_dp)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                      "DDS_DomainParticipantFactory_i"
                      "::create_participant - "
                      "Error: Unable to create DomainParticipant for domain <%d>\n",
                      domain_id));
          delete ccm_dds_dpl;
          return ::DDS::DomainParticipant::_nil ();
        }

      ::DDS::DomainParticipant_var retval;
      ACE_NEW_THROW_EX (retval,
                        DDS_DomainParticipant_i (dds_dp),
                        ::CORBA::NO_MEMORY ());
      retcode = dds_dp->enable ();
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                      "DDS_DomainParticipantFactory_i"
                      "::create_participant - "
                      "Error: Unable to enable the domainparticipant "
                      "for domain <%d>: <%C>\n",
                      domain_id,
                      ::CIAO::DDS4CCM::translate_retcode (retcode)));
          delete ccm_dds_dpl;
          throw ::CORBA::INTERNAL ();
        }

      if (ccm_dds_dpl)
        {
          ccm_dds_dpl->set_dds_dp (retval.in ());
        }

      return retval._retn ();
    }


    ::DDS::DomainParticipant_ptr
    DDS_DomainParticipantFactory_i::create_participant_with_profile (
      ::DDS::DomainId_t domain_id,
      const char * qos_profile,
      ::DDS::DomainParticipantListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::"
                     "create_participant_with_profile");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_i::create_participant_with_profile - "
                    "Start creating domain participant: "
                    "profile <%C> - domain <%d>\n",
                    qos_profile, domain_id));
      DDS_DomainParticipantListener_i *ccm_dds_dpl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_dpl,
                            DDS_DomainParticipantListener_i (a_listener),
                            ::CORBA::NO_MEMORY ());
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_i::create_participant_with_profile - "
                    "Creating participant: profile <%C> - domain <%d>\n",
                    qos_profile,
                    domain_id));

      char * lib_name = get_library_name(qos_profile);
      char * prof_name = get_profile_name(qos_profile);

      DDSDomainParticipant * dds_dp = 0;
      if (lib_name != 0 && prof_name != 0)
        {
          dds_dp = DDSDomainParticipantFactory::get_instance ()->
                            create_participant_with_profile (domain_id,
                                                            lib_name,
                                                            prof_name,
                                                            ccm_dds_dpl,
                                                            mask);
        }

      ACE_OS::free (lib_name);
      ACE_OS::free (prof_name);

      if (!dds_dp)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipantFactory_i::create_participant_with_profile <%C> - "
                        "Error: Unable to create DomainParticipant for domain <%d>\n",
                        qos_profile,
                        domain_id));
          delete ccm_dds_dpl;
          return ::DDS::DomainParticipant::_nil ();
        }
      ::DDS::DomainParticipant_var retval;
      ACE_NEW_THROW_EX (retval,
                        DDS_DomainParticipant_i (dds_dp),
                        ::CORBA::NO_MEMORY ());
      DDS_ReturnCode_t retcode = dds_dp->enable ();
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipantFactory_i::create_participant_with_profile <%C> - "
                        "Error: Unable to enable DomainParticipant for domain <%d>: <%C>\n",
                        qos_profile,
                        domain_id,
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          delete ccm_dds_dpl;
          throw ::CORBA::INTERNAL ();
        }

      if (ccm_dds_dpl)
        {
          ccm_dds_dpl->set_dds_dp (retval.in ());
        }
      DDS_DomainParticipant_i * typed_dp =
        dynamic_cast < DDS_DomainParticipant_i *>
        (retval.in ());

      typed_dp->set_rti_entity (dds_dp);

      return retval._retn ();
    }

    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_i::delete_participant (
      ::DDS::DomainParticipant_ptr a_participant)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::delete_participant");

      DDS_DomainParticipant_i *part =
        dynamic_cast< DDS_DomainParticipant_i * > (a_participant);

      if (!part)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipantFactory_i"
                        "::delete_participant - "
                        "Unable to cast provided object reference to servant "
                        "type, bailing.\n"));
          return DDS::RETCODE_ERROR;
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_SUCCESSFUL, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_i"
                    "::delete_participant - "
                    "Successfully casted provided object reference to servant type.\n"));

      const DDS_ReturnCode_t retcode = DDSDomainParticipantFactory::get_instance ()->
                  delete_participant (part->get_rti_entity ());
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_i"
                    "::delete_participant - "
                    "delete_participant returned non-ok error code %C\n",
                    ::CIAO::DDS4CCM::translate_retcode (retcode)));
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_i"
                    "::delete_participant - "
                    "Successfully deleted provided participant.\n"));
        }
      return retcode;
    }


    ::DDS::DomainParticipant_ptr
    DDS_DomainParticipantFactory_i::lookup_participant (
      ::DDS::DomainId_t domain_id)
    {
      ::DDS::DomainParticipant_var retval;
      ::DDSDomainParticipant* dp = DDSDomainParticipantFactory::get_instance ()
        ->lookup_participant (domain_id);
       ACE_NEW_THROW_EX (retval,
                         DDS_DomainParticipant_i (dp),
                         ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_i::set_default_participant_qos (
      const ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::"
        "set_default_participant_qos");
      DDS_DomainParticipantQos ccm_dds_qos;
      DDS_ReturnCode_t const retcode = DDSDomainParticipantFactory::get_instance()->get_default_participant_qos (ccm_dds_qos);
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                      "DDS_DomainParticipantFactory_i"
                      "::set_default_participant_qos - "
                      "Error: Unable to retrieve default participant qos\n"));
          return retcode;
        }
      ccm_dds_qos <<= qos;
      return DDSDomainParticipantFactory::get_instance ()->
        set_default_participant_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_i::get_default_participant_qos (
      ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::"
        "get_default_participant_qos");
      DDS_DomainParticipantQos ccm_dds_qos;
      ::DDS::ReturnCode_t const retcode =
        DDSDomainParticipantFactory::get_instance ()->
          get_default_participant_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }

    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_i::set_qos (
      const ::DDS::DomainParticipantFactoryQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::set_qos");
      DDS_DomainParticipantFactoryQos ccm_dds_qos;
      DDS_ReturnCode_t const retcode = DDSDomainParticipantFactory::get_instance ()->get_qos (ccm_dds_qos);
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                      "DDS_DomainParticipantFactory_i"
                      "::set_qos - "
                      "Error: Unable to retrieve participant factory qos\n"));
          return retcode;
        }
      ccm_dds_qos <<= qos;
      return DDSDomainParticipantFactory::get_instance ()->set_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_i::get_qos (
      ::DDS::DomainParticipantFactoryQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::get_qos");
      DDS_DomainParticipantFactoryQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      ::DDS::ReturnCode_t const retcode =
        DDSDomainParticipantFactory::get_instance ()->get_qos (ccm_dds_qos);
      ccm_dds_qos <<= qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_i::set_default_participant_qos_with_profile (
      const char * qos_profile)
    {
      char * lib_name = get_library_name(qos_profile);
      char * prof_name = get_profile_name(qos_profile);

      ::DDS::ReturnCode_t retcode = ::DDS::RETCODE_ERROR;
      if (lib_name != 0 && prof_name != 0)
        {
          retcode = DDSDomainParticipantFactory::get_instance ()->
            set_default_participant_qos_with_profile (lib_name, prof_name);
        }

      ACE_OS::free (lib_name);
      ACE_OS::free (prof_name);

      return retcode;
    }
  }
}
