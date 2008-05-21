// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Statistic::Information_Type
TAO_Statistic::type (void) const
{
  return this->type_;
}

ACE_INLINE size_t
TAO_Statistic::count (void) const
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);
  
  return (this->type_ == TS_COUNTER
          ? static_cast<size_t> (this->last_)
          : this->index_);
}

ACE_INLINE double
TAO_Statistic::minimum_sample (void) const
{
  if (this->type_ == TS_LIST)
    {
      throw Invalid_Operation( );
    }

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);
  
  return this->minimum_;
}

ACE_INLINE double
TAO_Statistic::maximum_sample (void) const
{
  if (this->type_ == TS_LIST)
    {
      throw Invalid_Operation ();
    }

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);
  
  return this->maximum_;
}

ACE_INLINE double
TAO_Statistic::last_sample (void) const
{
  if (this->type_ == TS_LIST)
    {
      throw Invalid_Operation ();
    }

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);
  
  return this->last_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
