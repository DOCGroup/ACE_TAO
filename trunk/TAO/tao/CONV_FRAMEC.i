// $Id$

// *************************************************************
// Inline operations for class CONV_FRAME::CodeSetComponent_var
// *************************************************************

ACE_INLINE
CONV_FRAME::CodeSetComponent_var::CodeSetComponent_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
CONV_FRAME::CodeSetComponent_var::CodeSetComponent_var (CONV_FRAME::CodeSetComponent *p)
  : ptr_ (p)
{}

ACE_INLINE
CONV_FRAME::CodeSetComponent_var::CodeSetComponent_var (const CONV_FRAME::CodeSetComponent_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, CONV_FRAME::CodeSetComponent(*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent_var::~CodeSetComponent_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent_var &
CONV_FRAME::CodeSetComponent_var::operator= (CONV_FRAME::CodeSetComponent *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CONV_FRAME::CodeSetComponent_var &
CONV_FRAME::CodeSetComponent_var::operator= (const CONV_FRAME::CodeSetComponent_var &p)
{
  if (this != &p)
    {
      if (p.ptr_ == 0)
        {
          delete this->ptr_;
          this->ptr_ = 0;
        }
      else
        {
          CONV_FRAME::CodeSetComponent *deep_copy = 
                new CONV_FRAME::CodeSetComponent (*p.ptr_);
          
          if (deep_copy != 0)
            {
              CONV_FRAME::CodeSetComponent *tmp = deep_copy;
              deep_copy = this->ptr_;
              this->ptr_ = tmp;
              delete deep_copy;
            }
        }
    }
  
  return *this;
}

ACE_INLINE const CONV_FRAME::CodeSetComponent *
CONV_FRAME::CodeSetComponent_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent *
CONV_FRAME::CodeSetComponent_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent_var::operator const CONV_FRAME::CodeSetComponent &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent_var::operator CONV_FRAME::CodeSetComponent &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent_var::operator CONV_FRAME::CodeSetComponent &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE const CONV_FRAME::CodeSetComponent &
CONV_FRAME::CodeSetComponent_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent &
CONV_FRAME::CodeSetComponent_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE CONV_FRAME::CodeSetComponent *&
CONV_FRAME::CodeSetComponent_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent *
CONV_FRAME::CodeSetComponent_var::_retn (void)
{
  CONV_FRAME::CodeSetComponent *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE CONV_FRAME::CodeSetComponent *
CONV_FRAME::CodeSetComponent_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class CONV_FRAME::CodeSetComponent_out
// *************************************************************

ACE_INLINE
CONV_FRAME::CodeSetComponent_out::CodeSetComponent_out (CONV_FRAME::CodeSetComponent *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent_out::CodeSetComponent_out (CONV_FRAME::CodeSetComponent_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent_out::CodeSetComponent_out (const CONV_FRAME::CodeSetComponent_out &p) // copy constructor
  : ptr_ (ACE_const_cast (CONV_FRAME::CodeSetComponent_out&,p).ptr_)
{}

ACE_INLINE CONV_FRAME::CodeSetComponent_out &
CONV_FRAME::CodeSetComponent_out::operator= (const CONV_FRAME::CodeSetComponent_out &p)
{
  this->ptr_ = ACE_const_cast (CONV_FRAME::CodeSetComponent_out&,p).ptr_;
  return *this;
}

ACE_INLINE CONV_FRAME::CodeSetComponent_out &
CONV_FRAME::CodeSetComponent_out::operator= (CONV_FRAME::CodeSetComponent *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent_out::operator CONV_FRAME::CodeSetComponent *&() // cast
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent *&
CONV_FRAME::CodeSetComponent_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent *
CONV_FRAME::CodeSetComponent_out::operator-> (void)
{
  return this->ptr_;
}


#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

#if !defined (__TAO_UNBOUNDED_SEQUENCE_CONV_FRAME_CODESETCOMPONENT__TAO_SEQ_CODESETID_CI_)
#define __TAO_UNBOUNDED_SEQUENCE_CONV_FRAME_CODESETCOMPONENT__TAO_SEQ_CODESETID_CI_

  // = Static operations.
  ACE_INLINE CORBA::ULong *
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::allocbuf (CORBA::ULong size)
  // Allocate storage for the sequence.
  {
    CORBA::ULong *retval = 0;
    ACE_NEW_RETURN (retval, CORBA::ULong[size], 0);
    return retval;
  }

  ACE_INLINE void CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::freebuf (CORBA::ULong *buffer)
  // Free the sequence.
  {
    delete [] buffer;
  }

  ACE_INLINE
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId (void) // Default constructor.
  {
  }

  ACE_INLINE
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId (CORBA::ULong maximum) // Constructor using a maximum length value.
    : TAO_Unbounded_Base_Sequence (maximum, CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::allocbuf (maximum))
  {
  }

  ACE_INLINE
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId (CORBA::ULong maximum,
    CORBA::ULong length,
    CORBA::ULong *data,
    CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, data, release)
  {
  }

  ACE_INLINE
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId (const CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId &rhs)
  // Copy constructor.
    : TAO_Unbounded_Base_Sequence (rhs)
  {
    CORBA::ULong *tmp1 = CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::allocbuf (this->maximum_);
    CORBA::ULong * const tmp2 = ACE_reinterpret_cast (CORBA::ULong * ACE_CAST_CONST, rhs.buffer_);

    for (CORBA::ULong i = 0; i < this->length_; ++i)
      tmp1[i] = tmp2[i];

    this->buffer_ = tmp1;
  }

  ACE_INLINE CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId &
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::operator= (const CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId &rhs)
  // Assignment operator.
  {
    if (this == &rhs)
      return *this;

    if (this->release_)
    {
      if (this->maximum_ < rhs.maximum_)
      {
        // free the old buffer
        CORBA::ULong *tmp = ACE_reinterpret_cast (CORBA::ULong *, this->buffer_);
        CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::freebuf (tmp);
        this->buffer_ = CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::allocbuf (rhs.maximum_);
      }
    }
    else
      this->buffer_ = CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::allocbuf (rhs.maximum_);

    TAO_Unbounded_Base_Sequence::operator= (rhs);

    CORBA::ULong *tmp1 = ACE_reinterpret_cast (CORBA::ULong *, this->buffer_);
    CORBA::ULong * const tmp2 = ACE_reinterpret_cast (CORBA::ULong * ACE_CAST_CONST, rhs.buffer_);

    for (CORBA::ULong i = 0; i < this->length_; ++i)
      tmp1[i] = tmp2[i];

    return *this;
  }

  // = Accessors.
  ACE_INLINE CORBA::ULong &
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::operator[] (CORBA::ULong i)
  // operator []
  {
    ACE_ASSERT (i < this->maximum_);
    CORBA::ULong* tmp = ACE_reinterpret_cast(CORBA::ULong*,this->buffer_);
    return tmp[i];
  }

  ACE_INLINE const CORBA::ULong &
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::operator[] (CORBA::ULong i) const
  // operator []
  {
    ACE_ASSERT (i < this->maximum_);
    CORBA::ULong * const tmp = ACE_reinterpret_cast (CORBA::ULong* ACE_CAST_CONST, this->buffer_);
    return tmp[i];
  }

  // Implement the TAO_Base_Sequence methods (see Sequence.h)

  ACE_INLINE CORBA::ULong *
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::get_buffer (CORBA::Boolean orphan)
  {
    CORBA::ULong *result = 0;
    if (orphan == 0)
    {
      // We retain ownership.
      if (this->buffer_ == 0)
      {
        result = CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::allocbuf (this->length_);
        this->buffer_ = result;
        this->release_ = 1;
      }
      else
      {
        result = ACE_reinterpret_cast (CORBA::ULong*, this->buffer_);
      }
    }
    else // if (orphan == 1)
    {
      if (this->release_ != 0)
      {
        // We set the state back to default and relinquish
        // ownership.
        result = ACE_reinterpret_cast(CORBA::ULong*,this->buffer_);
        this->maximum_ = 0;
        this->length_ = 0;
        this->buffer_ = 0;
        this->release_ = 0;
      }
    }
    return result;
  }

  ACE_INLINE const CORBA::ULong *
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::get_buffer (void) const
  {
    return ACE_reinterpret_cast(const CORBA::ULong * ACE_CAST_CONST, this->buffer_);
  }

  ACE_INLINE void
  CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::replace (CORBA::ULong max,
  CORBA::ULong length,
  CORBA::ULong *data,
  CORBA::Boolean release)
  {
    this->maximum_ = max;
    this->length_ = length;
    if (this->buffer_ && this->release_ == 1)
    {
      CORBA::ULong *tmp = ACE_reinterpret_cast(CORBA::ULong*,this->buffer_);
      CONV_FRAME::CodeSetComponent::_TAO_Unbounded_Sequence_CONV_FRAME_CodeSetComponent__tao_seq_CodeSetId::freebuf (tmp);
    }
    this->buffer_ = data;
    this->release_ = release;
  }

#endif /* end #if !defined */


#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

#if !defined (_CONV_FRAME_CODESETCOMPONENT__TAO_SEQ_CODESETID_CI_)
#define _CONV_FRAME_CODESETCOMPONENT__TAO_SEQ_CODESETID_CI_

// *************************************************************
// Inline operations for class CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var
// *************************************************************

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::_tao_seq_CodeSetId_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::_tao_seq_CodeSetId_var (CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *p)
  : ptr_ (p)
{}

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::_tao_seq_CodeSetId_var (const CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId(*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::~_tao_seq_CodeSetId_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var &
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::operator= (CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var &
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::operator= (const CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var &p) // deep copy
{
  if (this != &p)
    {
      if (p.ptr_ == 0)
        {
          delete this->ptr_;
          this->ptr_ = 0;
        }
      else
        {
          CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *deep_copy = 
            new CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId (*p.ptr_);
          
          if (deep_copy != 0)
            {
              CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *tmp = 
                deep_copy;
              deep_copy = this->ptr_;
              this->ptr_ = tmp;
              delete deep_copy;
            }
        }
    }
  
  return *this;
}

ACE_INLINE const CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::operator const CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::operator CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::operator CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetId &
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

ACE_INLINE const CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId &
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId &
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *&
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::_retn (void)
{
  CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out
// *************************************************************

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out::_tao_seq_CodeSetId_out (CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out::_tao_seq_CodeSetId_out (CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out::_tao_seq_CodeSetId_out (const CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out &p) // copy constructor
  : ptr_ (ACE_const_cast (CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out&,p).ptr_)
{}

ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out &
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out::operator= (const CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out &p)
{
  this->ptr_ = ACE_const_cast (CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out&,p).ptr_;
  return *this;
}

ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out &
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out::operator= (CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out::operator CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *&() // cast
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *&
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId *
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetId &
CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId_out::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}


#endif /* end #if !defined */

// *************************************************************
// Inline operations for class CONV_FRAME::CodeSetComponentInfo_var
// *************************************************************

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_var::CodeSetComponentInfo_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_var::CodeSetComponentInfo_var (CONV_FRAME::CodeSetComponentInfo *p)
  : ptr_ (p)
{}

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_var::CodeSetComponentInfo_var (const CONV_FRAME::CodeSetComponentInfo_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, CONV_FRAME::CodeSetComponentInfo(*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_var::~CodeSetComponentInfo_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponentInfo_var &
CONV_FRAME::CodeSetComponentInfo_var::operator= (CONV_FRAME::CodeSetComponentInfo *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CONV_FRAME::CodeSetComponentInfo_var &
CONV_FRAME::CodeSetComponentInfo_var::operator= (const CONV_FRAME::CodeSetComponentInfo_var &p)
{
  if (this != &p)
    {
      if (p.ptr_ == 0)
        {
          delete this->ptr_;
          this->ptr_ = 0;
        }
      else
        {
          CONV_FRAME::CodeSetComponentInfo *deep_copy = 
            new CONV_FRAME::CodeSetComponentInfo (*p.ptr_);
          
          if (deep_copy != 0)
            {
              CONV_FRAME::CodeSetComponentInfo *tmp = deep_copy;
              deep_copy = this->ptr_;
              this->ptr_ = tmp;
              delete deep_copy;
            }
        }
    }
  
  return *this;
}

ACE_INLINE const CONV_FRAME::CodeSetComponentInfo *
CONV_FRAME::CodeSetComponentInfo_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponentInfo *
CONV_FRAME::CodeSetComponentInfo_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_var::operator const CONV_FRAME::CodeSetComponentInfo &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_var::operator CONV_FRAME::CodeSetComponentInfo &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_var::operator CONV_FRAME::CodeSetComponentInfo &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE const CONV_FRAME::CodeSetComponentInfo &
CONV_FRAME::CodeSetComponentInfo_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponentInfo &
CONV_FRAME::CodeSetComponentInfo_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE CONV_FRAME::CodeSetComponentInfo *&
CONV_FRAME::CodeSetComponentInfo_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponentInfo *
CONV_FRAME::CodeSetComponentInfo_var::_retn (void)
{
  CONV_FRAME::CodeSetComponentInfo *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE CONV_FRAME::CodeSetComponentInfo *
CONV_FRAME::CodeSetComponentInfo_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class CONV_FRAME::CodeSetComponentInfo_out
// *************************************************************

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_out::CodeSetComponentInfo_out (CONV_FRAME::CodeSetComponentInfo *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_out::CodeSetComponentInfo_out (CONV_FRAME::CodeSetComponentInfo_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_out::CodeSetComponentInfo_out (const CONV_FRAME::CodeSetComponentInfo_out &p) // copy constructor
  : ptr_ (ACE_const_cast (CONV_FRAME::CodeSetComponentInfo_out&,p).ptr_)
{}

ACE_INLINE CONV_FRAME::CodeSetComponentInfo_out &
CONV_FRAME::CodeSetComponentInfo_out::operator= (const CONV_FRAME::CodeSetComponentInfo_out &p)
{
  this->ptr_ = ACE_const_cast (CONV_FRAME::CodeSetComponentInfo_out&,p).ptr_;
  return *this;
}

ACE_INLINE CONV_FRAME::CodeSetComponentInfo_out &
CONV_FRAME::CodeSetComponentInfo_out::operator= (CONV_FRAME::CodeSetComponentInfo *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CONV_FRAME::CodeSetComponentInfo_out::operator CONV_FRAME::CodeSetComponentInfo *&() // cast
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponentInfo *&
CONV_FRAME::CodeSetComponentInfo_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetComponentInfo *
CONV_FRAME::CodeSetComponentInfo_out::operator-> (void)
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class CONV_FRAME::CodeSetContext_var
// *************************************************************

ACE_INLINE
CONV_FRAME::CodeSetContext_var::CodeSetContext_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
CONV_FRAME::CodeSetContext_var::CodeSetContext_var (CONV_FRAME::CodeSetContext *p)
  : ptr_ (p)
{}

ACE_INLINE
CONV_FRAME::CodeSetContext_var::CodeSetContext_var (const CONV_FRAME::CodeSetContext_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, CONV_FRAME::CodeSetContext(*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
CONV_FRAME::CodeSetContext_var::~CodeSetContext_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetContext_var &
CONV_FRAME::CodeSetContext_var::operator= (CONV_FRAME::CodeSetContext *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CONV_FRAME::CodeSetContext_var &
CONV_FRAME::CodeSetContext_var::operator= (const CONV_FRAME::CodeSetContext_var &p)
{
  if (this != &p)
    {
      if (p.ptr_ == 0)
        {
          delete this->ptr_;
          this->ptr_ = 0;
        }
      else
        {
          CONV_FRAME::CodeSetContext *deep_copy = 
            new CONV_FRAME::CodeSetContext (*p.ptr_);
          
          if (deep_copy != 0)
            {
              CONV_FRAME::CodeSetContext *tmp = deep_copy;
              deep_copy = this->ptr_;
              this->ptr_ = tmp;
              delete deep_copy;
            }
        }
    }
  
  return *this;
}

ACE_INLINE const CONV_FRAME::CodeSetContext *
CONV_FRAME::CodeSetContext_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetContext *
CONV_FRAME::CodeSetContext_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetContext_var::operator const CONV_FRAME::CodeSetContext &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetContext_var::operator CONV_FRAME::CodeSetContext &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
CONV_FRAME::CodeSetContext_var::operator CONV_FRAME::CodeSetContext &() const// cast
{
  return *this->ptr_;
}

ACE_INLINE const CONV_FRAME::CodeSetContext &
CONV_FRAME::CodeSetContext_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetContext &
CONV_FRAME::CodeSetContext_var::inout (void)
{
  return *this->ptr_;
}

// mapping for fixed size
ACE_INLINE CONV_FRAME::CodeSetContext &
CONV_FRAME::CodeSetContext_var::out (void)
{
  return *this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetContext
CONV_FRAME::CodeSetContext_var::_retn (void)
{
  return *this->ptr_;
}

ACE_INLINE CONV_FRAME::CodeSetContext *
CONV_FRAME::CodeSetContext_var::ptr (void) const
{
  return this->ptr_;
}

CORBA::Boolean TAO_Export operator<< (
    TAO_OutputCDR &,
    const CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId &
  );
CORBA::Boolean TAO_Export operator>> (
    TAO_InputCDR &,
    CONV_FRAME::CodeSetComponent::_tao_seq_CodeSetId &
  );

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const CONV_FRAME::CodeSetComponent &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.native_code_set) &&
    (strm << _tao_aggregate.conversion_code_sets)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, CONV_FRAME::CodeSetComponent &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.native_code_set) &&
    (strm >> _tao_aggregate.conversion_code_sets)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const CONV_FRAME::CodeSetComponentInfo &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.ForCharData) &&
    (strm << _tao_aggregate.ForWcharData)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, CONV_FRAME::CodeSetComponentInfo &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.ForCharData) &&
    (strm >> _tao_aggregate.ForWcharData)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const CONV_FRAME::CodeSetContext &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.char_data) &&
    (strm << _tao_aggregate.wchar_data)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, CONV_FRAME::CodeSetContext &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.char_data) &&
    (strm >> _tao_aggregate.wchar_data)
  )
    return 1;
  else
    return 0;

}
