// $Id$

#ifndef TAO_SEQUENCE_T_C
#define TAO_SEQUENCE_T_C

#include "tao/Sequence_T.h"
#include "tao/Environment.h"

#if !defined (__ACE_INLINE__)
#include "tao/Sequence_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao, 
           Sequence_T, 
           "$Id$")

template <class T>
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence (
    const TAO_Unbounded_Sequence<T> &rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T *tmp1 = TAO_Unbounded_Sequence<T>::allocbuf (this->maximum_);
      T * const tmp2 =
        ACE_reinterpret_cast (T * ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          tmp1[i] = tmp2[i];
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <class T> TAO_Unbounded_Sequence<T> &
TAO_Unbounded_Sequence<T>::operator= (const TAO_Unbounded_Sequence<T> &rhs)
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      if (this->maximum_ < rhs.maximum_)
        {
          // Free the old buffer.
          T *tmp = ACE_reinterpret_cast (T *,
                                         this->buffer_);
          TAO_Unbounded_Sequence<T>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_Sequence<T>::allocbuf (rhs.maximum_);
        }
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Unbounded_Sequence<T>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  T *tmp1 = ACE_reinterpret_cast (T *, 
                                  this->buffer_);
  T * const tmp2 = ACE_reinterpret_cast (T * ACE_CAST_CONST, 
                                         rhs.buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      tmp1[i] = tmp2[i];
    }

  return *this;
}

template<class T>
TAO_Unbounded_Sequence<T>::~TAO_Unbounded_Sequence (void)
{
  this->_deallocate_buffer ();
}

template<class T> void
TAO_Unbounded_Sequence<T>::_allocate_buffer (CORBA::ULong length)
{
  T* tmp = TAO_Unbounded_Sequence<T>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      T *old = ACE_reinterpret_cast (T *,
                                     this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          tmp[i] = old[i];
        }

      if (this->release_)
        {
          TAO_Unbounded_Sequence<T>::freebuf (old);
        }
    }

  this->buffer_ = tmp;
}

template<class T> void
TAO_Unbounded_Sequence<T>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  T *tmp = ACE_reinterpret_cast (T *,
                                 this->buffer_);

  TAO_Unbounded_Sequence<T>::freebuf (tmp);
  this->buffer_ = 0;
}

// The following function is not inline, to avoid build (template
// instantiation) problems with Sun C++ 4.2 patch 104631-07/SunOS 5.6.
template <class T> T &
TAO_Unbounded_Sequence<T>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->maximum_);
  T* tmp = ACE_reinterpret_cast (T *,
                                 this->buffer_);
  return tmp[i];
}

// The following function is not inline, to avoid build (template
// instantiation) problems with Sun C++ 4.2 patch 104631-07/SunOS 5.6.
template <class T> const T &
TAO_Unbounded_Sequence<T>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->maximum_);
  T * const tmp = ACE_reinterpret_cast (T* ACE_CAST_CONST, 
                                        this->buffer_);
  return tmp[i];
}

// ****************************************************************
// Bounded_Sequence
// ****************************************************************

template <class T, size_t MAX>
TAO_Bounded_Sequence<T, MAX>::TAO_Bounded_Sequence (
    const TAO_Bounded_Sequence<T, MAX> &rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T *tmp1 = TAO_Bounded_Sequence<T, MAX>::allocbuf (MAX);

      T * const tmp2 =
        ACE_reinterpret_cast (T * ACE_CAST_CONST, 
                              rhs.buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          tmp1[i] = tmp2[i];
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <class T, size_t MAX> TAO_Bounded_Sequence<T, MAX> &
TAO_Bounded_Sequence<T, MAX>::operator= (
    const TAO_Bounded_Sequence<T, MAX> &rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      // We never need to reallocate the buffer because they are
      // always of size MAX.
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Bounded_Sequence<T, MAX>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  T* tmp1 = ACE_reinterpret_cast (T *, this->buffer_);
  T* const tmp2 = ACE_reinterpret_cast (T * ACE_CAST_CONST, rhs.buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      tmp1[i] = tmp2[i];
    }

  return *this;
}

template<class T, size_t MAX>
TAO_Bounded_Sequence<T, MAX>::~TAO_Bounded_Sequence (void)
{
  this->_deallocate_buffer ();
}

template<class T, size_t MAX> void
TAO_Bounded_Sequence<T, MAX>::_allocate_buffer (CORBA::ULong)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ = TAO_Bounded_Sequence<T, MAX>::allocbuf (MAX);
}

template<class T, size_t MAX> void
TAO_Bounded_Sequence<T, MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  T *tmp = ACE_reinterpret_cast (T *,
                                 this->buffer_);
  TAO_Bounded_Sequence<T, MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

// *************************************************************
// class TAO_Object_Manager
// *************************************************************

template <class T, class T_var> TAO_Object_Manager<T,T_var>&
TAO_Object_Manager<T,T_var>::
    operator= (const TAO_Object_Manager<T,T_var> &rhs)
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T_var::tao_release (*this->ptr_);
      *this->ptr_ = T_var::tao_duplicate (*rhs.ptr_);
    }
  else
    *this->ptr_ = *rhs.ptr_;

  return *this;
}

template <class T, class T_var>TAO_Object_Manager<T,T_var> &
TAO_Object_Manager<T,T_var>::operator=(T* p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we will not duplicate the
      // user provided pointer before assigning it to the internal
      // variable.
      T_var::tao_release (*this->ptr_);
      *this->ptr_ = p;
    }
  else
    *this->ptr_ = p;

  return *this;
}

template <class T, class T_var>TAO_Object_Manager<T,T_var> &
TAO_Object_Manager<T,T_var>::operator=(const T_var &p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we duplicate p's
      // pointer before assigning it to the internal
      // variable.
      T_var::tao_release (*this->ptr_);
      *this->ptr_ = T_var::tao_duplicate (p.in ());
    }
  else
    {
      *this->ptr_ = p.in ();
    }

  return *this;
}

template <class T, class T_var> T *&
TAO_Object_Manager<T,T_var>::out (void)
{
  T_var::tao_release (*this->ptr_);
  *this->ptr_ = T_var::tao_nil ();
  return *this->ptr_;
}

template <class T, class T_var> T *
TAO_Object_Manager<T,T_var>::_retn (void)
{
  T *temp = *this->ptr_;
  *this->ptr_ = T_var::tao_nil ();
  return temp;
}

// *************************************************************
// class TAO_Pseudo_Object_Manager
// *************************************************************

template <class T, class T_var> TAO_Pseudo_Object_Manager<T,T_var>&
TAO_Pseudo_Object_Manager<T,T_var>::operator= (
    const TAO_Pseudo_Object_Manager<T,T_var> &rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      CORBA::release (*this->ptr_);
      *this->ptr_ = T::_duplicate (*rhs.ptr_);
    }
  else
    {
      *this->ptr_ = *rhs.ptr_;
    }

  return *this;
}

template <class T, class T_var> TAO_Pseudo_Object_Manager<T,T_var> &
TAO_Pseudo_Object_Manager<T,T_var>::operator=(T* p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we will not duplicate the
      // user provided pointer before assigning it to the internal
      // variable.  However, we will release it.
      CORBA::release (*this->ptr_);
      *this->ptr_ = p;
    }
  else
    {
      *this->ptr_ = p;
    }

  return *this;
}

template <class T, class T_var> TAO_Pseudo_Object_Manager<T,T_var> &
TAO_Pseudo_Object_Manager<T,T_var>::operator=(const T_var &p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we duplicate p's
      // pointer before assigning it to the internal
      // variable.
      CORBA::release (*this->ptr_);
      *this->ptr_ = T::_duplicate (p.in ());
    }
  else
    {
      *this->ptr_ = p.in ();
    }

  return *this;
}

// *************************************************************
// class TAO_Valuetype_Manager
// *************************************************************

template <class T, class T_var> TAO_Valuetype_Manager<T,T_var>&
TAO_Valuetype_Manager<T,T_var>::
    operator= (const TAO_Valuetype_Manager<T,T_var> &rhs)
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      if (*this->ptr_ != 0)
        {
          (*this->ptr_)->_remove_ref ();
        }

      *this->ptr_ = *rhs.ptr_;

      if (*this->ptr_ != 0)
        {
          (*this->ptr_)->_add_ref ();
        }
    }
  else
    {
      *this->ptr_ = *rhs.ptr_;
    }

  return *this;
}

template <class T, class T_var>TAO_Valuetype_Manager<T,T_var> &
TAO_Valuetype_Manager<T,T_var>::operator=(T* p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we will not duplicate the
      // user provided pointer before assigning it to the internal
      // variable.
      if (*this->ptr_ != 0)
        {
          (*this->ptr_)->_remove_ref ();
        }

      *this->ptr_ = p;
    }
  else
    {
      *this->ptr_ = p;
    }

  return *this;
}

template <class T, class T_var>TAO_Valuetype_Manager<T,T_var> &
TAO_Valuetype_Manager<T,T_var>::operator=(const T_var &p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we duplicate p's
      // pointer before assigning it to the internal
      // variable.
      if (*this->ptr_ != 0)
        {
          (*this->ptr_)->_remove_ref ();
        }

      *this->ptr_ = p.in ();

      if (p != 0)
        {
          p->_add_ref ();
        }
    }
  else
    {
      *this->ptr_ = p.in ();
    }

  return *this;
}

// *************************************************************
// Operations for class TAO_Unbounded_Object_Sequence
// *************************************************************

// constructor for unbounded seq
template <class T, class T_var>
TAO_Unbounded_Object_Sequence<T,T_var>::TAO_Unbounded_Object_Sequence (
    CORBA::ULong maximum
  )
  : TAO_Unbounded_Base_Sequence (
        maximum,
         TAO_Unbounded_Object_Sequence<T,T_var>::allocbuf (maximum)
       )
{
}

template <class T, class T_var>
TAO_Unbounded_Object_Sequence<T,T_var>::TAO_Unbounded_Object_Sequence (
    const TAO_Unbounded_Object_Sequence<T,T_var> &rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T **tmp1 =
        TAO_Unbounded_Object_Sequence<T,T_var>::allocbuf (this->maximum_);
      T ** const tmp2 =
        ACE_reinterpret_cast (T ** ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          tmp1[i] = T_var::tao_duplicate (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template<class T, class T_var>
TAO_Unbounded_Object_Sequence<T,T_var>::~TAO_Unbounded_Object_Sequence (void)
{
  this->_deallocate_buffer ();
}

// Assignment operator.
template <class T, class T_var>TAO_Unbounded_Object_Sequence<T,T_var>&
TAO_Unbounded_Object_Sequence<T,T_var>::
operator= (const TAO_Unbounded_Object_Sequence<T,T_var> &rhs)
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T **tmp = ACE_reinterpret_cast (T **,
                                      this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          T_var::tao_release (tmp[i]);
          tmp[i] = T_var::tao_nil ();
        }

      if (this->maximum_ < rhs.maximum_)
        {
          TAO_Unbounded_Object_Sequence<T,T_var>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_Object_Sequence<T,T_var>::allocbuf (rhs.maximum_);
        }
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Unbounded_Object_Sequence<T,T_var>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  T **tmp1 = ACE_reinterpret_cast (T **, this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      tmp1[i] = T_var::tao_duplicate (tmp2[i]);
    }

  return *this;
}

template <class T, class T_var> T **
TAO_Unbounded_Object_Sequence<T,T_var>::allocbuf (CORBA::ULong nelems)
{
  T **buf = 0;
  ACE_NEW_RETURN (buf, 
                  T*[nelems], 
                  0);

  for (CORBA::ULong i = 0; i < nelems; i++)
    {
      buf[i] = T_var::tao_nil ();
    }

  return buf;
}

template <class T, class T_var> void
TAO_Unbounded_Object_Sequence<T,T_var>::freebuf (T **buffer)
{
  if (buffer == 0)
    {
      return;
    }

  // {orbos/97-05-15:16.11} The freebuf function ensures that the
  // destructor for each element is called before the buffer is
  // destroyed, except for string elements, which are freed using
  // string_free(), and object reference elements, which are freed
  // using release(). The freebuf function will ignore null pointers
  // passed to it.

  // @@ How are we supposed to implement that! We don't know the
  // length of the buffer here.
  // Mark the length in the first four bytes? For the moment we let
  // that be.

  delete [] buffer;
}

template<class T, class T_var> void
TAO_Unbounded_Object_Sequence<T,T_var>::_allocate_buffer (CORBA::ULong length)
{
  T **tmp = TAO_Unbounded_Object_Sequence<T,T_var>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      T **old = ACE_reinterpret_cast (T**, 
                                      this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        // Only call duplicate when we did not own the previous
        // buffer, since after this method we own it we must also own
        // the objects.  If we already own the objects there is no
        // need to copy them, if we did we would also have to remove
        // the old instances.
        if (!this->release_)
          {
            tmp[i] = T_var::tao_duplicate (old[i]);
          }
        else
          {
            tmp[i] = old[i];
          }

      if (this->release_)
        {
          delete [] old;
        }
    }

  this->buffer_ = tmp;
}

template<class T, class T_var> void
TAO_Unbounded_Object_Sequence<T,T_var>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

  T **tmp = ACE_reinterpret_cast (T**, 
                                  this->buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      T_var::tao_release (tmp[i]);
      tmp[i] = T_var::tao_nil ();
    }

  TAO_Unbounded_Object_Sequence<T,T_var>::freebuf (tmp);
  this->buffer_ = 0;
}

template<class T, class T_var> void
TAO_Unbounded_Object_Sequence<T,T_var>::_shrink_buffer (CORBA::ULong nl,
                                                        CORBA::ULong ol)
{
  T **tmp = ACE_static_cast (T**, this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      T_var::tao_release (tmp[i]);
      tmp[i] = T_var::tao_nil ();
    }
}

template <class T, class T_var> void
TAO_Unbounded_Object_Sequence<T,T_var>::_downcast (void* target,
                                                   CORBA_Object* src
                                                   ACE_ENV_ARG_DECL)
{
  T **tmp = ACE_static_cast (T**, 
                             target);

  *tmp = T_var::tao_narrow (src 
                            ACE_ENV_ARG_PARAMETER);

  ACE_CHECK;
}

template <class T, class T_var> CORBA_Object*
TAO_Unbounded_Object_Sequence<T,T_var>::_upcast (void* src) const
{
  return T_var::tao_upcast (src);
}

// *************************************************************
// Operations for class TAO_Bounded_Object_Sequence
// *************************************************************

template <class T, class T_var, size_t MAX>
TAO_Bounded_Object_Sequence<T, T_var,MAX>::TAO_Bounded_Object_Sequence (void)
  : TAO_Bounded_Base_Sequence (
        MAX,
        TAO_Bounded_Object_Sequence<T, T_var,MAX>::allocbuf (MAX)
      )
{
}

template <class T, class T_var, size_t MAX>
TAO_Bounded_Object_Sequence<T, T_var,MAX>::TAO_Bounded_Object_Sequence (
    const TAO_Bounded_Object_Sequence<T, T_var,MAX> &rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T **tmp1 =
        TAO_Bounded_Object_Sequence<T, T_var,MAX>::allocbuf (MAX);

      T ** const tmp2 =
        ACE_reinterpret_cast (T** ACE_CAST_CONST, 
                              rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; i++)
        {
          tmp1[i] = T_var::tao_duplicate (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <class T, class T_var, size_t MAX> TAO_Bounded_Object_Sequence<T, T_var,MAX>&
TAO_Bounded_Object_Sequence<T, T_var,MAX>::operator= (
    const TAO_Bounded_Object_Sequence<T,T_var, MAX> &rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T **tmp = ACE_reinterpret_cast (T **, 
                                      this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          T_var::tao_release (tmp[i]);
          tmp[i] = T_var::tao_nil ();
        }
      // No need to reallocate the buffer since it is always of size
      // MAX
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Bounded_Object_Sequence<T,T_var,MAX>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  T **tmp1 = ACE_reinterpret_cast (T **, 
                                   this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i=0; i < rhs.length_; ++i)
    {
      tmp1[i] = T_var::tao_duplicate (tmp2[i]);
    }

  return *this;
}

template <class T, class T_var, size_t MAX> T **
TAO_Bounded_Object_Sequence<T, T_var,MAX>::allocbuf (CORBA::ULong)
{
  T **buf = 0;
  ACE_NEW_RETURN (buf, 
                  T*[MAX], 
                  0);

  for (CORBA::ULong i = 0; i < MAX; i++)
    {
      buf[i] = T_var::tao_nil ();
    }

  return buf;
}

template <class T, class T_var, size_t MAX> void
TAO_Bounded_Object_Sequence<T, T_var,MAX>::freebuf (T **buffer)
{
  // How much do we deallocate? Easy! allocbuf() always creates MAX
  // elements and initialize them to T::_nil().  So we can be
  // complaint and call CORBA::release() on each one.
  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      if (buffer[i] != T_var::tao_nil ())
        {
          T_var::tao_release (buffer[i]);
          buffer[i] = T_var::tao_nil ();
        }
    }

  delete [] buffer;
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Object_Sequence<T, T_var,MAX>::_allocate_buffer (CORBA::ULong length)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_Object_Sequence<T, T_var,MAX>::allocbuf (length);
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Object_Sequence<T,T_var,MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  T **tmp = ACE_reinterpret_cast (T **, 
                                  this->buffer_);
  TAO_Bounded_Object_Sequence<T,T_var,MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Object_Sequence<T,T_var, MAX>::_shrink_buffer (CORBA::ULong nl,
                                                           CORBA::ULong ol)
{
  T **tmp = ACE_reinterpret_cast (T **, this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      T_var::tao_release (tmp[i]);
      tmp[i] = T_var::tao_nil ();
    }
}

template <class T, class T_var, size_t MAX> void
TAO_Bounded_Object_Sequence<T, T_var,MAX>::_downcast (void* target,
                                                      CORBA_Object* src
                                                      ACE_ENV_ARG_DECL)
{
  T **tmp = ACE_static_cast (T**, 
                             target);

  *tmp = T_var::tao_narrow (src 
                            ACE_ENV_ARG_PARAMETER);

  ACE_CHECK;
}

template <class T, class T_var, size_t MAX> CORBA_Object*
TAO_Bounded_Object_Sequence<T, T_var,MAX>::_upcast (void* src) const
{
  return T_var::tao_upcast (src);
}

// *************************************************************
// Operations for class TAO_Unbounded_Pseudo_Sequence
// *************************************************************

// Constructor for unbounded sequence.
template <class T, class T_var>
TAO_Unbounded_Pseudo_Sequence<T,T_var>::
TAO_Unbounded_Pseudo_Sequence (CORBA::ULong maximum)
  : TAO_Unbounded_Base_Sequence (
        maximum,
        TAO_Unbounded_Pseudo_Sequence<T,T_var>::allocbuf (maximum)
      )
{
}

template <class T, class T_var>
TAO_Unbounded_Pseudo_Sequence<T,T_var>::TAO_Unbounded_Pseudo_Sequence (
    const TAO_Unbounded_Pseudo_Sequence<T,T_var> &rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T **tmp1 =
        TAO_Unbounded_Pseudo_Sequence<T,T_var>::allocbuf (this->maximum_);

      T ** const tmp2 =
        ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                              rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          tmp1[i] = T::_duplicate (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template<class T, class T_var>
TAO_Unbounded_Pseudo_Sequence<T,T_var>::~TAO_Unbounded_Pseudo_Sequence (void)
{
  this->_deallocate_buffer ();
}

// assignment operator
template <class T, class T_var> TAO_Unbounded_Pseudo_Sequence<T,T_var>&
TAO_Unbounded_Pseudo_Sequence<T,T_var>::
operator= (const TAO_Unbounded_Pseudo_Sequence<T,T_var> &rhs)
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T **tmp = ACE_reinterpret_cast (T **,
                                      this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::release (tmp[i]);
          tmp[i] = T::_nil ();
        }

      if (this->maximum_ < rhs.maximum_)
        {
          TAO_Unbounded_Pseudo_Sequence<T,T_var>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_Pseudo_Sequence<T,T_var>::allocbuf (rhs.maximum_);
        }
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Unbounded_Pseudo_Sequence<T,T_var>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  T **tmp1 = ACE_reinterpret_cast (T **, 
                                   this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      tmp1[i] = T::_duplicate (tmp2[i]);
    }

  return *this;
}

template <class T, class T_var> T **
TAO_Unbounded_Pseudo_Sequence<T,T_var>::allocbuf (CORBA::ULong nelems)
{
  T **buf = 0;
  ACE_NEW_RETURN (buf, 
                  T*[nelems], 
                  0);

  for (CORBA::ULong i = 0; i < nelems; i++)
    {
      buf[i] = T::_nil ();
    }

  return buf;
}

template <class T, class T_var> void
TAO_Unbounded_Pseudo_Sequence<T,T_var>::freebuf (T **buffer)
{
  if (buffer == 0)
    {
      return;
    }

  // {orbos/97-05-15:16.11} The freebuf function ensures that the
  // destructor for each element is called before the buffer is
  // destroyed, except for string elements, which are freed using
  // string_free(), and object reference elements, which are freed
  // using release(). The freebuf function will ignore null pointers
  // passed to it.

  // @@ How are we supposed to implement that! We don't know the
  // length of the buffer here.
  // Mark the length in the first four bytes? For the moment we let
  // that be.

  delete [] buffer;
}

template<class T, class T_var> void
TAO_Unbounded_Pseudo_Sequence<T,T_var>::_allocate_buffer (CORBA::ULong length)
{
  T **tmp = TAO_Unbounded_Pseudo_Sequence<T,T_var>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      T **old = ACE_reinterpret_cast (T**, 
                                      this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        // Only call duplicate when we did not own the previous
        // buffer, since after this method we own it we must also own
        // the objects.  If we already own the objects there is no
        // need to copy them, if we did we would also have to remove
        // the old instances.
        if (!this->release_)
          {
            tmp[i] = T::_duplicate (old[i]);
          }
        else
          {
            tmp[i] = old[i];
          }

      if (this->release_)
        {
          delete [] old;
        }
    }
  this->buffer_ = tmp;
}

template<class T, class T_var> void
TAO_Unbounded_Pseudo_Sequence<T,T_var>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

  T **tmp = ACE_reinterpret_cast (T**, 
                                  this->buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      CORBA::release (tmp[i]);
      tmp[i] = T::_nil ();
    }

  TAO_Unbounded_Pseudo_Sequence<T,T_var>::freebuf (tmp);
  this->buffer_ = 0;
}

template<class T, class T_var> void
TAO_Unbounded_Pseudo_Sequence<T,T_var>::_shrink_buffer (CORBA::ULong nl,
                                                        CORBA::ULong ol)
{
  T **tmp = ACE_static_cast (T**, this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::release (tmp[i]);
      tmp[i] = T::_nil ();
    }
}

// *************************************************************
// Operations for class TAO_Bounded_Pseudo_Sequence
// *************************************************************

template <class T, class T_var, size_t MAX>
TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::
TAO_Bounded_Pseudo_Sequence (void)
  : TAO_Bounded_Base_Sequence (
        MAX,
        TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::allocbuf (MAX)
      )
{
}

template <class T, class T_var, size_t MAX>
TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::TAO_Bounded_Pseudo_Sequence (
    const TAO_Bounded_Pseudo_Sequence<T, T_var,MAX> &rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T **tmp1 =
        TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::allocbuf (MAX);

      T ** const tmp2 =
       ACE_reinterpret_cast (T** ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; i++)
        {
          tmp1[i] = T::_duplicate (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <class T, class T_var, size_t MAX> TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>&
TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::operator= (
    const TAO_Bounded_Pseudo_Sequence<T, T_var,MAX> &rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T **tmp = ACE_reinterpret_cast (T **, 
                                      this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::release (tmp[i]);
          tmp[i] = T::_nil ();
        }
      // No need to reallocate the buffer since it is always of size
      // MAX
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  T **tmp1 = ACE_reinterpret_cast (T **, 
                                   this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i=0; i < rhs.length_; ++i)
    {
      tmp1[i] = T::_duplicate (tmp2[i]);
    }

  return *this;
}

template <class T, class T_var, size_t MAX> T **
TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::allocbuf (CORBA::ULong)
{
  T **buf = 0;
  ACE_NEW_RETURN (buf, 
                  T*[MAX], 
                  0);

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      buf[i] = T::_nil ();
    }

  return buf;
}

template <class T, class T_var, size_t MAX> void
TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::freebuf (T **buffer)
{
  // How much do we deallocate? Easy! allocbuf() always creates MAX
  // elements and initialize them to T::_nil().  So we can be
  // complaint and call CORBA::release() on each one.
  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      if (buffer[i] != T::_nil ())
        {
          CORBA::release (buffer[i]);
          buffer[i] = T::_nil ();
        }
    }

  delete [] buffer;
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::_allocate_buffer (CORBA::ULong length)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::allocbuf (length);
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  T **tmp = ACE_reinterpret_cast (T **, 
                                  this->buffer_);
  TAO_Bounded_Pseudo_Sequence<T, T_var, MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::_shrink_buffer (CORBA::ULong nl,
                                                           CORBA::ULong ol)
{
  T **tmp = ACE_reinterpret_cast (T **, 
                                  this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::release (tmp[i]);
      tmp[i] = T::_nil ();
    }
}

// *************************************************************
// Operations for class TAO_Unbounded_Valuetype_Sequence
// *************************************************************

// Constructor for unbounded sequence.
template <class T, class T_var>
TAO_Unbounded_Valuetype_Sequence<T,T_var>::TAO_Unbounded_Valuetype_Sequence (
    CORBA::ULong maximum
  )
  : TAO_Unbounded_Base_Sequence (
        maximum,
        TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (maximum)
      )
{
}

template <class T, class T_var>
TAO_Unbounded_Valuetype_Sequence<T,T_var>::TAO_Unbounded_Valuetype_Sequence (
    const TAO_Unbounded_Valuetype_Sequence<T,T_var> &rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T **tmp1 =
        TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (this->maximum_);
      T ** const tmp2 =
        ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                              rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          if (tmp2[i] != 0)
            {
              tmp2[i]->_add_ref ();
            }

          tmp1[i] = tmp2[i];
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template<class T, class T_var>
TAO_Unbounded_Valuetype_Sequence<T,T_var>::~TAO_Unbounded_Valuetype_Sequence (void)
{
  this->_deallocate_buffer ();
}

// assignment operator
template <class T, class T_var>TAO_Unbounded_Valuetype_Sequence<T,T_var>&
TAO_Unbounded_Valuetype_Sequence<T,T_var>::operator= (
    const TAO_Unbounded_Valuetype_Sequence<T,T_var> &rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T **tmp = ACE_reinterpret_cast (T **,
                                      this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          if (tmp[i] != 0)
            {
              tmp[i]->_remove_ref ();
            }

          tmp[i] = 0;
        }

      if (this->maximum_ < rhs.maximum_)
        {
          TAO_Unbounded_Valuetype_Sequence<T,T_var>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (rhs.maximum_);
        }
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  T **tmp1 = ACE_reinterpret_cast (T **, 
                                   this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      if (tmp2[i] != 0)
        {
          tmp2[i]->_add_ref ();
        }

      tmp1[i] = tmp2[i];
    }

  return *this;
}

template <class T, class T_var> T **
TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (CORBA::ULong nelems)
{
  T **buf = 0;
  ACE_NEW_RETURN (buf, 
                  T*[nelems], 
                  0);

  for (CORBA::ULong i = 0; i < nelems; i++)
    {
      buf[i] = 0;
    }

  return buf;
}

template <class T, class T_var> void
TAO_Unbounded_Valuetype_Sequence<T,T_var>::freebuf (T **buffer)
{
  if (buffer == 0)
    {
      return;
    }

  // {orbos/97-05-15:16.11} The freebuf function ensures that the
  // destructor for each element is called before the buffer is
  // destroyed, except for string elements, which are freed using
  // string_free(), and object reference elements, which are freed
  // using release(). The freebuf function will ignore null pointers
  // passed to it.

  // @@ How are we supposed to implement that! We don't know the
  // length of the buffer here.
  // Mark the length in the first four bytes? For the moment we let
  // that be.

  delete [] buffer;
}

template<class T, class T_var> void
TAO_Unbounded_Valuetype_Sequence<T,T_var>::_allocate_buffer (CORBA::ULong length)
{
  T **tmp = TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      T **old = ACE_reinterpret_cast (T**, 
                                      this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        // Only call duplicate when we did not own the previous
        // buffer, since after this method we own it we must also own
        // the objects.  If we already own the objects there is no
        // need to copy them, if we did we would also have to remove
        // the old instances.
        if (!this->release_)
          {
            if (old[i] != 0)
              {
                old[i]->_add_ref ();
              }

            tmp[i] = old[i];
          }
        else
          {
            tmp[i] = old[i];
          }

      if (this->release_)
        {
          delete [] old;
        }
    }
  this->buffer_ = tmp;
}

template<class T, class T_var> void
TAO_Unbounded_Valuetype_Sequence<T,T_var>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

  T **tmp = ACE_reinterpret_cast (T**, 
                                  this->buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      if (tmp[i] != 0)
        {
          tmp[i]->_remove_ref ();
        }

      tmp[i] = 0;
    }

  TAO_Unbounded_Valuetype_Sequence<T,T_var>::freebuf (tmp);
  this->buffer_ = 0;
}

template<class T, class T_var> void
TAO_Unbounded_Valuetype_Sequence<T,T_var>::_shrink_buffer (CORBA::ULong nl,
                                                           CORBA::ULong ol)
{
  T **tmp = ACE_static_cast (T**, 
                             this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      if (tmp[i] != 0)
        {
          tmp[i]->_remove_ref ();
        }

      tmp[i] = 0;
    }
}

// *************************************************************
// Operations for class TAO_Bounded_Valuetype_Sequence
// *************************************************************

template <class T, class T_var, size_t MAX>
TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>::TAO_Bounded_Valuetype_Sequence (
    void
  )
  : TAO_Bounded_Base_Sequence (
        MAX,
        TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>::allocbuf (MAX)
      )
{
}

template <class T, class T_var, size_t MAX>
TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>::TAO_Bounded_Valuetype_Sequence (
    const TAO_Bounded_Valuetype_Sequence<T, T_var,MAX> &rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T **tmp1 =
        TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>::allocbuf (MAX);

      T ** const tmp2 =
        ACE_reinterpret_cast (T** ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; i++)
        {
          if (tmp2[i] != 0)
            {
              tmp2[i]->_add_ref ();
            }

          tmp1[i] = tmp2[i];
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <class T, class T_var, size_t MAX> TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>&
TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>::operator= (
    const TAO_Bounded_Valuetype_Sequence<T,T_var, MAX> &rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T **tmp = ACE_reinterpret_cast (T **, 
                                      this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          if (tmp[i] != 0)
            {
              tmp[i]->_remove_ref ();
            }

          tmp[i] = 0;
        }
      // No need to reallocate the buffer since it is always of size
      // MAX
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  T **tmp1 = ACE_reinterpret_cast (T **, 
                                   this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i=0; i < rhs.length_; ++i)
    {
      if (tmp2[i] != 0)
        {
          tmp2[i]->_add_ref ();
        }

      tmp1[i] = tmp2[i];
    }

  return *this;
}

template <class T, class T_var, size_t MAX> T **
TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>::allocbuf (CORBA::ULong)
{
  T **buf = 0;
  ACE_NEW_RETURN (buf, 
                  T*[MAX], 
                  0);

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      buf[i] = 0;
    }

  return buf;
}

template <class T, class T_var, size_t MAX> void
TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>::freebuf (T **buffer)
{
  // How much do we deallocate? Easy! allocbuf() always creates MAX
  // elements and initialize them to T::_nil().  So we can be
  // complaint and call CORBA::release() on each one.
  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      if (buffer[i] != 0)
        {
          if (buffer[i] != 0)
            {
              buffer[i]->_remove_ref ();
            }

          buffer[i] = 0;
        }
    }

  delete [] buffer;
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>::_allocate_buffer (
    CORBA::ULong length
  )
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>::allocbuf (length);
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  T **tmp = ACE_reinterpret_cast (T **, 
                                  this->buffer_);
  TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Valuetype_Sequence<T,T_var, MAX>::_shrink_buffer (CORBA::ULong nl,
                                                              CORBA::ULong ol)
{
  T **tmp = ACE_reinterpret_cast (T **, 
                                  this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      if (tmp[i] != 0)
        {
          tmp[i]->_remove_ref ();
        }

      tmp[i] = 0;
    }
}

// *************************************************************
// Operations for class TAO_Unbounded_Array_Sequence
// *************************************************************

template <class T, class T_var> T *
TAO_Unbounded_Array_Sequence<T, T_var>::allocbuf (CORBA::ULong nelems)
{
  T *buf = 0;
  ACE_NEW_RETURN (buf,
                  T[nelems],
                  0);

  return buf;
}

template <class T, class T_var>
TAO_Unbounded_Array_Sequence<T, T_var>::TAO_Unbounded_Array_Sequence (
    const TAO_Unbounded_Array_Sequence<T, T_var> &rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T *tmp1 =
        TAO_Unbounded_Array_Sequence<T, T_var>::allocbuf (this->maximum_);

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
      const T *tmp2 = (const T *) rhs.buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
      const T *tmp2 = ACE_reinterpret_cast (const T *,
                                            rhs.buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          T_var::copy (tmp1[i], tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template<class T, class T_var>
TAO_Unbounded_Array_Sequence<T, T_var>::~TAO_Unbounded_Array_Sequence (void)
{
  this->_deallocate_buffer ();
}

// assignment operator
template <class T, class T_var> TAO_Unbounded_Array_Sequence<T, T_var>&
TAO_Unbounded_Array_Sequence<T, T_var>::operator= (
    const TAO_Unbounded_Array_Sequence<T, T_var> &rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      if (this->maximum_ < rhs.maximum_)
        {
          // free the old buffer
#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
          T *tmp = (T *) this->buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
          T *tmp = ACE_reinterpret_cast (T *,
                                         this->buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

          TAO_Unbounded_Array_Sequence<T, T_var>::freebuf (tmp);

          if (rhs.maximum_ == 0)
            {
              this->buffer_ = 0;
            }
          else
            {
              this->buffer_ =
                TAO_Unbounded_Array_Sequence<T, T_var>::allocbuf (
                    rhs.maximum_
                  );
            }
        }
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Unbounded_Array_Sequence<T,T_var>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
      T_var::copy (((T *) this->buffer_)[i],
                   ((const T *) rhs.buffer_)[i]);
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
      T_var::copy (ACE_reinterpret_cast (T *,
                                         this->buffer_)[i],
                   ACE_reinterpret_cast (const T *,
                                         rhs.buffer_)[i]);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
    }

  return *this;
}

template<class T, class T_var> void
TAO_Unbounded_Array_Sequence<T, T_var>::_allocate_buffer (CORBA::ULong length)
{
  T *tmp = TAO_Unbounded_Array_Sequence<T, T_var>::allocbuf (length);

  if (this->buffer_ != 0)
    {
#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
      T *old = (T *) this->buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
      T *old = ACE_reinterpret_cast (T *,
                                     this->buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          T_var::copy (tmp[i], old[i]);
        }

      if (this->release_)
        {
          delete [] old;
        }
    }

  this->buffer_ = tmp;
}

template<class T, class T_var> void
TAO_Unbounded_Array_Sequence<T, T_var>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
  T *tmp = (T *) this->buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
  T *tmp = ACE_reinterpret_cast (T *,
                                 this->buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

  TAO_Unbounded_Array_Sequence<T, T_var>::freebuf (tmp);

  this->buffer_ = 0;
}

// *************************************************************
// Operations for class TAO_Bounded_Array_Sequence
// *************************************************************

template <class T, class T_var, size_t MAX> T *
TAO_Bounded_Array_Sequence<T, T_var, MAX>::allocbuf (CORBA::ULong)
{
  T *buf = 0;
  ACE_NEW_RETURN (buf,
                  T[MAX],
                  0);

  return buf;
}

template <class T, class T_var, size_t MAX>
TAO_Bounded_Array_Sequence<T, T_var, MAX>::TAO_Bounded_Array_Sequence (
    const TAO_Bounded_Array_Sequence<T, T_var, MAX> &rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T *tmp1 =
        TAO_Bounded_Array_Sequence<T, T_var, MAX>::allocbuf (MAX);

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
      const T *tmp2 = (const T *) rhs.buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
      const T *tmp2 = ACE_reinterpret_cast (const T *,
                                            rhs.buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

      for (CORBA::ULong i = 0; i < rhs.length_; i++)
        {
          T_var::copy (tmp1[i], tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template<class T, class T_var, size_t MAX>
TAO_Bounded_Array_Sequence<T, T_var, MAX>::~TAO_Bounded_Array_Sequence (void)
{
  this->_deallocate_buffer ();
}

template <class T, class T_var, size_t MAX> TAO_Bounded_Array_Sequence<T, T_var, MAX>&
TAO_Bounded_Array_Sequence<T, T_var, MAX>::operator= (
    const TAO_Bounded_Array_Sequence<T, T_var, MAX> &rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      // We never need to reallocate the buffer because they are
      // always of size MAX.
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Bounded_Array_Sequence<T,T_var,MAX>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
      T_var::copy (((T *) this->buffer_)[i],
                   ((const T *) rhs.buffer_)[i]);
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
      T_var::copy (ACE_reinterpret_cast (T *,
                                         this->buffer_)[i],
                   ACE_reinterpret_cast (const T *,
                                         rhs.buffer_)[i]);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
    }

  return *this;
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Array_Sequence<T, T_var, MAX>::_allocate_buffer (CORBA::ULong length)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_Array_Sequence<T, T_var, MAX>::allocbuf (length);
}

template<class T, class T_var, size_t MAX> void
TAO_Bounded_Array_Sequence<T, T_var, MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
  T *tmp = (T *) this->buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
  T *tmp = ACE_reinterpret_cast (T *,
                                 this->buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

  TAO_Bounded_Array_Sequence<T, T_var, MAX>::freebuf (tmp);

  this->buffer_ = 0;
}

// *************************************************************
// Operations for class TAO_Bounded_String_Sequence
// *************************************************************

template<size_t MAX>
TAO_Bounded_String_Sequence<MAX>::TAO_Bounded_String_Sequence (void)
  : TAO_Bounded_Base_Sequence (
        MAX,
        TAO_Bounded_String_Sequence<MAX>::allocbuf(MAX)
      )
{
}

template<size_t MAX>
TAO_Bounded_String_Sequence<MAX>::TAO_Bounded_String_Sequence (
    const TAO_Bounded_String_Sequence<MAX> &rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      char **tmp1 =
        TAO_Bounded_String_Sequence<MAX>::allocbuf (this->maximum_);

      char ** const tmp2 =
        ACE_reinterpret_cast (char ** ACE_CAST_CONST,
                              rhs.buffer_);

      for (CORBA::ULong i=0; i < rhs.length_; i++)
        {
          tmp1[i] = CORBA::string_dup (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template<size_t MAX> TAO_Bounded_String_Sequence<MAX>&
TAO_Bounded_String_Sequence<MAX>::operator= (
    const TAO_Bounded_String_Sequence<MAX> &rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      char **tmp = ACE_reinterpret_cast (char **, 
                                         this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::string_free (tmp[i]);
          tmp[i] = 0;
        }
      // No need to reallocate because the buffer is supposed to be of
      // <MAX> size.
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Bounded_String_Sequence<MAX>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  char **tmp1 = ACE_reinterpret_cast (char **, 
                                      this->buffer_);
  char ** const tmp2 = ACE_reinterpret_cast (char ** ACE_CAST_CONST,
                                             rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      tmp1[i] = CORBA::string_dup (tmp2[i]);
    }

  return *this;
}

template<size_t MAX> TAO_SeqElem_String_Manager
TAO_Bounded_String_Sequence<MAX>::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  char **const tmp = ACE_reinterpret_cast (char **ACE_CAST_CONST,
                                           this->buffer_);
  return TAO_SeqElem_String_Manager (tmp + slot,
                                     this->release_);
}

template<size_t MAX> char **
TAO_Bounded_String_Sequence<MAX>::allocbuf (CORBA::ULong)
{
  char **buf = 0;
  ACE_NEW_RETURN (buf, 
                  char *[MAX], 
                  0);

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      buf[i] = 0;
    }

  return buf;
}

template<size_t MAX> void
TAO_Bounded_String_Sequence<MAX>::freebuf (char* *buffer)
{
  // How much do we deallocate? Easy! <allocbuf> always creates MAX
  // elements and initialize them to 0 (they say NULL, yuck!).  So we
  // can be complaint and call CORBA::string_free() on each one.

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      if (buffer[i] != 0)
        {
          CORBA::string_free (buffer[i]);
          buffer[i] = 0;
        }
    }

  delete [] buffer;
}

template<size_t MAX> void
TAO_Bounded_String_Sequence<MAX>::_allocate_buffer (CORBA::ULong /* length */)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_String_Sequence<MAX>::allocbuf (MAX);
}

template<size_t MAX> void
TAO_Bounded_String_Sequence<MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  char **tmp = ACE_reinterpret_cast (char **, 
                                     this->buffer_);
  TAO_Bounded_String_Sequence<MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template<size_t MAX> void
TAO_Bounded_String_Sequence<MAX>::_shrink_buffer (CORBA::ULong nl,
                                                  CORBA::ULong ol)
{
  char **tmp = ACE_reinterpret_cast (char **,
                                     this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::string_free (tmp[i]);
      tmp[i] = 0;
    }
}

template<size_t MAX> void
TAO_Bounded_String_Sequence<MAX>::replace (CORBA::ULong length,
                                           char * *data,
                                           CORBA::Boolean release)
{
  ACE_ASSERT (length < this->maximum_);
  this->_deallocate_buffer ();
  this->length_ = length;

  // If 'release' is 1, caller is responsible for allocating new buffer
  // with CORBA::string_alloc.
  this->buffer_ = data;
  this->release_ = release;
}
// *************************************************************
// Operations for class TAO_Bounded_WString_Sequence
// *************************************************************

template<size_t MAX>
TAO_Bounded_WString_Sequence<MAX>::TAO_Bounded_WString_Sequence (void)
  : TAO_Bounded_Base_Sequence (
        MAX,
        TAO_Bounded_WString_Sequence<MAX>::allocbuf(MAX)
      )
{
}

template<size_t MAX>
TAO_Bounded_WString_Sequence<MAX>::TAO_Bounded_WString_Sequence (
    const TAO_Bounded_WString_Sequence<MAX> &rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      CORBA::WChar **tmp1 =
        TAO_Bounded_WString_Sequence<MAX>::allocbuf (this->maximum_);

      CORBA::WChar ** const tmp2 =
        ACE_reinterpret_cast (CORBA::WChar ** ACE_CAST_CONST,
                              rhs.buffer_);

      for (CORBA::ULong i=0; i < rhs.length_; ++i)
        {
          tmp1[i] = CORBA::wstring_dup (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template<size_t MAX> TAO_Bounded_WString_Sequence<MAX>&
TAO_Bounded_WString_Sequence<MAX>::operator= (
    const TAO_Bounded_WString_Sequence<MAX> &rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      CORBA::WChar **tmp = ACE_reinterpret_cast (CORBA::WChar **,
                                                 this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::wstring_free (tmp[i]);
          tmp[i] = 0;
        }
      // No need to reallocate because the buffer is supposed to be of
      // <MAX> size.
    }
  else
    {
      if (rhs.maximum_ == 0)
        {
          this->buffer_ = 0;
        }
      else
        {
          this->buffer_ =
            TAO_Bounded_WString_Sequence<MAX>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  CORBA::WChar **tmp1 = ACE_reinterpret_cast (CORBA::WChar **,
                                              this->buffer_);
  CORBA::WChar ** const tmp2 =
    ACE_reinterpret_cast (CORBA::WChar ** ACE_CAST_CONST,
                          rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      tmp1[i] = CORBA::wstring_dup (tmp2[i]);
    }

  return *this;
}

template<size_t MAX> TAO_SeqElem_WString_Manager
TAO_Bounded_WString_Sequence<MAX>::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  CORBA::WChar **const tmp =
    ACE_reinterpret_cast (CORBA::WChar **ACE_CAST_CONST,
                          this->buffer_);
  return TAO_SeqElem_WString_Manager (tmp + slot,
                                      this->release_);
}

template<size_t MAX> CORBA::WChar **
TAO_Bounded_WString_Sequence<MAX>::allocbuf (CORBA::ULong)
{
  CORBA::WChar **buf = 0;
  ACE_NEW_RETURN (buf, 
                  CORBA::WChar *[MAX], 
                  0);

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      buf[i] = 0;
    }

  return buf;
}

template<size_t MAX> void
TAO_Bounded_WString_Sequence<MAX>::freebuf (CORBA::WChar* *buffer)
{
  // How much do we deallocate? Easy! <allocbuf> always creates MAX
  // elements and initialize them to 0 (they say NULL, yuck!).  So we
  // can be complaint and call CORBA::wstring_free() on each one.

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      if (buffer[i] != 0)
        {
          CORBA::wstring_free (buffer[i]);
          buffer[i] = 0;
        }
    }

  delete [] buffer;
}

template<size_t MAX> void
TAO_Bounded_WString_Sequence<MAX>::_allocate_buffer (CORBA::ULong /* length */)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_WString_Sequence<MAX>::allocbuf (MAX);
}

template<size_t MAX> void
TAO_Bounded_WString_Sequence<MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  CORBA::WChar **tmp = ACE_reinterpret_cast (CORBA::WChar **,
                                             this->buffer_);
  TAO_Bounded_WString_Sequence<MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template<size_t MAX> void
TAO_Bounded_WString_Sequence<MAX>::_shrink_buffer (CORBA::ULong nl,
                                                   CORBA::ULong ol)
{
  CORBA::WChar **tmp = ACE_reinterpret_cast (CORBA::WChar **,
                                             this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::wstring_free (tmp[i]);
      tmp[i] = 0;
    }
}

template<size_t MAX> void
TAO_Bounded_WString_Sequence<MAX>::replace (CORBA::ULong length,
                                            CORBA::WChar * *data,
                                            CORBA::Boolean release)
{ 
  ACE_ASSERT (length < this->maximum_);
  this->_deallocate_buffer ();
  this->length_ = length;

  // If 'release' is 1, caller is responsible for allocating new buffer
  // with CORBA::wstring_alloc.
  this->buffer_ = data;
  this->release_ = release;
}

#endif /* TAO_SEQUENCE_T_C */
