// $Id$

#ifndef ACE_ARRAY_BASE_C
#define ACE_ARRAY_BASE_C

#include "ace/Array_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/Array_Base.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Malloc_Base.h"
#include "ace/os_include/os_errno.h"

ACE_RCSID(ace, Array_Base, "$Id$")


// Dynamically initialize an array.

template <class T>
ACE_Array_Base<T>::ACE_Array_Base (size_t size,
                                   ACE_Allocator *alloc)
  : max_size_ (size),
    cur_size_ (size),
    allocator_ (alloc)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (size != 0)
    {
      ACE_ALLOCATOR (this->array_,
                     (T *) this->allocator_->malloc (size * sizeof (T)));
      for (size_t i = 0; i < size; ++i)
        new (&array_[i]) T;
    }
  else
    this->array_ = 0;
}

template <class T>
ACE_Array_Base<T>::ACE_Array_Base (size_t size,
                                   const T &default_value,
                                   ACE_Allocator *alloc)
  : max_size_ (size),
    cur_size_ (size),
    allocator_ (alloc)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (size != 0)
    {
      ACE_ALLOCATOR (this->array_,
                     (T *) this->allocator_->malloc (size * sizeof (T)));
      for (size_t i = 0; i < size; ++i)
        new (&array_[i]) T (default_value);
    }
  else
    this->array_ = 0;
}

// The copy constructor (performs initialization).

template <class T>
ACE_Array_Base<T>::ACE_Array_Base (const ACE_Array_Base<T> &s)
  : max_size_ (s.size ()),
    cur_size_ (s.size ()),
    allocator_ (s.allocator_)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_ALLOCATOR (this->array_,
                 (T *) this->allocator_->malloc (s.size () * sizeof (T)));
  for (size_t i = 0; i < this->size (); i++)
    new (&this->array_[i]) T (s.array_[i]);
}

// Assignment operator (performs assignment).

template <class T> void
ACE_Array_Base<T>::operator= (const ACE_Array_Base<T> &s)
{
  // Check for "self-assignment".

  if (this != &s)
    {
      if (this->max_size_ < s.size ())
        {
          ACE_DES_ARRAY_FREE (this->array_,
                              this->max_size_,
                              this->allocator_->free,
                              T);
          ACE_ALLOCATOR (this->array_,
                         (T *) this->allocator_->malloc (s.size () * sizeof (T)));
          this->max_size_ = s.size ();
        }
      else
        {
          ACE_DES_ARRAY_NOFREE (this->array_,
                                s.size (),
                                T);
        }

      this->cur_size_ = s.size ();

      for (size_t i = 0; i < this->size (); i++)
        new (&this->array_[i]) T (s.array_[i]);
    }
}

// Set an item in the array at location slot.

template <class T> int
ACE_Array_Base<T>::set (const T &new_item, size_t slot)
{
  if (this->in_range (slot))
    {
      this->array_[slot] = new_item;
      return 0;
    }
  else
    return -1;
}

// Get an item in the array at location slot.

template <class T> int
ACE_Array_Base<T>::get (T &item, size_t slot) const
{
  if (this->in_range (slot))
    {
      // Copies the item.  If you don't want to copy, use operator []
      // instead (but then you'll be responsible for range checking).
      item = this->array_[slot];
      return 0;
    }
  else
    return -1;
}

template<class T> int
ACE_Array_Base<T>::max_size (size_t new_size)
{
  if (new_size > this->max_size_)
    {
      T *tmp;

      ACE_ALLOCATOR_RETURN (tmp,
                            (T *) this->allocator_->malloc (new_size * sizeof (T)),
                            -1);
      for (size_t i = 0; i < this->cur_size_; ++i)
        new (&tmp[i]) T (this->array_[i]);

      // Initialize the new portion of the array that exceeds the
      // previously allocated section.
      for (size_t j = this->cur_size_; j < new_size; j++)
        new (&tmp[j]) T;

      ACE_DES_ARRAY_FREE (this->array_,
                          this->max_size_,
                          this->allocator_->free,
                          T);
      this->array_ = tmp;
      this->max_size_ = new_size;
      this->cur_size_ = new_size;
    }

  return 0;
}

template<class T> int
ACE_Array_Base<T>::size (size_t new_size)
{
  int r = this->max_size (new_size);
  if (r != 0)
    return r;
  this->cur_size_ = new_size;
  return 0;
}

// ****************************************************************

template <class T> int
ACE_Array_Iterator<T>::next (T *&item)
{
  // ACE_TRACE ("ACE_Array_Iterator<T>::next");

  if (this->done ())
    {
      item = 0;
      return 0;
    }
  else
    {
      item = &array_[current_];
      return 1;
    }
}

#endif /* ACE_ARRAY_BASE_C */
