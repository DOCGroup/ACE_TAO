// -*- C++ -*-
//
// $Id$


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

#if (TAO_HAS_INTERCEPTORS == 1)
ACE_INLINE PortableInterceptor::ClientRequestInterceptor_ptr
CORBA_ORB::_register_client_interceptor
  (PortableInterceptor::ClientRequestInterceptor_ptr ci,
   CORBA_Environment &ACE_TRY_ENV)
{
  if (ci != 0)
      {
        PortableInterceptor::ClientRequestInterceptor_var oci =
          PortableInterceptor::ClientRequestInterceptor::_duplicate (this->client_interceptor_.in ());
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
  if (si != 0)
      {
        PortableInterceptor::ServerRequestInterceptor_var oci =
          PortableInterceptor::ServerRequestInterceptor::_duplicate (this->server_interceptor_.in ());
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
    PortableInterceptor::ClientRequestInterceptor::_duplicate (this->client_interceptor_.in ());
}

ACE_INLINE PortableInterceptor::ServerRequestInterceptor_ptr
CORBA_ORB::_get_server_interceptor (CORBA_Environment &)
{
  return
    PortableInterceptor::ServerRequestInterceptor::_duplicate (this->server_interceptor_.in ());
}
#endif /* TAO_HAS_INTERCEPTORS */

// ************************************************************
// These are in CORBA namespace
// ************************************************************

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::ORB_ptr obj)
{
  return obj == CORBA_ORB::_nil ();
}

ACE_INLINE void
CORBA::release (CORBA::ORB_ptr obj)
{
  if (!CORBA::is_nil (obj))
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
