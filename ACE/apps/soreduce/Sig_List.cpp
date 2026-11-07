// File: Sig_List.cpp

// Author: Phil Mesnier

#include "ace/OS_NS_string.h"
#include "Sig_List.h"
#include <utility>

//-----------------------------------------------------------------------------

Sig_List::Sig_List (int cap)
  : size_(0),
    capacity_(cap),
    index_(0),
    has_nulls_ (0),
    modified_ (false),
    array_ (nullptr)
{
  array_.reset (new Signature*[capacity_] ());
}

Sig_List::~Sig_List ()
{
  for (int i = 0; i < size_; i++)
    if (array_[i] != nullptr)
      array_[i]->release ();
}

void
Sig_List::add (ACE_CString const &s)
{
  if (this->index_of (s) != -1)
    return;
  modified_ = true;
  if (has_nulls_)
    for (int i = 0; i < size_; i++)
      if (array_[i] == nullptr) {
        array_[i] = new Signature (s);
        has_nulls_ --;
        return;
      }
  if (size_ == capacity_) {
    int ncap = capacity_ * 2;
    std::unique_ptr<Signature *[]> narray (new Signature *[ncap]);
    ACE_OS::memcpy (narray.get (), array_.get (), capacity_ * sizeof (Signature *));
    array_ = std::move (narray);
    capacity_ = ncap;
  }
  array_[size_++] = new Signature(s);
}

void
Sig_List::add (Sig_List const &other)
{
  if (capacity_ < size_ + other.capacity_) {
    int ncap = size_ + other.capacity_ + 50;
    std::unique_ptr<Signature *[]> narray (new Signature *[ncap]);
    ACE_OS::memcpy (narray.get (), array_.get (), capacity_ * sizeof (Signature *));
    array_ = std::move (narray);
    capacity_ = ncap;
  }
  modified_ = true;
  for (int i = 0; i < other.size_; i++)
    if (other.array_[i] != nullptr &&
        this->index_of (other.array_[i]->name()) == -1)
      {
        if (!has_nulls_)
          array_[size_++] = other.array_[i]->dup();
        else
          for (int i = 0; i < size_; i++)
            if (array_[i] == nullptr)
              {
                array_[i] = other.array_[i]->dup();
                has_nulls_ --;
                break;
              }
      }
}

void
Sig_List::remove (Signature const &s)
{
  for (int i = 0; i < size_; i++)
    if (array_[i] != nullptr && array_[i]->name() == s.name())
      {
        array_[i]->release ();
        array_[i] = nullptr;
        modified_ = true;
        if (i == size_ - 1)
          size_--;
        else
          has_nulls_++;
        break;
      }
}

void
Sig_List::remove_current ()
{
  array_[index_]->release();
  array_[index_] = nullptr;
  modified_ = true;
  if (index_ == size_ - 1)
    size_--;
  else
    has_nulls_++;
}

int
Sig_List::index_of (Signature const *s)
{
  for (int i = 0; i < size_; i++)
    if (array_[i] != nullptr && array_[i]->name() == s->name()) {
      array_[i]->used();
      return i;
    }
  return -1;
}

int
Sig_List::index_of (ACE_CString const &s)
{
  for (int i = 0; i < size_; i++)
    if (array_[i] != nullptr && array_[i]->name() == s) {
      return i;
    }
  return -1;
}


Signature const *
Sig_List::first()
{
  for (index_ = 0; index_ < size_; index_++)
    if (array_[index_] != nullptr)
      return array_[index_];
  return nullptr;
}

Signature const *
Sig_List::next()
{
  for (++index_; index_ < size_; index_++)
    if (array_[index_] != nullptr)
      return array_[index_];
  return nullptr;
}

bool
Sig_List::hasmore ()
{
  return index_ < size_;
}

int
Sig_List::size()
{
  return size_;
}

bool
Sig_List::modified()
{
  bool rtn = modified_;
  modified_ = false;
  int insert = 0;
  if (has_nulls_) {
    for (int i = 0; i < size_; i++)
      if (array_[i] != nullptr) {
        if (i != insert) {
          array_[insert] = array_[i];
          array_[i] = nullptr;
        }
        insert++;
      }
    size_ = insert+1;
    has_nulls_ = 0;
  }
  return rtn;
}
