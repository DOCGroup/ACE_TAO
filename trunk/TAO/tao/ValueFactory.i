// $Id$

ACE_INLINE
CORBA_ValueFactoryBase::CORBA_ValueFactoryBase ()
  : _tao_reference_count_ (1)
{
}

ACE_INLINE void
CORBA_ValueFactoryBase::_add_ref (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->_tao_reference_count_lock_);
  ++_tao_reference_count_;
}

ACE_INLINE void
CORBA_ValueFactoryBase::_remove_ref (void)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->_tao_reference_count_lock_);
    -- this->_tao_reference_count_;
    if (this->_tao_reference_count_ != 0)
      return;
  }
  delete this;
}

// *************************************************************
// Inline operations for class CORBA_ValueFactoryBase_var
// *************************************************************

ACE_INLINE
CORBA_ValueFactoryBase_var::CORBA_ValueFactoryBase_var (void)
  : ptr_ (0)
{
}

ACE_INLINE
CORBA_ValueFactoryBase_var::CORBA_ValueFactoryBase_var (
    CORBA::ValueFactoryBase *p
  )
  : ptr_ (p)
{
}

ACE_INLINE
CORBA_ValueFactoryBase_var::~CORBA_ValueFactoryBase_var (void)
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_remove_ref ();
    }
}

ACE_INLINE CORBA::ValueFactoryBase *
CORBA_ValueFactoryBase_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ValueFactoryBase_var::CORBA_ValueFactoryBase_var (
    const CORBA_ValueFactoryBase_var &p
  )
  : ptr_ (p.ptr_)
{
  p.ptr_->_add_ref ();
}

ACE_INLINE CORBA_ValueFactoryBase_var &
CORBA_ValueFactoryBase_var::operator= (CORBA::ValueFactoryBase *p)
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_remove_ref ();
    }

  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_ValueFactoryBase_var &
CORBA_ValueFactoryBase_var::operator= (const CORBA_ValueFactoryBase_var &p)
{
  if (this != &p)
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->_remove_ref ();
        }

      p.ptr_->_add_ref ();
      this->ptr_ = p.ptr_;
    }

  return *this;
}

// cast
ACE_INLINE
CORBA_ValueFactoryBase_var::operator const CORBA::ValueFactoryBase *&() const
{
  return ACE_const_cast (const CORBA::ValueFactoryBase *&,
                         this->ptr_);
}

// cast
ACE_INLINE
CORBA_ValueFactoryBase_var::operator CORBA::ValueFactoryBase *&()
{
  return this->ptr_;
}

ACE_INLINE CORBA::ValueFactoryBase *
CORBA_ValueFactoryBase_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ValueFactoryBase *
CORBA_ValueFactoryBase_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ValueFactoryBase *&
CORBA_ValueFactoryBase_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::ValueFactoryBase *&
CORBA_ValueFactoryBase_var::out (void)
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_remove_ref ();
    }

  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CORBA::ValueFactoryBase *
CORBA_ValueFactoryBase_var::_retn (void)
{
  // Yield ownership of valuebase.
  CORBA::ValueFactoryBase *val = this->ptr_;
  this->ptr_ = 0;
  return val;
}

