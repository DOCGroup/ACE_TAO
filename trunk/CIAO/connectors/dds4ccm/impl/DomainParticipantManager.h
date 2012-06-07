/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 */

#ifndef DOMAINPARTICIPANTMANAGER_H_
#define DOMAINPARTICIPANTMANAGER_H_

#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

#include "ace/Singleton.h"
#include "tao/orbconf.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include <map>
#include <string>

namespace CIAO
{
  namespace DDS4CCM
  {
    /**
     *
     * @class : DomainParticipantManager
     *
     * @brief :
     *
     * Stores a DDSParticipantTopic per domain ID and QOS profile string.
     * When several connectors are joining the same domain ID, using the
     * same QOS settings (ie have the same QOS profile string),
     * the DomainParticipant for that combination is shared. This is done
     * to save resources (running threads/memory usage).
     *
     * The DomainParticipantManager is a singleton.
     *
     */
    class DDS4CCM_DDS_IMPL_Export DomainParticipantManager : private ACE_Copy_Disabled
    {
    friend class ACE_Singleton<DomainParticipantManager, TAO_SYNCH_MUTEX>;

    /**
     *
     * @class : DDSParticipantTopic
     *
     * @brief :
     *
     * Stores a list of topics for a specific domain. If
     * several connectors run in the same process and those
     * connectors are making use of the same topic, the topics
     * are shared amongst the connectors.
     *
     * This class maintains a reference count. It's save to remove
     * a topic once the reference count becomes one.
     *
     */
    class DDSParticipantTopic
    {
      public:
        DDSParticipantTopic (DDS::DomainParticipant_ptr dp);
        ~DDSParticipantTopic (void);

        DDS::DomainParticipant_ptr get_participant ();

        /**
         * Adds a topic to the internal list. Returns
         * false if the topic is already there or when
         * the insertion fails.
         */
        bool register_topic (DDS::Topic_ptr tp);

        /**
         * Removes a topic from the internal list when
         * the reference count for that specific topic
         * is one. If not, it decrements the reference
         * count for the given topic.
         * Returns true if deleted from the list.
         * Returns false if reference count != 1.
         */
        bool unregister_topic (DDS::Topic_ptr tp);

        /**
         * Returns the reference count of this class
         */
        int _ref_count ();
        /**
         * Increments the reference count of this class
         */
        void _inc_ref ();
        /**
         * Decrements the reference count of this class
         */
        void _dec_ref ();

        /**
         * Increments the reference count of the given
         * topic (ie: search for the topic in the internal
         * map and increments its refcount
         */
        void _inc_ref_topic (DDS::Topic_ptr tp);
      private:
        int ref_count_;
        DDS::DomainParticipant_var dp_;

        typedef std::map <std::string, int> Topics;
        typedef Topics::iterator Topics_iterator;
        Topics tps_;
    };

    private:
      /// Constructor
      DomainParticipantManager (void);

    public:
      /// Destructor
      ~DomainParticipantManager (void);

      /**
       * Invokes register_topic on the corresponding
       * DDSParticipantTopic instance.
       * DDSParticipantTopic is found based on the
       * given DomainParticipant_ptr
       */
      bool register_topic (DDS::DomainParticipant_ptr dp,
                           DDS::Topic_ptr tp);

      /**
       * Invokes remove_topic on the corresponding
       * DDSParticipantTopic instance.
       * DDSParticipantTopic is found based on the
       * given DomainParticipant_ptr
       */
      bool unregister_topic (DDS::DomainParticipant_ptr dp,
                             DDS::Topic_ptr tp);

      /**
       * Searches for the DomainParticipant_ptr in the internal map.
       * Search is based on the given domain ID and the given QOS
       * (QOS profile string). If found, it'll increment the
       * reference count of the DDSParticipantTopic instance.
       */
      DDS::DomainParticipant_ptr get_participant (const DDS::DomainId_t domain_id,
                                                  const char * qos_profile);

      /**
       * Adding a DDSParticipantTopic instance when the
       * internal maps doesn't contain a reference. Returns
       * false if there's already an DDSParticipantTopic
       * available (base on domain ID and QOS)
       */
      bool register_participant (const char * qos_profile,
                            DDS::DomainParticipant_ptr dp);

      /**
       * Removes the DDSParticipantTopic instance when the
       * reference count is one.
       * Returns false if the reference count of the corresponding
       * DDSParticipantTopic was not nil
       */
      bool unregister_participant (DDS::DomainParticipant_ptr dp);

      void _inc_ref (DDS::DomainParticipant_ptr dp,
                     DDS::Topic_ptr tp);

    private:
      TAO_SYNCH_MUTEX dps_mutex_;

      typedef std::pair <std::string, DDS::DomainId_t> IdQosProfile;
      typedef std::map < IdQosProfile, DDSParticipantTopic *> DomainParticipants;
      DomainParticipants dps_;

      typedef DomainParticipants::iterator DomainParticipants_iterator;

      /**
       * Search for the DomainParticipant in the internal map.
       * Result is base on the instance handles of the
       * DomainParticipants
       */
      DomainParticipants_iterator
      get_participanttopic_by_participant (DDS::DomainParticipant_ptr dp);
    };

    typedef ACE_Singleton<DomainParticipantManager,
              TAO_SYNCH_MUTEX> Domain_Participant_Manager;
  }
}

#define DPMANAGER ::CIAO::DDS4CCM::Domain_Participant_Manager::instance ()

/// Declare a process wide singleton
DDS4CCM_DDS_IMPL_SINGLETON_DECLARE (ACE_Singleton,
                                    ::CIAO::DDS4CCM::DomainParticipantManager,
                                    TAO_SYNCH_MUTEX)

#endif
