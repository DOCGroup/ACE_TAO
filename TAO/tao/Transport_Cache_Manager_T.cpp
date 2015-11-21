#include "tao/Transport_Cache_Manager_T.h"
#include "tao/debug.h"
#include "tao/Connection_Purging_Strategy.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Condition.h"
#include "tao/Wait_Strategy.h"
#include "ace/ACE.h"
#include "ace/Reactor.h"
#include "ace/Lock_Adapter_T.h"

#include <algorithm>

#if !defined (__ACE_INLINE__)
# include "tao/Transport_Cache_Manager_T.inl"
#endif /* __ACE_INLINE__ */

// notes on debug level and LM_xxxx codes for transport cache
// TAO_debug_level > 0: recoverable error condition (LM_ERROR)
// TAO_debug_level > 4: normal transport cache operations (LM_INFO)
// TAO_debug_level > 6: detailed cache operations (LM_DEBUG)
// TAO_debug_level > 8: for debugging the cache itself (LM_DEBUG)


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template <typename TT, typename TRDT, typename PSTRAT>
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::Transport_Cache_Manager_T (
    int percent,
    purging_strategy* purging_strategy,
    size_t cache_maximum,
    bool locked,
    const char *orbid)
    : percent_ (percent)
    , purging_strategy_ (purging_strategy)
    , cache_map_ (cache_maximum)
    , cache_lock_ (0)
    , cache_maximum_ (cache_maximum)
#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
    , purge_monitor_ (0)
    , size_monitor_ (0)
#endif /* TAO_HAS_MONITOR_POINTS==1 */
  {
    if (locked)
      {
        ACE_NEW (this->cache_lock_,
                 ACE_Lock_Adapter <TAO_SYNCH_MUTEX> (this->cache_map_mutex_));
      }
    else
      {
        ACE_NEW (this->cache_lock_,
                 ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX>);
      }

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
    ACE_NEW (this->purge_monitor_,
             ACE::Monitor_Control::Size_Monitor);
    ACE_NEW (this->size_monitor_,
             ACE::Monitor_Control::Size_Monitor);

    ACE_CString purge_name ("Connection_Cache_Purge_");
    ACE_CString size_name ("Connection_Cache_Size_");

    purge_name += orbid;
    size_name += orbid;

    this->purge_monitor_->name (purge_name.c_str ());
    this->size_monitor_->name (size_name.c_str ());

    this->purge_monitor_->add_to_registry ();
    this->size_monitor_->add_to_registry ();
#else
  ACE_UNUSED_ARG (orbid);
#endif /* TAO_HAS_MONITOR_POINTS==1 */
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::~Transport_Cache_Manager_T (void)
  {
    delete this->cache_lock_;
    this->cache_lock_ = 0;

    delete this->purging_strategy_;
    this->purging_strategy_ = 0;

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
    this->purge_monitor_->remove_from_registry ();
    this->size_monitor_->remove_from_registry ();
    this->purge_monitor_->remove_ref ();
    this->size_monitor_->remove_ref ();
#endif /* TAO_HAS_MONITOR_POINTS==1 */
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  void
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::set_entry_state (HASH_MAP_ENTRY_REF &entry,
                                            TAO::Cache_Entries_State state)
  {
    ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->cache_lock_));
    if (entry.entry_ != 0 && entry.int_id_ != 0)
      {
        entry.int_id_->recycle_state (state);
        if (state != ENTRY_UNKNOWN && state != ENTRY_CONNECTING
            && entry.int_id_->transport ())
          {
            entry.int_id_->is_connected (
              entry.int_id_->transport ()->is_connected ());
          }
      }
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::bind_i (
    Cache_ExtId &ext_id,
    Cache_IntId &int_id)
  {
    if (TAO_debug_level > 4)
       {
         TAOLIB_DEBUG ((LM_INFO,
            ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::bind_i, ")
            ACE_TEXT ("Transport[%d] @ hash{%d}\n"),
            int_id.transport ()->id (),
            ext_id.hash ()));
       }

    // Get the entry too
    HASH_MAP_ENTRY *entry = 0;

    // Update the purging strategy information while we
    // are holding our lock
    this->purging_strategy_->update_item (*(int_id.transport ()));
    int retval = 0;
    if (this->cache_map_.current_size () >= cache_maximum_)
      {
         retval = -1;
         if (TAO_debug_level > 0)
           {
             TAOLIB_ERROR ((LM_ERROR,
               ACE_TEXT("TAO (%P|%t) - Transport_Cache_Manager_T::bind_i, ")
               ACE_TEXT("ERROR: unable to bind transport, cache is full\n")));
           }
      }
    else
      {
        retval = this->cache_map_.bind (ext_id, int_id, entry);
        if (retval == 0)
          {
            // The entry has been added to cache successfully
            // Add the cache_map_entry to the transport
            Cache_IntId* int_id_p = 0;
            ACE_Unbounded_Set<Cache_IntId>& int_ids = entry->item();
            for (typename ACE_Unbounded_Set<Cache_IntId>::iterator it = int_ids.begin();
                 it != int_ids.end(); ++it)
              {
                if ((*it) == int_id)
                  {
                    int_id_p = &*it;
                    break;
                  }
              }
            ACE_ASSERT (int_id_p != 0);
            HASH_MAP_ENTRY_REF ref;
            ref.entry_ = entry;
            ref.int_id_ = int_id_p;
            int_id.transport ()->cache_map_entry (ref);
          }
        else if (retval == 1)
          {
            ACE_Unbounded_Set<Cache_IntId>& int_ids = entry->item();
            for (typename ACE_Unbounded_Set<Cache_IntId>::iterator it = int_ids.begin();
                 it != int_ids.end(); ++it)
              {
                if ((*it).transport () == int_id.transport ())
                  {
                    // update the cache status
                    // we are already holding the lock, do not call set_entry_state
                    (*it).recycle_state (int_id.recycle_state ());
                    if (TAO_debug_level > 9 &&
                        (*it).is_connected () != int_id.is_connected ())
                      TAOLIB_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("TAO (%P|%t) - Transport_Cache_")
                                  ACE_TEXT ("Manager::bind_i, Updating existing ")
                                  ACE_TEXT ("entry sets is_connected to %C\n"),
                                  (int_id.is_connected () ? "true" : "false")));
                    (*it).is_connected (int_id.is_connected ());
                    retval = 0;
                    break;
                  }
              }
          }
      }
    if (retval != 0)
      {
        if (TAO_debug_level > 0)
          {
            TAOLIB_ERROR ((LM_ERROR,
                           ACE_TEXT("TAO (%P|%t) - Transport_Cache_Manager_T::bind_i, ")
                           ACE_TEXT("ERROR: unable to bind transport\n")));
          }
      }
    else // (retval == 0)
      {
        if (TAO_debug_level > 4)
          {
            TAOLIB_DEBUG ((LM_INFO,
              ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::bind_i: ")
              ACE_TEXT ("Success Transport[%d] @ hash{%d}. ")
              ACE_TEXT ("Cache size is [%d]\n"),
              int_id.transport ()->id (),
              ext_id.hash (),
              this->current_size ()
              ));
          }
      }

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
    this->size_monitor_->receive (this->current_size ());
#endif /* TAO_HAS_MONITOR_POINTS==1 */

    return retval;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  typename Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::Find_Result
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::find_transport (
    transport_descriptor_type *prop,
    transport_type *&transport,
    size_t &busy_count)
  {
    if (prop == 0)
      {
        transport = 0;
        return CACHE_FOUND_NONE;
      }

    Find_Result const find_result = this->find (prop, transport, busy_count);
    if (find_result != CACHE_FOUND_NONE)
      {
        if (find_result == CACHE_FOUND_AVAILABLE)
          {
            if (transport->wait_strategy ()->non_blocking () == 0 &&
                transport->orb_core ()->client_factory ()->use_cleanup_options ())
              {
                ACE_Event_Handler * const eh =
                  transport->event_handler_i ();

                ACE_Reactor * const r =
                  transport->orb_core ()->reactor ();

                if (eh &&
                    r->remove_handler (eh,
                                       ACE_Event_Handler::READ_MASK |
                                       ACE_Event_Handler::DONT_CALL) == -1)
                  {
                    if (TAO_debug_level > 0)
                      TAOLIB_ERROR ((LM_ERROR,
                                  ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T[%d]")
                                  ACE_TEXT ("::find_transport, remove_handler failed\n"),
                                  transport->id ()));
                  }
                else
                  {
                    transport->wait_strategy ()->is_registered (false);
                  }
              }
          }
      }
    return find_result;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  typename Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::Find_Result
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::find_i (
    transport_descriptor_type *prop,
    transport_type *&transport,
    size_t &busy_count)
  {
    // Get the entry from the Hash Map
    Find_Result found = CACHE_FOUND_NONE;

    // Make a temporary object. It does not do a copy.
    Cache_ExtId key (prop);
    HASH_MAP_ENTRY *entry = 0;
    busy_count = 0;
    HASH_MAP_ENTRY *found_entry = 0;
    Cache_IntId *found_int_id = 0;

    int cache_status = this->cache_map_.find (key, entry);
    if (cache_status == 0)
    {
      // loop until we find a usable transport, or until we've checked
      // all cached entries for this endpoint
      ACE_Unbounded_Set<Cache_IntId>& int_ids = entry->item();
      for (typename ACE_Unbounded_Set<Cache_IntId>::iterator it = int_ids.begin();
           it != int_ids.end(); ++it)
        {
          if (is_entry_available_i (*it))
              {
                // Successfully found a transport_type.
                found = CACHE_FOUND_AVAILABLE;
                found_entry = entry;
                found_int_id = &*it;
                found_int_id->recycle_state (ENTRY_BUSY);

                if (TAO_debug_level > 6)
                  {
                    TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::find_i, ")
                      ACE_TEXT ("Found available Transport[%d] @hash {%d}\n"),
                      found_int_id->transport ()->id (),
                      entry->ext_id_.hash ()
                      ));
                  }
                break;
              }
          else if (is_entry_connecting_i (*it))
              {
                if (TAO_debug_level > 6)
                  {
                    TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::find_i, ")
                      ACE_TEXT ("Found connecting Transport[%d] @hash {%d}\n"),
                      (*it).transport ()->id (),
                      entry->ext_id_.hash ()
                      ));
                  }
                // if this is the first interesting entry
                if (found != CACHE_FOUND_CONNECTING)
                  {
                    found_entry = entry;
                    found_int_id = &*it;
                    found = CACHE_FOUND_CONNECTING;
                  }
              }
            else
              {
                // if this is the first busy entry
                if (found == CACHE_FOUND_NONE && busy_count == 0)
                  {
                    found_entry = entry;
                    found_int_id = &*it;
                    found = CACHE_FOUND_BUSY;
                  }
                ++busy_count;
                if (TAO_debug_level > 6)
                  {
                    TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::find_i, ")
                      ACE_TEXT ("Found busy Transport[%d] @hash {%d}\n"),
                      (*it).transport ()->id (),
                      entry->ext_id_.hash ()
                      ));
                  }
              }
        }
    }
    if (found_entry != 0 && found_int_id != 0)
    {
      transport = found_int_id->transport ();
      transport->add_reference ();
      if (found == CACHE_FOUND_AVAILABLE)
        {
          // Update the purging strategy information while we
          // are holding our lock
          this->purging_strategy_->update_item (*transport);
        }
    }
    return found;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::make_idle_i (HASH_MAP_ENTRY_REF &entry)
  {
    entry.int_id_->recycle_state (ENTRY_IDLE_AND_PURGABLE);

    return 0;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::update_entry (HASH_MAP_ENTRY_REF &entry)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                              guard,
                              *this->cache_lock_, -1));

    if (entry.entry_ == 0)
      return -1;

    purging_strategy *st = this->purging_strategy_;
    (void) st->update_item (*(entry.int_id_->transport ()));

    return 0;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::close_i (Connection_Handler_Set &handlers)
  {
    HASH_MAP_ITER end_iter = this->cache_map_.end ();
    HASH_MAP_ENTRY_REF zero_ref;

    for (HASH_MAP_ITER iter = this->cache_map_.begin ();
         iter != end_iter;
         ++iter)
      {
        ACE_Unbounded_Set<Cache_IntId>& int_ids = (*iter).item();

        for (typename ACE_Unbounded_Set<Cache_IntId>::iterator it = int_ids.begin();
             it != int_ids.end(); ++it)
        {
          // Get the transport to fill its associated connection's handler.
          (*it).transport ()->provide_handler (handlers);

          // Inform the transport that has a reference to the entry in the
          // map that we are *gone* now. So, the transport should not use
          // the reference to the entry that he has, to access us *at any
          // time*.
          (*it).transport ()->cache_map_entry (zero_ref);
        }
      }

    // Unbind all the entries in the map
    this->cache_map_.unbind_all ();

    return 0;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  bool
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::blockable_client_transports_i (
    Connection_Handler_Set &h)
  {
    HASH_MAP_ITER end_iter = this->cache_map_.end ();

    for (HASH_MAP_ITER iter = this->cache_map_.begin ();
         iter != end_iter;
         ++iter)
      {
        // Get the transport to fill its associated connection's
        // handler.
        bool const retval =
          (*iter).int_id_.transport ()->provide_blockable_handler (h);

        // Do not mark the entry as closed if we don't have a
        // blockable handler added
        if (retval)
          (*iter).int_id_.recycle_state (ENTRY_CLOSED);
      }

    return true;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::purge_entry_i (const HASH_MAP_ENTRY_REF &entry)
  {
    // Remove the entry from the Map
    Cache_ExtId ext_id = entry.entry_->key();
    Cache_IntId int_id = *entry.int_id_;
    int retval = this->cache_map_.unbind (ext_id, int_id);

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
    this->size_monitor_->receive (this->current_size ());
#endif /* TAO_HAS_MONITOR_POINTS==1 */

    return retval;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  bool
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::is_entry_available_i (const Cache_IntId& int_id)
  {
    Cache_Entries_State entry_state = int_id.recycle_state ();
    bool result = (entry_state == ENTRY_IDLE_AND_PURGABLE);

    if (result && int_id.transport () != 0)
    {
      // if it's not connected, it's not available
      result = int_id.is_connected ();
    }

    if (TAO_debug_level > 8)
      {
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::")
                    ACE_TEXT ("is_entry_available_i[%d], %C, state is %C\n"),
                    int_id.transport () ? int_id.transport ()->id () : 0,
                    (result ? "true" : "false"),
                    Cache_IntId::state_name (entry_state)));
      }

    return result;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  bool
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::is_entry_purgable_i (
    const HASH_MAP_ENTRY_REF &entry)
  {
    Cache_Entries_State entry_state = entry.int_id_->recycle_state ();
    transport_type* transport = entry.int_id_->transport ();
    bool result = (entry_state == ENTRY_IDLE_AND_PURGABLE ||
                   entry_state == ENTRY_PURGABLE_BUT_NOT_IDLE)
                   && transport->can_be_purged ();

    if (TAO_debug_level > 8)
      {
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::")
                    ACE_TEXT ("is_entry_purgable_i[%d], %C, state is %C\n"),
                    entry.int_id_->transport ()->id (),
                    (result ? "true" : "false"),
                    Cache_IntId::state_name (entry_state)));
      }

    return result;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  bool
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::
    is_entry_connecting_i (const Cache_IntId& int_id)
  {
    Cache_Entries_State entry_state = int_id.recycle_state ();
    bool result = (entry_state == ENTRY_CONNECTING);

    if (!result && int_id.transport () != 0)
      {
        // if we're not connected, that counts, too.
        // Can this happen?  Not sure <wilsond@ociweb.com>
        result = !int_id.is_connected ();
      }

    if (TAO_debug_level > 8)
      {
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::")
                    ACE_TEXT ("is_entry_connecting_i[%d], %C, state is %C\n"),
                    int_id.transport () ? int_id.transport ()->id () : 0,
                    (result ? "true" : "false"),
                    Cache_IntId::state_name (entry_state)));
      }

    return result;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  bool
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::
    cpsless(const HASH_MAP_ENTRY_REF& left, const HASH_MAP_ENTRY_REF& right)
  {
    if (is_entry_purgable_i (left) && !is_entry_purgable_i (right))
      return true;
    if (!is_entry_purgable_i (left) && is_entry_purgable_i (right))
      return false;

    return left.int_id_->transport ()->purging_order () <
            right.int_id_->transport ()->purging_order ();
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::purge (void)
  {
    typedef ACE_Array<HASH_MAP_ENTRY_REF> transport_set_type;
    transport_set_type transports_to_be_closed;

    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->cache_lock_, 0));

      // Calculate the number of entries to purge
      int const amount = (this->current_size() * this->percent_) / 100;

      this->fill_set_i (transports_to_be_closed, amount);

      // Only call close_entries () if transports_to_be_closed is not empty,
      // otherwise there is nothing to de-allocate.
      if (transports_to_be_closed.size() != 0)
        {
          // BEGIN FORMER close_entries

          if (TAO_debug_level > 4)
            {
              TAOLIB_DEBUG ((LM_INFO,
                ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::purge, ")
                ACE_TEXT ("Trying to purge %d of %d cache entries\n"),
                amount,
                transports_to_be_closed.size ()));
            }
          for (typename transport_set_type::iterator it =
                 transports_to_be_closed.begin();
               it != transports_to_be_closed.end(); ++it)
            {
              (*it).int_id_->recycle_state (ENTRY_BUSY);
              transport_type* transport = (*it).int_id_->transport ();
              transport->add_reference ();
              if (TAO_debug_level > 4)
                {
                  TAOLIB_DEBUG ((LM_INFO,
                        ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::purge, ")
                        ACE_TEXT ("Purgable Transport[%d] found in ")
                        ACE_TEXT ("cache\n"),
                        transport->id ()));
                }
            }
          // END FORMER close_entries
        }
    }

    // Now, without the lock held, lets go through and close all the transports.
    for (typename transport_set_type::iterator it =
           transports_to_be_closed.begin();
         it != transports_to_be_closed.end(); ++it)
      {
            transport_type *transport = (*it).int_id_->transport ();

            if (transport)
              {
                transport->close_connection ();
                transport->remove_reference ();
              }
      }

    if (TAO_debug_level > 4)
      {
        TAOLIB_DEBUG ((LM_INFO,
          ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::purge, ")
          ACE_TEXT ("Cache size after purging is [%d]\n"),
          this->current_size ()
          ));
      }

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
    /// The value doesn't matter, all we need is the timestamp,
    /// which is added automatically.
    this->purge_monitor_->receive (static_cast<size_t> (0UL));
    /// And update the size monitor as well.
    this->size_monitor_->receive (this->current_size ());
#endif /* TAO_HAS_MONITOR_POINTS==1 */

    return 0;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  void
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::
    fill_set_i (ACE_Array<HASH_MAP_ENTRY_REF>& entries, int amount)
  {
    int current_size = 0;
    int const cache_maximum = this->purging_strategy_->cache_maximum ();

    // set entries to 0.  This signifies nothing to purge.
    entries.size (0);

    // Do we need to worry about cache purging?
    if (cache_maximum >= 0)
      {
        current_size = static_cast<int> (this->current_size ());

        if (TAO_debug_level > 6)
          {
            TAOLIB_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::fill_set_i, ")
              ACE_TEXT ("current_size = %d, cache_maximum = %d\n"),
              current_size, cache_maximum));
          }

        if (current_size >= cache_maximum)
          {
            if (entries.size(current_size) < 0)
              return;

            HASH_MAP_ITER iter = this->cache_map_.begin ();

            for (int i = 0; i < current_size; ++i)
              {
                ACE_Unbounded_Set<Cache_IntId>& int_ids = (*iter).item();
                for (typename ACE_Unbounded_Set<Cache_IntId>::iterator it =
                       int_ids.begin(); it != int_ids.end(); ++it)
                  {
                    HASH_MAP_ENTRY_REF ref;
                    ref.entry_ = &*iter;
                    ref.int_id_ = &*it;
                    entries[i] = ref;
                  }
                ++iter;
              }

#if !defined (ACE_LACKS_STD_PARTIAL_SORT)
           std::partial_sort (
              entries.begin(),
              entries.begin() + amount,
              entries.end(),
              &cpsless);
#else
           // Use (partial) selection sort if we don't have std::partial_sort
           for (int i = 0; i < amount; ++i)
             {
               int m = i;
               for (int j = i + 1; j < current_size; ++j)
                 {
                   if (cpsless (entries[j], entries[m]))
                     m = j;
                 }
               if (m != i)
                 std::swap (entries[m], entries[i]);
             }
#endif // ACE_LACKS_STD_PARTIAL_SORT
            entries.size (amount);
          }
      }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
