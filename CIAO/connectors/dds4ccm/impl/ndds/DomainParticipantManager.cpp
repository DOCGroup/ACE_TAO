// $Id$

#include "DomainParticipantManager.h"
#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/impl/Utils.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    //============================================================
    // DomainParticipantManager::DDSParticipantTopic::DDSParticipantTopic
    //============================================================
    DomainParticipantManager::DDSParticipantTopic::DDSParticipantTopic (
      DDSDomainParticipant * dp)
      : ref_count_ (1),
        dp_ (dp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::DDSParticipantTopic");
    }

    DomainParticipantManager::DDSParticipantTopic::~DDSParticipantTopic (void)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::~DDSParticipantTopic");
    }

    DDSDomainParticipant *
    DomainParticipantManager::DDSParticipantTopic::get_participant ()
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::get_participant");

      return this->dp_;
    }

    bool
    DomainParticipantManager::DDSParticipantTopic::add_topic (DDSTopic * tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::add_topic");

      if (this->tps_.find (tp) == this->tps_.end ())
        {
          this->tps_[tp] = 1;

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DomainParticipantManager::DDSParticipantTopic"
                        "::add_topic - Added provided topic. topic <%C> - "
                        "ref_count <%d>\n",
                        tp->get_name (),
                        this->tps_[tp]));
          return true;
        }
      return false;
    }

    bool
    DomainParticipantManager::DDSParticipantTopic::remove_topic (DDSTopic * tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::remove_topic");
      if (this->tps_[tp] == 1)
        {
          return true;
        }
      --this->tps_[tp];
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::DDSParticipantTopic"
                    "::remove_topic - Decremented ref_count. topic <%C> - "
                    "ref_count <%d>\n",
                    tp->get_name (),
                    this->tps_[tp]));
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
    DomainParticipantManager::DDSParticipantTopic::_inc_ref_topic (DDSTopic * tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DDSParticipantTopic::_inc_ref_topic");

      ++this->tps_[tp];

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::DDSParticipantTopic"
                    "::_inc_ref_topic - Increment topic ref_count. topic <%C> "
                    "- ref_count <%d>\n",
                    tp->get_name (),
                    this->tps_[tp]));
    }

    //============================================================
    // DomainParticipantManager::DomainParticipantManager
    //============================================================
    DomainParticipantManager::DomainParticipantManager (void)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::DomainParticipantManager");
    }

    DomainParticipantManager::~DomainParticipantManager (void)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::~DomainParticipantManager");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::~DomainParticipantManager - "
                    "Finalizing DDS\n"));

      DDSDomainParticipantFactory::finalize_instance ();
    }

    bool
    DomainParticipantManager::add_topic (DDSDomainParticipant * dp,
                                         DDSTopic * tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::add_topic");
      DomainParticipants_iterator iter =
        this->get_participanttopic_by_participant (dp);
      if (iter != this->dps_.end ())
        {
          return iter->second->add_topic (tp);
        }
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DomainParticipantManager::add_topic - "
                    "Unable to find provided DomainParticipant.\n"));
      return false;
    }

    bool
    DomainParticipantManager::remove_topic (DDSDomainParticipant * dp,
                                            DDSTopic * tp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::remove_topic");
      DomainParticipants_iterator iter =
        this->get_participanttopic_by_participant (dp);
      if (iter != this->dps_.end ())
        {
          return iter->second->remove_topic (tp);
        }
      return false;
    }

    DDSDomainParticipant *
    DomainParticipantManager::get_participant (
      const char * qos_profile)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::get_participant");

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());
      DDSParticipantTopic * dpt = this->dps_[qos_profile];
      if (dpt)
        {
          dpt->_inc_ref ();
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DomainParticipantManager::get_participant - "
                        "DomainParticipant found. profile <%C> - ref_count <%d>\n",
                        qos_profile,
                        dpt->_ref_count ()));
          return dpt->get_participant ();
        }
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::get_participant - "
                    "DomainParticipant with profile <%C> does not exist.\n",
                    qos_profile));
      return 0;
    }

    bool
    DomainParticipantManager::add_participant (
      const char * qos_profile,
      DDSDomainParticipant * dp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::add_participant");

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());
      DomainParticipants_iterator iter =
        this->get_participanttopic_by_participant (dp);
      if (iter == this->dps_.end ())
        {
          DDSParticipantTopic * dpt = 0;
          ACE_NEW_THROW_EX (dpt,
                            DDSParticipantTopic (dp),
                            ::CORBA::NO_MEMORY ());
          this->dps_[qos_profile] = dpt;
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DomainParticipantManager::add_participant - "
                        "Added participant with profile <%C>.\n",
                        qos_profile));
          return true;
        }
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DomainParticipantManager::add_participant - "
                    "Don't add participant with profile <%C> since it already "
                    "exists.\n",
                    qos_profile));
      return false;
    }

    bool
    DomainParticipantManager::remove_participant (
      DDSDomainParticipant * dp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::remove_participant");

      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                      this->dps_mutex_, CORBA::INTERNAL ());

      DomainParticipants_iterator iter =
        this->get_participanttopic_by_participant (dp);
      if (iter != this->dps_.end ())
        {
          if (iter->second->_ref_count () == 1)
            {
              delete iter->second;

              // Save to remove from list
              this->dps_.erase (iter);
            }
          else
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_UNIMP_ACTION, (LM_TRACE, DDS4CCM_INFO
                            "DomainParticipantManager::remove_participant - "
                            "Don't delete participant since "
                            "it's still used - ref_count <%d>\n",
                            iter->second->_ref_count ()));
              iter->second->_dec_ref ();
              return false;
            }
        }
      return true;
    }

    void
    DomainParticipantManager::_inc_ref (DDSDomainParticipant * dp,
                                        DDSTopic * tp)
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
    DomainParticipantManager::get_participanttopic_by_participant (
      DDSDomainParticipant * dp)
    {
      DDS4CCM_TRACE ("DomainParticipantManager::get_participanttopic_by_participant");

      DomainParticipants_iterator pos = this->dps_.begin();
      while (pos != this->dps_.end())
        {
          if (pos->second && pos->second->get_participant () == dp)
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
