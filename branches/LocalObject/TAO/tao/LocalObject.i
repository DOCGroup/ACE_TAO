// $Id$

// ****************************************************************

ACE_INLINE CORBA::LocalObject_ptr
CORBA::LocalObject::_duplicate (CORBA::LocalObject_ptr obj)
{
  if (obj)
    obj->_add_ref ();
  return obj;
}

// Null pointers represent nil objects.

ACE_INLINE CORBA::LocalObject_ptr
CORBA::LocalObject::_nil (void)
{
  return 0;
}

ACE_INLINE CORBA::LocalObject_ptr
CORBA::LocalObject::_narrow (CORBA::Object_ptr obj, CORBA::Environment& ACE_TRY_ENV)
{
  if (obj->_is_local () != 0)
    return
      ACE_reinterpret_cast (CORBA::LocalObject_ptr,
                            obj->_tao_QueryInterface
                              (ACE_reinterpret_cast (ptr_arith_t,
                                                     &CORBA::LocalObject::_narrow)));
  return 0;
}

ACE_INLINE CORBA::LocalObject_ptr
CORBA::LocalObject::_unchecked_narrow (CORBA::Object_ptr obj, CORBA::Environment&)
{
  if (obj->_is_local () != 0)
    return
      ACE_reinterpret_cast (CORBA::LocalObject_ptr,
                            obj->_tao_QueryInterface
                              (ACE_reinterpret_cast (ptr_arith_t,
                                                     &CORBA::LocalObject::_narrow)));
  return 0;
}

ACE_INLINE
CORBA::LocalObject::LocalObject ()
  : ACE_NESTED_CLASS (CORBA, Object (0))
{
}

// *************************************************************
// Inline operations for class CORBA::LocalObject_var
// *************************************************************

ACE_INLINE
CORBA::LocalObject_var::LocalObject_var (void)
  : ptr_ (CORBA::LocalObject::_nil ())
{
}

ACE_INLINE
CORBA::LocalObject_var::LocalObject_var (CORBA::LocalObject_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
CORBA::LocalObject_var::~LocalObject_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA::LocalObject_ptr
CORBA::LocalObject_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::LocalObject_var::LocalObject_var (const CORBA::LocalObject_var &p) // copy constructor
        : ptr_ (CORBA::LocalObject::_duplicate (p.ptr ()))
{}

ACE_INLINE CORBA::LocalObject_var &
CORBA::LocalObject_var::operator= (CORBA::LocalObject_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA::LocalObject_var &
CORBA::LocalObject_var::operator= (const CORBA::LocalObject_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA::LocalObject::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA::LocalObject_var::operator const CORBA::LocalObject_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA::LocalObject_var::operator CORBA::LocalObject_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::LocalObject_ptr
CORBA::LocalObject_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::LocalObject_ptr
CORBA::LocalObject_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::LocalObject_ptr &
CORBA::LocalObject_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::LocalObject_ptr &
CORBA::LocalObject_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA::LocalObject::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA::LocalObject_ptr
CORBA::LocalObject_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA::LocalObject_ptr val = this->ptr_;
  this->ptr_ = CORBA::LocalObject::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA::LocalObject_out
// *************************************************************

ACE_INLINE
CORBA::LocalObject_out::LocalObject_out (CORBA::LocalObject_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA::LocalObject::_nil ();
}

ACE_INLINE
CORBA::LocalObject_out::LocalObject_out (CORBA::LocalObject_var &p) // constructor from _var
        : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA::LocalObject::_nil ();
}

ACE_INLINE
CORBA::LocalObject_out::LocalObject_out (const CORBA::LocalObject_out &p) // copy constructor
        : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA::LocalObject_out &
CORBA::LocalObject_out::operator= (const CORBA::LocalObject_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA::LocalObject_out &
CORBA::LocalObject_out::operator= (const CORBA::LocalObject_var &p)
{
  this->ptr_ = CORBA::LocalObject::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA::LocalObject_out &
CORBA::LocalObject_out::operator= (CORBA::LocalObject_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA::LocalObject_out::operator CORBA::LocalObject_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::LocalObject_ptr &
CORBA::LocalObject_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA::LocalObject_ptr
CORBA::LocalObject_out::operator-> (void)
{
  return this->ptr_;
}
