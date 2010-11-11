// $Id$

#include "dds4ccm/impl/ndds/DomainParticipantFactory.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/DomainParticipantListener.h"
#include "dds4ccm/impl/ndds/convertors/DomainParticipantFactoryQos.h"
#include "dds4ccm/impl/ndds/convertors/DomainParticipantQos.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DomainParticipantManager.h"
#include "dds4ccm/idl/dds4ccm_BaseC.h" // @todo, this should not be here
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

      ACE_UNUSED_ARG (qos);

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

      DDSDomainParticipant *dds_dp =
        DPMANAGER->get_participant (qos_profile.c_str ());

      if (!dds_dp)
        {
          dds_dp = DDSDomainParticipantFactory::get_instance ()->
                      create_participant (domain_id,
                                          DDS_PARTICIPANT_QOS_DEFAULT,
                                          ccm_dds_dpl,
                                          mask);

          if (!dds_dp)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                          "DDS_DomainParticipantFactory_i"
                          "::create_participant - "
                          "Error: Unable to create DomainParticipant for domain <%d>\n",
                          domain_id));
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
            }

          ::DDS::DomainParticipant_var retval;
          ACE_NEW_THROW_EX (retval,
                            DDS_DomainParticipant_i (dds_dp),
                            ::CORBA::NO_MEMORY ());
          dds_dp->enable ();

          DPMANAGER->add_participant (qos_profile.c_str (), dds_dp);

          return retval._retn ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_DomainParticipantFactory_i::create_participant - Re-using participant "
                        "for QOS profile <%C> and domain <%d>.\n",
                        qos_profile.c_str (),
                        domain_id));
          ::DDS::DomainParticipant_var retval;
          ACE_NEW_THROW_EX (retval,
                            DDS_DomainParticipant_i (dds_dp),
                            ::CORBA::NO_MEMORY ());
          return retval._retn ();
        }
    }


    ::DDS::DomainParticipant_ptr
    DDS_DomainParticipantFactory_i::create_participant_with_profile (
      ::DDS::DomainId_t domain_id,
      const char * library_name,
      const char * profile_name,
      ::DDS::DomainParticipantListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::"
                     "create_participant_with_profile");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_i::create_participant_with_profile - "
                    "Start creating domain participant: "
                    "profile <%C#%C> - domain <%d>\n",
                    library_name, profile_name, domain_id));
      DDS_DomainParticipantListener_i *ccm_dds_dpl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_dpl,
                            DDS_DomainParticipantListener_i (a_listener),
                            ::CORBA::NO_MEMORY ());
        }

      ACE_CString qos_profile = library_name;
      qos_profile += "#";
      qos_profile += profile_name;


      DDSDomainParticipant *dds_dp =
        DPMANAGER->get_participant (qos_profile.c_str ());

      if (!dds_dp)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_DomainParticipantFactory_i::create_participant_with_profile - "
                        "Creating participant: profile <%C> - domain <%d>\n",
                        qos_profile.c_str (),
                        domain_id));
          dds_dp = DDSDomainParticipantFactory::get_instance ()->
                            create_participant_with_profile (domain_id,
                                                             library_name,
                                                             profile_name,
                                                             ccm_dds_dpl,
                                                             mask);
          if (!dds_dp)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_DomainParticipantFactory_i::create_participant_with_profile - "
                            "Error: Unable to create DomainParticipant\n"));
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
            }
          ::DDS::DomainParticipant_var retval;
          ACE_NEW_THROW_EX (retval,
                            DDS_DomainParticipant_i (dds_dp),
                            ::CORBA::NO_MEMORY ());

          dds_dp->enable ();

          DDS_DomainParticipant_i * typed_dp =
            dynamic_cast < DDS_DomainParticipant_i *>
            (retval.in ());

          typed_dp->set_rti_entity (dds_dp);
          DPMANAGER->add_participant (qos_profile.c_str (), dds_dp);
          return retval._retn ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_DomainParticipantFactory_i::create_participant_with_profile - Re-using "
                        "participant for QOS profile <%C> and domain <%d>.\n",
                        qos_profile.c_str (),
                        domain_id));
          ::DDS::DomainParticipant_var retval;
          ACE_NEW_THROW_EX (retval,
                            DDS_DomainParticipant_i (dds_dp),
                            ::CORBA::NO_MEMORY ());
          return retval._retn ();
        }
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

      ::DDS::ReturnCode_t retval = DDS::RETCODE_OK;

      if (DPMANAGER->remove_participant (part->get_rti_entity ()))
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_i"
                    "::delete_participant - "
                    "Going to delete participant.\n"));

          retval = DDSDomainParticipantFactory::get_instance ()->
              delete_participant (part->get_rti_entity ());

          if (retval != DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_DomainParticipantFactory_i"
                            "::delete_participant - "
                            "RTI delete_participant returned non-ok error code %C\n",
                            ::CIAO::DDS4CCM::translate_retcode (retval)));
            }
          else
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                             "DDS_DomainParticipantFactory_i"
                             "::delete_participant - "
                             "Successfully deleted provided participant.\n"));
            }
        }
      return retval;
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
      ::DDS::ReturnCode_t retcode =
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
      ccm_dds_qos <<= qos;
      return DDSDomainParticipantFactory::get_instance ()->set_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_i::get_qos (
      ::DDS::DomainParticipantFactoryQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::get_qos");
      DDS_DomainParticipantFactoryQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
        DDSDomainParticipantFactory::get_instance ()->get_qos (ccm_dds_qos);
      ccm_dds_qos <<= qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_i::set_default_participant_qos_with_profile (
                                                        const char * library_name,
                                                        const char * profile_name)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_i::"
                     "set_default_participant_qos_with_profile");

      return DDSDomainParticipantFactory::get_instance ()->
        set_default_participant_qos_with_profile (library_name, profile_name);
    }
  }
}
