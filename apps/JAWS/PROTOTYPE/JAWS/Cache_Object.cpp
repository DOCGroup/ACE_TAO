// $Id$

#include "JAWS/Cache_Object.h"

ACE_Cache_Object::ACE_Cache_Object (const void *data, size_t size)
  : internal_ (0),
    data_ (data),
    size_ (size)
{
  this->first_access_ = ACE_OS::time ((time_t *)0);
  this->new_last_access_ = this->last_access_ = this->first_access_;
}

ACE_Cache_Object::~ACE_Cache_Object ()
{
  this->data_ = 0;
  this->size_ = 0;
}

void *
ACE_Cache_Object::internal (void) const
{
  return this->internal_;
}

void
ACE_Cache_Object::internal (void *item)
{
  this->internal_ = item;
}

const void *
ACE_Cache_Object::data (void) const
{
  return this->data_;
}

size_t
ACE_Cache_Object::size (void) const
{
  return this->size_;
}

unsigned int
ACE_Cache_Object::count (void) const
{
  return this->count_i ();
}

int
ACE_Cache_Object::acquire (void)
{
  this->new_last_access_ = ACE_OS::time ((time_t *)0);
  return this->acquire_i ();
}

int
ACE_Cache_Object::release (void)
{
  this->last_access_ = this->new_last_access_;
  return this->release_i ();
}

time_t
ACE_Cache_Object::last_access (void) const
{
  return this->last_access_;
}

time_t
ACE_Cache_Object::first_access (void) const
{
  return this->first_access_;
}

unsigned int
ACE_Cache_Object::priority (void) const
{
  return this->priority_i ();
}

void *
ACE_Cache_Object::heap_item (void) const
{
  return this->heap_item_;
}

void
ACE_Cache_Object::heap_item (void *item)
{
  this->heap_item_ = item;
}


ACE_Referenced_Cache_Object::
ACE_Referenced_Cache_Object (const void *data, size_t size)
  : ACE_Cache_Object (data, size),
    lock_adapter_ (count_)
{
}

ACE_Referenced_Cache_Object::~ACE_Referenced_Cache_Object (void)
{
}

ACE_Lock &
ACE_Referenced_Cache_Object::lock (void)
{
  return this->lock_adapter_;
}

unsigned int
ACE_Referenced_Cache_Object::count_i (void) const
{
  ACE_Referenced_Cache_Object *mutable_this
    = (ACE_Referenced_Cache_Object *) this;

  if (mutable_this->count_.tryacquire_write () == 0)
    return 0;

  return 1;
}

int
ACE_Referenced_Cache_Object::acquire_i (void)
{
  return this->count_.acquire_read ();
}

int
ACE_Referenced_Cache_Object::release_i (void)
{
  return this->count_.release ();
}

unsigned int
ACE_Referenced_Cache_Object::priority_i (void) const
{
  unsigned int priority = ~(0U);
  double delta
    = ACE_OS::difftime (this->last_access (), this->first_access ());

  if (delta >= 0.0 && delta < ~(0U))
    priority = (unsigned) delta;

  return priority;
}



ACE_Counted_Cache_Object::
ACE_Counted_Cache_Object (const void *data, size_t size)
  : ACE_Cache_Object (data, size),
    count_ (0),
    new_count_ (0),
    lock_adapter_ (lock_)
{
}

ACE_Counted_Cache_Object::~ACE_Counted_Cache_Object (void)
{
}

ACE_Lock &
ACE_Counted_Cache_Object::lock (void)
{
  return this->lock_adapter_;
}

unsigned int
ACE_Counted_Cache_Object::count_i (void) const
{
  ACE_Counted_Cache_Object *mutable_this = (ACE_Counted_Cache_Object *) this;

  {
    ACE_Guard<ACE_SYNCH_MUTEX> g (mutable_this->lock_);

    return this->count_;
  }
}

int
ACE_Counted_Cache_Object::acquire_i (void)
{
  ACE_Guard<ACE_SYNCH_MUTEX> g (this->lock_);

  this->new_count_++;
  return 0;
}

int
ACE_Counted_Cache_Object::release_i (void)
{
  ACE_Guard<ACE_SYNCH_MUTEX> g (this->lock_);

  this->new_count_--;
  this->count_ = this->new_count_;
  return 0;
}

unsigned int
ACE_Counted_Cache_Object::priority_i (void) const
{
  return this->count_i ();
}

ACE_Cache_Object_Factory::ACE_Cache_Object_Factory (ACE_Allocator *alloc)
  : allocator_ (alloc)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();
}

ACE_Cache_Object_Factory::~ACE_Cache_Object_Factory (void)
{
}

int
ACE_Cache_Object_Factory::open (ACE_Allocator *alloc)
{
  this->allocator_ = alloc;

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  return 0;
}

ACE_Referenced_Cache_Object_Factory
::ACE_Referenced_Cache_Object_Factory (ACE_Allocator *alloc)
  : ACE_Cache_Object_Factory (alloc)
{
}

ACE_Referenced_Cache_Object_Factory
::~ACE_Referenced_Cache_Object_Factory (void)
{
}

ACE_Cache_Object *
ACE_Referenced_Cache_Object_Factory::create (const void *data, size_t size)
{
  ACE_Referenced_Cache_Object *obj;

  size_t obj_size = sizeof (ACE_Referenced_Cache_Object);
  ACE_NEW_MALLOC_RETURN (obj,
                         (ACE_Referenced_Cache_Object *)
                         this->allocator_->malloc (obj_size),
                         ACE_Referenced_Cache_Object (data, size), 0);

  return obj;
}

void
ACE_Referenced_Cache_Object_Factory::destroy (ACE_Cache_Object *obj)
{
  ACE_Referenced_Cache_Object *rco = (ACE_Referenced_Cache_Object *) obj;
  ACE_DES_FREE (rco, this->allocator_->free, ACE_Referenced_Cache_Object);
}

ACE_Counted_Cache_Object_Factory
::ACE_Counted_Cache_Object_Factory (ACE_Allocator *alloc)
  : ACE_Cache_Object_Factory (alloc)
{
}

ACE_Counted_Cache_Object_Factory
::~ACE_Counted_Cache_Object_Factory (void)
{
}

ACE_Cache_Object *
ACE_Counted_Cache_Object_Factory::create (const void *data, size_t size)
{
  ACE_Counted_Cache_Object *obj;

  size_t obj_size = sizeof (ACE_Counted_Cache_Object);
  ACE_NEW_MALLOC_RETURN (obj,
                         (ACE_Counted_Cache_Object *)
                         this->allocator_->malloc (obj_size),
                         ACE_Counted_Cache_Object (data, size), 0);

  return obj;
}

void
ACE_Counted_Cache_Object_Factory::destroy (ACE_Cache_Object *obj)
{
  ACE_Counted_Cache_Object *cco = (ACE_Counted_Cache_Object *) obj;
  ACE_DES_FREE (cco, this->allocator_->free, ACE_Counted_Cache_Object);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// These are only specialized with ACE_HAS_THREADS.
template class ACE_Lock_Adapter<ACE_SYNCH_RW_MUTEX>;
template class ACE_Lock_Adapter<ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// These are only specialized with ACE_HAS_THREADS.
#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

