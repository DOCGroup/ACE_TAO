#include "tao/Transport_Cache_Manager.h"
#include "tao/Transport.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Resource_Factory.h"
#include "tao/Connection_Purging_Strategy.h"
#include "tao/Condition.h"
#include "ace/Synch_T.h"
#include "ace/Handle_Set.h"

#if !defined (__ACE_INLINE__)
# include "tao/Transport_Cache_Manager.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID (TAO,
           Transport_Cache_Manager,
           "$Id$")


TAO_Transport_Cache_Manager::TAO_Transport_Cache_Manager (TAO_ORB_Core &orb_core)
  : percent_ (orb_core.resource_factory ()->purge_percentage ()),
    purging_strategy_ (orb_core.resource_factory ()->create_purging_strategy ()),
    cache_map_ (ACE_static_cast (size_t, ACE::max_handles ())),
    condition_ (0),
    cache_lock_ (0),
    muxed_number_ (orb_core.resource_factory ()->max_muxed_connections ()),
    no_waiting_threads_ (0),
    last_entry_returned_ (0)
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

TAO_Transport_Cache_Manager::~TAO_Transport_Cache_Manager (void)
{
  // Wakeup all the waiting threads threads before we shutdown stuff
  if (this->no_waiting_threads_)
    this->condition_->broadcast ();

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
TAO_Transport_Cache_Manager::bind_i (TAO_Cache_ExtId &ext_id,
                                     TAO_Cache_IntId &int_id)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("TAO_Transport_Cache_Manager")
                  ACE_TEXT ("::bind_i (0x%x, 0x%x)\n"),
                  ext_id.property (), int_id.transport ()));
    }

  // Get the entry too
  HASH_MAP_ENTRY *entry = 0;

  // Update the purging strategy information while we
  // are holding our lock
  this->purging_strategy_->update_item (int_id.transport ());

  // When it comes for bind we know the transport is going to be busy
  // and is marked for a partcular thread. So, mark it busy
  int_id.recycle_state (ACE_RECYCLABLE_BUSY);

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
      if (TAO_debug_level > 4 && retval != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) TAO_Transport_Cache_Manager::bind_i")
                      ACE_TEXT (" unable to bind in the first attempt \n")
                      ACE_TEXT (" So trying with a new index\n")));
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
                  ACE_TEXT ("(%P|%t) TAO_Transport_Cache_Manager::bind_i")
                  ACE_TEXT (" unable to bind \n")));
    }

  return retval;
}

int
TAO_Transport_Cache_Manager::find_transport (
    TAO_Transport_Descriptor_Interface *prop,
    TAO_Transport *&transport)
{
  if (prop == 0)
    {
      transport = 0;
      return -1;
    }

  // Compose the ExternId
  TAO_Cache_ExtId ext_id (prop);
  TAO_Cache_IntId int_id;

  int retval = this->find (ext_id,
                           int_id);
  if (retval == 0)
    {
      transport = int_id.relinquish_transport ();
    }

  return retval;
}

int
TAO_Transport_Cache_Manager::find (const TAO_Cache_ExtId &key,
                                    TAO_Cache_IntId &value)
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
TAO_Transport_Cache_Manager::find_i (const TAO_Cache_ExtId &key,
                                     TAO_Cache_IntId &value)
{
  HASH_MAP_ENTRY *entry = 0;

  // Get the entry from the Hash Map
  int retval = 0;

  // Make a temporary object. It does not do a copy.
  TAO_Cache_ExtId tmp_key (key.property ());

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

              // NOTE: This assignment operator indirectly incurs two
              //       lock operations since it duplicates and releases
              //       TAO_Transport objects.
              value = entry->int_id_;

              if (TAO_debug_level > 4)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P |%t) index in find <%d>\n"),
                              entry->ext_id_.index ()));
                }
              return 0;
            }
        }

      // Bump the index up
      tmp_key.incr_index ();
    }

  // If we are here then it is an error
  if (TAO_debug_level > 4 && retval != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) TAO_Transport_Cache_Manager::find_i")
                  ACE_TEXT (" unable to locate a free connection\n")));
    }

  return retval;
}

int
TAO_Transport_Cache_Manager::make_idle_i (HASH_MAP_ENTRY *&entry)
{
  // First get the entry again (if at all things had changed in the
  // cache map in the mean time)

  // @todo: Is this required? Looks like a legacy one..

  HASH_MAP_ENTRY *new_entry = 0;
  int retval = this->cache_map_.find (entry->ext_id_,
                                      new_entry);
  if (retval == 0)
    {

      new_entry->int_id_.
        recycle_state (ACE_RECYCLABLE_IDLE_AND_PURGABLE);

      entry = new_entry;

      // Does any one need waking?
      if (this->no_waiting_threads_)
        {
          // We returned this entry to the map
          this->last_entry_returned_ = &new_entry->ext_id_;

          // Wake up a thread
          this->condition_->signal ();
        }
    }
  else if (TAO_debug_level > 0 && retval != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) TAO_Transport_Cache_Manager::make_idle_i:\n")
                  ACE_TEXT ("(%P|%t)     unable to locate the entry to make it idle\n")));
    }

  return retval;
}


int
TAO_Transport_Cache_Manager::close_i (ACE_Handle_Set &reactor_registered,
                                      TAO_EventHandlerSet &unregistered)
{
  for (HASH_MAP_ITER iter = this->cache_map_.begin ();
       iter != this->cache_map_.end ();
       ++iter)
    {
      // First we look through whether we have an entry that has already
      // been closed.

      if ((*iter).int_id_.recycle_state () != ACE_RECYCLABLE_CLOSED)
        {
          // Get the transport to fill its associated connection's handle in
          // the handle sets.
          (*iter).int_id_.transport ()->provide_handle (reactor_registered,
                                                        unregistered);
          // Inform the transport that has a reference to the entry in the
          // map that we are *gone* now. So, the transport should not use
          // the reference to the entry that he has, to acces us *at any
          // time*.
          (*iter).int_id_.transport ()->cache_map_entry (0);
        }
     }

  // Unbind all the entries in the map
  this->cache_map_.unbind_all ();

  return 0;
}

int
TAO_Transport_Cache_Manager::purge_entry_i (HASH_MAP_ENTRY *&entry)
{
  int retval = 0;

  if (entry)
    {
      // Remove the entry from the Map
      retval =  this->cache_map_.unbind (entry);
    }

  // Set the entry pointer to zero
  entry = 0;

  return retval;
}

void
TAO_Transport_Cache_Manager::mark_invalid_i (HASH_MAP_ENTRY *&entry)
{
  if (entry)
    {
      // Mark the entry as not usable
      entry->int_id_.recycle_state (ACE_RECYCLABLE_PURGABLE_BUT_NOT_IDLE);
    }

}



int
TAO_Transport_Cache_Manager::get_last_index_bind (TAO_Cache_ExtId &key,
                                                  TAO_Cache_IntId &val,
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
      retval =
        this->cache_map_.find (key);
    }

  // Now do a bind again with the new index
  return  this->cache_map_.bind (key,
                                 val,
                                 entry);
}


int
TAO_Transport_Cache_Manager::is_entry_idle (HASH_MAP_ENTRY *&entry)
{
  if (entry->int_id_.recycle_state () == ACE_RECYCLABLE_IDLE_AND_PURGABLE ||
      entry->int_id_.recycle_state () == ACE_RECYCLABLE_IDLE_BUT_NOT_PURGABLE)
    {
      // Save that in the transport
      entry->int_id_.transport ()->cache_map_entry (entry);

      // Mark the connection as busy
      entry->int_id_.recycle_state (ACE_RECYCLABLE_BUSY);

      return 1;
    }

  return 0;
}


#if !defined (ACE_LACKS_QSORT)
int
TAO_Transport_Cache_Manager::cpscmp(const void* a, const void* b)
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


void
TAO_Transport_Cache_Manager::sort_set (DESCRIPTOR_SET& entries,
                                       int current_size)
{
#if defined (ACE_LACKS_QSORT)
  // Use insertion sort if we don't have qsort
  for(int i = 1; i < current_size; i++)
    {
      if (entries[i]->int_id_.transport ()->purging_order () <
                    entries[i - 1]->int_id_.transport ()->purging_order ())
        {
          HASH_MAP_ENTRY* entry = entries[i];
          for(int j = i; j > 0 &&
              entries[j - 1]->int_id_.transport ()->purging_order () >
              entry->int_id_.transport ()->purging_order (); j--)
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
TAO_Transport_Cache_Manager::fill_set_i (DESCRIPTOR_SET& sorted_set)
{
  int current_size = 0;
  int cache_maximum = this->purging_strategy_->cache_maximum ();

  // set sorted_set to 0.  This signifies nothing to purge.
  sorted_set = 0;

  // Do we need to worry about cache purging?
  if (cache_maximum >= 0)
    {
      current_size = this->cache_map_.current_size ();

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                                ACE_TEXT ("TAO_Transport_Cache_Manager")
                                ACE_TEXT ("::fill_set_i - %d %d\n"),
                                current_size, cache_maximum));
        }

      if (current_size >= cache_maximum)
        {
          ACE_NEW_RETURN (sorted_set, HASH_MAP_ENTRY*[current_size], 0);

          HASH_MAP_ITER iter = this->cache_map_.begin ();
          for (int i = 0; i < current_size; i++)
            {
              sorted_set[i] = &(*iter);
              iter++;
            }
          this->sort_set (sorted_set, current_size);
        }
    }

  return current_size;
}


void
TAO_Transport_Cache_Manager::close_entries(DESCRIPTOR_SET& sorted_set,
                                           int sorted_size)
{
  // Calculate the number of entries to purge
  const int amount = (sorted_size * this->percent_) / 100;

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                            ACE_TEXT ("Purging %d of %d cache entries\n"),
                            amount,
                            sorted_size));
    }

  int count = 0;
  for(int i = 0; count < amount && i < sorted_size; i++)
    {
      if (this->is_entry_idle(sorted_set[i]))
        {
          TAO_Transport* transport = sorted_set[i]->int_id_.transport ();
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                                    ACE_TEXT ("Idle transport found in ")
                                    ACE_TEXT ("cache: 0x%x\n"),
                                    transport));
            }

          // We need to save the cache_map_entry before we
          // set it to zero, so we can call purge_entry_i()
          // after we call close_connection_i().
          HASH_MAP_ENTRY* entry = transport->cache_map_entry ();

          // This is a bit ugly, but we must do this to
          // avoid taking and giving locks inside this loop.
          transport->cache_map_entry (0);
          transport->close_connection_i ();
          this->purge_entry_i (entry);

          // Count this as a successful purged entry
          count++;
        }
    }

  delete [] sorted_set;
  sorted_set = 0;
}

int
TAO_Transport_Cache_Manager::wait_for_connection (TAO_Cache_ExtId &extid)
{
  if (this->muxed_number_ &&
      this->muxed_number_ == extid.index ())
    {
      // If we have a limit on the number of muxed connections for
      // a particular endpoint just wait to get the connection
      ++this->no_waiting_threads_;

      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Going to wait for connections.. \n"));

      int ready_togo = 0;

      while (ready_togo == 0)
        {
          this->condition_->wait ();

          // Check whether we are waiting for this connection
          ready_togo = this->is_wakeup_useful (extid);
        }

      // We are not waiting anymore
      --this->no_waiting_threads_;
    }

  return 0;
}

int
TAO_Transport_Cache_Manager::is_wakeup_useful (TAO_Cache_ExtId &extid)
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  // Instantiations for the Hash Map
template class ACE_Equal_To<TAO_Cache_ExtId>;
template class ACE_Hash<TAO_Cache_ExtId>;
template class ACE_Hash_Map_Entry<TAO_Cache_ExtId, TAO_Cache_IntId>;
template class ACE_Hash_Map_Manager_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Unbounded_Set<ACE_Event_Handler*>;
template class ACE_Unbounded_Set_Iterator<ACE_Event_Handler*>;
template class ACE_Node<ACE_Event_Handler*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  // Instantiations for the Hash Map
#pragma instantiate ACE_Equal_To<TAO_Cache_ExtId>
#pragma instantiate ACE_Hash<TAO_Cache_ExtId>
#pragma instantiate ACE_Hash_Map_Entry<TAO_Cache_ExtId, TAO_Cache_IntId>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Unbounded_Set<ACE_Event_Handler*>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Event_Handler*>
#pragma instantiate ACE_Node<ACE_Event_Handler*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
