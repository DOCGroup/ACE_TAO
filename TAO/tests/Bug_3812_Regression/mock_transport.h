// $Id$

#include "tao/ORB_Core.h"

typedef TAO::Transport_Cache_Manager_T<mock_transport, mock_tdi, mock_ps> TCM;

extern TCM* tcm;
extern int result;
extern TAO_SYNCH_MUTEX test_lock;
extern TAO_Condition<TAO_SYNCH_MUTEX> test_condition;


class mock_transport
{
public:
  mock_transport (TAO_ORB_Core *orb_core)
      : id_(0)
      , is_connected_(false)
      , entry_(0)
      , purging_order_ (0)
      , purged_count_ (0)
      , handler_lock_ (orb_core->resource_factory ()->create_cached_connection_lock ())
  {}

  size_t id (void) const {return id_;}
  void id (size_t id) { this->id_ = id;}
  unsigned long purging_order (void) const {return purging_order_;}
  void purging_order (unsigned long purging_order) { this->purging_order_ = purging_order;}
  bool is_connected (void) const {return is_connected_;}
  void is_connected (bool is_connected) { this->is_connected_ = is_connected;}
  ACE_Event_Handler::Reference_Count add_reference (void) {return 0;}
  ACE_Event_Handler::Reference_Count remove_reference (void) {return 0;}

  // Implementation needs be similar to TAO_Transport::cache_map_entry().
  void cache_map_entry (TCM::HASH_MAP_ENTRY *entry) {
    ACE_GUARD (ACE_Lock, ace_mon, *this->handler_lock_);
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)cache_map_entry %X\n", entry));
    this->entry_ = entry;
  }
  TCM::HASH_MAP_ENTRY *cache_map_entry (void) {return this->entry_;}
  void close_connection (void) { purged_count_ = ++global_purged_count;};
  int purged_count (void) { return this->purged_count_;}
  bool can_be_purged (void) { return true;}

  // Implementation needs be similar to TAO_Transport::purge_entry().
  int purge_entry (void)
  {
    TCM::HASH_MAP_ENTRY* entry = 0;
    {
      ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, -1);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t)purge_entry %X\n", this->entry_));
      entry = this->entry_;
      this->entry_ = 0;
    }
    return tcm->purge_entry (entry);
  }

  // Implementation needs be similar to TAO_Transport::make_idle().
  //
  //  int
  //  TAO_Transport::make_idle (void)
  //  {
  //    if (TAO_debug_level > 3)
  //      {
  //        ACE_DEBUG ((LM_DEBUG,
  //                    ACE_TEXT ("TAO (%P|%t) - Transport[%d]::make_idle\n"),
  //                    this->id ()));
  //      }
  //
  //    return this->transport_cache_manager ().make_idle (this->cache_map_entry_);
  //  }

  // Code are added to simulate the situation that the cached entry pointer passed
  // to TCM to make entry idle is deleted by another thread b/c of re-cache transport
  // and cause TCM make idle on an invalid entry. Otherwise we need delay and similar
  // code in TCM::make_idl() to reproduce the problem.

  int make_idle (void)
  {
    static bool is_first = true;

    ACE_DEBUG ((LM_DEBUG, "(%P|%t)make_idle pass entry %X\n", this->entry_));

    TCM::HASH_MAP_ENTRY* entry = this->entry_;

    // The first thread comes to this point, record the entry
    // and wait for second thread to process listen point which
    // re-cache transport.
    if (is_first)
    {
      is_first = false;
      test_condition.wait ();
    }

    ACE_DEBUG ((LM_DEBUG, "(%P|%t)make_idle execute on entry %X and now entry %X\n",
      entry, this->entry_));

    // When the first thread is at this point, the entry is
    // deleted by second thread, so make idle on the invalid
    // entry cause SEGV. It's possible that the memory is still
    // available then next checking if entry is changed should
    // confirm if the entry is valid or not.
    int ret = tcm->make_idle (entry);
    if (entry != this->entry_)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)ERROR: Entry was changed  ")
        ACE_TEXT ("after passing to TCM and before calling make_idl.\n")));
      result = 1;
    }

    return ret;
  }


private:
  size_t id_;
  bool is_connected_;
  TCM::HASH_MAP_ENTRY *entry_;
  unsigned long purging_order_;
  /// When did we got purged
  int purged_count_;
  mutable ACE_Lock *handler_lock_;
};


