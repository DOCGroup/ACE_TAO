/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DOMAINPARTICIPANTMANAGER_H_
#define DOMAINPARTICIPANTMANAGER_H_

#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

#include "ace/Singleton.h"
#include "ace/SString.h"
#include "tao/orbconf.h"

#include "ndds/ndds_cpp.h"

#include <map>

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export DomainParticipantManager
    {
    friend class ACE_Singleton<DomainParticipantManager, TAO_SYNCH_MUTEX>;

    class DDSParticipantTopic
    {
      public:
        DDSParticipantTopic (DDSDomainParticipant * dp);
        ~DDSParticipantTopic (void);

        DDSDomainParticipant * get_participant ();

        bool add_topic (DDSTopic  * tp);
        bool remove_topic (DDSTopic * tp);

        int _ref_count ();
        void _inc_ref ();
        void _dec_ref ();

        void _inc_ref_topic (DDSTopic * tp);
      private:
        int ref_count_;
        DDSDomainParticipant * dp_;

        typedef std::map <DDSTopic *, int> Topics;
        typedef Topics::iterator Topics_iterator;
        Topics tps_;
    };

    public:
      /// Constructor
      DomainParticipantManager (void);

      /// Destructor
      virtual ~DomainParticipantManager (void);

      bool
      add_topic (DDSDomainParticipant *dp,
                 DDSTopic * tp);

      bool
      remove_topic (DDSDomainParticipant * dp,
                    DDSTopic *tp);


      DDSDomainParticipant *
      get_participant (const char * qos_profile);

      bool
      add_participant (const char * qos_profile,
                       DDSDomainParticipant * dp);

      bool
      remove_participant (DDSDomainParticipant * dp);

      void
      _inc_ref (DDSDomainParticipant * dp,
                DDSTopic * tp);

    private:
      TAO_SYNCH_MUTEX dps_mutex_;
      typedef std::map <ACE_CString, DDSParticipantTopic *> DomainParticipants;
      typedef DomainParticipants::iterator DomainParticipants_iterator;
      DomainParticipants dps_;

      DomainParticipants_iterator
      get_participanttopic_by_participant (DDSDomainParticipant * dp);

    private:
      ACE_UNIMPLEMENTED_FUNC (void operator= (const DomainParticipantManager &))
      ACE_UNIMPLEMENTED_FUNC (DomainParticipantManager (const DomainParticipantManager &))
    };

    typedef ACE_Singleton<DomainParticipantManager,
              TAO_SYNCH_MUTEX> Domain_Participant_Factory;
  }
}

#define DPMANAGER ::CIAO::DDS4CCM::Domain_Participant_Factory::instance ()

/// Declare a process wide singleton
DDS4CCM_DDS_IMPL_SINGLETON_DECLARE (ACE_Singleton,
                                    ::CIAO::DDS4CCM::DomainParticipantManager,
                                    TAO_SYNCH_MUTEX)

#endif
