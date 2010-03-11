// $Id$

#include "DomainParticipantFactory.h"
#include "DomainParticipant.h"
#include "DomainParticipantListener.h"

#include "ndds/DomainParticipantFactoryQos.h"
#include "ndds/DomainParticipantQos.h"

#include "Utils.h"

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/idl/dds4ccm_BaseC.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef  ::DDS::DomainParticipantFactory DDSDomainParticipantFactory;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
     CCM_DDS_DomainParticipantFactory_i::CCM_DDS_DomainParticipantFactory_i (void)
     {
       DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::CCM_DDS_DomainParticipantFactory_i");
     }

     CCM_DDS_DomainParticipantFactory_i::~CCM_DDS_DomainParticipantFactory_i (void)
     {
       DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::~CCM_DDS_DomainParticipantFactory_i");
     }

    ::DDS::DomainParticipant_ptr
    CCM_DDS_DomainParticipantFactory_i::create_participant (::DDS::DomainId_t domain_id,
                                                        const ::DDS::DomainParticipantQos & qos,
                                                        ::DDS::DomainParticipantListener_ptr a_listener,
                                                        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::create_participant");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (qos);

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipantFactory_i::create_participant - "
                   "Creating domain participant for domain <%d>\n",
                   domain_id));

      CCM_DDS_DomainPublisherListener_i *ccm_dds_dpl = 0;
      if (!CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_dpl,
                            CCM_DDS_DomainPublisherListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }

      ACE_CString qos_profile = ACE_TEXT ("default");

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());
      {
        CCM_DDS_DomainParticipant_i *ccm_dds_dp = this->dps_[qos_profile];

        if (!ccm_dds_dp)
          {
            DDSDomainParticipant *part = DDSDomainParticipantFactory::get_instance ()->
              create_participant (domain_id,
                                  DDS_PARTICIPANT_QOS_DEFAULT,
                                  ccm_dds_dpl,
                                  mask);

            if (!part)
              {
                DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipantFactory_i::create_participant - "
                            "Error: Unable to create DomainParticipant for domain <%d>\n",
                            domain_id));
                throw CCM_DDS::InternalError (1, 0);
              }

            ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
            ACE_NEW_THROW_EX (retval,
                              CCM_DDS_DomainParticipant_i (part),
                              CORBA::NO_MEMORY ());
            part->enable ();
            ccm_dds_dp = dynamic_cast < CCM_DDS_DomainParticipant_i *> (retval.in ());
            ccm_dds_dp->set_impl (part);

            this->dps_[qos_profile] = ccm_dds_dp;

            return retval._retn ();
          }
        else
          {
            DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CCM_DDS_DomainParticipantFactory_i::create_participant_with_profile - "
                        "Re-using participant for QOS profile <%C> and domain <%d>.\n",
                        qos_profile.c_str (),
                        domain_id));
            return ::DDS::DomainParticipant::_duplicate (ccm_dds_dp);
          }
      }

#else
      return DDSDomainParticipantFactory::get_instance ()->
        create_participant (domain_id,
                            qos,
                            a_listener,
                            mask);
#endif
    }

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
    ::DDS::DomainParticipant_ptr
    CCM_DDS_DomainParticipantFactory_i::create_participant_with_profile (
      ::DDS::DomainId_t domain_id,
      const char * library_name,
      const char * profile_name,
      ::DDS::DomainParticipantListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::create_participant_with_profile");

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipantFactory_i::create_participant_with_profile - "
                   "Creating domain participant: profile <%C#%C> - domain <%d>\n",
                   library_name, profile_name, domain_id));
      CCM_DDS_DomainPublisherListener_i *ccm_dds_dpl = 0;
      if (!CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_dpl,
                            CCM_DDS_DomainPublisherListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }

      ACE_CString qos_profile = library_name;
      qos_profile += ACE_TEXT ("#");
      qos_profile += profile_name;

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());
      {
        CCM_DDS_DomainParticipant_i *ccm_dds_dp = this->dps_[qos_profile];

        if (!ccm_dds_dp)
          {
            DDS4CCM_DEBUG (6, (LM_DEBUG, "CCM_DDS_DomainParticipantFactory_i::create_participant_with_profile - "
                                      "Creating participant: profile <%C> - domain <%d>\n",
                                      qos_profile.c_str (),
                                      domain_id));
            DDSDomainParticipant * part = DDSDomainParticipantFactory::get_instance ()->
                    create_participant_with_profile (domain_id,
                                      library_name,
                                      profile_name,
                                      ccm_dds_dpl,
                                      mask);
            if (!part)
              {
                DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipantFactory_i::create_participant_with_profile - "
                            "Error: Unable to create DomainParticipant\n"));
                throw CCM_DDS::InternalError (1, 0);
              }
            ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
            ACE_NEW_THROW_EX (retval,
                              CCM_DDS_DomainParticipant_i (part),
                              CORBA::NO_MEMORY ());

            part->enable ();

            ccm_dds_dp = dynamic_cast < CCM_DDS_DomainParticipant_i *> (retval.in ());
            ccm_dds_dp->set_impl (part);
            this->dps_[qos_profile] = ccm_dds_dp;
            return retval._retn ();
          }
        else
          {
            DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CCM_DDS_DomainParticipantFactory_i::create_participant_with_profile - "
                        "Re-using participant for QOS profile <%C> and domin <%d>.\n",
                        qos_profile.c_str (),
                        domain_id));
            return ::DDS::DomainParticipant::_duplicate (ccm_dds_dp);
          }
      }
    }
#endif

    void
    CCM_DDS_DomainParticipantFactory_i::remove_participant (CCM_DDS_DomainParticipant_i * part)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::remove_participant");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());

      if (part->_refcount_value () == 1)
        {
          DomainParticipants::iterator pos;
          for (pos = this->dps_.begin(); pos != this->dps_.end(); ++pos)
            {
              if (pos->second == part)
                {
                  DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipantFactory_i::remove_participant - "
                            "Removing participant for %C from list.\n",
                            pos->first.c_str ()));
                  this->dps_.erase (pos->first);
                  break;
                }
            }
        }
      else
        {
          DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipantFactory_i::remove_participant - "
                    "Don't delete participant since it's still used - ref_count <%d>\n",
                    part->_refcount_value ()));
        }
#else
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_i::delete_participant (::DDS::DomainParticipant_ptr a_participant)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::delete_participant");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_DomainParticipant_i *part = dynamic_cast< CCM_DDS_DomainParticipant_i * > (a_participant);

      if (!part)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipantFactory_i::delete_participant - "
                       "Unable to cast provided object reference to servant type, bailing.\n"));
          return DDS::RETCODE_ERROR;
        }
      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_DomainParticipantFactory_i::delete_participant - "
                   "Successfully casted provided object reference to servant type.\n"));

      this->remove_participant (part);

      DDS::ReturnCode_t retval = DDS::RETCODE_OK;

      if (part->_refcount_value () == 1)
        {
          retval = DDSDomainParticipantFactory::get_instance ()->
              delete_participant (part->get_impl ());

          if (retval != DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_DomainParticipantFactory_i::delete_participant - "
                          "RTI delete_participant returned non-ok error code %C\n",
                          translate_retcode (retval)));
            }
          else DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_DomainParticipantFactory_i::delete_participant - "
                            "Successfully deleted provided participant.\n"));
        }
      return retval;
#else
      return DDSDomainParticipantFactory::get_instance ()->
              delete_participant (a_participant);
#endif
    }

    ::DDS::DomainParticipant_ptr
    CCM_DDS_DomainParticipantFactory_i::lookup_participant (::DDS::DomainId_t domain_id)
    {
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipant* dp = DDSDomainParticipantFactory::get_instance ()->lookup_participant (domain_id);
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DomainParticipant_i (dp),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::DomainParticipant_var dp = DDSDomainParticipantFactory::get_instance ()->lookup_participant (domain_id);
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DomainParticipant_i (dp.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_i::set_default_participant_qos (const ::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::set_default_participant_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDS_DomainParticipantQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return DDSDomainParticipantFactory::get_instance ()->set_default_participant_qos (ccm_dds_qos);
#else
      return DDSDomainParticipantFactory::get_instance ()->set_default_participant_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_i::get_default_participant_qos (::DDS::DomainParticipantQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::get_default_participant_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDS_DomainParticipantQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
        DDSDomainParticipantFactory::get_instance ()->get_default_participant_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
#else
      return DDSDomainParticipantFactory::get_instance ()->get_default_participant_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_i::set_qos (const ::DDS::DomainParticipantFactoryQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::set_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDS_DomainParticipantFactoryQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return DDSDomainParticipantFactory::get_instance ()->set_qos (ccm_dds_qos);
#else
      return DDSDomainParticipantFactory::get_instance ()->set_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_i::get_qos (::DDS::DomainParticipantFactoryQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::get_qos");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      DDS_DomainParticipantFactoryQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
        DDSDomainParticipantFactory::get_instance ()->get_qos (ccm_dds_qos);
      ccm_dds_qos <<= qos;
      return retcode;
#else
      return DDSDomainParticipantFactory::get_instance ()->get_qos (qos);
#endif
    }

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
    ::DDS::ReturnCode_t
    CCM_DDS_DomainParticipantFactory_i::set_default_participant_qos_with_profile (
                                                        const char * library_name,
                                                        const char * profile_name)
    {
      DDS4CCM_TRACE ("CCM_DDS_DomainParticipantFactory_i::set_default_participant_qos_with_profile");

      return DDSDomainParticipantFactory::get_instance ()->set_default_participant_qos_with_profile (library_name, profile_name);
    }
#endif

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
    ::DDS::DomainParticipantFactory_ptr
    CCM_DDS_DomainParticipantFactory_i::get_instance (void)
    {
      return 0;
    }
#endif
  }
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<CIAO::DDS4CCM::CCM_DDS_DomainParticipantFactory_i, TAO_SYNCH_MUTEX> *
         ACE_Singleton<CIAO::DDS4CCM::CCM_DDS_DomainParticipantFactory_i, TAO_SYNCH_MUTEX>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
