// -*- C++ -*-
// $Id$

ACE_INLINE
CORBA_ExceptionList::CORBA_ExceptionList (void)
  : ref_count_ (1)
{
}

ACE_INLINE CORBA::ULong
CORBA_ExceptionList::count (void)
{
  return (CORBA::ULong) this->tc_list_.size ();
}

ACE_INLINE CORBA_ExceptionList_ptr
CORBA_ExceptionList::_nil (void)
{
  return (CORBA_ExceptionList_ptr)0;
}

ACE_INLINE CORBA_ExceptionList *
CORBA_ExceptionList::_duplicate (CORBA_ExceptionList* x)
{
  if (x != 0)
    {
      x->_incr_refcnt ();
    }

  return x;
}

ACE_INLINE
CORBA_ExceptionList_var::CORBA_ExceptionList_var (void) // default constructor
  : ptr_ (CORBA_ExceptionList::_nil ())
{}

ACE_INLINE
CORBA_ExceptionList_var::CORBA_ExceptionList_var (CORBA_ExceptionList_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA_ExceptionList_ptr
CORBA_ExceptionList_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ExceptionList_var::CORBA_ExceptionList_var (const CORBA_ExceptionList_var &p) // copy constructor
  : ptr_ (CORBA_ExceptionList::_duplicate (p.ptr_))
{
}

ACE_INLINE
CORBA_ExceptionList_var::~CORBA_ExceptionList_var (void) // destructor
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_destroy ();
    }
}

ACE_INLINE CORBA_ExceptionList_var &
CORBA_ExceptionList_var::operator= (CORBA_ExceptionList_ptr p)
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_destroy ();
    }

  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_ExceptionList_var &
CORBA_ExceptionList_var::operator= (const CORBA_ExceptionList_var &p)
{
  if (this != &p)
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->_destroy ();
	}

      this->ptr_ = CORBA_ExceptionList::_duplicate (p.ptr_);
    }

  return *this;
}

ACE_INLINE
CORBA_ExceptionList_var::operator const CORBA_ExceptionList_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ExceptionList_var::operator CORBA_ExceptionList_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_ExceptionList_ptr
CORBA_ExceptionList_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_ExceptionList_ptr
CORBA_ExceptionList_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_ExceptionList_ptr &
CORBA_ExceptionList_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_ExceptionList_ptr &
CORBA_ExceptionList_var::out (void)
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_destroy ();
    }

  this->ptr_ = CORBA_ExceptionList::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_ExceptionList_ptr
CORBA_ExceptionList_var::_retn (void)
{
  CORBA_ExceptionList_ptr val = this->ptr_;
  this->ptr_ = CORBA_ExceptionList::_nil ();
  return val;
}
