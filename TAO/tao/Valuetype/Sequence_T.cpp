// $Id$

#ifndef TAO_VALUETYPE_SEQUENCE_T_C
#define TAO_VALUETYPE_SEQUENCE_T_C

#include "Sequence_T.h"

#if !defined (__ACE_INLINE__)
#include "Sequence_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Valuetype, 
           Sequence_T, 
           "$Id$")

// *************************************************************
// class TAO_Valuetype_Manager
// *************************************************************

template <typename T, typename T_var> 
TAO_Valuetype_Manager<T,T_var> &
TAO_Valuetype_Manager<T,T_var>::operator= (
    const TAO_Valuetype_Manager<T,T_var> & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      TAO::Value_Traits<T>::tao_remove_ref (*this->ptr_);
      *this->ptr_ = *rhs.ptr_;
      TAO::Value_Traits<T>::tao_add_ref (*this->ptr_);
    }
  else
    {
      *this->ptr_ = *rhs.ptr_;
    }

  return *this;
}

template <typename T, typename T_var> 
TAO_Valuetype_Manager<T,T_var> &
TAO_Valuetype_Manager<T,T_var>::operator= (T * p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we will not duplicate the
      // user provided pointer before assigning it to the internal
      // variable.
      TAO::Value_Traits<T>::tao_remove_ref (*this->ptr_);
      *this->ptr_ = p;
    }
  else
    {
      *this->ptr_ = p;
    }

  return *this;
}

template <typename T, typename T_var> 
TAO_Valuetype_Manager<T,T_var> &
TAO_Valuetype_Manager<T,T_var>::operator= (const T_var & p)
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

      TAO::Value_Traits<T>::tao_remove_ref (*this->ptr_);
      *this->ptr_ = p.in ();
      TAO::Value_Traits<T>::tao_add_ref (*this->ptr_);
    }
  else
    {
      *this->ptr_ = p.in ();
    }

  return *this;
}

// *************************************************************
// class TAO_Abstract_Manager
// *************************************************************

template<typename T, typename T_var>
TAO_Abstract_Manager<T,T_var> &
TAO_Abstract_Manager<T,T_var>::operator= (
    const TAO_Abstract_Manager<T,T_var> & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      TAO::Objref_Traits<T>::tao_release (*this->ptr_);
      TAO::Objref_Traits<T>::tao_duplicate (*rhs->ptr_);
    }

  *this->ptr_ = *rhs.ptr_;
  return *this;
}

template<typename T, typename T_var>
TAO_Abstract_Manager<T,T_var> &
TAO_Abstract_Manager<T,T_var>::operator= (T * p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we will not duplicate the
      // user provided pointer before assigning it to the internal
      // variable.
      TAO::Objref_Traits<T>::tao_release (*this->ptr_);
    }

  *this->ptr_ = p;
  return *this;
}

template<typename T, typename T_var>
TAO_Abstract_Manager<T,T_var> &
TAO_Abstract_Manager<T,T_var>::operator= (const T_var & p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we duplicate p's
      // pointer before assigning it to the internal
      // variable.
      TAO::Objref_Traits<T>::tao_release (*this->ptr_);
      TAO::Objref_Traits<T>::tao_duplicate (p.in ());
    }

  *this->ptr_ = p.in ();
  return *this;
}

template<typename T, typename T_var>
TAO_Abstract_Manager<T,T_var>::operator const T_var () const
{
  TAO::Objref_Traits<T>::tao_duplicate (*this->ptr_);
  return *this->ptr_;
}

template<typename T, typename T_var>
T *&
TAO_Abstract_Manager<T,T_var>::out (void)
{
  TAO::Objref_Traits<T>::tao_release (*this->ptr_);
  *this->ptr_ = 0;
  return *this->ptr_;
}

// *************************************************************
// Operations for class TAO_Unbounded_Valuetype_Sequence
// *************************************************************

template <typename T, typename T_var> 
TAO_Unbounded_Valuetype_Sequence<T,T_var>::TAO_Unbounded_Valuetype_Sequence (
    CORBA::ULong maximum
  )
  : TAO_Unbounded_Base_Sequence (
        maximum,
        TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (maximum)
      )
{
}

template <typename T, typename T_var> 
TAO_Unbounded_Valuetype_Sequence<T,T_var>::TAO_Unbounded_Valuetype_Sequence (
    const TAO_Unbounded_Valuetype_Sequence<T,T_var> & rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T ** tmp1 =
        TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (
            this->maximum_
          );
      T ** const tmp2 =
        ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                              rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          TAO::Value_Traits<T>::tao_add_ref (tmp2[i]);
          tmp1[i] = tmp2[i];
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <typename T, typename T_var> 
TAO_Unbounded_Valuetype_Sequence<T,T_var>::~TAO_Unbounded_Valuetype_Sequence (
    void
  )
{
  this->_deallocate_buffer ();
}

template <typename T, typename T_var> 
TAO_Unbounded_Valuetype_Sequence<T,T_var> &
TAO_Unbounded_Valuetype_Sequence<T,T_var>::operator= (
    const TAO_Unbounded_Valuetype_Sequence<T,T_var> & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T ** tmp = ACE_reinterpret_cast (T **,
                                       this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          TAO::Value_Traits<T>::tao_remove_ref (tmp[i]);
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
            TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (
                rhs.maximum_
              );
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  T ** tmp1 = ACE_reinterpret_cast (T **, 
                                    this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      TAO::Value_Traits<T>::tao_add_ref (tmp2[i]);
      tmp1[i] = tmp2[i];
    }

  return *this;
}

template <typename T, typename T_var> 
T **
TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (CORBA::ULong nelems)
{
  T ** buf = 0;
  ACE_NEW_RETURN (buf, 
                  T * [nelems], 
                  0);

  for (CORBA::ULong i = 0; i < nelems; ++i)
    {
      buf[i] = 0;
    }

  return buf;
}

template <typename T, typename T_var> 
void
TAO_Unbounded_Valuetype_Sequence<T,T_var>::freebuf (T ** buffer)
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

template <typename T, typename T_var> 
void
TAO_Unbounded_Valuetype_Sequence<T,T_var>::_allocate_buffer (
    CORBA::ULong length
  )
{
  T ** tmp = 
    TAO_Unbounded_Valuetype_Sequence<T,T_var>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      T ** old = ACE_reinterpret_cast (T **, 
                                       this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        // Only call duplicate when we did not own the previous
        // buffer, since after this method we own it we must also own
        // the objects.  If we already own the objects there is no
        // need to copy them, if we did we would also have to remove
        // the old instances.
        if (!this->release_)
          {
            TAO::Value_Traits<T>::tao_add_ref (tmp[i]);
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

template <typename T, typename T_var> 
void
TAO_Unbounded_Valuetype_Sequence<T,T_var>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

  T ** tmp = ACE_reinterpret_cast (T **, 
                                   this->buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      TAO::Value_Traits<T>::tao_remove_ref (tmp[i]);
      tmp[i] = 0;
    }

  TAO_Unbounded_Valuetype_Sequence<T,T_var>::freebuf (tmp);
  this->buffer_ = 0;
}

template <typename T, typename T_var> 
void
TAO_Unbounded_Valuetype_Sequence<T,T_var>::_shrink_buffer (
    CORBA::ULong nl,
    CORBA::ULong ol
  )
{
  T ** tmp = ACE_static_cast (T **, 
                              this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      TAO::Value_Traits<T>::tao_remove_ref (tmp[i]);
      tmp[i] = 0;
    }
}

// *************************************************************
// Operations for class TAO_Bounded_Valuetype_Sequence
// *************************************************************

template <typename T, typename T_var, size_t MAX>
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::TAO_Bounded_Valuetype_Sequence (
    void
  )
  : TAO_Bounded_Base_Sequence (
        MAX,
        TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::allocbuf (MAX)
      )
{
}

template <typename T, typename T_var, size_t MAX>
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::TAO_Bounded_Valuetype_Sequence (
    const TAO_Bounded_Valuetype_Sequence<T,T_var,MAX> & rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T ** tmp1 =
        TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::allocbuf (MAX);

      T ** const tmp2 =
        ACE_reinterpret_cast (T ** ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          TAO::Value_Traits<T>::tao_add_ref (tmp2[i]);
          tmp1[i] = tmp2[i];
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <typename T, typename T_var, size_t MAX>
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>&
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::operator= (
    const TAO_Bounded_Valuetype_Sequence<T,T_var,MAX> & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T ** tmp = ACE_reinterpret_cast (T **, 
                                       this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          TAO::Value_Traits<T>::tao_remove_ref (tmp[i]);
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
            TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::allocbuf (
                rhs.maximum_
              );
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  T ** tmp1 = ACE_reinterpret_cast (T **, 
                                    this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      TAO::Value_Traits<T>::tao_add_ref (tmp2[i]);
      tmp1[i] = tmp2[i];
    }

  return *this;
}

template <typename T, typename T_var, size_t MAX>
T **
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::allocbuf (CORBA::ULong)
{
  T ** buf = 0;
  ACE_NEW_RETURN (buf, 
                  T * [MAX], 
                  0);

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      buf[i] = 0;
    }

  return buf;
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::freebuf (T ** buffer)
{
  // How much do we deallocate? Easy! allocbuf() always creates MAX
  // elements and initialize them to T::_nil().  So we can be
  // complaint and call CORBA::release() on each one.
  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      if (buffer[i] != 0)
        {
          TAO::Value_Traits<T>::tao_remove_ref (buffer[i]);
          buffer[i] = 0;
        }
    }

  delete [] buffer;
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::_allocate_buffer (
    CORBA::ULong length
  )
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::allocbuf (length);
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  T ** tmp = ACE_reinterpret_cast (T **, 
                                   this->buffer_);
  TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::_shrink_buffer (
    CORBA::ULong nl,
    CORBA::ULong ol
  )
{
  T ** tmp = ACE_reinterpret_cast (T **, 
                                   this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      TAO::Value_Traits<T>::tao_remove_ref (tmp[i]);
      tmp[i] = 0;
    }
}

// *************************************************************
// Operations for class TAO_Unbounded_Abstract_Sequence
// *************************************************************

template <typename T, typename T_var>
TAO_Unbounded_Abstract_Sequence<T,T_var>::TAO_Unbounded_Abstract_Sequence (
    CORBA::ULong maximum
  )
  : TAO_Unbounded_Base_Sequence (
        maximum,
        TAO_Unbounded_Abstract_Sequence<T,T_var>::allocbuf (maximum)
      )
{
}

template <typename T, typename T_var>
TAO_Unbounded_Abstract_Sequence<T,T_var>::TAO_Unbounded_Abstract_Sequence (
    const TAO_Unbounded_Abstract_Sequence<T,T_var> & rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T ** tmp1 =
        TAO_Unbounded_Abstract_Sequence<T,T_var>::allocbuf (
            this->maximum_
          );
      T ** const tmp2 =
        ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                              rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          tmp1[i] = TAO::Objref_Traits<T>::tao_duplicate (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <typename T, typename T_var>
TAO_Unbounded_Abstract_Sequence<T,T_var>::~TAO_Unbounded_Abstract_Sequence (
    void
  )
{
  this->_deallocate_buffer ();
}

template <typename T, typename T_var>
TAO_Unbounded_Abstract_Sequence<T,T_var> &
TAO_Unbounded_Abstract_Sequence<T,T_var>::operator= (
    const TAO_Unbounded_Abstract_Sequence<T,T_var> & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T ** tmp = ACE_reinterpret_cast (T **,
                                       this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          TAO::Objref_Traits<T>::tao_release (tmp[i]);
          tmp[i] = TAO::Objref_Traits<T>::tao_nil ();
        }

      if (this->maximum_ < rhs.maximum_)
        {
          TAO_Unbounded_Abstract_Sequence<T,T_var>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_Abstract_Sequence<T,T_var>::allocbuf (
                rhs.maximum_
              );
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
            TAO_Unbounded_Abstract_Sequence<T,T_var>::allocbuf (
                rhs.maximum_
              );
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  T ** tmp1 = ACE_reinterpret_cast (T **, 
                                    this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      tmp1[i] = TAO::Objref_Traits<T>::tao_duplicate (tmp2[i]);
    }

  return *this;
}

template <typename T, typename T_var>
T **
TAO_Unbounded_Abstract_Sequence<T,T_var>::allocbuf (
    CORBA::ULong nelems
  )
{
  T ** buf = 0;
  ACE_NEW_RETURN (buf, 
                  T * [nelems], 
                  0);

  for (CORBA::ULong i = 0; i < nelems; ++i)
    {
      buf[i] = TAO::Objref_Traits<T>::tao_nil ();
    }

  return buf;
}

template <typename T, typename T_var>
void
TAO_Unbounded_Abstract_Sequence<T,T_var>::freebuf (T ** buffer)
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

template <typename T, typename T_var>
void
TAO_Unbounded_Abstract_Sequence<T,T_var>::_allocate_buffer (
    CORBA::ULong length
  )
{
  T ** tmp = TAO_Unbounded_Abstract_Sequence<T,T_var>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      T ** old = ACE_reinterpret_cast (T **, 
                                       this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        // Only call duplicate when we did not own the previous
        // buffer, since after this method we own it we must also own
        // the objects.  If we already own the objects there is no
        // need to copy them, if we did we would also have to remove
        // the old instances.
        if (!this->release_)
          {
            tmp[i] = TAO::Objref_Traits<T>::tao_duplicate (old[i]);
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

template <typename T, typename T_var>
void
TAO_Unbounded_Abstract_Sequence<T,T_var>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

  T ** tmp = ACE_reinterpret_cast (T **, 
                                   this->buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      TAO::Objref_Traits<T>::tao_release (tmp[i]);
      tmp[i] = TAO::Objref_Traits<T>::tao_nil ();
    }

  TAO_Unbounded_Abstract_Sequence<T,T_var>::freebuf (tmp);
  this->buffer_ = 0;
}

template <typename T, typename T_var>
void
TAO_Unbounded_Abstract_Sequence<T,T_var>::_shrink_buffer (
    CORBA::ULong nl,
    CORBA::ULong ol
  )
{
  T ** tmp = ACE_static_cast (T **, 
                              this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      TAO::Objref_Traits<T>::tao_release (tmp[i]);
      tmp[i] = TAO::Objref_Traits<T>::tao_nil ();
    }
}

// *************************************************************
// Operations for class TAO_Bounded_Abstract_Sequence
// *************************************************************

template <typename T, typename T_var, size_t MAX>
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::TAO_Bounded_Abstract_Sequence (
    void
  )
  : TAO_Bounded_Base_Sequence (
        MAX,
        TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::allocbuf (MAX)
      )
{
}

template <typename T, typename T_var, size_t MAX>
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::TAO_Bounded_Abstract_Sequence (
    const TAO_Bounded_Abstract_Sequence<T,T_var,MAX> & rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T ** tmp1 =
        TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::allocbuf (MAX);

      T ** const tmp2 =
        ACE_reinterpret_cast (T ** ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          tmp1[i] = TAO::Objref_Traits<T>::tao_duplicate (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <typename T, typename T_var, size_t MAX>
TAO_Bounded_Abstract_Sequence<T,T_var,MAX> &
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::operator= (
    const TAO_Bounded_Abstract_Sequence<T,T_var,MAX> & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      T ** tmp = ACE_reinterpret_cast (T **, 
                                       this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          TAO::Objref_Traits<T>::tao_release (tmp[i]);
          tmp[i] = TAO::Objref_Traits<T>::tao_nil ();
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
            TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::allocbuf (
                rhs.maximum_
              );
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  T ** tmp1 = ACE_reinterpret_cast (T **, 
                                    this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      tmp1[i] = TAO::Objref_Traits<T>::tao_duplicate (tmp2[i]);
    }

  return *this;
}

template <typename T, typename T_var, size_t MAX>
T **
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::allocbuf (CORBA::ULong)
{
  T ** buf = 0;
  ACE_NEW_RETURN (buf, 
                  T * [MAX], 
                  0);

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      buf[i] = TAO::Objref_Traits<T>::tao_nil ();
    }

  return buf;
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::freebuf (T ** buffer)
{
  // How much do we deallocate? Easy! allocbuf() always creates MAX
  // elements and initialize them to T::_nil().  So we can be
  // complaint and call CORBA::release() on each one.
  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      if (buffer[i] != TAO::Objref_Traits<T>::tao_nil ())
        {
          TAO::Objref_Traits<T>::tao_release (buffer[i]);
          buffer[i] = TAO::Objref_Traits<T>::tao_nil ();
        }
    }

  delete [] buffer;
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::_allocate_buffer (
    CORBA::ULong length
  )
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::allocbuf (length);
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  T ** tmp = ACE_reinterpret_cast (T **, 
                                   this->buffer_);
  TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::_shrink_buffer (
    CORBA::ULong nl,
    CORBA::ULong ol
  )
{
  T ** tmp = ACE_reinterpret_cast (T **, 
                                   this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      TAO::Objref_Traits<T>::tao_release (tmp[i]);
      tmp[i] = TAO::Objref_Traits<T>::tao_nil ();
    }
}

#endif /* TAO_VALUETYPE_SEQUENCE_T_C */
