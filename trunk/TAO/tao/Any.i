// $Id$
// This may look like C, but it's really -*- C++ -*-

// Insertion from special types.

ACE_INLINE void
CORBA_Any::replace (CORBA::TypeCode_ptr type,
                    const void *value,
                    CORBA::Environment &env)
{
  // invoke the first form of the replace method and pass the default value
  // (FALSE) for the "any_owns_data" parameter
  this->replace (type, value, CORBA::B_FALSE, env);
}

// insertion operators

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::Short s)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_short, new CORBA::Short (s), CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::UShort s)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_ushort, new CORBA::UShort (s), CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::Long l)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_long, new CORBA::Long (l), CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::ULong l)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_ulong, new CORBA::ULong (l), CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::LongLong l)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_longlong, new CORBA::LongLong (l), CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::ULongLong l)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_ulonglong, new CORBA::ULongLong (l), CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::Float f)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_float, new CORBA::Float (f), CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (CORBA::Double d)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_double, new CORBA::Double (d), CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (const CORBA_Any& a)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_any, new CORBA_Any (a), CORBA::B_TRUE, env);
}

// this is a copying version for unbounded strings
ACE_INLINE void
CORBA_Any::operator<<= (const char* s)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_string, new char* (CORBA::string_dup (s)),
                 CORBA::B_TRUE, env);
}

// implementing the special types
ACE_INLINE
CORBA_Any::from_boolean::from_boolean (CORBA::Boolean b)
  : val_ (b)
{
}

ACE_INLINE
CORBA_Any::to_boolean::to_boolean (CORBA::Boolean &b)
  : ref_ (b)
{
}

ACE_INLINE
CORBA_Any::from_octet::from_octet (CORBA::Octet o)
  : val_ (o)
{
}

ACE_INLINE
CORBA_Any::to_octet::to_octet (CORBA::Octet &o)
  : ref_ (o)
{
}

ACE_INLINE
CORBA_Any::from_char::from_char (CORBA::Char c)
  : val_ (c)
{
}

ACE_INLINE
CORBA_Any::to_char::to_char (CORBA::Char &c)
  : ref_ (c)
{
}

ACE_INLINE
CORBA_Any::from_string::from_string (char *s,
                                     CORBA::ULong b,
                                     CORBA::Boolean nocopy)
  : val_ (s),
    bound_ (b),
    nocopy_ (nocopy)
{
}

ACE_INLINE
CORBA_Any::to_string::to_string (char *&s,
                                 CORBA::ULong b)
  : val_ (s),
    bound_ (b)
{
}

ACE_INLINE
CORBA_Any::to_object::to_object (CORBA::Object_ptr &obj)
  : ref_ (obj)
{
}

ACE_INLINE void
CORBA_Any::operator<<= (from_boolean b)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_boolean, new CORBA::Boolean (b.val_),
                 CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (from_octet o)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_octet, new CORBA::Octet (o.val_),
                 CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (from_char c)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_char, new CORBA::Char (c.val_),
                 CORBA::B_TRUE, env);
}

ACE_INLINE void
CORBA_Any::operator<<= (from_wchar wc)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_wchar, new CORBA::WChar (wc.val_),
                 CORBA::B_TRUE, env);
}

// ----------------------------------------------------------------------
// CORBA_Any_var type
// ----------------------------------------------------------------------

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

// ----------------------------------------------------------------------
// CORBA_Any_out type
// ----------------------------------------------------------------------

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
CORBA_Any_out::CORBA_Any_out (CORBA_Any_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA_Any_out &
CORBA_Any_out::operator= (CORBA_Any_out &s)
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

ACE_INLINE CORBA::Boolean
CORBA_Any::any_owns_data (void) const
{
  return this->any_owns_data_;
}
