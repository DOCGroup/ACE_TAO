// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::NamedValue_ptr nv)
{
  return nv == 0;
}

ACE_INLINE void
CORBA::release (CORBA::NamedValue_ptr nv)
{
  if (nv)
    nv->_decr_refcnt ();
}

// constructor
ACE_INLINE
CORBA::NamedValue::NamedValue (void)
  : refcount_ (1),
    flags_ (0),
    name_ (0)
{
}

// return the name
ACE_INLINE const char*
CORBA::NamedValue::name (void) const
{
  return this->name_;
}

// return the value
ACE_INLINE CORBA::Any_ptr
CORBA::NamedValue::value (void) const
{
  return ACE_const_cast (CORBA::Any_ptr, &this->any_);
}

// return the flags
ACE_INLINE CORBA::Flags
CORBA::NamedValue::flags (void) const
{
  return this->flags_;
}

ACE_INLINE CORBA::NamedValue*
CORBA::NamedValue::_duplicate (CORBA::NamedValue *x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}

ACE_INLINE CORBA::NamedValue*
CORBA::NamedValue::_nil (void)
{
  return 0;
}

// *************************************************************
// Inline operations for class CORBA::NamedValue_var
// *************************************************************

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::NVList_ptr nvl)
{
  return (CORBA::Boolean) (nvl == 0);
}

ACE_INLINE void
CORBA::release (CORBA::NVList_ptr nvl)
{
  if (nvl)
    nvl->_decr_refcnt ();
}

ACE_INLINE
CORBA::NamedValue_var::NamedValue_var (void)
  : ptr_ (CORBA::NamedValue::_nil ())
{
}

ACE_INLINE
CORBA::NamedValue_var::NamedValue_var (CORBA::NamedValue_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
CORBA::NamedValue_var::~NamedValue_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA::NamedValue_ptr
CORBA::NamedValue_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::NamedValue_var::NamedValue_var (const CORBA::NamedValue_var &p)
  : ptr_ (CORBA::NamedValue::_duplicate (p.ptr ()))
{}

ACE_INLINE CORBA::NamedValue_var &
CORBA::NamedValue_var::operator= (CORBA::NamedValue_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA::NamedValue_var &
CORBA::NamedValue_var::operator= (const CORBA::NamedValue_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA::NamedValue::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA::NamedValue_var::operator const CORBA::NamedValue_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::NamedValue_var::operator CORBA::NamedValue_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA::NamedValue_ptr
CORBA::NamedValue_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::NamedValue_ptr
CORBA::NamedValue_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::NamedValue_ptr &
CORBA::NamedValue_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::NamedValue_ptr &
CORBA::NamedValue_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA::NamedValue::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA::NamedValue_ptr
CORBA::NamedValue_var::_retn (void)
{
  // yield ownership
  CORBA::NamedValue_ptr val = this->ptr_;
  this->ptr_ = CORBA::NamedValue::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA::NamedValue_out
// *************************************************************

ACE_INLINE
CORBA::NamedValue_out::NamedValue_out (CORBA::NamedValue_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA::NamedValue::_nil ();
}

ACE_INLINE
CORBA::NamedValue_out::NamedValue_out (CORBA::NamedValue_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA::NamedValue::_nil ();
}

ACE_INLINE
CORBA::NamedValue_out::NamedValue_out (CORBA::NamedValue_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA::NamedValue_out &
CORBA::NamedValue_out::operator= (CORBA::NamedValue_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA::NamedValue_out &
CORBA::NamedValue_out::operator= (const CORBA::NamedValue_var &p)
{
  this->ptr_ = CORBA::NamedValue::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA::NamedValue_out &
CORBA::NamedValue_out::operator= (CORBA::NamedValue_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA::NamedValue_out::operator CORBA::NamedValue_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA::NamedValue_ptr &
CORBA::NamedValue_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::NamedValue_ptr
CORBA::NamedValue_out::operator-> (void)
{
  return this->ptr_;
}

// ****************************************************************

// = methods for the NVList class

// constructor
ACE_INLINE
CORBA::NVList::NVList (void)
  : max_ (0),
    refcount_ (1),
    incoming_ (0),
    incoming_flag_ (0)
{
}

ACE_INLINE CORBA::ULong
CORBA::NVList::count (ACE_ENV_SINGLE_ARG_DECL) const
{
  (ACE_const_cast (CORBA::NVList *,
                   this))->evaluate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->max_;
}

ACE_INLINE CORBA::NVList*
CORBA::NVList::_duplicate (CORBA::NVList *x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}

ACE_INLINE CORBA::NVList*
CORBA::NVList::_nil (void)
{
  return 0;
}

// *************************************************************
// Inline operations for class CORBA::NVList_var
// *************************************************************

ACE_INLINE
CORBA::NVList_var::NVList_var (void)
  : ptr_ (CORBA::NVList::_nil ())
{
}

ACE_INLINE
CORBA::NVList_var::NVList_var (CORBA::NVList_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
CORBA::NVList_var::~NVList_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA::NVList_ptr
CORBA::NVList_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::NVList_var::NVList_var (const CORBA::NVList_var &p)
  : ptr_ (CORBA::NVList::_duplicate (p.ptr ()))
{}

ACE_INLINE CORBA::NVList_var &
CORBA::NVList_var::operator= (CORBA::NVList_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA::NVList_var &
CORBA::NVList_var::operator= (const CORBA::NVList_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA::NVList::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA::NVList_var::operator const CORBA::NVList_ptr &() const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::NVList_var::operator CORBA::NVList_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA::NVList_ptr
CORBA::NVList_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::NVList_ptr
CORBA::NVList_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::NVList_ptr &
CORBA::NVList_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::NVList_ptr &
CORBA::NVList_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA::NVList::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA::NVList_ptr
CORBA::NVList_var::_retn (void)
{
  // yield ownership
  CORBA::NVList_ptr val = this->ptr_;
  this->ptr_ = CORBA::NVList::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA::NVList_out
// *************************************************************

ACE_INLINE
CORBA::NVList_out::NVList_out (CORBA::NVList_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA::NVList::_nil ();
}

ACE_INLINE
CORBA::NVList_out::NVList_out (CORBA::NVList_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA::NVList::_nil ();
}

ACE_INLINE
CORBA::NVList_out::NVList_out (CORBA::NVList_out &p)
  : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA::NVList_out &
CORBA::NVList_out::operator= (CORBA::NVList_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA::NVList_out &
CORBA::NVList_out::operator= (const CORBA::NVList_var &p)
{
  this->ptr_ = CORBA::NVList::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA::NVList_out &
CORBA::NVList_out::operator= (CORBA::NVList_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA::NVList_out::operator CORBA::NVList_ptr &()
{
  return this->ptr_;
}

ACE_INLINE CORBA::NVList_ptr &
CORBA::NVList_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::NVList_ptr
CORBA::NVList_out::operator-> (void)
{
  return this->ptr_;
}
