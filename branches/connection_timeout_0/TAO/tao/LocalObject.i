// -*- C++ -*-
//
// $Id$

// ****************************************************************

ACE_INLINE CORBA_LocalObject_ptr
CORBA_LocalObject::_duplicate (CORBA_LocalObject_ptr obj)
{
  if (obj)
    obj->_add_ref ();
  return obj;
}

// Null pointers represent nil objects.

ACE_INLINE CORBA_LocalObject_ptr
CORBA_LocalObject::_nil (void)
{
  return 0;
}

ACE_INLINE CORBA_LocalObject_ptr
CORBA_LocalObject::_narrow (CORBA_Object_ptr obj
                            ACE_ENV_ARG_DECL_NOT_USED)
{
  if (obj->_is_local () != 0)
    return
      ACE_reinterpret_cast (
        CORBA_LocalObject_ptr,
        obj->_tao_QueryInterface (
          ACE_reinterpret_cast (ptr_arith_t,
                                &CORBA_LocalObject::_narrow)));

  return 0;
}

ACE_INLINE CORBA_LocalObject_ptr
CORBA_LocalObject::_unchecked_narrow (CORBA_Object_ptr obj
                                      ACE_ENV_ARG_DECL_NOT_USED)
{
  if (obj->_is_local () != 0)
    return
      ACE_reinterpret_cast (
        CORBA_LocalObject_ptr,
        obj->_tao_QueryInterface (
          ACE_reinterpret_cast (ptr_arith_t,
                                &CORBA_LocalObject::_narrow)));

  return 0;
}

ACE_INLINE
CORBA_LocalObject::CORBA_LocalObject (void)
  : CORBA_Object (0)
{
}

ACE_INLINE
TAO_Local_RefCounted_Object::TAO_Local_RefCounted_Object (void)
  : refcount_lock_ (),
    refcount_ (1)
{
}

// *************************************************************
// Inline operations for class CORBA_LocalObject_var
// *************************************************************

ACE_INLINE
CORBA_LocalObject_var::CORBA_LocalObject_var (void)
  : ptr_ (CORBA_LocalObject::_nil ())
{
}

ACE_INLINE
CORBA_LocalObject_var::CORBA_LocalObject_var (CORBA_LocalObject_ptr p)
  : ptr_ (p)
{}

ACE_INLINE
CORBA_LocalObject_var::~CORBA_LocalObject_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_LocalObject_ptr
CORBA_LocalObject_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_LocalObject_var::CORBA_LocalObject_var (const CORBA_LocalObject_var &p)
  : ptr_ (CORBA_LocalObject::_duplicate (p.ptr ()))
{
}

ACE_INLINE CORBA_LocalObject_var &
CORBA_LocalObject_var::operator= (CORBA_LocalObject_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_LocalObject_var &
CORBA_LocalObject_var::operator= (const CORBA_LocalObject_var &p)
{
  if (this != &p)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = CORBA_LocalObject::_duplicate (p.ptr ());
    }

  return *this;
}

ACE_INLINE
CORBA_LocalObject_var::operator const CORBA_LocalObject_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_LocalObject_var::operator CORBA_LocalObject_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_LocalObject_ptr
CORBA_LocalObject_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_LocalObject_ptr
CORBA_LocalObject_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_LocalObject_ptr &
CORBA_LocalObject_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_LocalObject_ptr &
CORBA_LocalObject_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_LocalObject::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_LocalObject_ptr
CORBA_LocalObject_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA_LocalObject_ptr val = this->ptr_;
  this->ptr_ = CORBA_LocalObject::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_LocalObject_out
// *************************************************************

ACE_INLINE
CORBA_LocalObject_out::CORBA_LocalObject_out (CORBA_LocalObject_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = CORBA_LocalObject::_nil ();
}

ACE_INLINE
CORBA_LocalObject_out::CORBA_LocalObject_out (CORBA_LocalObject_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_LocalObject::_nil ();
}

ACE_INLINE
CORBA_LocalObject_out::CORBA_LocalObject_out (const CORBA_LocalObject_out &p)
  : ptr_ (p.ptr_)
{
}

ACE_INLINE CORBA_LocalObject_out &
CORBA_LocalObject_out::operator= (const CORBA_LocalObject_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_LocalObject_out &
CORBA_LocalObject_out::operator= (const CORBA_LocalObject_var &p)
{
  this->ptr_ = CORBA_LocalObject::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_LocalObject_out &
CORBA_LocalObject_out::operator= (CORBA_LocalObject_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_LocalObject_out::operator CORBA_LocalObject_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_LocalObject_ptr &
CORBA_LocalObject_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA_LocalObject_ptr
CORBA_LocalObject_out::operator-> (void)
{
  return this->ptr_;
}
