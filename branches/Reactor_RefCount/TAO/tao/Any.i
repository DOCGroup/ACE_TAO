// -*- C++ -*-
//
// $Id$

ACE_INLINE TAO::Any_Impl *
CORBA::Any::impl (void) const
{
  return this->impl_;
}

// *************************************************************
// Inline operations for class CORBA::Any_var
// *************************************************************

ACE_INLINE
CORBA::Any_var::Any_var (void)
  : ptr_ (0)
{
}

ACE_INLINE
CORBA::Any_var::Any_var (CORBA::Any *p)
  : ptr_ (p)
{
}

ACE_INLINE
CORBA::Any_var::Any_var (const CORBA::Any_var &r)
{
  CORBA::Any_ptr nptr;
  ACE_NEW (nptr,
           CORBA::Any (*r.ptr_));
  this->ptr_ = nptr;
}

ACE_INLINE
CORBA::Any_var::~Any_var (void)
{
  delete this->ptr_;
}

ACE_INLINE
CORBA::Any_var::operator CORBA::Any *&()
{
  return this->ptr_;
}

ACE_INLINE
CORBA::Any_var::operator const CORBA::Any *() const
{
  return this->ptr_;
}

ACE_INLINE CORBA::Any *
CORBA::Any_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE const CORBA::Any &
CORBA::Any_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE CORBA::Any &
CORBA::Any_var::inout (void)
{
  return *this->ptr_;
}

ACE_INLINE CORBA::Any *&
CORBA::Any_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CORBA::Any *
CORBA::Any_var::_retn (void)
{
  CORBA::Any *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

// *************************************************************
// CORBA::Any_var insertion operators
// *************************************************************

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::Short rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::UShort rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::Long rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::ULong rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::LongLong rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::ULongLong rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::Float rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::Double rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, const CORBA::Any& rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, const char* rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::TypeCode_ptr rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, const CORBA::Object_ptr rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::Any::from_boolean rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::Any::from_char rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::Any::from_wchar rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::Any::from_octet rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::Any::from_string rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA::Any_var &lhs, CORBA::Any::from_wstring rhs)
{
  lhs.inout () <<= rhs;
}

// *************************************************************
// CORBA::Any_var extraction operators
// *************************************************************

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Short &rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::UShort &rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Long &rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::ULong &rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::LongLong &rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::ULongLong &rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Float &rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Double &rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, const CORBA::Any *&rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::TypeCode_ptr &rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, const char *&rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, const CORBA::WChar *&rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Any::to_boolean rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Any::to_octet rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Any::to_char rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Any::to_wchar rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Any::to_string rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Any::to_wstring rhs)
{
  return lhs.in () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (const CORBA::Any_var &lhs, CORBA::Any::to_object rhs)
{
  return lhs.in () >>= rhs;
}

// *************************************************************
// Inline operations for class CORBA::Any_out
// *************************************************************

ACE_INLINE
CORBA::Any_out::Any_out (CORBA::Any *&s)
  : ptr_ (s)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA::Any_out::Any_out (CORBA::Any_var &s)
  : ptr_ (s.out ())
{
}

ACE_INLINE
CORBA::Any_out::Any_out (const CORBA::Any_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA::Any_out &
CORBA::Any_out::operator= (const CORBA::Any_out &s)
{
  this->ptr_ = s.ptr_;
  return *this;
}

ACE_INLINE CORBA::Any_out &
CORBA::Any_out::operator= (CORBA::Any *s)
{
  this->ptr_ = s;
  return *this;
}

ACE_INLINE
CORBA::Any_out::operator CORBA::Any *&()
{
  return this->ptr_;
}

ACE_INLINE CORBA::Any *&
CORBA::Any_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::Any *
CORBA::Any_out::operator-> (void)
{
  return this->ptr_;
}

// ***********************************************************************

ACE_INLINE void
TAO::Any_Impl::free_value (void)
{
  // Does nothing so that subclasses for basic types don't have to
  // override it.
}

ACE_INLINE CORBA::TypeCode_ptr
TAO::Any_Impl::type (void) const
{
  return CORBA::TypeCode::_duplicate (this->type_);
}

ACE_INLINE CORBA::TypeCode_ptr
TAO::Any_Impl::_tao_get_typecode (void) const
{
  return this->type_;
}

ACE_INLINE void
TAO::Any_Impl::type (CORBA::TypeCode_ptr tc)
{
  this->type_ = tc;
}

ACE_INLINE ACE_Message_Block *
TAO::Any_Impl::_tao_get_cdr (void) const
{
  return 0;
}

ACE_INLINE int
TAO::Any_Impl::_tao_byte_order (void) const
{
  return TAO_ENCAP_BYTE_ORDER;
}

ACE_INLINE CORBA::Boolean
TAO::Any_Impl::to_object (CORBA::Object_ptr &) const
{
  return 0;
}

ACE_INLINE CORBA::Boolean
TAO::Any_Impl::to_value (CORBA::ValueBase *&) const
{
  return 0;
}

ACE_INLINE CORBA::Boolean
TAO::Any_Impl::to_abstract_base (CORBA::AbstractBase_ptr &) const
{
  return 0;
}

// ***********************************************************************

ACE_INLINE const void *
TAO::Unknown_IDL_Type::value (void) const
{
  return this->cdr_;
}

ACE_INLINE ACE_Message_Block *
TAO::Unknown_IDL_Type::_tao_get_cdr (void) const
{
  return this->cdr_;
}

ACE_INLINE int
TAO::Unknown_IDL_Type::_tao_byte_order (void) const
{
  return this->byte_order_;
}

