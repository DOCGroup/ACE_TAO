// File: Sig_List.cpp

// Author: Phil Mesnier

#include "Sig_List.h"
#include <algorithm>

//-----------------------------------------------------------------------------

Sig_List::Sig_List (size_t cap)
{
  this->array_.reserve (cap);
}

void
Sig_List::add (ACE_CString const &s)
{
  if (this->contains (s))
    return;
  modified_ = true;
  if (has_nulls_)
    for (size_t i = 0; i < array_.size (); ++i)
      if (!array_[i]) {
        array_[i] = std::make_shared<Signature> (s);
        --has_nulls_;
        return;
      }
  array_.push_back (std::make_shared<Signature> (s));
}

void
Sig_List::add (Sig_List const &other)
{
  modified_ = true;
  for (size_t i = 0; i < other.array_.size (); ++i)
    if (other.array_[i] &&
        !this->contains (other.array_[i]->name ()))
      {
        if (!has_nulls_)
          array_.push_back (other.array_[i]);
        else
          for (size_t j = 0; j < array_.size (); ++j)
            if (!array_[j])
              {
                array_[j] = other.array_[i];
                --has_nulls_;
                break;
              }
      }
}

void
Sig_List::remove_current ()
{
  array_[index_].reset ();
  modified_ = true;
  if (index_ == array_.size () - 1)
    array_.pop_back ();
  else
    ++has_nulls_;
}

bool
Sig_List::find_and_mark_used (Signature const *s)
{
  for (size_t i = 0; i < array_.size (); ++i)
    if (array_[i] && array_[i]->name () == s->name ()) {
      array_[i]->used ();
      return true;
    }
  return false;
}

bool
Sig_List::contains (ACE_CString const &s) const
{
  for (size_t i = 0; i < array_.size (); ++i)
    if (array_[i] && array_[i]->name () == s) {
      return true;
    }
  return false;
}

Signature const *
Sig_List::first ()
{
  for (index_ = 0; index_ < array_.size (); ++index_)
    if (array_[index_])
      return array_[index_].get ();
  return nullptr;
}

Signature const *
Sig_List::next ()
{
  for (++index_; index_ < array_.size (); ++index_)
    if (array_[index_])
      return array_[index_].get ();
  return nullptr;
}

bool
Sig_List::hasmore () const
{
  return index_ < array_.size ();
}

size_t
Sig_List::size () const
{
  return array_.size ();
}

bool
Sig_List::modified()
{
  bool const rtn = modified_;
  modified_ = false;
  if (has_nulls_)
    {
      array_.erase (std::remove (array_.begin (), array_.end (), nullptr), array_.end ());
      array_.push_back (nullptr);
      has_nulls_ = 0;
    }
  return rtn;
}
