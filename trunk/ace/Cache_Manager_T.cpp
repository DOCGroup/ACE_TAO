// $Id$

#ifndef ACE_CACHE_MANAGER_T_CPP
#define ACE_CACHE_MANAGER_T_CPP

#include "ace/Cache_Manager_T.h"
#include "ace/Cache_Hash_T.h"
#include "ace/Cache_Heap_T.h"

ACE_RCSID(ace, Cache_Manager_T, "$Id$")

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC>
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::ACE_Cache_Manager (ACE_Allocator *alloc,
                     ACE_Cache_Object_Factory *cof,
                     size_t hashsize,
                     size_t maxsize,
                     size_t maxobjsize,
                     size_t minobjsize,
                     size_t highwater,
                     size_t lowwater,
                     int timetolive,
                     int counted)
  : allocator_ (alloc),
    factory_ (cof),
    hashsize_ (hashsize),
    maxsize_ (maxsize),
    maxobjsize_ (maxobjsize),
    minobjsize_ (minobjsize),
    highwater_ (highwater),
    lowwater_ (lowwater),
    waterlevel_ (0),
    timetolive_ (timetolive),
    counted_ (counted),
    hash_ (0),
    heap_ (0)
{
  // Some sanity checking needed here --
  if (this->lowwater_ > this->highwater_)
    this->lowwater_ = this->highwater_ / 2;

  // @@ James, please don't use "magic numbers" like 1024.  Make sure
  // you use the same symbolic constants as the other places...
  if (this->maxobjsize_ > (this->highwater_ - this->lowwater_) * 1024)
    this->maxobjsize_ = (this->highwater_ - this->lowwater_) * (1024/2);

  if (this->minobjsize_ > this->maxobjsize_)
    this->minobjsize_ = this->maxobjsize_ / 2;

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (this->factory_ == 0)
    this->factory_ = Object_Factory::instance ();

  ACE_NEW_MALLOC (this->hash_,
                  (Cache_Hash *)
                  this->allocator_->malloc (sizeof (Cache_Hash)),
                  Cache_Hash (alloc, hashsize));
  // @@ James, please check the ACE_NEW_MALLOC macro -- it should do
  // this check and bailout if this->hash_ == 0...

  if (this->hash_ == 0)
    {
      this->hashsize_ = 0;
      return;
    }

  ACE_NEW_MALLOC (this->heap_,
                  (Cache_Heap *)
                  this->allocator_->malloc (sizeof (Cache_Heap)),
                  Cache_Heap (alloc, maxsize));

  // @@ James, please check the ACE_NEW_MALLOC macro -- it should do
  // this check and bailout if this->hash_ == 0...

  if (this->heap_ == 0)
    {
      this->maxsize_ = 0;

      ACE_DES_FREE (this->hash_, this->allocator_->free, Cache_Hash);
      this->hash_ = 0;
      this->hashsize_ = 0;
    }
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::open (ACE_Allocator *alloc,
        ACE_Cache_Object_Factory *cof,
        size_t hashsize,
        size_t maxsize,
        size_t maxobjsize,
        size_t minobjsize,
        size_t highwater,
        size_t lowwater,
        int timetolive,
        int counted)
{
  this->close ();

  this->allocator_ = alloc;
  this->factory_ = cof;
  this->hashsize_ = hashsize;
  this->maxsize_ = maxsize;
  this->maxobjsize_ = maxobjsize;
  this->minobjsize_ = minobjsize;
  this->highwater_ = highwater;
  this->lowwater_ = lowwater;
  this->waterlevel_ = 0;
  this->timetolive_ = timetolive;
  this->counted_ = counted;

  // Some sanity checking needed here --
  if (this->lowwater_ > this->highwater_)
    this->lowwater_ = this->highwater_ / 2;

  if (this->maxobjsize_ > (this->highwater_ - this->lowwater_) * 1024)
    this->maxobjsize_ = (this->highwater_ - this->lowwater_) * (1024/2);

  if (this->minobjsize_ > this->maxobjsize_)
    this->minobjsize_ = this->maxobjsize_ / 2;

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (this->factory_ == 0)
    this->factory_ = Object_Factory::instance ();

  // @@ James, please use the ACE_NEW_MALLOC_RETURN macro here.
  this->hash_ = (Cache_Hash *) this->allocator_->malloc (sizeof (Cache_Hash));
  if (this->hash_ == 0)
    {
      errno = ENOMEM;
      this->hashsize_ = 0;

      return -1;
    }

  // @@ James, please use the ACE_NEW_*_RETURN macro here.
  new (this->hash_) Cache_Hash (alloc, hashsize);

  // @@ James, please use the ACE_NEW_MALLOC_RETURN macro here.
  this->heap_ = (Cache_Heap *) this->allocator_->malloc (sizeof (Cache_Heap));

  if (this->heap_ == 0)
    {
      errno = ENOMEM;
      this->maxsize_ = 0;

      ACE_DES_FREE (this->hash_, this->allocator_->free, Cache_Hash);
      this->hash_ = 0;
      this->hashsize_ = 0;

      return -1;
    }

  // @@ James, please use the ACE_NEW_*_RETURN macro here.
  new (this->heap_) Cache_Heap (alloc, maxsize);

  return 0;
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC>
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>::~ACE_Cache_Manager (void)
{
  this->close ();
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>::close (void)
{
  while (this->waterlevel_ > 0)
    this->FLUSH_i ();

  if (this->hash_)
    {
      ACE_DES_FREE (this->hash_, this->allocator_->free, Cache_Hash);
      this->hash_ = 0;
    }

  if (this->heap_)
    {
      ACE_DES_FREE (this->heap_, this->allocator_->free, Cache_Heap);
      this->heap_ = 0;
    }

  return 0;
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::GET_i (const KEY &key, ACE_Cache_Object *&object)
{
  int result = this->hash_->find (key, object);

  if (result == 0)
    this->TAKE (object);
  else
    object = 0;

  return result;
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::PUT_i (const KEY &key, const void *data, size_t size, ACE_Cache_Object *&obj)
{
  int result = 0;

  this->FLUSH_i (key);

  result = this->MAKE (data, size, obj);
  if (result == -1)
    return -1;

  result = this->hash_->bind (key, obj);
  if (result == -1)
    {
      this->DROP_i (obj);
      return -1;
    }

  result = this->heap_->insert (key, obj);
  if (result == -1)
    {
      this->hash_->unbind (key);
      this->DROP_i (obj);
      return -1;
    }

  this->waterlevel_ += size;

  // Acquire this one for the putter.
  this->TAKE (obj);

  return 0;
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::FLUSH_i (const KEY &key)
{
  ACE_Cache_Object *temp_object;

  int result = this->hash_->unbind (key, temp_object);
  if (result == 0)
    {
      this->waterlevel_ -= temp_object->size ();
      this->heap_->remove (temp_object->heap_item ());
      this->DROP_i (temp_object);
    }

  return result;
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::FLUSH_i (void)
{
  KEY temp_key;
  ACE_Cache_Object *temp_object;

  int result = this->heap_->remove (temp_key, temp_object);
  if (result == 0)
    {
      result = this->hash_->unbind (temp_key);
      this->waterlevel_ -= temp_object->size ();
      this->DROP_i (temp_object);
    }

  return result;
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::DROP_i (ACE_Cache_Object *const &obj)
{
  int result = obj->release ();

  if (result == 0)
    {
      if (obj->count () == 0)
        this->factory_->destroy (obj);
    }
  else
    result = this->heap_->adjust (obj->heap_item ());

  return result;
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::GET (const KEY &key, ACE_Cache_Object *&object)
{
  ACE_Read_Guard<ACE_SYNCH_RW_MUTEX> g (this->lock_);

  return this->GET_i (key, object);
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::PUT (const KEY &key, const void *data, size_t size, ACE_Cache_Object *&obj)
{
  ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> g (this->lock_);

  return this->PUT_i (key, data, size, obj);
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::MAKE (const void *data, size_t size, ACE_Cache_Object *&obj)
{
  // verify object is within cacheable range
  // @@ James, please don't use these magic numbers...
  if (size/1024 > this->maxobjsize_ || size/1024 < this->minobjsize_)
    return -1;

  // make sure heap has enough room
  if (this->heap_->is_full () && this->FLUSH_i () == -1)
    return -1;

  // make sure we have sufficient memory
  if (this->waterlevel_ + size > this->highwater_ * (1024 * 1024))
    do
      if (this->FLUSH_i () == -1)
        return -1;
    while (this->waterlevel_ > this->lowwater_ * (1024 * 1024));

  obj = this->factory_->create (data, size);
  if (this->TAKE (obj) == -1)
    {
      this->factory_->destroy (obj);
      obj = 0;
      return -1;
    }

  return 0;
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::TAKE (ACE_Cache_Object *const &obj)
{
  if (obj == 0)
    return -1;

  return obj->acquire ();
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::DROP (ACE_Cache_Object *const &obj)
{
  if (obj == 0)
    return -1;

  {
    ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> g (this->lock_);

    return this->DROP_i (obj);
  }
}

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC> int
ACE_Cache_Manager<KEY,FACTORY,HASH_FUNC,EQ_FUNC>
::FLUSH (void)
{
  ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> g (this->lock_);

  return this->FLUSH_i ();
}


template <class KEY, class DATA, class CACHE_MANAGER>
ACE_Cache_Proxy<KEY, DATA, CACHE_MANAGER>
::ACE_Cache_Proxy (const KEY &key, Cache_Manager *manager)
  : object_ (0),
    manager_ (manager)
{
  if (this->manager_ == 0)
    this->manager_ = Cache_Manager_Singleton::instance ();

  int result = this->manager_->GET (key, this->object_);
  if (result == -1)
    this->object_ = 0;
}

template <class KEY, class DATA, class CACHE_MANAGER>
ACE_Cache_Proxy<KEY, DATA, CACHE_MANAGER>
::ACE_Cache_Proxy (const KEY &key, DATA *data, size_t size,
                   Cache_Manager *manager)
  : object_ (0),
    manager_ (manager)
{
  if (this->manager_ == 0)
    this->manager_ = Cache_Manager_Singleton::instance ();

  int result = this->manager_->PUT (key, data, size, this->object_);
  if (result == -1)
    this->object_ = 0;
}

template <class KEY, class DATA, class CACHE_MANAGER>
ACE_Cache_Proxy<KEY, DATA, CACHE_MANAGER>::~ACE_Cache_Proxy (void)
{
  this->manager_->DROP (this->object_);
  this->object_ = 0;
}

template <class KEY, class DATA, class CACHE_MANAGER> DATA *
ACE_Cache_Proxy<KEY, DATA, CACHE_MANAGER>::data (void) const
{
  return (DATA *) this->object_->data ();
}

template <class KEY, class DATA, class CACHE_MANAGER>
ACE_Cache_Proxy<KEY, DATA, CACHE_MANAGER>::operator DATA * (void) const
{
  return this->data ();
}

#endif /* ACE_CACHE_MANAGER_T_CPP */
