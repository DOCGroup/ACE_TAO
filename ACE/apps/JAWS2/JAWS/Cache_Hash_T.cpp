#ifndef JAWS_CACHE_HASH_T_CPP
#define JAWS_CACHE_HASH_T_CPP

#include "JAWS/Cache_Hash_T.h"
#include "JAWS/Hash_Bucket_T.h"

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> unsigned long
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::hash (const EXT_ID &ext_id) const
{
  return HASH_FUNC (ext_id) % this->size_;
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> bool
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::isprime (size_t number) const
{
  size_t d = 3;

  if (number <= 2) return number == 2;

  if (number % 2 == 0) return false;

  while (d <= number/d)
    {
      if (number % d == 0) return false;
      d += 2;
    }

  return true;
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> int
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::new_cachebucket (size_t hash_idx)
{
  if (this->hashtable_[hash_idx] == nullptr)
    {
      size_t const alloc_size = sizeof (CACHE_BUCKET_MANAGER);
      ACE_NEW_MALLOC_RETURN (this->hashtable_[hash_idx],
                             (CACHE_BUCKET_MANAGER *)
                             this->allocator_->malloc (alloc_size),
                             CACHE_BUCKET_MANAGER (this->allocator_), -1);
    }

  return 0;
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC>
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::JAWS_Cache_Hash (ACE_Allocator *alloc,
                                                          size_t size)
  : allocator_ (alloc),
    hashtable_ {}
{
  while (!this->isprime (size))
    ++size;

  this->size_ = size;

  if (this->allocator_ == nullptr)
    this->allocator_ = ACE_Allocator::instance ();

  size_t const memsize = this->size_ * sizeof (CACHE_BUCKET_MANAGER *);

  this->hashtable_
    = (CACHE_BUCKET_MANAGER **) this->allocator_->malloc (memsize);

  if (this->hashtable_)
    {
      for (size_t i = 0; i < this->size_; ++i)
        this->hashtable_[i] = nullptr;
    }
  else
    {
      this->size_ = 0;
      // should indicate something is wrong to the user.
    }
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC>
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::~JAWS_Cache_Hash ()
{
  if (this->hashtable_)
    {
      for (size_t i = 0; i < this->size_; ++i)
        {
          if (this->hashtable_[i])
            {
              ACE_DES_FREE_TEMPLATE3(this->hashtable_[i],
                                     this->allocator_->free,
                                     JAWS_Hash_Bucket_Manager,
                                     EXT_ID,
                                     JAWS_Cache_Object *,
                                     EQ_FUNC);

              this->hashtable_[i] = nullptr;
            }
        }
      this->allocator_->free (this->hashtable_);
      this->hashtable_ = nullptr;
    }

  this->allocator_ = nullptr;
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> int
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::find (const EXT_ID &ext_id) const
{
  unsigned long const hash_idx = this->hash (ext_id);

  if (this->hashtable_[hash_idx] == nullptr)
    return -1;

  return this->hashtable_[hash_idx]->find (ext_id);
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> int
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::find (const EXT_ID &ext_id,
                                                JAWS_Cache_Object *&int_id) const
{
  unsigned long const hash_idx = this->hash (ext_id);

  if (this->hashtable_[hash_idx] == nullptr)
    return -1;

  return this->hashtable_[hash_idx]->find (ext_id, int_id);
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> int
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::bind (const EXT_ID &ext_id,
                                                JAWS_Cache_Object *const &int_id)
{
  int result;
  unsigned long const hash_idx = this->hash (ext_id);

  if (this->hashtable_[hash_idx] == nullptr)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, g, this->lock_, -1);

      if (this->new_cachebucket (hash_idx) == -1)
        return -1;

      result = this->hashtable_[hash_idx]->bind (ext_id, int_id);
    }
  else
    result = this->hashtable_[hash_idx]->bind (ext_id, int_id);

  return result;
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> int
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::trybind (const EXT_ID &ext_id,
                                                   JAWS_Cache_Object *&int_id)
{
  int result;
  unsigned long const hash_idx = this->hash (ext_id);

  if (this->hashtable_[hash_idx] == nullptr)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, g, this->lock_, -1);

      if (this->new_cachebucket (hash_idx) == -1)
        return -1;

      result = this->hashtable_[hash_idx]->trybind (ext_id, int_id);
    }
  else
    result = this->hashtable_[hash_idx]->trybind (ext_id, int_id);

  return result;
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> int
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::rebind (const EXT_ID &ext_id,
                                                  JAWS_Cache_Object *const &int_id,
                                                  EXT_ID &old_ext_id,
                                                  JAWS_Cache_Object *&old_int_id)
{
  int result;
  unsigned long const hash_idx = this->hash (ext_id);

  if (this->hashtable_[hash_idx] == nullptr)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, g, this->lock_, -1);

      if (this->new_cachebucket (hash_idx) == -1)
        return -1;

      result = this->hashtable_[hash_idx]->rebind (ext_id, int_id,
                                                   old_ext_id, old_int_id);
    }
  else
    result = this->hashtable_[hash_idx]->rebind (ext_id, int_id,
                                                 old_ext_id, old_int_id);

  return result;
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> int
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::unbind (const EXT_ID &ext_id)
{
  unsigned long const hash_idx = this->hash (ext_id);

  if (this->hashtable_[hash_idx] == nullptr)
    return -1;

  return this->hashtable_[hash_idx]->unbind (ext_id);
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> int
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::unbind (const EXT_ID &ext_id,
                                                  JAWS_Cache_Object *&int_id)
{
  unsigned long const hash_idx = this->hash (ext_id);

  if (this->hashtable_[hash_idx] == nullptr)
    return -1;

  return this->hashtable_[hash_idx]->unbind (ext_id, int_id);
}

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC> size_t
JAWS_Cache_Hash<EXT_ID,HASH_FUNC,EQ_FUNC>::size () const
{
  return this->size_;
}

#endif /* JAWS_CACHEHASH_T_CPP */
