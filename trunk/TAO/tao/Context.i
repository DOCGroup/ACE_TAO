// $Id$

// This may look like C, but it's really -*- C++ -*-

// These should never be non-NULL, but the method
// is required of pseudo objects.
ACE_INLINE void
CORBA::release (CORBA::Context_ptr ctx)
{
  if (ctx)
    ctx->_decr_refcnt ();
}

ACE_INLINE CORBA_Context*
CORBA_Context::_duplicate (CORBA_Context* x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}

ACE_INLINE CORBA_Context*
CORBA_Context::_nil (void)
{
  return (CORBA::Context*)0;
}

// *************************************************************
// Inline operations for class CORBA_Context_var
// *************************************************************

ACE_INLINE
CORBA_Context_var::CORBA_Context_var (void)
  : ptr_ (CORBA_Context::_nil ())
{
}

ACE_INLINE
CORBA_Context_var::CORBA_Context_var (CORBA_Context_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
CORBA_Context_var::~CORBA_Context_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_Context_ptr
CORBA_Context_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Context_var::CORBA_Context_var (const CORBA_Context_var &p)
  : ptr_ (CORBA_Context::_duplicate (p.ptr ()))
{}

ACE_INLINE CORBA_Context_var &
CORBA_Context_var::operator= (CORBA_Context_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_Context_var &
CORBA_Context_var::operator= (const CORBA_Context_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA_Context::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_Context_var::operator const CORBA_Context_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Context_var::operator CORBA_Context_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_Context_ptr
CORBA_Context_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Context_ptr
CORBA_Context_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Context_ptr &
CORBA_Context_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Context_ptr &
CORBA_Context_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_Context::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_Context_ptr
CORBA_Context_var::_retn (void)
{
  // yield ownership
  CORBA_Context_ptr val = this->ptr_;
  this->ptr_ = CORBA_Context::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_Context_out
// *************************************************************

ACE_INLINE
CORBA_Context_out::CORBA_Context_out (CORBA_Context_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA_Context::_nil ();
}

ACE_INLINE
CORBA_Context_out::CORBA_Context_out (CORBA_Context_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_Context::_nil ();
}

ACE_INLINE
CORBA_Context_out::CORBA_Context_out (CORBA_Context_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_Context_out &
CORBA_Context_out::operator= (CORBA_Context_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_Context_out &
CORBA_Context_out::operator= (const CORBA_Context_var &p)
{
  this->ptr_ = CORBA_Context::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_Context_out &
CORBA_Context_out::operator= (CORBA_Context_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_Context_out::operator CORBA_Context_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_Context_ptr &
CORBA_Context_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Context_ptr
CORBA_Context_out::operator-> (void)
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class CORBA_ContextList
// *************************************************************

ACE_INLINE
CORBA_ContextList::CORBA_ContextList (void)
{
}

ACE_INLINE CORBA::ULong
CORBA_ContextList::count (void)
{
  return (CORBA::ULong) this->ctx_list_.size ();
}

ACE_INLINE CORBA_ContextList_ptr
CORBA_ContextList::_nil (void)
{
  return (CORBA_ContextList_ptr)0;
}

// *************************************************************
// Inline operations for class CORBA_ContextList_var
// *************************************************************

ACE_INLINE
CORBA_ContextList_var::CORBA_ContextList_var (void) // default constructor
  : ptr_ (CORBA_ContextList::_nil ())
{}

ACE_INLINE
CORBA_ContextList_var::CORBA_ContextList_var (CORBA_ContextList_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA_ContextList_ptr
CORBA_ContextList_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ContextList_var::CORBA_ContextList_var (const CORBA_ContextList_var &p) // copy constructor
  : ptr_ (p.ptr_->_duplicate ())
{}

ACE_INLINE
CORBA_ContextList_var::~CORBA_ContextList_var (void) // destructor
{
  this->ptr_->_destroy ();
}

ACE_INLINE CORBA_ContextList_var &
CORBA_ContextList_var::operator= (CORBA_ContextList_ptr p)
{
  this->ptr_->_destroy ();
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_ContextList_var &
CORBA_ContextList_var::operator= (const CORBA_ContextList_var &p)
{
  if (this != &p)
    {
      this->ptr_->_destroy ();
      this->ptr_ = p.ptr_->_duplicate ();
    }
  return *this;
}

ACE_INLINE
CORBA_ContextList_var::operator const CORBA_ContextList_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ContextList_var::operator CORBA_ContextList_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_ContextList_ptr
CORBA_ContextList_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_ContextList_ptr
CORBA_ContextList_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_ContextList_ptr &
CORBA_ContextList_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_ContextList_ptr &
CORBA_ContextList_var::out (void)
{
  this->ptr_->_destroy ();
  this->ptr_ = CORBA_ContextList::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_ContextList_ptr
CORBA_ContextList_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA_ContextList_ptr val = this->ptr_;
  this->ptr_ = CORBA_ContextList::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_ContextList_out
// *************************************************************

ACE_INLINE
CORBA_ContextList_out::CORBA_ContextList_out (CORBA_ContextList_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA_ContextList::_nil ();
}

ACE_INLINE
CORBA_ContextList_out::CORBA_ContextList_out (CORBA_ContextList_var &p)
  : ptr_ (p.out ())
{
  this->ptr_->_destroy ();
  this->ptr_ = CORBA_ContextList::_nil ();
}

ACE_INLINE
CORBA_ContextList_out::CORBA_ContextList_out (CORBA_ContextList_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_ContextList_out &
CORBA_ContextList_out::operator= (CORBA_ContextList_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_ContextList_out &
CORBA_ContextList_out::operator= (const CORBA_ContextList_var &p)
{
  this->ptr_ = p.ptr ()->_duplicate ();
  return *this;
}

ACE_INLINE CORBA_ContextList_out &
CORBA_ContextList_out::operator= (CORBA_ContextList_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_ContextList_out::operator CORBA_ContextList_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_ContextList_ptr &
CORBA_ContextList_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_ContextList_ptr
CORBA_ContextList_out::operator-> (void)
{
  return this->ptr_;
}

