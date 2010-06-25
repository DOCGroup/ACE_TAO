// $Id$

#include "DomainParticipantFactory_T.h"
#include "DomainParticipant_T.h"
#include "DomainParticipantListener.h"

#include "ndds/DomainParticipantFactoryQos.h"
#include "ndds/DomainParticipantQos.h"

#include "Utils.h"

#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_NDDS==1)
# include "DomainParticipantManager.h"
#endif


#if (CIAO_DDS4CCM_OPENDDS==1)
  typedef  ::DDS::DomainParticipantFactory DDSDomainParticipantFactory;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::CCM_DDS_DomainParticipantFactory_T (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::CCM_DDS_DomainParticipantFactory_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::~CCM_DDS_DomainParticipantFactory_T (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::~CCM_DDS_DomainParticipantFactory_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DomainParticipant_ptr
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::create_participant (::DDS::DomainId_t domain_id,
                                                        const ::DDS::DomainParticipantQos & qos,
                                                        ::DDS::DomainParticipantListener_ptr a_listener,
                                                        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::create_participant");

#if (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (qos);

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::create_participant - "
                   "Creating domain participant for domain <%d>\n",
                   domain_id));

      CCM_DDS_DomainParticipantListener_i *ccm_dds_dpl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_dpl,
                            CCM_DDS_DomainParticipantListener_i (a_listener),
                            CORBA::NO_MEMORY ());
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
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipantFactory_T"
                          "<DDS_TYPE, CCM_TYPE>::create_participant - "
                          "Error: Unable to create DomainParticipant for domain <%d>\n",
                          domain_id));
              throw CCM_DDS::InternalError (1, 0);
            }

          ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (dds_dp),
                            CORBA::NO_MEMORY ());
          dds_dp->enable ();

          DomainParticipant_type *typed_dp =
            dynamic_cast < DomainParticipant_type *>
            (retval.in ());

          typed_dp->set_impl (dds_dp);

          DPMANAGER->add_participant (qos_profile.c_str (), dds_dp);

          return retval._retn ();
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CCM_DDS_DomainParticipantFactory_T"
                      "<DDS_TYPE, CCM_TYPE>::create_participant - "
                      "Re-using participant for QOS profile <%C> and domain <%d>.\n",
                      qos_profile.c_str (),
                      domain_id));
          ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (dds_dp),
                            CORBA::NO_MEMORY ());
          return retval._retn ();
        }
#else
      return DDSDomainParticipantFactory::get_instance ()->
        create_participant (domain_id,
                            qos,
                            a_listener,
                            mask);
#endif
    }

#if (CIAO_DDS4CCM_NDDS==1)
    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DomainParticipant_ptr
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::create_participant_with_profile (
      ::DDS::DomainId_t domain_id,
      const char * library_name,
      const char * profile_name,
      ::DDS::DomainParticipantListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::"
                     "create_participant_with_profile");

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipantFactory_T"
                    "<DDS_TYPE, CCM_TYPE>::create_participant_with_profile - "
                    "Creating domain participant: profile <%C#%C> - domain <%d>\n",
                    library_name, profile_name, domain_id));
      CCM_DDS_DomainParticipantListener_i *ccm_dds_dpl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_dpl,
                            CCM_DDS_DomainParticipantListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }

      ACE_CString qos_profile = library_name;
      qos_profile += "#";
      qos_profile += profile_name;


      DDSDomainParticipant *dds_dp =
        DPMANAGER->get_participant (qos_profile.c_str ());

      if (!dds_dp)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CCM_DDS_DomainParticipantFactory_T"
                        "<DDS_TYPE, CCM_TYPE>::create_participant_with_profile - "
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
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipantFactory_T"
                            "<DDS_TYPE, CCM_TYPE>::create_participant_with_profile - "
                            "Error: Unable to create DomainParticipant\n"));
              throw CCM_DDS::InternalError (1, 0);
            }
          ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (dds_dp),
                            CORBA::NO_MEMORY ());

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
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CCM_DDS_DomainParticipantFactory_T"
                      "<DDS_TYPE, CCM_TYPE>::create_participant_with_profile - "
                      "Re-using participant for QOS profile <%C> and domain <%d>.\n",
                      qos_profile.c_str (),
                      domain_id));
          ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (dds_dp),
                            CORBA::NO_MEMORY ());
          return retval._retn ();
        }
    }
#endif


    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::delete_participant (
      ::DDS::DomainParticipant_ptr a_participant)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::delete_participant");

#if (CIAO_DDS4CCM_NDDS==1)
      DomainParticipant_type *part =
        dynamic_cast< DomainParticipant_type * > (a_participant);

      if (!part)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipantFactory_T"
                        "<DDS_TYPE, CCM_TYPE>::delete_participant - "
                        "Unable to cast provided object reference to servant type, bailing.\n"));
          return DDS::RETCODE_ERROR;
        }
      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipantFactory_T"
                    "<DDS_TYPE, CCM_TYPE>::delete_participant - "
                    "Successfully casted provided object reference to servant type.\n"));

      ::DDS::ReturnCode_t retval = DDS::RETCODE_OK;
      if (DPMANAGER->remove_participant (part->get_impl ()))
        {
          retval = DDSDomainParticipantFactory::get_instance ()->
              delete_participant (part->get_impl ());

          if (retval != DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipantFactory_T"
                            "<DDS_TYPE, CCM_TYPE>::delete_participant - "
                            "RTI delete_participant returned non-ok error code %C\n",
                            translate_retcode (retval)));
            }
          else DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_DomainParticipantFactory_T"
                            "<DDS_TYPE, CCM_TYPE>::delete_participant - "
                            "Successfully deleted provided participant.\n"));
        }
      return retval;
#else
      return DDSDomainParticipantFactory::get_instance ()->
              delete_participant (a_participant);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DomainParticipant_ptr
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::lookup_participant (
      ::DDS::DomainId_t domain_id)
    {
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDSDomainParticipant* dp = DDSDomainParticipantFactory::get_instance ()
        ->lookup_participant (domain_id);
       ACE_NEW_THROW_EX (retval,
                         DomainParticipant_type (dp),
                         CORBA::NO_MEMORY ());
#else
      ::DDS::DomainParticipant_var dp =
        DDSDomainParticipantFactory::get_instance ()->lookup_participant (domain_id);
      ACE_NEW_THROW_EX (retval,
                        DomainParticipant_type  (dp.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::set_default_participant_qos (
      const ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::"
        "set_default_participant_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_DomainParticipantQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return DDSDomainParticipantFactory::get_instance ()->
        set_default_participant_qos (ccm_dds_qos);
#else
      return DDSDomainParticipantFactory::get_instance ()->
        set_default_participant_qos (qos);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::get_default_participant_qos (
      ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::"
        "get_default_participant_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_DomainParticipantQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
        DDSDomainParticipantFactory::get_instance ()->
          get_default_participant_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
#else
      return DDSDomainParticipantFactory::get_instance ()->
        get_default_participant_qos (qos);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::set_qos (
      const ::DDS::DomainParticipantFactoryQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::set_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_DomainParticipantFactoryQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return DDSDomainParticipantFactory::get_instance ()->set_qos (ccm_dds_qos);
#else
      return DDSDomainParticipantFactory::get_instance ()->set_qos (qos);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::get_qos (
      ::DDS::DomainParticipantFactoryQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::get_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_DomainParticipantFactoryQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
        DDSDomainParticipantFactory::get_instance ()->get_qos (ccm_dds_qos);
      ccm_dds_qos <<= qos;
      return retcode;
#else
      return DDSDomainParticipantFactory::get_instance ()->get_qos (qos);
#endif
    }

#if (CIAO_DDS4CCM_NDDS==1)
    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::set_default_participant_qos_with_profile (
                                                        const char * library_name,
                                                        const char * profile_name)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::"
                     "set_default_participant_qos_with_profile");

      return DDSDomainParticipantFactory::get_instance ()->
        set_default_participant_qos_with_profile (library_name, profile_name);
    }
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DomainParticipantFactory_ptr
    CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>::get_instance (void)
    {
      ::DDS::DomainParticipantFactory_var retval =
        ::DDS::DomainParticipantFactory::_nil ();

      ACE_NEW_THROW_EX (retval,
        CCM_DDS_DomainParticipantFactory_T<DDS_TYPE, CCM_TYPE>,
        CORBA::NO_MEMORY ());

      return retval._retn ();
    }
#endif
  }
}
