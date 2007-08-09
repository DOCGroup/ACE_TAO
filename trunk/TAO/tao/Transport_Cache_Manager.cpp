//$Id$
#include "tao/Transport_Cache_Manager.h"
#include "tao/Transport.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Connection_Purging_Strategy.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Condition.h"
#include "tao/Wait_Strategy.h"
#include "ace/ACE.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
# include "tao/Transport_Cache_Manager.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID (tao,
           Transport_Cache_Manager,
           "$Id$")

// notes on debug level for transport cache
// TAO_debug_level > 0: recoverable error condition (LM_ERROR)
// TAO_debug_level > 4: normal transport cache operations (LM_INFO)
// TAO_debug_level > 6: detailed cache operations (LM_DEBUG)
// TAO_debug_level > 8: for debugging the cache itself


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Transport_Cache_Manager::Transport_Cache_Manager (TAO_ORB_Core &orb_core)
    : percent_ (orb_core.resource_factory ()->purge_percentage ())
    , purging_strategy_ (orb_core.resource_factory ()->create_purging_strategy ())
    , cache_map_ (orb_core.resource_factory ()->cache_maximum ())
    , cache_lock_ (0)
  {
    if (orb_core.resource_factory ()->locked_transport_cache ())
      {
        ACE_NEW (this->cache_lock_,
                 ACE_Lock_Adapter <TAO_SYNCH_MUTEX> (this->cache_map_mutex_));
      }
    else
      {
        ACE_NEW (this->cache_lock_,
                 ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX>);
      }
  }

  Transport_Cache_Manager::~Transport_Cache_Manager (void)
  {
    // Delete the lock that we have
    if (this->cache_lock_)
      {
        delete this->cache_lock_;
        this->cache_lock_ = 0;
      }

    // Delete the purging strategy
    if (this->purging_strategy_)
      {
        delete this->purging_strategy_;
        this->purging_strategy_ = 0;
      }

  }


  int
  Transport_Cache_Manager::bind_i (Cache_ExtId &ext_id,
                                   Cache_IntId &int_id)
  {
    if (TAO_debug_level > 4)
       {
         ACE_DEBUG ((LM_INFO,
            ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::bind_i: ")
            ACE_TEXT (" Transport[%d] @ hash:index{%d:%d}\n"),
            int_id.transport ()->id (),
            ext_id.hash (),
            ext_id.index ()
            ));
       }

    // Get the entry too
    HASH_MAP_ENTRY *entry = 0;

    // Update the purging strategy information while we
    // are holding our lock
    this->purging_strategy_->update_item (int_id.transport ());

    int retval = this->cache_map_.bind (ext_id,
                                        int_id,
                                        entry);
    if (retval == 0)
      {
        // The entry has been added to cache succesfully
        // Add the cache_map_entry to the transport
        int_id.transport ()->cache_map_entry (entry);
      }
    else if (retval == 1)
      {
        // if this is already in the cache, just ignore the request
        // this happens because some protocols bind their transport early
        // to avoid duplication simultaneous connection attempts
        if (entry != 0 && entry->item ().transport () == int_id.transport ())
          {
            // rebind this entry to update cache status
            retval = this->cache_map_.rebind (ext_id,
                                        int_id,
                                        entry);
            if(retval == 1) // 1 from rebind means replaced
              {
                retval = 0;
              }
          }
        else
        {
          if (TAO_debug_level > 4)
            {
              ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::bind_i: ")
                ACE_TEXT ("Unable to bind in the first attempt. ")
                ACE_TEXT ("Trying with a new index\n")
                ));
            }

          // There was an entry like this before, so let us do some
          // minor adjustments and rebind
          retval = this->get_last_index_bind (ext_id,
                                              int_id,
                                              entry);
          if (retval == 0)
            {
              int_id.transport ()->cache_map_entry (entry);
            }
        }
      }

    if (retval != 0)
      {
        if (TAO_debug_level > 0)
          {
            ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) - Transport_Cache_Manager::bind_i, "
                      "unable to bind\n"));
          }
      }
    else if (TAO_debug_level > 4)
      {
        ACE_DEBUG ((LM_INFO,
          ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::bind_i: ")
          ACE_TEXT ("Cache size is [%d]\n"),
          this->current_size ()
          ));
      }

    return retval;
  }

  Transport_Cache_Manager::Find_Result
  Transport_Cache_Manager::find_transport (
    TAO_Transport_Descriptor_Interface *prop,
    TAO_Transport *&transport,
    size_t &busy_count)
  {
    if (prop == 0)
      {
        transport = 0;
        return CACHE_FOUND_NONE;
      }

    // Compose the ExternId
    Cache_ExtId ext_id (prop);
    Cache_IntId int_id;

    Transport_Cache_Manager::Find_Result find_result = this->find (ext_id,
                                   int_id, busy_count);
    if (find_result != CACHE_FOUND_NONE)
      {
        transport = int_id.relinquish_transport ();
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
                      ACE_ERROR ((LM_ERROR,
                                  ACE_TEXT ("TAO (%P|%t) - TAO_Transport_Cache_Manager[%d]")
                                  ACE_TEXT ("::find_transport, remove_handler failed \n"),
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

  Transport_Cache_Manager::Find_Result
  Transport_Cache_Manager::find (const Cache_ExtId &key,
                                 Cache_IntId &value,
                                 size_t &busy_count)
  {
    ACE_MT (ACE_GUARD_RETURN  (ACE_Lock,
                               guard,
                               *this->cache_lock_,
                               Transport_Cache_Manager::CACHE_FOUND_NONE));

    Transport_Cache_Manager::Find_Result status =  this->find_i (key,
                                      value, busy_count);

    if (status != CACHE_FOUND_NONE)
      {
        // Update the purging strategy information while we
        // are holding our lock
        this->purging_strategy_->update_item (value.transport ());
      }

    return status;
  }

  Transport_Cache_Manager::Find_Result
  Transport_Cache_Manager::find_i (const Cache_ExtId &key,
                                   Cache_IntId &value,
                                   size_t & busy_count)
  {
    HASH_MAP_ENTRY *entry = 0;
    busy_count = 0;

    // Get the entry from the Hash Map
    Transport_Cache_Manager::Find_Result found = CACHE_FOUND_NONE;

    // Make a temporary object. It does not do a copy.
    Cache_ExtId tmp_key (key.property ());
    int cache_status = 0;

    // loop until we find a usable transport, or until we've checked
    // all cached entries for this endpoint
    while (found != CACHE_FOUND_AVAILABLE && cache_status == 0)
      {
        entry = 0;
        cache_status  = this->cache_map_.find (tmp_key,
                                        entry);
        if (cache_status == 0 && entry)
          {
            if (this->is_entry_available (*entry))
              {
                // Successfully found a TAO_Transport.

                found = CACHE_FOUND_AVAILABLE;
                entry->item ().recycle_state (ENTRY_BUSY);

                // NOTE: This assignment operator indirectly incurs two
                //       lock operations since it duplicates and releases
                //       TAO_Transport objects.
                value = entry->item ();

                if (TAO_debug_level > 6)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::find_i: ")
                      ACE_TEXT ("Found available Transport[%d] @hash:index {%d:%d}(\n"),
                      entry->item ().transport ()->id (),
                      entry->ext_id_.hash (),
                      entry->ext_id_.index ()
                      ));
                  }
              }
            else if (this->is_entry_connecting (*entry))
              {
                if (TAO_debug_level > 6)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::find_i: ")
                      ACE_TEXT ("Found connecting Transport[%d] @hash:index {%d:%d}\n"),
                      entry->item ().transport ()->id (),
                      entry->ext_id_.hash (),
                      entry->ext_id_.index ()
                      ));
                  }
                // if this is the first interesting entry
                if (found != CACHE_FOUND_CONNECTING)
                  {
                    // NOTE: This assignment operator indirectly incurs two
                    //       lock operations since it duplicates and releases
                    //       TAO_Transport objects.
                    value = entry->item ();
                    found = CACHE_FOUND_CONNECTING;
                  }
              }
            else
              {
                // if this is the first busy entry
                if (found == CACHE_FOUND_NONE && busy_count == 0)
                  {
                    value = entry->item ();
                    found = CACHE_FOUND_BUSY;
                  }
                busy_count += 1;
                if (TAO_debug_level > 6)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::find_i: ")
                      ACE_TEXT ("Found busy Transport[%d] @hash:index {%d:%d}\n"),
                      entry->item ().transport ()->id (),
                      entry->ext_id_.hash (),
                      entry->ext_id_.index ()
                      ));
                  }
              }
          }

        // Bump the index up
        tmp_key.incr_index ();
      }

    if (TAO_debug_level > 6 && found != CACHE_FOUND_AVAILABLE)
      {
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - Transport_Cache_Manager::find_i, "
                    "no idle transport is available for hash {%d}\n",
                    tmp_key.hash ()
                    ));
      }

    return found;
  }

  int
  Transport_Cache_Manager::make_idle_i (HASH_MAP_ENTRY *entry)
  {
    if (entry == 0)
      return -1;

    entry->item ().recycle_state (ENTRY_IDLE_AND_PURGABLE);

    return 0;
  }

  int
  Transport_Cache_Manager::update_entry (HASH_MAP_ENTRY *&entry)
  {
    if(entry == 0)
      return -1;

    ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                              guard,
                            *this->cache_lock_, -1));

    if (entry == 0)
      return -1;

    TAO_Connection_Purging_Strategy *st = this->purging_strategy_;
    (void) st->update_item (entry->item ().transport ());

    return 0;
  }

  int
  Transport_Cache_Manager::close_i (Connection_Handler_Set &handlers)
  {
    HASH_MAP_ITER end_iter = this->cache_map_.end ();

    for (HASH_MAP_ITER iter = this->cache_map_.begin ();
         iter != end_iter;
         ++iter)
      {
        // Get the transport to fill its associated connection's handler.
        (*iter).int_id_.transport ()->provide_handler (handlers);

        // Inform the transport that has a reference to the entry in the
        // map that we are *gone* now. So, the transport should not use
        // the reference to the entry that he has, to acces us *at any
        // time*.
        (*iter).int_id_.transport ()->cache_map_entry (0);
      }

    // Unbind all the entries in the map
    this->cache_map_.unbind_all ();

    return 0;
  }

  bool
  Transport_Cache_Manager::blockable_client_transports_i (
    Connection_Handler_Set &h)
  {
    HASH_MAP_ITER end_iter = this->cache_map_.end ();

    for (HASH_MAP_ITER iter = this->cache_map_.begin ();
         iter != end_iter;
         ++iter)
      {
        // Get the transport to fill its associated connection's
        // handler.
        bool retval =
          (*iter).int_id_.transport ()->provide_blockable_handler (h);

        // Do not mark the entry as closed if we don't have a
        // blockable handler added
        if (retval)
          (*iter).int_id_.recycle_state (ENTRY_CLOSED);
      }

    return true;
  }

  int
  Transport_Cache_Manager::purge_entry_i (HASH_MAP_ENTRY *&entry)
  {
    if (entry == 0)
      {
        return 0;
      }

    // Remove the entry from the Map
    int retval = this->cache_map_.unbind (entry);

    // Set the entry pointer to zero
    entry = 0;

    return retval;
  }

  void
  Transport_Cache_Manager::mark_invalid_i (HASH_MAP_ENTRY *entry)
  {
    if (entry == 0)
      {
        return;
      }

    // Mark the entry as not usable
    entry->item ().recycle_state (ENTRY_PURGABLE_BUT_NOT_IDLE);
  }

  int
  Transport_Cache_Manager::get_last_index_bind (Cache_ExtId &key,
                                                Cache_IntId &val,
                                                HASH_MAP_ENTRY *&entry)
  {
    CORBA::ULong ctr = entry->ext_id_.index ();
    int retval = 0;

    while (retval == 0)
      {
        // Set the index
        key.index (++ctr);

        // Check to see if an element exists in the Map. If it exists we
        // loop, else we drop out of the loop
        retval = this->cache_map_.find (key);
      }

    // Now do a bind again with the new index
    return  this->cache_map_.bind (key,
                                   val,
                                   entry);
  }


  bool
  Transport_Cache_Manager::is_entry_available (const HASH_MAP_ENTRY &entry)
  {
    Cache_Entries_State entry_state =
      entry.int_id_.recycle_state ();
    bool result = (entry_state == ENTRY_IDLE_AND_PURGABLE ||
        entry_state == ENTRY_IDLE_BUT_NOT_PURGABLE);

    if (result && entry.int_id_.transport () != 0)
    {
      // if it's not connected, it's not available
      result = entry.int_id_.transport ()->is_connected();
    }

    if (TAO_debug_level > 8)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::is_entry_available:")
          ACE_TEXT ("returns %s state is [%d]\n"),
          (result?"True":"False"),
          entry_state));
      }

    return result;
  }

  bool
  Transport_Cache_Manager::is_entry_connecting (const HASH_MAP_ENTRY &entry)
  {
    Cache_Entries_State entry_state =
      entry.int_id_.recycle_state ();
    bool result = (entry_state == ENTRY_CONNECTING);
    if (!result && entry.int_id_.transport () != 0)
    {
      // if we're not connected, that counts, too.
      // Can this happen?  Not sure <wilsond@ociweb.com>
      result = !entry.int_id_.transport ()->is_connected();
    }

    if (TAO_debug_level  > 8)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::is_entry_connecting: ")
          ACE_TEXT ("Returns %s, state is [%d]\n"),
          (result?"True":"False"),
          entry_state));
      }

    return result;
  }

#if !defined (ACE_LACKS_QSORT)
  int
  Transport_Cache_Manager::cpscmp(const void* a, const void* b)
  {
    const HASH_MAP_ENTRY** left  = (const HASH_MAP_ENTRY**)a;
    const HASH_MAP_ENTRY** right = (const HASH_MAP_ENTRY**)b;

    if ((*left)->int_id_.transport ()->purging_order () <
        (*right)->int_id_.transport ()->purging_order ())
      return -1;

    if ((*left)->int_id_.transport ()->purging_order () >
        (*right)->int_id_.transport ()->purging_order ())
      return 1;

    return 0;
  }
#endif /* ACE_LACKS_QSORT */

  int
  Transport_Cache_Manager::purge (void)
  {
    ACE_Unbounded_Stack<TAO_Transport*> transports_to_be_closed;

    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->cache_lock_, 0));

      DESCRIPTOR_SET sorted_set = 0;
      int sorted_size = this->fill_set_i (sorted_set);

      // Only call close_entries () if sorted_set != 0.  It takes
      // control of sorted_set and cleans up any allocated memory.  If
      // sorted_set == 0, then there is nothing to de-allocate.
      if (sorted_set != 0)
        {
          // BEGIN FORMER close_entries
          // Calculate the number of entries to purge
          const int amount = (sorted_size * this->percent_) / 100;

          if (TAO_debug_level > 4)
            {
              ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::purge: ")
                ACE_TEXT ("Purging %d of %d cache entries\n"),
                amount,
                sorted_size));
            }

          int count = 0;

          for (int i = 0; count < amount && i < sorted_size; ++i)
            {
              if (this->is_entry_available (*sorted_set[i]))
                {
                  sorted_set[i]->int_id_.recycle_state (ENTRY_BUSY);

                  TAO_Transport* transport =
                    sorted_set[i]->int_id_.transport ();
                  transport->add_reference ();

                  if (TAO_debug_level > 4)
                    {
                      ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("TAO (%P|%t) Transport_Cache_Manager::purge ")
                        ACE_TEXT ("Purgable Transport[%d] found in ")
                        ACE_TEXT ("cache\n"),
                        transport->id ()));
                    }

                  if (transports_to_be_closed.push (transport) != 0)
                    {
                      if (TAO_debug_level > 0)
                        {
                          ACE_DEBUG ((LM_ERROR,
                            ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::purge: ")
                            ACE_TEXT ("Unable to push transport[%u] ")
                            ACE_TEXT ("on the to-be-closed stack, so ")
                            ACE_TEXT ("it will leak\n"),
                            transport->id ()));
                        }
                    }

                  // Count this as a successful purged entry
                  count++;
                }
            }

          delete [] sorted_set;
          sorted_set = 0;
          // END FORMER close_entries
        }
    }

    // Now, without the lock held, lets go through and close all the transports.
    TAO_Transport *transport = 0;

    while (! transports_to_be_closed.is_empty ())
      {
        if (transports_to_be_closed.pop (transport) == 0)
          {
            if (transport)
              {
                transport->close_connection ();
                transport->remove_reference ();
              }
          }
      }
    if (TAO_debug_level > 4)
          {
            ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::purge: ")
              ACE_TEXT ("Cache size after purging is [%d]\n"),
              this->current_size ()
              ));
          }

    return 0;
  }


  void
  Transport_Cache_Manager::sort_set (DESCRIPTOR_SET& entries,
                                     int current_size)
  {
#if defined (ACE_LACKS_QSORT)
    // Use insertion sort if we don't have qsort
    for(int i = 1; i < current_size; ++i)
      {
        if (entries[i]->int_id_.transport ()->purging_order () <
            entries[i - 1]->int_id_.transport ()->purging_order ())
          {
            HASH_MAP_ENTRY* entry = entries[i];

            for(int j = i; j > 0 &&
                  entries[j - 1]->int_id_.transport ()->purging_order () >
                  entry->item ().transport ()->purging_order (); --j)
              {
                HASH_MAP_ENTRY* holder = entries[j];
                entries[j] = entries[j - 1];
                entries[j - 1] = holder;
              }
          }
      }
#else
    ACE_OS::qsort (entries, current_size,
                   sizeof (HASH_MAP_ENTRY*), (ACE_COMPARE_FUNC)cpscmp);
#endif /* ACE_LACKS_QSORT */
  }


  int
  Transport_Cache_Manager::fill_set_i (DESCRIPTOR_SET& sorted_set)
  {
    int current_size = 0;
    int cache_maximum = this->purging_strategy_->cache_maximum ();

    // set sorted_set to 0.  This signifies nothing to purge.
    sorted_set = 0;

    // Do we need to worry about cache purging?
    if (cache_maximum >= 0)
      {
        current_size = static_cast<int> (this->cache_map_.current_size ());

        if (TAO_debug_level > 6)
          {
            ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::fill_set_i: ")
              ACE_TEXT ("current_size = %d, cache_maximum = %d\n"),
              current_size,
              cache_maximum
              ));
          }

        if (current_size >= cache_maximum)
          {
            ACE_NEW_RETURN (sorted_set, HASH_MAP_ENTRY*[current_size], 0);

            HASH_MAP_ITER iter = this->cache_map_.begin ();

            for (int i = 0; i < current_size; ++i)
              {
                sorted_set[i] = &(*iter);
                iter++;
              }

            this->sort_set (sorted_set, current_size);
          }
    }

    return current_size;
  }

}

TAO_END_VERSIONED_NAMESPACE_DECL
