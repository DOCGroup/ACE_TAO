// File: Sig_List.cpp

// Author: Phil Mesnier

#include "Sig_List.h"
#include <algorithm>

//-----------------------------------------------------------------------------

Sig_List::Sig_List (int cap)
{
  this->array_.reserve (cap);
}

void
Sig_List::add (ACE_CString const &s)
{
  if (this->index_of (s) != -1)
    return;
  modified_ = true;
  if (has_nulls_)
    for (int i = 0; i < static_cast<int> (array_.size ()); i++)
      if (!array_[i]) {
        array_[i] = std::make_shared<Signature> (s);
        has_nulls_ --;
        return;
      }
  array_.push_back (std::make_shared<Signature> (s));
}

void
Sig_List::add (Sig_List const &other)
{
  modified_ = true;
  for (int i = 0; i < static_cast<int> (other.array_.size ()); i++)
    if (other.array_[i] &&
        this->index_of (other.array_[i]->name ()) == -1)
      {
        if (!has_nulls_)
          array_.push_back (other.array_[i]);
        else
          for (int j = 0; j < static_cast<int> (array_.size ()); j++)
            if (!array_[j])
              {
                array_[j] = other.array_[i];
                has_nulls_ --;
                break;
              }
      }
}

void
Sig_List::remove_current ()
{
  array_[index_].reset ();
  modified_ = true;
  if (index_ == static_cast<int> (array_.size ()) - 1)
    array_.pop_back ();
  else
    has_nulls_++;
}

int
Sig_List::index_of (Signature const *s)
{
  for (int i = 0; i < static_cast<int> (array_.size ()); i++)
    if (array_[i] && array_[i]->name () == s->name ()) {
      array_[i]->used();
      return i;
    }
  return -1;
}

int
Sig_List::index_of (ACE_CString const &s) const
{
  for (int i = 0; i < static_cast<int> (array_.size ()); i++)
    if (array_[i] && array_[i]->name () == s) {
      return i;
    }
  return -1;
}

Signature const *
Sig_List::first ()
{
  for (index_ = 0; index_ < static_cast<int> (array_.size ()); index_++)
    if (array_[index_])
      return array_[index_].get ();
  return nullptr;
}

Signature const *
Sig_List::next ()
{
  for (++index_; index_ < static_cast<int> (array_.size ()); index_++)
    if (array_[index_])
      return array_[index_].get ();
  return nullptr;
}

bool
Sig_List::hasmore () const
{
  return index_ < static_cast<int> (array_.size ());
}

int
Sig_List::size () const
{
  return static_cast<int> (array_.size ());
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
