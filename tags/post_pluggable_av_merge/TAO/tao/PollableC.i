// $Id$

ACE_INLINE
CORBA_Pollable::CORBA_Pollable (void) // default constructor
{}

ACE_INLINE
CORBA_Pollable::CORBA_Pollable (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean _tao_collocated) // constructor
  : CORBA_Object (objref, _tao_servant, _tao_collocated)
{}

ACE_INLINE
CORBA_Pollable::~CORBA_Pollable (void) // destructor
{}

ACE_INLINE CORBA_Pollable_ptr
CORBA_Pollable::_duplicate (CORBA_Pollable_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_incr_refcnt ();
  return obj;
}

ACE_INLINE CORBA_Pollable_ptr
CORBA_Pollable::_nil (void)
{
  return (CORBA_Pollable_ptr)0;
}

// *************************************************************
// Inline operations for class CORBA_Pollable_var
// *************************************************************

ACE_INLINE
CORBA_Pollable_var::CORBA_Pollable_var (void) // default constructor
  : ptr_ (CORBA_Pollable::_nil ())
{}

ACE_INLINE
CORBA_Pollable_var::CORBA_Pollable_var (CORBA_Pollable_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA_Pollable_ptr
CORBA_Pollable_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Pollable_var::CORBA_Pollable_var (const CORBA_Pollable_var &p) // copy constructor
  : ptr_ (CORBA_Pollable::_duplicate (p.ptr ()))
{}

ACE_INLINE
CORBA_Pollable_var::~CORBA_Pollable_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_Pollable_var &
CORBA_Pollable_var::operator= (CORBA_Pollable_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_Pollable_var &
CORBA_Pollable_var::operator= (const CORBA_Pollable_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA_Pollable::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_Pollable_var::operator const CORBA_Pollable_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_Pollable_var::operator CORBA_Pollable_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_Pollable_ptr
CORBA_Pollable_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Pollable_ptr
CORBA_Pollable_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_Pollable_ptr &
CORBA_Pollable_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_Pollable_ptr &
CORBA_Pollable_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_Pollable::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_Pollable_ptr
CORBA_Pollable_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA_Pollable_ptr val = this->ptr_;
  this->ptr_ = CORBA_Pollable::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_Pollable_out
// *************************************************************

ACE_INLINE
CORBA_Pollable_out::CORBA_Pollable_out (CORBA_Pollable_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = CORBA_Pollable::_nil ();
}

ACE_INLINE
CORBA_Pollable_out::CORBA_Pollable_out (CORBA_Pollable_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_Pollable::_nil ();
}

ACE_INLINE
CORBA_Pollable_out::CORBA_Pollable_out (const CORBA_Pollable_out &p) // copy constructor
  : ptr_ (ACE_const_cast (CORBA_Pollable_out&,p).ptr_)
{}

ACE_INLINE CORBA_Pollable_out &
CORBA_Pollable_out::operator= (const CORBA_Pollable_out &p)
{
  this->ptr_ = ACE_const_cast (CORBA_Pollable_out&,p).ptr_;
  return *this;
}

ACE_INLINE CORBA_Pollable_out &
CORBA_Pollable_out::operator= (const CORBA_Pollable_var &p)
{
  this->ptr_ = CORBA_Pollable::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_Pollable_out &
CORBA_Pollable_out::operator= (CORBA_Pollable_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_Pollable_out::operator CORBA_Pollable_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_Pollable_ptr &
CORBA_Pollable_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA_Pollable_ptr
CORBA_Pollable_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
CORBA_DIIPollable::CORBA_DIIPollable (void) // default constructor
{}

ACE_INLINE
CORBA_DIIPollable::CORBA_DIIPollable (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean _tao_collocated) // constructor
  : CORBA_Object (objref, _tao_servant, _tao_collocated)
{}

ACE_INLINE
CORBA_DIIPollable::~CORBA_DIIPollable (void) // destructor
{}

ACE_INLINE CORBA_DIIPollable_ptr
CORBA_DIIPollable::_duplicate (CORBA_DIIPollable_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_incr_refcnt ();
  return obj;
}

ACE_INLINE CORBA_DIIPollable_ptr
CORBA_DIIPollable::_nil (void)
{
  return (CORBA_DIIPollable_ptr)0;
}


// *************************************************************
// Inline operations for class CORBA_DIIPollable_var
// *************************************************************

ACE_INLINE
CORBA_DIIPollable_var::CORBA_DIIPollable_var (void) // default constructor
  : ptr_ (CORBA_DIIPollable::_nil ())
{}

ACE_INLINE
CORBA_DIIPollable_var::CORBA_DIIPollable_var (CORBA_DIIPollable_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA_DIIPollable_ptr
CORBA_DIIPollable_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_DIIPollable_var::CORBA_DIIPollable_var (const CORBA_DIIPollable_var &p) // copy constructor
  : ptr_ (CORBA_DIIPollable::_duplicate (p.ptr ()))
{}

ACE_INLINE
CORBA_DIIPollable_var::~CORBA_DIIPollable_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_DIIPollable_var &
CORBA_DIIPollable_var::operator= (CORBA_DIIPollable_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_DIIPollable_var &
CORBA_DIIPollable_var::operator= (const CORBA_DIIPollable_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA_DIIPollable::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_DIIPollable_var::operator const CORBA_DIIPollable_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_DIIPollable_var::operator CORBA_DIIPollable_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_DIIPollable_ptr
CORBA_DIIPollable_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_DIIPollable_ptr
CORBA_DIIPollable_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_DIIPollable_ptr &
CORBA_DIIPollable_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_DIIPollable_ptr &
CORBA_DIIPollable_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_DIIPollable::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_DIIPollable_ptr
CORBA_DIIPollable_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA_DIIPollable_ptr val = this->ptr_;
  this->ptr_ = CORBA_DIIPollable::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_DIIPollable_out
// *************************************************************

ACE_INLINE
CORBA_DIIPollable_out::CORBA_DIIPollable_out (CORBA_DIIPollable_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = CORBA_DIIPollable::_nil ();
}

ACE_INLINE
CORBA_DIIPollable_out::CORBA_DIIPollable_out (CORBA_DIIPollable_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_DIIPollable::_nil ();
}

ACE_INLINE
CORBA_DIIPollable_out::CORBA_DIIPollable_out (const CORBA_DIIPollable_out &p) // copy constructor
  : ptr_ (ACE_const_cast (CORBA_DIIPollable_out&,p).ptr_)
{}

ACE_INLINE CORBA_DIIPollable_out &
CORBA_DIIPollable_out::operator= (const CORBA_DIIPollable_out &p)
{
  this->ptr_ = ACE_const_cast (CORBA_DIIPollable_out&,p).ptr_;
  return *this;
}

ACE_INLINE CORBA_DIIPollable_out &
CORBA_DIIPollable_out::operator= (const CORBA_DIIPollable_var &p)
{
  this->ptr_ = CORBA_DIIPollable::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_DIIPollable_out &
CORBA_DIIPollable_out::operator= (CORBA_DIIPollable_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_DIIPollable_out::operator CORBA_DIIPollable_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_DIIPollable_ptr &
CORBA_DIIPollable_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA_DIIPollable_ptr
CORBA_DIIPollable_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
CORBA_PollableSet::CORBA_PollableSet (void) // default constructor
{}

ACE_INLINE
CORBA_PollableSet::CORBA_PollableSet (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean _tao_collocated) // constructor
  : CORBA_Object (objref, _tao_servant, _tao_collocated)
{}

ACE_INLINE
CORBA_PollableSet::~CORBA_PollableSet (void) // destructor
{}

ACE_INLINE CORBA_PollableSet_ptr
CORBA_PollableSet::_duplicate (CORBA_PollableSet_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_incr_refcnt ();
  return obj;
}

ACE_INLINE CORBA_PollableSet_ptr
CORBA_PollableSet::_nil (void)
{
  return (CORBA_PollableSet_ptr)0;
}

// *************************************************************
// Inline operations for class CORBA_PollableSet_var
// *************************************************************

ACE_INLINE
CORBA_PollableSet_var::CORBA_PollableSet_var (void) // default constructor
  : ptr_ (CORBA_PollableSet::_nil ())
{}

ACE_INLINE
CORBA_PollableSet_var::CORBA_PollableSet_var (CORBA_PollableSet_ptr p)
  : ptr_ (p)
{}

ACE_INLINE CORBA_PollableSet_ptr
CORBA_PollableSet_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_PollableSet_var::CORBA_PollableSet_var (const CORBA_PollableSet_var &p) // copy constructor
  : ptr_ (CORBA_PollableSet::_duplicate (p.ptr ()))
{}

ACE_INLINE
CORBA_PollableSet_var::~CORBA_PollableSet_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_PollableSet_var &
CORBA_PollableSet_var::operator= (CORBA_PollableSet_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_PollableSet_var &
CORBA_PollableSet_var::operator= (const CORBA_PollableSet_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = CORBA_PollableSet::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
CORBA_PollableSet_var::operator const CORBA_PollableSet_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_PollableSet_var::operator CORBA_PollableSet_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_PollableSet_ptr
CORBA_PollableSet_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_PollableSet_ptr
CORBA_PollableSet_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA_PollableSet_ptr &
CORBA_PollableSet_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA_PollableSet_ptr &
CORBA_PollableSet_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_PollableSet::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA_PollableSet_ptr
CORBA_PollableSet_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA_PollableSet_ptr val = this->ptr_;
  this->ptr_ = CORBA_PollableSet::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_PollableSet_out
// *************************************************************

ACE_INLINE
CORBA_PollableSet_out::CORBA_PollableSet_out (CORBA_PollableSet_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = CORBA_PollableSet::_nil ();
}

ACE_INLINE
CORBA_PollableSet_out::CORBA_PollableSet_out (CORBA_PollableSet_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_PollableSet::_nil ();
}

ACE_INLINE
CORBA_PollableSet_out::CORBA_PollableSet_out (const CORBA_PollableSet_out &p) // copy constructor
  : ptr_ (ACE_const_cast (CORBA_PollableSet_out&,p).ptr_)
{}

ACE_INLINE CORBA_PollableSet_out &
CORBA_PollableSet_out::operator= (const CORBA_PollableSet_out &p)
{
  this->ptr_ = ACE_const_cast (CORBA_PollableSet_out&,p).ptr_;
  return *this;
}

ACE_INLINE CORBA_PollableSet_out &
CORBA_PollableSet_out::operator= (const CORBA_PollableSet_var &p)
{
  this->ptr_ = CORBA_PollableSet::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_PollableSet_out &
CORBA_PollableSet_out::operator= (CORBA_PollableSet_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_PollableSet_out::operator CORBA_PollableSet_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA_PollableSet_ptr &
CORBA_PollableSet_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA_PollableSet_ptr
CORBA_PollableSet_out::operator-> (void)
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for exception CORBA_PollableSet::NoPossiblePollable
// *************************************************************

// *************************************************************
// Inline operations for exception CORBA_PollableSet::UnknownPollable
// *************************************************************

CORBA::Boolean TAO_Export
operator<< (
    TAO_OutputCDR &,
    const CORBA_PollableSet_ptr
  );
CORBA::Boolean TAO_Export
operator>> (
    TAO_InputCDR &,
    CORBA_PollableSet_ptr &
  );

CORBA::Boolean TAO_Export
operator<< (
    TAO_OutputCDR &,
    const CORBA_Pollable_ptr
  );
CORBA::Boolean TAO_Export
operator>> (
    TAO_InputCDR &,
    CORBA_Pollable_ptr &
  );

CORBA::Boolean TAO_Export
operator<< (
    TAO_OutputCDR &,
    const CORBA_DIIPollable_ptr
  );
CORBA::Boolean TAO_Export
operator>> (
    TAO_InputCDR &,
    CORBA_DIIPollable_ptr &
  );

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const CORBA_PollableSet::NoPossiblePollable &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, CORBA_PollableSet::NoPossiblePollable &_tao_aggregate)
{
  // retrieve  RepoID and verify if we are of that type
  char *_tao_repoID;
  if ((strm >> _tao_repoID) &&
      (_tao_aggregate._is_a (_tao_repoID)))
  {
    return 1;
  }
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const CORBA_PollableSet::UnknownPollable &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, CORBA_PollableSet::UnknownPollable &_tao_aggregate)
{
  // retrieve  RepoID and verify if we are of that type
  char *_tao_repoID;
  if ((strm >> _tao_repoID) &&
      (_tao_aggregate._is_a (_tao_repoID)))
  {
    return 1;
  }
  else
    return 0;
}
