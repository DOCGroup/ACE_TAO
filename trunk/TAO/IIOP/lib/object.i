ACE_INLINE void *
CORBA_Object::get_subclass (void)
{
  return sub_;
}

// CORBA dup/release build on top of COM's (why not).

ACE_INLINE void
CORBA_release (CORBA_Object_ptr obj)
{
  if (obj)
    obj->Release ();
}

ACE_INLINE CORBA_Object_ptr
CORBA_Object::_duplicate (CORBA_Object_ptr obj)
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

ACE_INLINE CORBA_Boolean
CORBA_is_nil (CORBA_Object_ptr	obj)
{
  return (CORBA_Boolean) (obj == 0);
}

// Constructor and destructor are accessible to subclasses

ACE_INLINE
CORBA_Object::CORBA_Object (IUnknown *_jan)
  : parent_ (_jan),
    orb_ (0)
{
  //    assert (parent != 0);
  // we removed this as it doesn't fit in our schema of things
}

ACE_INLINE void
CORBA_Object::set_parent (IUnknown *p)
{
  this->parent_ = p;
  assert (this->parent_ != 0);
}

ACE_INLINE void
CORBA_Object::orb (CORBA_ORB_ptr orb)
{
  orb_ = orb;
}

ACE_INLINE CORBA_ORB_ptr
CORBA_Object::orb (void) const
{
  return orb_;
}

ACE_INLINE
CORBA_Object::~CORBA_Object (void)
{
}

// DII hook to objref
//
// The mapping for create_request is split into two forms,
// corresponding to the two usage styles described in CORBA section
// 6.2.1.

ACE_INLINE void __stdcall
CORBA_Object::_create_request (const CORBA_Char *operation,
			       CORBA_NVList_ptr arg_list,
			       CORBA_NamedValue_ptr result,
			       CORBA_Request_ptr &request,
			       CORBA_Flags req_flags,
			       CORBA_Environment &env)
{
  env.clear ();
  request = new CORBA_Request (this, operation, arg_list, result, req_flags);
}

ACE_INLINE CORBA_Request_ptr __stdcall
CORBA_Object::_request (const CORBA_Char *operation,
			CORBA_Environment &env)
{
  env.clear ();
  return new CORBA_Request (this, operation);
}

ACE_INLINE ULONG __stdcall
CORBA_Object::AddRef (void)
{
  return parent_->AddRef ();
}

ACE_INLINE ULONG __stdcall
CORBA_Object::Release (void)
{
  return parent_->Release ();
}

ACE_INLINE HRESULT __stdcall
CORBA_Object::QueryInterface (REFIID riid,
                              void **ppv)
{
  return parent_->QueryInterface (riid, ppv);
}
