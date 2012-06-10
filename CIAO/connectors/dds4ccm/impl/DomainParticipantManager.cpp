// $Id$

#include "dds4ccm/impl/DomainParticipantManager.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/Utils.h"
#include "ace/Env_Value_T.h"

#if (CIAO_DDS4CCM_NDDS==1)
# include "ndds/convertors/InstanceHandle_t.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
# include "dds/DCPS/transport/framework/TransportDebug.h"
# include "dds/DCPS/Service_Participant.h"
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    //============================================================
    // DomainParticipantManager::DDSParticipantTopic::DDSParticipantTopic
    //============================================================
    DomainParticipantManager::DDSParticipantTopic::DDSParticipantTopic (
      DDS::DomainParticipant_ptr dp)
      : ref_count_ (1),
        dp_ (DDS::DomainParticipant::_duplicate(dp))
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::DDSParticipantTopic");
    }

    DomainParticipantManager::DDSParticipantTopic::~DDSParticipantTopic (void)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::~DDSParticipantTopic");
    }

    DDS::DomainParticipant_ptr
    DomainParticipantManager::DDSParticipantTopic::get_participant ()
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::get_participant");

      return DDS::DomainParticipant::_duplicate(this->dp_);
    }

    int
    DomainParticipantManager::DDSParticipantTopic::_ref_count ()
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::_ref_count");

      return this->ref_count_;
    }

    void
    DomainParticipantManager::DDSParticipantTopic::_inc_ref ()
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::_inc_ref");

      ++this->ref_count_;
    }

    void
    DomainParticipantManager::DDSParticipantTopic::_dec_ref ()
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::_dec_ref");

      --this->ref_count_;
    }

    //============================================================
    // DomainParticipantManager::DomainParticipantManager
    //============================================================
    DomainParticipantManager::DomainParticipantManager (void)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DomainParticipantManager");

#if (CIAO_DDS4CCM_NDDS==1)
      ACE_Env_Value<int> verbosity (ACE_TEXT("DDS4CCM_NDDS_LOG_VERBOSITY"),
        NDDS_CONFIG_LOG_VERBOSITY_SILENT);

      NDDS_Config_LogVerbosity n_verbosity =
        static_cast <NDDS_Config_LogVerbosity> (verbosity.operator int());
      NDDSConfigLogger::get_instance()->set_verbosity (n_verbosity);
#elif (CIAO_DDS4CCM_OPENDDS==1)
      ACE_Env_Value<int> dcpsdl (ACE_TEXT("DDS4CCM_OPENDDS_LOG_LEVEL"), 0);
      OpenDDS::DCPS::DCPS_debug_level = dcpsdl;
      ACE_Env_Value<int> dcpsdtl (ACE_TEXT("DDS4CCM_OPENDDS_TRANSPORT_LOG_LEVEL"), 0);
      OpenDDS::DCPS::Transport_debug_level = dcpsdtl;
#endif
    }

    DomainParticipantManager::~DomainParticipantManager (void)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::~DomainParticipantManager");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::~DomainParticipantManager\n"));
    }

    DDS::DomainParticipant_ptr
    DomainParticipantManager::get_participant (const DDS::DomainId_t domain_id,
      const char * qos_profile)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::get_participant");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::get_participant - "
                    "Searching DomainParticipant for domain <%d> "
                    "with profile <%C>\n",
                    domain_id, qos_profile));

      std::string profile;
      if (qos_profile)
        {
          profile = qos_profile;
        }
      IdQosProfile idqos =
        std::make_pair<std::string, DDS::DomainId_t>(profile, domain_id);
      DomainParticipants_iterator it_found = this->dps_.find (idqos);

      if (it_found != this->dps_.end () && it_found->second)
        {
          ::DDS::DomainParticipant_var dp = it_found->second->get_participant ();
          it_found->second->_inc_ref ();
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DomainParticipantManager::get_participant - "
                        "DomainParticipant found. domain <%d> - "
                        "profile <%C> - ref_count <%d> - handle "
                        DDS_ENTITY_FORMAT_SPECIFIER "\n",
                        domain_id,
                        qos_profile,
                        it_found->second->_ref_count (),
                        DDS_ENTITY_LOG (dp.in ())));

          return dp._retn ();
        }
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::get_participant - "
                    "DomainParticipant for domain <%d> with profile <%C> "
                    "does not exist.\n",
                    domain_id, qos_profile));
      return ::DDS::DomainParticipant::_nil ();
    }

    bool
    DomainParticipantManager::register_participant (
      DDS::DomainId_t domain_id,
      const char * qos_profile,
      DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::register_participant");

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());

      std::string profile;
      if (qos_profile)
        {
          profile = qos_profile;
        }
      IdQosProfile idqos =
        std::make_pair<std::string, DDS::DomainId_t>(profile, domain_id);
      DomainParticipants_iterator it_found = this->dps_.find (idqos);

      if (it_found == this->dps_.end())
        {
          DDSParticipantTopic * dpt = 0;
          ACE_NEW_THROW_EX (dpt,
                            DDSParticipantTopic (dp),
                            ::CORBA::NO_MEMORY ());
          std::pair <DomainParticipants_iterator, bool> to_insert =
            this->dps_.insert (std::make_pair<IdQosProfile,
                                              DDSParticipantTopic *>(idqos, dpt));
          if (!to_insert.second)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DomainParticipantManager::register_participant - "
                            "Unable to insert a new DomainParticipant "
                            "for <%d> <%C>.\n",
                            domain_id, qos_profile));
              return false;
            }

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DomainParticipantManager::register_participant - "
                        "Added a new DomainParticipant "
                        "for <%d> <%C>. Handle "
                        DDS_ENTITY_FORMAT_SPECIFIER ".\n",
                        domain_id, qos_profile,
                        DDS_ENTITY_LOG (dp)));
          return true;

        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::register_participant - "
                    "Don't add participant for domain <%d> with profile <%C> since it already "
                    "exists : <"
                    DDS_ENTITY_FORMAT_SPECIFIER ">\n",
                    domain_id, qos_profile,
                    DDS_ENTITY_LOG (dp)));
      return false;
    }

    bool
    DomainParticipantManager::unregister_participant (
      DDS::DomainId_t domain_id,
      const char * qos_profile,
      DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::unregister_participant");

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());

      std::string profile;
      if (qos_profile)
        {
          profile = qos_profile;
        }
      IdQosProfile idqos =
        std::make_pair<std::string, DDS::DomainId_t>(profile, domain_id);
      DomainParticipants_iterator iter = this->dps_.find (idqos);

      if (iter != this->dps_.end ())
        {
          if (iter->second->_ref_count () == 1)
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_UNIMP_ACTION, (LM_TRACE, DDS4CCM_INFO
                            "DomainParticipantManager::unregister_participant - "
                            "Delete participant "
                            DDS_ENTITY_FORMAT_SPECIFIER
                            " for domain <%d> since ref_count is one.\n",
                            DDS_ENTITY_LOG (dp),
                            domain_id));
              delete iter->second;

              // Save to remove from list
              this->dps_.erase (iter);
            }
          else
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_UNIMP_ACTION, (LM_TRACE, DDS4CCM_INFO
                            "DomainParticipantManager::unregister_participant - "
                            "Don't delete participant "
                            DDS_ENTITY_FORMAT_SPECIFIER
                            " for domain <%d> since "
                            "it's still used - ref_count is <%d>\n",
                            DDS_ENTITY_LOG (dp),
                            domain_id,
                            iter->second->_ref_count ()));
              iter->second->_dec_ref ();
              return false;
            }
        }

      return true;
    }

    bool
    DomainParticipantManager::close ()
    {
      DDS4CCM_TRACE ("DomainParticipantManager::close");

      if (this->dps_.empty ())
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DomainParticipantManager::close - "
                        "No participants anymore, shutting down DDS.\n"));
#if (CIAO_DDS4CCM_OPENDDS==1)
          TheServiceParticipant->shutdown ();
#elif (CIAO_DDS4CCM_NDDS==1)
          DDSDomainParticipantFactory::finalize_instance ();
#endif
          return true;
        }

      return false;
    }
  }
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<CIAO::DDS4CCM::DomainParticipantManager, TAO_SYNCH_MUTEX> *
         ACE_Singleton<CIAO::DDS4CCM::DomainParticipantManager, TAO_SYNCH_MUTEX>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
