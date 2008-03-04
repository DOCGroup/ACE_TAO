// $Id$
#include "orbsvcs/Notify/MonitorControl/Statistic.h"
#include "tao/SystemException.h"

#if !defined (__ACE_INLINE__)
# include "Statistic.inl"
#endif /* ! __ACE_INLINE__ */

static size_t Growth_Size = 32;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Statistic::TAO_Statistic (const char* name,
                              TAO_Statistic::Information_Type type)
 : TAO_NS_Generic (name),
   type_ (type),
   index_ (0),
   minimum_set_ (false),
   minimum_ (0),
   maximum_ (0),
   last_ (0)
{
}

TAO_Statistic::~TAO_Statistic (void)
{
  ACE_WRITE_GUARD (ACE_SYNCH_RW_MUTEX, guard, this->mutex_);
  if (this->type_ == TS_LIST)
    for(size_t i = 0; i < this->index_; i++)
      {
        delete [] this->data_[i].string_;
      }
}

void
TAO_Statistic::calculate (void)
{
}

void
TAO_Statistic::receive (double data)
{
  ACE_WRITE_GUARD (ACE_SYNCH_RW_MUTEX, guard, this->mutex_);
  if (this->type_ == TS_LIST)
    throw Invalid_Operation();

  if (this->type_ != TS_COUNTER)
    {
      size_t max = this->data_.max_size();
      if (this->index_ >= max)
        {
          if (this->data_.max_size(max + Growth_Size) == -1)
            {
              throw CORBA::NO_MEMORY();
            }
        }
      this->data_[this->index_++].number_ = data;
    }

  if (this->type_ == TS_COUNTER)
    {
      ++this->last_;
      this->maximum_ = last_;
    }
  else
    {
      this->last_ = data;
      if (!this->minimum_set_)
        {
          this->minimum_set_ = true;
          this->minimum_ = data;
        }
      else if (this->minimum_ > data)
        {
          this->minimum_ = data;
        }
      if (this->maximum_ < data)
        this->maximum_ = data;
    }
}

void
TAO_Statistic::receive (const TAO_Statistic::List& data)
{
  if (this->type_ != TS_LIST)
    throw Invalid_Operation();

  ACE_WRITE_GUARD (ACE_SYNCH_RW_MUTEX, guard, this->mutex_);
  for(size_t i = 0; i < this->index_; i++)
    {
      delete [] this->data_[i].string_;
    }

  this->index_ = data.size ();
  this->data_.max_size (this->index_);
  for(size_t i = 0; i < this->index_; i++)
    {
      this->data_[i].string_ = CORBA::string_dup (data[i].c_str ());
    }
}

void
TAO_Statistic::clear (void)
{
  ACE_WRITE_GUARD (ACE_SYNCH_RW_MUTEX, guard, this->mutex_);
  // If the type is a string list, we need to delete the data
  // before we change the index
  if (this->type_ == TS_LIST)
    for(size_t i = 0; i < this->index_; i++)
      {
        delete [] this->data_[i].string_;
      }

  this->index_ = 0;
  this->minimum_set_ = false;
  this->minimum_ = 0;
  this->maximum_ = 0;
  this->last_ = 0;
}

double
TAO_Statistic::average (void) const
{
  if (this->type_ == TS_COUNTER || this->type_ == TS_LIST)
    throw Invalid_Operation();

  ACE_READ_GUARD_RETURN (ACE_SYNCH_RW_MUTEX, guard, this->mutex_, 0);

  double accum = 0;
  for(size_t i = 0; i < this->index_; i++)
    {
      accum += this->data_[i].number_;
    }
  return (this->index_ == 0 ? 0.0 :accum / this->index_);
}

double
TAO_Statistic::sum_of_squares (void) const
{
  if (this->type_ == TS_COUNTER || this->type_ == TS_LIST)
    throw Invalid_Operation();

  ACE_READ_GUARD_RETURN (ACE_SYNCH_RW_MUTEX, guard, this->mutex_, 0);

  double accum = 0;
  for(size_t i = 0; i < this->index_; i++)
    {
      double data = this->data_[i].number_;
      accum += (data * data);
    }
  return accum;
}

TAO_Statistic::List
TAO_Statistic::get_list (void) const
{
  if (this->type_ != TS_LIST)
    throw Invalid_Operation();

  List list;
  ACE_READ_GUARD_RETURN (ACE_SYNCH_RW_MUTEX, guard, this->mutex_, list);
  for(size_t i = 0; i < this->index_; i++)
    {
      list.push_back (this->data_[i].string_);
    }
  return list;

}
TAO_END_VERSIONED_NAMESPACE_DECL
