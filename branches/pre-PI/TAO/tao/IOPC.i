// $Id$

// *************************************************************
// Inline operations for class IOP::TaggedProfile_var
// *************************************************************

ACE_INLINE
IOP::TaggedProfile_var::TaggedProfile_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
IOP::TaggedProfile_var::TaggedProfile_var (IOP::TaggedProfile *p)
  : ptr_ (p)
{}

ACE_INLINE
IOP::TaggedProfile_var::TaggedProfile_var (const IOP::TaggedProfile_var &p) // copy constructor
{
  if (p.ptr_)
    this->ptr_ = new IOP::TaggedProfile(*p.ptr_);
  else
    this->ptr_ = 0;
}

ACE_INLINE
IOP::TaggedProfile_var::~TaggedProfile_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE IOP::TaggedProfile_var &
IOP::TaggedProfile_var::operator= (IOP::TaggedProfile *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE IOP::TaggedProfile_var &
IOP::TaggedProfile_var::operator= (const IOP::TaggedProfile_var &p)
{
  if (this != &p)
  {
    delete this->ptr_;
    this->ptr_ = new IOP::TaggedProfile (*p.ptr_);
  }
  return *this;
}

ACE_INLINE const IOP::TaggedProfile *
IOP::TaggedProfile_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE IOP::TaggedProfile *
IOP::TaggedProfile_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
IOP::TaggedProfile_var::operator const IOP::TaggedProfile &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::TaggedProfile_var::operator IOP::TaggedProfile &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::TaggedProfile_var::operator IOP::TaggedProfile &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE const IOP::TaggedProfile &
IOP::TaggedProfile_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE IOP::TaggedProfile &
IOP::TaggedProfile_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE IOP::TaggedProfile *&
IOP::TaggedProfile_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE IOP::TaggedProfile *
IOP::TaggedProfile_var::_retn (void)
{
  IOP::TaggedProfile *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE IOP::TaggedProfile *
IOP::TaggedProfile_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class IOP::TaggedProfile_out
// *************************************************************

ACE_INLINE
IOP::TaggedProfile_out::TaggedProfile_out (IOP::TaggedProfile *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
IOP::TaggedProfile_out::TaggedProfile_out (IOP::TaggedProfile_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
IOP::TaggedProfile_out::TaggedProfile_out (const IOP::TaggedProfile_out &p) // copy constructor
  : ptr_ (ACE_const_cast (IOP::TaggedProfile_out&,p).ptr_)
{}

ACE_INLINE IOP::TaggedProfile_out &
IOP::TaggedProfile_out::operator= (const IOP::TaggedProfile_out &p)
{
  this->ptr_ = ACE_const_cast (IOP::TaggedProfile_out&,p).ptr_;
  return *this;
}

ACE_INLINE IOP::TaggedProfile_out &
IOP::TaggedProfile_out::operator= (IOP::TaggedProfile *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
IOP::TaggedProfile_out::operator IOP::TaggedProfile *&() // cast
{
  return this->ptr_;
}

ACE_INLINE IOP::TaggedProfile *&
IOP::TaggedProfile_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE IOP::TaggedProfile *
IOP::TaggedProfile_out::operator-> (void)
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class IOP::IOR_var
// *************************************************************

ACE_INLINE
IOP::IOR_var::IOR_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
IOP::IOR_var::IOR_var (IOP::IOR *p)
  : ptr_ (p)
{}

ACE_INLINE
IOP::IOR_var::IOR_var (const IOP::IOR_var &p) // copy constructor
{
  if (p.ptr_)
    this->ptr_ = new IOP::IOR(*p.ptr_);
  else
    this->ptr_ = 0;
}

ACE_INLINE
IOP::IOR_var::~IOR_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE IOP::IOR_var &
IOP::IOR_var::operator= (IOP::IOR *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE IOP::IOR_var &
IOP::IOR_var::operator= (const IOP::IOR_var &p)
{
  if (this != &p)
  {
    delete this->ptr_;
    this->ptr_ = new IOP::IOR (*p.ptr_);
  }
  return *this;
}

ACE_INLINE const IOP::IOR *
IOP::IOR_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE IOP::IOR *
IOP::IOR_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
IOP::IOR_var::operator const IOP::IOR &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::IOR_var::operator IOP::IOR &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::IOR_var::operator IOP::IOR &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE const IOP::IOR &
IOP::IOR_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE IOP::IOR &
IOP::IOR_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE IOP::IOR *&
IOP::IOR_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE IOP::IOR *
IOP::IOR_var::_retn (void)
{
  IOP::IOR *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE IOP::IOR *
IOP::IOR_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class IOP::IOR_out
// *************************************************************

ACE_INLINE
IOP::IOR_out::IOR_out (IOP::IOR *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
IOP::IOR_out::IOR_out (IOP::IOR_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
IOP::IOR_out::IOR_out (const IOP::IOR_out &p) // copy constructor
  : ptr_ (ACE_const_cast (IOP::IOR_out&,p).ptr_)
{}

ACE_INLINE IOP::IOR_out &
IOP::IOR_out::operator= (const IOP::IOR_out &p)
{
  this->ptr_ = ACE_const_cast (IOP::IOR_out&,p).ptr_;
  return *this;
}

ACE_INLINE IOP::IOR_out &
IOP::IOR_out::operator= (IOP::IOR *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
IOP::IOR_out::operator IOP::IOR *&() // cast
{
  return this->ptr_;
}

ACE_INLINE IOP::IOR *&
IOP::IOR_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE IOP::IOR *
IOP::IOR_out::operator-> (void)
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class IOP::TaggedComponent_var
// *************************************************************

ACE_INLINE
IOP::TaggedComponent_var::TaggedComponent_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
IOP::TaggedComponent_var::TaggedComponent_var (IOP::TaggedComponent *p)
  : ptr_ (p)
{}

ACE_INLINE
IOP::TaggedComponent_var::TaggedComponent_var (const IOP::TaggedComponent_var &p) // copy constructor
{
  if (p.ptr_)
    this->ptr_ = new IOP::TaggedComponent(*p.ptr_);
  else
    this->ptr_ = 0;
}

ACE_INLINE
IOP::TaggedComponent_var::~TaggedComponent_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE IOP::TaggedComponent_var &
IOP::TaggedComponent_var::operator= (IOP::TaggedComponent *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE IOP::TaggedComponent_var &
IOP::TaggedComponent_var::operator= (const IOP::TaggedComponent_var &p)
{
  if (this != &p)
  {
    delete this->ptr_;
    this->ptr_ = new IOP::TaggedComponent (*p.ptr_);
  }
  return *this;
}

ACE_INLINE const IOP::TaggedComponent *
IOP::TaggedComponent_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE IOP::TaggedComponent *
IOP::TaggedComponent_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
IOP::TaggedComponent_var::operator const IOP::TaggedComponent &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::TaggedComponent_var::operator IOP::TaggedComponent &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::TaggedComponent_var::operator IOP::TaggedComponent &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE const IOP::TaggedComponent &
IOP::TaggedComponent_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE IOP::TaggedComponent &
IOP::TaggedComponent_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE IOP::TaggedComponent *&
IOP::TaggedComponent_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE IOP::TaggedComponent *
IOP::TaggedComponent_var::_retn (void)
{
  IOP::TaggedComponent *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE IOP::TaggedComponent *
IOP::TaggedComponent_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class IOP::TaggedComponent_out
// *************************************************************

ACE_INLINE
IOP::TaggedComponent_out::TaggedComponent_out (IOP::TaggedComponent *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
IOP::TaggedComponent_out::TaggedComponent_out (IOP::TaggedComponent_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
IOP::TaggedComponent_out::TaggedComponent_out (const IOP::TaggedComponent_out &p) // copy constructor
  : ptr_ (ACE_const_cast (IOP::TaggedComponent_out&,p).ptr_)
{}

ACE_INLINE IOP::TaggedComponent_out &
IOP::TaggedComponent_out::operator= (const IOP::TaggedComponent_out &p)
{
  this->ptr_ = ACE_const_cast (IOP::TaggedComponent_out&,p).ptr_;
  return *this;
}

ACE_INLINE IOP::TaggedComponent_out &
IOP::TaggedComponent_out::operator= (IOP::TaggedComponent *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
IOP::TaggedComponent_out::operator IOP::TaggedComponent *&() // cast
{
  return this->ptr_;
}

ACE_INLINE IOP::TaggedComponent *&
IOP::TaggedComponent_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE IOP::TaggedComponent *
IOP::TaggedComponent_out::operator-> (void)
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class IOP::MultipleComponentProfile_var
// *************************************************************

ACE_INLINE
IOP::MultipleComponentProfile_var::MultipleComponentProfile_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
IOP::MultipleComponentProfile_var::MultipleComponentProfile_var (IOP::MultipleComponentProfile *p)
  : ptr_ (p)
{}

ACE_INLINE
IOP::MultipleComponentProfile_var::MultipleComponentProfile_var (const IOP::MultipleComponentProfile_var &p) // copy constructor
{
  if (p.ptr_)
    this->ptr_ = new IOP::MultipleComponentProfile(*p.ptr_);
  else
    this->ptr_ = 0;
}

ACE_INLINE
IOP::MultipleComponentProfile_var::~MultipleComponentProfile_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE IOP::MultipleComponentProfile_var &
IOP::MultipleComponentProfile_var::operator= (IOP::MultipleComponentProfile *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE IOP::MultipleComponentProfile_var &
IOP::MultipleComponentProfile_var::operator= (const IOP::MultipleComponentProfile_var &p) // deep copy
{
  if (this != &p)
  {
    delete this->ptr_;
    this->ptr_ = new IOP::MultipleComponentProfile (*p.ptr_);
  }
  return *this;
}

ACE_INLINE const IOP::MultipleComponentProfile *
IOP::MultipleComponentProfile_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE IOP::MultipleComponentProfile *
IOP::MultipleComponentProfile_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
IOP::MultipleComponentProfile_var::operator const IOP::MultipleComponentProfile &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::MultipleComponentProfile_var::operator IOP::MultipleComponentProfile &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::MultipleComponentProfile_var::operator IOP::MultipleComponentProfile &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE IOP::TaggedComponent &
IOP::MultipleComponentProfile_var::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

ACE_INLINE const IOP::MultipleComponentProfile &
IOP::MultipleComponentProfile_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE IOP::MultipleComponentProfile &
IOP::MultipleComponentProfile_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE IOP::MultipleComponentProfile *&
IOP::MultipleComponentProfile_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE IOP::MultipleComponentProfile *
IOP::MultipleComponentProfile_var::_retn (void)
{
  IOP::MultipleComponentProfile *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE IOP::MultipleComponentProfile *
IOP::MultipleComponentProfile_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class IOP::MultipleComponentProfile_out
// *************************************************************

ACE_INLINE
IOP::MultipleComponentProfile_out::MultipleComponentProfile_out (IOP::MultipleComponentProfile *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
IOP::MultipleComponentProfile_out::MultipleComponentProfile_out (IOP::MultipleComponentProfile_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
IOP::MultipleComponentProfile_out::MultipleComponentProfile_out (const IOP::MultipleComponentProfile_out &p) // copy constructor
  : ptr_ (ACE_const_cast (IOP::MultipleComponentProfile_out&,p).ptr_)
{}

ACE_INLINE IOP::MultipleComponentProfile_out &
IOP::MultipleComponentProfile_out::operator= (const IOP::MultipleComponentProfile_out &p)
{
  this->ptr_ = ACE_const_cast (IOP::MultipleComponentProfile_out&,p).ptr_;
  return *this;
}

ACE_INLINE IOP::MultipleComponentProfile_out &
IOP::MultipleComponentProfile_out::operator= (IOP::MultipleComponentProfile *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
IOP::MultipleComponentProfile_out::operator IOP::MultipleComponentProfile *&() // cast
{
  return this->ptr_;
}

ACE_INLINE IOP::MultipleComponentProfile *&
IOP::MultipleComponentProfile_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE IOP::MultipleComponentProfile *
IOP::MultipleComponentProfile_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE IOP::TaggedComponent &
IOP::MultipleComponentProfile_out::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

// *************************************************************
// Inline operations for class IOP::ServiceContext_var
// *************************************************************

ACE_INLINE
IOP::ServiceContext_var::ServiceContext_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
IOP::ServiceContext_var::ServiceContext_var (IOP::ServiceContext *p)
  : ptr_ (p)
{}

ACE_INLINE
IOP::ServiceContext_var::ServiceContext_var (const IOP::ServiceContext_var &p) // copy constructor
{
  if (p.ptr_)
    this->ptr_ = new IOP::ServiceContext(*p.ptr_);
  else
    this->ptr_ = 0;
}

ACE_INLINE
IOP::ServiceContext_var::~ServiceContext_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE IOP::ServiceContext_var &
IOP::ServiceContext_var::operator= (IOP::ServiceContext *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE IOP::ServiceContext_var &
IOP::ServiceContext_var::operator= (const IOP::ServiceContext_var &p)
{
  if (this != &p)
  {
    delete this->ptr_;
    this->ptr_ = new IOP::ServiceContext (*p.ptr_);
  }
  return *this;
}

ACE_INLINE const IOP::ServiceContext *
IOP::ServiceContext_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE IOP::ServiceContext *
IOP::ServiceContext_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
IOP::ServiceContext_var::operator const IOP::ServiceContext &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::ServiceContext_var::operator IOP::ServiceContext &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::ServiceContext_var::operator IOP::ServiceContext &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE const IOP::ServiceContext &
IOP::ServiceContext_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE IOP::ServiceContext &
IOP::ServiceContext_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE IOP::ServiceContext *&
IOP::ServiceContext_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE IOP::ServiceContext *
IOP::ServiceContext_var::_retn (void)
{
  IOP::ServiceContext *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE IOP::ServiceContext *
IOP::ServiceContext_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class IOP::ServiceContext_out
// *************************************************************

ACE_INLINE
IOP::ServiceContext_out::ServiceContext_out (IOP::ServiceContext *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
IOP::ServiceContext_out::ServiceContext_out (IOP::ServiceContext_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
IOP::ServiceContext_out::ServiceContext_out (const IOP::ServiceContext_out &p) // copy constructor
  : ptr_ (ACE_const_cast (IOP::ServiceContext_out&,p).ptr_)
{}

ACE_INLINE IOP::ServiceContext_out &
IOP::ServiceContext_out::operator= (const IOP::ServiceContext_out &p)
{
  this->ptr_ = ACE_const_cast (IOP::ServiceContext_out&,p).ptr_;
  return *this;
}

ACE_INLINE IOP::ServiceContext_out &
IOP::ServiceContext_out::operator= (IOP::ServiceContext *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
IOP::ServiceContext_out::operator IOP::ServiceContext *&() // cast
{
  return this->ptr_;
}

ACE_INLINE IOP::ServiceContext *&
IOP::ServiceContext_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE IOP::ServiceContext *
IOP::ServiceContext_out::operator-> (void)
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class IOP::ServiceContextList_var
// *************************************************************

ACE_INLINE
IOP::ServiceContextList_var::ServiceContextList_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
IOP::ServiceContextList_var::ServiceContextList_var (IOP::ServiceContextList *p)
  : ptr_ (p)
{}

ACE_INLINE
IOP::ServiceContextList_var::ServiceContextList_var (const IOP::ServiceContextList_var &p) // copy constructor
{
  if (p.ptr_)
    this->ptr_ = new IOP::ServiceContextList(*p.ptr_);
  else
    this->ptr_ = 0;
}

ACE_INLINE
IOP::ServiceContextList_var::~ServiceContextList_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE IOP::ServiceContextList_var &
IOP::ServiceContextList_var::operator= (IOP::ServiceContextList *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE IOP::ServiceContextList_var &
IOP::ServiceContextList_var::operator= (const IOP::ServiceContextList_var &p) // deep copy
{
  if (this != &p)
  {
    delete this->ptr_;
    this->ptr_ = new IOP::ServiceContextList (*p.ptr_);
  }
  return *this;
}

ACE_INLINE const IOP::ServiceContextList *
IOP::ServiceContextList_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE IOP::ServiceContextList *
IOP::ServiceContextList_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
IOP::ServiceContextList_var::operator const IOP::ServiceContextList &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::ServiceContextList_var::operator IOP::ServiceContextList &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
IOP::ServiceContextList_var::operator IOP::ServiceContextList &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE IOP::ServiceContext &
IOP::ServiceContextList_var::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

ACE_INLINE const IOP::ServiceContextList &
IOP::ServiceContextList_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE IOP::ServiceContextList &
IOP::ServiceContextList_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE IOP::ServiceContextList *&
IOP::ServiceContextList_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE IOP::ServiceContextList *
IOP::ServiceContextList_var::_retn (void)
{
  IOP::ServiceContextList *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE IOP::ServiceContextList *
IOP::ServiceContextList_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class IOP::ServiceContextList_out
// *************************************************************

ACE_INLINE
IOP::ServiceContextList_out::ServiceContextList_out (IOP::ServiceContextList *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
IOP::ServiceContextList_out::ServiceContextList_out (IOP::ServiceContextList_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
IOP::ServiceContextList_out::ServiceContextList_out (const IOP::ServiceContextList_out &p) // copy constructor
  : ptr_ (ACE_const_cast (IOP::ServiceContextList_out&,p).ptr_)
{}

ACE_INLINE IOP::ServiceContextList_out &
IOP::ServiceContextList_out::operator= (const IOP::ServiceContextList_out &p)
{
  this->ptr_ = ACE_const_cast (IOP::ServiceContextList_out&,p).ptr_;
  return *this;
}

ACE_INLINE IOP::ServiceContextList_out &
IOP::ServiceContextList_out::operator= (IOP::ServiceContextList *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
IOP::ServiceContextList_out::operator IOP::ServiceContextList *&() // cast
{
  return this->ptr_;
}

ACE_INLINE IOP::ServiceContextList *&
IOP::ServiceContextList_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE IOP::ServiceContextList *
IOP::ServiceContextList_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE IOP::ServiceContext &
IOP::ServiceContextList_out::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

CORBA::Boolean TAO_Export operator<< (
    TAO_OutputCDR &,
    const IOP::TaggedProfile::_tao_seq_Octet &
  );
CORBA::Boolean TAO_Export operator>> (
    TAO_InputCDR &,
    IOP::TaggedProfile::_tao_seq_Octet &
  );

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const IOP::TaggedProfile &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.tag) &&
    (strm << _tao_aggregate.profile_data)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, IOP::TaggedProfile &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.tag) &&
    (strm >> _tao_aggregate.profile_data)
  )
    return 1;
  else
    return 0;

}

CORBA::Boolean TAO_Export operator<< (
    TAO_OutputCDR &,
    const IOP::IOR::_tao_seq_TaggedProfile &
  );
CORBA::Boolean TAO_Export operator>> (
    TAO_InputCDR &,
    IOP::IOR::_tao_seq_TaggedProfile &
  );

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const IOP::IOR &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.type_id.in ()) &&
    (strm << _tao_aggregate.profiles)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, IOP::IOR &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.type_id.out ()) &&
    (strm >> _tao_aggregate.profiles)
  )
    return 1;
  else
    return 0;

}

CORBA::Boolean TAO_Export operator<< (
    TAO_OutputCDR &,
    const IOP::TaggedComponent::_tao_seq_Octet &
  );
CORBA::Boolean TAO_Export operator>> (
    TAO_InputCDR &,
    IOP::TaggedComponent::_tao_seq_Octet &
  );

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const IOP::TaggedComponent &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.tag) &&
    (strm << _tao_aggregate.component_data)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, IOP::TaggedComponent &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.tag) &&
    (strm >> _tao_aggregate.component_data)
  )
    return 1;
  else
    return 0;

}

CORBA::Boolean TAO_Export operator<< (
    TAO_OutputCDR &,
    const IOP::MultipleComponentProfile &
  );
CORBA::Boolean TAO_Export operator>> (
    TAO_InputCDR &,
    IOP::MultipleComponentProfile &
  );

CORBA::Boolean TAO_Export operator<< (
    TAO_OutputCDR &,
    const IOP::ServiceContext::_tao_seq_Octet &
  );
CORBA::Boolean TAO_Export operator>> (
    TAO_InputCDR &,
    IOP::ServiceContext::_tao_seq_Octet &
  );

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const IOP::ServiceContext &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.context_id) &&
    (strm << _tao_aggregate.context_data)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, IOP::ServiceContext &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.context_id) &&
    (strm >> _tao_aggregate.context_data)
  )
    return 1;
  else
    return 0;

}

CORBA::Boolean TAO_Export operator<< (
    TAO_OutputCDR &,
    const IOP::ServiceContextList &
  );
CORBA::Boolean TAO_Export operator>> (
    TAO_InputCDR &,
    IOP::ServiceContextList &
  );
