/* -*- C++ -*- */

//
// $Id$
//

#if !defined (TAO_SEQUENCE_T_C)
#define TAO_SEQUENCE_T_C

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

template<class T>
int TAO_Unbounded_Sequence<T>::_bounded (void) const
{
  return 0;
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

template<class T, CORBA::ULong MAX>
int TAO_Bounded_Sequence<T,MAX>::_bounded (void) const
{
  return 0;
}

#endif /* TAO_SEQUENCE_T_C */
