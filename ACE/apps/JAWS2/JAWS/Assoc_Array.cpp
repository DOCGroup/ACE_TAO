#ifndef JAWS_ASSOC_ARRAY_CPP
#define JAWS_ASSOC_ARRAY_CPP

#include "ace/config-all.h"
#include "JAWS/Assoc_Array.h"

template <class KEY, class DATA>
JAWS_Assoc_Array<KEY,DATA>::JAWS_Assoc_Array (int maxsize)
  : k_array_ (maxsize),
    d_array_ (maxsize),
    maxsize_ (maxsize)
{
}

template <class KEY, class DATA>
JAWS_Assoc_Array<KEY,DATA>::~JAWS_Assoc_Array ()
{
  this->clear ();
}

template <class KEY, class DATA> int
JAWS_Assoc_Array<KEY,DATA>::index (const KEY &k)
{
  return this->find_i (k);
}

template <class KEY, class DATA> DATA *
JAWS_Assoc_Array<KEY,DATA>::find (const KEY &k)
{
  int const i = this->find_i (k);

  return i < this->maxsize_ ? this->d_array_[i].get () : nullptr;
}

template <class KEY, class DATA> DATA *
JAWS_Assoc_Array<KEY,DATA>::find_by_index (int i)
{
  return (0 <= i) && (i < this->maxsize_) ? this->d_array_[i].get () : nullptr;
}

template <class KEY, class DATA> DATA *
JAWS_Assoc_Array<KEY,DATA>::insert (const KEY &k, const DATA &d)
{
  int const i = this->find_i (k);

  if (i == this->maxsize_)
    return nullptr;

  if (!this->k_array_[i])
    {
      std::unique_ptr<DATA> data = std::make_unique<DATA> (d);
      std::unique_ptr<KEY> key = std::make_unique<KEY> (k);
      this->d_array_[i] = std::move (data);
      this->k_array_[i] = std::move (key);
    }

  return this->d_array_[i].get ();
}

template <class KEY, class DATA> int
JAWS_Assoc_Array<KEY,DATA>::remove (const KEY &k)
{
  int const i = this->find_i (k);

  if (i == this->maxsize_)
    return 0;

  if (this->k_array_[i])
    {
      this->k_array_[i].reset ();
      this->d_array_[i].reset ();
      return 1;
    }

  return 0;
}

template <class KEY, class DATA> void
JAWS_Assoc_Array<KEY,DATA>::clear ()
{
  for (int i = 0; i < this->maxsize_; ++i)
    {
      if (this->k_array_[i])
        {
          this->k_array_[i].reset ();
          this->d_array_[i].reset ();
        }
    }
}

template <class KEY, class DATA> int
JAWS_Assoc_Array<KEY,DATA>::find_i (const KEY &k)
{
  int j = this->maxsize_;

  for (int i = 0; i < this->maxsize_; ++i)
    {
      KEY const * const kk = this->k_array_[i].get ();
      if (kk)
        {
          if (*kk == k)
            return i;
        }
      else if (j == this->maxsize_)
        j = i;
    }
  return j;
}

template <class KEY, class DATA>
JAWS_Assoc_Array_Iterator<KEY,DATA>::
JAWS_Assoc_Array_Iterator (const JAWS_Assoc_Array<KEY, DATA> &aa)
  : aa_ (aa),
    i_ (0),
    j_ (0)
{
}

template <class KEY, class DATA> KEY *
JAWS_Assoc_Array_Iterator<KEY,DATA>::key () const
{
  return this->aa_.k_array_[this->i_].get ();
}

template <class KEY, class DATA> DATA *
JAWS_Assoc_Array_Iterator<KEY,DATA>::data () const
{
  return this->aa_.d_array_[this->i_].get ();
}

template <class KEY, class DATA> int
JAWS_Assoc_Array_Iterator<KEY,DATA>::first ()
{
  this->i_ = 0;
  for (this->j_ = 0; this->j_ < this->aa_.maxsize_; ++this->j_)
    {
      if (this->aa_.k_array_[this->j_])
        break;
    }
  return this->next ();
}

template <class KEY, class DATA> int
JAWS_Assoc_Array_Iterator<KEY,DATA>::last ()
{
  this->j_ = this->aa_.maxsize_;
  for (this->i_ = this->aa_.maxsize_; this->i_ > 0; --this->i_)
    {
      if (this->aa_.k_array_[this->i_ - 1])
        break;
    }

  int const previous_index = this->i_;
  --this->i_;
  return previous_index > 0;
}

template <class KEY, class DATA> int
JAWS_Assoc_Array_Iterator<KEY,DATA>::next ()
{
  if (this->j_ < this->aa_.maxsize_)
    {
      this->i_ = this->j_;
      for (++this->j_; this->j_ < this->aa_.maxsize_; ++this->j_)
        {
          if (this->aa_.k_array_[this->j_])
            break;
        }
    }

  return (this->i_ < this->aa_.maxsize_);
}

template <class KEY, class DATA> int
JAWS_Assoc_Array_Iterator<KEY,DATA>::previous ()
{
  if (this->i_ > 0)
    {
      for (this->j_ = this->i_; this->i_ > 0; --this->i_)
        {
          if (this->aa_.k_array_[this->i_ - 1])
            break;
        }
    }

  if (this->i_ == 0)
    this->first ();
  else
    --this->i_;

  return 1;
}

template <class KEY, class DATA> bool
JAWS_Assoc_Array_Iterator<KEY,DATA>::is_done () const
{
  return (this->j_ == this->aa_.maxsize_);
}

#endif /* !defined (JAWS_ASSOC_ARRAY_CPP) */
