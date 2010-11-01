// $Id$

#include "dds4ccm/impl/ndds/DomainParticipantFactory_T.h"
#include "dds4ccm/impl/ndds/DomainParticipant_T.h"

#include "dds4ccm/impl/ndds/DDSDomainParticipantListener.h"
#include "dds4ccm/impl/ndds/convertors/DomainParticipantFactoryQos.h"
#include "dds4ccm/impl/ndds/convertors/DomainParticipantQos.h"

#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#include "dds4ccm/impl/ndds/DomainParticipantManager.h"


namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE>
    DDS_DomainParticipantFactory_T<DDS_TYPE>::DDS_DomainParticipantFactory_T (void)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_T<DDS_TYPE>::DDS_DomainParticipantFactory_T");
    }

    template <typename DDS_TYPE>
    DDS_DomainParticipantFactory_T<DDS_TYPE>::~DDS_DomainParticipantFactory_T (void)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_T<DDS_TYPE>::~DDS_DomainParticipantFactory_T");
    }

    template <typename DDS_TYPE>
    ::DDS::DomainParticipant_ptr
    DDS_DomainParticipantFactory_T<DDS_TYPE>::create_participant (::DDS::DomainId_t domain_id,
                                                        const ::DDS::DomainParticipantQos & qos,
                                                        ::DDS::DomainParticipantListener_ptr a_listener,
                                                        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_T<DDS_TYPE>::create_participant");

      ACE_UNUSED_ARG (qos);

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_T<DDS_TYPE, "
                    "VENDOR_TYPE>::create_participant - "
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
                          "DDS_DomainParticipantFactory_T"
                          "<DDS_TYPE>::create_participant - "
                          "Error: Unable to create DomainParticipant for domain <%d>\n",
                          domain_id));
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
            }

          ::DDS::DomainParticipant_var retval;
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (dds_dp),
                            ::CORBA::NO_MEMORY ());
          dds_dp->enable ();

          DPMANAGER->add_participant (qos_profile.c_str (), dds_dp);

          return retval._retn ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_DomainParticipantFactory_T<DDS_TYPE, "
                        "VENDOR_TYPE>::create_participant - Re-using participant "
                        "for QOS profile <%C> and domain <%d>.\n",
                        qos_profile.c_str (),
                        domain_id));
          ::DDS::DomainParticipant_var retval;
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (dds_dp),
                            ::CORBA::NO_MEMORY ());
          return retval._retn ();
        }
    }

    template <typename DDS_TYPE>
    ::DDS::DomainParticipant_ptr
    DDS_DomainParticipantFactory_T<DDS_TYPE>::create_participant_with_profile (
      ::DDS::DomainId_t domain_id,
      const char * library_name,
      const char * profile_name,
      ::DDS::DomainParticipantListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_T<DDS_TYPE>::"
                     "create_participant_with_profile");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_T<DDS_TYPE, "
                    "VENDOR_TYPE>::create_participant_with_profile - "
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
                        "DDS_DomainParticipantFactory_T<DDS_TYPE, "
                        "VENDOR_TYPE>::create_participant_with_profile - "
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
                            "DDS_DomainParticipantFactory_T<DDS_TYPE, "
                            "VENDOR_TYPE>::create_participant_with_profile - "
                            "Error: Unable to create DomainParticipant\n"));
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
            }
          ::DDS::DomainParticipant_var retval;
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (dds_dp),
                            ::CORBA::NO_MEMORY ());

          dds_dp->enable ();

          DomainParticipant_type * typed_dp =
            dynamic_cast < DomainParticipant_type *>
            (retval.in ());

          typed_dp->set_impl (dds_dp);
          DPMANAGER->add_participant (qos_profile.c_str (), dds_dp);
          return retval._retn ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_DomainParticipantFactory_T<DDS_TYPE, "
                        "VENDOR_TYPE>::create_participant_with_profile - Re-using "
                        "participant for QOS profile <%C> and domain <%d>.\n",
                        qos_profile.c_str (),
                        domain_id));
          ::DDS::DomainParticipant_var retval;
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (dds_dp),
                            ::CORBA::NO_MEMORY ());
          return retval._retn ();
        }
    }


    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_T<DDS_TYPE>::delete_participant (
      ::DDS::DomainParticipant_ptr a_participant)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_T<DDS_TYPE>::delete_participant");

      DomainParticipant_type *part =
        dynamic_cast< DomainParticipant_type * > (a_participant);

      if (!part)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_DomainParticipantFactory_T"
                        "<DDS_TYPE>::delete_participant - "
                        "Unable to cast provided object reference to servant "
                        "type, bailing.\n"));
          return DDS::RETCODE_ERROR;
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_SUCCESSFUL, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_T"
                    "<DDS_TYPE>::delete_participant - "
                    "Successfully casted provided object reference to servant type.\n"));

      ::DDS::ReturnCode_t retval = DDS::RETCODE_OK;

      if (DPMANAGER->remove_participant (part->get_impl ()))
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
                    "DDS_DomainParticipantFactory_T"
                    "<DDS_TYPE>::delete_participant - "
                    "Going to delete participant.\n"));

          retval = DDSDomainParticipantFactory::get_instance ()->
              delete_participant (part->get_impl ());

          if (retval != DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_DomainParticipantFactory_T"
                            "<DDS_TYPE>::delete_participant - "
                            "RTI delete_participant returned non-ok error code %C\n",
                            translate_retcode (retval)));
            }
          else
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                             "DDS_DomainParticipantFactory_T"
                             "<DDS_TYPE>::delete_participant - "
                             "Successfully deleted provided participant.\n"));
            }
        }
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::DomainParticipant_ptr
    DDS_DomainParticipantFactory_T<DDS_TYPE>::lookup_participant (
      ::DDS::DomainId_t domain_id)
    {
      ::DDS::DomainParticipant_var retval;
      ::DDSDomainParticipant* dp = DDSDomainParticipantFactory::get_instance ()
        ->lookup_participant (domain_id);
       ACE_NEW_THROW_EX (retval,
                         DomainParticipant_type (dp),
                         ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_T<DDS_TYPE>::set_default_participant_qos (
      const ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_T<DDS_TYPE>::"
        "set_default_participant_qos");
      DDS_DomainParticipantQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return DDSDomainParticipantFactory::get_instance ()->
        set_default_participant_qos (ccm_dds_qos);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_T<DDS_TYPE>::get_default_participant_qos (
      ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_T<DDS_TYPE>::"
        "get_default_participant_qos");
      DDS_DomainParticipantQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
        DDSDomainParticipantFactory::get_instance ()->
          get_default_participant_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_T<DDS_TYPE>::set_qos (
      const ::DDS::DomainParticipantFactoryQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_T<DDS_TYPE>::set_qos");
      DDS_DomainParticipantFactoryQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return DDSDomainParticipantFactory::get_instance ()->set_qos (ccm_dds_qos);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_T<DDS_TYPE>::get_qos (
      ::DDS::DomainParticipantFactoryQos & qos)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_T<DDS_TYPE>::get_qos");
      DDS_DomainParticipantFactoryQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
        DDSDomainParticipantFactory::get_instance ()->get_qos (ccm_dds_qos);
      ccm_dds_qos <<= qos;
      return retcode;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DDS_DomainParticipantFactory_T<DDS_TYPE>::set_default_participant_qos_with_profile (
                                                        const char * library_name,
                                                        const char * profile_name)
    {
      DDS4CCM_TRACE ("DDS_DomainParticipantFactory_T<DDS_TYPE>::"
                     "set_default_participant_qos_with_profile");

      return DDSDomainParticipantFactory::get_instance ()->
        set_default_participant_qos_with_profile (library_name, profile_name);
    }
  }
}
