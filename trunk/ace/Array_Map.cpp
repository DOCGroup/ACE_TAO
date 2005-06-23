// $Id$

#ifndef ACE_ARRAY_MAP_CPP
#define ACE_ARRAY_MAP_CPP

#include "ace/Array_Map.h"

#ifndef __ACE_INLINE__
# include "ace/Array_Map.inl"
#endif  /* !__ACE_INLINE__ */

#include <algorithm>
#include <functional>


#ifndef ACE_LACKS_MEMBER_TEMPLATES
template<typename Key, typename Value>
template<typename InputIterator>
ACE_Array_Map<Key, Value>::ACE_Array_Map (InputIterator f, InputIterator l)
  : size_ (l - f)
  , capacity_ (size_)
  , nodes_ (size_ == 0 ? 0 : new value_type[size_])
{
  (void) std::copy (f, l, this->begin ());

//   iterator n = this->begin ();

//   for (InputIterator i = f; i != l; ++i, ++n)
//     *n = *i;
}
#else
template<typename Key, typename Value>
ACE_Array_Map<Key, Value>::ACE_Array_Map (
  ACE_Array_Map<Key, Value>::const_iterator f,
  ACE_Array_Map<Key, Value>::const_iterator l)
  : size_ (l - f)
  , capacity_ (size_)
  , nodes_ (size_ == 0 ? 0 : new value_type[size_])
{
  (void) std::copy (f, l, this->begin ());

//   iterator n = this->begin ();

//   for (const_iterator i = f; i != l; ++i, ++n)
//     *n = *i;
}
#endif  /* !ACE_LACKS_MEMBER_TEMPLATES */

template<typename Key, typename Value>
ACE_Array_Map<Key, Value>::ACE_Array_Map (
  ACE_Array_Map<Key, Value> const & map)
  : size_ (map.size_)
  , capacity_ (map.size_)
  , nodes_ (size_ == 0 ? 0 : new value_type[size_])
{
  std::copy (map.begin (), map.end (), this->begin ());

//   iterator f = map.begin ();
//   iterator l = map.end ();
//   iterator n = this->begin ();

//   for (iterator i = f; i != l; ++i, ++n)
//     *n = *i;
}

template<typename Key, typename Value>
ACE_Array_Map<Key, Value>::~ACE_Array_Map (void)
{
  delete[] this->nodes_;
}

template<typename Key, typename Value>
void
ACE_Array_Map<Key, Value>::swap (ACE_Array_Map<Key, Value> & map)
{
  std::swap (this->size_, map.size_);
  std::swap (this->capacity_, map.capacity_);
  std::swap (this->nodes_, map.nodes_);
}

template<typename Key, typename Value>
std::pair<typename ACE_Array_Map<Key, Value>::iterator, bool>
ACE_Array_Map<Key, Value>::insert (
  ACE_Array_Map<Key, Value>::value_type const & x)
{
  // Linear insertion due to linear duplicate key search.

  bool inserted = false;
  iterator i = this->find (x.first);

  if (i == this->end ())
    {
      // Add the element to the array.

      size_type const old_size = this->size ();
      this->grow (1);  // Increase size by at least one.

      i = this->begin () + old_size;
      *i = x;

       ++this->size_;

      inserted = true;
    }

  return std::make_pair (i, inserted);
}

#ifndef ACE_LACKS_MEMBER_TEMPLATES
template<typename Key, typename Value>
template<typename InputIterator>
void
ACE_Array_Map<Key, Value>::insert (InputIterator f, InputIterator l)
{
  this->grow (l - f);  // Preallocate storage.

  for (InputIterator i = f; i != l; ++i)
    {
      (void) this->insert (*i);
    }
}
#else
template<typename Key, typename Value>
void
ACE_Array_Map<Key, Value>::insert (ACE_Array_Map<Key, Value>::const_iterator f,
                                   ACE_Array_Map<Key, Value>::const_iterator l)
{
  this->grow (l - f);  // Preallocate storage.

  for (const_iterator i = f; i != l; ++i)
    {
      (void) this->insert (*i);
    }
}
#endif  /* ACE_LACKS_MEMBER_TEMPLATES */

template<typename Key, typename Value>
void
ACE_Array_Map<Key, Value>::erase (ACE_Array_Map<Key, Value>::iterator pos)
{
  iterator const first = this->begin ();
  iterator const last = this->end ();

  if (pos >= first && pos < last)
    {
      if (this->size_ > 1 && pos < last - 1)
        *pos = *(last - 1);

      --this->size_;
    }
}

template<typename Key, typename Value>
typename ACE_Array_Map<Key, Value>::size_type
ACE_Array_Map<Key, Value>::erase (
  ACE_Array_Map<Key, Value>::key_type const & k)
{
  iterator pos = this->find (k);

  size_type const old_size = this->size_;

  this->erase (pos);

  return old_size - this->size_;
}

template<typename Key, typename Value>
void
ACE_Array_Map<Key, Value>::erase (
  ACE_Array_Map<Key, Value>::iterator first,
  ACE_Array_Map<Key, Value>::iterator last)
{
  if (this->begin () <= first && first < last && last < this->end ())
    for (iterator i = first; i != last; ++i)
      this->erase (i);
}

template<typename Key, typename Value>
void
ACE_Array_Map<Key, Value>::clear (void)
{
  this->size_ = 0;  // No need to deallocate array nor destroy elements.
}

template<typename Key, typename Value>
typename ACE_Array_Map<Key, Value>::iterator
ACE_Array_Map<Key, Value>::find (ACE_Array_Map<Key, Value>::key_type const & k)
{
  iterator const the_end = this->end ();

  for (iterator i = this->begin (); i != the_end; ++i)
    if (k == i->first)
      return i;

  return this->end ();
}

template<typename Key, typename Value>
typename ACE_Array_Map<Key, Value>::const_iterator
ACE_Array_Map<Key, Value>::find (
  ACE_Array_Map<Key, Value>::key_type const & k) const
{
  const_iterator const the_end = this->end ();

  for (const_iterator i = this->begin (); i != the_end; ++i)
    if (k == i->first)
      return i;

  return this->end ();
}

template<typename Key, typename Value>
void
ACE_Array_Map<Key, Value>::grow (ACE_Array_Map<Key, Value>::size_type s)
{
  if (this->size () + s > this->capacity_)
    {
      // This implementation focuses more on static footprint than
      // speed.

      // Strongly exception safe.

      ACE_Array_Map<Key, Value> temp (this->size () + s);

      std::copy (this->begin (), this->end (), temp.begin ());

      size_type const n = this->size ();  // Do not swap out the size
                                          // since we bypassed the
                                          // temporary map's element
                                          // counting code.
      this->swap (temp);

      this->size_ = n;
    }
}

// ---------------------------------------------------------------

template <typename Key, typename Value>
bool
operator== (ACE_Array_Map<Key, Value> const & lhs,
            ACE_Array_Map<Key, Value> const & rhs)
{
  // Do not include Array_Map capacity in comparison.  It isn't useful
  // in this case.

  return (lhs.size () == rhs.size ()
          && std::equal (lhs.begin (), lhs.end (), rhs.begin ()));
}

template <typename Key, typename Value>
bool
operator< (ACE_Array_Map<Key, Value> const & lhs,
           ACE_Array_Map<Key, Value> const & rhs)
{
  return std::lexicographical_compare (lhs.begin (), lhs.end (),
                                       rhs.begin (), rhs.end ());
}

#endif  /* ACE_ARRAY_MAP_CPP */
