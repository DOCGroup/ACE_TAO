#include "tao/Connection_Cache_Manager.h"
#include "tao/Connection_Handler.h"
#include "tao/debug.h"


#if !defined (__ACE_INLINE__)
# include "tao/Connection_Cache_Manager.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID(tao, Connection_Cache_Hash_Manager, "$Id$")


TAO_Connection_Cache_Manager::
    TAO_Connection_Cache_Manager (void)
      : cache_map_ ()
{
  ACE_NEW (this->cache_lock_,
           ACE_Lock_Adapter<ACE_SYNCH_MUTEX>);
}

TAO_Connection_Cache_Manager::~TAO_Connection_Cache_Manager (void)
{
  // Delete the lock that we have
  delete this->cache_lock_;
}

int
TAO_Connection_Cache_Manager::bind_i (TAO_Cache_ExtId &ext_id,
                                      TAO_Cache_IntId &int_id)
{
  // Get the entry too
  HASH_MAP_ENTRY *entry = 0;
  int retval = this->cache_map_.bind (ext_id,
                                      int_id,
                                      entry);
  if (retval == 0)
    {
      // The entry has been added to cache succesfully
      // Add the cache_map_entry to the handler
      int_id.handler () ->cache_map_entry (entry);
    }
  else if (retval == 1)
    {
      if (TAO_debug_level > 0 && retval != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) TAO_Connection_Cache_Manager::bind_i")
                      ACE_TEXT (" unable to bind in the first attempt")
                      ACE_TEXT (" so trying with a new index \n")));
        }

      // There was an entry like this before, so let us do some
      // minor adjustments
      retval = this->get_last_index_bind (ext_id,
                                          int_id,
                                          entry);
      if (retval == 0)
        int_id.handler ()->cache_map_entry (entry);
    }

  if (TAO_debug_level > 0 && retval != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) TAO_Connection_Cache_Manager::bind_i")
                  ACE_TEXT (" unable to bind \n")));
    }

  return retval;
}


int
TAO_Connection_Cache_Manager::find_i (const TAO_Cache_ExtId &key,
                                      TAO_Cache_IntId &value)
{
  HASH_MAP_ENTRY *entry = 0;

  // Get the entry from the Hash Map
  int retval = this->cache_map_.find (key,
                                      entry);
  if (retval == 0)
    {
      retval = this->get_idle_handler (key,
                                       entry);

      // We have a succesful entry
      if (entry)
        {
          value = entry->int_id_;
        }

      if (TAO_debug_level > 0 && retval != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) TAO_Connection_Cache_Manager::find_i")
                      ACE_TEXT (" unable to locate a free connection \n")));
        }
    }


  return retval;
}

int
TAO_Connection_Cache_Manager::rebind_i (const TAO_Cache_ExtId &key,
                                           const TAO_Cache_IntId &value)
{
  return this->cache_map_.rebind (key,
                                  value);
}

int
TAO_Connection_Cache_Manager::trybind_i (const TAO_Cache_ExtId &key,
                                         TAO_Cache_IntId &value)
{
  return this->cache_map_.trybind (key, value);
}

int
TAO_Connection_Cache_Manager::unbind_i (const TAO_Cache_ExtId &key)
{
  return this->cache_map_.unbind (key);
}

int
TAO_Connection_Cache_Manager::unbind_i (const TAO_Cache_ExtId &key,
                                        TAO_Cache_IntId &value)
{
  return this->cache_map_.unbind (key,
                          value);
}

int
TAO_Connection_Cache_Manager::make_idle_i (HASH_MAP_ENTRY *&entry)
{

  // First get the entry again (if at all things had changed in the
  // cache map in the mean time)
  HASH_MAP_ENTRY *new_entry = 0;
  int retval = this->cache_map_.find (entry->ext_id_,
                                      new_entry);
  if (retval == 0)
    {
      new_entry->int_id_.
        recycle_state (ACE_RECYCLABLE_IDLE_AND_PURGABLE);

      entry = new_entry;
    }
  else if (TAO_debug_level > 0 && retval != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) TAO_Connection_Cache_Manager::make_idle_i")
                  ACE_TEXT ("unable to locate the entry to make it idle \n")));
    }

  return retval;
}

int
TAO_Connection_Cache_Manager::mark_closed_i (HASH_MAP_ENTRY *&entry)
{
  // First get the entry again (if at all things had changed in the
  // cache map in the mean time)
  HASH_MAP_ENTRY *new_entry = 0;

  int retval = this->cache_map_.find (entry->ext_id_,
                                      new_entry);
  if (retval == 0)
    {
      new_entry->int_id_.
        recycle_state (ACE_RECYCLABLE_CLOSED);

      entry = new_entry;
    }
    else if (TAO_debug_level > 0 && retval != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) TAO_Connection_Cache_Manager::make_idle_i")
                  ACE_TEXT ("unable to locate the entry to mark it as closed \n")));
    }

  return retval;
}

int
TAO_Connection_Cache_Manager::close_i (void)
{
  // Call the close the on the Hash Map that we hold
  return this->cache_map_.close ();
}

int
TAO_Connection_Cache_Manager::
    get_last_index_bind (TAO_Cache_ExtId &key,
                         TAO_Cache_IntId &val,
                         HASH_MAP_ENTRY *&entry)
{
  CORBA::ULong ctr = entry->ext_id_.index ();

  // Start looking at the succesive elements
  while (entry->next_ != 0 &&
         entry->next_->ext_id_.index () != 0)
    {
      ctr++;

      // Change the entry
      entry = entry->next_;
    }

  // Set the index
  key.index (ctr + 1);

  // Now do a bind again with the new index
  return  this->cache_map_.bind (key,
                                 val,
                                 entry);
}


int
TAO_Connection_Cache_Manager::
    get_idle_handler (const TAO_Cache_ExtId & /*ext_id*/,
                      HASH_MAP_ENTRY *&entry)
{
  // We are sure that we have an entry
  do
    {
      // Found the entry, so check whether it is busy
      if (entry->int_id_.recycle_state () == ACE_RECYCLABLE_IDLE_AND_PURGABLE ||
          entry->int_id_.recycle_state () == ACE_RECYCLABLE_IDLE_AND_PURGABLE)
        {
          // Save that in the handler
          entry->int_id_.handler ()->cache_map_entry (entry);

          // Mark the connection as busy
          entry->int_id_.recycle_state (ACE_RECYCLABLE_BUSY);
          return 0;
        }
      else
        {
          entry = entry->next_;
        }
    }
  // This would prevent us from moving to the next ext_id..
  while (entry->next_->ext_id_.index () != 0);

  // @@ There is a subtle assumption that I have made, ie. the
  // elements with higher indexes of ext_id will be placed
  // continously. That could be *bad*

  // We havent got a connection, so set the pointer to null.
  entry = 0;
  return -1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  // Instantiations for the Hash Map
template class ACE_Equal_To<TAO_Cache_ExtId>;
template class ACE_Hash<TAO_Cache_ExtId>;
template class ACE_Hash_Map_Entry<TAO_Cache_ExtId, TAO_Cache_IntId>;
template class ACE_Hash_Map_Manager<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  // Instantiations for the Hash Map
#pragma instantiate ACE_Equal_To<TAO_Cache_ExtId>
#pragma instantiate ACE_Hash<TAO_Cache_ExtId>
#pragma instantiate ACE_Hash_Map_Entry<TAO_Cache_ExtId, TAO_Cache_IntId>
#pragma instantiate ACE_Hash_Map_Manager<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
