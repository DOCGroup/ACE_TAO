// This may not look like C++, but it's really -*- C++ -*-
//
// $Id$

// static operations in namespace CORBA ========================

ACE_INLINE void
CORBA::add_ref (CORBA::ValueBase *val)
{
  if (val)
    {
      val->_add_ref ();
    }
}

ACE_INLINE void
CORBA::remove_ref (CORBA::ValueBase *val)
{
  if (val)
    {
      val->_remove_ref ();
    }
}

// ===========================================================

// constructor
ACE_INLINE
CORBA::ValueBase::ValueBase (void)
{
}

// *************************************************************
// Inline operations for class CORBA::ValueBase_var
// *************************************************************

ACE_INLINE
CORBA::ValueBase_var::ValueBase_var (void)
  : ptr_ (0)
{
}

ACE_INLINE
CORBA::ValueBase_var::ValueBase_var (CORBA::ValueBase *p)
  : ptr_ (p)
{
}

ACE_INLINE
CORBA::ValueBase_var::~ValueBase_var (void)
{
  CORBA::remove_ref (this->ptr_);
}

ACE_INLINE CORBA::ValueBase *
CORBA::ValueBase_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA::ValueBase_var::ValueBase_var (const CORBA::ValueBase_var &p)
  : ptr_ (p.ptr_)
{
  p.ptr_->_add_ref ();
}

ACE_INLINE CORBA::ValueBase_var &
CORBA::ValueBase_var::operator= (CORBA::ValueBase *p)
{
  CORBA::remove_ref (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA::ValueBase_var &
CORBA::ValueBase_var::operator= (const CORBA::ValueBase_var &p)
{
  if (this != &p)
    {
      CORBA::remove_ref (this->ptr_);
      p.ptr_->_add_ref ();
      this->ptr_ = p.ptr_;
    }

  return *this;
}

ACE_INLINE
CORBA::ValueBase_var::operator const CORBA::ValueBase *() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA::ValueBase_var::operator CORBA::ValueBase *&() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::ValueBase *
CORBA::ValueBase_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ValueBase *
CORBA::ValueBase_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ValueBase *&
CORBA::ValueBase_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::ValueBase *&
CORBA::ValueBase_var::out (void)
{
  CORBA::remove_ref (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CORBA::ValueBase *
CORBA::ValueBase_var::_retn (void)
{
  // Yield ownership of valuebase.
  CORBA::ValueBase *val = this->ptr_;
  this->ptr_ = 0;
  return val;
}

ACE_INLINE CORBA::ValueBase *
CORBA::ValueBase_var::tao_duplicate (CORBA::ValueBase *p)
{
  p->_add_ref ();
  return p;
}

ACE_INLINE void
CORBA::ValueBase_var::tao_release (CORBA::ValueBase *p)
{
  p->_remove_ref ();
}

ACE_INLINE CORBA::ValueBase *
CORBA::ValueBase_var::tao_nil (void)
{
  return 0;
}

ACE_INLINE CORBA::ValueBase *
CORBA::ValueBase_var::tao_narrow (
    CORBA::ValueBase *p
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  p->_add_ref ();
  return CORBA::ValueBase::_downcast (p);
}

ACE_INLINE CORBA::ValueBase *
CORBA::ValueBase_var::tao_upcast (void *src)
{
  CORBA::ValueBase **tmp =
    ACE_static_cast (CORBA::ValueBase **, src);
  return *tmp;
}

// *************************************************************
// Inline operations for class CORBA::ValueBase_out
// *************************************************************

ACE_INLINE
CORBA::ValueBase_out::ValueBase_out (CORBA::ValueBase *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA::ValueBase_out::ValueBase_out (CORBA::ValueBase_var &p)
  : ptr_ (p.out ())
{
  this->ptr_->_remove_ref ();
  this->ptr_ = 0;
}

ACE_INLINE
CORBA::ValueBase_out::ValueBase_out (const CORBA::ValueBase_out &p)
  : ptr_ (p.ptr_)
{
}

ACE_INLINE CORBA::ValueBase_out &
CORBA::ValueBase_out::operator= (const CORBA::ValueBase_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA::ValueBase_out &
CORBA::ValueBase_out::operator= (const CORBA::ValueBase_var &p)
{
  p.ptr ()->_add_ref ();
  this->ptr_ = p.ptr ();
  return *this;
}

ACE_INLINE CORBA::ValueBase_out &
CORBA::ValueBase_out::operator= (CORBA::ValueBase *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA::ValueBase_out::operator CORBA::ValueBase *&() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::ValueBase *&
CORBA::ValueBase_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA::ValueBase *
CORBA::ValueBase_out::operator-> (void)
{
  return this->ptr_;
}

// ===========================================================

// constructor
ACE_INLINE
CORBA::DefaultValueRefCountBase::DefaultValueRefCountBase (void)
  : _tao_reference_count_ (1)
{
}


ACE_INLINE void
CORBA::DefaultValueRefCountBase::_tao_add_ref (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->_tao_reference_count_lock_);
  ++_tao_reference_count_;
}

ACE_INLINE void
CORBA::DefaultValueRefCountBase::_tao_remove_ref (void)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->_tao_reference_count_lock_);
    -- this->_tao_reference_count_;
    if (this->_tao_reference_count_ != 0)
      return;
  }
  // The guard has been already given up, but this is no problem
  // cause we have held the last reference, and this one we don't give
  // away anymore, we do:
  delete this;
}

ACE_INLINE CORBA::ULong
CORBA::DefaultValueRefCountBase::_tao_refcount_value (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->_tao_reference_count_lock_,
                    0);
  return _tao_reference_count_;
}

// ===========================================================

// Detection of flags in the CDR Stream

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_null_ref (CORBA::ULong tag)
{
  return (tag == 0);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_value_tag (CORBA::ULong tag)
{
  return ((tag & Value_tag_sigbits) == 0x7FFFFF00L);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: has_codebase_url (CORBA::ULong tag)
{
  return (CORBA::Boolean) (tag & Codebase_url);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_no_type_info (CORBA::ULong tag)
{
  return ((tag & Type_info_sigbits) == Type_info_none);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_single_type_info (CORBA::ULong tag)
{
  return ((tag & Type_info_sigbits) == Type_info_single);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_list_type_info (CORBA::ULong tag)
{
  return ((tag & Type_info_sigbits) == Type_info_list);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: is_chunked (CORBA::ULong tag)
{
  return (CORBA::Boolean) (tag & 8);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_indirection_tag (CORBA::ULong tag)
{
  return (tag == 0xFFFFFFFFL);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_indirection (CORBA::ULong value)
{
  return (0x80000000L < value && value <= (0xFFFFFFFFL - 4));
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_block_size (CORBA::ULong value)
{
  return ( 0 < value && value < 0x7fffff00L);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_end_tag (CORBA::ULong tag)
{
  return (0x80000000L < tag  && tag <= 0xFFFFFFFFL);
}

