// $Id$

// = methods for class CORBA_Object

ACE_INLINE ULONG
CORBA_Object::AddRef (void)
{
  return ++this->refcount_;
}

ACE_INLINE ULONG
CORBA_Object::Release (void)
{
  if (--this->refcount_ != 0)
    return this->refcount_;
  delete this;
  return 0;
}

ACE_INLINE TAO_HRESULT
CORBA_Object::QueryInterface (REFIID riid,
                              void **ppv)
{
  *ppv = 0;

  if (IID_IIOP_Object == riid
      || IID_STUB_Object == riid
      || IID_TAO_IUnknown == riid)
    *ppv = this->parent_;
  else if (IID_CORBA_Object == riid)
    *ppv = this;

  if (*ppv == 0)
    return TAO_ResultFromScode (TAO_E_NOINTERFACE);

 (void) this->AddRef ();
  return TAO_NOERROR;
}

ACE_INLINE CORBA_Object_ptr
CORBA_Object::_duplicate (CORBA::Object_ptr obj)
{
  if (obj)
    obj->AddRef ();
  return obj;
}

// Null pointers represent nil objects.

ACE_INLINE CORBA_Object_ptr
CORBA_Object::_nil (void)
{
  return 0;
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::Object_ptr obj)
{
  return (CORBA::Boolean) (obj == 0);
}

ACE_INLINE void
CORBA_Object::_set_parent (STUB_Object *p)
{
  if (p != 0 && p->AddRef () != 0)
    {
      // Release the one we had.
      if (this->parent_)
        this->parent_->Release ();

      // Hold on to the new one
      this->parent_ = p;
    }

  ACE_ASSERT (this->parent_ != 0);
}

ACE_INLINE STUB_Object *
CORBA_Object::_get_parent (void) const
{
  return this->parent_;
}

// DII hook to objref
//
// The mapping for create_request is split into two forms,
// corresponding to the two usage styles described in CORBA section
// 6.2.1.

ACE_INLINE void
CORBA_Object::_create_request (const CORBA::Char *operation,
                               CORBA::NVList_ptr arg_list,
                               CORBA::NamedValue_ptr result,
                               CORBA::Request_ptr &request,
                               CORBA::Flags req_flags,
                               CORBA::Environment &env)
{
  env.clear ();
  request = new CORBA::Request (this, operation, arg_list, result, req_flags);
}

ACE_INLINE CORBA::Request_ptr
CORBA_Object::_request (const CORBA::Char *operation,
                        CORBA::Environment &env)
{
  env.clear ();
  return new CORBA::Request (this, operation);
}


// *************************************************************
// Inline operations for class CORBA_Object_var
// *************************************************************

ACE_INLINE
CORBA_Object_var::CORBA_Object_var (CORBA_Object_ptr p)
        : ptr_ (p)
{}

ACE_INLINE CORBA_Object_ptr
CORBA_Object_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Object_var::CORBA_Object_var (const CORBA_Object_var &p) // copy constructor
        : ptr_ (CORBA_Object::_duplicate (p.ptr ()))
{}

ACE_INLINE CORBA_Object_var &
CORBA_Object_var::operator= (CORBA_Object_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_Object_var &
CORBA_Object_var::operator= (const CORBA_Object_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA_Object::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_Object_var::operator const CORBA_Object_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Object_var::operator CORBA_Object_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_Object_ptr
CORBA_Object_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Object_ptr
CORBA_Object_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Object_ptr &
CORBA_Object_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Object_ptr &
CORBA_Object_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_Object::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_Object_ptr
CORBA_Object_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA_Object_ptr val = this->ptr_;
  this->ptr_ = CORBA_Object::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_Object_out
// *************************************************************

ACE_INLINE
CORBA_Object_out::CORBA_Object_out (CORBA_Object_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA_Object::_nil ();
}

ACE_INLINE
CORBA_Object_out::CORBA_Object_out (CORBA_Object_var &p) // constructor from _var
        : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_Object::_nil ();
}

ACE_INLINE
CORBA_Object_out::CORBA_Object_out (CORBA_Object_out &p) // copy constructor
        : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_Object_out &
CORBA_Object_out::operator= (CORBA_Object_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_Object_out &
CORBA_Object_out::operator= (const CORBA_Object_var &p)
{
  this->ptr_ = CORBA_Object::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_Object_out &
CORBA_Object_out::operator= (CORBA_Object_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_Object_out::operator CORBA_Object_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_Object_ptr &
CORBA_Object_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA_Object_ptr
CORBA_Object_out::operator-> (void)
{
  return this->ptr_;
}
