/* -*- C++ -*- */

//
// $Id$
//

#if !defined (TAO_SEQUENCE_T_C)
#define TAO_SEQUENCE_T_C

template <class T>
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence
(const TAO_Unbounded_Sequence<T> &rhs)
  : TAO_Base_Sequence (rhs),
{
  this->buffer_ = TAO_Unbounded_Sequence<T>::allocbuf (this->maximum_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp[i] = rhs[i];
}

template <class T> TAO_Unbounded_Sequence<T> &
TAO_Unbounded_Sequence<T>::operator=
(const TAO_Unbounded_Sequence<T> &rhs)
{
  if (this != &rhs)
    {
      CORBA::ULong i;
      if (this->release_)
        {
          // free the old buffer
          T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
          TAO_Unbounded_Sequence<T>::freebuf (tmp);
        }

      this->TAO_Base_Sequence::operator= (rhs);
      T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	tmp[i] = rhs[i];
    }
  return *this;
}

template<class T>
void TAO_Unbounded_Sequence<T>::_allocate_buffer (CORBA::ULong length)
{
  T* tmp;
  ACE_NEW (tmp, T[length]);

  if (this->buffer_ != 0)
    {
      T* old = ACE_reinterpret_cast(T*,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  tmp[i] = old[i];
	}
      delete[] old;
    }
  this->buffer_ = tmp;
}

template<class T>
void TAO_Unbounded_Sequence<T>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0)
    return;
  T* tmp = ACE_reinterpret_cast (T*,this->buffer_);
  delete[] tmp;
  this->buffer_ = 0;
}

template<class T, CORBA::ULong MAX>
void TAO_Bounded_Sequence<T,MAX>::_allocate_buffer (CORBA::ULong)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  ACE_NEW (this->buffer_, T[MAX]);
  this->maximum_ = MAX;
}

template<class T, CORBA::ULong MAX>
void TAO_Bounded_Sequence<T,MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0)
    return;
  T* tmp = ACE_reinterpret_cast (T*,this->buffer_);
  delete[] tmp;
  this->buffer_ = 0;
}

// *************************************************************
// class TAO_Unbounded_ObjectSequence::TAO_ObjRefMngType
// *************************************************************

// destructor
template <class T, class T_ptr>
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType::~TAO_ObjRefMngType (void)
{
}

template <class T, class T_ptr>
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType::
TAO_ObjRefMngType (const
                   TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType
                   &_tao_mng_type)
{
  *this->ptr_ = T::_duplicate (*_tao_mng_type.ptr_);
  this->release_ = _tao_mng_type.release_;
}

template <class T, class T_ptr>
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType&
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType::operator=
(const TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType &_tao_mng_type)
{
  if (this == &_tao_mng_type) return *this;
  if (this->release_) // need to free old one
    CORBA::release (*this->ptr_);
  *this->ptr_ = T::_duplicate (*_tao_mng_type.ptr_);
  return *this;
}

template <class T, class T_ptr>
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType &
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType::operator=
(T_ptr p)
{
  if (this->release_) // need to free old one
    CORBA::release (*this->ptr_);
  *this->ptr_ = p; // no copy
  return *this;
}

// mapping for variable size
template <class T, class T_ptr> T_ptr &
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType::out (void)
{
  if (this->release_)
    CORBA::release (*this->ptr_);
  *this->ptr_ = T::_nil ();
  return *this->ptr_;
}

template <class T, class T_ptr> T_ptr
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType::_retn (void)
{
  T_ptr tmp = *this->ptr_;
  *this->ptr_ = T::_nil ();
  return tmp;
}

// *************************************************************
// Operations for class
// TAO_Unbounded_ObjectSequence<T,T_ptr>
// *************************************************************

template<class T, class T_ptr>
void TAO_Unbounded_ObjectSequence<T,T_ptr>::_allocate_buffer (CORBA::ULong length)
{
  T_ptr *tmp;
  ACE_NEW (tmp, T_ptr [length]);

  if (this->buffer_ != 0)
    {
      T_ptr *old = ACE_reinterpret_cast(T_ptr*,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  tmp [i] = T::_duplicate (old[i]);
	}
      delete[] old;
    }
  this->buffer_ = tmp;
}

template<class T, class T_ptr>
void TAO_Unbounded_ObjectSequence<T,T_ptr>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0)
    return;
  T_ptr *tmp = ACE_reinterpret_cast (T_ptr*,this->buffer_);
  // XXXASG: Do we release each object here?
  delete[] tmp;
  this->buffer_ = 0;
}

// copy constructor
template <class T, class T_ptr>
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_Unbounded_ObjectSequence
(const TAO_Unbounded_ObjectSequence<T,T_ptr> &seq)
  : TAO_Base_Sequence (seq)
{
  this->buffer_ = TAO_Unbounded_ObjectSequence<T,T_ptr>::
    allocbuf (this->maximum_);
  T_ptr *tmp1 = ACE_reinterpret_cast(T_ptr *,this->buffer_);
  T_ptr *tmp2 = ACE_reinterpret_cast(T_ptr *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp [i] = T::_duplicate (tmp2 [i]);
}

// destructor
template <class T, class T_ptr>
TAO_Unbounded_ObjectSequence<T,T_ptr>::~TAO_Unbounded_ObjectSequence (void)
{
  if (this->release_) // we own the buffer
  {
    T_ptr *tmp = ACE_reinterpret_cast (T_ptr *,this->buffer_);
    TAO_Unbounded_ObjectSequence<T,T_ptr>::freebuf (tmp,
                                                    this->maximum_);
  }
}

// assignment operator
template <class T, class T_ptr>
TAO_Unbounded_ObjectSequence<T,T_ptr>&
TAO_Unbounded_ObjectSequence<T,T_ptr>::operator=
(const TAO_Unbounded_ObjectSequence<T,T_ptr> &seq)
{
  if (this == &seq) return *this;
  if (this->release_)
  {
    T_ptr *tmp = ACE_reinterpret_cast (T_ptr *,this->buffer_);
    TAO_Unbounded_ObjectSequence<T,T_ptr>::freebuf (tmp,
                                                    this->maximum_);
  }
  T_ptr *tmp1 = ACE_reinterpret_cast(T_ptr *,this->buffer_);
  T_ptr *tmp2 = ACE_reinterpret_cast(T_ptr *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp [i] = T::_duplicate (tmp2 [i]);
  return *this;
}

template <class T, class T_ptr> T_ptr *
TAO_Unbounded_ObjectSequence<T,T_ptr>::allocbuf (CORBA::ULong nelems)
{
  T_ptr *buf = new T_ptr[nelems]; // allocate from heap
  for (CORBA::ULong i=0; i < nelems; i++)
  	buf[i] = T::_nil ();
  return buf;
}

template <class T, class T_ptr> void
TAO_Unbounded_ObjectSequence<T,T_ptr>::freebuf (T_ptr *seq,
                                       CORBA::ULong nelems)
{
  if (!seq) return; // null sequence
  for (CORBA::ULong i=0; i < nelems; i++)
  	CORBA::release (seq[i]);
  TAO_Unbounded_ObjectSequence<T,T_ptr>::freebuf (seq);
}

// *************************************************************
// class TAO_Bounded_ObjectSequence::TAO_ObjRefMngType
// *************************************************************

// destructor
template <class T, class T_ptr, CORBA::ULong MAX>
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType::
~TAO_ObjRefMngType (void)
{
}

template <class T, class T_ptr, CORBA::ULong MAX>
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType::
TAO_ObjRefMngType (const
                   TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType
                   &_tao_mng_type)
{
  *this->ptr_ = T::_duplicate (*_tao_mng_type.ptr_);
  this->release_ = _tao_mng_type.release_;
}

template <class T, class T_ptr, CORBA::ULong MAX>
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType&
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType::operator=
(const TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType &_tao_mng_type)
{
  if (this == &_tao_mng_type) return *this;
  if (this->release_) // need to free old one
    CORBA::release (*this->ptr_);
  *this->ptr_ = T::_duplicate (*_tao_mng_type.ptr_);
  return *this;
}

template <class T, class T_ptr, CORBA::ULong MAX>
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType &
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType::operator=
(T_ptr p)
{
  if (this->release_) // need to free old one
    CORBA::release (*this->ptr_);
  *this->ptr_ = p; // no copy
  return *this;
}

// mapping for variable size
template <class T, class T_ptr, CORBA::ULong MAX> T_ptr &
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType::out (void)
{
  if (this->release_)
    CORBA::release (*this->ptr_);
  *this->ptr_ = T::_nil ();
  return *this->ptr_;
}

template <class T, class T_ptr, CORBA::ULong MAX> T_ptr
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType::_retn (void)
{
  T_ptr tmp = *this->ptr_;
  *this->ptr_ = T::_nil ();
  return tmp;
}

// *************************************************************
// Operations for class
// TAO_Bounded_ObjectSequence<T,T_ptr,MAX>
// *************************************************************

template<class T, class T_ptr, CORBA::ULong MAX>
void TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::_allocate_buffer (CORBA::ULong
                                                                length)
{
  T_ptr *tmp;
  ACE_NEW (tmp, T_ptr [length]);

  if (this->buffer_ != 0)
    {
      T_ptr *old = ACE_reinterpret_cast(T_ptr*,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  tmp [i] = T::_duplicate (old[i]);
	}
      delete[] old;
    }
  this->buffer_ = tmp;
}

template<class T, class T_ptr, CORBA::ULong MAX>
void TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0)
    return;
  T_ptr *tmp = ACE_reinterpret_cast (T_ptr*,this->buffer_);
  // XXXASG: Do we release each object here?
  delete[] tmp;
  this->buffer_ = 0;
}

// copy constructor
template <class T, class T_ptr, CORBA::ULong MAX>
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_Bounded_ObjectSequence
(const TAO_Bounded_ObjectSequence<T,T_ptr,MAX> &seq)
  : TAO_Base_Sequence (seq)
{
  this->buffer_ = TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::
    allocbuf (this->maximum_);
  T_ptr *tmp1 = ACE_reinterpret_cast(T_ptr *,this->buffer_);
  T_ptr *tmp2 = ACE_reinterpret_cast(T_ptr *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp [i] = T::_duplicate (tmp2 [i]);
}

// destructor
template <class T, class T_ptr, CORBA::ULong MAX>
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::~TAO_Bounded_ObjectSequence (void)
{
  if (this->release_) // we own the buffer
  {
    T_ptr *tmp = ACE_reinterpret_cast (T_ptr *,this->buffer_);
    TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::freebuf (tmp,
                                                    this->maximum_);
  }
}

// assignment operator
template <class T, class T_ptr, CORBA::ULong MAX>
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>&
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::operator=
(const TAO_Bounded_ObjectSequence<T,T_ptr,MAX> &seq)
{
  if (this == &seq) return *this;
  if (this->release_)
  {
    T_ptr *tmp = ACE_reinterpret_cast (T_ptr *,this->buffer_);
    TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::freebuf (tmp,
                                                    this->maximum_);
  }
  T_ptr *tmp1 = ACE_reinterpret_cast(T_ptr *,this->buffer_);
  T_ptr *tmp2 = ACE_reinterpret_cast(T_ptr *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp [i] = T::_duplicate (tmp2 [i]);
  return *this;
}

template <class T, class T_ptr, CORBA::ULong MAX> T_ptr *
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::allocbuf (CORBA::ULong nelems)
{
  T_ptr *buf = new T_ptr[nelems]; // allocate from heap
  for (CORBA::ULong i=0; i < nelems; i++)
  	buf[i] = T::_nil ();
  return buf;
}

template <class T, class T_ptr, CORBA::ULong MAX> void
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::freebuf (T_ptr *seq,
                                       CORBA::ULong nelems)
{
  if (!seq) return; // null sequence
  for (CORBA::ULong i=0; i < nelems; i++)
  	CORBA::release (seq[i]);
  TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::freebuf (seq);
}

// *************************************************************
// Operations for class TAO_Bounded_StringSequence<MAX>::TAO_StrMngType
// *************************************************************

// destructor
template <CORBA::ULong MAX>
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType::~TAO_StrMngType (void)
{
}

// copy constructor
template <CORBA::ULong MAX>
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType::TAO_StrMngType
(const TAO_Bounded_StringSequence<MAX>::TAO_StrMngType &_tao_mng_type)
{
  *this->ptr_ = CORBA::string_dup (*_tao_mng_type.ptr_);
  this->release_ = _tao_mng_type.release_;
}

// assignment
template <CORBA::ULong MAX>
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType&
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType::operator=
(const TAO_Bounded_StringSequence<MAX>::TAO_StrMngType &_tao_mng_type)
{
  if (this == &_tao_mng_type) return *this;
  if (this->release_) // need to free old one
    CORBA::string_free (*this->ptr_);
  *this->ptr_ = CORBA::string_dup (*_tao_mng_type.ptr_);
  return *this;
}

// assignment from char*
template <CORBA::ULong MAX>
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType &
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType::operator= (char * p)
{
  if (this->release_) // need to free old one
    CORBA::string_free (*this->ptr_);
  *this->ptr_ = p; // no copy
  return *this;
}

template <CORBA::ULong MAX>
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType &
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType::operator= (const char * p)
{
  if (this->release_) // need to free old one
  {
    CORBA::string_free (*this->ptr_);
    *this->ptr_ = CORBA::string_dup (p);// make copy
  }
  else // make no copy as we don't own it
    *this->ptr_ = (char *)p; // no copy
  return *this;
}

// mapping for variable size - out
template <CORBA::ULong MAX>
char *&
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType::out (void)
{
  if (this->release_)
    CORBA::string_free (*this->ptr_);
  *this->ptr_ = 0;
  return *this->ptr_;
}

// _retn (gives up ownership)
template <CORBA::ULong MAX>
char *
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType::_retn (void)
{
  char *tmp = *this->ptr_;
  *this->ptr_ = 0;
  return tmp;
}

// *************************************************************
// class TAO_Bounded_StringSequence<MAX>
// *************************************************************

// copy constructor
template <CORBA::ULong MAX>
TAO_Bounded_StringSequence<MAX>::TAO_Bounded_StringSequence
(const TAO_Bounded_StringSequence<MAX> &seq)
  : TAO_Base_Sequence (seq),
{
  this->buffer_ = TAO_Bounded_StringSequence<MAX>::allocbuf (this->maximum_);
  char **tmp = ACE_reinterpret_cast(char**,this->buffer);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp[i] = CORBA::string_dup (seq[i]);
}

// destructor
template <CORBA::ULong MAX>
TAO_Bounded_StringSequence<MAX>::~TAO_Bounded_StringSequence (void)
{
  if (this->release_) // we own the buffer
  {
    TAO_Bounded_StringSequence<MAX>::freebuf (this->buffer_, this->maximum_);
  }
}

// assignment operator
template <CORBA::ULong MAX>
TAO_Bounded_StringSequence<MAX>&
TAO_Bounded_StringSequence<MAX>::operator=
(const TAO_Bounded_StringSequence<MAX> &seq)
{
  if (this == &seq) return *this;

  // first free all elements if the release flag is set
  if (this->release_)
  {
    char **tmp = ACE_reinterpret_cast (char**,this->buffer);
    TAO_Bounded_StringSequence<MAX>::freebuf (tmp, this->maximum_);
  }

  this->TAO_Base_Sequence::operator= (seq); // set the underlying static
                                            // members
  // allocate memory according to the new maximum
  this->buffer_ = TAO_Bounded_StringSequence<MAX>::allocbuf (this->maximum_);
  char **tmp = ACE_reinterpret_cast(char**,this->buffer);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp[i] = CORBA::string_dup (seq[i]);

  return *this;
}

template <CORBA::ULong MAX>
char **
TAO_Bounded_StringSequence<MAX>::allocbuf (CORBA::ULong nelems)
{
  char **buf;
  ACE_NEW (buf, char *[nelems]); // allocate
  // initialize to 0
  for (CORBA::ULong i=0; i < nelems; i++)
    buf[i] = (char *)0;
  return buf;
}

template <CORBA::ULong MAX>
void
TAO_Bounded_StringSequence<MAX>::freebuf (char **buf, CORBA::ULong nelems)
{
  if (!buf) return; // null sequence
  for (CORBA::ULong i=0; i < nelems; i++)
  	CORBA::string_free (buf[i]);
  TAO_Bounded_StringSequence<MAX>::freebuf (buf);
}

template <CORBA::ULong MAX>
void TAO_Bounded_StringSequence<MAX>::_allocate_buffer (CORBA::ULong length)
{
  char **tmp;
  ACE_NEW (tmp, char*[length]);

  if (this->buffer_ != 0)
    {
      char **old = ACE_reinterpret_cast(char**,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  tmp[i] = CORBA::string_dup (old[i]);
	}
      delete[] old;
    }
  this->buffer_ = tmp;
}

template <CORBA::ULong MAX>
void TAO_Bounded_StringSequence<MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0)
    return;

  char **tmp = ACE_reinterpret_cast (char**,this->buffer_);
  delete[] tmp;
  this->buffer_ = 0;
}

#endif /* TAO_SEQUENCE_T_C */
