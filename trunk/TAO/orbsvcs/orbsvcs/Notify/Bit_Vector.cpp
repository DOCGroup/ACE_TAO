// $Id$

#include "orbsvcs/Notify/Bit_Vector.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{

Bit_Vector::Bit_Vector()
  : size_(0)
  , first_set_bit_(0)
  , first_cleared_bit_(0)
{
}

Bit_Vector::~Bit_Vector()
{
}

bool
Bit_Vector::is_set(const size_t location) const
{
  bool result = false;
  if (location < this->size_)
  {
    result = (0 != (this->bitvec_[location >> BPW_LOG_2] & (1 << (location % BITS_PER_WORD))));
  }
  return result;
}

void
Bit_Vector::set_bit(const size_t location, bool set)
{
  if (location >= this->size_)
  {
    if ((location >> BPW_LOG_2) >= (this->size_ >> BPW_LOG_2))
    {
      size_t need = (location >> BPW_LOG_2) - (this->size_ >> BPW_LOG_2);
      this->bitvec_.resize(this->bitvec_.size() + need + 1, 0);
    }
    this->size_ = location + 1;
  }
  if (set)
  {
    this->bitvec_[location >> BPW_LOG_2] |= (1 << (location % BITS_PER_WORD));
  }
  else
  {
    this->bitvec_[location >> BPW_LOG_2] &= ~(1 << (location % BITS_PER_WORD));
  }
  this->evaluate_firsts(location, set);
}

size_t
Bit_Vector::find_first_bit(bool set) const
{
  size_t result = 0;
  if (set)
  {
    result = this->first_set_bit_;
  }
  else
  {
    result = this->first_cleared_bit_;
  }
  return result;
}

void
Bit_Vector::evaluate_firsts(const size_t location, bool set)
{
  if (set)
  {
    if (this->first_cleared_bit_ == location)
    {
      this->first_cleared_bit_ = this->find_first_bit_of(location, false);
    }
    if (this->first_set_bit_ > location)
    {
      this->first_set_bit_ = location;
    }
  }
  else if (!set)
  {
    if (this->first_set_bit_ == location)
    {
      this->first_set_bit_ = this->find_first_bit_of(location, true);
    }
    if (this->first_cleared_bit_ > location)
    {
      this->first_cleared_bit_ = location;
    }
  }
}

size_t
Bit_Vector::find_first_bit_of(const size_t location, bool set)
{
  size_t newloc = 0;
  size_t idx = 0;
  for (idx = location; (newloc == 0) && (idx < this->size_ + 1); idx++)
  {
    if (is_set(idx) == set)
    {
      newloc = idx;
    }
  }
  return newloc;
}

}

TAO_END_VERSIONED_NAMESPACE_DECL
