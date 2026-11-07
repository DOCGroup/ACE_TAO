#ifndef JAWS_CACHED_ALLOCATOR_T_CPP
#define JAWS_CACHED_ALLOCATOR_T_CPP

#include "jaws3/Cached_Allocator_T.h"

template <class T, class ACE_LOCK> char *
JAWS_Cached_Allocator<T, ACE_LOCK>::get_next_pool (char *pool)
{
  char *next = nullptr;
  char *next_indirect = pool + (this->pool_size_);
  ACE_OS::memcpy (&next, next_indirect, sizeof (char *));
  return next;
}

template <class T, class ACE_LOCK> void
JAWS_Cached_Allocator<T, ACE_LOCK>::set_next_pool (char *pool, char *next_pool)
{
  char *next_indirect = pool + (this->pool_size_);
  ACE_OS::memcpy (next_indirect, &next_pool, sizeof (char *));
}

template <class T, class ACE_LOCK> void
JAWS_Cached_Allocator<T, ACE_LOCK>::extend_pool ()
{
  char *new_pool = nullptr;
  ACE_NEW (new_pool, char[this->pool_size_ + sizeof (char *)]);

  for (size_t c = 0; c < (this->pool_size_ / sizeof (T)); c++)
    {
      void* placement = new_pool + c * sizeof(T);
      this->free_list_.add (new (placement) ACE_Cached_Mem_Pool_Node<T>);
      this->set_next_pool (new_pool, nullptr);
    }

  if (this->pool_head_ == nullptr)
    this->pool_head_ = this->pool_tail_ = new_pool;
  else
    this->set_next_pool (this->pool_tail_, new_pool);

  this->pool_tail_ = new_pool;
}

template <class T, class ACE_LOCK>
JAWS_Cached_Allocator<T, ACE_LOCK>::JAWS_Cached_Allocator (size_t n_chunks)
  : pool_size_ (n_chunks * sizeof (T))
  , pool_head_ (nullptr)
  , pool_tail_ (nullptr)
  , free_list_ (ACE_PURE_FREE_LIST)
{
  this->extend_pool ();
}

template <class T, class ACE_LOCK>
JAWS_Cached_Allocator<T, ACE_LOCK>::~JAWS_Cached_Allocator ()
{
  char *curr = this->pool_head_;

  while (curr)
    {
      char *next = this->get_next_pool (curr);
      delete [] curr;
      curr = next;
    }
}

template <class T, class ACE_LOCK> void *
JAWS_Cached_Allocator<T, ACE_LOCK>::malloc (size_t nbytes)
{
  if (nbytes > sizeof (T))
    return nullptr;

  ACE_Cached_Mem_Pool_Node<T> *node = nullptr;
  node = this->free_list_.remove ();

  if (node == nullptr)
    {
      this->extend_pool ();
      node = this->free_list_.remove ();
      // ASSERT node != nullptr
    }

  // addr() call is really not absolutely necessary because of the way
  // ACE_Cached_Mem_Pool_Node's internal structure arranged.
  return node->addr ();
}

template <class T, class ACE_LOCK> void
JAWS_Cached_Allocator<T, ACE_LOCK>::free (void *ptr)
{
  this->free_list_.add ((ACE_Cached_Mem_Pool_Node<T> *) ptr);
}


template <class T> JAWS_Cached_Allocator<T, ACE_SYNCH_NULL_MUTEX> *
JAWS_TSS_Cached_Allocator<T>::ts_allocator ()
{
  JAWS_Cached_Allocator<T, ACE_SYNCH_NULL_MUTEX> *ts_obj = nullptr;

  ts_obj = this->ts_allocator_.ts_object ();

  // Don't need double-check locking since this value is
  // obtained from a thread specific context.
  if (ts_obj == nullptr)
    {
      ACE_NEW_RETURN (ts_obj,
                      JAWS_CACHED_ALLOCATOR(T) (this->n_chunks_),
                      nullptr);
      this->ts_allocator_.ts_object (ts_obj);
    }

  return ts_obj;
}

template <class T>
JAWS_TSS_Cached_Allocator<T>::JAWS_TSS_Cached_Allocator (size_t n_chunks)
  : n_chunks_ (n_chunks)
{
}

template <class T>
JAWS_TSS_Cached_Allocator<T>::~JAWS_TSS_Cached_Allocator ()
{
}

template <class T> void *
JAWS_TSS_Cached_Allocator<T>::malloc (size_t nbytes)
{
  return this->ts_allocator ()->malloc (nbytes);
}

template <class T> void
JAWS_TSS_Cached_Allocator<T>::free (void *ptr)
{
  this->ts_allocator ()->free (ptr);
}


#endif /* JAWS_CACHED_ALLOCATOR_T_CPP */
