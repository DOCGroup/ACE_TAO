//$Id$
#if (TAO_HAS_FT_CORBA == 1)


ACE_INLINE
FT_GenericFactory::FT_GenericFactory (TAO_Stub *objref,
				       TAO_ServantBase *_tao_servant,
				       CORBA::Boolean _tao_collocated) // constructor
  : CORBA_Object (objref, _tao_servant,	_tao_collocated)
{}



// *************************************************************
// Inline operations for class FT::GenericFactory_var
// *************************************************************

ACE_INLINE
FT_GenericFactory_var::FT_GenericFactory_var (void) // default constructor
  : ptr_ (FT_GenericFactory::_nil ())
{}

ACE_INLINE FT_GenericFactory_ptr
FT_GenericFactory_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
FT_GenericFactory_var::FT_GenericFactory_var (const FT_GenericFactory_var &p) // copy constructor
  : TAO_Base_var (),
    ptr_ (FT_GenericFactory::_duplicate	(p.ptr ()))
{}

ACE_INLINE
FT_GenericFactory_var::~FT_GenericFactory_var (void) //	destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE FT_GenericFactory_var &
FT_GenericFactory_var::operator= (FT_GenericFactory_ptr	p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_GenericFactory_var &
FT_GenericFactory_var::operator= (const	FT_GenericFactory_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = FT_GenericFactory::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
FT_GenericFactory_var::operator	const FT_GenericFactory_ptr &()	const // cast
{
  return this->ptr_;
}

ACE_INLINE
FT_GenericFactory_var::operator	FT_GenericFactory_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE FT_GenericFactory_ptr
FT_GenericFactory_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE FT_GenericFactory_ptr
FT_GenericFactory_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE FT_GenericFactory_ptr &
FT_GenericFactory_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE FT_GenericFactory_ptr &
FT_GenericFactory_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = FT_GenericFactory::_nil ();
  return this->ptr_;
}

ACE_INLINE FT_GenericFactory_ptr
FT_GenericFactory_var::_retn (void)
{
  // yield ownership of	managed	obj reference
  FT_GenericFactory_ptr	val = this->ptr_;
  this->ptr_ = FT_GenericFactory::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class FT::GenericFactory_out
// *************************************************************

ACE_INLINE
FT_GenericFactory_out::FT_GenericFactory_out (FT_GenericFactory_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = FT_GenericFactory::_nil ();
}

ACE_INLINE
FT_GenericFactory_out::FT_GenericFactory_out (FT_GenericFactory_var &p)	// constructor from _var
  : ptr_ (p.out	())
{
  CORBA::release (this->ptr_);
  this->ptr_ = FT_GenericFactory::_nil ();
}

ACE_INLINE
FT_GenericFactory_out::FT_GenericFactory_out (const FT_GenericFactory_out &p) // copy constructor
  : ptr_ (ACE_const_cast (FT_GenericFactory_out&,p).ptr_)
{}

ACE_INLINE FT_GenericFactory_out &
FT_GenericFactory_out::operator= (const	FT_GenericFactory_out &p)
{
  this->ptr_ = ACE_const_cast (FT_GenericFactory_out&,p).ptr_;
  return *this;
}

ACE_INLINE FT_GenericFactory_out &
FT_GenericFactory_out::operator= (const	FT_GenericFactory_var &p)
{
  this->ptr_ = FT_GenericFactory::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE FT_GenericFactory_out &
FT_GenericFactory_out::operator= (FT_GenericFactory_ptr	p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_GenericFactory_out::operator	FT_GenericFactory_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE FT_GenericFactory_ptr &
FT_GenericFactory_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE FT_GenericFactory_ptr
FT_GenericFactory_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
FT_FaultNotifier::FT_FaultNotifier (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean _tao_collocated) //	constructor
  : CORBA_Object (objref, _tao_servant,	_tao_collocated)
{}


// *************************************************************
// Inline operations for class FT::FaultNotifier_var
// *************************************************************

ACE_INLINE
FT_FaultNotifier_var::FT_FaultNotifier_var (void) // default constructor
  : ptr_ (FT_FaultNotifier::_nil ())
{}

ACE_INLINE FT_FaultNotifier_ptr
FT_FaultNotifier_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
FT_FaultNotifier_var::FT_FaultNotifier_var (const FT_FaultNotifier_var &p) // copy constructor
  : TAO_Base_var (),
    ptr_ (FT_FaultNotifier::_duplicate (p.ptr ()))
{}

ACE_INLINE
FT_FaultNotifier_var::~FT_FaultNotifier_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE FT_FaultNotifier_var	&
FT_FaultNotifier_var::operator=	(FT_FaultNotifier_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_FaultNotifier_var	&
FT_FaultNotifier_var::operator=	(const FT_FaultNotifier_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = FT_FaultNotifier::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
FT_FaultNotifier_var::operator const FT_FaultNotifier_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
FT_FaultNotifier_var::operator FT_FaultNotifier_ptr &()	// cast
{
  return this->ptr_;
}

ACE_INLINE FT_FaultNotifier_ptr
FT_FaultNotifier_var::operator-> (void)	const
{
  return this->ptr_;
}

ACE_INLINE FT_FaultNotifier_ptr
FT_FaultNotifier_var::in (void)	const
{
  return this->ptr_;
}

ACE_INLINE FT_FaultNotifier_ptr	&
FT_FaultNotifier_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE FT_FaultNotifier_ptr	&
FT_FaultNotifier_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = FT_FaultNotifier::_nil ();
  return this->ptr_;
}

ACE_INLINE FT_FaultNotifier_ptr
FT_FaultNotifier_var::_retn (void)
{
  // yield ownership of	managed	obj reference
  FT_FaultNotifier_ptr val = this->ptr_;
  this->ptr_ = FT_FaultNotifier::_nil ();
  return val;
}


// *************************************************************
// Inline operations for class FT::FaultNotifier_out
// *************************************************************

ACE_INLINE
FT_FaultNotifier_out::FT_FaultNotifier_out (FT_FaultNotifier_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = FT_FaultNotifier::_nil ();
}

ACE_INLINE
FT_FaultNotifier_out::FT_FaultNotifier_out (FT_FaultNotifier_var &p) //	constructor from _var
  : ptr_ (p.out	())
{
  CORBA::release (this->ptr_);
  this->ptr_ = FT_FaultNotifier::_nil ();
}

ACE_INLINE
FT_FaultNotifier_out::FT_FaultNotifier_out (const FT_FaultNotifier_out &p) // copy constructor
  : ptr_ (ACE_const_cast (FT_FaultNotifier_out&,p).ptr_)
{}

ACE_INLINE FT_FaultNotifier_out	&
FT_FaultNotifier_out::operator=	(const FT_FaultNotifier_out &p)
{
  this->ptr_ = ACE_const_cast (FT_FaultNotifier_out&,p).ptr_;
  return *this;
}

ACE_INLINE FT_FaultNotifier_out	&
FT_FaultNotifier_out::operator=	(const FT_FaultNotifier_var &p)
{
  this->ptr_ = FT_FaultNotifier::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE FT_FaultNotifier_out	&
FT_FaultNotifier_out::operator=	(FT_FaultNotifier_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_FaultNotifier_out::operator FT_FaultNotifier_ptr &()	// cast
{
  return this->ptr_;
}

ACE_INLINE FT_FaultNotifier_ptr	&
FT_FaultNotifier_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE FT_FaultNotifier_ptr
FT_FaultNotifier_out::operator-> (void)
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class FT_Name_var
// *************************************************************

ACE_INLINE
FT_Name_var::FT_Name_var (void)	// default constructor
  : ptr_ (0)
{}

ACE_INLINE
FT_Name_var::FT_Name_var (FT_Name *p)
  : ptr_ (p)
{}

ACE_INLINE
FT_Name_var::FT_Name_var (const	FT_Name_var &p)	// copy	constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, FT_Name (*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
FT_Name_var::~FT_Name_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE FT_Name_var &
FT_Name_var::operator= (FT_Name	*p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_Name_var &
FT_Name_var::operator= (const FT_Name_var &p) // deep copy
{
  if (this != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,	FT_Name	(*p.ptr_), *this);
  }
  return *this;
}

ACE_INLINE const FT_Name *
FT_Name_var::operator->	(void) const
{
  return this->ptr_;
}

ACE_INLINE FT_Name *
FT_Name_var::operator->	(void)
{
  return this->ptr_;
}

ACE_INLINE
FT_Name_var::operator const FT_Name &()	const // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_Name_var::operator FT_Name &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_Name_var::operator FT_Name &() const	// cast
{
  return *this->ptr_;
}

// variable-size types only
ACE_INLINE
FT_Name_var::operator FT_Name *&() // cast
{
  return this->ptr_;
}

ACE_INLINE CosNaming::NameComponent &
FT_Name_var::operator[]	(CORBA::ULong index)
{
  return this->ptr_->operator[]	(index);
}

ACE_INLINE const FT_Name &
FT_Name_var::in	(void) const
{
  return *this->ptr_;
}

ACE_INLINE FT_Name &
FT_Name_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable	size
ACE_INLINE FT_Name *&
FT_Name_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE FT_Name *
FT_Name_var::_retn (void)
{
  FT_Name *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE FT_Name *
FT_Name_var::ptr (void)	const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class FT_Name_out
// *************************************************************

ACE_INLINE
FT_Name_out::FT_Name_out (FT_Name *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
FT_Name_out::FT_Name_out (FT_Name_var &p) // constructor from _var
  : ptr_ (p.out	())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
FT_Name_out::FT_Name_out (const	::FT_Name_out &p) // copy constructor
  : ptr_ (ACE_const_cast (FT_Name_out&,	p).ptr_)
{}

ACE_INLINE ::FT_Name_out &
FT_Name_out::operator= (const ::FT_Name_out &p)
{
  this->ptr_ = ACE_const_cast (FT_Name_out&, p).ptr_;
  return *this;
}

ACE_INLINE ::FT_Name_out &
FT_Name_out::operator= (FT_Name	*p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_Name_out::operator ::FT_Name	*&() //	cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_Name *&
FT_Name_out::ptr (void)	// ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_Name *
FT_Name_out::operator->	(void)
{
  return this->ptr_;
}

ACE_INLINE CosNaming::NameComponent &
FT_Name_out::operator[]	(CORBA::ULong index)
{
  return this->ptr_->operator[]	(index);
}

// *************************************************************
// Inline operations for class FT::Property_var
// *************************************************************

ACE_INLINE
FT_Property_var::FT_Property_var (void)	// default constructor
  : ptr_ (0)
{}

ACE_INLINE
FT_Property_var::FT_Property_var (FT_Property *p)
  : ptr_ (p)
{}

ACE_INLINE
FT_Property_var::FT_Property_var (const	::FT_Property_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::FT_Property (*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
FT_Property_var::~FT_Property_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE FT_Property_var &
FT_Property_var::operator= (FT_Property	*p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE ::FT_Property_var &
FT_Property_var::operator= (const ::FT_Property_var &p)
{
  if (this != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,	::FT_Property (*p.ptr_), *this);
  }
  return *this;
}

ACE_INLINE const ::FT_Property *
FT_Property_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_Property *
FT_Property_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
FT_Property_var::operator const	::FT_Property &() const	// cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_Property_var::operator ::FT_Property	&() // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_Property_var::operator ::FT_Property	&() const // cast
{
  return *this->ptr_;
}

// variable-size types only
ACE_INLINE
FT_Property_var::operator ::FT_Property	*&() //	cast
{
  return this->ptr_;
}

ACE_INLINE const ::FT_Property &
FT_Property_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE ::FT_Property &
FT_Property_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable	size
ACE_INLINE ::FT_Property *&
FT_Property_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE ::FT_Property *
FT_Property_var::_retn (void)
{
  ::FT_Property	*tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE ::FT_Property *
FT_Property_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class FT_Property_out
// *************************************************************

ACE_INLINE
FT_Property_out::FT_Property_out (::FT_Property	*&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
FT_Property_out::FT_Property_out (FT_Property_var &p) // constructor from _var
  : ptr_ (p.out	())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
FT_Property_out::FT_Property_out (const	::FT_Property_out &p) // copy constructor
  : ptr_ (ACE_const_cast (FT_Property_out&, p).ptr_)
{}

ACE_INLINE FT_Property_out &
FT_Property_out::operator= (const ::FT_Property_out &p)
{
  this->ptr_ = ACE_const_cast (FT_Property_out&, p).ptr_;
  return *this;
}

ACE_INLINE FT_Property_out &
FT_Property_out::operator= (FT_Property	*p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_Property_out::operator ::FT_Property	*&() //	cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_Property *&
FT_Property_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_Property *
FT_Property_out::operator-> (void)
{
  return this->ptr_;
}


#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

// = Static operations.
ACE_INLINE FT_Property *
_TAO_Unbounded_Sequence_FT_Properties::allocbuf	(CORBA::ULong size)
  // Allocate storage for the sequence.
{
  FT_Property *retval =	0;
  ACE_NEW_RETURN (retval, FT_Property[size], 0);
  return retval;
}

ACE_INLINE void
_TAO_Unbounded_Sequence_FT_Properties::freebuf (FT_Property *buffer)
  // Free the sequence.
{
  delete [] buffer;
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Properties::_TAO_Unbounded_Sequence_FT_Properties (void) // Default constructor.
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Properties::_TAO_Unbounded_Sequence_FT_Properties (CORBA::ULong maximum) // Constructor using a maximum length value.
  : TAO_Unbounded_Base_Sequence	(maximum, _TAO_Unbounded_Sequence_FT_Properties::allocbuf (maximum))
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Properties::_TAO_Unbounded_Sequence_FT_Properties (CORBA::ULong maximum,
									      CORBA::ULong length,
									      FT_Property *data,
									      CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence	(maximum, length, data,	release)
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Properties::_TAO_Unbounded_Sequence_FT_Properties (const _TAO_Unbounded_Sequence_FT_Properties &rhs)
  // Copy constructor.
  : TAO_Unbounded_Base_Sequence	(rhs)
{
  if (rhs.buffer_ != 0)
    {
      FT_Property *tmp1	= _TAO_Unbounded_Sequence_FT_Properties::allocbuf (this->maximum_);
      FT_Property * const tmp2 = ACE_reinterpret_cast (FT_Property * ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong	i = 0; i < this->length_; ++i)
	tmp1[i]	= tmp2[i];

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

ACE_INLINE _TAO_Unbounded_Sequence_FT_Properties &
_TAO_Unbounded_Sequence_FT_Properties::operator= (const	_TAO_Unbounded_Sequence_FT_Properties &rhs)
  // Assignment	operator.
{
  if (this == &rhs)
    return *this;

  if (this->release_)
    {
      if (this->maximum_ < rhs.maximum_)
	{
	  // free the old buffer
	  FT_Property *tmp = ACE_reinterpret_cast (FT_Property *, this->buffer_);
	  _TAO_Unbounded_Sequence_FT_Properties::freebuf (tmp);
	  this->buffer_	= _TAO_Unbounded_Sequence_FT_Properties::allocbuf (rhs.maximum_);
	}
    }
  else
    this->buffer_ = _TAO_Unbounded_Sequence_FT_Properties::allocbuf (rhs.maximum_);

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  FT_Property *tmp1 = ACE_reinterpret_cast (FT_Property	*, this->buffer_);
  FT_Property *	const tmp2 = ACE_reinterpret_cast (FT_Property * ACE_CAST_CONST, rhs.buffer_);

  for (CORBA::ULong i =	0; i < this->length_; ++i)
    tmp1[i] = tmp2[i];

  return *this;
}

// = Accessors.
ACE_INLINE FT_Property &
_TAO_Unbounded_Sequence_FT_Properties::operator[] (CORBA::ULong	i)
  // operator []
{
  ACE_ASSERT (i	< this->maximum_);
  FT_Property* tmp = ACE_reinterpret_cast(FT_Property*,this->buffer_);
  return tmp[i];
}

ACE_INLINE const FT_Property &
_TAO_Unbounded_Sequence_FT_Properties::operator[] (CORBA::ULong	i) const
  // operator []
{
  ACE_ASSERT (i	< this->maximum_);
  FT_Property *	const tmp = ACE_reinterpret_cast (FT_Property* ACE_CAST_CONST,
						  this->buffer_);
  return tmp[i];
}

// Implement the TAO_Base_Sequence methods (see	Sequence.h)

ACE_INLINE FT_Property *
_TAO_Unbounded_Sequence_FT_Properties::get_buffer (CORBA::Boolean orphan)
{
  FT_Property *result =	0;
  if (orphan ==	0)
    {
      // We retain ownership.
      if (this->buffer_	== 0)
	{
	  result = _TAO_Unbounded_Sequence_FT_Properties::allocbuf (this->length_);
	  this->buffer_	= result;
	}
      else
	{
	  result = ACE_reinterpret_cast	(FT_Property*, this->buffer_);
	}
    }
  else // if (orphan ==	1)
    {
      if (this->release_ != 0)
	{
	  // We	set the	state back to default and relinquish
	  // ownership.
	  result = ACE_reinterpret_cast(FT_Property*,this->buffer_);
	  this->maximum_ = 0;
	  this->length_	= 0;
	  this->buffer_	= 0;
	  this->release_ = 0;
	}
    }
  return result;
}

ACE_INLINE const FT_Property *
_TAO_Unbounded_Sequence_FT_Properties::get_buffer (void) const
{
  return ACE_reinterpret_cast(const FT_Property	* ACE_CAST_CONST, this->buffer_);
}

ACE_INLINE void
_TAO_Unbounded_Sequence_FT_Properties::replace (CORBA::ULong max,
  CORBA::ULong length,
  FT_Property *data,
  CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_	= length;
  if (this->buffer_ && this->release_ == 1)
    {
      FT_Property *tmp = ACE_reinterpret_cast(FT_Property*,this->buffer_);
      _TAO_Unbounded_Sequence_FT_Properties::freebuf (tmp);
    }
    this->buffer_ = data;
    this->release_ = release;
}


#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

// Need	to clean from here -- Bala
// @@@@@@@@@@

// *************************************************************
// Inline operations for class FT_Properties_var
// *************************************************************

ACE_INLINE
FT_Properties_var::FT_Properties_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
FT_Properties_var::FT_Properties_var (FT_Properties *p)
  : ptr_ (p)
{}

ACE_INLINE
FT_Properties_var::FT_Properties_var (const ::FT_Properties_var	&p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::FT_Properties (*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
FT_Properties_var::~FT_Properties_var (void) //	destructor
{
  delete this->ptr_;
}

ACE_INLINE FT_Properties_var &
FT_Properties_var::operator= (FT_Properties *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_Properties_var &
FT_Properties_var::operator= (const ::FT_Properties_var	&p) // deep copy
{
  if (this != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,	::FT_Properties	(*p.ptr_), *this);
  }
  return *this;
}

ACE_INLINE const ::FT_Properties *
FT_Properties_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_Properties *
FT_Properties_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
FT_Properties_var::operator const ::FT_Properties &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_Properties_var::operator ::FT_Properties &()	// cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_Properties_var::operator ::FT_Properties &()	const // cast
{
  return *this->ptr_;
}

// variable-size types only
ACE_INLINE
FT_Properties_var::operator ::FT_Properties *&() // cast
{
  return this->ptr_;
}

ACE_INLINE FT_Property &
FT_Properties_var::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[]	(index);
}

ACE_INLINE const ::FT_Properties &
FT_Properties_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE ::FT_Properties &
FT_Properties_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable	size
ACE_INLINE ::FT_Properties *&
FT_Properties_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE ::FT_Properties *
FT_Properties_var::_retn (void)
{
  ::FT_Properties *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE ::FT_Properties *
FT_Properties_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class FT_Properties_out
// *************************************************************

ACE_INLINE
FT_Properties_out::FT_Properties_out (FT_Properties *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
FT_Properties_out::FT_Properties_out (FT_Properties_var	&p) // constructor from	_var
  : ptr_ (p.out	())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
FT_Properties_out::FT_Properties_out (const ::FT_Properties_out	&p) // copy constructor
  : ptr_ (ACE_const_cast (FT_Properties_out&, p).ptr_)
{}

ACE_INLINE ::FT_Properties_out &
FT_Properties_out::operator= (const ::FT_Properties_out	&p)
{
  this->ptr_ = ACE_const_cast (FT_Properties_out&, p).ptr_;
  return *this;
}

ACE_INLINE ::FT_Properties_out &
FT_Properties_out::operator= (FT_Properties *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_Properties_out::operator ::FT_Properties *&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_Properties *&
FT_Properties_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_Properties *
FT_Properties_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE FT_Property &
FT_Properties_out::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[]	(index);
}


#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

// = Static operations.
ACE_INLINE FT::Location	*
_TAO_Unbounded_Sequence_FT_Locations::allocbuf (CORBA::ULong size)
  // Allocate storage for the sequence.
{
  FT::Location *retval = 0;
  ACE_NEW_RETURN (retval, FT::Location[size], 0);
  return retval;
}

ACE_INLINE void
_TAO_Unbounded_Sequence_FT_Locations::freebuf (FT::Location *buffer)
  // Free the sequence.
{
  delete [] buffer;
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Locations::_TAO_Unbounded_Sequence_FT_Locations (void) // Default constructor.
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Locations::_TAO_Unbounded_Sequence_FT_Locations (CORBA::ULong maximum) // Constructor using a maximum length	value.
  : TAO_Unbounded_Base_Sequence	(maximum, _TAO_Unbounded_Sequence_FT_Locations::allocbuf (maximum))
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Locations::_TAO_Unbounded_Sequence_FT_Locations (CORBA::ULong maximum,
    CORBA::ULong length,
    FT::Location *data,
    CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence	(maximum, length, data,	release)
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Locations::_TAO_Unbounded_Sequence_FT_Locations (const _TAO_Unbounded_Sequence_FT_Locations &rhs)
  // Copy constructor.
  : TAO_Unbounded_Base_Sequence	(rhs)
{
  if (rhs.buffer_ != 0)
    {
      FT::Location *tmp1 = _TAO_Unbounded_Sequence_FT_Locations::allocbuf (this->maximum_);
      FT::Location * const tmp2	= ACE_reinterpret_cast (FT::Location * ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong	i = 0; i < this->length_; ++i)
	tmp1[i]	= tmp2[i];

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

ACE_INLINE _TAO_Unbounded_Sequence_FT_Locations	&
_TAO_Unbounded_Sequence_FT_Locations::operator=	(const _TAO_Unbounded_Sequence_FT_Locations &rhs)
  // Assignment	operator.
{
  if (this == &rhs)
  return *this;

  if (this->release_)
  {
    if (this->maximum_ < rhs.maximum_)
    {
      // free the old buffer
      FT::Location *tmp	= ACE_reinterpret_cast (FT::Location *,	this->buffer_);
      _TAO_Unbounded_Sequence_FT_Locations::freebuf (tmp);
      this->buffer_ = _TAO_Unbounded_Sequence_FT_Locations::allocbuf (rhs.maximum_);
    }
  }
  else
  this->buffer_	= _TAO_Unbounded_Sequence_FT_Locations::allocbuf (rhs.maximum_);

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  FT::Location *tmp1 = ACE_reinterpret_cast (FT::Location *, this->buffer_);
  FT::Location * const tmp2 = ACE_reinterpret_cast (FT::Location * ACE_CAST_CONST, rhs.buffer_);

  for (CORBA::ULong i =	0; i < this->length_; ++i)
    tmp1[i] = tmp2[i];

  return *this;
}

// = Accessors.
ACE_INLINE FT::Location	&
_TAO_Unbounded_Sequence_FT_Locations::operator[] (CORBA::ULong i)
  // operator []
{
  ACE_ASSERT (i	< this->maximum_);
  FT::Location*	tmp = ACE_reinterpret_cast(FT::Location*,
					   this->buffer_);
  return tmp[i];
}

ACE_INLINE const FT::Location &
_TAO_Unbounded_Sequence_FT_Locations::operator[] (CORBA::ULong i) const
  // operator []
{
  ACE_ASSERT (i	< this->maximum_);
  FT::Location * const tmp = ACE_reinterpret_cast (FT::Location* ACE_CAST_CONST,
						   this->buffer_);
  return tmp[i];
}

// Implement the TAO_Base_Sequence methods (see	Sequence.h)

ACE_INLINE FT::Location	*
_TAO_Unbounded_Sequence_FT_Locations::get_buffer (CORBA::Boolean orphan)
{
  FT::Location *result = 0;
  if (orphan ==	0)
    {
      // We retain ownership.
      if (this->buffer_	== 0)
	{
	  result = _TAO_Unbounded_Sequence_FT_Locations::allocbuf (this->length_);
	  this->buffer_	= result;
	}
      else
	{
	  result = ACE_reinterpret_cast	(FT::Location*,	this->buffer_);
	}
    }
  else // if (orphan ==	1)
    {
      if (this->release_ != 0)
	{
	  // We	set the	state back to default and relinquish
	  // ownership.
	  result = ACE_reinterpret_cast(FT::Location*,this->buffer_);
	  this->maximum_ = 0;
	  this->length_	= 0;
	  this->buffer_	= 0;
	  this->release_ = 0;
	}
    }
  return result;
}

ACE_INLINE const FT::Location *
_TAO_Unbounded_Sequence_FT_Locations::get_buffer (void)	const
{
  return ACE_reinterpret_cast(const FT::Location * ACE_CAST_CONST, this->buffer_);
}


ACE_INLINE void
_TAO_Unbounded_Sequence_FT_Locations::replace (CORBA::ULong max,
  CORBA::ULong length,
  FT::Location *data,
  CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_	= length;
  if (this->buffer_ && this->release_ == 1)
    {
      FT::Location *tmp	= ACE_reinterpret_cast(FT::Location*,this->buffer_);
      _TAO_Unbounded_Sequence_FT_Locations::freebuf (tmp);
    }
  this->buffer_	= data;
  this->release_ = release;
}

#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

// *************************************************************
// Inline operations for class FT_Locations_var
// *************************************************************

ACE_INLINE
FT_Locations_var::FT_Locations_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
FT_Locations_var::FT_Locations_var (FT_Locations *p)
  : ptr_ (p)
{}

ACE_INLINE
FT_Locations_var::FT_Locations_var (const ::FT_Locations_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::FT_Locations	(*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
FT_Locations_var::~FT_Locations_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE FT_Locations_var &
FT_Locations_var::operator= (FT_Locations *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_Locations_var &
FT_Locations_var::operator= (const ::FT_Locations_var &p) // deep copy
{
  if (this != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,	::FT_Locations (*p.ptr_), *this);
  }
  return *this;
}

ACE_INLINE const ::FT_Locations	*
FT_Locations_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_Locations *
FT_Locations_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
FT_Locations_var::operator const ::FT_Locations	&() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_Locations_var::operator ::FT_Locations &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_Locations_var::operator ::FT_Locations &() const // cast
{
  return *this->ptr_;
}

// variable-size types only
ACE_INLINE
FT_Locations_var::operator ::FT_Locations *&() // cast
{
  return this->ptr_;
}

ACE_INLINE FT::Location	&
FT_Locations_var::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[]	(index);
}

ACE_INLINE const ::FT_Locations	&
FT_Locations_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE ::FT_Locations &
FT_Locations_var::inout	(void)
{
  return *this->ptr_;
}

// mapping for variable	size
ACE_INLINE ::FT_Locations *&
FT_Locations_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE ::FT_Locations *
FT_Locations_var::_retn	(void)
{
  ::FT_Locations *tmp =	this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE ::FT_Locations *
FT_Locations_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class FT_Locations_out
// *************************************************************

ACE_INLINE
FT_Locations_out::FT_Locations_out (FT_Locations *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
FT_Locations_out::FT_Locations_out (FT_Locations_var &p) // constructor	from _var
  : ptr_ (p.out	())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
FT_Locations_out::FT_Locations_out (const ::FT_Locations_out &p) // copy constructor
  : ptr_ (ACE_const_cast (FT_Locations_out&, p).ptr_)
{}

ACE_INLINE ::FT_Locations_out &
FT_Locations_out::operator= (const ::FT_Locations_out &p)
{
  this->ptr_ = ACE_const_cast (FT_Locations_out&, p).ptr_;
  return *this;
}

ACE_INLINE ::FT_Locations_out &
FT_Locations_out::operator= (FT_Locations *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_Locations_out::operator ::FT_Locations *&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_Locations *&
FT_Locations_out::ptr (void) //	ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_Locations *
FT_Locations_out::operator-> (void)
{
  return this->ptr_;
}

/*ACE_INLINE FT::Location &
FT_Locations_out::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[]	(index);
}
*/

// *************************************************************
// Inline operations for class FT_FactoryInfo_var
// *************************************************************

ACE_INLINE
FT_FactoryInfo_var::FT_FactoryInfo_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
FT_FactoryInfo_var::FT_FactoryInfo_var (FT_FactoryInfo *p)
  : ptr_ (p)
{}

ACE_INLINE
FT_FactoryInfo_var::FT_FactoryInfo_var (const ::FT_FactoryInfo_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::FT_FactoryInfo (*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
FT_FactoryInfo_var::~FT_FactoryInfo_var	(void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE FT_FactoryInfo_var &
FT_FactoryInfo_var::operator= (FT_FactoryInfo *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE ::FT_FactoryInfo_var	&
FT_FactoryInfo_var::operator= (const ::FT_FactoryInfo_var &p)
{
  if (this != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,	::FT_FactoryInfo (*p.ptr_), *this);
  }
  return *this;
}

ACE_INLINE const ::FT_FactoryInfo *
FT_FactoryInfo_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_FactoryInfo *
FT_FactoryInfo_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
FT_FactoryInfo_var::operator const ::FT_FactoryInfo &()	const // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_FactoryInfo_var::operator ::FT_FactoryInfo &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_FactoryInfo_var::operator ::FT_FactoryInfo &() const	// cast
{
  return *this->ptr_;
}

// variable-size types only
ACE_INLINE
FT_FactoryInfo_var::operator ::FT_FactoryInfo *&() // cast
{
  return this->ptr_;
}

ACE_INLINE const ::FT_FactoryInfo &
FT_FactoryInfo_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE ::FT_FactoryInfo &
FT_FactoryInfo_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable	size
ACE_INLINE ::FT_FactoryInfo *&
FT_FactoryInfo_var::out	(void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE ::FT_FactoryInfo *
FT_FactoryInfo_var::_retn (void)
{
  ::FT_FactoryInfo *tmp	= this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE ::FT_FactoryInfo *
FT_FactoryInfo_var::ptr	(void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class FT_FactoryInfo_out
// *************************************************************

ACE_INLINE
FT_FactoryInfo_out::FT_FactoryInfo_out (::FT_FactoryInfo *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
FT_FactoryInfo_out::FT_FactoryInfo_out (FT_FactoryInfo_var &p) // constructor from _var
  : ptr_ (p.out	())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
FT_FactoryInfo_out::FT_FactoryInfo_out (const ::FT_FactoryInfo_out &p) // copy constructor
  : ptr_ (ACE_const_cast (FT_FactoryInfo_out&, p).ptr_)
{}

ACE_INLINE FT_FactoryInfo_out &
FT_FactoryInfo_out::operator= (const ::FT_FactoryInfo_out &p)
{
  this->ptr_ = ACE_const_cast (FT_FactoryInfo_out&, p).ptr_;
  return *this;
}

ACE_INLINE FT_FactoryInfo_out &
FT_FactoryInfo_out::operator= (FT_FactoryInfo *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_FactoryInfo_out::operator ::FT_FactoryInfo *&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_FactoryInfo *&
FT_FactoryInfo_out::ptr	(void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_FactoryInfo *
FT_FactoryInfo_out::operator-> (void)
{
  return this->ptr_;
}


#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

// = Static operations.
ACE_INLINE FT_FactoryInfo *
_TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (CORBA::ULong	size)
  // Allocate storage for the sequence.
{
  FT_FactoryInfo *retval = 0;
  ACE_NEW_RETURN (retval, FT_FactoryInfo[size],	0);
  return retval;
}

ACE_INLINE void
_TAO_Unbounded_Sequence_FT_FactoryInfos::freebuf (FT_FactoryInfo *buffer)
  // Free the sequence.
{
  delete [] buffer;
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_FactoryInfos::_TAO_Unbounded_Sequence_FT_FactoryInfos (void)	// Default constructor.
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_FactoryInfos::_TAO_Unbounded_Sequence_FT_FactoryInfos (CORBA::ULong maximum)	// Constructor using a maximum length value.
  : TAO_Unbounded_Base_Sequence	(maximum, _TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (maximum))
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_FactoryInfos::_TAO_Unbounded_Sequence_FT_FactoryInfos (CORBA::ULong maximum,
  CORBA::ULong length,
  FT_FactoryInfo *data,
  CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence	(maximum, length, data,	release)
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_FactoryInfos::_TAO_Unbounded_Sequence_FT_FactoryInfos (const	_TAO_Unbounded_Sequence_FT_FactoryInfos	&rhs)
  // Copy constructor.
  : TAO_Unbounded_Base_Sequence	(rhs)
{
  if (rhs.buffer_ != 0)
    {
      FT_FactoryInfo *tmp1 = _TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (this->maximum_);
      FT_FactoryInfo * const tmp2 = ACE_reinterpret_cast (FT_FactoryInfo * ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong	i = 0; i < this->length_; ++i)
	tmp1[i]	= tmp2[i];

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

ACE_INLINE _TAO_Unbounded_Sequence_FT_FactoryInfos &
_TAO_Unbounded_Sequence_FT_FactoryInfos::operator= (const _TAO_Unbounded_Sequence_FT_FactoryInfos &rhs)
  // Assignment	operator.
{
  if (this == &rhs)
  return *this;

  if (this->release_)
  {
    if (this->maximum_ < rhs.maximum_)
    {
      // free the old buffer
      FT_FactoryInfo *tmp = ACE_reinterpret_cast (FT_FactoryInfo *, this->buffer_);
      _TAO_Unbounded_Sequence_FT_FactoryInfos::freebuf (tmp);
      this->buffer_ = _TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf	(rhs.maximum_);
    }
  }
  else
  this->buffer_	= _TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (rhs.maximum_);

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  FT_FactoryInfo *tmp1 = ACE_reinterpret_cast (FT_FactoryInfo *, this->buffer_);
  FT_FactoryInfo * const tmp2 =	ACE_reinterpret_cast (FT_FactoryInfo * ACE_CAST_CONST, rhs.buffer_);

  for (CORBA::ULong i =	0; i < this->length_; ++i)
  tmp1[i] = tmp2[i];

  return *this;
}

// = Accessors.
ACE_INLINE FT_FactoryInfo &
_TAO_Unbounded_Sequence_FT_FactoryInfos::operator[] (CORBA::ULong i)
  // operator []
{
  ACE_ASSERT (i	< this->maximum_);
  FT_FactoryInfo* tmp =	ACE_reinterpret_cast(FT_FactoryInfo*,this->buffer_);
  return tmp[i];
}

ACE_INLINE const FT_FactoryInfo	&
_TAO_Unbounded_Sequence_FT_FactoryInfos::operator[] (CORBA::ULong i) const
  // operator []
{
  ACE_ASSERT (i	< this->maximum_);
  FT_FactoryInfo * const tmp = ACE_reinterpret_cast (FT_FactoryInfo* ACE_CAST_CONST, this->buffer_);
  return tmp[i];
}

// Implement the TAO_Base_Sequence methods (see	Sequence.h)
ACE_INLINE FT_FactoryInfo *
_TAO_Unbounded_Sequence_FT_FactoryInfos::get_buffer (CORBA::Boolean orphan)
{
  FT_FactoryInfo *result = 0;
  if (orphan ==	0)
    {
      // We retain ownership.
      if (this->buffer_	== 0)
	{
	  result = _TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (this->length_);
	  this->buffer_	= result;
	}
      else
	{
	  result = ACE_reinterpret_cast	(FT_FactoryInfo*, this->buffer_);
	}
    }
  else // if (orphan ==	1)
    {
      if (this->release_ != 0)
	{
	  // We	set the	state back to default and relinquish
	  // ownership.
	  result = ACE_reinterpret_cast(FT_FactoryInfo*,this->buffer_);
	  this->maximum_ = 0;
	  this->length_	= 0;
	  this->buffer_	= 0;
	  this->release_ = 0;
	}
    }
  return result;
}

ACE_INLINE const FT_FactoryInfo	*
_TAO_Unbounded_Sequence_FT_FactoryInfos::get_buffer (void) const
{
  return ACE_reinterpret_cast(const FT_FactoryInfo * ACE_CAST_CONST, this->buffer_);
}

ACE_INLINE void
_TAO_Unbounded_Sequence_FT_FactoryInfos::replace (CORBA::ULong max,
  CORBA::ULong length,
  FT_FactoryInfo *data,
  CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_	= length;
  if (this->buffer_ && this->release_ == 1)
    {
      FT_FactoryInfo *tmp = ACE_reinterpret_cast(FT_FactoryInfo*,this->buffer_);
      _TAO_Unbounded_Sequence_FT_FactoryInfos::freebuf (tmp);
    }
  this->buffer_	= data;
  this->release_ = release;
}


#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

// *************************************************************
// Inline operations for class FT_FactoryInfos_var
// *************************************************************

ACE_INLINE
FT_FactoryInfos_var::FT_FactoryInfos_var (void)	// default constructor
  : ptr_ (0)
{}

ACE_INLINE
FT_FactoryInfos_var::FT_FactoryInfos_var (FT_FactoryInfos *p)
  : ptr_ (p)
{}

ACE_INLINE
FT_FactoryInfos_var::FT_FactoryInfos_var (const	::FT_FactoryInfos_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::FT_FactoryInfos (*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
FT_FactoryInfos_var::~FT_FactoryInfos_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE FT_FactoryInfos_var &
FT_FactoryInfos_var::operator= (FT_FactoryInfos	*p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_FactoryInfos_var &
FT_FactoryInfos_var::operator= (const ::FT_FactoryInfos_var &p)	// deep	copy
{
  if (this != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,	::FT_FactoryInfos (*p.ptr_), *this);
  }
  return *this;
}

ACE_INLINE const ::FT_FactoryInfos *
FT_FactoryInfos_var::operator->	(void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_FactoryInfos *
FT_FactoryInfos_var::operator->	(void)
{
  return this->ptr_;
}

ACE_INLINE
FT_FactoryInfos_var::operator const ::FT_FactoryInfos &() const	// cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_FactoryInfos_var::operator ::FT_FactoryInfos	&() // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_FactoryInfos_var::operator ::FT_FactoryInfos	&() const // cast
{
  return *this->ptr_;
}

// variable-size types only
ACE_INLINE
FT_FactoryInfos_var::operator ::FT_FactoryInfos	*&() //	cast
{
  return this->ptr_;
}

ACE_INLINE FT_FactoryInfo &
FT_FactoryInfos_var::operator[]	(CORBA::ULong index)
{
  return this->ptr_->operator[]	(index);
}

ACE_INLINE const ::FT_FactoryInfos &
FT_FactoryInfos_var::in	(void) const
{
  return *this->ptr_;
}

ACE_INLINE ::FT_FactoryInfos &
FT_FactoryInfos_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable	size
ACE_INLINE ::FT_FactoryInfos *&
FT_FactoryInfos_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE ::FT_FactoryInfos *
FT_FactoryInfos_var::_retn (void)
{
  ::FT_FactoryInfos *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE ::FT_FactoryInfos *
FT_FactoryInfos_var::ptr (void)	const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class FT_FactoryInfos_out
// *************************************************************

ACE_INLINE
FT_FactoryInfos_out::FT_FactoryInfos_out (FT_FactoryInfos *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
FT_FactoryInfos_out::FT_FactoryInfos_out (FT_FactoryInfos_var &p) // constructor from _var
  : ptr_ (p.out	())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
FT_FactoryInfos_out::FT_FactoryInfos_out (const	::FT_FactoryInfos_out &p) // copy constructor
  : ptr_ (ACE_const_cast (FT_FactoryInfos_out&,	p).ptr_)
{}

ACE_INLINE ::FT_FactoryInfos_out &
FT_FactoryInfos_out::operator= (const ::FT_FactoryInfos_out &p)
{
  this->ptr_ = ACE_const_cast (FT_FactoryInfos_out&, p).ptr_;
  return *this;
}

ACE_INLINE ::FT_FactoryInfos_out &
FT_FactoryInfos_out::operator= (FT_FactoryInfos	*p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_FactoryInfos_out::operator ::FT_FactoryInfos	*&() //	cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_FactoryInfos *&
FT_FactoryInfos_out::ptr (void)	// ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_FactoryInfos *
FT_FactoryInfos_out::operator->	(void)
{
  return this->ptr_;
}

ACE_INLINE FT_FactoryInfo &
FT_FactoryInfos_out::operator[]	(CORBA::ULong index)
{
  return this->ptr_->operator[]	(index);
}


// *************************************************************
// Inline operations for class FT_FaultMonitoringIntervalAndTimeoutValue_var
// *************************************************************

ACE_INLINE
FT_FaultMonitoringIntervalAndTimeoutValue_var::FT_FaultMonitoringIntervalAndTimeoutValue_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
FT_FaultMonitoringIntervalAndTimeoutValue_var::FT_FaultMonitoringIntervalAndTimeoutValue_var (FT_FaultMonitoringIntervalAndTimeoutValue	*p)
  : ptr_ (p)
{}

ACE_INLINE
FT_FaultMonitoringIntervalAndTimeoutValue_var::FT_FaultMonitoringIntervalAndTimeoutValue_var (const ::FT_FaultMonitoringIntervalAndTimeoutValue_var &p)	// copy	constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::FT_FaultMonitoringIntervalAndTimeoutValue (*p.ptr_));
  else
    this->ptr_ = 0;
}

// fixed-size types only
ACE_INLINE
FT_FaultMonitoringIntervalAndTimeoutValue_var::FT_FaultMonitoringIntervalAndTimeoutValue_var (const ::FT_FaultMonitoringIntervalAndTimeoutValue	&p)
{
  ACE_NEW (this->ptr_, ::FT_FaultMonitoringIntervalAndTimeoutValue (p));
}

ACE_INLINE
FT_FaultMonitoringIntervalAndTimeoutValue_var::~FT_FaultMonitoringIntervalAndTimeoutValue_var (void) //	destructor
{
  delete this->ptr_;
}

ACE_INLINE FT_FaultMonitoringIntervalAndTimeoutValue_var &
FT_FaultMonitoringIntervalAndTimeoutValue_var::operator= (FT_FaultMonitoringIntervalAndTimeoutValue *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue_var &
FT_FaultMonitoringIntervalAndTimeoutValue_var::operator= (const	::FT_FaultMonitoringIntervalAndTimeoutValue_var	&p)
{
  if (this != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,	::FT_FaultMonitoringIntervalAndTimeoutValue (*p.ptr_), *this);
  }
  return *this;
}

// fixed-size types only
ACE_INLINE FT_FaultMonitoringIntervalAndTimeoutValue_var &
FT_FaultMonitoringIntervalAndTimeoutValue_var::operator= (const	::FT_FaultMonitoringIntervalAndTimeoutValue &p)
{
  if (this->ptr_ != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,	::FT_FaultMonitoringIntervalAndTimeoutValue (p), *this);
  }
  return *this;
}

ACE_INLINE const ::FT_FaultMonitoringIntervalAndTimeoutValue *
FT_FaultMonitoringIntervalAndTimeoutValue_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue *
FT_FaultMonitoringIntervalAndTimeoutValue_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
FT_FaultMonitoringIntervalAndTimeoutValue_var::operator	const ::FT_FaultMonitoringIntervalAndTimeoutValue &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_FaultMonitoringIntervalAndTimeoutValue_var::operator	::FT_FaultMonitoringIntervalAndTimeoutValue &()	// cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_FaultMonitoringIntervalAndTimeoutValue_var::operator	::FT_FaultMonitoringIntervalAndTimeoutValue &()	const // cast
{
  return *this->ptr_;
}

ACE_INLINE const ::FT_FaultMonitoringIntervalAndTimeoutValue &
FT_FaultMonitoringIntervalAndTimeoutValue_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue &
FT_FaultMonitoringIntervalAndTimeoutValue_var::inout (void)
{
  return *this->ptr_;
}

// mapping for fixed size
ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue &
FT_FaultMonitoringIntervalAndTimeoutValue_var::out (void)
{
  return *this->ptr_;
}

ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue
FT_FaultMonitoringIntervalAndTimeoutValue_var::_retn (void)
{
  return *this->ptr_;
}

ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue *
FT_FaultMonitoringIntervalAndTimeoutValue_var::ptr (void) const
{
  return this->ptr_;
}


ACE_INLINE
FT_PropertyManager::FT_PropertyManager (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean	_tao_collocated) // constructor
  : CORBA_Object (objref, _tao_servant,	_tao_collocated)
{}


// *************************************************************
// Inline operations for class FT_PropertyManager_var
// *************************************************************

ACE_INLINE
FT_PropertyManager_var::FT_PropertyManager_var (void) // default constructor
  : ptr_ (FT_PropertyManager::_nil ())
{}

ACE_INLINE ::FT_PropertyManager_ptr
FT_PropertyManager_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
FT_PropertyManager_var::FT_PropertyManager_var (const ::FT_PropertyManager_var &p) // copy constructor
  : TAO_Base_var (),
    ptr_ (FT_PropertyManager::_duplicate (p.ptr	()))
{}

ACE_INLINE
FT_PropertyManager_var::~FT_PropertyManager_var	(void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE FT_PropertyManager_var &
FT_PropertyManager_var::operator= (FT_PropertyManager_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_PropertyManager_var &
FT_PropertyManager_var::operator= (const ::FT_PropertyManager_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::FT_PropertyManager::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
FT_PropertyManager_var::operator const ::FT_PropertyManager_ptr	&() const // cast
{
  return this->ptr_;
}

ACE_INLINE
FT_PropertyManager_var::operator ::FT_PropertyManager_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_PropertyManager_ptr
FT_PropertyManager_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_PropertyManager_ptr
FT_PropertyManager_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_PropertyManager_ptr &
FT_PropertyManager_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE ::FT_PropertyManager_ptr &
FT_PropertyManager_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_PropertyManager::_nil ();
  return this->ptr_;
}

ACE_INLINE ::FT_PropertyManager_ptr
FT_PropertyManager_var::_retn (void)
{
  // yield ownership of	managed	obj reference
  ::FT_PropertyManager_ptr val = this->ptr_;
  this->ptr_ = ::FT_PropertyManager::_nil ();
  return val;
}


// *************************************************************
// Inline operations for class FT_PropertyManager_out
// *************************************************************

ACE_INLINE
FT_PropertyManager_out::FT_PropertyManager_out (FT_PropertyManager_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = ::FT_PropertyManager::_nil ();
}

ACE_INLINE
FT_PropertyManager_out::FT_PropertyManager_out (FT_PropertyManager_var &p) // constructor from _var
  : ptr_ (p.out	())
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_PropertyManager::_nil ();
}

ACE_INLINE
FT_PropertyManager_out::FT_PropertyManager_out (const ::FT_PropertyManager_out &p) // copy constructor
  : ptr_ (ACE_const_cast (FT_PropertyManager_out &, p).ptr_)
{}

ACE_INLINE ::FT_PropertyManager_out &
FT_PropertyManager_out::operator= (const ::FT_PropertyManager_out &p)
{
  this->ptr_ = ACE_const_cast (FT_PropertyManager_out&,	p).ptr_;
  return *this;
}

ACE_INLINE FT_PropertyManager_out &
FT_PropertyManager_out::operator= (const ::FT_PropertyManager_var &p)
{
  this->ptr_ = ::FT_PropertyManager::_duplicate	(p.ptr ());
  return *this;
}

ACE_INLINE FT_PropertyManager_out &
FT_PropertyManager_out::operator= (FT_PropertyManager_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_PropertyManager_out::operator ::FT_PropertyManager_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_PropertyManager_ptr &
FT_PropertyManager_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_PropertyManager_ptr
FT_PropertyManager_out::operator-> (void)
{
  return this->ptr_;
}


ACE_INLINE
FT_ObjectGroupManager::FT_ObjectGroupManager (TAO_Stub *objref,
					      TAO_ServantBase *_tao_servant,
					      CORBA::Boolean _tao_collocated) // constructor
  : CORBA_Object (objref, _tao_servant,	_tao_collocated)
{}


// *************************************************************
// Inline operations for class FT_ObjectGroupManager_var
// *************************************************************

ACE_INLINE
FT_ObjectGroupManager_var::FT_ObjectGroupManager_var (void) // default constructor
  : ptr_ (FT_ObjectGroupManager::_nil ())
{}

ACE_INLINE ::FT_ObjectGroupManager_ptr
FT_ObjectGroupManager_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
FT_ObjectGroupManager_var::FT_ObjectGroupManager_var (const ::FT_ObjectGroupManager_var	&p) // copy constructor
  : TAO_Base_var (),
    ptr_ (FT_ObjectGroupManager::_duplicate (p.ptr ()))
{}

ACE_INLINE
FT_ObjectGroupManager_var::~FT_ObjectGroupManager_var (void) //	destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE FT_ObjectGroupManager_var &
FT_ObjectGroupManager_var::operator= (FT_ObjectGroupManager_ptr	p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_ObjectGroupManager_var &
FT_ObjectGroupManager_var::operator= (const ::FT_ObjectGroupManager_var	&p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::FT_ObjectGroupManager::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
FT_ObjectGroupManager_var::operator const ::FT_ObjectGroupManager_ptr &() const	// cast
{
  return this->ptr_;
}

ACE_INLINE
FT_ObjectGroupManager_var::operator ::FT_ObjectGroupManager_ptr	&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_ObjectGroupManager_ptr
FT_ObjectGroupManager_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_ObjectGroupManager_ptr
FT_ObjectGroupManager_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_ObjectGroupManager_ptr &
FT_ObjectGroupManager_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE ::FT_ObjectGroupManager_ptr &
FT_ObjectGroupManager_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_ObjectGroupManager::_nil ();
  return this->ptr_;
}

ACE_INLINE ::FT_ObjectGroupManager_ptr
FT_ObjectGroupManager_var::_retn (void)
{
  // yield ownership of	managed	obj reference
  ::FT_ObjectGroupManager_ptr val = this->ptr_;
  this->ptr_ = ::FT_ObjectGroupManager::_nil ();
  return val;
}


// *************************************************************
// Inline operations for class FT_ObjectGroupManager_out
// *************************************************************

ACE_INLINE
FT_ObjectGroupManager_out::FT_ObjectGroupManager_out (FT_ObjectGroupManager_ptr	&p)
  : ptr_ (p)
{
  this->ptr_ = ::FT_ObjectGroupManager::_nil ();
}

ACE_INLINE
FT_ObjectGroupManager_out::FT_ObjectGroupManager_out (FT_ObjectGroupManager_var	&p) // constructor from	_var
  : ptr_ (p.out	())
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_ObjectGroupManager::_nil ();
}

ACE_INLINE
FT_ObjectGroupManager_out::FT_ObjectGroupManager_out (const ::FT_ObjectGroupManager_out	&p) // copy constructor
  : ptr_ (ACE_const_cast (FT_ObjectGroupManager_out &, p).ptr_)
{}

ACE_INLINE ::FT_ObjectGroupManager_out &
FT_ObjectGroupManager_out::operator= (const ::FT_ObjectGroupManager_out	&p)
{
  this->ptr_ = ACE_const_cast (FT_ObjectGroupManager_out&, p).ptr_;
  return *this;
}

ACE_INLINE FT_ObjectGroupManager_out &
FT_ObjectGroupManager_out::operator= (const ::FT_ObjectGroupManager_var	&p)
{
  this->ptr_ = ::FT_ObjectGroupManager::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE FT_ObjectGroupManager_out &
FT_ObjectGroupManager_out::operator= (FT_ObjectGroupManager_ptr	p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_ObjectGroupManager_out::operator ::FT_ObjectGroupManager_ptr	&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_ObjectGroupManager_ptr &
FT_ObjectGroupManager_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_ObjectGroupManager_ptr
FT_ObjectGroupManager_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
FT_ReplicationManager::FT_ReplicationManager (TAO_Stub *objref,	TAO_ServantBase	*_tao_servant, CORBA::Boolean _tao_collocated) // constructor
  : CORBA_Object (objref, _tao_servant,	_tao_collocated)
{}


// *************************************************************
// Inline operations for class FT_ReplicationManager_var
// *************************************************************

ACE_INLINE
FT_ReplicationManager_var::FT_ReplicationManager_var (void) // default constructor
  : ptr_ (FT_ReplicationManager::_nil ())
{}

ACE_INLINE ::FT_ReplicationManager_ptr
FT_ReplicationManager_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
FT_ReplicationManager_var::FT_ReplicationManager_var (const ::FT_ReplicationManager_var	&p) // copy constructor
  : TAO_Base_var (),
    ptr_ (FT_ReplicationManager::_duplicate (p.ptr ()))
{}

ACE_INLINE
FT_ReplicationManager_var::~FT_ReplicationManager_var (void) //	destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE FT_ReplicationManager_var &
FT_ReplicationManager_var::operator= (FT_ReplicationManager_ptr	p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_ReplicationManager_var &
FT_ReplicationManager_var::operator= (const ::FT_ReplicationManager_var	&p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::FT_ReplicationManager::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
FT_ReplicationManager_var::operator const ::FT_ReplicationManager_ptr &() const	// cast
{
  return this->ptr_;
}

ACE_INLINE
FT_ReplicationManager_var::operator ::FT_ReplicationManager_ptr	&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_ReplicationManager_ptr
FT_ReplicationManager_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_ReplicationManager_ptr
FT_ReplicationManager_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_ReplicationManager_ptr &
FT_ReplicationManager_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE ::FT_ReplicationManager_ptr &
FT_ReplicationManager_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_ReplicationManager::_nil ();
  return this->ptr_;
}

ACE_INLINE ::FT_ReplicationManager_ptr
FT_ReplicationManager_var::_retn (void)
{
  // yield ownership of	managed	obj reference
  ::FT_ReplicationManager_ptr val = this->ptr_;
  this->ptr_ = ::FT_ReplicationManager::_nil ();
  return val;
}


// *************************************************************
// Inline operations for class FT_ReplicationManager_out
// *************************************************************

ACE_INLINE
FT_ReplicationManager_out::FT_ReplicationManager_out (FT_ReplicationManager_ptr	&p)
  : ptr_ (p)
{
  this->ptr_ = ::FT_ReplicationManager::_nil ();
}

ACE_INLINE
FT_ReplicationManager_out::FT_ReplicationManager_out (FT_ReplicationManager_var	&p) // constructor from	_var
  : ptr_ (p.out	())
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_ReplicationManager::_nil ();
}

ACE_INLINE
FT_ReplicationManager_out::FT_ReplicationManager_out (const ::FT_ReplicationManager_out	&p) // copy constructor
  : ptr_ (ACE_const_cast (FT_ReplicationManager_out &, p).ptr_)
{}

ACE_INLINE ::FT_ReplicationManager_out &
FT_ReplicationManager_out::operator= (const ::FT_ReplicationManager_out	&p)
{
  this->ptr_ = ACE_const_cast (FT_ReplicationManager_out&, p).ptr_;
  return *this;
}

ACE_INLINE FT_ReplicationManager_out &
FT_ReplicationManager_out::operator= (const ::FT_ReplicationManager_var	&p)
{
  this->ptr_ = ::FT_ReplicationManager::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE FT_ReplicationManager_out &
FT_ReplicationManager_out::operator= (FT_ReplicationManager_ptr	p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_ReplicationManager_out::operator ::FT_ReplicationManager_ptr	&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_ReplicationManager_ptr &
FT_ReplicationManager_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_ReplicationManager_ptr
FT_ReplicationManager_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
FT_PullMonitorable::FT_PullMonitorable (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean	_tao_collocated) // constructor
  : CORBA_Object (objref, _tao_servant,	_tao_collocated)
{}


// *************************************************************
// Inline operations for class FT_PullMonitorable_var
// *************************************************************

ACE_INLINE
FT_PullMonitorable_var::FT_PullMonitorable_var (void) // default constructor
  : ptr_ (FT_PullMonitorable::_nil ())
{}

ACE_INLINE ::FT_PullMonitorable_ptr
FT_PullMonitorable_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
FT_PullMonitorable_var::FT_PullMonitorable_var (const ::FT_PullMonitorable_var &p) // copy constructor
  : TAO_Base_var (),
    ptr_ (FT_PullMonitorable::_duplicate (p.ptr	()))
{}

ACE_INLINE
FT_PullMonitorable_var::~FT_PullMonitorable_var	(void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE FT_PullMonitorable_var &
FT_PullMonitorable_var::operator= (FT_PullMonitorable_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_PullMonitorable_var &
FT_PullMonitorable_var::operator= (const ::FT_PullMonitorable_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::FT_PullMonitorable::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
FT_PullMonitorable_var::operator const ::FT_PullMonitorable_ptr	&() const // cast
{
  return this->ptr_;
}

ACE_INLINE
FT_PullMonitorable_var::operator ::FT_PullMonitorable_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_PullMonitorable_ptr
FT_PullMonitorable_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_PullMonitorable_ptr
FT_PullMonitorable_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_PullMonitorable_ptr &
FT_PullMonitorable_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE ::FT_PullMonitorable_ptr &
FT_PullMonitorable_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_PullMonitorable::_nil ();
  return this->ptr_;
}

ACE_INLINE ::FT_PullMonitorable_ptr
FT_PullMonitorable_var::_retn (void)
{
  // yield ownership of	managed	obj reference
  ::FT_PullMonitorable_ptr val = this->ptr_;
  this->ptr_ = ::FT_PullMonitorable::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class FT_PullMonitorable_out
// *************************************************************

ACE_INLINE
FT_PullMonitorable_out::FT_PullMonitorable_out (FT_PullMonitorable_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = ::FT_PullMonitorable::_nil ();
}

ACE_INLINE
FT_PullMonitorable_out::FT_PullMonitorable_out (FT_PullMonitorable_var &p) // constructor from _var
  : ptr_ (p.out	())
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_PullMonitorable::_nil ();
}

ACE_INLINE
FT_PullMonitorable_out::FT_PullMonitorable_out (const ::FT_PullMonitorable_out &p) // copy constructor
  : ptr_ (ACE_const_cast (FT_PullMonitorable_out &, p).ptr_)
{}

ACE_INLINE ::FT_PullMonitorable_out &
FT_PullMonitorable_out::operator= (const ::FT_PullMonitorable_out &p)
{
  this->ptr_ = ACE_const_cast (FT_PullMonitorable_out&,	p).ptr_;
  return *this;
}

ACE_INLINE FT_PullMonitorable_out &
FT_PullMonitorable_out::operator= (const ::FT_PullMonitorable_var &p)
{
  this->ptr_ = ::FT_PullMonitorable::_duplicate	(p.ptr ());
  return *this;
}

ACE_INLINE FT_PullMonitorable_out &
FT_PullMonitorable_out::operator= (FT_PullMonitorable_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_PullMonitorable_out::operator ::FT_PullMonitorable_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_PullMonitorable_ptr &
FT_PullMonitorable_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_PullMonitorable_ptr
FT_PullMonitorable_out::operator-> (void)
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class FT_State_var
// *************************************************************

ACE_INLINE
FT_State_var::FT_State_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
FT_State_var::FT_State_var (FT_State *p)
  : ptr_ (p)
{}

ACE_INLINE
FT_State_var::FT_State_var (const ::FT_State_var &p) //	copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::FT_State (*p.ptr_));
  else
    this->ptr_ = 0;
}

// fixed-size base types only
ACE_INLINE
FT_State_var::FT_State_var (const ::FT_State &p)
{
  ACE_NEW (this->ptr_, ::FT_State (p));
}

ACE_INLINE
FT_State_var::~FT_State_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE FT_State_var	&
FT_State_var::operator=	(FT_State *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_State_var	&
FT_State_var::operator=	(const ::FT_State_var &p) // deep copy
{
  if (this != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,	::FT_State (*p.ptr_), *this);
  }
  return *this;
}

// fixed-size types only
ACE_INLINE ::FT_State_var &
FT_State_var::operator=	(const ::FT_State &p)
{
  if (this->ptr_ != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,	::FT_State (p),	*this);
  }
  return *this;
}

ACE_INLINE const ::FT_State *
FT_State_var::operator-> (void)	const
{
  return this->ptr_;
}

ACE_INLINE ::FT_State *
FT_State_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
FT_State_var::operator const ::FT_State	&() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_State_var::operator ::FT_State &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
FT_State_var::operator ::FT_State &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE CORBA::Octet	&
FT_State_var::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[]	(index);
}

ACE_INLINE const ::FT_State &
FT_State_var::in (void)	const
{
  return *this->ptr_;
}

ACE_INLINE ::FT_State &
FT_State_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable	size
ACE_INLINE ::FT_State *&
FT_State_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE ::FT_State *
FT_State_var::_retn (void)
{
  ::FT_State *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE ::FT_State *
FT_State_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class FT_State_out
// *************************************************************

ACE_INLINE
FT_State_out::FT_State_out (FT_State *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
FT_State_out::FT_State_out (FT_State_var &p) //	constructor from _var
  : ptr_ (p.out	())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
FT_State_out::FT_State_out (const ::FT_State_out &p) //	copy constructor
  : ptr_ (ACE_const_cast (FT_State_out&, p).ptr_)
{}

ACE_INLINE ::FT_State_out &
FT_State_out::operator=	(const ::FT_State_out &p)
{
  this->ptr_ = ACE_const_cast (FT_State_out&, p).ptr_;
  return *this;
}

ACE_INLINE ::FT_State_out &
FT_State_out::operator=	(FT_State *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_State_out::operator ::FT_State *&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_State *&
FT_State_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_State *
FT_State_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::Octet	&
FT_State_out::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[]	(index);
}

ACE_INLINE
FT_Checkpointable::FT_Checkpointable (TAO_Stub *objref,	TAO_ServantBase	*_tao_servant, CORBA::Boolean _tao_collocated) // constructor
  : CORBA_Object (objref, _tao_servant,	_tao_collocated)
{}


// *************************************************************
// Inline operations for class FT_Checkpointable_var
// *************************************************************

ACE_INLINE
FT_Checkpointable_var::FT_Checkpointable_var (void) // default constructor
  : ptr_ (FT_Checkpointable::_nil ())
{}

ACE_INLINE ::FT_Checkpointable_ptr
FT_Checkpointable_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
FT_Checkpointable_var::FT_Checkpointable_var (const ::FT_Checkpointable_var &p)	// copy	constructor
  : TAO_Base_var (),
    ptr_ (FT_Checkpointable::_duplicate	(p.ptr ()))
{}

ACE_INLINE
FT_Checkpointable_var::~FT_Checkpointable_var (void) //	destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE FT_Checkpointable_var &
FT_Checkpointable_var::operator= (FT_Checkpointable_ptr	p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_Checkpointable_var &
FT_Checkpointable_var::operator= (const	::FT_Checkpointable_var	&p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::FT_Checkpointable::_duplicate (p.ptr	());
  }
  return *this;
}

ACE_INLINE
FT_Checkpointable_var::operator	const ::FT_Checkpointable_ptr &() const	// cast
{
  return this->ptr_;
}

ACE_INLINE
FT_Checkpointable_var::operator	::FT_Checkpointable_ptr	&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_Checkpointable_ptr
FT_Checkpointable_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_Checkpointable_ptr
FT_Checkpointable_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_Checkpointable_ptr &
FT_Checkpointable_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE ::FT_Checkpointable_ptr &
FT_Checkpointable_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_Checkpointable::_nil ();
  return this->ptr_;
}

ACE_INLINE ::FT_Checkpointable_ptr
FT_Checkpointable_var::_retn (void)
{
  // yield ownership of	managed	obj reference
  ::FT_Checkpointable_ptr val =	this->ptr_;
  this->ptr_ = ::FT_Checkpointable::_nil ();
  return val;
}



// *************************************************************
// Inline operations for class FT_Checkpointable_out
// *************************************************************

ACE_INLINE
FT_Checkpointable_out::FT_Checkpointable_out (FT_Checkpointable_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = ::FT_Checkpointable::_nil ();
}

ACE_INLINE
FT_Checkpointable_out::FT_Checkpointable_out (FT_Checkpointable_var &p)	// constructor from _var
  : ptr_ (p.out	())
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_Checkpointable::_nil ();
}

ACE_INLINE
FT_Checkpointable_out::FT_Checkpointable_out (const ::FT_Checkpointable_out &p)	// copy	constructor
  : ptr_ (ACE_const_cast (FT_Checkpointable_out	&, p).ptr_)
{}

ACE_INLINE ::FT_Checkpointable_out &
FT_Checkpointable_out::operator= (const	::FT_Checkpointable_out	&p)
{
  this->ptr_ = ACE_const_cast (FT_Checkpointable_out&, p).ptr_;
  return *this;
}

ACE_INLINE FT_Checkpointable_out &
FT_Checkpointable_out::operator= (const	::FT_Checkpointable_var	&p)
{
  this->ptr_ = ::FT_Checkpointable::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE FT_Checkpointable_out &
FT_Checkpointable_out::operator= (FT_Checkpointable_ptr	p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_Checkpointable_out::operator	::FT_Checkpointable_ptr	&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_Checkpointable_ptr &
FT_Checkpointable_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_Checkpointable_ptr
FT_Checkpointable_out::operator-> (void)
{
  return this->ptr_;
}


ACE_INLINE
FT_Updateable::FT_Updateable (TAO_Stub *objref,	TAO_ServantBase	*_tao_servant, CORBA::Boolean _tao_collocated) // constructor
  : CORBA_Object (objref, _tao_servant,	_tao_collocated)
{}


// *************************************************************
// Inline operations for class FT_Updateable_var
// *************************************************************

ACE_INLINE
FT_Updateable_var::FT_Updateable_var (void) // default constructor
  : ptr_ (FT_Updateable::_nil ())
{}

ACE_INLINE ::FT_Updateable_ptr
FT_Updateable_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
FT_Updateable_var::FT_Updateable_var (const ::FT_Updateable_var	&p) // copy constructor
  : TAO_Base_var (),
    ptr_ (FT_Updateable::_duplicate (p.ptr ()))
{}

ACE_INLINE
FT_Updateable_var::~FT_Updateable_var (void) //	destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE FT_Updateable_var &
FT_Updateable_var::operator= (FT_Updateable_ptr	p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE FT_Updateable_var &
FT_Updateable_var::operator= (const ::FT_Updateable_var	&p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::FT_Updateable::_duplicate (p.ptr ());
  }
  return *this;
}

ACE_INLINE
FT_Updateable_var::operator const ::FT_Updateable_ptr &() const	// cast
{
  return this->ptr_;
}

ACE_INLINE
FT_Updateable_var::operator ::FT_Updateable_ptr	&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_Updateable_ptr
FT_Updateable_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_Updateable_ptr
FT_Updateable_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE ::FT_Updateable_ptr &
FT_Updateable_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE ::FT_Updateable_ptr &
FT_Updateable_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_Updateable::_nil ();
  return this->ptr_;
}

ACE_INLINE ::FT_Updateable_ptr
FT_Updateable_var::_retn (void)
{
  // yield ownership of	managed	obj reference
  ::FT_Updateable_ptr val = this->ptr_;
  this->ptr_ = ::FT_Updateable::_nil ();
  return val;
}



// *************************************************************
// Inline operations for class FT_Updateable_out
// *************************************************************

ACE_INLINE
FT_Updateable_out::FT_Updateable_out (FT_Updateable_ptr	&p)
  : ptr_ (p)
{
  this->ptr_ = ::FT_Updateable::_nil ();
}

ACE_INLINE
FT_Updateable_out::FT_Updateable_out (FT_Updateable_var	&p) // constructor from	_var
  : ptr_ (p.out	())
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::FT_Updateable::_nil ();
}

ACE_INLINE
FT_Updateable_out::FT_Updateable_out (const ::FT_Updateable_out	&p) // copy constructor
  : ptr_ (ACE_const_cast (FT_Updateable_out &, p).ptr_)
{}

ACE_INLINE ::FT_Updateable_out &
FT_Updateable_out::operator= (const ::FT_Updateable_out	&p)
{
  this->ptr_ = ACE_const_cast (FT_Updateable_out&, p).ptr_;
  return *this;
}

ACE_INLINE FT_Updateable_out &
FT_Updateable_out::operator= (const ::FT_Updateable_var	&p)
{
  this->ptr_ = ::FT_Updateable::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE FT_Updateable_out &
FT_Updateable_out::operator= (FT_Updateable_ptr	p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
FT_Updateable_out::operator ::FT_Updateable_ptr	&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::FT_Updateable_ptr &
FT_Updateable_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::FT_Updateable_ptr
FT_Updateable_out::operator-> (void)
{
  return this->ptr_;
}


ACE_INLINE
CORBA::Boolean operator<< (TAO_OutputCDR &strm,	const FT_Property &_tao_aggregate)
{
  if (
      (strm << _tao_aggregate.nam) &&
      (strm << _tao_aggregate.val))
    return 1;
  else
    return 0;

}

ACE_INLINE
CORBA::Boolean operator>> (TAO_InputCDR	&strm, FT_Property &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.nam) &&
    (strm >> _tao_aggregate.val)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE
CORBA::Boolean operator<< (TAO_OutputCDR &strm,
			   const FT_GenericFactory_ptr _tao_objref)
{
  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
  return (strm << _tao_corba_obj);
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,
				      FT_GenericFactory_ptr &_tao_objref)
{
  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj;
    if ((strm >> obj.inout ()) == 0)
      return 0;
    // narrow to the right type
    _tao_objref	=
      FT_GenericFactory::_unchecked_narrow (
	  obj.in (),
	  ACE_TRY_ENV
	);
    ACE_TRY_CHECK;
    return 1;
  }
  ACE_CATCHANY
  {
    // do nothing
  }
  ACE_ENDTRY;
  return 0;
}


ACE_INLINE
CORBA::Boolean operator<< (TAO_OutputCDR &strm,	const FT_FactoryInfo &_tao_aggregate)
{
  if ((strm << _tao_aggregate.factory.in ()) &&
      (strm << _tao_aggregate.the_location) &&
      (strm << _tao_aggregate.the_criteria))
    return 1;
  else
    return 0;
}

ACE_INLINE
CORBA::Boolean operator>> (TAO_InputCDR	&strm, FT_FactoryInfo &_tao_aggregate)
{
  if ((strm >> _tao_aggregate.factory.out ()) &&
      (strm >> _tao_aggregate.the_location) &&
      (strm >> _tao_aggregate.the_criteria))
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm,
				      const FT_FaultMonitoringIntervalAndTimeoutValue &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.monitoring_interval) &&
    (strm << _tao_aggregate.timeout)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,
				      FT_FaultMonitoringIntervalAndTimeoutValue	&_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.monitoring_interval) &&
    (strm >> _tao_aggregate.timeout)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm,
				      const FT_InterfaceNotFound &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_InterfaceNotFound&)
{
  return 1;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_InterfaceNotFound &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_ObjectGroupNotFound	&_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}


ACE_INLINE
ostream	& operator<< (ostream &os, const FT_MemberNotFound &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}


ACE_INLINE
ostream	& operator<< (ostream &os, const FT_ObjectNotFound &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_ObjectGroupNotFound	&_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_ObjectGroupNotFound&)
{
  return 1;
}


ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_MemberNotFound &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_MemberNotFound&)
{
  return 1;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm,
				      const FT_ObjectNotFound &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,
				      FT_ObjectNotFound&)
{
  return 1;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_MemberAlreadyPresent &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_MemberAlreadyPresent&)
{
  return 1;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_MemberAlreadyPresent &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_BadReplicationStyle	&_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_BadReplicationStyle&)
{
  return 1;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_BadReplicationStyle	&_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_ObjectNotCreated &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_ObjectNotCreated&)
{
  return 1;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_ObjectNotCreated &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_ObjectNotAdded &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_ObjectNotAdded&)
{
  return 1;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_ObjectNotAdded &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_PrimaryNotSet &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_PrimaryNotSet&)
{
  return 1;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_PrimaryNotSet &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_UnsupportedProperty	&_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
  {
    // now marshal the members (if any)
    if (
      (strm << _tao_aggregate.nam) &&
      (strm << _tao_aggregate.val)
    )
      return 1;
    else
      return 0;
  }
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,FT_UnsupportedProperty	&_tao_aggregate)
{
  // now marshal the members
  if (
    (strm >> _tao_aggregate.nam) &&
    (strm >> _tao_aggregate.val)
  )
    return 1;
  else
    return 0;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_UnsupportedProperty	&_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_InvalidProperty &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
  {
    // now marshal the members (if any)
    if (
      (strm << _tao_aggregate.nam) &&
      (strm << _tao_aggregate.val)
    )
      return 1;
    else
      return 0;
  }
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,FT_InvalidProperty &_tao_aggregate)
{
  // now marshal the members
  if (
    (strm >> _tao_aggregate.nam) &&
    (strm >> _tao_aggregate.val)
  )
    return 1;
  else
    return 0;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_InvalidProperty &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_NoFactory &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
  {
    // now marshal the members (if any)
    if (
      (strm << _tao_aggregate.the_location) &&
      (strm << _tao_aggregate.type_id.in ())
    )
      return 1;
    else
      return 0;
  }
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,FT_NoFactory &_tao_aggregate)
{
  // now marshal the members
  if (
    (strm >> _tao_aggregate.the_location) &&
    (strm >> _tao_aggregate.type_id.out	())
  )
    return 1;
  else
    return 0;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_NoFactory &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_InvalidCriteria &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
  {
    // now marshal the members (if any)
    if (
      (strm << _tao_aggregate.invalid_criteria)
    )
      return 1;
    else
      return 0;
  }
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,FT_InvalidCriteria &_tao_aggregate)
{
  // now marshal the members
  if (
    (strm >> _tao_aggregate.invalid_criteria)
  )
    return 1;
  else
    return 0;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_InvalidCriteria &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_CannotMeetCriteria &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
  {
    // now marshal the members (if any)
    if (
      (strm << _tao_aggregate.unmet_criteria)
    )
      return 1;
    else
      return 0;
  }
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,FT_CannotMeetCriteria &_tao_aggregate)
{
  // now marshal the members
  if (
    (strm >> _tao_aggregate.unmet_criteria)
  )
    return 1;
  else
    return 0;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_CannotMeetCriteria &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean
operator<< (
    TAO_OutputCDR &strm,
    const FT_PropertyManager_ptr _tao_objref
  )
{
  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
  return (strm << _tao_corba_obj);
}

ACE_INLINE CORBA::Boolean
operator>> (
    TAO_InputCDR &strm,
    FT_PropertyManager_ptr &_tao_objref
  )
{
  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj;
    if ((strm >> obj.inout ()) == 0)
      return 0;
    // narrow to the right type
    _tao_objref	=
      FT_PropertyManager::_unchecked_narrow (
	  obj.in (),
	  ACE_TRY_ENV
	);
    ACE_TRY_CHECK;
    return 1;
  }
  ACE_CATCHANY
  {
    // do nothing
  }
  ACE_ENDTRY;
  return 0;
}


ACE_INLINE CORBA::Boolean
operator<< (
    TAO_OutputCDR &strm,
    const FT_ObjectGroupManager_ptr _tao_objref
  )
{
  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
  return (strm << _tao_corba_obj);
}

ACE_INLINE CORBA::Boolean
operator>> (
    TAO_InputCDR &strm,
    FT_ObjectGroupManager_ptr &_tao_objref
  )
{
  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj;
    if ((strm >> obj.inout ()) == 0)
      return 0;
    // narrow to the right type
    _tao_objref	=
      FT_ObjectGroupManager::_unchecked_narrow (
	  obj.in (),
	  ACE_TRY_ENV
	);
    ACE_TRY_CHECK;
    return 1;
  }
  ACE_CATCHANY
  {
    // do nothing
  }
  ACE_ENDTRY;
  return 0;
}



ACE_INLINE CORBA::Boolean
operator<< (
    TAO_OutputCDR &strm,
    const FT_ReplicationManager_ptr _tao_objref
  )
{
  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
  return (strm << _tao_corba_obj);
}

ACE_INLINE CORBA::Boolean
operator>> (
    TAO_InputCDR &strm,
    FT_ReplicationManager_ptr &_tao_objref
  )
{
  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj;
    if ((strm >> obj.inout ()) == 0)
      return 0;
    // narrow to the right type
    _tao_objref	=
      FT_ReplicationManager::_unchecked_narrow (
	  obj.in (),
	  ACE_TRY_ENV
	);
    ACE_TRY_CHECK;
    return 1;
  }
  ACE_CATCHANY
  {
    // do nothing
  }
  ACE_ENDTRY;
  return 0;
}


ACE_INLINE CORBA::Boolean
operator<< (
    TAO_OutputCDR &strm,
    const FT_PullMonitorable_ptr _tao_objref
  )
{
  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
  return (strm << _tao_corba_obj);
}

ACE_INLINE CORBA::Boolean
operator>> (
    TAO_InputCDR &strm,
    FT_PullMonitorable_ptr &_tao_objref
  )
{
  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj;
    if ((strm >> obj.inout ()) == 0)
      return 0;
    // narrow to the right type
    _tao_objref	=
      FT_PullMonitorable::_unchecked_narrow (
	  obj.in (),
	  ACE_TRY_ENV
	);
    ACE_TRY_CHECK;
    return 1;
  }
  ACE_CATCHANY
  {
    // do nothing
  }
  ACE_ENDTRY;
  return 0;
}

ACE_INLINE CORBA::Boolean
operator<< (
    TAO_OutputCDR &,
    const FT_FaultNotifier_ptr
  );
ACE_INLINE CORBA::Boolean
operator>> (
    TAO_InputCDR &,
    FT_FaultNotifier_ptr &
  );

ACE_INLINE CORBA::Boolean
operator<< (
    TAO_OutputCDR &strm,
    const FT_FaultNotifier_ptr _tao_objref
  )
{
  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
  return (strm << _tao_corba_obj);
}

ACE_INLINE CORBA::Boolean
operator>> (
    TAO_InputCDR &strm,
    FT_FaultNotifier_ptr &_tao_objref
  )
{
  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj;
    if ((strm >> obj.inout ()) == 0)
      return 0;
    // narrow to the right type
    _tao_objref	=
      FT_FaultNotifier::_unchecked_narrow (
	  obj.in (),
	  ACE_TRY_ENV
	);
    ACE_TRY_CHECK;
    return 1;
  }
  ACE_CATCHANY
  {
    // do nothing
  }
  ACE_ENDTRY;
  return 0;
}


CORBA::Boolean TAO_Export operator<< (
    TAO_OutputCDR &,
    const FT_State &
  );
CORBA::Boolean TAO_Export operator>> (
    TAO_InputCDR &,
    FT_State &
  );


ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_NoStateAvailable &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_NoStateAvailable&)
{
  return 1;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_NoStateAvailable &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_InvalidState &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_InvalidState&)
{
  return 1;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_InvalidState &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_NoUpdateAvailable &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_NoUpdateAvailable&)
{
  return 1;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_NoUpdateAvailable &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_InvalidUpdate &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_InvalidUpdate&)
{
  return 1;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ACE_INLINE
ostream	& operator<< (ostream &os, const FT_InvalidUpdate &_tao_aggregate)
{
  CORBA::TypeCode_ptr tc = _tao_aggregate._type	();
  if (tc)
    {
      os << tc->name ()	<< " (";
    }
  os <<	_tao_aggregate._id ();
  if (tc)
    {
      os << ")";
    }
  return os;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

ACE_INLINE CORBA::Boolean
operator<< (
    TAO_OutputCDR &,
    const FT_Checkpointable_ptr
  );
ACE_INLINE CORBA::Boolean
operator>> (
    TAO_InputCDR &,
    FT_Checkpointable_ptr &
  );

ACE_INLINE CORBA::Boolean
operator<< (
    TAO_OutputCDR &strm,
    const FT_Checkpointable_ptr	_tao_objref
  )
{
  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
  return (strm << _tao_corba_obj);
}

ACE_INLINE CORBA::Boolean
operator>> (
    TAO_InputCDR &strm,
    FT_Checkpointable_ptr &_tao_objref
  )
{
  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj;
    if ((strm >> obj.inout ()) == 0)
      return 0;
    // narrow to the right type
    _tao_objref	=
      FT_Checkpointable::_unchecked_narrow (
	  obj.in (),
	  ACE_TRY_ENV
	);
    ACE_TRY_CHECK;
    return 1;
  }
  ACE_CATCHANY
  {
    // do nothing
  }
  ACE_ENDTRY;
  return 0;
}

ACE_INLINE CORBA::Boolean
operator<< (
    TAO_OutputCDR &,
    const FT_Updateable_ptr
  );
ACE_INLINE CORBA::Boolean
operator>> (
    TAO_InputCDR &,
    FT_Updateable_ptr &
  );

ACE_INLINE CORBA::Boolean
operator<< (
    TAO_OutputCDR &strm,
    const FT_Updateable_ptr _tao_objref
  )
{
  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
  return (strm << _tao_corba_obj);
}

ACE_INLINE CORBA::Boolean
operator>> (
    TAO_InputCDR &strm,
    FT_Updateable_ptr &_tao_objref
  )
{
  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj;
    if ((strm >> obj.inout ()) == 0)
      return 0;
    // narrow to the right type
    _tao_objref	=
      FT_Updateable::_unchecked_narrow (
	  obj.in (),
	  ACE_TRY_ENV
	);
    ACE_TRY_CHECK;
    return 1;
  }
  ACE_CATCHANY
  {
    // do nothing
  }
  ACE_ENDTRY;
  return 0;
}

//
//ACE_INLINE
//FT_GenericFactory::FT_GenericFactory (TAO_Stub *objref,
//					TAO_ServantBase	*_tao_servant,
//					CORBA::Boolean _tao_collocated)	// constructor
//  : CORBA_Object (objref, _tao_servant, _tao_collocated)
//{}
//
//
//// *************************************************************
//// Inline operations for class FT_GenericFactory_var
//// *************************************************************
//
//ACE_INLINE
//FT_GenericFactory_var::FT_GenericFactory_var (void) // default constructor
//  : ptr_ (FT_GenericFactory::_nil ())
//{}
//
//ACE_INLINE FT_GenericFactory_ptr
//FT_GenericFactory_var::ptr (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_GenericFactory_var::FT_GenericFactory_var (const FT_GenericFactory_var &p)	// copy	constructor
//  : TAO_Base_var (),
//    ptr_ (FT_GenericFactory::_duplicate (p.ptr ()))
//{}
//
//ACE_INLINE
//FT_GenericFactory_var::~FT_GenericFactory_var	(void) // destructor
//{
//  CORBA::release (this->ptr_);
//}
//
//ACE_INLINE FT_GenericFactory_var &
//FT_GenericFactory_var::operator= (FT_GenericFactory_ptr p)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_GenericFactory_var &
//FT_GenericFactory_var::operator= (const FT_GenericFactory_var	&p)
//{
//  if (this !=	&p)
//  {
//    CORBA::release (this->ptr_);
//    this->ptr_ = FT_GenericFactory::_duplicate (p.ptr	());
//  }
//  return *this;
//}
//
//ACE_INLINE
//FT_GenericFactory_var::operator const	FT_GenericFactory_ptr &() const	// cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_GenericFactory_var::operator FT_GenericFactory_ptr	&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_GenericFactory_ptr
//FT_GenericFactory_var::operator-> (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_GenericFactory_ptr
//FT_GenericFactory_var::in (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_GenericFactory_ptr &
//FT_GenericFactory_var::inout (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_GenericFactory_ptr &
//FT_GenericFactory_var::out (void)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = FT_GenericFactory::_nil ();
//  return this->ptr_;
//}
//
//ACE_INLINE FT_GenericFactory_ptr
//FT_GenericFactory_var::_retn (void)
//{
//  // yield ownership of managed obj reference
//  FT_GenericFactory_ptr val =	this->ptr_;
//  this->ptr_ = FT_GenericFactory::_nil ();
//  return val;
//}
//
//
// *************************************************************
// Inline operations for class FT_GenericFactory_out
// *************************************************************

//ACE_INLINE
//FT_GenericFactory_out::FT_GenericFactory_out (FT_GenericFactory_ptr &p)
//  : ptr_ (p)
//{
//  this->ptr_ = FT_GenericFactory::_nil ();
//}
//
//ACE_INLINE
//FT_GenericFactory_out::FT_GenericFactory_out (FT_GenericFactory_var &p) // constructor from _var
//  : ptr_ (p.out ())
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = FT_GenericFactory::_nil ();
//}
//
//ACE_INLINE
//FT_GenericFactory_out::FT_GenericFactory_out (const FT_GenericFactory_out &p)	// copy	constructor
//  : ptr_ (ACE_const_cast (FT_GenericFactory_out&,p).ptr_)
//{}
//
//ACE_INLINE FT_GenericFactory_out &
//FT_GenericFactory_out::operator= (const FT_GenericFactory_out	&p)
//{
//  this->ptr_ = ACE_const_cast	(FT_GenericFactory_out&,p).ptr_;
//  return *this;
//}
//
//ACE_INLINE FT_GenericFactory_out &
//FT_GenericFactory_out::operator= (const FT_GenericFactory_var	&p)
//{
//  this->ptr_ = FT_GenericFactory::_duplicate (p.ptr ());
//  return *this;
//}
//
//ACE_INLINE FT_GenericFactory_out &
//FT_GenericFactory_out::operator= (FT_GenericFactory_ptr p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_GenericFactory_out::operator FT_GenericFactory_ptr	&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_GenericFactory_ptr &
//FT_GenericFactory_out::ptr (void) // ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_GenericFactory_ptr
//FT_GenericFactory_out::operator-> (void)
//{
//  return this->ptr_;
//}
//

//ACE_INLINE
//FT_FaultNotifier::FT_FaultNotifier (TAO_Stub *objref,
//				      TAO_ServantBase *_tao_servant,
//				      CORBA::Boolean _tao_collocated) // constructor
//  : CORBA_Object (objref, _tao_servant, _tao_collocated)
//{}
//
// *************************************************************
// Inline operations for class FT_FaultNotifier_var
// *************************************************************

//ACE_INLINE
//FT_FaultNotifier_var::FT_FaultNotifier_var (void) // default constructor
//  : ptr_ (FT_FaultNotifier::_nil ())
//{}
//
//ACE_INLINE FT_FaultNotifier_ptr
//FT_FaultNotifier_var::ptr (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_FaultNotifier_var::FT_FaultNotifier_var (const FT_FaultNotifier_var &p) //	copy constructor
//  : TAO_Base_var (),
//    ptr_ (FT_FaultNotifier::_duplicate (p.ptr	()))
//{}
//
//ACE_INLINE
//FT_FaultNotifier_var::~FT_FaultNotifier_var (void) //	destructor
//{
//  CORBA::release (this->ptr_);
//}
//
//ACE_INLINE FT_FaultNotifier_var &
//FT_FaultNotifier_var::operator= (FT_FT_FaultNotifier_ptr p)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_FaultNotifier_var &
//FT_FaultNotifier_var::operator= (const FT_FaultNotifier_var &p)
//{
//  if (this !=	&p)
//  {
//    CORBA::release (this->ptr_);
//    this->ptr_ = FT_FaultNotifier::_duplicate	(p.ptr ());
//  }
//  return *this;
//}
//
//ACE_INLINE
//FT_FaultNotifier_var::operator const FT_FaultNotifier_ptr &()	const // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_FaultNotifier_var::operator FT_FaultNotifier_ptr &() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_FaultNotifier_ptr
//FT_FaultNotifier_var::operator-> (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_FaultNotifier_ptr
//FT_FaultNotifier_var::in (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_FaultNotifier_ptr &
//FT_FaultNotifier_var::inout (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_FaultNotifier_ptr &
//FT_FaultNotifier_var::out (void)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = FT_FaultNotifier::_nil	();
//  return this->ptr_;
//}
//
//ACE_INLINE FT_FaultNotifier_ptr
//FT_FaultNotifier_var::_retn (void)
//{
//  // yield ownership of managed obj reference
//  FT_FaultNotifier_ptr val = this->ptr_;
//  this->ptr_ = FT_FaultNotifier::_nil	();
//  return val;
//}
//


// *************************************************************
// Inline operations for class FT_FaultNotifier_out
// *************************************************************

//ACE_INLINE
//FT_FaultNotifier_out::FT_FaultNotifier_out (FT_FaultNotifier_ptr &p)
//  : ptr_ (p)
//{
//  this->ptr_ = FT_FaultNotifier::_nil	();
//}
//
//ACE_INLINE
//FT_FaultNotifier_out::FT_FaultNotifier_out (FT_FaultNotifier_var &p) // constructor from _var
//  : ptr_ (p.out ())
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = FT_FaultNotifier::_nil	();
//}
//
//ACE_INLINE
//FT_FaultNotifier_out::FT_FaultNotifier_out (const FT_FaultNotifier_out &p) //	copy constructor
//  : ptr_ (ACE_const_cast (FT_FaultNotifier_out&,p).ptr_)
//{}
//
//ACE_INLINE FT_FaultNotifier_out &
//FT_FaultNotifier_out::operator= (const FT_FaultNotifier_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_FaultNotifier_out&,p).ptr_;
//  return *this;
//}
//
//ACE_INLINE FT_FaultNotifier_out &
//FT_FaultNotifier_out::operator= (const FT_FaultNotifier_var &p)
//{
//  this->ptr_ = FT_FaultNotifier::_duplicate (p.ptr ());
//  return *this;
//}
//
//ACE_INLINE FT_FaultNotifier_out &
//FT_FaultNotifier_out::operator= (FT_FaultNotifier_ptr	p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_FaultNotifier_out::operator FT_FaultNotifier_ptr &() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_FaultNotifier_ptr &
//FT_FaultNotifier_out::ptr (void) // ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_FaultNotifier_ptr
//FT_FaultNotifier_out::operator-> (void)
//{
//  return this->ptr_;
//}
//
// *************************************************************
// Inline operations for class FT_Property_var
// *************************************************************
//
//ACE_INLINE
//FT_Property_var::FT_Property_var (void) // default constructor
//  : ptr_ (0)
//{}
//
//ACE_INLINE
//FT_Property_var::FT_Property_var (FT_Property	*p)
//  : ptr_ (p)
//{}
//
//ACE_INLINE
//FT_Property_var::FT_Property_var (const ::FT_Property_var &p)	// copy	constructor
//{
//  if (p.ptr_)
//    ACE_NEW (this->ptr_, ::FT_Property (*p.ptr_));
//  else
//    this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_Property_var::~FT_Property_var (void) // destructor
//{
//  delete this->ptr_;
//}
//
//ACE_INLINE FT_Property_var &
//FT_Property_var::operator= (FT_Property *p)
//{
//  delete this->ptr_;
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE ::FT_Property_var &
//FT_Property_var::operator= (const ::FT_Property_var &p)
//{
//  if (this !=	&p)
//  {
//    delete this->ptr_;
//    ACE_NEW_RETURN (this->ptr_, ::FT_Property	(*p.ptr_), *this);
//  }
//  return *this;
//}
//
//ACE_INLINE const ::FT_Property *
//FT_Property_var::operator-> (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Property *
//FT_Property_var::operator-> (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_Property_var::operator const ::FT_Property	&() const // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_Property_var::operator ::FT_Property &() // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_Property_var::operator ::FT_Property &() const // cast
//{
//  return *this->ptr_;
//}
//
//// variable-size types only
//ACE_INLINE
//FT_Property_var::operator ::FT_Property *&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE const ::FT_Property &
//FT_Property_var::in (void) const
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE ::FT_Property &
//FT_Property_var::inout (void)
//{
//  return *this->ptr_;
//}
//
//// mapping for variable size
//ACE_INLINE ::FT_Property *&
//FT_Property_var::out (void)
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Property *
//FT_Property_var::_retn (void)
//{
//  ::FT_Property *tmp = this->ptr_;
//  this->ptr_ = 0;
//  return tmp;
//}
//
//ACE_INLINE ::FT_Property *
//FT_Property_var::ptr (void) const
//{
//  return this->ptr_;
//}
//
// *************************************************************
// Inline operations for class FT_Property_out
// *************************************************************
//
//ACE_INLINE
//FT_Property_out::FT_Property_out (::FT_Property *&p)
//  : ptr_ (p)
//{
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_Property_out::FT_Property_out (FT_Property_var &p)	// constructor from _var
//  : ptr_ (p.out ())
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_Property_out::FT_Property_out (const ::FT_Property_out &p)	// copy	constructor
//  : ptr_ (ACE_const_cast (FT_Property_out&, p).ptr_)
//{}
//
//ACE_INLINE FT_Property_out &
//FT_Property_out::operator= (const ::FT_Property_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_Property_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE FT_Property_out &
//FT_Property_out::operator= (FT_Property *p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_Property_out::operator ::FT_Property *&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Property *&
//FT_Property_out::ptr (void) // ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Property *
//FT_Property_out::operator-> (void)
//{
//  return this->ptr_;
//}
//

// = Static operations.
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

/*ACE_INLINE FT_Property
_TAO_Unbounded_Sequence_FT_Properties::allocbuf	(CORBA::ULong size)
  // Allocate storage for the sequence.
{
  FT_Property *retval =	0;
  ACE_NEW_RETURN (retval, FT_Property[size], 0);
  return retval;
}

ACE_INLINE void
_TAO_Unbounded_Sequence_FT_Properties::freebuf (FT_Property *buffer)
  // Free the sequence.
{
  delete [] buffer;
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Properties::_TAO_Unbounded_Sequence_FT_Properties (void) // Default constructor.
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Properties::_TAO_Unbounded_Sequence_FT_Properties (CORBA::ULong maximum) // Constructor using a maximum length value.
  : TAO_Unbounded_Base_Sequence	(maximum, _TAO_Unbounded_Sequence_FT_Properties::allocbuf (maximum))
{
}

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Properties::_TAO_Unbounded_Sequence_FT_Properties (CORBA::ULong maximum,
   CORBA::ULong	length,
   FT_Property *data,
   CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence	(maximum, length, data,	release)
  {
  }

ACE_INLINE
_TAO_Unbounded_Sequence_FT_Properties::_TAO_Unbounded_Sequence_FT_Properties (const _TAO_Unbounded_Sequence_FT_Properties &rhs)
  // Copy constructor.
    : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      FT_Property *tmp1	= _TAO_Unbounded_Sequence_FT_Properties::allocbuf (this->maximum_);
      FT_Property * const tmp2 = ACE_reinterpret_cast (FT_Property * ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong	i = 0; i < this->length_; ++i)
	tmp1[i]	= tmp2[i];

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

ACE_INLINE FT_TAO_Unbounded_Sequence_FT_Properties &
_TAO_Unbounded_Sequence_FT_Properties::operator= (const	_TAO_Unbounded_Sequence_FT_Properties &rhs)
  // Assignment	operator.
{
  if (this == &rhs)
    return *this;

  if (this->release_)
  {
    if (this->maximum_ < rhs.maximum_)
    {
      // free the old buffer
      FT_Property *tmp = ACE_reinterpret_cast (FT_Property *, this->buffer_);
      _TAO_Unbounded_Sequence_FT_Properties::freebuf (tmp);
      this->buffer_ = _TAO_Unbounded_Sequence_FT_Properties::allocbuf (rhs.maximum_);
    }
  }
  else
    this->buffer_ = _TAO_Unbounded_Sequence_FT_Properties::allocbuf (rhs.maximum_);

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  FT_Property *tmp1 = ACE_reinterpret_cast (FT_Property	*, this->buffer_);
    FT_Property	* const	tmp2 = ACE_reinterpret_cast (FT_Property * ACE_CAST_CONST, rhs.buffer_);

  for (CORBA::ULong i =	0; i < this->length_; ++i)
  tmp1[i] = tmp2[i];

  return *this;
}

// = Accessors.
ACE_INLINE FT_Property &
_TAO_Unbounded_Sequence_FT_Properties::operator[] (CORBA::ULong	i)
  // operator []
{
  ACE_ASSERT (i	< this->maximum_);
  FT_Property* tmp = ACE_reinterpret_cast(FT_Property*,this->buffer_);
  return tmp[i];
}

ACE_INLINE const FT_Property &
_TAO_Unbounded_Sequence_FT_Properties::operator[] (CORBA::ULong	i) const
  // operator []
{
  ACE_ASSERT (i	< this->maximum_);
  FT_Property *	const tmp = ACE_reinterpret_cast (FT_Property* ACE_CAST_CONST, this->buffer_);
  return tmp[i];
}

// Implement the TAO_Base_Sequence methods (see	Sequence.h)

ACE_INLINE FT_Property *
_TAO_Unbounded_Sequence_FT_Properties::get_buffer (CORBA::Boolean orphan)
{
  FT_Property *result =	0;
  if (orphan ==	0)
    {
      // We retain ownership.
      if (this->buffer_	== 0)
	{
	  result = _TAO_Unbounded_Sequence_FT_Properties::allocbuf (this->length_);
	  this->buffer_	= result;
	}
      else
	{
	  result = ACE_reinterpret_cast	(FT_Property*, this->buffer_);
	}
    }
  else // if (orphan ==	1)
    {
      if (this->release_ != 0)
	{
	  // We	set the	state back to default and relinquish
	  // ownership.
	  result = ACE_reinterpret_cast(FT_Property*,this->buffer_);
	  this->maximum_ = 0;
	  this->length_	= 0;
	  this->buffer_	= 0;
	this->release_ = 0;
	}
    }
  return result;
}

ACE_INLINE const FT_Property *
_TAO_Unbounded_Sequence_FT_Properties::get_buffer (void) const
{
  return ACE_reinterpret_cast(const FT_Property	* ACE_CAST_CONST, this->buffer_);
}

ACE_INLINE void
_TAO_Unbounded_Sequence_FT_Properties::replace (CORBA::ULong max,
  CORBA::ULong length,
  FT_Property *data,
  CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_	= length;
  if (this->buffer_ && this->release_ == 1)
    {
      FT_Property *tmp = ACE_reinterpret_cast(FT_Property*,this->buffer_);
      _TAO_Unbounded_Sequence_FT_Properties::freebuf (tmp);
    }
  this->buffer_	= data;
  this->release_ = release;
}
*/
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */


// *************************************************************
// Inline operations for class FT_Properties_var
// *************************************************************

//ACE_INLINE
//FT_Properties_var::FT_Properties_var (void) // default constructor
//  : ptr_ (0)
//{}
//
//ACE_INLINE
//FT_Properties_var::FT_Properties_var (FT_Properties *p)
//  : ptr_ (p)
//{}
//
//ACE_INLINE
//FT_Properties_var::FT_Properties_var (const ::FT_Properties_var &p) // copy constructor
//{
//  if (p.ptr_)
//    ACE_NEW (this->ptr_, ::FT_Properties (*p.ptr_));
//  else
//    this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_Properties_var::~FT_Properties_var	(void) // destructor
//{
//  delete this->ptr_;
//}
//
//ACE_INLINE FT_Properties_var &
//FT_Properties_var::operator= (FT_Properties *p)
//{
//  delete this->ptr_;
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_Properties_var &
//FT_Properties_var::operator= (const ::FT_Properties_var &p) // deep copy
//{
//  if (this !=	&p)
//  {
//    delete this->ptr_;
//    ACE_NEW_RETURN (this->ptr_, ::FT_Properties (*p.ptr_), *this);
//  }
//  return *this;
//}
//
//ACE_INLINE const ::FT_Properties *
//FT_Properties_var::operator->	(void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Properties *
//FT_Properties_var::operator->	(void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_Properties_var::operator const ::FT_Properties &()	const // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_Properties_var::operator ::FT_Properties &() // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_Properties_var::operator ::FT_Properties &() const	// cast
//{
//  return *this->ptr_;
//}
//
//// variable-size types only
//ACE_INLINE
//FT_Properties_var::operator ::FT_Properties *&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_Property &
//FT_Properties_var::operator[]	(CORBA::ULong index)
//{
//  return this->ptr_->operator[] (index);
//}
//
//ACE_INLINE const ::FT_Properties &
//FT_Properties_var::in	(void) const
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE ::FT_Properties &
//FT_Properties_var::inout (void)
//{
//  return *this->ptr_;
//}
//
//// mapping for variable size
//ACE_INLINE ::FT_Properties *&
//FT_Properties_var::out (void)
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Properties *
//FT_Properties_var::_retn (void)
//{
//  ::FT_Properties *tmp = this->ptr_;
//  this->ptr_ = 0;
//  return tmp;
//}
//
//ACE_INLINE ::FT_Properties *
//FT_Properties_var::ptr (void)	const
//{
//  return this->ptr_;
//}
//
// *************************************************************
// Inline operations for class FT_Properties_out
// *************************************************************

//ACE_INLINE
//FT_Properties_out::FT_Properties_out (FT_Properties *&p)
//  : ptr_ (p)
//{
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_Properties_out::FT_Properties_out (FT_Properties_var &p) // constructor from _var
//  : ptr_ (p.out ())
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_Properties_out::FT_Properties_out (const ::FT_Properties_out &p) // copy constructor
//  : ptr_ (ACE_const_cast (FT_Properties_out&,	p).ptr_)
//{}
//
//ACE_INLINE ::FT_Properties_out &
//FT_Properties_out::operator= (const ::FT_Properties_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_Properties_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE ::FT_Properties_out &
//FT_Properties_out::operator= (FT_Properties *p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_Properties_out::operator ::FT_Properties *&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Properties *&
//FT_Properties_out::ptr (void)	// ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Properties *
//FT_Properties_out::operator->	(void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_Property &
//FT_Properties_out::operator[]	(CORBA::ULong index)
//{
//  return this->ptr_->operator[] (index);
//}
//
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
//
//// = Static operations.
//ACE_INLINE FT_Location *
//_TAO_Unbounded_Sequence_FT_Locations::allocbuf (CORBA::ULong size)
//  // Allocate	storage	for the	sequence.
//{
//  FT_Location	*retval	= 0;
//  ACE_NEW_RETURN (retval, FT_Location[size], 0);
//  return retval;
//}
//
//ACE_INLINE void
//_TAO_Unbounded_Sequence_FT_Locations::freebuf	(FT::Location *buffer)
//  // Free the	sequence.
//{
//  delete [] buffer;
//}
//
//ACE_INLINE
//_TAO_Unbounded_Sequence_FT_Locations::_TAO_Unbounded_Sequence_FT_Locations (void) // Default constructor.
//{
//}
//
//ACE_INLINE
//_TAO_Unbounded_Sequence_FT_Locations::_TAO_Unbounded_Sequence_FT_Locations (CORBA::ULong maximum) // Constructor using a maximum length value.
//  : TAO_Unbounded_Base_Sequence (maximum, _TAO_Unbounded_Sequence_FT_Locations::allocbuf (maximum))
//{
//}
//
//ACE_INLINE
//_TAO_Unbounded_Sequence_FT_Locations::_TAO_Unbounded_Sequence_FT_Locations (CORBA::ULong maximum,
//    CORBA::ULong length,
//    FT_Location *data,
//    CORBA::Boolean release)
//  : TAO_Unbounded_Base_Sequence (maximum, length, data, release)
//{
//}
//
//ACE_INLINE
//_TAO_Unbounded_Sequence_FT_Locations::_TAO_Unbounded_Sequence_FT_Locations (const _TAO_Unbounded_Sequence_FT_Locations &rhs)
//  // Copy constructor.
//  : TAO_Unbounded_Base_Sequence (rhs)
//{
//  if (rhs.buffer_ != 0)
//    {
//	FT_Location *tmp1 = _TAO_Unbounded_Sequence_FT_Locations::allocbuf (this->maximum_);
//	FT_Location * const tmp2 = ACE_reinterpret_cast	(FT_Location * ACE_CAST_CONST, rhs.buffer_);
//
//	for (CORBA::ULong i = 0; i < this->length_; ++i)
//	  tmp1[i] = tmp2[i];
//
//	this->buffer_ =	tmp1;
//    }
//  else
//    {
//	this->buffer_ =	0;
//    }
//}
//
//ACE_INLINE _TAO_Unbounded_Sequence_FT_Locations &
//_TAO_Unbounded_Sequence_FT_Locations::operator= (const _TAO_Unbounded_Sequence_FT_Locations &rhs)
//  // Assignment operator.
//{
//  if (this ==	&rhs)
//  return *this;
//
//  if (this->release_)
//  {
//    if (this->maximum_ < rhs.maximum_)
//    {
//	// free	the old	buffer
//	FT_Location *tmp = ACE_reinterpret_cast	(FT_Location *,	this->buffer_);
//	_TAO_Unbounded_Sequence_FT_Locations::freebuf (tmp);
//	this->buffer_ =	_TAO_Unbounded_Sequence_FT_Locations::allocbuf (rhs.maximum_);
//    }
//  }
//  else
//  this->buffer_ = _TAO_Unbounded_Sequence_FT_Locations::allocbuf (rhs.maximum_);
//
//  TAO_Unbounded_Base_Sequence::operator= (rhs);
//
//  FT_Location	*tmp1 =	ACE_reinterpret_cast (FT_Location *, this->buffer_);
//  FT_Location	* const	tmp2 = ACE_reinterpret_cast (FT_Location * ACE_CAST_CONST, rhs.buffer_);
//
//  for	(CORBA::ULong i	= 0; i < this->length_;	++i)
//  tmp1[i] = tmp2[i];
//
//  return *this;
//}
//
//// = Accessors.
//ACE_INLINE FT_Location &
//_TAO_Unbounded_Sequence_FT_Locations::operator[] (CORBA::ULong i)
//  // operator	[]
//{
//  ACE_ASSERT (i < this->maximum_);
//  FT_Location* tmp = ACE_reinterpret_cast(FT_Location*,this->buffer_);
//  return tmp[i];
//}
//
//ACE_INLINE const FT_Location &
//_TAO_Unbounded_Sequence_FT_Locations::operator[] (CORBA::ULong i) const
//  // operator	[]
//{
//  ACE_ASSERT (i < this->maximum_);
//  FT_Location	* const	tmp = ACE_reinterpret_cast (FT_Location* ACE_CAST_CONST, this->buffer_);
//  return tmp[i];
//}
//
//// Implement the TAO_Base_Sequence methods (see Sequence.h)
//
//ACE_INLINE FT_Location *
//_TAO_Unbounded_Sequence_FT_Locations::get_buffer (CORBA::Boolean orphan)
//{
//  FT_Location	*result	= 0;
//  if (orphan == 0)
//    {
//	// We retain ownership.
//	if (this->buffer_ == 0)
//	  {
//	    result = _TAO_Unbounded_Sequence_FT_Locations::allocbuf (this->length_);
//	    this->buffer_ = result;
//	  }
//	else
//	  {
//	    result = ACE_reinterpret_cast (FT_Location*, this->buffer_);
//	  }
//    }
//  else // if (orphan == 1)
//    {
//	if (this->release_ != 0)
//	  {
//	    // We set the state	back to	default	and relinquish
//	    // ownership.
//	    result = ACE_reinterpret_cast(FT_Location*,this->buffer_);
//	    this->maximum_ = 0;
//	    this->length_ = 0;
//	    this->buffer_ = 0;
//	    this->release_ = 0;
//	  }
//    }
//  return result;
//}
//
//ACE_INLINE const FT_Location *
//_TAO_Unbounded_Sequence_FT_Locations::get_buffer (void) const
//{
//  return ACE_reinterpret_cast(const FT_Location * ACE_CAST_CONST, this->buffer_);
//}
//
//ACE_INLINE void
//_TAO_Unbounded_Sequence_FT_Locations::replace	(CORBA::ULong max,
//  CORBA::ULong length,
//  FT_Location	*data,
//  CORBA::Boolean release)
//{
//  this->maximum_ = max;
//  this->length_ = length;
//  if (this->buffer_ && this->release_	== 1)
//    {
//	FT_Location *tmp = ACE_reinterpret_cast(FT_Location*,this->buffer_);
//	_TAO_Unbounded_Sequence_FT_Locations::freebuf (tmp);
//    }
//  this->buffer_ = data;
//  this->release_ = release;
//}
//
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

// *************************************************************
// Inline operations for class FT_Locations_var
// *************************************************************
//
//ACE_INLINE
//FT_Locations_var::FT_Locations_var (void) // default constructor
//  : ptr_ (0)
//{}
//
//ACE_INLINE
//FT_Locations_var::FT_Locations_var (FT_Locations *p)
//  : ptr_ (p)
//{}
//
//ACE_INLINE
//FT_Locations_var::FT_Locations_var (const ::FT_Locations_var &p) // copy constructor
//{
//  if (p.ptr_)
//    ACE_NEW (this->ptr_, ::FT_Locations (*p.ptr_));
//  else
//    this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_Locations_var::~FT_Locations_var (void) //	destructor
//{
//  delete this->ptr_;
//}
//
//ACE_INLINE FT_Locations_var &
//FT_Locations_var::operator= (FT_Locations *p)
//{
//  delete this->ptr_;
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_Locations_var &
//FT_Locations_var::operator= (const ::FT_Locations_var	&p) // deep copy
//{
//  if (this !=	&p)
//  {
//    delete this->ptr_;
//    ACE_NEW_RETURN (this->ptr_, ::FT_Locations (*p.ptr_), *this);
//  }
//  return *this;
//}
//
//ACE_INLINE const ::FT_Locations *
//FT_Locations_var::operator-> (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Locations *
//FT_Locations_var::operator-> (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_Locations_var::operator const ::FT_Locations &() const // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_Locations_var::operator ::FT_Locations &()	// cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_Locations_var::operator ::FT_Locations &()	const // cast
//{
//  return *this->ptr_;
//}
//
//// variable-size types only
//ACE_INLINE
//FT_Locations_var::operator ::FT_Locations *&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_Location &
//FT_Locations_var::operator[] (CORBA::ULong index)
//{
//  return this->ptr_->operator[] (index);
//}
//
//ACE_INLINE const ::FT_Locations &
//FT_Locations_var::in (void) const
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE ::FT_Locations &
//FT_Locations_var::inout (void)
//{
//  return *this->ptr_;
//}
//
//// mapping for variable size
//ACE_INLINE ::FT_Locations *&
//FT_Locations_var::out	(void)
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Locations *
//FT_Locations_var::_retn (void)
//{
//  ::FT_Locations *tmp	= this->ptr_;
//  this->ptr_ = 0;
//  return tmp;
//}
//
//ACE_INLINE ::FT_Locations *
//FT_Locations_var::ptr	(void) const
//{
//  return this->ptr_;
//}
//
//// *************************************************************
//// Inline operations for class FT_Locations_out
//// *************************************************************
//
//ACE_INLINE
//FT_Locations_out::FT_Locations_out (FT_Locations *&p)
//  : ptr_ (p)
//{
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_Locations_out::FT_Locations_out (FT_Locations_var &p) // constructor from _var
//  : ptr_ (p.out ())
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_Locations_out::FT_Locations_out (const ::FT_Locations_out &p) // copy constructor
//  : ptr_ (ACE_const_cast (Locations_out&, p).ptr_)
//{}
//
//ACE_INLINE ::FT_Locations_out	&
//FT_Locations_out::operator= (const ::FT_Locations_out	&p)
//{
//  this->ptr_ = ACE_const_cast	(Locations_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE ::FT_Locations_out	&
//FT_Locations_out::operator= (FT_Locations *p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_Locations_out::operator ::FT_Locations *&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Locations *&
//FT_Locations_out::ptr	(void) // ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Locations *
//FT_Locations_out::operator-> (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_Location &
//FT_Locations_out::operator[] (CORBA::ULong index)
//{
//  return this->ptr_->operator[] (index);
//}
//
//

// *************************************************************
// Inline operations for class FT_FactoryInfo_var
// *************************************************************

//ACE_INLINE
//FT_FactoryInfo_var::FT_FactoryInfo_var (void)	// default constructor
//  : ptr_ (0)
//{}
//
//ACE_INLINE
//FT_FactoryInfo_var::FT_FactoryInfo_var (FT_FactoryInfo *p)
//  : ptr_ (p)
//{}
//
//ACE_INLINE
//FT_FactoryInfo_var::FT_FactoryInfo_var (const	::FT_FactoryInfo_var &p) // copy constructor
//{
//  if (p.ptr_)
//    ACE_NEW (this->ptr_, ::FT_FactoryInfo (*p.ptr_));
//  else
//    this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_FactoryInfo_var::~FT_FactoryInfo_var (void) // destructor
//{
//  delete this->ptr_;
//}
//
//ACE_INLINE FT_FactoryInfo_var	&
//FT_FactoryInfo_var::operator=	(FT_FactoryInfo	*p)
//{
//  delete this->ptr_;
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE ::FT_FactoryInfo_var &
//FT_FactoryInfo_var::operator=	(const ::FT_FactoryInfo_var &p)
//{
//  if (this !=	&p)
//  {
//    delete this->ptr_;
//    ACE_NEW_RETURN (this->ptr_, ::FT_FactoryInfo (*p.ptr_), *this);
//  }
//  return *this;
//}
//
//ACE_INLINE const ::FT_FactoryInfo *
//FT_FactoryInfo_var::operator-> (void)	const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_FactoryInfo *
//FT_FactoryInfo_var::operator-> (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_FactoryInfo_var::operator const ::FT_FactoryInfo &() const	// cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_FactoryInfo_var::operator ::FT_FactoryInfo	&() // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_FactoryInfo_var::operator ::FT_FactoryInfo	&() const // cast
//{
//  return *this->ptr_;
//}
//
//// variable-size types only
//ACE_INLINE
//FT_FactoryInfo_var::operator ::FT_FactoryInfo	*&() //	cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE const ::FT_FactoryInfo &
//FT_FactoryInfo_var::in (void)	const
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE ::FT_FactoryInfo &
//FT_FactoryInfo_var::inout (void)
//{
//  return *this->ptr_;
//}
//
//// mapping for variable size
//ACE_INLINE ::FT_FactoryInfo *&
//FT_FactoryInfo_var::out (void)
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_FactoryInfo *
//FT_FactoryInfo_var::_retn (void)
//{
//  ::FT_FactoryInfo *tmp = this->ptr_;
//  this->ptr_ = 0;
//  return tmp;
//}
//
//ACE_INLINE ::FT_FactoryInfo *
//FT_FactoryInfo_var::ptr (void) const
//{
//  return this->ptr_;
//}
//
//// *************************************************************
//// Inline operations for class FT_FactoryInfo_out
//// *************************************************************
//
//ACE_INLINE
//FT_FactoryInfo_out::FT_FactoryInfo_out (::FT_FactoryInfo *&p)
//  : ptr_ (p)
//{
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_FactoryInfo_out::FT_FactoryInfo_out (FT_FactoryInfo_var &p) // constructor	from _var
//  : ptr_ (p.out ())
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_FactoryInfo_out::FT_FactoryInfo_out (const	::FT_FactoryInfo_out &p) // copy constructor
//  : ptr_ (ACE_const_cast (FT_FactoryInfo_out&, p).ptr_)
//{}
//
//ACE_INLINE FT_FactoryInfo_out	&
//FT_FactoryInfo_out::operator=	(const ::FT_FactoryInfo_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_FactoryInfo_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE FT_FactoryInfo_out	&
//FT_FactoryInfo_out::operator=	(FT_FactoryInfo	*p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_FactoryInfo_out::operator ::FT_FactoryInfo	*&() //	cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_FactoryInfo *&
//FT_FactoryInfo_out::ptr (void) // ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_FactoryInfo *
//FT_FactoryInfo_out::operator-> (void)
//{
//  return this->ptr_;
//}
//
//
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

// = Static operations.
//ACE_INLINE FT_FactoryInfo *
//_TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (CORBA::ULong size)
//  // Allocate	storage	for the	sequence.
//{
//  FT_FactoryInfo *retval = 0;
//  ACE_NEW_RETURN (retval, FT_FactoryInfo[size], 0);
//  return retval;
//}
//
//ACE_INLINE void
//_TAO_Unbounded_Sequence_FT_FactoryInfos::freebuf (FT_FactoryInfo *buffer)
//  // Free the	sequence.
//{
//  delete [] buffer;
//}
//
//ACE_INLINE
//_TAO_Unbounded_Sequence_FT_FactoryInfos::_TAO_Unbounded_Sequence_FT_FactoryInfos (void) // Default constructor.
//{
//}
//
//ACE_INLINE
//_TAO_Unbounded_Sequence_FT_FactoryInfos::_TAO_Unbounded_Sequence_FT_FactoryInfos (CORBA::ULong maximum) // Constructor using a maximum length	value.
//  : TAO_Unbounded_Base_Sequence (maximum, _TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (maximum))
//{
//}
//
//ACE_INLINE
//_TAO_Unbounded_Sequence_FT_FactoryInfos::_TAO_Unbounded_Sequence_FT_FactoryInfos (CORBA::ULong maximum,
//										    CORBA::ULong length,
//										    FT_FactoryInfo *data,
//										    CORBA::Boolean release)
//  : TAO_Unbounded_Base_Sequence (maximum, length, data, release)
//{
//}
//
//ACE_INLINE
//_TAO_Unbounded_Sequence_FT_FactoryInfos::_TAO_Unbounded_Sequence_FT_FactoryInfos (const _TAO_Unbounded_Sequence_FT_FactoryInfos &rhs)
//  // Copy constructor.
//  : TAO_Unbounded_Base_Sequence (rhs)
//{
//  if (rhs.buffer_ != 0)
//    {
//	FT_FactoryInfo *tmp1 = _TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (this->maximum_);
//	FT_FactoryInfo * const tmp2 = ACE_reinterpret_cast (FT_FactoryInfo * ACE_CAST_CONST, rhs.buffer_);
//
//	for (CORBA::ULong i = 0; i < this->length_; ++i)
//	  tmp1[i] = tmp2[i];
//
//	this->buffer_ =	tmp1;
//    }
//    else
//	{
//	  this->buffer_	= 0;
//	}
//}
//
//ACE_INLINE FT__TAO_Unbounded_Sequence_FT_FactoryInfos	&
//_TAO_Unbounded_Sequence_FT_FactoryInfos::operator= (const _TAO_Unbounded_Sequence_FT_FactoryInfos &rhs)
//  // Assignment operator.
//{
//  if (this ==	&rhs)
//  return *this;
//
//  if (this->release_)
//  {
//    if (this->maximum_ < rhs.maximum_)
//    {
//	// free	the old	buffer
//	FT_FactoryInfo *tmp = ACE_reinterpret_cast (FT_FactoryInfo *, this->buffer_);
//	_TAO_Unbounded_Sequence_FT_FactoryInfos::freebuf (tmp);
//	this->buffer_ =	_TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (rhs.maximum_);
//    }
//  }
//  else
//	this->buffer_ =	_TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (rhs.maximum_);
//
//  TAO_Unbounded_Base_Sequence::operator= (rhs);
//
//  FT_FactoryInfo *tmp1 = ACE_reinterpret_cast	(FT_FactoryInfo	*, this->buffer_);
//  FT_FactoryInfo * const tmp2	= ACE_reinterpret_cast (FT_FactoryInfo * ACE_CAST_CONST, rhs.buffer_);
//
//  for	(CORBA::ULong i	= 0; i < this->length_;	++i)
//  tmp1[i] = tmp2[i];
//
//  return *this;
//}
//
//// = Accessors.
//ACE_INLINE FT_FactoryInfo &
//_TAO_Unbounded_Sequence_FT_FactoryInfos::operator[] (CORBA::ULong i)
//  // operator	[]
//{
//  ACE_ASSERT (i < this->maximum_);
//  FT_FactoryInfo* tmp	= ACE_reinterpret_cast(FT_FactoryInfo*,this->buffer_);
//  return tmp[i];
//}
//
//ACE_INLINE const FT_FactoryInfo &
//_TAO_Unbounded_Sequence_FT_FactoryInfos::operator[] (CORBA::ULong i) const
//  // operator	[]
//{
//  ACE_ASSERT (i < this->maximum_);
//  FT_FactoryInfo * const tmp = ACE_reinterpret_cast (FT_FactoryInfo* ACE_CAST_CONST, this->buffer_);
//  return tmp[i];
//}
//
//  // Implement the TAO_Base_Sequence methods (see Sequence.h)
//
//ACE_INLINE FT_FactoryInfo *
//_TAO_Unbounded_Sequence_FT_FactoryInfos::get_buffer (CORBA::Boolean orphan)
//{
//  FT_FactoryInfo *result = 0;
//  if (orphan == 0)
//    {
//	// We retain ownership.
//	if (this->buffer_ == 0)
//	  {
//	    result = _TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (this->length_);
//	    this->buffer_ = result;
//	  }
//	else
//	{
//	  result = ACE_reinterpret_cast	(FT_FactoryInfo*, this->buffer_);
//	}
//    }
//  else // if (orphan == 1)
//    {
//	if (this->release_ != 0)
//	  {
//	    // We set the state	back to	default	and relinquish
//	    // ownership.
//	    result = ACE_reinterpret_cast(FT_FactoryInfo*,this->buffer_);
//	    this->maximum_ = 0;
//	    this->length_ = 0;
//	  this->buffer_	= 0;
//	  this->release_ = 0;
//	}
//    }
//    return result;
//}
//
//ACE_INLINE const FT_FactoryInfo *
//_TAO_Unbounded_Sequence_FT_FactoryInfos::get_buffer (void) const
//{
//  return ACE_reinterpret_cast(const FT_FactoryInfo * ACE_CAST_CONST, this->buffer_);
//}
//
//ACE_INLINE void
//_TAO_Unbounded_Sequence_FT_FactoryInfos::replace (CORBA::ULong max,
//						    CORBA::ULong length,
//						    FT_FactoryInfo *data,
//						    CORBA::Boolean release)
//{
//  this->maximum_ = max;
//  this->length_ = length;
//  if (this->buffer_ && this->release_	== 1)
//    {
//	FT_FactoryInfo *tmp = ACE_reinterpret_cast(FT_FactoryInfo*,this->buffer_);
//	_TAO_Unbounded_Sequence_FT_FactoryInfos::freebuf (tmp);
//    }
//  this->buffer_ = data;
//  this->release_ = release;
//}
//
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

// *************************************************************
// Inline operations for class FT_FactoryInfos_var
// *************************************************************

//ACE_INLINE
//FT_FactoryInfos_var::FT_FactoryInfos_var (void) // default constructor
//  : ptr_ (0)
//{}
//
//ACE_INLINE
//FT_FactoryInfos_var::FT_FactoryInfos_var (FT_FactoryInfos *p)
//  : ptr_ (p)
//{}
//
//ACE_INLINE
//FT_FactoryInfos_var::FT_FactoryInfos_var (const ::FT_FactoryInfos_var	&p) // copy constructor
//{
//  if (p.ptr_)
//    ACE_NEW (this->ptr_, ::FT_FactoryInfos (*p.ptr_));
//  else
//    this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_FactoryInfos_var::~FT_FactoryInfos_var (void) // destructor
//{
//  delete this->ptr_;
//}
//
//ACE_INLINE FT_FactoryInfos_var &
//FT_FactoryInfos_var::operator= (FT_FactoryInfos *p)
//{
//  delete this->ptr_;
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_FactoryInfos_var &
//FT_FactoryInfos_var::operator= (const	::FT_FactoryInfos_var &p) // deep copy
//{
//  if (this !=	&p)
//  {
//    delete this->ptr_;
//    ACE_NEW_RETURN (this->ptr_, ::FT_FactoryInfos (*p.ptr_), *this);
//  }
//  return *this;
//}
//
//ACE_INLINE const ::FT_FactoryInfos *
//FT_FactoryInfos_var::operator-> (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_FactoryInfos *
//FT_FactoryInfos_var::operator-> (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_FactoryInfos_var::operator	const ::FT_FactoryInfos	&() const // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_FactoryInfos_var::operator	::FT_FactoryInfos &() // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_FactoryInfos_var::operator	::FT_FactoryInfos &() const // cast
//{
//  return *this->ptr_;
//}
//
//// variable-size types only
//ACE_INLINE
//FT_FactoryInfos_var::operator	::FT_FactoryInfos *&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_FactoryInfo &
//FT_FactoryInfos_var::operator[] (CORBA::ULong	index)
//{
//  return this->ptr_->operator[] (index);
//}
//
//ACE_INLINE const ::FT_FactoryInfos &
//FT_FactoryInfos_var::in (void) const
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE ::FT_FactoryInfos &
//FT_FactoryInfos_var::inout (void)
//{
//  return *this->ptr_;
//}
//
//// mapping for variable size
//ACE_INLINE ::FT_FactoryInfos *&
//FT_FactoryInfos_var::out (void)
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_FactoryInfos *
//FT_FactoryInfos_var::_retn (void)
//{
//  ::FT_FactoryInfos *tmp = this->ptr_;
//  this->ptr_ = 0;
//  return tmp;
//}
//
//ACE_INLINE ::FT_FactoryInfos *
//FT_FactoryInfos_var::ptr (void) const
//{
//  return this->ptr_;
//}
//
//// *************************************************************
//// Inline operations for class FT_FactoryInfos_out
//// *************************************************************
//
//ACE_INLINE
//FT_FactoryInfos_out::FT_FactoryInfos_out (FT_FactoryInfos *&p)
//  : ptr_ (p)
//{
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_FactoryInfos_out::FT_FactoryInfos_out (FT_FactoryInfos_var	&p) // constructor from	_var
//  : ptr_ (p.out ())
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_FactoryInfos_out::FT_FactoryInfos_out (const ::FT_FactoryInfos_out	&p) // copy constructor
//  : ptr_ (ACE_const_cast (FT_FactoryInfos_out&, p).ptr_)
//{}
//
//ACE_INLINE ::FT_FactoryInfos_out &
//FT_FactoryInfos_out::operator= (const	::FT_FactoryInfos_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_FactoryInfos_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE ::FT_FactoryInfos_out &
//FT_FactoryInfos_out::operator= (FT_FactoryInfos *p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_FactoryInfos_out::operator	::FT_FactoryInfos *&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_FactoryInfos *&
//FT_FactoryInfos_out::ptr (void) // ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_FactoryInfos *
//FT_FactoryInfos_out::operator-> (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE FT_FactoryInfo &
//FT_FactoryInfos_out::operator[] (CORBA::ULong	index)
//{
//  return this->ptr_->operator[] (index);
//}
//


// *************************************************************
// Inline operations for class FT_FaultMonitoringIntervalAndTimeoutValue_var
// *************************************************************

//ACE_INLINE
//FT_FaultMonitoringIntervalAndTimeoutValue_var::FT_FaultMonitoringIntervalAndTimeoutValue_var (void) // default constructor
//  : ptr_ (0)
//{}
//
//ACE_INLINE
//FT_FaultMonitoringIntervalAndTimeoutValue_var::FT_FaultMonitoringIntervalAndTimeoutValue_var (FT_FaultMonitoringIntervalAndTimeoutValue *p)
//  : ptr_ (p)
//{}
//
//ACE_INLINE
//FT_FaultMonitoringIntervalAndTimeoutValue_var::FT_FaultMonitoringIntervalAndTimeoutValue_var (const ::FT_FaultMonitoringIntervalAndTimeoutValue_var &p) // copy constructor
//{
//  if (p.ptr_)
//    ACE_NEW (this->ptr_, ::FT_FaultMonitoringIntervalAndTimeoutValue (*p.ptr_));
//  else
//    this->ptr_ = 0;
//}
//
//// fixed-size	types only
//ACE_INLINE
//FT_FaultMonitoringIntervalAndTimeoutValue_var::FT_FaultMonitoringIntervalAndTimeoutValue_var (const ::FT_FaultMonitoringIntervalAndTimeoutValue &p)
//{
//  ACE_NEW (this->ptr_, ::FT_FaultMonitoringIntervalAndTimeoutValue (p));
//}
//
//ACE_INLINE
//FT_FaultMonitoringIntervalAndTimeoutValue_var::~FT_FaultMonitoringIntervalAndTimeoutValue_var	(void) // destructor
//{
//  delete this->ptr_;
//}
//
//ACE_INLINE FT_FaultMonitoringIntervalAndTimeoutValue_var &
//FT_FaultMonitoringIntervalAndTimeoutValue_var::operator= (FT_FaultMonitoringIntervalAndTimeoutValue *p)
//{
//  delete this->ptr_;
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue_var &
//FT_FaultMonitoringIntervalAndTimeoutValue_var::operator= (const ::FT_FaultMonitoringIntervalAndTimeoutValue_var &p)
//{
//  if (this !=	&p)
//  {
//    delete this->ptr_;
//    ACE_NEW_RETURN (this->ptr_, ::FT_FaultMonitoringIntervalAndTimeoutValue (*p.ptr_), *this);
//  }
//  return *this;
//}
//
//// fixed-size	types only
//ACE_INLINE FT_FaultMonitoringIntervalAndTimeoutValue_var &
//FT_FaultMonitoringIntervalAndTimeoutValue_var::operator= (const ::FT_FaultMonitoringIntervalAndTimeoutValue &p)
//{
//  if (this->ptr_ != &p)
//  {
//    delete this->ptr_;
//    ACE_NEW_RETURN (this->ptr_, ::FT_FaultMonitoringIntervalAndTimeoutValue (p), *this);
//  }
//  return *this;
//}
//
//ACE_INLINE const ::FT_FaultMonitoringIntervalAndTimeoutValue *
//FT_FaultMonitoringIntervalAndTimeoutValue_var::operator-> (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue *
//FT_FaultMonitoringIntervalAndTimeoutValue_var::operator-> (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_FaultMonitoringIntervalAndTimeoutValue_var::operator const	::FT_FaultMonitoringIntervalAndTimeoutValue &()	const // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_FaultMonitoringIntervalAndTimeoutValue_var::operator ::FT_FaultMonitoringIntervalAndTimeoutValue &() // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_FaultMonitoringIntervalAndTimeoutValue_var::operator ::FT_FaultMonitoringIntervalAndTimeoutValue &() const	// cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE const ::FT_FaultMonitoringIntervalAndTimeoutValue &
//FT_FaultMonitoringIntervalAndTimeoutValue_var::in (void) const
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue &
//FT_FaultMonitoringIntervalAndTimeoutValue_var::inout (void)
//{
//  return *this->ptr_;
//}
//
//// mapping for fixed size
//ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue &
//FT_FaultMonitoringIntervalAndTimeoutValue_var::out (void)
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue
//FT_FaultMonitoringIntervalAndTimeoutValue_var::_retn (void)
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE ::FT_FaultMonitoringIntervalAndTimeoutValue *
//FT_FaultMonitoringIntervalAndTimeoutValue_var::ptr (void) const
//{
//  return this->ptr_;
//}
//


//ACE_INLINE
//FT_PropertyManager::FT_PropertyManager (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean _tao_collocated) // constructor
//  : CORBA_Object (objref, _tao_servant, _tao_collocated)
//{}
//
//

// *************************************************************
// Inline operations for class FT_PropertyManager_var
// *************************************************************

//ACE_INLINE
//FT_PropertyManager_var::FT_PropertyManager_var (void)	// default constructor
//  : ptr_ (FT_PropertyManager::_nil ())
//{}
//
//ACE_INLINE ::FT_PropertyManager_ptr
//FT_PropertyManager_var::ptr (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_PropertyManager_var::FT_PropertyManager_var (const	::FT_PropertyManager_var &p) //	copy constructor
//  : TAO_Base_var (),
//    ptr_ (FT_PropertyManager::_duplicate (p.ptr ()))
//{}
//
//ACE_INLINE
//FT_PropertyManager_var::~FT_PropertyManager_var (void) // destructor
//{
//  CORBA::release (this->ptr_);
//}
//
//ACE_INLINE FT_PropertyManager_var &
//FT_PropertyManager_var::operator= (FT_PropertyManager_ptr p)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_PropertyManager_var &
//FT_PropertyManager_var::operator= (const ::FT_FT_PropertyManager_var &p)
//{
//  if (this !=	&p)
//  {
//    CORBA::release (this->ptr_);
//    this->ptr_ = ::FT_PropertyManager::_duplicate (p.ptr ());
//  }
//  return *this;
//}
//
//ACE_INLINE
//FT_PropertyManager_var::operator const ::FT_PropertyManager_ptr &() const // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_PropertyManager_var::operator ::FT_PropertyManager_ptr &()	// cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PropertyManager_ptr
//FT_PropertyManager_var::operator-> (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PropertyManager_ptr
//FT_PropertyManager_var::in (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PropertyManager_ptr &
//FT_PropertyManager_var::inout	(void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PropertyManager_ptr &
//FT_PropertyManager_var::out (void)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_PropertyManager::_nil ();
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PropertyManager_ptr
//FT_PropertyManager_var::_retn	(void)
//{
//  // yield ownership of managed obj reference
//  ::FT_PropertyManager_ptr val = this->ptr_;
//  this->ptr_ = ::FT_PropertyManager::_nil ();
//  return val;
//}
//


// *************************************************************
// Inline operations for class FT_PropertyManager_out
// *************************************************************

//ACE_INLINE
//FT_PropertyManager_out::FT_PropertyManager_out (FT_PropertyManager_ptr &p)
//  : ptr_ (p)
//{
//  this->ptr_ = ::FT_PropertyManager::_nil ();
//}
//
//ACE_INLINE
//FT_PropertyManager_out::FT_PropertyManager_out (FT_PropertyManager_var &p) //	constructor from _var
//  : ptr_ (p.out ())
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_PropertyManager::_nil ();
//}
//
//ACE_INLINE
//FT_PropertyManager_out::FT_PropertyManager_out (const	::FT_PropertyManager_out &p) //	copy constructor
//  : ptr_ (ACE_const_cast (FT_PropertyManager_out &, p).ptr_)
//{}
//
//ACE_INLINE ::FT_PropertyManager_out &
//FT_PropertyManager_out::operator= (const ::FT_PropertyManager_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_PropertyManager_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE FT_PropertyManager_out &
//FT_PropertyManager_out::operator= (const ::FT_PropertyManager_var &p)
//{
//  this->ptr_ = ::FT_PropertyManager::_duplicate (p.ptr ());
//  return *this;
//}
//
//ACE_INLINE FT_PropertyManager_out &
//FT_PropertyManager_out::operator= (FT_PropertyManager_ptr p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_PropertyManager_out::operator ::FT_PropertyManager_ptr &()	// cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PropertyManager_ptr &
//FT_PropertyManager_out::ptr (void) //	ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PropertyManager_ptr
//FT_PropertyManager_out::operator-> (void)
//{
//  return this->ptr_;
//}
//

//#endif /* end	#if !defined */

//ACE_INLINE
//FT_ObjectGroupManager::FT_ObjectGroupManager (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean	_tao_collocated) // constructor
//  : CORBA_Object (objref, _tao_servant, _tao_collocated)
//{}
//


// *************************************************************
// Inline operations for class FT_ObjectGroupManager_var
// *************************************************************

//ACE_INLINE
//FT_ObjectGroupManager_var::FT_ObjectGroupManager_var (void) // default constructor
//  : ptr_ (FT_ObjectGroupManager::_nil	())
//{}
//
//ACE_INLINE ::FT_ObjectGroupManager_ptr
//FT_ObjectGroupManager_var::ptr (void)	const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_ObjectGroupManager_var::FT_ObjectGroupManager_var (const ::FT_ObjectGroupManager_var &p) // copy constructor
//  : TAO_Base_var (),
//    ptr_ (FT_ObjectGroupManager::_duplicate (p.ptr ()))
//{}
//
//ACE_INLINE
//FT_ObjectGroupManager_var::~FT_ObjectGroupManager_var	(void) // destructor
//{
//  CORBA::release (this->ptr_);
//}
//
//ACE_INLINE FT_ObjectGroupManager_var &
//FT_ObjectGroupManager_var::operator= (FT_ObjectGroupManager_ptr p)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_ObjectGroupManager_var &
//FT_ObjectGroupManager_var::operator= (const ::FT_ObjectGroupManager_var &p)
//{
//  if (this !=	&p)
//  {
//    CORBA::release (this->ptr_);
//    this->ptr_ = ::FT_ObjectGroupManager::_duplicate (p.ptr ());
//  }
//  return *this;
//}
//
//ACE_INLINE
//FT_ObjectGroupManager_var::operator const ::FT_ObjectGroupManager_ptr	&() const // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_ObjectGroupManager_var::operator ::FT_ObjectGroupManager_ptr &() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ObjectGroupManager_ptr
//FT_ObjectGroupManager_var::operator->	(void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ObjectGroupManager_ptr
//FT_ObjectGroupManager_var::in	(void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ObjectGroupManager_ptr &
//FT_ObjectGroupManager_var::inout (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ObjectGroupManager_ptr &
//FT_ObjectGroupManager_var::out (void)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_ObjectGroupManager::_nil ();
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ObjectGroupManager_ptr
//FT_ObjectGroupManager_var::_retn (void)
//{
//  // yield ownership of managed obj reference
//  ::FT_ObjectGroupManager_ptr	val = this->ptr_;
//  this->ptr_ = ::FT_ObjectGroupManager::_nil ();
//  return val;
//}
//
//


// *************************************************************
// Inline operations for class FT_ObjectGroupManager_out
// *************************************************************

//ACE_INLINE
//FT_ObjectGroupManager_out::FT_ObjectGroupManager_out (FT_ObjectGroupManager_ptr &p)
//  : ptr_ (p)
//{
//  this->ptr_ = ::FT_ObjectGroupManager::_nil ();
//}
//
//ACE_INLINE
//FT_ObjectGroupManager_out::FT_ObjectGroupManager_out (FT_ObjectGroupManager_var &p) // constructor from _var
//  : ptr_ (p.out ())
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_ObjectGroupManager::_nil ();
//}
//
//ACE_INLINE
//FT_ObjectGroupManager_out::FT_ObjectGroupManager_out (const ::FT_ObjectGroupManager_out &p) // copy constructor
//  : ptr_ (ACE_const_cast (FT_ObjectGroupManager_out &, p).ptr_)
//{}
//
//ACE_INLINE ::FT_ObjectGroupManager_out &
//FT_ObjectGroupManager_out::operator= (const ::FT_ObjectGroupManager_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_ObjectGroupManager_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE FT_ObjectGroupManager_out &
//FT_ObjectGroupManager_out::operator= (const ::FT_ObjectGroupManager_var &p)
//{
//  this->ptr_ = ::FT_ObjectGroupManager::_duplicate (p.ptr ());
//  return *this;
//}
//
//ACE_INLINE FT_ObjectGroupManager_out &
//FT_ObjectGroupManager_out::operator= (FT_ObjectGroupManager_ptr p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_ObjectGroupManager_out::operator ::FT_ObjectGroupManager_ptr &() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ObjectGroupManager_ptr &
//FT_ObjectGroupManager_out::ptr (void)	// ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ObjectGroupManager_ptr
//FT_ObjectGroupManager_out::operator->	(void)
//{
//  return this->ptr_;
//}
//


//ACE_INLINE
//FT_ReplicationManager::FT_ReplicationManager (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean	_tao_collocated) // constructor
//  : CORBA_Object (objref, _tao_servant, _tao_collocated)
//{}
//

// *************************************************************
// Inline operations for class FT_ReplicationManager_var
// *************************************************************

//ACE_INLINE
//FT_ReplicationManager_var::FT_ReplicationManager_var (void) // default constructor
//  : ptr_ (FT_ReplicationManager::_nil	())
//{}
//
//ACE_INLINE ::FT_ReplicationManager_ptr
//FT_ReplicationManager_var::ptr (void)	const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_ReplicationManager_var::FT_ReplicationManager_var (const ::FT_ReplicationManager_var &p) // copy constructor
//  : TAO_Base_var (),
//    ptr_ (FT_ReplicationManager::_duplicate (p.ptr ()))
//{}
//
//ACE_INLINE
//FT_ReplicationManager_var::~FT_ReplicationManager_var	(void) // destructor
//{
//  CORBA::release (this->ptr_);
//}
//
//ACE_INLINE FT_ReplicationManager_var &
//FT_ReplicationManager_var::operator= (FT_ReplicationManager_ptr p)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_ReplicationManager_var &
//FT_ReplicationManager_var::operator= (const ::FT_ReplicationManager_var &p)
//{
//  if (this !=	&p)
//  {
//    CORBA::release (this->ptr_);
//    this->ptr_ = ::FT_ReplicationManager::_duplicate (p.ptr ());
//  }
//  return *this;
//}
//
//ACE_INLINE
//FT_ReplicationManager_var::operator const ::FT_ReplicationManager_ptr	&() const // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_ReplicationManager_var::operator ::FT_ReplicationManager_ptr &() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ReplicationManager_ptr
//FT_ReplicationManager_var::operator->	(void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ReplicationManager_ptr
//FT_ReplicationManager_var::in	(void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ReplicationManager_ptr &
//FT_ReplicationManager_var::inout (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ReplicationManager_ptr &
//FT_ReplicationManager_var::out (void)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_ReplicationManager::_nil ();
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ReplicationManager_ptr
//FT_ReplicationManager_var::_retn (void)
//{
//  // yield ownership of managed obj reference
//  ::FT_ReplicationManager_ptr	val = this->ptr_;
//  this->ptr_ = ::FT_ReplicationManager::_nil ();
//  return val;
//}
//
//
//#endif /* end	#if !defined */
//
//
//#if !defined (_FT_REPLICATIONMANAGER___OUT_CI_)
//#define _FT_REPLICATIONMANAGER___OUT_CI_
//
//// *************************************************************
//// Inline operations for class FT_ReplicationManager_out
//// *************************************************************
//
//ACE_INLINE
//FT_ReplicationManager_out::FT_ReplicationManager_out (ReplicationManager_ptr &p)
//  : ptr_ (p)
//{
//  this->ptr_ = ::FT_ReplicationManager::_nil ();
//}
//
//ACE_INLINE
//FT_ReplicationManager_out::FT_ReplicationManager_out (FT_ReplicationManager_var &p) // constructor from _var
//  : ptr_ (p.out ())
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_ReplicationManager::_nil ();
//}
//
//ACE_INLINE
//FT_ReplicationManager_out::FT_ReplicationManager_out (const ::FT_ReplicationManager_out &p) // copy constructor
//  : ptr_ (ACE_const_cast (ReplicationManager_out &, p).ptr_)
//{}
//
//ACE_INLINE ::FT_ReplicationManager_out &
//FT_ReplicationManager_out::operator= (const ::FT_ReplicationManager_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_ReplicationManager_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE FT_ReplicationManager_out &
//FT_ReplicationManager_out::operator= (const ::FT_ReplicationManager_var &p)
//{
//  this->ptr_ = ::FT_ReplicationManager::_duplicate (p.ptr ());
//  return *this;
//}
//
//ACE_INLINE FT_ReplicationManager_out &
//FT_ReplicationManager_out::operator= (FT_ReplicationManager_ptr p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_ReplicationManager_out::operator ::FT_ReplicationManager_ptr &() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ReplicationManager_ptr &
//FT_ReplicationManager_out::ptr (void)	// ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_ReplicationManager_ptr
//FT_ReplicationManager_out::operator->	(void)
//{
//  return this->ptr_;
//}
//
//
//#endif /* end	#if !defined */
//
//

//ACE_INLINE
//FT_PullMonitorable::FT_PullMonitorable (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean _tao_collocated) // constructor
//  : CORBA_Object (objref, _tao_servant, _tao_collocated)
//{}
//


// *************************************************************
// Inline operations for class FT_PullMonitorable_var
// *************************************************************

//ACE_INLINE
//FT_PullMonitorable_var::FT_PullMonitorable_var (void)	// default constructor
//  : ptr_ (PullMonitorable::_nil ())
//{}
//
//ACE_INLINE ::FT_PullMonitorable_ptr
//FT_PullMonitorable_var::ptr (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_PullMonitorable_var::FT_PullMonitorable_var (const	::FT_PullMonitorable_var &p) //	copy constructor
//  : TAO_Base_var (),
//    ptr_ (FT_PullMonitorable::_duplicate (p.ptr ()))
//{}
//
//ACE_INLINE
//FT_PullMonitorable_var::~FT_PullMonitorable_var (void) // destructor
//{
//  CORBA::release (this->ptr_);
//}
//
//ACE_INLINE FT_PullMonitorable_var &
//FT_PullMonitorable_var::operator= (FT_PullMonitorable_ptr p)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_PullMonitorable_var &
//FT_PullMonitorable_var::operator= (const ::FT_PullMonitorable_var &p)
//{
//  if (this !=	&p)
//  {
//    CORBA::release (this->ptr_);
//    this->ptr_ = ::FT_PullMonitorable::_duplicate (p.ptr ());
//  }
//  return *this;
//}
//
//ACE_INLINE
//FT_PullMonitorable_var::operator const ::FT_PullMonitorable_ptr &() const // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_PullMonitorable_var::operator ::FT_PullMonitorable_ptr &()	// cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PullMonitorable_ptr
//FT_PullMonitorable_var::operator-> (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PullMonitorable_ptr
//FT_PullMonitorable_var::in (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PullMonitorable_ptr &
//FT_PullMonitorable_var::inout	(void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PullMonitorable_ptr &
//FT_PullMonitorable_var::out (void)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_PullMonitorable::_nil ();
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PullMonitorable_ptr
//FT_PullMonitorable_var::_retn	(void)
//{
//  // yield ownership of managed obj reference
//  ::FT_PullMonitorable_ptr val = this->ptr_;
//  this->ptr_ = ::FT_PullMonitorable::_nil ();
//  return val;
//}
//

// *************************************************************
// Inline operations for class FT_PullMonitorable_out
// *************************************************************
//
//ACE_INLINE
//FT_PullMonitorable_out::FT_PullMonitorable_out (FT_PullMonitorable_ptr &p)
//  : ptr_ (p)
//{
//  this->ptr_ = ::FT_PullMonitorable::_nil ();
//}
//
//ACE_INLINE
//FT_PullMonitorable_out::FT_PullMonitorable_out (FT_PullMonitorable_var &p) //	constructor from _var
//  : ptr_ (p.out ())
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_PullMonitorable::_nil ();
//}
//
//ACE_INLINE
//FT_PullMonitorable_out::FT_PullMonitorable_out (const	::FT_FT_PullMonitorable_out &p)	// copy	constructor
//  : ptr_ (ACE_const_cast (FT_PullMonitorable_out &, p).ptr_)
//{}
//
//ACE_INLINE ::FT_PullMonitorable_out &
//FT_PullMonitorable_out::operator= (const ::FT_PullMonitorable_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_PullMonitorable_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE FT_PullMonitorable_out &
//FT_PullMonitorable_out::operator= (const ::FT_PullMonitorable_var &p)
//{
//  this->ptr_ = ::FT_PullMonitorable::_duplicate (p.ptr ());
//  return *this;
//}
//
//ACE_INLINE FT_PullMonitorable_out &
//FT_PullMonitorable_out::operator= (PullMonitorable_ptr p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_PullMonitorable_out::operator ::FT_PullMonitorable_ptr &()	// cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PullMonitorable_ptr &
//FT_PullMonitorable_out::ptr (void) //	ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_PullMonitorable_ptr
//FT_PullMonitorable_out::operator-> (void)
//{
//  return this->ptr_;
//}
//


// *************************************************************
// Inline operations for class FT_State_var
// *************************************************************

//ACE_INLINE
//FT_State_var::FT_State_var (void) // default constructor
//  : ptr_ (0)
//{}
//
//ACE_INLINE
//FT_FT_State_var::FT_State_var	(FT_State *p)
//  : ptr_ (p)
//{}
//
//ACE_INLINE
//FT_State_var::FT_State_var (const ::FT_State_var &p) // copy constructor
//{
//  if (p.ptr_)
//    ACE_NEW (this->ptr_, ::FT_State (*p.ptr_));
//  else
//    this->ptr_ = 0;
//}
//
//// fixed-size	base types only
//ACE_INLINE
//FT_State_var::FT_State_var (const ::FT_State &p)
//{
//  ACE_NEW (this->ptr_, ::FT_State (p));
//}
//
//ACE_INLINE
//FT_State_var::~FT_State_var (void) //	destructor
//{
//  delete this->ptr_;
//}
//
//ACE_INLINE FT_State_var &
//FT_State_var::operator= (State *p)
//{
//  delete this->ptr_;
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_State_var &
//FT_State_var::operator= (const ::FT_State_var	&p) // deep copy
//{
//  if (this !=	&p)
//  {
//    delete this->ptr_;
//    ACE_NEW_RETURN (this->ptr_, ::FT_State (*p.ptr_),	*this);
//  }
//  return *this;
//}
//
//// fixed-size	types only
//ACE_INLINE ::FT_State_var &
//FT_State_var::operator= (const ::FT_State &p)
//{
//  if (this->ptr_ != &p)
//  {
//    delete this->ptr_;
//    ACE_NEW_RETURN (this->ptr_, ::FT_State (p), *this);
//  }
//  return *this;
//}
//
//ACE_INLINE const ::FT_State *
//FT_State_var::operator-> (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_State	*
//FT_State_var::operator-> (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_State_var::operator const ::FT_State &() const // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_State_var::operator ::FT_State &()	// cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE
//FT_State_var::operator ::FT_State &()	const // cast
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE CORBA::Octet &
//FT_State_var::operator[] (CORBA::ULong index)
//{
//  return this->ptr_->operator[] (index);
//}
//
//ACE_INLINE const ::FT_State &
//FT_State_var::in (void) const
//{
//  return *this->ptr_;
//}
//
//ACE_INLINE ::FT_State	&
//FT_State_var::inout (void)
//{
//  return *this->ptr_;
//}
//
//// mapping for variable size
//ACE_INLINE ::FT_State	*&
//FT_State_var::out (void)
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_State	*
//FT_State_var::_retn (void)
//{
//  ::FT_State *tmp = this->ptr_;
//  this->ptr_ = 0;
//  return tmp;
//}
//
//ACE_INLINE ::FT_State	*
//FT_State_var::ptr (void) const
//{
//  return this->ptr_;
//}
//
// *************************************************************
// Inline operations for class FT_State_out
// *************************************************************

//ACE_INLINE
//FT_State_out::FT_State_out (FT_State *&p)
//  : ptr_ (p)
//{
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_State_out::FT_State_out (FT_State_var &p) // constructor from _var
//  : ptr_ (p.out ())
//{
//  delete this->ptr_;
//  this->ptr_ = 0;
//}
//
//ACE_INLINE
//FT_State_out::FT_State_out (const ::FT_State_out &p) // copy constructor
//  : ptr_ (ACE_const_cast (FT_State_out&, p).ptr_)
//{}
//
//ACE_INLINE ::FT_State_out &
//FT_State_out::operator= (const ::FT_State_out	&p)
//{
//  this->ptr_ = ACE_const_cast	(FT_State_out&,	p).ptr_;
//  return *this;
//}
//
//ACE_INLINE ::FT_State_out &
//FT_State_out::operator= (FT_State *p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_State_out::operator ::FT_State *&() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_State	*&
//FT_State_out::ptr (void) // ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_State	*
//FT_State_out::operator-> (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE CORBA::Octet &
//FT_State_out::operator[] (CORBA::ULong index)
//{
//  return this->ptr_->operator[] (index);
//}
//


//ACE_INLINE
//FT_Checkpointable::FT_Checkpointable (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean	_tao_collocated) // constructor
//  : CORBA_Object (objref, _tao_servant, _tao_collocated)
//{}
//

// *************************************************************
// Inline operations for class FT_Checkpointable_var
// *************************************************************

//ACE_INLINE
//FT_Checkpointable_var::FT_Checkpointable_var (void) // default constructor
//  : ptr_ (FT_Checkpointable::_nil ())
//{}
//
//ACE_INLINE ::FT_Checkpointable_ptr
//FT_Checkpointable_var::ptr (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_Checkpointable_var::FT_Checkpointable_var (const ::FT_Checkpointable_var &p) // copy constructor
//  : TAO_Base_var (),
//    ptr_ (FT_Checkpointable::_duplicate (p.ptr ()))
//{}
//
//ACE_INLINE
//FT_Checkpointable_var::~FT_Checkpointable_var	(void) // destructor
//{
//  CORBA::release (this->ptr_);
//}
//
//ACE_INLINE FT_Checkpointable_var &
//FT_Checkpointable_var::operator= (FT_Checkpointable_ptr p)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_Checkpointable_var &
//FT_Checkpointable_var::operator= (const ::FT_FT_Checkpointable_var &p)
//{
//  if (this !=	&p)
//  {
//    CORBA::release (this->ptr_);
//    this->ptr_ = ::FT_Checkpointable::_duplicate (p.ptr ());
//  }
//  return *this;
//}
//
//ACE_INLINE
//FT_Checkpointable_var::operator const	::FT_Checkpointable_ptr	&() const // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_Checkpointable_var::operator ::FT_Checkpointable_ptr &() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Checkpointable_ptr
//FT_Checkpointable_var::operator-> (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Checkpointable_ptr
//FT_Checkpointable_var::in (void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Checkpointable_ptr &
//FT_Checkpointable_var::inout (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Checkpointable_ptr &
//FT_Checkpointable_var::out (void)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_Checkpointable::_nil ();
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Checkpointable_ptr
//FT_Checkpointable_var::_retn (void)
//{
//  // yield ownership of managed obj reference
//  ::FT_Checkpointable_ptr val	= this->ptr_;
//  this->ptr_ = ::FT_Checkpointable::_nil ();
//  return val;
//}
//


// *************************************************************
// Inline operations for class FT_Checkpointable_out
// *************************************************************

//ACE_INLINE
//FT_Checkpointable_out::FT_Checkpointable_out (FT_Checkpointable_ptr &p)
//  : ptr_ (p)
//{
//  this->ptr_ = ::FT_Checkpointable::_nil ();
//}
//
//ACE_INLINE
//FT_Checkpointable_out::FT_Checkpointable_out (FT_Checkpointable_var &p) // constructor from _var
//  : ptr_ (p.out ())
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_Checkpointable::_nil ();
//}
//
//ACE_INLINE
//FT_Checkpointable_out::FT_Checkpointable_out (const ::FT_Checkpointable_out &p) // copy constructor
//  : ptr_ (ACE_const_cast (Checkpointable_out &, p).ptr_)
//{}
//
//ACE_INLINE ::FT_Checkpointable_out &
//FT_Checkpointable_out::operator= (const ::FT_Checkpointable_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_Checkpointable_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE FT_Checkpointable_out &
//FT_Checkpointable_out::operator= (const ::FT_Checkpointable_var &p)
//{
//  this->ptr_ = ::FT_Checkpointable::_duplicate (p.ptr	());
//  return *this;
//}
//
//ACE_INLINE FT_Checkpointable_out &
//FT_Checkpointable_out::operator= (FT_Checkpointable_ptr p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_Checkpointable_out::operator ::FT_Checkpointable_ptr &() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Checkpointable_ptr &
//FT_Checkpointable_out::ptr (void) // ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Checkpointable_ptr
//FT_Checkpointable_out::operator-> (void)
//{
//  return this->ptr_;
//}
//

//
//ACE_INLINE
//FT_Updateable::FT_Updateable (TAO_Stub *objref, TAO_ServantBase *_tao_servant, CORBA::Boolean	_tao_collocated) // constructor
//  : CORBA_Object (objref, _tao_servant, _tao_collocated)
//{}
//


// *************************************************************
// Inline operations for class FT_Updateable_var
// *************************************************************
//
//ACE_INLINE
//FT_Updateable_var::FT_Updateable_var (void) // default constructor
//  : ptr_ (FT_Updateable::_nil	())
//{}
//
//ACE_INLINE ::FT_Updateable_ptr
//FT_Updateable_var::ptr (void)	const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_Updateable_var::FT_Updateable_var (const ::FT_Updateable_var &p) // copy constructor
//  : TAO_Base_var (),
//    ptr_ (FT_Updateable::_duplicate (p.ptr ()))
//{}
//
//ACE_INLINE
//FT_Updateable_var::~FT_Updateable_var	(void) // destructor
//{
//  CORBA::release (this->ptr_);
//}
//
//ACE_INLINE FT_Updateable_var &
//FT_Updateable_var::operator= (FT_Updateable_ptr p)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE FT_Updateable_var &
//FT_Updateable_var::operator= (const ::FT_Updateable_var &p)
//{
//  if (this !=	&p)
//  {
//    CORBA::release (this->ptr_);
//    this->ptr_ = ::FT_Updateable::_duplicate (p.ptr ());
//  }
//  return *this;
//}
//
//ACE_INLINE
//FT_Updateable_var::operator const ::FT_Updateable_ptr	&() const // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE
//FT_Updateable_var::operator ::FT_Updateable_ptr &() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Updateable_ptr
//FT_Updateable_var::operator->	(void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Updateable_ptr
//FT_Updateable_var::in	(void) const
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Updateable_ptr &
//FT_Updateable_var::inout (void)
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Updateable_ptr &
//FT_Updateable_var::out (void)
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_Updateable::_nil ();
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Updateable_ptr
//FT_Updateable_var::_retn (void)
//{
//  // yield ownership of managed obj reference
//  ::FT_Updateable_ptr	val = this->ptr_;
//  this->ptr_ = ::FT_Updateable::_nil ();
//  return val;
//}
//

// *************************************************************
// Inline operations for class FT_Updateable_out
// *************************************************************
//
//ACE_INLINE
//FT_Updateable_out::FT_Updateable_out (FT_Updateable_ptr &p)
//  : ptr_ (p)
//{
//  this->ptr_ = ::FT_Updateable::_nil ();
//}
//
//ACE_INLINE
//FT_Updateable_out::FT_Updateable_out (FT_Updateable_var &p) // constructor from _var
//  : ptr_ (p.out ())
//{
//  CORBA::release (this->ptr_);
//  this->ptr_ = ::FT_Updateable::_nil ();
//}
//
//ACE_INLINE
//FT_Updateable_out::FT_Updateable_out (const ::FT_Updateable_out &p) // copy constructor
//  : ptr_ (ACE_const_cast (FT_Updateable_out &, p).ptr_)
//{}
//
//ACE_INLINE ::FT_Updateable_out &
//FT_Updateable_out::operator= (const ::FT_Updateable_out &p)
//{
//  this->ptr_ = ACE_const_cast	(FT_Updateable_out&, p).ptr_;
//  return *this;
//}
//
//ACE_INLINE FT_Updateable_out &
//FT_Updateable_out::operator= (const ::FT_Updateable_var &p)
//{
//  this->ptr_ = ::FT_Updateable::_duplicate (p.ptr ());
//  return *this;
//}
//
//ACE_INLINE FT_Updateable_out &
//FT_Updateable_out::operator= (FT_Updateable_ptr p)
//{
//  this->ptr_ = p;
//  return *this;
//}
//
//ACE_INLINE
//FT_Updateable_out::operator ::FT_Updateable_ptr &() // cast
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Updateable_ptr &
//FT_Updateable_out::ptr (void)	// ptr
//{
//  return this->ptr_;
//}
//
//ACE_INLINE ::FT_Updateable_ptr
//FT_Updateable_out::operator->	(void)
//{
//  return this->ptr_;
//}
//

//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_FactoryInfo &_tao_aggregate)
//{
//  if (
//    (strm << _tao_aggregate.factory.in ()) &&
//    (strm << _tao_aggregate.the_location) &&
//    (strm << _tao_aggregate.the_criteria)
//  )
//    return 1;
//  else
//    return 0;
//
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, FT_FactoryInfo &_tao_aggregate)
//{
//  if (
//    (strm >> _tao_aggregate.factory.out ()) &&
//    (strm >> _tao_aggregate.the_location) &&
//    (strm >> _tao_aggregate.the_criteria)
//  )
//    return 1;
//  else
//    return 0;
//
//}
//
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_FaultMonitoringIntervalAndTimeoutValue &_tao_aggregate)
//{
//  if (
//    (strm << _tao_aggregate.monitoring_interval) &&
//    (strm << _tao_aggregate.timeout)
//  )
//    return 1;
//  else
//    return 0;
//
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, FT_FaultMonitoringIntervalAndTimeoutValue &_tao_aggregate)
//{
//  if (
//    (strm >> _tao_aggregate.monitoring_interval) &&
//    (strm >> _tao_aggregate.timeout)
//  )
//    return 1;
//  else
//    return 0;
//
//}
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_InterfaceNotFound	&_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_InterfaceNotFound&)
//{
//  return 1;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_InterfaceNotFound	&_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_ObjectGroupNotFound &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_ObjectGroupNotFound&)
//{
//  return 1;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_ObjectGroupNotFound &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_MemberNotFound &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_MemberNotFound&)
//{
//  return 1;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_MemberNotFound &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_ObjectNotFound &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_ObjectNotFound&)
//{
//  return 1;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_ObjectNotFound &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_MemberAlreadyPresent &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_MemberAlreadyPresent&)
//{
//  return 1;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_MemberAlreadyPresent &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_BadReplicationStyle &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_BadReplicationStyle&)
//{
//  return 1;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_BadReplicationStyle &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_ObjectNotCreated &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_ObjectNotCreated&)
//{
//  return 1;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_ObjectNotCreated &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_ObjectNotAdded &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_ObjectNotAdded&)
//{
//  return 1;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_ObjectNotAdded &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_PrimaryNotSet &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_PrimaryNotSet&)
//{
//  return 1;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_PrimaryNotSet &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_UnsupportedProperty &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//  {
//    // now marshal the members (if any)
//    if (
//	(strm << _tao_aggregate.nam) &&
//	(strm << _tao_aggregate.val)
//    )
//	return 1;
//    else
//	return 0;
//  }
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,FT_UnsupportedProperty &_tao_aggregate)
//{
//  // now marshal the members
//  if (
//    (strm >> _tao_aggregate.nam) &&
//    (strm >> _tao_aggregate.val)
//  )
//    return 1;
//  else
//    return 0;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_UnsupportedProperty &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_InvalidProperty &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//  {
//    // now marshal the members (if any)
//    if (
//	(strm << _tao_aggregate.nam) &&
//	(strm << _tao_aggregate.val)
//    )
//	return 1;
//    else
//	return 0;
//  }
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,FT_InvalidProperty &_tao_aggregate)
//{
//  // now marshal the members
//  if (
//    (strm >> _tao_aggregate.nam) &&
//    (strm >> _tao_aggregate.val)
//  )
//    return 1;
//  else
//    return 0;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_InvalidProperty &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_NoFactory	&_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//  {
//    // now marshal the members (if any)
//    if (
//	(strm << _tao_aggregate.the_location) &&
//	(strm << _tao_aggregate.type_id.in ())
//    )
//	return 1;
//    else
//	return 0;
//  }
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,FT_NoFactory	&_tao_aggregate)
//{
//  // now marshal the members
//  if (
//    (strm >> _tao_aggregate.the_location) &&
//    (strm >> _tao_aggregate.type_id.out ())
//  )
//    return 1;
//  else
//    return 0;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_NoFactory	&_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_InvalidCriteria &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//  {
//    // now marshal the members (if any)
//    if (
//	(strm << _tao_aggregate.invalid_criteria)
//    )
//	return 1;
//    else
//	return 0;
//  }
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,FT_InvalidCriteria &_tao_aggregate)
//{
//  // now marshal the members
//  if (
//    (strm >> _tao_aggregate.invalid_criteria)
//  )
//    return 1;
//  else
//    return 0;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_InvalidCriteria &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_CannotMeetCriteria &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//  {
//    // now marshal the members (if any)
//    if (
//	(strm << _tao_aggregate.unmet_criteria)
//    )
//	return 1;
//    else
//	return 0;
//  }
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,FT_CannotMeetCriteria &_tao_aggregate)
//{
//  // now marshal the members
//  if (
//    (strm >> _tao_aggregate.unmet_criteria)
//  )
//    return 1;
//  else
//    return 0;
//}
//
//#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
//
//ACE_INLINE
//ostream & operator<< (ostream	&os, const FT_CannotMeetCriteria &_tao_aggregate)
//{
//  CORBA::TypeCode_ptr	tc = _tao_aggregate._type ();
//  if (tc)
//    {
//	os << tc->name () << " (";
//    }
//  os << _tao_aggregate._id ();
//  if (tc)
//    {
//	os << ")";
//    }
//  return os;
//}
//
//#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
//
//
//ACE_INLINE CORBA::Boolean
//operator<< (
//    TAO_OutputCDR &strm,
//    const FT_PropertyManager_ptr _tao_objref
//  )
//{
//  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
//  return (strm << _tao_corba_obj);
//}
//
//ACE_INLINE CORBA::Boolean
//operator>> (
//    TAO_InputCDR &strm,
//    FT_PropertyManager_ptr &_tao_objref
//  )
//{
//  ACE_TRY_NEW_ENV
//  {
//    CORBA::Object_var	obj;
//    if ((strm	>> obj.inout ()) == 0)
//	return 0;
//    // narrow	to the right type
//    _tao_objref =
//	FT_PropertyManager::_unchecked_narrow (
//	    obj.in (),
//	    ACE_TRY_ENV
//	  );
//    ACE_TRY_CHECK;
//    return 1;
//  }
//  ACE_CATCHANY
//  {
//    // do nothing
//  }
//  ACE_ENDTRY;
//  return 0;
//}
//
//
//ACE_INLINE CORBA::Boolean
//operator<< (
//    TAO_OutputCDR &strm,
//    const FT_ObjectGroupManager_ptr _tao_objref
//  )
//{
//  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
//  return (strm << _tao_corba_obj);
//}
//
//ACE_INLINE CORBA::Boolean
//operator>> (
//    TAO_InputCDR &strm,
//    FT_ObjectGroupManager_ptr	&_tao_objref
//  )
//{
//  ACE_TRY_NEW_ENV
//  {
//    CORBA::Object_var	obj;
//    if ((strm	>> obj.inout ()) == 0)
//	return 0;
//    // narrow	to the right type
//    _tao_objref =
//	FT_ObjectGroupManager::_unchecked_narrow (
//	    obj.in (),
//	    ACE_TRY_ENV
//	  );
//    ACE_TRY_CHECK;
//    return 1;
//  }
//  ACE_CATCHANY
//  {
//    // do nothing
//  }
//  ACE_ENDTRY;
//  return 0;
//}
//
//
//ACE_INLINE CORBA::Boolean
//operator<< (
//    TAO_OutputCDR &strm,
//    const FT_GenericFactory_ptr _tao_objref
//  )
//{
//  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
//  return (strm << _tao_corba_obj);
//}
//
//ACE_INLINE CORBA::Boolean
//operator>> (
//    TAO_InputCDR &strm,
//    FT_GenericFactory_ptr &_tao_objref
//  )
//{
//  ACE_TRY_NEW_ENV
//  {
//    CORBA::Object_var	obj;
//    if ((strm	>> obj.inout ()) == 0)
//	return 0;
//    // narrow	to the right type
//    _tao_objref =
//	FT_GenericFactory::_unchecked_narrow (
//	    obj.in (),
//	    ACE_TRY_ENV
//	  );
//    ACE_TRY_CHECK;
//    return 1;
//  }
//  ACE_CATCHANY
//  {
//    // do nothing
//  }
//  ACE_ENDTRY;
//  return 0;
//}
//
//
//ACE_INLINE CORBA::Boolean
//operator<< (
//    TAO_OutputCDR &strm,
//    const FT_ReplicationManager_ptr _tao_objref
//  )
//{
//  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
//  return (strm << _tao_corba_obj);
//}
//
//ACE_INLINE CORBA::Boolean
//operator>> (
//    TAO_InputCDR &strm,
//    FT_ReplicationManager_ptr	&_tao_objref
//  )
//{
//  ACE_TRY_NEW_ENV
//  {
//    CORBA::Object_var	obj;
//    if ((strm	>> obj.inout ()) == 0)
//	return 0;
//    // narrow	to the right type
//    _tao_objref =
//	FT_ReplicationManager::_unchecked_narrow (
//	    obj.in (),
//	    ACE_TRY_ENV
//	  );
//    ACE_TRY_CHECK;
//    return 1;
//  }
//  ACE_CATCHANY
//  {
//    // do nothing
//  }
//  ACE_ENDTRY;
//  return 0;
//}
//
//
//ACE_INLINE CORBA::Boolean
//operator<< (
//    TAO_OutputCDR &strm,
//    const FT_PullMonitorable_ptr _tao_objref
//  )
//{
//  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
//  return (strm << _tao_corba_obj);
//}
//
//ACE_INLINE CORBA::Boolean
//operator>> (
//    TAO_InputCDR &strm,
//    FT_PullMonitorable_ptr &_tao_objref
//  )
//{
//  ACE_TRY_NEW_ENV
//  {
//    CORBA::Object_var	obj;
//    if ((strm	>> obj.inout ()) == 0)
//	return 0;
//    // narrow	to the right type
//    _tao_objref =
//	FT_PullMonitorable::_unchecked_narrow (
//	    obj.in (),
//	    ACE_TRY_ENV
//	  );
//    ACE_TRY_CHECK;
//    return 1;
//  }
//  ACE_CATCHANY
//  {
//    // do nothing
//  }
//  ACE_ENDTRY;
//  return 0;
//}
//
//
//ACE_INLINE CORBA::Boolean
//operator<< (
//    TAO_OutputCDR &strm,
//    const FT_FaultNotifier_ptr _tao_objref
//  )
//{
//  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
//  return (strm << _tao_corba_obj);
//}
//
//ACE_INLINE CORBA::Boolean
//operator>> (
//    TAO_InputCDR &strm,
//    FT_FaultNotifier_ptr &_tao_objref
//  )
//{
//  ACE_TRY_NEW_ENV
//  {
//    CORBA::Object_var	obj;
//    if ((strm	>> obj.inout ()) == 0)
//	return 0;
//    // narrow	to the right type
//    _tao_objref =
//	FT_FaultNotifier::_unchecked_narrow (
//	    obj.in (),
//	    ACE_TRY_ENV
//	  );
//    ACE_TRY_CHECK;
//    return 1;
//  }
//  ACE_CATCHANY
//  {
//    // do nothing
//  }
//  ACE_ENDTRY;
//  return 0;
//}
//
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_NoStateAvailable &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_NoStateAvailable&)
//{
//  return 1;
//}
//
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_InvalidState &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_InvalidState&)
//{
//  return 1;
//}
//
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_NoUpdateAvailable	&_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_NoUpdateAvailable&)
//{
//  return 1;
//}
//
//
//ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const FT_InvalidUpdate &_tao_aggregate)
//{
//  // first marshal the repository ID
//  if (strm <<	_tao_aggregate._id ())
//    return 1;
//  else
//    return 0;
//}
//
//ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,FT_InvalidUpdate&)
//{
//  return 1;
//}
//
//
//ACE_INLINE CORBA::Boolean
//operator<< (
//    TAO_OutputCDR &strm,
//    const FT_Checkpointable_ptr _tao_objref
//  )
//{
//  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
//  return (strm << _tao_corba_obj);
//}
//
//ACE_INLINE CORBA::Boolean
//operator>> (
//    TAO_InputCDR &strm,
//    FT_Checkpointable_ptr &_tao_objref
//  )
//{
//  ACE_TRY_NEW_ENV
//  {
//    CORBA::Object_var	obj;
//    if ((strm	>> obj.inout ()) == 0)
//	return 0;
//    // narrow	to the right type
//    _tao_objref =
//	FT_Checkpointable::_unchecked_narrow (
//	    obj.in (),
//	    ACE_TRY_ENV
//	  );
//    ACE_TRY_CHECK;
//    return 1;
//  }
//  ACE_CATCHANY
//  {
//    // do nothing
//  }
//  ACE_ENDTRY;
//  return 0;
//}
//
//
//ACE_INLINE CORBA::Boolean
//operator<< (
//    TAO_OutputCDR &strm,
//    const FT_Updateable_ptr _tao_objref
//  )
//{
//  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
//  return (strm << _tao_corba_obj);
//}
//
//ACE_INLINE CORBA::Boolean
//operator>> (
//    TAO_InputCDR &strm,
//    FT_Updateable_ptr	&_tao_objref
//  )
//{
//  ACE_TRY_NEW_ENV
//  {
//    CORBA::Object_var	obj;
//    if ((strm	>> obj.inout ()) == 0)
//	return 0;
//    // narrow	to the right type
//    _tao_objref =
//	FT_Updateable::_unchecked_narrow (
//	    obj.in (),
//	    ACE_TRY_ENV
//	  );
//    ACE_TRY_CHECK;
//    return 1;
//  }
//  ACE_CATCHANY
//  {
//    // do nothing
//  }
//  ACE_ENDTRY;
//  return 0;
//}
//

#endif /*TAO_HAS_FT_CORBA == 1 */
