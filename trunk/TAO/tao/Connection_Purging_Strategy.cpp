// $Id$

#include "tao/Connection_Purging_Strategy.h"
#include "tao/Resource_Factory.h"
#include "tao/Transport.h"

#if !defined (__ACE_INLINE__)
# include "tao/Connection_Purging_Strategy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Connection_Purging_Strategy, "$Id$")

TAO_Connection_Purging_Strategy::TAO_Connection_Purging_Strategy (
                                               TAO_Resource_Factory* rf)
 : cache_maximum_ (rf->cache_maximum ()),
   percent_(rf->purge_percentage ()),
   lock_(rf->create_cached_connection_lock ()),
   cache_manager_ ()
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                            ACE_TEXT ("TAO_Connection_Purging_Strategy::")
                            ACE_TEXT ("TAO_Connection_Purging_Strategy\n")));
    }
}


TAO_Connection_Purging_Strategy::~TAO_Connection_Purging_Strategy (void)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                            ACE_TEXT ("TAO_Connection_Purging_Strategy::")
                            ACE_TEXT ("~TAO_Connection_Purging_Strategy\n")));
    }
  delete lock_;
}


void
TAO_Connection_Purging_Strategy::close_entries(DESCRIPTOR_SET& sorted_set)
{
  const int sorted_size = sorted_set.size ();
  const int amount = (sorted_size * this->percent ()) / 100;

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                            ACE_TEXT ("Purging %d of %d cache entries\n"),
                            amount,
                            sorted_size));
    }

  for(int i = 0; i < sorted_size; i++)
    {
      TAO_DESCRIPTOR_INTERFACE* prop = sorted_set[i];
      if (i < amount)
        {
          TAO_PURGING_CACHE_ITEM* item = 0;
          if (this->cache_manager ().find_transport (prop, item) == 0)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                                        ACE_TEXT ("Idle item found in ")
                                        ACE_TEXT ("cache: 0x%x\n"),
                                        item));
                }

              // Eventually calls purge_from_cache() on the cache
              // which will call remove_item_i() on us.
              item->close_connection ();
            }
        }
      delete prop;
    }
}


// ======================================================================

TAO_ULong_Connection_Purging_Strategy::TAO_ULong_Connection_Purging_Strategy (
                                               TAO_Resource_Factory* rf)
 : TAO_Connection_Purging_Strategy (rf)
{
}


TAO_ULong_Connection_Purging_Strategy::~TAO_ULong_Connection_Purging_Strategy ()
{
}


int
TAO_ULong_Connection_Purging_Strategy::fill_set_i (DESCRIPTOR_SET& sorted_set)
{
  int current_size = this->tracking_map_.current_size ();
  int amount = current_size - this->cache_maximum ();

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                            ACE_TEXT ("TAO_ULong_Connection_Purging_Strategy")
                            ACE_TEXT ("::fill_set_i - %d %d\n"),
                            current_size,
                            this->cache_maximum ()));
    }

  if (amount >= 0)
    {
      // I could not use ACE_Array_Base<HASH_MAP_ENTRY*> because
      // the compiler kept complaining about HASH_MAP_ENTRY not having
      // a default construtor.
      HASH_MAP_ENTRY** entries;
      ACE_NEW_RETURN(entries, HASH_MAP_ENTRY*[current_size], 0);
      sorted_set.size (current_size);

      int i;
      HASH_MAP_ITER iter = this->tracking_map_.begin ();
      for (i = 0; i < current_size; i++)
        {
          entries[i] = &(*iter);
          iter++;
        }

      this->sort_set (entries, current_size);

      for(i = 0; i < current_size; i++)
        {
          // The owner of the sorted_set array accepts responsibility
          // for deleting the memory allocated here.
          sorted_set[i] = entries[i]->ext_id_.property ()->duplicate ();
        }

      delete [] entries;
    }

  return (amount >= 0);
}


void
TAO_ULong_Connection_Purging_Strategy::remove_item_i (
                                       TAO_DESCRIPTOR_INTERFACE* prop)
{
  TAO_Cache_ExtId ext_id (prop);
  this->tracking_map_.unbind (ext_id);
}


void
TAO_ULong_Connection_Purging_Strategy::remove_all_i ()
{
  this->tracking_map_.unbind_all ();
}


#if !defined (ACE_LACKS_QSORT)
int
TAO_ULong_Connection_Purging_Strategy::cpscmp(const void* a, const void* b)
{
  const HASH_MAP_ENTRY** left  = (const HASH_MAP_ENTRY**)a;
  const HASH_MAP_ENTRY** right = (const HASH_MAP_ENTRY**)b;

  if ((*left)->int_id_ > (*right)->int_id_)
    return 1;

  if ((*left)->int_id_ < (*right)->int_id_)
    return -1;

  return 0;
}
#endif /* ACE_LACKS_QSORT */


void
TAO_ULong_Connection_Purging_Strategy::sort_set (HASH_MAP_ENTRY**& entries,
                                               int current_size)
{
#if defined (ACE_LACKS_QSORT)
  // Use insertion sort if we don't have qsort
  for(int i = 1; i < current_size; i++)
    {
      if (entries[i]->int_id_ <
                    entries[i - 1]->int_id_)
        {
          HASH_MAP_ENTRY* entry = entries[i];
          for(int j = i; j > 0 &&
                         entries[j - 1]->int_id_ >
                           entry->int_id_; j--)
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


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array_Base<TAO_Transport_Descriptor_Interface*>;
template class ACE_Hash_Map_Entry<TAO_Cache_ExtId, unsigned long>;
template class ACE_Hash_Map_Manager<TAO_Cache_ExtId, unsigned long, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_Cache_ExtId, unsigned long, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_Cache_ExtId, unsigned long, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_Cache_ExtId, unsigned long, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_Cache_ExtId, unsigned long, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Cache_ExtId, unsigned long, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Array_Base<TAO_Transport_Descriptor_Interface*>
#pragma instantiate ACE_Hash_Map_Entry<TAO_Cache_ExtId, unsigned long>
#pragma instantiate ACE_Hash_Map_Manager<TAO_Cache_ExtId, unsigned long, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_Cache_ExtId, unsigned long, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_Cache_ExtId, unsigned long, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_Cache_ExtId, unsigned long, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_Cache_ExtId, unsigned long, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Cache_ExtId, unsigned long, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
