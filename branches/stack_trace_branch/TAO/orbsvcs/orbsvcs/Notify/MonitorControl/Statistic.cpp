// $Id$

#include "orbsvcs/orbsvcs/Notify/MonitorControl/Statistic.h"

#include "tao/String_Alloc.h"

#if !defined (__ACE_INLINE__)
# include "Statistic.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Statistic::TAO_Statistic (const char* name,
                              TAO_Statistic::Information_Type type)
 : Monitor_Base (name),
   type_ (type),
   index_ (0UL),
   minimum_set_ (false),
   minimum_ (0.0),
   maximum_ (0.0),
   sum_ (0.0),
   sum_of_squares_ (0.0),
   last_ (0.0)
{
}

TAO_Statistic::~TAO_Statistic (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->mutex_);
  
  if (this->type_ == TS_LIST)
    {
      for (size_t i = 0UL; i < this->index_; ++i)
        {
          delete [] this->data_[i];
        }
    }
}

void
TAO_Statistic::calculate (void)
{
}

void
TAO_Statistic::receive (double data)
{
  if (this->type_ == TS_LIST)
    {
      throw Invalid_Operation ();
    }

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->mutex_);
  
  if (this->type_ != TS_COUNTER)
    {
      this->sum_ += data;
      this->sum_of_squares_ += (data * data);
      ++this->index_;
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
        {
          this->maximum_ = data;
        }
    }
}

void
TAO_Statistic::receive (size_t data)
{
  double converted = static_cast<double> (data);
  this->receive (converted);
}

void
TAO_Statistic::receive (const TAO_Statistic::List& data)
{
  if (this->type_ != TS_LIST)
    {
      throw Invalid_Operation ();
    }

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->mutex_);
  
  for (size_t i = 0UL; i < this->index_; ++i)
    {
      delete [] this->data_[i];
    }

  this->index_ = data.size ();
  this->data_.max_size (this->index_);
  
  for (size_t i = 0UL; i < this->index_; ++i)
    {
      this->data_[i] = CORBA::string_dup (data[i].c_str ());
    }
}

void
TAO_Statistic::clear (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->mutex_);
  
  // If the type is a string list, we need to delete the data
  // before we change the index.
  if (this->type_ == TS_LIST)
    {
      for (size_t i = 0UL; i < this->index_; ++i)
        {
          delete [] this->data_[i];
        }
    }

  this->index_ = 0UL;
  this->minimum_set_ = false;
  this->minimum_ = 0.0;
  this->maximum_ = 0.0;
  this->sum_ = 0.0;
  this->sum_of_squares_ = 0.0;
  this->last_ = 0.0;
}

double
TAO_Statistic::average (void) const
{
  if (this->type_ == TS_COUNTER || this->type_ == TS_LIST)
    {
      throw Invalid_Operation ();
    }

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->mutex_, 0);
  
  return (this->index_== 0UL ? 0.0 : this->sum_ / this->index_);
}

double
TAO_Statistic::sum_of_squares (void) const
{
  if (this->type_ == TS_COUNTER || this->type_ == TS_LIST)
    {
      throw Invalid_Operation ();
    }

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->mutex_, 0);
  
  return this->sum_of_squares_;
}

TAO_Statistic::List
TAO_Statistic::get_list (void) const
{
  if (this->type_ != TS_LIST)
    {
      throw Invalid_Operation ();
    }

  List list;
  
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->mutex_, list);
  
  for (size_t i = 0UL; i < this->index_; ++i)
    {
      list.push_back (this->data_[i]);
    }
    
  return list;
}

TAO_END_VERSIONED_NAMESPACE_DECL
