// -*- C++ -*-

//=============================================================================
/**
 *  @file    Hash_Map_Manager_RT_T.cpp
 *
 *  $Id$
 *
 *  @author Jeremy M. Nolle <jmn3@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_HASH_MAP_MANAGER_RT_T_CPP
#define ACE_HASH_MAP_MANAGER_RT_T_CPP

#include "Hash_Map_Manager_RT_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#if !defined (__ACE_INLINE__)
#include "ace/Hash_Map_Manager_RT_T.inl"
#endif /* __ACE_INLINE__ */

#include "Synch.h"
#include "Service_Config.h"
#include "Malloc.h"
#include <string.h> 

ACE_RCSID(ace, Hash_Map_Manager_RT_T, "$Id$")
  
//***************************************************************/

ACE_Hash_Map_RT_Coord::ACE_Hash_Map_RT_Coord (int table,
                                              int bucket)
  : table_  (table),
    bucket_ (bucket)
{
}

ACE_Hash_Map_RT_Coord::~ACE_Hash_Map_RT_Coord (void)
{
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY>
ACE_Hash_Map_RT_Hash_Function<EXT_ID, INT_ID, HASH_KEY>::ACE_Hash_Map_RT_Hash_Function (int number_of_tables,
                                                                                        int buckets_per_table)
  : max_tables_        (number_of_tables),
    buckets_per_table_ (buckets_per_table)
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY>
ACE_Hash_Map_RT_Hash_Function<EXT_ID, INT_ID, HASH_KEY>::~ACE_Hash_Map_RT_Hash_Function (void)
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY> int
ACE_Hash_Map_RT_Hash_Function<EXT_ID, INT_ID, HASH_KEY>::hash (const EXT_ID& ext_id,
                                                               ACE_Hash_Map_RT_Coord& coord)
{
  int hashCode = hash_key_ (ext_id);
  if ( hashCode < 0 ) hashCode *= -1;
  
  this->hash_i (hashCode, coord);
  
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> int
ACE_Hash_Map_RT_Hash_Function<EXT_ID, INT_ID, HASH_KEY>::hash_i (int key,
                                                                 ACE_Hash_Map_RT_Coord& coord)
{
  // Have you thought about precisions in all the platforms that
  // ACE runs on?  Will these numbers work?

  double prod = (key * 0.618033988) - (size_t)(key * 0.618033988);
  size_t u    = (int)(this->max_tables_ * this->buckets_per_table_ * prod);
  
  coord.set_table(u / this->buckets_per_table_);
  coord.set_bucket(u % this->buckets_per_table_);
  
  return 0;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY>
ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY>::ACE_Hash_Map_RT_POD (int num_tables,
                                                                    ACE_Hash_Map_RT_Hash_Function<EXT_ID, INT_ID, HASH_KEY>& func)
  : num_tables_ (num_tables),
    func_       (func)
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY>
ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY>::~ACE_Hash_Map_RT_POD (void)
{
}

//***************************************************************/

template <class EXT_ID, class INT_ID>
ACE_Hash_Map_RT_ListItem<EXT_ID, INT_ID>::ACE_Hash_Map_RT_ListItem (ENTRY* entry,
                                                                    LITEM* next)
  : entry_ (entry),
    next_  (next)
{
}

template <class EXT_ID, class INT_ID>
ACE_Hash_Map_RT_ListItem<EXT_ID, INT_ID>::ACE_Hash_Map_RT_ListItem ()
  : entry_ (0),
    next_  (0)
{
}

template <class EXT_ID, class INT_ID>
ACE_Hash_Map_RT_ListItem<EXT_ID, INT_ID>::~ACE_Hash_Map_RT_ListItem (void)
{
}

//***************************************************************/

template <class EXT_ID, class INT_ID>
ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID>::ACE_Hash_Map_RT_Entry (const EXT_ID& ext_id,
                                                              const INT_ID& int_id)
  : ext_id_ (ext_id),
    int_id_ (int_id)
{
}

template <class EXT_ID, class INT_ID>
ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID>::ACE_Hash_Map_RT_Entry ()
{ 
}

# if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
template <class EXT_ID, class INT_ID>
ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID>::~ACE_Hash_Map_RT_Entry (void)
{
}
# endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY>
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::ACE_Hash_Map_RT_Bucket (POD* pod)
 : curPOD_ (pod)
{
  init_bucket_i(pod);
}

template <class EXT_ID, class INT_ID, class HASH_KEY>
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::ACE_Hash_Map_RT_Bucket (void)
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY>
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::~ACE_Hash_Map_RT_Bucket (void)
{

  // The "if" second argument results in a no-op instead of deallocation.  
  ACE_DES_FREE_TEMPLATE2 (this->curPOD_, 
                          ACE_NOOP,
                          ACE_Hash_Map_RT_POD, 
                          EXT_ID, 
                          INT_ID);
  this->curPOD_  = 0;

  // head_ has already been deleted by remove_all at this point, so we need
  // to clear the bucket's pointer b/c it points to a nonexistant object
  this->head_    = 0;
  this->last_    = 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> int
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::init_bucket_i (POD* pod)
{
  this->head_    = new LITEM();
  this->last_    = head_;
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> int
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::prepend (ENTRY& entry)
{

  this->head_->next_ = new LITEM((new ENTRY(entry.get_ext_id(),
                                            entry.get_int_id())),
                                 head_->next_);
  if (this->last_ == this->head_)
    this->last_ = this->head_->next_;
  this->incr_length();
  return 0;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY>
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>::ACE_Hash_Map_RT_Table ()
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY>
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>::ACE_Hash_Map_RT_Table (ACE_Allocator *allocator,
                                                                        size_t buckets_per_table,
                                                                        POD* pod)
{
  init_table_i(allocator, buckets_per_table, pod);
}

template <class EXT_ID, class INT_ID, class HASH_KEY>
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>::~ACE_Hash_Map_RT_Table (void)
{
  //
}

template <class EXT_ID, class INT_ID, class HASH_KEY> int
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>::init_table_i (ACE_Allocator *allocator,
                                                               size_t buckets_per_table,
                                                               POD* pod)
{
  size_t bytes = buckets_per_table * sizeof (BUCKET);
  void *ptr;
  
  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (bytes),
                        -1);
  
  this->buckets_ = (BUCKET *) ptr;
  
  for (size_t i = 0; i < buckets_per_table; i++)
    new (&this->buckets_[i]) BUCKET(pod);
  
  return 0;
};

//***************************************************************/

ACE_Hash_Map_RT_StatsManager::ACE_Hash_Map_RT_StatsManager (void)
  : max_chain_      (0),
    total_elements_ (0)
{
}

ACE_Hash_Map_RT_StatsManager::~ACE_Hash_Map_RT_StatsManager (void)
{
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ACE_Hash_Map_RT_Clean_Manager (ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* thismanager)
  : manager_   (thismanager),
    cur_table_ (0),
    cur_bucket_(0)
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
ACE_Hash_Map_RT_Clean_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ACE_Hash_Map_RT_Clean_Visitor (ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* thismanager)
{   
  this->manager_ = thismanager;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>*
ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::incremental_clean (void)
{
  if (more_to_do())
    {
		  BUCKET& bucket = manager_->get_table(cur_table_).get_bucket(cur_bucket_);
      if (!bucket.is_clean(*manager_->curPOD_))
        manager_->clean_vis_->process_bucket(bucket);

      ++cur_bucket_;  
		  if (cur_bucket_ >= manager_->buckets_per_table_)
        {
		      ++cur_table_;
		      cur_bucket_ = 0;
		    }
	  }
  if (cur_table_ < manager_->oldPOD_->get_num_tables()) 
    return this;
	else
    {
		  this->manager_->oldPOD_ = 0;
      this->cur_table_ = 0;
      this->manager_->stats_->reset_max_chain();
		  return 0;
	  }
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::more_to_do (void)
{
  return ((manager_->oldPOD_ != 0) && (cur_table_ < manager_->oldPOD_->get_num_tables()));
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ACE_Hash_Map_RT_Manager_Ex (size_t bucketsPerTable,
                                                                                                          size_t newrtl,
                                                                                                          size_t newMaxTables,
                                                                                                          ACE_Allocator *alloc,
                                                                                                          size_t clean_rate)
: buckets_per_table_ (bucketsPerTable),
  number_of_tables_  (1),
  RTL_               (newrtl),
  max_tables_        (newMaxTables),
  allocator_         (alloc),
  cleaning_rate_     (clean_rate)
{
  open_i(buckets_per_table_, alloc);
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::~ACE_Hash_Map_RT_Manager_Ex (void)
{
  close_manager();
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::open_i (size_t buckets_per_table, 
                                                                                      ACE_Allocator *alloc)
{
  void *ptr, *ptr2;
  
  ACE_ALLOCATOR_RETURN (ptr,
                        allocator_->malloc (sizeof(ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY>)),
                        -1);
  
  ACE_ALLOCATOR_RETURN (ptr2,
                        allocator_->malloc (sizeof(ACE_Hash_Map_RT_Hash_Function<EXT_ID, INT_ID, HASH_KEY>)),
                        -1);
  
  this->curPOD_ = new (ptr) ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY>(1, *(new (ptr2) ACE_Hash_Map_RT_Hash_Function<EXT_ID, INT_ID, HASH_KEY>(1, this->buckets_per_table_)));
  this->oldPOD_ = 0;

  ACE_ALLOCATOR_RETURN (ptr,
                        allocator_->malloc (sizeof(ACE_Hash_Map_RT_StatsManager)),
                        -1);

  this->stats_ = new (ptr) ACE_Hash_Map_RT_StatsManager();

  ACE_ALLOCATOR_RETURN (ptr,
                        allocator_->malloc (sizeof(ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>)),
                        -1);

  this->clean_ = new (ptr) ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> (this);

  ACE_ALLOCATOR_RETURN (ptr,
                        allocator_->malloc (sizeof(ACE_Hash_Map_RT_Clean_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>)),
                        -1);

  this->clean_vis_ = new (ptr) ACE_Hash_Map_RT_Clean_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>(this);

  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);
 
  if (alloc == 0)
    alloc = ACE_Allocator::instance ();

  this->allocator_ = alloc;
  
  size_t bytes = this->max_tables_ * sizeof(TABLE);
  
  ACE_ALLOCATOR_RETURN (ptr,
                        allocator_->malloc (bytes),
                        -1);
  
  this->table_ = (TABLE *) ptr;
  
  for (size_t i = 0; i < max_tables_; i++)
    new (&this->get_table(i)) TABLE(allocator_, buckets_per_table, this->curPOD_);
  
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::close_manager (void)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->close_i ();
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::close_i (void)
{
  // Protect against "double-deletion" in case the destructor also
  // gets called.
  if (this->table_ != 0)
    {
      // Remove all the entries.
      this->remove_all_i ();
      
      // Iterate through the buckets
      for (size_t i = 0; i < this->max_tables_; i++)
        {
          for (size_t j = 0; j < this->buckets_per_table_; j++)
            {
              BUCKET *temp_bkt = &(this->get_table(i).get_bucket(j));
                            
              // The "if" second argument results in a no-op instead of
              // deallocation.	      
              ACE_DES_FREE_TEMPLATE2 (temp_bkt, 
                                      ACE_NOOP,
                                      ACE_Hash_Map_RT_Bucket, 
                                      EXT_ID, 
                                      INT_ID);
              temp_bkt = 0;
            }

          TABLE *temp_tbl = &(this->get_table(i));
          // The "if" second argument results in a no-op instead of
          // deallocation.	      
          ACE_DES_FREE_TEMPLATE2 (temp_tbl, 
                                  ACE_NOOP,
                                  ACE_Hash_Map_RT_Table, 
                                  EXT_ID, 
                                  INT_ID);
          temp_tbl = 0;
        }

      POD *pod = (this->get_POD());

      ACE_DES_FREE_TEMPLATE2 (pod, 
                              ACE_NOOP,
                              ACE_Hash_Map_RT_POD, 
                              EXT_ID, 
                              INT_ID);
      pod = 0;

      delete this->oldPOD_;
      this->oldPOD_    = 0;
      
      ACE_Hash_Map_RT_StatsManager *stats = (this->stats_);
      ACE_DES_FREE_TEMPLATE2 (stats, 
                              ACE_NOOP,
                              ACE_Hash_Map_RT_StatsManager, 
                              EXT_ID, 
                              INT_ID);
      stats = 0;

      CLEAN *clean = (this->clean_);
      ACE_DES_FREE_TEMPLATE2 (clean, 
                              ACE_NOOP,
                              ACE_Hash_Map_RT_Clean_Manager, 
                              EXT_ID, 
                              INT_ID);
      clean = 0;
      
      CLEANV *cleanv = (this->clean_vis_);
      ACE_DES_FREE_TEMPLATE2 (cleanv, 
                              ACE_NOOP,
                              ACE_Hash_Map_RT_Clean_Visitor,
                              EXT_ID, 
                              INT_ID);
      cleanv = 0;

      // Free table memory.
      this->allocator_->free (this->table_);

      // Should be done last...
      this->table_ = 0;
    }
  
  return 0;
}


template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::remove_all_i (void)
{
  // Iterate through the entire map calling the destuctor of each <ACE_Hash_Map_ListItem>.
  
  for (size_t i = 0; i < this->max_tables_; i++)
    {
      for (size_t j = 0; j < this->buckets_per_table_; j++)
        {
          for (LITEM *temp_ptr = this->get_table(i).get_bucket(j).head_; ((temp_ptr != this->get_table(i).get_bucket(j).last_) && (temp_ptr != 0)); )
            {
              LITEM *hold_ptr = temp_ptr;
              if (temp_ptr->next_ != 0)
                {
                  temp_ptr = temp_ptr->next_;
                  delete hold_ptr;
                  hold_ptr = 0;
		  
                  /// Explicitly call the destructor.
                  ACE_DES_FREE_TEMPLATE2 (hold_ptr,
                                          this->allocator_->free,
                                          ACE_Hash_Map_RT_ListItem,
                                          EXT_ID,
                                          INT_ID);
                }
	          }
          //done with the bucket
          this->get_table(i).get_bucket(j).set_length(0);
        }
      //done with the table
      this->get_table(i).set_tablesize(0);
    }

  //this->cur_size_ = 0;
  
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::locate_i (const EXT_ID &ext_id,
                                                                                        ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>& visitor,
                                                                                        INT_ID& int_id)
{
  ACE_Hash_Map_RT_Coord oldcoord = ACE_Hash_Map_RT_Coord(0,0);
  ACE_Hash_Map_RT_Coord newCoord = ACE_Hash_Map_RT_Coord(0,0);
  
  BUCKET& new_bucket = find_bucket(this->curPOD_, ext_id);
  
  if (is_stable() == 1)
    {
      int_id = 0;
    
      size_t num_cleaned = 0;
      if (!find_bucket(this->oldPOD_, ext_id).is_clean(*this->curPOD_))
        {
          clean_vis_->process_bucket(find_bucket(oldPOD_, ext_id));
          num_cleaned++;
        }
      if (!new_bucket.is_clean(*this->curPOD_))
        {
          clean_vis_->process_bucket(new_bucket);
          num_cleaned++;
        }
      visitor.process_bucket(new_bucket);
      while ((clean_ != 0) && (num_cleaned < get_cleaning_rate())) {
        clean_ = clean_->incremental_clean(); // Incremental clean & Method that ends a rehash
        num_cleaned++;
      }
      if (clean_ == 0)
        clean_ = new ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> (this);
    }
  
  else
    {
      visitor.process_bucket(new_bucket);
    }
  
  if (visitor.is_found() == 1)
    { 
      ENTRY& temp = *(visitor.get_found()).entry_;
      if (temp.get_int_id() != 0)
      	{
	        int_id = temp.get_int_id();
	        return 0;
	      }
    }
  
  // we looped without finding the bucket
  return -1;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY>*
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::create_POD (void)
{
  int bucketsPerTable = buckets_per_table_;
  int currNumTables = number_of_tables_;

  double tuningKnob          = 0.000;

  int new_num_of_tables      = 0;
	int currNumBuckets         = bucketsPerTable * currNumTables;
	double new_num_of_buckets  = currNumBuckets + (double)(currNumBuckets+1)/(RTL_-1);
	double idealLoadFactor     = RTL_ - 1;
	
	double currNumElements     = (double)(stats_->get_total_elements ());
	double currentLoadFactor   = currNumElements / currNumBuckets;
	double addPercentage       = (idealLoadFactor - currentLoadFactor) / idealLoadFactor;

	new_num_of_tables    = (int)(ceil(new_num_of_buckets / (double)bucketsPerTable));
	
	double extraFraction = addPercentage * new_num_of_buckets;
  if (extraFraction < 0)
      extraFraction *= -1;
  
	new_num_of_buckets   = (int)(ceil(new_num_of_buckets + extraFraction * tuningKnob));
	new_num_of_tables    = (int)(ceil(new_num_of_buckets / (double)bucketsPerTable));
	
  if(new_num_of_tables > this->max_tables_)
    new_num_of_tables = this->max_tables_;

  if(new_num_of_buckets > this->buckets_per_table_)
     new_num_of_buckets = this->buckets_per_table_;

  this->number_of_tables_  = new_num_of_tables;
  this->buckets_per_table_ = new_num_of_buckets;

  return (new ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY> (new_num_of_tables, *(new ACE_Hash_Map_RT_Hash_Function<EXT_ID, INT_ID, HASH_KEY>(new_num_of_tables, bucketsPerTable))));
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> size_t
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ceil (double number)
{
  if ((int)number != number)
    ++number;
  return (size_t)number;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_RT_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::process_bucket (BUCKET& temp_bucket)
{
  BUCKET& bucket = *(start(&temp_bucket));

  LITEM* item    = bucket.head_;    // head doesnt have an entry
  this->mtfPrev_ = bucket.head_;    // prev is head until we step forward
  
  while( item != (bucket.last_) )   // all ListItems have a next except last
    {
      item = item->next_;
      visit(item);                  // so let's visit that next_ ListItem
      
      if(is_found() == 0)           // If we havent found the one we are looking for,
        this->mtfPrev_ = item;      // then save the one we are on (the 'previous').
                                    // After we find the one we want, we dont want to overwrite 
                                    // our mtfPrev_ ptr anymore
    }
  done(bucket);
  
  return 0;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ACE_Hash_Map_RT_Searching_Visitor (const EXT_ID &ext_id,
                                                                                                                        ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* manager)
  : ext_id_  (ext_id),
    manager_ (manager)
{
    this->found_ = 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ACE_Hash_Map_RT_Searching_Visitor (void)
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::visit (LITEM* litem)
{
  if ((litem != 0) && (strcmp(litem->entry_->get_ext_id(), this->ext_id_) == 0))
    {
      found_key (litem);
      return 0;
    }
  return -1;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
ACE_Hash_Map_RT_Put_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ACE_Hash_Map_RT_Put_Visitor (const EXT_ID &ext_id,
                                                                                                            const INT_ID &int_id,
                                                                                                            ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* manager)
  : ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> (ext_id, manager),
    int_id_ (int_id)
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> void
ACE_Hash_Map_RT_Put_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::found_key (LITEM* litem)
{
  this->found_ =  litem;

  // this will overwrite the old value,
  // we need this in case we insert the same key with a new value
  litem->entry_->set_int_id(this->int_id_);
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_RT_Put_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::done (BUCKET& bucket)
{
  if (is_found() == 0)   // if we havent found what we're looking for
    {                    // then bind a new one
      ENTRY entry = ENTRY(this->ext_id_,
			                    this->int_id_);
      
      bucket.prepend(entry);
      
      LITEM litem = LITEM (&entry,
			                     bucket.head_->next_);      

      found_key(&litem);

      this->manager_->stats_->incr_total_elements();

      if (manager_->is_stable() == 0)
        {
		      this->manager_->stats_->compare_max_chain(bucket.get_length());
          if (bucket.get_length() >= this->manager_->RTL_)
            {
              POD* pod = this->manager_->create_POD();
              if (pod != 0)
                this->manager_->change_POD(*pod);
            }  
        }
      return 0;
    }
  
  return -1;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
ACE_Hash_Map_RT_Get_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ACE_Hash_Map_RT_Get_Visitor (const EXT_ID &ext_id,
                                                                                                          INT_ID &int_id,
                                                                                                          ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* thismanager)
  : ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> (ext_id, thismanager),
    int_id_ (int_id)
{
}


//***************************************************************/


template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
ACE_Hash_Map_RT_Remove_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ACE_Hash_Map_RT_Remove_Visitor (const EXT_ID &ext_id,
                                                                                                                ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* thismanager)
  : ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>(ext_id, thismanager)
{
}


template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_RT_Remove_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::done (BUCKET& bucket)
{
  if(is_found() == 1)
    {
      if (this->found_ == bucket.last_)
        bucket.last_ = this->mtfPrev_;
      
      this->mtfPrev_->next_ = this->get_found().next_;
      
      delete this->found_;
      this->found_ = 0;
      
      bucket.decr_length();
      this->manager_->stats_->decr_total_elements();
    }
  
  return 0;
}


#endif /* ACE_HASH_MAP_RT_MANAGER_RT_CPP */
