// This may not look like C++, but it's really -*- C++ -*-
// $Id$

ACE_INLINE 
CORBA::AbstractBase_ptr
CORBA_AbstractBase::_nil (void)
{
  return 0;
}

ACE_INLINE 
CORBA_AbstractBase_ptr
CORBA_AbstractBase::_duplicate (CORBA_AbstractBase_ptr obj)
{
  if (obj)
    {
      obj->_add_ref ();
    }

  return obj;
}

/// Just call _duplicate and let it decide what to do.
ACE_INLINE 
CORBA::AbstractBase_ptr
CORBA_AbstractBase::_narrow (CORBA::AbstractBase_ptr obj
                             ACE_ENV_ARG_DECL_NOT_USED)
{
  return CORBA_AbstractBase::_duplicate (obj);
}

/// Same for this one.
ACE_INLINE 
CORBA::AbstractBase_ptr
CORBA_AbstractBase::_unchecked_narrow (CORBA::AbstractBase_ptr obj
                                       ACE_ENV_ARG_DECL_NOT_USED)
{
  return CORBA_AbstractBase::_duplicate (obj);
}

ACE_INLINE
CORBA::Boolean
CORBA_AbstractBase::_is_a (const char *type_id
                           ACE_ENV_ARG_DECL_NOT_USED)
{
  return ! ACE_OS::strcmp (type_id,
                           "IDL:omg.org/CORBA/AbstractBase:1.0");
}

ACE_INLINE
const char *
CORBA_AbstractBase::_interface_repository_id (void) const
{
  return "IDL:omg.org/CORBA/AbstractBase:1.0";
}

ACE_INLINE
const char *
CORBA_AbstractBase::_tao_obv_repository_id (void) const
{
  return "IDL:omg.org/CORBA/AbstractBase:1.0";
}

ACE_INLINE
void *
CORBA_AbstractBase::_tao_obv_narrow (ptr_arith_t /* type_id */)
{
  return this;
}

ACE_INLINE
CORBA::Boolean
CORBA_AbstractBase::_is_objref (void) const
{
  return this->is_objref_;
}

ACE_INLINE
TAO_Stub *
CORBA_AbstractBase::_stubobj (void) const
{
  return this->concrete_stubobj_;
}

ACE_INLINE
CORBA::Boolean
CORBA_AbstractBase::_is_collocated (void) const
{
  return this->is_collocated_;
}

ACE_INLINE
TAO_Abstract_ServantBase *
CORBA_AbstractBase::_servant (void) const
{
  return this->servant_;
}

ACE_INLINE
CORBA::Boolean
CORBA_AbstractBase::_is_local (void) const
{
  return this->is_local_;
}

// ************************************************************
// These are in CORBA namespace

ACE_INLINE 
void
CORBA::release (CORBA::AbstractBase_ptr obj)
{
  if (obj)
    {
      obj->_remove_ref ();
    }
}

ACE_INLINE 
CORBA::Boolean
CORBA::is_nil (CORBA::AbstractBase_ptr obj)
{
  return (obj == 0);
}

// *************************************************************
// Inline operations for class CORBA_AbstractBase_var
// *************************************************************

ACE_INLINE
CORBA_AbstractBase_var::CORBA_AbstractBase_var (void)
  : ptr_ (CORBA::AbstractBase::_nil ())
{
}

ACE_INLINE
CORBA_AbstractBase_var::CORBA_AbstractBase_var (CORBA::AbstractBase_ptr p)
  : ptr_ (p)
{
}

ACE_INLINE
CORBA_AbstractBase_var::~CORBA_AbstractBase_var (void)
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA::AbstractBase_ptr
CORBA_AbstractBase_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_AbstractBase_var::CORBA_AbstractBase_var (const CORBA_AbstractBase_var &p)
  : ptr_ (CORBA_AbstractBase::_duplicate (p.ptr ()))
{
}

ACE_INLINE CORBA_AbstractBase_var &
CORBA_AbstractBase_var::operator= (CORBA::AbstractBase_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_AbstractBase_var &
CORBA_AbstractBase_var::operator= (const CORBA_AbstractBase_var &p)
{
  if (this != &p)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = CORBA_AbstractBase::_duplicate (p.ptr ());
    }

  return *this;
}

ACE_INLINE
CORBA_AbstractBase_var::operator const CORBA::AbstractBase_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_AbstractBase_var::operator CORBA::AbstractBase_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::AbstractBase_ptr
CORBA_AbstractBase_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::AbstractBase_ptr
CORBA_AbstractBase_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::AbstractBase_ptr &
CORBA_AbstractBase_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::AbstractBase_ptr &
CORBA_AbstractBase_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_AbstractBase::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA::AbstractBase_ptr
CORBA_AbstractBase_var::_retn (void)
{
  // Yield ownership of valuebase.
  CORBA::AbstractBase_ptr val = this->ptr_;
  this->ptr_ = CORBA_AbstractBase::_nil ();
  return val;
}

ACE_INLINE CORBA::AbstractBase_ptr
CORBA_AbstractBase_var::tao_duplicate (CORBA::AbstractBase_ptr p)
{
  return CORBA_AbstractBase::_duplicate (p);
}

ACE_INLINE void
CORBA_AbstractBase_var::tao_release (CORBA::AbstractBase_ptr p)
{
  CORBA::release (p);
}

ACE_INLINE CORBA::AbstractBase_ptr
CORBA_AbstractBase_var::tao_nil (void)
{
  return CORBA_AbstractBase::_nil ();
}

ACE_INLINE CORBA::AbstractBase_ptr
CORBA_AbstractBase_var::tao_narrow (
    CORBA::AbstractBase *p
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  return CORBA_AbstractBase::_duplicate (p);
}

ACE_INLINE CORBA::AbstractBase *
CORBA_AbstractBase_var::tao_upcast (void *src)
{
  CORBA_AbstractBase **tmp =
    ACE_static_cast (CORBA_AbstractBase **, src);
  return *tmp;
}

// *************************************************************
// Inline operations for class CORBA_AbstractBase_out
// *************************************************************

ACE_INLINE
CORBA_AbstractBase_out::CORBA_AbstractBase_out (CORBA::AbstractBase_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = CORBA_AbstractBase::_nil ();
}

ACE_INLINE
CORBA_AbstractBase_out::CORBA_AbstractBase_out (CORBA_AbstractBase_var &p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_AbstractBase::_nil ();
}

ACE_INLINE
CORBA_AbstractBase_out::CORBA_AbstractBase_out (const CORBA_AbstractBase_out &p)
  : ptr_ (p.ptr_)
{
}

ACE_INLINE CORBA_AbstractBase_out &
CORBA_AbstractBase_out::operator= (const CORBA_AbstractBase_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_AbstractBase_out &
CORBA_AbstractBase_out::operator= (const CORBA_AbstractBase_var &p)
{
  this->ptr_ = CORBA_AbstractBase::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_AbstractBase_out &
CORBA_AbstractBase_out::operator= (CORBA::AbstractBase_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_AbstractBase_out::operator CORBA::AbstractBase_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::AbstractBase_ptr &
CORBA_AbstractBase_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA::AbstractBase_ptr
CORBA_AbstractBase_out::operator-> (void)
{
  return this->ptr_;
}


