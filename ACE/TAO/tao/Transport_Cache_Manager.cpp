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


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Transport_Cache_Manager::Transport_Cache_Manager (TAO_ORB_Core &orb_core)
    : percent_ (orb_core.resource_factory ()->purge_percentage ())
    , purging_strategy_ (orb_core.resource_factory ()->create_purging_strategy ())
    , cache_map_ (orb_core.resource_factory ()->cache_maximum ())
    , condition_ (0)
    , cache_lock_ (0)
    , muxed_number_ (orb_core.resource_factory ()->max_muxed_connections ())
    , no_waiting_threads_ (0)
    , last_entry_returned_ (0)
  {
    if (orb_core.resource_factory ()->locked_transport_cache ())
      {
        ACE_NEW (this->condition_,
                 TAO_Condition <TAO_SYNCH_MUTEX>);

        ACE_NEW (this->cache_lock_,
                 ACE_Lock_Adapter <TAO_SYNCH_MUTEX> (*this->condition_->mutex ()));
      }
    else
      {
        /// If the cache is not going to be locked then dont create a
        /// condition variable. Make the <muxed_number_> to 0, else a
        /// single thread could get into waiting mode
        this->muxed_number_ = 0;
        ACE_NEW (this->cache_lock_,
                 ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX>);
      }
  }

  Transport_Cache_Manager::~Transport_Cache_Manager (void)
  {
    // Wakeup all the waiting threads threads before we shutdown stuff
    if (this->no_waiting_threads_)
      {
        this->condition_->broadcast ();
      }

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

    // Delete the condition variable
    if (this->condition_)
      {
        delete this->condition_;
        this->condition_ = 0;
      }
  }


  int
  Transport_Cache_Manager::bind_i (Cache_ExtId &ext_id,
                                   Cache_IntId &int_id)
  {
    if (TAO_debug_level > 0)
       {
         ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Transport_Cache_Manager::bind_i, "
                    "0x%x -> 0x%x Transport[%d]\n",
                     ext_id.property (),
                    int_id.transport (),
                    int_id.transport ()->id ()));
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
        if (TAO_debug_level > 4)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - Transport_Cache_Manager::bind_i, "
                        "unable to bind in the first attempt. "
                        "Trying with a new index\n"));
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

    if (TAO_debug_level > 5 && retval != 0)
      {
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - Transport_Cache_Manager::bind_i, "
                    "unable to bind\n"));
      }
    else if (TAO_debug_level > 3)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Transport_Cache_Manager::bind_i, "
                    "cache size is [%d]\n",
                    this->current_size ()));
      }

    return retval;
  }

  int
  Transport_Cache_Manager::find_transport (
    TAO_Transport_Descriptor_Interface *prop,
    TAO_Transport *&transport)
  {
    if (prop == 0)
      {
        transport = 0;
        return -1;
      }

    // Compose the ExternId
    Cache_ExtId ext_id (prop);
    Cache_IntId int_id;

    int retval = this->find (ext_id,
                             int_id);
    if (retval == 0)
      {
        transport = int_id.relinquish_transport ();

        if (transport->wait_strategy ()->non_blocking () == 0 &&
            transport->orb_core ()->client_factory ()->use_cleanup_options ())
          {
            ACE_Event_Handler *eh =
              transport->event_handler_i ();

            ACE_Reactor *r =
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

    return retval;
  }

  int
  Transport_Cache_Manager::find (const Cache_ExtId &key,
                                 Cache_IntId &value)
  {
    ACE_MT (ACE_GUARD_RETURN  (ACE_Lock,
                               guard,
                               *this->cache_lock_,
                               -1));

    int status =  this->find_i (key,
                                value);

    if (status == 0)
      {
        // Update the purging strategy information while we
        // are holding our lock
        this->purging_strategy_->update_item (value.transport ());
      }

    return status;
  }

  int
  Transport_Cache_Manager::find_i (const Cache_ExtId &key,
                                   Cache_IntId &value)
  {
    HASH_MAP_ENTRY *entry = 0;

    // Get the entry from the Hash Map
    int retval = 0;

    // Make a temporary object. It does not do a copy.
    Cache_ExtId tmp_key (key.property ());

    while (retval == 0)
      {
        // Wait for a connection..
        this->wait_for_connection (tmp_key);

        // Look for an entry in the map
        retval = this->cache_map_.find (tmp_key,
                                        entry);

        // We have an entry in the map, check whether it is idle.
        if (entry)
          {
            CORBA::Boolean idle =
              this->is_entry_idle (entry);

            if (idle)
              {
                // Successfully found a TAO_Transport.

                entry->int_id_.recycle_state (ENTRY_BUSY);

                // NOTE: This assignment operator indirectly incurs two
                //       lock operations since it duplicates and releases
                //       TAO_Transport objects.
                value = entry->int_id_;

                if (TAO_debug_level > 4)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                ACE_TEXT("TAO (%P|%t) - Transport_Cache_Manager::find_i, ")
                                ACE_TEXT("at index %d (Transport[%d]) - idle\n"),
                                entry->ext_id_.index (),
                                entry->int_id_.transport ()->id ()));
                  }

                return 0;
              }
            else if (TAO_debug_level > 6)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                ACE_TEXT("TAO (%P|%t) - Transport_Cache_Manager::find_i, ")
                                ACE_TEXT("at index %d (Transport[%d]) - not idle\n"),
                                entry->ext_id_.index (),
                                entry->int_id_.transport ()->id ()));
                  }
          }

        // Bump the index up
        tmp_key.incr_index ();
      }

    // If we are here then it is an error
    if (TAO_debug_level > 4 && retval != 0)
      {
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - Transport_Cache_Manager::find_i, "
                    "no idle transport is available\n"));
      }

    return retval;
  }

  int
  Transport_Cache_Manager::make_idle_i (HASH_MAP_ENTRY *&entry)
  {
    if (entry == 0)
      return -1;

    entry->int_id_.recycle_state (ENTRY_IDLE_AND_PURGABLE);

    // Does any one need waking?
    if (this->no_waiting_threads_)
      {
        // We returned this entry to the map
        this->last_entry_returned_ = &entry->ext_id_;

        // Wake up a thread
        this->condition_->signal ();
      }

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
    (void) st->update_item (entry->int_id_.transport ());

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
  Transport_Cache_Manager::mark_invalid_i (HASH_MAP_ENTRY *&entry)
  {
    if (entry == 0)
      {
        return;
      }

    // Mark the entry as not usable
    entry->int_id_.recycle_state (ENTRY_PURGABLE_BUT_NOT_IDLE);
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
  Transport_Cache_Manager::is_entry_idle (HASH_MAP_ENTRY *&entry)
  {
    Cache_Entries_State entry_state =
      entry->int_id_.recycle_state ();

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) - Transport_Cache_Manager::is_entry_idle, ")
                    ACE_TEXT("state is [%d]\n"),
                    entry_state));
      }

    if (entry_state == ENTRY_IDLE_AND_PURGABLE ||
        entry_state == ENTRY_IDLE_BUT_NOT_PURGABLE)
      return true;

    return false;
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

          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager::")
                          ACE_TEXT ("purge,  purging %d of %d cache entries\n"),
                          amount,
                          sorted_size));
            }

          int count = 0;

          for (int i = 0; count < amount && i < sorted_size; ++i)
            {
              if (this->is_entry_idle (sorted_set[i]))
                {
                  sorted_set[i]->int_id_.recycle_state (ENTRY_BUSY);

                  TAO_Transport* transport =
                    sorted_set[i]->int_id_.transport ();
                  transport->add_reference ();

                  if (transports_to_be_closed.push (transport) != 0)
                    {
                      ACE_DEBUG ((LM_INFO,
                                  ACE_TEXT ("TAO (%P|%t) - ")
                                  ACE_TEXT ("Unable to push transport %u ")
                                  ACE_TEXT ("on the to-be-closed stack, so ")
                                  ACE_TEXT ("it will leak\n"),
                                transport->id ()));
                    }

                  if (TAO_debug_level > 0)
                    {
                      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                                  ACE_TEXT ("Idle transport found in ")
                                  ACE_TEXT ("cache: [%d] \n"),
                                  transport->id ()));
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
                  entry->int_id_.transport ()->purging_order (); --j)
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

        if (TAO_debug_level > 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("TAO (%P|%t) - Transport_Cache_Manager::fill_set_i, ")
                        ACE_TEXT("current_size = %d, cache_maximum = %d\n"),
                        current_size, cache_maximum));
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


  int
  Transport_Cache_Manager::wait_for_connection (Cache_ExtId &extid)
  {
    if (this->muxed_number_ && this->muxed_number_ == extid.index ())
      {
        // If we have a limit on the number of muxed connections for
        // a particular endpoint just wait to get the connection
        ++this->no_waiting_threads_;

        if (TAO_debug_level > 2)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("TAO (%P|%t) - Transport_Cache_Manager")
                        ACE_TEXT("::wait_for_connection, ")
                        ACE_TEXT("entering wait loop\n")));
          }

        int ready_togo = 0;

        while (ready_togo == 0)
          {
            this->condition_->wait ();

            // Check whether we are waiting for this connection
            ready_togo = this->is_wakeup_useful (extid);
          }

        if (TAO_debug_level > 2)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("TAO (%P|%t) - Transport_Cache_Manager::wait_for_connection, ")
                        ACE_TEXT("left wait loop\n")));
          }

        // We are not waiting anymore
        --this->no_waiting_threads_;
      }

    return 0;
  }

  int
  Transport_Cache_Manager::is_wakeup_useful (Cache_ExtId &extid)
  {
    // Get the underlying property that we are looking for
    TAO_Transport_Descriptor_Interface *prop = extid.property ();

    // Just check the underlying property for equivalence. If the last
    // connection that was returned  had the same property just return
    // 1.
    if (this->last_entry_returned_ &&
        prop->is_equivalent (this->last_entry_returned_->property ()))
      {
        // Set the index to be right so that we can pick teh connection
        // right away..
        extid.index (this->last_entry_returned_->index ());

        // There is no more use for it ...
        this->last_entry_returned_ = 0;

        return 1;
      }

    // If there  is an entry that was returned and if there are more
    // threads just wake up the peer to check for the returned
    // connection.
    if (this->last_entry_returned_ &&
        this->no_waiting_threads_ > 1)
      {
        this->condition_->signal ();
      }

    return 0;
  }

}

TAO_END_VERSIONED_NAMESPACE_DECL
