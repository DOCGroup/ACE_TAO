// This may look like C, but it's really -*- C++ -*-
// $Id$

// Insertion from special types.

ACE_INLINE void
CORBA_Any::replace (CORBA::TypeCode_ptr type,
                    const void *value,
                    CORBA::Environment &TAO_IN_ENV)
{
  // invoke the first form of the replace method and pass the default value
  // (FALSE) for the "any_owns_data" parameter
  this->replace (type, value, 0, TAO_IN_ENV);
}

ACE_INLINE CORBA::Boolean
CORBA_Any::any_owns_data (void) const
{
  return (this->any_owns_data_ != 0 && this->value_ != 0);
}

ACE_INLINE ACE_Message_Block*
CORBA_Any::_tao_get_cdr (void) const
{
  return this->cdr_;
}

// insertion operators

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::Short s)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_short, new CORBA::Short (s), 1, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::UShort s)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_ushort, new CORBA::UShort (s), 1, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::Long l)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_long, new CORBA::Long (l), 1, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::ULong l)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_ulong, new CORBA::ULong (l), 1, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::LongLong l)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_longlong, new CORBA::LongLong (l), 1, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::ULongLong l)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_ulonglong, new CORBA::ULongLong (l), 1, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::Float f)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_float, new CORBA::Float (f), 1, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::Double d)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_double, new CORBA::Double (d), 1, env);
}

// insertion of Any - copying
ACE_INLINE void
CORBA_Any::operator<<= (const CORBA_Any& a)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_any, new CORBA_Any (a), 1, env);
}

// insertion of Any - non-copying
ACE_INLINE void
CORBA::Any::operator<<= (CORBA::Any_ptr anyptr)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_any, anyptr, 1, env);
}

// implementing the special types
ACE_INLINE
CORBA_Any::to_object::to_object (CORBA_Object_out obj)
  : ref_ (obj.ptr ())
{
}

ACE_INLINE void
CORBA_Any::operator<<= (from_boolean b)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_boolean, new CORBA::Boolean (b.val_),
                 1, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (from_octet o)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_octet, new CORBA::Octet (o.val_),
                 1, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (from_char c)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_char, new CORBA::Char (c.val_),
                 1, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (from_wchar wc)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_wchar, new CORBA::WChar (wc.val_),
                 1, env);
}

// *************************************************************
// Inline operations for class CORBA_Any_var
// *************************************************************

ACE_INLINE
CORBA_Any_var::CORBA_Any_var (void)
  : ptr_ (0)
{
}

ACE_INLINE
CORBA_Any_var::CORBA_Any_var (CORBA_Any *p)
  : ptr_ (p)
{
}

ACE_INLINE
CORBA_Any_var::CORBA_Any_var (const CORBA_Any_var& r)
  : ptr_ (new CORBA::Any (*r.ptr_))
{
}

ACE_INLINE
CORBA_Any_var::~CORBA_Any_var (void)
{
  delete this->ptr_;
}

ACE_INLINE
CORBA_Any_var::operator CORBA_Any *&()
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Any_var::operator const CORBA_Any *() const
{
  return this->ptr_;
}

ACE_INLINE CORBA::Any *
CORBA_Any_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE const CORBA_Any &
CORBA_Any_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE CORBA_Any &
CORBA_Any_var::inout (void)
{
  return *this->ptr_;
}

ACE_INLINE CORBA_Any *&
CORBA_Any_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CORBA_Any *
CORBA_Any_var::_retn (void)
{
  CORBA_Any *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

// *************************************************************
// CORBA_Any_var insertion operators
// *************************************************************

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::Short rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::UShort rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::Long rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::ULong rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::LongLong rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::ULongLong rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::Float rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::Double rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, const CORBA_Any& rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, const char* rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::TypeCode_ptr rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, const CORBA::Object_ptr rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::Any::from_boolean rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::Any::from_char rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::Any::from_wchar rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::Any::from_octet rhs)
{
  lhs.inout () <<= rhs;
}

ACE_INLINE void
operator <<= (CORBA_Any_var lhs, CORBA::Any::from_string rhs)
{
  lhs.inout () <<= rhs;
}

// *************************************************************
// CORBA_Any_var extraction operators
// *************************************************************

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::Short &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::UShort &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::Long &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::ULong &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::LongLong &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::ULongLong &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::Float &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::Double &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA_Any &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::TypeCode_ptr &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, char* &rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::Any::to_boolean rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::Any::to_octet rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::Any::to_char rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::Any::to_wchar rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::Any::to_string rhs)
{
  return lhs.inout () >>= rhs;
}

ACE_INLINE CORBA::Boolean
operator >>= (CORBA_Any_var lhs, CORBA::Any::to_object rhs)
{
  return lhs.inout () >>= rhs;
}

// *************************************************************
// Inline operations for class CORBA_Any_out
// *************************************************************

ACE_INLINE
CORBA_Any_out::CORBA_Any_out (CORBA_Any *&s)
  : ptr_ (s)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA_Any_out::CORBA_Any_out (CORBA_Any_var &s)
  : ptr_ (s.out ())
{
}

ACE_INLINE
CORBA_Any_out::CORBA_Any_out (const CORBA_Any_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA_Any_out &
CORBA_Any_out::operator= (const CORBA_Any_out &s)
{
  this->ptr_ = s.ptr_;
  return *this;
}

ACE_INLINE CORBA_Any_out &
CORBA_Any_out::operator= (CORBA_Any *s)
{
  this->ptr_ = s;
  return *this;
}

ACE_INLINE CORBA_Any_out &
CORBA_Any_out::operator= (const CORBA_Any *s)
{
  this->ptr_ = new CORBA::Any (*s);
  return *this;
}

ACE_INLINE
CORBA_Any_out::operator CORBA_Any *&()
{
  return this->ptr_;
}

ACE_INLINE CORBA_Any *&
CORBA_Any_out::ptr (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Any *
CORBA_Any_out::operator-> (void)
{
  return this->ptr_;
}
