// $Id$

// *************************************************************
// Inline operations for class TAO_Priority_Mapping_Manager_var
// *************************************************************

ACE_INLINE
TAO_Priority_Mapping_Manager_var::TAO_Priority_Mapping_Manager_var (void) // default constructor
  : ptr_ (TAO_Priority_Mapping_Manager::_nil ())
{}

ACE_INLINE
TAO_Priority_Mapping_Manager_var::TAO_Priority_Mapping_Manager_var (TAO_Priority_Mapping_Manager_ptr p)
  : ptr_ (p)
{}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
TAO_Priority_Mapping_Manager_var::TAO_Priority_Mapping_Manager_var (const ::TAO_Priority_Mapping_Manager_var &p) // copy constructor
  : TAO_Base_var (),
    ptr_ (TAO_Priority_Mapping_Manager::_duplicate (p.ptr ()))
{}

ACE_INLINE
TAO_Priority_Mapping_Manager_var::~TAO_Priority_Mapping_Manager_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE TAO_Priority_Mapping_Manager_var &
TAO_Priority_Mapping_Manager_var::operator= (TAO_Priority_Mapping_Manager_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE TAO_Priority_Mapping_Manager_var &
TAO_Priority_Mapping_Manager_var::operator= (const ::TAO_Priority_Mapping_Manager_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::TAO_Priority_Mapping_Manager::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
TAO_Priority_Mapping_Manager_var::operator const ::TAO_Priority_Mapping_Manager_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
TAO_Priority_Mapping_Manager_var::operator ::TAO_Priority_Mapping_Manager_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr &
TAO_Priority_Mapping_Manager_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr &
TAO_Priority_Mapping_Manager_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::TAO_Priority_Mapping_Manager::_nil ();
  return this->ptr_;
}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager_var::_retn (void)
{
  // yield ownership of managed obj reference
  ::TAO_Priority_Mapping_Manager_ptr val = this->ptr_;
  this->ptr_ = ::TAO_Priority_Mapping_Manager::_nil ();
  return val;
}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager_var::duplicate (TAO_Priority_Mapping_Manager_ptr p)
{
  return ::TAO_Priority_Mapping_Manager::_duplicate (p);
}

ACE_INLINE void
TAO_Priority_Mapping_Manager_var::release (TAO_Priority_Mapping_Manager_ptr p)
{
  CORBA::release (p);
}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager_var::nil (void)
{
  return ::TAO_Priority_Mapping_Manager::_nil ();
}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager_var::narrow (
    CORBA::Object *p
    TAO_ENV_ARG_DECL
  )
{
  return ::TAO_Priority_Mapping_Manager::_narrow (p TAO_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::Object *
TAO_Priority_Mapping_Manager_var::upcast (void *src)
{
  TAO_Priority_Mapping_Manager **tmp =
    ACE_static_cast (TAO_Priority_Mapping_Manager **, src);
  return *tmp;
}


// *************************************************************
// Inline operations for class TAO_Priority_Mapping_Manager_out
// *************************************************************

ACE_INLINE
TAO_Priority_Mapping_Manager_out::TAO_Priority_Mapping_Manager_out (TAO_Priority_Mapping_Manager_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = ::TAO_Priority_Mapping_Manager::_nil ();
}

ACE_INLINE
TAO_Priority_Mapping_Manager_out::TAO_Priority_Mapping_Manager_out (TAO_Priority_Mapping_Manager_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::TAO_Priority_Mapping_Manager::_nil ();
}

ACE_INLINE
TAO_Priority_Mapping_Manager_out::TAO_Priority_Mapping_Manager_out (const ::TAO_Priority_Mapping_Manager_out &p) // copy constructor
  : ptr_ (ACE_const_cast (TAO_Priority_Mapping_Manager_out &, p).ptr_)
{}

ACE_INLINE ::TAO_Priority_Mapping_Manager_out &
TAO_Priority_Mapping_Manager_out::operator= (const ::TAO_Priority_Mapping_Manager_out &p)
{
  this->ptr_ = ACE_const_cast (TAO_Priority_Mapping_Manager_out&, p).ptr_;
  return *this;
}

ACE_INLINE TAO_Priority_Mapping_Manager_out &
TAO_Priority_Mapping_Manager_out::operator= (const ::TAO_Priority_Mapping_Manager_var &p)
{
  this->ptr_ = ::TAO_Priority_Mapping_Manager::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE TAO_Priority_Mapping_Manager_out &
TAO_Priority_Mapping_Manager_out::operator= (TAO_Priority_Mapping_Manager_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
TAO_Priority_Mapping_Manager_out::operator ::TAO_Priority_Mapping_Manager_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr &
TAO_Priority_Mapping_Manager_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager_out::operator-> (void)
{
  return this->ptr_;
}

///////////////////////////////////////////////////////////////////////////
ACE_INLINE
TAO_Priority_Mapping_Manager::TAO_Priority_Mapping_Manager (RTCORBA::PriorityMapping * mapping)
  : mapping_ (mapping)
{
}

ACE_INLINE RTCORBA::PriorityMapping *
TAO_Priority_Mapping_Manager::mapping (void)
{
  return mapping_;
}

ACE_INLINE void
TAO_Priority_Mapping_Manager::mapping (RTCORBA::PriorityMapping *mapping)
{
  delete mapping_;
  mapping_ = mapping;
}


