// -*- c++ -*-
// $Id$

// ----------------------------------------------------------------------
// String_var type
// ----------------------------------------------------------------------

ACE_INLINE
CORBA_String_var::CORBA_String_var (void)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA_String_var::CORBA_String_var (const char *p)
  : ptr_ (CORBA::string_dup (p))
{
}

ACE_INLINE CORBA::Char &
CORBA_String_var::operator[] (CORBA::ULong slot)
{
  // We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE CORBA::Char
CORBA_String_var::operator[] (CORBA::ULong slot) const
{
  // We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE
CORBA_String_var::operator char *()
{
  return this->ptr_;
}

ACE_INLINE
CORBA_String_var::operator const char *() const
{
  return this->ptr_;
}

ACE_INLINE const char *
CORBA_String_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA_String_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA_String_var::out (void)
{
  CORBA::string_free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE char *
CORBA_String_var::_retn (void)
{
  char *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

// ----------------------------------------------------
//  String_out type
// ----------------------------------------------------

ACE_INLINE
CORBA_String_out::CORBA_String_out (char *&s)
  : ptr_ (s)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA_String_out::CORBA_String_out (CORBA_String_var &s)
  : ptr_ (s.out ())
{
}

ACE_INLINE
CORBA_String_out::CORBA_String_out (const CORBA_String_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA_String_out &
CORBA_String_out::operator= (const CORBA_String_out &s)
{
  this->ptr_ = s.ptr_;
  return *this;
}

ACE_INLINE CORBA_String_out &
CORBA_String_out::operator= (char *s)
{
  this->ptr_ = s;
  return *this;
}

ACE_INLINE CORBA_String_out &
CORBA_String_out::operator= (const char *s)
{
  this->ptr_ = CORBA::string_dup (s);
  return *this;
}

ACE_INLINE
CORBA_String_out::operator char *&()
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA_String_out::ptr (void)
{
  return this->ptr_;
}

// ****************************************************************

// ----------------------------------------------------------------------
// String_var type
// ----------------------------------------------------------------------

ACE_INLINE
CORBA_WString_var::CORBA_WString_var (void)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA_WString_var::CORBA_WString_var (const CORBA::WChar *p)
  : ptr_ (CORBA::wstring_dup (p))
{
}

ACE_INLINE CORBA::WChar &
CORBA_WString_var::operator[] (CORBA::ULong slot)
{
  // We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE CORBA::WChar
CORBA_WString_var::operator[] (CORBA::ULong slot) const
{
  // We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE
CORBA_WString_var::operator CORBA::WChar *()
{
  return this->ptr_;
}

ACE_INLINE
CORBA_WString_var::operator const CORBA::WChar *() const
{
  return this->ptr_;
}

ACE_INLINE const CORBA::WChar *
CORBA_WString_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
CORBA_WString_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
CORBA_WString_var::out (void)
{
  CORBA::wstring_free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *
CORBA_WString_var::_retn (void)
{
  CORBA::WChar *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

// ----------------------------------------------------
//  String_out type
// ----------------------------------------------------

ACE_INLINE
CORBA_WString_out::CORBA_WString_out (CORBA::WChar *&s)
  : ptr_ (s)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA_WString_out::CORBA_WString_out (CORBA_WString_var &s)
  : ptr_ (s.out ())
{
}

ACE_INLINE
CORBA_WString_out::CORBA_WString_out (const CORBA_WString_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA_WString_out &
CORBA_WString_out::operator= (const CORBA_WString_out &s)
{
  this->ptr_ = s.ptr_;
  return *this;
}

ACE_INLINE CORBA_WString_out &
CORBA_WString_out::operator= (CORBA::WChar *s)
{
  this->ptr_ = s;
  return *this;
}

ACE_INLINE
CORBA_WString_out::operator CORBA::WChar *&()
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
CORBA_WString_out::ptr (void)
{
  return this->ptr_;
}

// ****************************************************************

// ---------------------------------------------------------------------------
//  ORB specific
// ---------------------------------------------------------------------------

ACE_INLINE CORBA::ULong
CORBA_ORB::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, 0);
  return ++this->refcount_;
}

ACE_INLINE CORBA::ULong
CORBA_ORB::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB::_duplicate (CORBA::ORB_ptr obj)
{
  if (obj)
    obj->_incr_refcnt ();
  return obj;
}

// Null pointers represent nil objects.

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB::_nil (void)
{
  return 0;
}

ACE_INLINE void
CORBA_ORB::_use_omg_ior_format (CORBA::Boolean ior)
{
  this->use_omg_ior_format_ = ior;
}

ACE_INLINE CORBA::Boolean
CORBA_ORB::_use_omg_ior_format (void)
{
  return this->use_omg_ior_format_;
}

ACE_INLINE TAO_ORB_Core *
CORBA_ORB::orb_core (void) const
{
  return this->orb_core_;
}

#if defined (TAO_HAS_INTERCEPTORS)
ACE_INLINE PortableInterceptor::ClientRequestInterceptor_ptr
CORBA_ORB::_register_client_interceptor
  (PortableInterceptor::ClientRequestInterceptor_ptr ci,
   CORBA_Environment &ACE_TRY_ENV)
{
  if (ci == 0 ||
      ci->_is_a ("IDL:TAO/PortableInterceptor/ClientRequestInterceptor:1.0"))
      {
        PortableInterceptor::ClientRequestInterceptor_var oci =
          PortableInterceptor::ClientRequestInterceptor::_duplicate (this->client_interceptor_);
        this->client_interceptor_ = ci;
        return oci._retn ();
      }
  else
    ACE_THROW_RETURN (CORBA::INV_OBJREF (), 0);
}

ACE_INLINE PortableInterceptor::ServerRequestInterceptor_ptr
CORBA_ORB::_register_server_interceptor
  (PortableInterceptor::ServerRequestInterceptor_ptr si,
   CORBA_Environment &ACE_TRY_ENV)
{
  if (si == 0 ||
      si->_is_a ("IDL:TAO/PortableInterceptor/ServerRequestInterceptor:1.0"))
      {
        PortableInterceptor::ServerRequestInterceptor_var oci =
          PortableInterceptor::ServerRequestInterceptor::_duplicate (this->server_interceptor_);
        this->server_interceptor_ = si;
        return oci._retn ();
      }
  else
    ACE_THROW_RETURN (CORBA::INV_OBJREF (), 0);
}

ACE_INLINE PortableInterceptor::ClientRequestInterceptor_ptr
CORBA_ORB::_get_client_interceptor (CORBA_Environment &)
{
  return
    PortableInterceptor::ClientRequestInterceptor::_duplicate (this->client_interceptor_);
}

ACE_INLINE PortableInterceptor::ServerRequestInterceptor_ptr
CORBA_ORB::_get_server_interceptor (CORBA_Environment &)
{
  return
    PortableInterceptor::ServerRequestInterceptor::_duplicate (this->server_interceptor_);
}
#endif /* TAO_HAS_INTERCEPTORS */

// ************************************************************
// These are in CORBA namespace
// ************************************************************

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::ORB_ptr obj)
{
  return obj == 0;
}

ACE_INLINE void
CORBA::release (CORBA::ORB_ptr obj)
{
  if (obj)
    obj->_decr_refcnt ();
}

// *************************************************************
// Inline operations for class CORBA_ORB_var
// *************************************************************

ACE_INLINE
CORBA_ORB_var::CORBA_ORB_var (void) // default constructor
  : ptr_ (CORBA_ORB::_nil ())
{
}

ACE_INLINE
CORBA_ORB_var::CORBA_ORB_var (CORBA::ORB_ptr p)
        : ptr_ (p)
{
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ORB_var::CORBA_ORB_var (const CORBA_ORB_var &p) // copy constructor
  : ptr_ (CORBA_ORB::_duplicate (p.ptr ()))
{
}

ACE_INLINE
CORBA_ORB_var::~CORBA_ORB_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_ORB_var &
CORBA_ORB_var::operator= (CORBA::ORB_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_ORB_var &
CORBA_ORB_var::operator= (const CORBA_ORB_var &p)
{
  if (this != &p)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = CORBA_ORB::_duplicate (p.ptr ());
    }
  return *this;
}

ACE_INLINE
CORBA_ORB_var::operator const CORBA::ORB_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ORB_var::operator CORBA::ORB_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr &
CORBA_ORB_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr &
CORBA_ORB_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_ORB::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA::ORB_ptr val = this->ptr_;
  this->ptr_ = CORBA_ORB::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_ORB_out
// *************************************************************

ACE_INLINE
CORBA_ORB_out::CORBA_ORB_out (CORBA::ORB_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA_ORB::_nil ();
}

ACE_INLINE
CORBA_ORB_out::CORBA_ORB_out (CORBA_ORB_var &p) // constructor from _var
        : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_ORB::_nil ();
}

ACE_INLINE
CORBA_ORB_out::CORBA_ORB_out (const CORBA_ORB_out &p) // copy constructor
        : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_ORB_out &
CORBA_ORB_out::operator= (CORBA_ORB_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_ORB_out &
CORBA_ORB_out::operator= (const CORBA_ORB_var &p)
{
  this->ptr_ = CORBA_ORB::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_ORB_out &
CORBA_ORB_out::operator= (CORBA::ORB_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_ORB_out::operator CORBA::ORB_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr &
CORBA_ORB_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_out::operator-> (void)
{
  return this->ptr_;
}
