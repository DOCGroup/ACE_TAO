// $Id$

#ifndef ACE_HASH_MAP_MANAGER_RT_T_INL
#define ACE_HASH_MAP_MANAGER_RT_T_INL

#include "Synch.h"


ACE_INLINE 
int
ACE_Hash_Map_RT_Coord::get_table (void)
{
  return this->table_;
}

ACE_INLINE 
int
ACE_Hash_Map_RT_Coord::get_bucket (void)
{
  return this->bucket_;
}

ACE_INLINE 
int
ACE_Hash_Map_RT_Coord::set_table (int newtables)
{
  this->table_ = newtables;
  return 0;
}

ACE_INLINE 
int
ACE_Hash_Map_RT_Coord::set_bucket (int newbuckets)
{
  this->bucket_ = newbuckets;
  return 0;
}

//***************************************************************/


template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
int
ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY>::hash (const EXT_ID& ext_id, ACE_Hash_Map_RT_Coord& coord)
{
  return this->func_.hash(ext_id, coord);
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
size_t
ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY>::get_num_tables()
{
  return this->num_tables_;
}

//***************************************************************/

template <class EXT_ID, class INT_ID> ACE_INLINE 
void
ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID>::set_ext_id (EXT_ID id)
{ 
  this->ext_id_ = id;
}

template <class EXT_ID, class INT_ID> ACE_INLINE 
void
ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID>::set_int_id (INT_ID id)
{ 
  this->int_id_ = id;
}

template <class EXT_ID, class INT_ID> ACE_INLINE 
EXT_ID&
ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID>::get_ext_id ()
{ 
  return this->ext_id_;
}

template <class EXT_ID, class INT_ID> ACE_INLINE 
INT_ID&
ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID>::get_int_id ()
{ 
  return this->int_id_;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
size_t
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::get_length (void)
{
  return this->length_;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
int
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::set_length (int new_length)
{
  this->length_ = new_length;
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
int
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::incr_length (void)
{
  return ++this->length_;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
int
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::decr_length (void)
{
  return --this->length_;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
int
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::is_clean (POD& manager_POD)
{
  return curPOD_ == &manager_POD;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
int
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::empty_bucket (POD* newPOD)
{
  this->head_    = new LITEM();
  this->last_    = this->head_;  
  this->length_ = 0;

  curPOD_ = newPOD;

  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
void
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>::set_POD (POD* pod)
{
  curPOD_ = pod;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
int
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>::get_bucket (int loc,
                                                            BUCKET& bucket)
{
  bucket = get_bucket(loc);
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
void
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>::incr_tablesize (void)
{
  ++this->numBuckets;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
int
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>::get_tablesize (void)
{
  return this->numBuckets;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
void
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>::set_tablesize (int numRemoved)
{
  this->numBuckets = numRemoved;
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>&
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>::get_bucket (int loc)
{
  return this->buckets_[loc];
}

template <class EXT_ID, class INT_ID, class HASH_KEY> ACE_INLINE 
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>* 
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>::get_bucket_array (void)
{
  return this->buckets_;
}


//***************************************************************/

ACE_INLINE 
int
ACE_Hash_Map_RT_StatsManager::get_total_elements (void)
{
  return this->total_elements_;
}

ACE_INLINE 
int
ACE_Hash_Map_RT_StatsManager::get_max_chain (void)
{
  return this->max_chain_;
}

ACE_INLINE 
void
ACE_Hash_Map_RT_StatsManager::compare_max_chain (int chainSize)
{
  if (chainSize > this->max_chain_)
    this->max_chain_ = chainSize;
}

ACE_INLINE 
void
ACE_Hash_Map_RT_StatsManager::reset_max_chain (void)
{
  this->max_chain_ = 0;
}

ACE_INLINE 
int
ACE_Hash_Map_RT_StatsManager::incr_total_elements (void)
{
  compare_max_chain (++this->total_elements_);
  return this->total_elements_;
}

ACE_INLINE 
int
ACE_Hash_Map_RT_StatsManager::decr_total_elements (void)
{
  return --this->total_elements_;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::size (void)
{
  return this->stats.getTotalElements();
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE
int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::bind (const EXT_ID &ext_id,
                                                                                    const INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  INT_ID dummy = int_id;

  ACE_Hash_Map_RT_Put_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> visitor (ext_id,
								                                                                  			 int_id,
											                                                                   this);
  return locate_i(ext_id,
		              visitor,
                  dummy);
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY>&
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::get_table (size_t loc)
{
  return table_[loc];
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::find (const EXT_ID &ext_id,
                                                                                    INT_ID &int_id)
{

  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  ACE_Hash_Map_RT_Get_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> visitor (ext_id, 
											 int_id,
											 this);
  return locate_i(ext_id, 
                  visitor,
                  int_id);
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::unbind (const EXT_ID &ext_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);
 
  INT_ID dummy;

  ACE_Hash_Map_RT_Remove_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> visitor (ext_id,
	                                                                   	 									    this);
  return locate_i(ext_id, 
                  visitor,
                  dummy);
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::find_bucket (POD* pod,
                                                                                           const EXT_ID &ext_id,
                                                                                           ACE_Hash_Map_RT_Coord& coord)
{
  pod->hash(ext_id, coord);
  return 1;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>&
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::find_bucket (POD* pod,
                                                                                           const EXT_ID &ext_id)
{
  ACE_Hash_Map_RT_Coord coord = ACE_Hash_Map_RT_Coord(0,0);
  pod->hash(ext_id, coord);
  return get_table(coord.get_table()).get_bucket(coord.get_bucket());
}


template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::change_POD (ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY>& newPOD)
{
  this->oldPOD_ = this->curPOD_;
  this->curPOD_ = &newPOD;
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY>*
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::get_POD (void)
{
    return curPOD_;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::is_stable (void)
{
  return (this->oldPOD_ != 0);
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE
int
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::hash (const KEY& key, ACE_Hash_Map_RT_Coord& coord)
{
  this->curPOD_->hash(key, coord);
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE
int 
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::rehash (ENTRY& entry)
{
  find_bucket(curPOD_, entry.get_ext_id()).prepend(entry);
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
size_t
ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::get_cleaning_rate (void)
{
  return cleaning_rate_;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::is_found (void)
{
  return (this->found_ != 0);
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
void
ACE_Hash_Map_RT_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::found_key (LITEM* litem)
{
  this->found_ = litem;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
ACE_Hash_Map_RT_ListItem<EXT_ID, INT_ID>&
ACE_Hash_Map_RT_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::get_found (void)
{
  return *this->found_;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Clean_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::visit (LITEM* litem)
{
  manager_->rehash(*litem->entry_);
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>*
ACE_Hash_Map_RT_Clean_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::start (BUCKET* bucket)
{  
  dead_bucket = *bucket;
  
  (*bucket).empty_bucket(this->manager_->get_POD());
  
  return &dead_bucket;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Clean_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::done (BUCKET& bucket)
{
  bucket.set_POD(manager_->get_POD());
  return 0;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>*
ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::start (BUCKET* bucket)
{
  return bucket;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::done (BUCKET&)
{
  return 0;
}

//***************************************************************/

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE 
int
ACE_Hash_Map_RT_Get_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::done (BUCKET& bucket)
{
  this->manager_->stats_->compare_max_chain(bucket.get_length());
  return 0;
}

#endif
