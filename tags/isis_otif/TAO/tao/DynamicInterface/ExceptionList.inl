// -*- C++ -*-
//
// $Id$

ACE_INLINE
CORBA::ExceptionList::ExceptionList (void)
  : ref_count_ (1)
{
}

ACE_INLINE CORBA::ULong
CORBA::ExceptionList::count (void)
{
  return (CORBA::ULong) this->tc_list_.size ();
}

ACE_INLINE CORBA::ExceptionList_ptr
CORBA::ExceptionList::_nil (void)
{
  return (CORBA::ExceptionList_ptr)0;
}

ACE_INLINE CORBA::ExceptionList *
CORBA::ExceptionList::_duplicate (CORBA::ExceptionList* x)
{
  if (x != 0)
    {
      x->_incr_refcnt ();
    }

  return x;
}

ACE_INLINE
CORBA::ExceptionList_var::ExceptionList_var (void) // default constructor
  : ptr_ (CORBA::ExceptionList::_nil ())
{}

ACE_INLINE
CORBA::ExceptionList_var::ExceptionList_var (CORBA::ExceptionList_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA::ExceptionList_ptr
CORBA::ExceptionList_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::ExceptionList_var::ExceptionList_var (const CORBA::ExceptionList_var &p) // copy constructor
  : ptr_ (CORBA::ExceptionList::_duplicate (p.ptr_))
{
}

ACE_INLINE
CORBA::ExceptionList_var::~ExceptionList_var (void) // destructor
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_destroy ();
    }
}

ACE_INLINE CORBA::ExceptionList_var &
CORBA::ExceptionList_var::operator= (CORBA::ExceptionList_ptr p)
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_destroy ();
    }

  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA::ExceptionList_var &
CORBA::ExceptionList_var::operator= (const CORBA::ExceptionList_var &p)
{
  if (this != &p)
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->_destroy ();
	}

      this->ptr_ = CORBA::ExceptionList::_duplicate (p.ptr_);
    }

  return *this;
}

ACE_INLINE
CORBA::ExceptionList_var::operator const CORBA::ExceptionList_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::ExceptionList_var::operator CORBA::ExceptionList_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA::ExceptionList_ptr
CORBA::ExceptionList_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ExceptionList_ptr
CORBA::ExceptionList_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ExceptionList_ptr &
CORBA::ExceptionList_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::ExceptionList_ptr &
CORBA::ExceptionList_var::out (void)
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_destroy ();
    }

  this->ptr_ = CORBA::ExceptionList::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA::ExceptionList_ptr
CORBA::ExceptionList_var::_retn (void)
{
  CORBA::ExceptionList_ptr val = this->ptr_;
  this->ptr_ = CORBA::ExceptionList::_nil ();
  return val;
}
