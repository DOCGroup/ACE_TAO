// $Id$
// This may look like C, but it's really -*- C++ -*-

// constructor
ACE_INLINE
CORBA_NamedValue::CORBA_NamedValue (void)
  : refcount_ (1),
    flags_ (0),
    name_ (0)
{
}

// return the name
ACE_INLINE const char*
CORBA_NamedValue::name (void) const
{
  return this->name_;
}

// return the value
ACE_INLINE CORBA::Any_ptr
CORBA_NamedValue::value (void) const
{
  return ACE_const_cast (CORBA::Any_ptr, &this->any_);
}

// return the flags
ACE_INLINE CORBA::Flags
CORBA_NamedValue::flags (void) const
{
  return this->flags_;
}

ACE_INLINE void
CORBA::release (CORBA::NamedValue_ptr nv)
{
  if (nv)
    nv->_decr_refcnt ();
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::NamedValue_ptr nv)
{
  return nv == 0;
}

ACE_INLINE CORBA_NamedValue*
CORBA_NamedValue::_duplicate (CORBA_NamedValue *x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}

ACE_INLINE CORBA_NamedValue*
CORBA_NamedValue::_nil (void)
{
  return 0;
}

// *************************************************************
// Inline operations for class CORBA_NamedValue_var
// *************************************************************

ACE_INLINE
CORBA_NamedValue_var::CORBA_NamedValue_var (void)
  : ptr_ (CORBA_NamedValue::_nil ())
{
}

ACE_INLINE
CORBA_NamedValue_var::CORBA_NamedValue_var (CORBA_NamedValue_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
CORBA_NamedValue_var::~CORBA_NamedValue_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_NamedValue_ptr
CORBA_NamedValue_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_NamedValue_var::CORBA_NamedValue_var (const CORBA_NamedValue_var &p)
  : ptr_ (CORBA_NamedValue::_duplicate (p.ptr ()))
{}

ACE_INLINE CORBA_NamedValue_var &
CORBA_NamedValue_var::operator= (CORBA_NamedValue_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_NamedValue_var &
CORBA_NamedValue_var::operator= (const CORBA_NamedValue_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA_NamedValue::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_NamedValue_var::operator const CORBA_NamedValue_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_NamedValue_var::operator CORBA_NamedValue_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_NamedValue_ptr
CORBA_NamedValue_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_NamedValue_ptr
CORBA_NamedValue_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_NamedValue_ptr &
CORBA_NamedValue_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_NamedValue_ptr &
CORBA_NamedValue_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_NamedValue::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_NamedValue_ptr
CORBA_NamedValue_var::_retn (void)
{
  // yield ownership
  CORBA_NamedValue_ptr val = this->ptr_;
  this->ptr_ = CORBA_NamedValue::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_NamedValue_out
// *************************************************************

ACE_INLINE
CORBA_NamedValue_out::CORBA_NamedValue_out (CORBA_NamedValue_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA_NamedValue::_nil ();
}

ACE_INLINE
CORBA_NamedValue_out::CORBA_NamedValue_out (CORBA_NamedValue_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_NamedValue::_nil ();
}

ACE_INLINE
CORBA_NamedValue_out::CORBA_NamedValue_out (CORBA_NamedValue_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_NamedValue_out &
CORBA_NamedValue_out::operator= (CORBA_NamedValue_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_NamedValue_out &
CORBA_NamedValue_out::operator= (const CORBA_NamedValue_var &p)
{
  this->ptr_ = CORBA_NamedValue::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_NamedValue_out &
CORBA_NamedValue_out::operator= (CORBA_NamedValue_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_NamedValue_out::operator CORBA_NamedValue_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_NamedValue_ptr &
CORBA_NamedValue_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_NamedValue_ptr
CORBA_NamedValue_out::operator-> (void)
{
  return this->ptr_;
}

// ****************************************************************

// = methods for the NVList class

// constructor
ACE_INLINE
CORBA_NVList::CORBA_NVList (void)
  : max_ (0),
    refcount_ (1)
{
}

ACE_INLINE CORBA::ULong
CORBA_NVList::count (void) const
{
  return this->max_;
}

ACE_INLINE void
CORBA::release (CORBA::NVList_ptr nvl)
{
  if (nvl)
    nvl->_decr_refcnt ();
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::NVList_ptr nvl)
{
  return (CORBA::Boolean) (nvl == 0);
}

ACE_INLINE CORBA_NVList*
CORBA_NVList::_duplicate (CORBA_NVList *x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}

ACE_INLINE CORBA_NVList*
CORBA_NVList::_nil (void)
{
  return 0;
}

// *************************************************************
// Inline operations for class CORBA_NVList_var
// *************************************************************

ACE_INLINE
CORBA_NVList_var::CORBA_NVList_var (void)
  : ptr_ (CORBA_NVList::_nil ())
{
}

ACE_INLINE
CORBA_NVList_var::CORBA_NVList_var (CORBA_NVList_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
CORBA_NVList_var::~CORBA_NVList_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_NVList_ptr
CORBA_NVList_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_NVList_var::CORBA_NVList_var (const CORBA_NVList_var &p)
  : ptr_ (CORBA_NVList::_duplicate (p.ptr ()))
{}

ACE_INLINE CORBA_NVList_var &
CORBA_NVList_var::operator= (CORBA_NVList_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_NVList_var &
CORBA_NVList_var::operator= (const CORBA_NVList_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA_NVList::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_NVList_var::operator const CORBA_NVList_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_NVList_var::operator CORBA_NVList_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_NVList_ptr
CORBA_NVList_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_NVList_ptr
CORBA_NVList_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_NVList_ptr &
CORBA_NVList_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_NVList_ptr &
CORBA_NVList_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_NVList::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_NVList_ptr
CORBA_NVList_var::_retn (void)
{
  // yield ownership
  CORBA_NVList_ptr val = this->ptr_;
  this->ptr_ = CORBA_NVList::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_NVList_out
// *************************************************************

ACE_INLINE
CORBA_NVList_out::CORBA_NVList_out (CORBA_NVList_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA_NVList::_nil ();
}

ACE_INLINE
CORBA_NVList_out::CORBA_NVList_out (CORBA_NVList_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_NVList::_nil ();
}

ACE_INLINE
CORBA_NVList_out::CORBA_NVList_out (CORBA_NVList_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_NVList_out &
CORBA_NVList_out::operator= (CORBA_NVList_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_NVList_out &
CORBA_NVList_out::operator= (const CORBA_NVList_var &p)
{
  this->ptr_ = CORBA_NVList::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_NVList_out &
CORBA_NVList_out::operator= (CORBA_NVList_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_NVList_out::operator CORBA_NVList_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA_NVList_ptr &
CORBA_NVList_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_NVList_ptr
CORBA_NVList_out::operator-> (void)
{
  return this->ptr_;
}
