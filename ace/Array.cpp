// Array.cpp
// $Id$

#if !defined (ARRAY_C)
#define ARRAY_C

#define ACE_BUILD_DLL
#include "Array.h"

#if !defined (__ACE_INLINE__)
#include "Array.i"
#endif /* __ACE_INLINE__ */

// Dynamically initialize an array.

template <class T>
ACE_Array<T>::ACE_Array (size_t size)
  : max_size_ (size),
    cur_size_ (size)
{
  ACE_NEW (this->array_, T[size]);
}

template <class T>
ACE_Array<T>::ACE_Array (size_t size,
		 const T &default_value)
  : max_size_ (size),
    cur_size_ (size)
{
  ACE_NEW (this->array_, T[size]);

  for (size_t i = 0; i < size; i++)
    this->array_[i] = default_value;
}

// The copy constructor (performs initialization).

template <class T>
ACE_Array<T>::ACE_Array (const ACE_Array<T> &s)
   : max_size_ (s.size ()),
     cur_size_ (s.size ())
{
  ACE_NEW (this->array_, T[s.size ()]);

  for (size_t i = 0; i < this->size (); i++)
    this->array_[i] = s.array_[i];
}

// Assignment operator (performs assignment).

template <class T> void
ACE_Array<T>::operator= (const ACE_Array<T> &s)
{
  // Check for "self-assignment".

  if (this == &s)
    return;
  else if (this->max_size_ < s.size ())
    {
      delete [] this->array_;

      ACE_NEW (this->array_, T[s.size ()]);

      this->max_size_ = s.size ();
    }

  this->cur_size_ = s.size ();

  for (size_t i = 0; i < this->size (); i++)
    this->array_[i] = s.array_[i];
}

// Set an item in the array at location index.

template <class T> int
ACE_Array<T>::set (const T &new_item, size_t index)
{
  if (this->in_range (index))
    {
      this->array_[index] = new_item;
      return 0;
    }
  else
    return -1;
}

// Get an item in the array at location index.

template <class T> int
ACE_Array<T>::get (T &item, size_t index) const
{
   if (this->in_range (index))
     {
       item = this->array_[index];  // Copies the item.  If you don't
                                    // want to copy, use operator [] instead
                                    // (but then you'll be responsible for
                                    // range checking).
       return 0;
     }
   else
     return -1;
}

// Compare this array with <s> for equality.

template <class T> int
ACE_Array<T>::operator== (const ACE_Array<T> &s) const
{
  if (this == &s)
    return 1;
  else if (this->cur_size_ != s.cur_size_)
    return 0;

  for (size_t index = 0; index < s.cur_size_; index++)
    if (this->array_[index] != s.array_[index])
      return 0;

  return 1;
}

// Compare this array with <s> for inequality.

template <class T> int
ACE_Array<T>::operator!= (const ACE_Array<T> &s) const
{
  return !(*this == s);
}

#endif /* ARRAY_C */
