//
// $Id$
//

ACE_INLINE
TAO_RT_Current::TAO_RT_Current (TAO_Stub *stub)
  :  CORBA_Current (stub)
{}

ACE_INLINE
TAO_RT_Current::~TAO_RT_Current (void)
{}

// ****************************************************************

ACE_INLINE
TAO_RT_Current_var::TAO_RT_Current_var (void)
  : ptr_ (TAO_RT_Current::_nil ())
{}

ACE_INLINE
TAO_RT_Current_var::TAO_RT_Current_var (TAO_RT_Current_ptr p)
  : ptr_ (p)
{}

ACE_INLINE TAO_RT_Current_ptr
TAO_RT_Current_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
TAO_RT_Current_var::TAO_RT_Current_var (const TAO_RT_Current_var &p)
  : ptr_ (TAO_RT_Current::_duplicate (p.ptr ()))
{}

ACE_INLINE
TAO_RT_Current_var::~TAO_RT_Current_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE TAO_RT_Current_var &
TAO_RT_Current_var::operator= (TAO_RT_Current_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE TAO_RT_Current_var &
TAO_RT_Current_var::operator= (const TAO_RT_Current_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = TAO_RT_Current::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
TAO_RT_Current_var::operator const TAO_RT_Current_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
TAO_RT_Current_var::operator TAO_RT_Current_ptr &()
{
  return this->ptr_;
}

ACE_INLINE TAO_RT_Current_ptr
TAO_RT_Current_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE TAO_RT_Current_ptr
TAO_RT_Current_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE TAO_RT_Current_ptr &
TAO_RT_Current_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE TAO_RT_Current_ptr &
TAO_RT_Current_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = TAO_RT_Current::_nil ();
  return this->ptr_;
}

ACE_INLINE TAO_RT_Current_ptr
TAO_RT_Current_var::_retn (void)
{
  // yield ownership of managed obj reference
  TAO_RT_Current_ptr val = this->ptr_;
  this->ptr_ = TAO_RT_Current::_nil ();
  return val;
}

// ****************************************************************

ACE_INLINE
TAO_RT_Current_out::TAO_RT_Current_out (TAO_RT_Current_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = TAO_RT_Current::_nil ();
}

ACE_INLINE
TAO_RT_Current_out::TAO_RT_Current_out (TAO_RT_Current_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = TAO_RT_Current::_nil ();
}

ACE_INLINE
TAO_RT_Current_out::TAO_RT_Current_out (const TAO_RT_Current_out &p)
  : ptr_ (ACE_const_cast (RTCORBA::Current_out&,p).ptr_)
{}

ACE_INLINE TAO_RT_Current_out &
TAO_RT_Current_out::operator= (const TAO_RT_Current_out &p)
{
  this->ptr_ = ACE_const_cast (RTCORBA::Current_out&,p).ptr_;
  return *this;
}

ACE_INLINE TAO_RT_Current_out &
TAO_RT_Current_out::operator= (const TAO_RT_Current_var &p)
{
  this->ptr_ = TAO_RT_Current::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE TAO_RT_Current_out &
TAO_RT_Current_out::operator= (TAO_RT_Current_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
TAO_RT_Current_out::operator TAO_RT_Current_ptr &()
{
  return this->ptr_;
}

ACE_INLINE TAO_RT_Current_ptr &
TAO_RT_Current_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE TAO_RT_Current_ptr
TAO_RT_Current_out::operator-> (void)
{
  return this->ptr_;
}
