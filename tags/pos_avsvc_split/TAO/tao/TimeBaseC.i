// $Id$

// *************************************************************
// Inline operations for class TimeBase::UtcT_var
// *************************************************************

ACE_INLINE
TimeBase::UtcT_var::UtcT_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
TimeBase::UtcT_var::UtcT_var (TimeBase::UtcT *p)
  : ptr_ (p)
{}

ACE_INLINE
TimeBase::UtcT_var::UtcT_var (const TimeBase::UtcT_var &p) // copy constructor
{
  if (p.ptr_)
    this->ptr_ = new TimeBase::UtcT(*p.ptr_);
  else
    this->ptr_ = 0;
}

ACE_INLINE
TimeBase::UtcT_var::~UtcT_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE TimeBase::UtcT_var &
TimeBase::UtcT_var::operator= (TimeBase::UtcT *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE TimeBase::UtcT_var &
TimeBase::UtcT_var::operator= (const TimeBase::UtcT_var &p)
{
  if (this != &p)
  {
    delete this->ptr_;
    this->ptr_ = new TimeBase::UtcT (*p.ptr_);
  }
  return *this;
}

ACE_INLINE const TimeBase::UtcT *
TimeBase::UtcT_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE TimeBase::UtcT *
TimeBase::UtcT_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
TimeBase::UtcT_var::operator const TimeBase::UtcT &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
TimeBase::UtcT_var::operator TimeBase::UtcT &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
TimeBase::UtcT_var::operator TimeBase::UtcT &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE const TimeBase::UtcT &
TimeBase::UtcT_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE TimeBase::UtcT &
TimeBase::UtcT_var::inout (void)
{
  return *this->ptr_;
}

// mapping for fixed size
ACE_INLINE TimeBase::UtcT &
TimeBase::UtcT_var::out (void)
{
  return *this->ptr_;
}

ACE_INLINE TimeBase::UtcT
TimeBase::UtcT_var::_retn (void)
{
  return *this->ptr_;
}

ACE_INLINE TimeBase::UtcT *
TimeBase::UtcT_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class TimeBase::IntervalT_var
// *************************************************************

ACE_INLINE
TimeBase::IntervalT_var::IntervalT_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
TimeBase::IntervalT_var::IntervalT_var (TimeBase::IntervalT *p)
  : ptr_ (p)
{}

ACE_INLINE
TimeBase::IntervalT_var::IntervalT_var (const TimeBase::IntervalT_var &p) // copy constructor
{
  if (p.ptr_)
    this->ptr_ = new TimeBase::IntervalT(*p.ptr_);
  else
    this->ptr_ = 0;
}

ACE_INLINE
TimeBase::IntervalT_var::~IntervalT_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE TimeBase::IntervalT_var &
TimeBase::IntervalT_var::operator= (TimeBase::IntervalT *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE TimeBase::IntervalT_var &
TimeBase::IntervalT_var::operator= (const TimeBase::IntervalT_var &p)
{
  if (this != &p)
  {
    delete this->ptr_;
    this->ptr_ = new TimeBase::IntervalT (*p.ptr_);
  }
  return *this;
}

ACE_INLINE const TimeBase::IntervalT *
TimeBase::IntervalT_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE TimeBase::IntervalT *
TimeBase::IntervalT_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
TimeBase::IntervalT_var::operator const TimeBase::IntervalT &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
TimeBase::IntervalT_var::operator TimeBase::IntervalT &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
TimeBase::IntervalT_var::operator TimeBase::IntervalT &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE const TimeBase::IntervalT &
TimeBase::IntervalT_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE TimeBase::IntervalT &
TimeBase::IntervalT_var::inout (void)
{
  return *this->ptr_;
}

// mapping for fixed size
ACE_INLINE TimeBase::IntervalT &
TimeBase::IntervalT_var::out (void)
{
  return *this->ptr_;
}

ACE_INLINE TimeBase::IntervalT
TimeBase::IntervalT_var::_retn (void)
{
  return *this->ptr_;
}

ACE_INLINE TimeBase::IntervalT *
TimeBase::IntervalT_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const TimeBase::UtcT &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.time) &&
    (strm << _tao_aggregate.inacclo) &&
    (strm << _tao_aggregate.inacchi) &&
    (strm << _tao_aggregate.tdf)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, TimeBase::UtcT &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.time) &&
    (strm >> _tao_aggregate.inacclo) &&
    (strm >> _tao_aggregate.inacchi) &&
    (strm >> _tao_aggregate.tdf)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const TimeBase::IntervalT &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.lower_bound) &&
    (strm << _tao_aggregate.upper_bound)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, TimeBase::IntervalT &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.lower_bound) &&
    (strm >> _tao_aggregate.upper_bound)
  )
    return 1;
  else
    return 0;

}
