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

    bool
    DomainParticipantManager::DDSParticipantTopic::register_topic (DDS::Topic_ptr tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::register_topic");

      CORBA::String_var name = tp->get_name ();
      if (this->tps_.find (name.in ()) == this->tps_.end ())
        {
          std::pair <Topics_iterator, bool> to_insert =
            this->tps_.insert (std::make_pair<std::string, int>(name.in (), 1));
          if (!to_insert.second)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DomainParticipantManager::DDSParticipantTopic"
                            "::register_topic - Unable to register topic <%C>\n",
                            name.in ()));
              return false;
            }

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DomainParticipantManager::DDSParticipantTopic"
                        "::register_topic - Registered provided topic <%C> - "
                        "ref_count <%d>\n",
                        name.in (), to_insert.first->second));
          return true;
        }
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::DDSParticipantTopic"
                    "::register_topic - Topic <%C> already registered. "
                    "No need to register it again.\n",
                    name.in ()));
      return true;
    }

    bool
    DomainParticipantManager::DDSParticipantTopic::unregister_topic (DDS::Topic_ptr tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::unregister_topic");

      CORBA::String_var name = tp->get_name ();

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::DDSParticipantTopic"
                    "::unregister_topic - Check if topic <%C> with "
                    "ref_count <%d> can be unregistered\n",
                    name.in (),
                    this->tps_[name.in ()]));

      Topics_iterator it = this->tps_.find(name.in ());
      if (it != this->tps_.end ())
        {
          if (it->second == 1)
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                            "DomainParticipantManager::DDSParticipantTopic"
                            "::unregister_topic - Save to unregister topic <%C>\n",
                            name.in ()));
              return true;
            }
        }
      --it->second;
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::DDSParticipantTopic"
                    "::unregister_topic - Decremented ref_count for topic <%C>: "
                    "ref_count <%d>\n",
                    name.in (),
                    it->second));
      return false;
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

    void
    DomainParticipantManager::DDSParticipantTopic::_inc_ref_topic (DDS::Topic_ptr tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::_inc_ref_topic");

      CORBA::String_var name = tp->get_name ();
      Topics_iterator it = this->tps_.find (name.in ());
      if (it == this->tps_.end ())
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DomainParticipantManager::register_topic - "
                        "Unable to find provided topic <%C>\n",
                        name.in ()));
        }

      ++this->tps_[name.in ()];

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::DDSParticipantTopic"
                    "::_inc_ref_topic - Increment topic ref_count. topic <%C> "
                    "- ref_count <%d>\n",
                    tp->get_name (),
                    this->tps_[name.in ()]));
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
                    "DomainParticipantManager::~DomainParticipantManager - "
                    "Finalizing DDS\n"));

#if (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipantFactory::finalize_instance ();
#elif (CIAO_DDS4CCM_OPENDDS==1)
//      TheServiceParticipant->shutdown ();
#endif
    }

    bool
    DomainParticipantManager::register_topic (DDS::DomainParticipant_ptr dp,
      DDS::Topic_ptr tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::register_topic");

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());

      DomainParticipants_iterator iter =
        this->get_participanttopic_by_participant (dp);

      if (iter != this->dps_.end ())
        {
          return iter->second->register_topic (tp);
        }
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DomainParticipantManager::register_topic - "
                    "Unable to find provided DomainParticipant with handle "
                    DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER ".\n",
                    DDS_INSTANCE_HANDLE_LOG (dp->get_instance_handle ())));
      return false;
    }

    bool
    DomainParticipantManager::unregister_topic (DDS::DomainParticipant_ptr dp,
      DDS::Topic_ptr tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::unregister_topic");

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());

      DomainParticipants_iterator iter =
        this->get_participanttopic_by_participant (dp);

      if (iter != this->dps_.end ())
        {
          return iter->second->unregister_topic (tp);
        }

      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DomainParticipantManager::unregister_topic - "
                    "Unable to find provided DomainParticipant with handle "
                    DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER ".\n",
                    DDS_INSTANCE_HANDLE_LOG (dp->get_instance_handle ())));
      return false;
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
                        DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER "\n",
                        domain_id,
                        qos_profile,
                        it_found->second->_ref_count (),
                        DDS_INSTANCE_HANDLE_LOG (dp->get_instance_handle ())));

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
    DomainParticipantManager::register_participant (const char * qos_profile,
      DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::register_participant");

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());

      DDS::DomainId_t const domain_id = dp->get_domain_id ();
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
                            "Unable to insert a new DomainParticipant/Topic "
                            "combination for <%d, %C>.\n",
                            domain_id, qos_profile));
              return false;
            }

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DomainParticipantManager::register_participant - "
                        "Added a new DomainParticipant/Topic "
                        "combination for <%d, %C>. Handle "
                        DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER ".\n",
                        domain_id, qos_profile,
                        DDS_INSTANCE_HANDLE_LOG (dp->get_instance_handle ())));
          return true;

        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::register_participant - "
                    "Don't add participant for domain <%d> with profile <%C> since it already "
                    "exists : <"
                    DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER ">\n",
                    domain_id, qos_profile,
                    DDS_INSTANCE_HANDLE_LOG (dp->get_instance_handle ())));
      return false;
    }

    bool
    DomainParticipantManager::unregister_participant (DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::unregister_participant");

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());

      DDS::DomainId_t const domain_id = dp->get_domain_id ();
      DomainParticipants_iterator iter =
        this->get_participanttopic_by_participant (dp);
      if (iter != this->dps_.end ())
        {
          if (iter->second->_ref_count () == 1)
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_UNIMP_ACTION, (LM_TRACE, DDS4CCM_INFO
                            "DomainParticipantManager::unregister_participant - "
                            "Delete participant "
                            DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER
                            " for domain <%d> since ref_count is one.\n",
                            DDS_INSTANCE_HANDLE_LOG (dp->get_instance_handle ()),
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
                            DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER
                            " for domain <%d> since "
                            "it's still used - ref_count is <%d>\n",
                            DDS_INSTANCE_HANDLE_LOG (dp->get_instance_handle ()),
                            domain_id,
                            iter->second->_ref_count ()));
              iter->second->_dec_ref ();
              return false;
            }
        }

#if (CIAO_DDS4CCM_OPENDDS==1)
      if (this->dps_.empty ())
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DomainParticipantManager::unregister_participant - "
                        "No participants anymore, shutting down OpenDDS.\n"));
          // TheServiceParticipant->shutdown ();
        }
#endif

      return true;
    }

    void
    DomainParticipantManager::_inc_ref (DDS::DomainParticipant_ptr dp,
      DDS::Topic_ptr tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::_inc_ref");

      DomainParticipants_iterator iter =
        this->get_participanttopic_by_participant (dp);
      if (iter != this->dps_.end ())
        {
          iter->second->_inc_ref_topic (tp);
        }
    }

    DomainParticipantManager::DomainParticipants_iterator
    DomainParticipantManager::get_participanttopic_by_participant (DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::get_participanttopic_by_participant");

      DomainParticipants_iterator pos = this->dps_.begin();
      while (pos != this->dps_.end())
        {
          if (pos->second && pos->second->get_participant ()->get_instance_handle () ==
              dp->get_instance_handle ())
            {
              break;
            }
          ++pos;
        }
      return pos;
    }
  }
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<CIAO::DDS4CCM::DomainParticipantManager, TAO_SYNCH_MUTEX> *
         ACE_Singleton<CIAO::DDS4CCM::DomainParticipantManager, TAO_SYNCH_MUTEX>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
