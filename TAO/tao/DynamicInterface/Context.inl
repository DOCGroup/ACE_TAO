// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::Context_ptr
CORBA::Context::_duplicate (CORBA::Context_ptr x)
{
  if (x != 0)
    {
      x->_incr_refcnt ();
    }

  return x;
}

ACE_INLINE CORBA::Context_ptr
CORBA::Context::_nil (void)
{
  return (CORBA::Context_ptr)0;
}

// *************************************************************
// Inline operations for class CORBA::Context_var
// *************************************************************

ACE_INLINE
CORBA::Context_var::Context_var (void)
  : ptr_ (CORBA::Context::_nil ())
{
}

ACE_INLINE
CORBA::Context_var::Context_var (CORBA::Context_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
CORBA::Context_var::~Context_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA::Context_ptr
CORBA::Context_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::Context_var::Context_var (const CORBA::Context_var &p)
  : ptr_ (CORBA::Context::_duplicate (p.ptr ()))
{}

ACE_INLINE CORBA::Context_var &
CORBA::Context_var::operator= (CORBA::Context_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA::Context_var &
CORBA::Context_var::operator= (const CORBA::Context_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA::Context::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA::Context_var::operator const CORBA::Context_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::Context_var::operator CORBA::Context_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA::Context_ptr
CORBA::Context_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::Context_ptr
CORBA::Context_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::Context_ptr &
CORBA::Context_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::Context_ptr &
CORBA::Context_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA::Context::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA::Context_ptr
CORBA::Context_var::_retn (void)
{
  // yield ownership
  CORBA::Context_ptr val = this->ptr_;
  this->ptr_ = CORBA::Context::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA::Context_out
// *************************************************************

ACE_INLINE
CORBA::Context_out::Context_out (CORBA::Context_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA::Context::_nil ();
}

ACE_INLINE
CORBA::Context_out::Context_out (CORBA::Context_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA::Context::_nil ();
}

ACE_INLINE
CORBA::Context_out::Context_out (CORBA::Context_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA::Context_out &
CORBA::Context_out::operator= (CORBA::Context_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA::Context_out &
CORBA::Context_out::operator= (const CORBA::Context_var &p)
{
  this->ptr_ = CORBA::Context::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA::Context_out &
CORBA::Context_out::operator= (CORBA::Context_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA::Context_out::operator CORBA::Context_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA::Context_ptr &
CORBA::Context_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::Context_ptr
CORBA::Context_out::operator-> (void)
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class CORBA::ContextList
// *************************************************************

ACE_INLINE
CORBA::ContextList::ContextList (void)
{
}

ACE_INLINE CORBA::ULong
CORBA::ContextList::count (void)
{
  return (CORBA::ULong) this->ctx_list_.size ();
}

ACE_INLINE CORBA::ContextList_ptr
CORBA::ContextList::_nil (void)
{
  return (CORBA::ContextList_ptr)0;
}

ACE_INLINE CORBA::ContextList *
CORBA::ContextList::_duplicate (CORBA::ContextList* x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}

// *************************************************************
// Inline operations for class CORBA::ContextList_var
// *************************************************************

// default constructor

ACE_INLINE
CORBA::ContextList_var::ContextList_var (void)
  : ptr_ (CORBA::ContextList::_nil ())
{}

ACE_INLINE
CORBA::ContextList_var::ContextList_var (CORBA::ContextList_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA::ContextList_ptr
CORBA::ContextList_var::ptr (void) const
{
  return this->ptr_;
}

// copy constructor
ACE_INLINE
CORBA::ContextList_var::ContextList_var (const CORBA::ContextList_var &p)
  : ptr_ (p.ptr_->_duplicate ())
{}

ACE_INLINE
CORBA::ContextList_var::~ContextList_var (void) // destructor
{
  this->ptr_->_destroy ();
}

ACE_INLINE CORBA::ContextList_var &
CORBA::ContextList_var::operator= (CORBA::ContextList_ptr p)
{
  this->ptr_->_destroy ();
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA::ContextList_var &
CORBA::ContextList_var::operator= (const CORBA::ContextList_var &p)
{
  if (this != &p)
    {
      this->ptr_->_destroy ();
      this->ptr_ = p.ptr_->_duplicate ();
    }
  return *this;
}

ACE_INLINE
CORBA::ContextList_var::operator const CORBA::ContextList_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA::ContextList_var::operator CORBA::ContextList_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::ContextList_ptr
CORBA::ContextList_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ContextList_ptr
CORBA::ContextList_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ContextList_ptr &
CORBA::ContextList_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::ContextList_ptr &
CORBA::ContextList_var::out (void)
{
  this->ptr_->_destroy ();
  this->ptr_ = CORBA::ContextList::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA::ContextList_ptr
CORBA::ContextList_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA::ContextList_ptr val = this->ptr_;
  this->ptr_ = CORBA::ContextList::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA::ContextList_out
// *************************************************************

ACE_INLINE
CORBA::ContextList_out::ContextList_out (CORBA::ContextList_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA::ContextList::_nil ();
}

ACE_INLINE
CORBA::ContextList_out::ContextList_out (CORBA::ContextList_var &p)
  : ptr_ (p.out ())
{
  this->ptr_->_destroy ();
  this->ptr_ = CORBA::ContextList::_nil ();
}

ACE_INLINE
CORBA::ContextList_out::ContextList_out (CORBA::ContextList_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA::ContextList_out &
CORBA::ContextList_out::operator= (CORBA::ContextList_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA::ContextList_out &
CORBA::ContextList_out::operator= (const CORBA::ContextList_var &p)
{
  this->ptr_ = p.ptr ()->_duplicate ();
  return *this;
}

ACE_INLINE CORBA::ContextList_out &
CORBA::ContextList_out::operator= (CORBA::ContextList_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA::ContextList_out::operator CORBA::ContextList_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA::ContextList_ptr &
CORBA::ContextList_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::ContextList_ptr
CORBA::ContextList_out::operator-> (void)
{
  return this->ptr_;
}
