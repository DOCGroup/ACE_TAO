// ================================================================
/**
 *  @file   Connection_Purging_Strategy.h
 *
 *  $Id$
 *
 *  @author Chad Elliott (elliott_c@ociweb.com)
 */
// ================================================================

#ifndef TAO_PURGING_STRATEGY_H
#define TAO_PURGING_STRATEGY_H
#include "ace/pre.h"

#include "tao/Transport_Cache_Manager.h"  // Data member
#include "tao/debug.h"                    // Debug used in the .inl
#include "ace/Array_Base.h"               // Template definition

class TAO_Resource_Factory;

typedef TAO_Transport_Cache_Manager        TAO_PURGING_CACHE_MANAGER;
typedef TAO_Transport_Descriptor_Interface TAO_DESCRIPTOR_INTERFACE;
typedef TAO_Transport                      TAO_PURGING_CACHE_ITEM;
typedef TAO_Transport_Cache_Manager::HASH_MAP_ENTRY TAO_PURGING_MAP_ENTRY;

class TAO_Export TAO_Connection_Purging_Strategy
{
public:
  /// The constructor
  TAO_Connection_Purging_Strategy (TAO_Resource_Factory* rf);

  /// The destructor
  virtual ~TAO_Connection_Purging_Strategy ();

  /// Get the purging percentage
  int percent () const;

  /// Set the purging percentage
  void percent (int pvalue);

  /// Maximum number of entries in the connection cache
  int cache_maximum () const;

  /// Remove connections based on the underlying strategy
  int purge ();

  /**
   * Pass-through methods to provide access to the cache manager.
   * Use these methods to update information about each item.
   */
  //@{
  /// Add the item to the cache. The item has the property
  /// definition based on which caching can be done.
  int add_to_cache (TAO_DESCRIPTOR_INTERFACE* prop,
                    TAO_PURGING_CACHE_ITEM* item);

  /// Check the cache to see whether the item exists
  /// in the cache and return it.
  int find_in_cache (TAO_DESCRIPTOR_INTERFACE* prop,     
                     TAO_PURGING_CACHE_ITEM*& item);

  /// Purge the entry from the cache
  int purge_from_cache (TAO_PURGING_MAP_ENTRY*& entry);

  /// Make the entry idle and ready for use.
  int make_idle (TAO_PURGING_MAP_ENTRY*& entry);

  /// Mark the entry as invalid for use but keep it in cache.
  void mark_invalid (TAO_PURGING_MAP_ENTRY*& entry);

  /// Initialize the cache with <size> elements.
  int open_cache (TAO_ORB_Core* orb_core,
                  size_t size = ACE_DEFAULT_MAP_SIZE);

  /// Close the underlying cache manager and return the handle set
  /// that have been registered with the reactor
  int close_cache (ACE_Handle_Set &reactor_registered,
                   TAO_EventHandlerSet &unregistered);

  /// Return the total size of the TAO_PURGING_CACHE_MANAGER.
  size_t total_size () const;
  //@}

protected:
  typedef ACE_Array_Base<TAO_DESCRIPTOR_INTERFACE*> DESCRIPTOR_SET;

  /// Accessor for the cache manager
  TAO_PURGING_CACHE_MANAGER& cache_manager ();

  /// Look through the sorted set and close the connection on
  /// the required number of items in the set.
  void close_entries (DESCRIPTOR_SET& sorted_set);

  // Sub-classes must implement these methods
  virtual int fill_set_i (DESCRIPTOR_SET& sorted_set) = 0;
  virtual void update_item_i (TAO_DESCRIPTOR_INTERFACE* prop,
                              TAO_PURGING_CACHE_ITEM* item) = 0;
  virtual void remove_item_i (TAO_DESCRIPTOR_INTERFACE* prop) = 0;
  virtual void remove_all_i () = 0;

private:

  /// The maximum number of entries in the cache
  int cache_maximum_;

  /// The percentage of the cache that will be purged
  int percent_;

  /// The lock for the underlying stratgy
  ACE_Lock* lock_;

  /// The caching implementation
  TAO_PURGING_CACHE_MANAGER cache_manager_;
};



#include "ace/Hash_Map_Manager_T.h"  // Template definition

class TAO_Export TAO_ULong_Connection_Purging_Strategy: public TAO_Connection_Purging_Strategy
{
public:
  /// The constructor
  TAO_ULong_Connection_Purging_Strategy (TAO_Resource_Factory* rf);

  /// The destructor
  virtual ~TAO_ULong_Connection_Purging_Strategy ();

protected:

  typedef unsigned long ATTRIBUTE_TYPE;
  typedef ACE_Hash_Map_Manager <TAO_Cache_ExtId,
                                ATTRIBUTE_TYPE,
                                ACE_Null_Mutex>
          HASH_MAP;

  /// Fill sorted_set in with the TAO_DESCRIPTOR_INTERFACE's in a
  /// sorted order.
  virtual int fill_set_i (DESCRIPTOR_SET& sorted_set);

  /// Removes the entry from the tracking_map_ when purge_from_cache()
  /// is called.
  virtual void remove_item_i (TAO_DESCRIPTOR_INTERFACE* prop);

  /// Remove all entries from tracking_map_
  virtual void remove_all_i ();

  HASH_MAP& tracking_map ();

private:

  typedef ACE_Hash_Map_Iterator <TAO_Cache_ExtId,
                                 ATTRIBUTE_TYPE,
                                 ACE_Null_Mutex>
          HASH_MAP_ITER;
  typedef ACE_Hash_Map_Entry <TAO_Cache_ExtId,
                              ATTRIBUTE_TYPE>
          HASH_MAP_ENTRY;

#if !defined(ACE_LACKS_QSORT)
  /// Used by qsort
  static int cpscmp(const void* a, const void* b);
#endif

  /// Sort the list of entries
  void sort_set (HASH_MAP_ENTRY**& entries, int size);

  HASH_MAP tracking_map_;
};


#if defined (__ACE_INLINE__)
# include "tao/Connection_Purging_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_PURGING_STRATEGY_H */
