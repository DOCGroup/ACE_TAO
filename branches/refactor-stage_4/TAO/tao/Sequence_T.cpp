// $Id$

#ifndef TAO_SEQUENCE_T_C
#define TAO_SEQUENCE_T_C

#include "tao/Sequence_T.h"
#include "tao/Typecode.h"
#include "tao/Environment.h"

#if !defined (__ACE_INLINE__)
#include "tao/Sequence_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao, 
           Sequence_T, 
           "$Id$")

template <typename T>
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence (
    const TAO_Unbounded_Sequence<T> & rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T * tmp1 = TAO_Unbounded_Sequence<T>::allocbuf (this->maximum_);
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

template <typename T> 
TAO_Unbounded_Sequence<T> &
TAO_Unbounded_Sequence<T>::operator= (const TAO_Unbounded_Sequence<T> & rhs)
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
          T * tmp = ACE_reinterpret_cast (T *,
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

  T * tmp1 = ACE_reinterpret_cast (T *, 
                                   this->buffer_);
  T * const tmp2 = ACE_reinterpret_cast (T * ACE_CAST_CONST, 
                                         rhs.buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      tmp1[i] = tmp2[i];
    }

  return *this;
}

template<typename T>
TAO_Unbounded_Sequence<T>::~TAO_Unbounded_Sequence (void)
{
  this->_deallocate_buffer ();
}

template<typename T> 
void
TAO_Unbounded_Sequence<T>::_allocate_buffer (CORBA::ULong length)
{
  T * tmp = TAO_Unbounded_Sequence<T>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      T * old = ACE_reinterpret_cast (T *,
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

template<typename T> 
void
TAO_Unbounded_Sequence<T>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  T * tmp = ACE_reinterpret_cast (T *,
                                  this->buffer_);

  TAO_Unbounded_Sequence<T>::freebuf (tmp);
  this->buffer_ = 0;
}

template <typename T>  
T *
TAO_Unbounded_Sequence<T>::get_buffer (CORBA::Boolean orphan)
{
  T * result = 0;

  if (orphan == 0)
    {
      // We retain ownership.
      if (this->buffer_ == 0)
        {
          if (this->length_ > 0)
            {
              result = TAO_Unbounded_Sequence<T>::allocbuf (this->length_);
              this->buffer_ = result;
	            this->release_ = 1;
            }
        }
      else
        {
          result = ACE_reinterpret_cast (T *, 
	                                       this->buffer_);
        }
    }
  else
    {
      result = ACE_reinterpret_cast (T *,
	                                   this->buffer_);

      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish ownership.
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }

  return result;
}

// The following function is not inline, to avoid build (template
// instantiation) problems with Sun C++ 4.2 patch 104631-07/SunOS 5.6.
template <typename T> 
T &
TAO_Unbounded_Sequence<T>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->maximum_);
  T * tmp = ACE_reinterpret_cast (T *,
                                  this->buffer_);
  return tmp[i];
}

// The following function is not inline, to avoid build (template
// instantiation) problems with Sun C++ 4.2 patch 104631-07/SunOS 5.6.
template <typename T> 
const T &
TAO_Unbounded_Sequence<T>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->maximum_);
  T * const tmp = ACE_reinterpret_cast (T * ACE_CAST_CONST, 
                                        this->buffer_);
  return tmp[i];
}

template <typename T> 
void
TAO_Unbounded_Sequence<T>::replace (CORBA::ULong max,
                                    CORBA::ULong length,
                                    T * data,
                                    CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;

  if (this->buffer_ && this->release_ == 1)
    {
      T * tmp = ACE_reinterpret_cast (T *, this->buffer_);
      TAO_Unbounded_Sequence<T>::freebuf (tmp);
    }

  this->buffer_ = data;
  this->release_ = release;
}

// ****************************************************************
// Bounded_Sequence
// ****************************************************************

template <typename T, size_t MAX>
TAO_Bounded_Sequence<T, MAX>::TAO_Bounded_Sequence (
    const TAO_Bounded_Sequence<T,MAX> &rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T * tmp1 = TAO_Bounded_Sequence<T,MAX>::allocbuf (MAX);

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

template <typename T, size_t MAX> 
TAO_Bounded_Sequence<T,MAX> &
TAO_Bounded_Sequence<T,MAX>::operator= (
    const TAO_Bounded_Sequence<T,MAX> &rhs
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

  T * tmp1 = ACE_reinterpret_cast (T *, this->buffer_);
  T * const tmp2 = ACE_reinterpret_cast (T * ACE_CAST_CONST, rhs.buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      tmp1[i] = tmp2[i];
    }

  return *this;
}

template<typename T, size_t MAX>
TAO_Bounded_Sequence<T, MAX>::~TAO_Bounded_Sequence (void)
{
  this->_deallocate_buffer ();
}

template<typename T, size_t MAX> 
void
TAO_Bounded_Sequence<T, MAX>::_allocate_buffer (CORBA::ULong)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ = TAO_Bounded_Sequence<T, MAX>::allocbuf (MAX);
}

template<typename T, size_t MAX> 
void
TAO_Bounded_Sequence<T, MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  T * tmp = ACE_reinterpret_cast (T *,
                                  this->buffer_);
  TAO_Bounded_Sequence<T, MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template <typename T, size_t MAX> 
T *
TAO_Bounded_Sequence<T, MAX>::get_buffer (CORBA::Boolean orphan)
{
  T * result = 0;

  if (orphan == 0)
    {
      // We retain ownership.

      if (this->buffer_ == 0)
        {
          result = TAO_Bounded_Sequence<T,MAX>::allocbuf (this->maximum_);
          this->buffer_ = result;
	        this->release_ = 1;
        }
      else
        {
          result =
            ACE_reinterpret_cast (T*, this->buffer_);
        }
    }
  else // if (orphan == 1)
    {
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          result = ACE_reinterpret_cast(T*,this->buffer_);
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }

  return result;
}

template <typename T, size_t MAX>  
void
TAO_Bounded_Sequence<T,MAX>::replace (CORBA::ULong max,
                                      CORBA::ULong length,
                                      T * data,
                                      CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;

  if (this->buffer_ && this->release_ == 1)
    {
      T * tmp = ACE_reinterpret_cast (T *, this->buffer_);
      TAO_Bounded_Sequence<T,MAX>::freebuf (tmp);
    }

  this->buffer_ = data;
  this->release_ = release;
}

// *************************************************************
// class TAO_Object_Manager
// *************************************************************

template <typename T, typename T_var, typename T_life> 
TAO_Object_Manager<T,T_var,T_life> &
TAO_Object_Manager<T,T_var,T_life>::operator= (
    const TAO_Object_Manager<T,T_var,T_life> & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      TAO::Objref_Traits<T>::tao_release (*this->ptr_);
      *this->ptr_ = TAO::Objref_Traits<T>::tao_duplicate (*rhs.ptr_);
    }
  else
    {
      *this->ptr_ = *rhs.ptr_;
    }

  return *this;
}

template <typename T, typename T_var, typename T_life>
TAO_Object_Manager<T,T_var,T_life> &
TAO_Object_Manager<T,T_var,T_life>::operator= (T * p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we will not duplicate the
      // user provided pointer before assigning it to the internal
      // variable.
      TAO::Objref_Traits<T>::tao_release (*this->ptr_);
      *this->ptr_ = p;
    }
  else
    {
      *this->ptr_ = p;
    }

  return *this;
}

template <typename T, typename T_var, typename T_life>
TAO_Object_Manager<T,T_var,T_life> &
TAO_Object_Manager<T,T_var,T_life>::operator= (const T_var & p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we duplicate p's
      // pointer before assigning it to the internal
      // variable.
      TAO::Objref_Traits<T>::tao_release (*this->ptr_);
      *this->ptr_ = TAO::Objref_Traits<T>::tao_duplicate (p.in ());
    }
  else
    {
      *this->ptr_ = p.in ();
    }

  return *this;
}

template <typename T, typename T_var, typename T_life> 
T *&
TAO_Object_Manager<T,T_var,T_life>::out (void)
{
  TAO::Objref_Traits<T>::tao_release (*this->ptr_);
  *this->ptr_ = TAO::Objref_Traits<T>::tao_nil ();
  return *this->ptr_;
}

template <typename T, typename T_var, typename T_life> 
T *
TAO_Object_Manager<T,T_var,T_life>::_retn (void)
{
  T *temp = *this->ptr_;
  *this->ptr_ = TAO::Objref_Traits<T>::tao_nil ();
  return temp;
}

// *************************************************************
// class TAO_Pseudo_Object_Manager
// *************************************************************

template <typename T, typename T_var>
TAO_Pseudo_Object_Manager<T,T_var> &
TAO_Pseudo_Object_Manager<T,T_var>::operator= (
    const TAO_Pseudo_Object_Manager<T,T_var> & rhs
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

template <typename T, typename T_var>
TAO_Pseudo_Object_Manager<T,T_var> &
TAO_Pseudo_Object_Manager<T,T_var>::operator= (T * p)
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

template <typename T, typename T_var>
TAO_Pseudo_Object_Manager<T,T_var> &
TAO_Pseudo_Object_Manager<T,T_var>::operator= (const T_var & p)
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
// Operations for class TAO_Unbounded_Object_Sequence
// *************************************************************

// constructor for unbounded seq
template <typename T, typename T_var, typename T_life, typename T_cast>
TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::
TAO_Unbounded_Object_Sequence (CORBA::ULong maximum)
  : TAO_Unbounded_Base_Sequence (
         maximum,
         TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::allocbuf (
             maximum
           )
       )
{
}

template <typename T, typename T_var, typename T_life, typename T_cast>
TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::
TAO_Unbounded_Object_Sequence (
    const TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast> &rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T ** tmp1 =
        TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::allocbuf (
            this->maximum_
          );
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

template<typename T, typename T_var, typename T_life, typename T_cast>
TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::
~TAO_Unbounded_Object_Sequence (void)
{
  this->_deallocate_buffer ();
}

// Assignment operator.
template <typename T, typename T_var, typename T_life, typename T_cast>
TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>&
TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::
operator= (const TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast> &rhs)
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
          TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::allocbuf (
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
            TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::allocbuf (
                rhs.maximum_
              );
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  T ** tmp1 = ACE_reinterpret_cast (T **, this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      tmp1[i] = TAO::Objref_Traits<T>::tao_duplicate (tmp2[i]);
    }

  return *this;
}

template <typename T, typename T_var, typename T_life, typename T_cast> 
T **
TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::allocbuf (
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

template <typename T, typename T_var, typename T_life, typename T_cast> 
void
TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::freebuf (T ** buffer)
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

template<typename T, typename T_var, typename T_life, typename T_cast> 
void
TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::_allocate_buffer (
    CORBA::ULong length
  )
{
  T ** tmp = 
    TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::allocbuf (length);

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

template<typename T, typename T_var, typename T_life, typename T_cast> 
void
TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::_deallocate_buffer (
    void
  )
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

  TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::freebuf (tmp);
  this->buffer_ = 0;
}

template<typename T, typename T_var, typename T_life, typename T_cast> 
void
TAO_Unbounded_Object_Sequence<T,T_var,T_life,T_cast>::_shrink_buffer (
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
// Operations for class TAO_Bounded_Object_Sequence
// *************************************************************

template <typename T, 
          typename T_var, 
          typename T_life, 
          typename T_cast, 
          size_t MAX>
TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::
TAO_Bounded_Object_Sequence (void)
  : TAO_Bounded_Base_Sequence (
        MAX,
        TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::allocbuf (MAX)
      )
{
}

template <typename T, 
          typename T_var, 
          typename T_life, 
          typename T_cast, 
          size_t MAX>
TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::
TAO_Bounded_Object_Sequence (
    const TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX> & rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T ** tmp1 =
        TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::allocbuf (MAX);

      T ** const tmp2 =
        ACE_reinterpret_cast (T** ACE_CAST_CONST, 
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

template <typename T, 
          typename T_var, 
          typename T_life, 
          typename T_cast, 
          size_t MAX>
TAO_Bounded_Object_Sequence<T, T_var,T_life,T_cast,MAX> &
TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::operator= (
    const TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX> & rhs
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
            TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::allocbuf (
                rhs.maximum_
              );
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  T ** tmp1 = ACE_reinterpret_cast (T **, 
                                    this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i=0; i < rhs.length_; ++i)
    {
      tmp1[i] = TAO::Objref_Traits<T>::tao_duplicate (tmp2[i]);
    }

  return *this;
}

template <typename T, 
          typename T_var, 
          typename T_life, 
          typename T_cast, 
          size_t MAX>
T **
TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::allocbuf (
    CORBA::ULong
  )
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

template <typename T, 
          typename T_var, 
          typename T_life, 
          typename T_cast, 
          size_t MAX>
void
TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::freebuf (T ** buffer)
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

template<typename T, 
         typename T_var, 
         typename T_life, 
         typename T_cast, 
         size_t MAX> 
void
TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::_allocate_buffer (
    CORBA::ULong length
  )
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::allocbuf (length);
}

template<typename T, 
          typename T_var, 
          typename T_life, 
          typename T_cast, 
          size_t MAX>
void
TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::_deallocate_buffer (
    void
  )
{
  if (this->release_ == 0)
    {
      return;
    }

  T ** tmp = ACE_reinterpret_cast (T **, 
                                   this->buffer_);
  TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template<typename T, 
         typename T_var, 
         typename T_life, 
         typename T_cast, 
         size_t MAX>
void
TAO_Bounded_Object_Sequence<T,T_var,T_life,T_cast,MAX>::_shrink_buffer (
    CORBA::ULong nl,
    CORBA::ULong ol
  )
{
  T ** tmp = ACE_reinterpret_cast (T **, this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      TAO::Objref_Traits<T>::tao_release (tmp[i]);
      tmp[i] = TAO::Objref_Traits<T>::tao_nil ();
    }
}

// *************************************************************
// Operations for class TAO_Unbounded_Pseudo_Sequence
// *************************************************************

template <typename T, typename T_var>
TAO_Unbounded_Pseudo_Sequence<T,T_var>::TAO_Unbounded_Pseudo_Sequence (
    CORBA::ULong maximum
  )
  : TAO_Unbounded_Base_Sequence (
        maximum,
        TAO_Unbounded_Pseudo_Sequence<T,T_var>::allocbuf (maximum)
      )
{
}

template <typename T, typename T_var>
TAO_Unbounded_Pseudo_Sequence<T,T_var>::TAO_Unbounded_Pseudo_Sequence (
    const TAO_Unbounded_Pseudo_Sequence<T,T_var> & rhs
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

template <typename T, typename T_var>
TAO_Unbounded_Pseudo_Sequence<T,T_var>::~TAO_Unbounded_Pseudo_Sequence (void)
{
  this->_deallocate_buffer ();
}

template <typename T, typename T_var>
TAO_Unbounded_Pseudo_Sequence<T,T_var> &
TAO_Unbounded_Pseudo_Sequence<T,T_var>::operator= (
    const TAO_Unbounded_Pseudo_Sequence<T,T_var> & rhs
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

  T ** tmp1 = ACE_reinterpret_cast (T **, 
                                    this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      tmp1[i] = T::_duplicate (tmp2[i]);
    }

  return *this;
}

template <typename T, typename T_var>
T **
TAO_Unbounded_Pseudo_Sequence<T,T_var>::allocbuf (CORBA::ULong nelems)
{
  T ** buf = 0;
  ACE_NEW_RETURN (buf, 
                  T * [nelems], 
                  0);

  for (CORBA::ULong i = 0; i < nelems; ++i)
    {
      buf[i] = T::_nil ();
    }

  return buf;
}

template <typename T, typename T_var>
void
TAO_Unbounded_Pseudo_Sequence<T,T_var>::freebuf (T ** buffer)
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
TAO_Unbounded_Pseudo_Sequence<T,T_var>::_allocate_buffer (CORBA::ULong length)
{
  T ** tmp = TAO_Unbounded_Pseudo_Sequence<T,T_var>::allocbuf (length);

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

template <typename T, typename T_var>
void
TAO_Unbounded_Pseudo_Sequence<T,T_var>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

  T ** tmp = ACE_reinterpret_cast (T **, 
                                   this->buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      CORBA::release (tmp[i]);
      tmp[i] = T::_nil ();
    }

  TAO_Unbounded_Pseudo_Sequence<T,T_var>::freebuf (tmp);
  this->buffer_ = 0;
}

template <typename T, typename T_var>
void
TAO_Unbounded_Pseudo_Sequence<T,T_var>::_shrink_buffer (CORBA::ULong nl,
                                                  CORBA::ULong ol)
{
  T ** tmp = ACE_static_cast (T **, this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::release (tmp[i]);
      tmp[i] = T::_nil ();
    }
}

// *************************************************************
// Operations for class TAO_Bounded_Pseudo_Sequence
// *************************************************************

template <typename T, typename T_var, size_t MAX>
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::TAO_Bounded_Pseudo_Sequence (void)
  : TAO_Bounded_Base_Sequence (
        MAX,
        TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::allocbuf (MAX)
      )
{
}

template <typename T, typename T_var, size_t MAX>
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::TAO_Bounded_Pseudo_Sequence (
    const TAO_Bounded_Pseudo_Sequence<T,T_var,MAX> & rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T ** tmp1 =
        TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::allocbuf (MAX);

      T ** const tmp2 =
       ACE_reinterpret_cast (T ** ACE_CAST_CONST, rhs.buffer_);

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

template <typename T, typename T_var, size_t MAX>
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX> &
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::operator= (
    const TAO_Bounded_Pseudo_Sequence<T,T_var,MAX> & rhs
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

  T ** tmp1 = ACE_reinterpret_cast (T **, 
                                    this->buffer_);
  T ** const tmp2 = ACE_reinterpret_cast (T ** ACE_CAST_CONST, 
                                          rhs.buffer_);

  for (CORBA::ULong i=0; i < rhs.length_; ++i)
    {
      tmp1[i] = T::_duplicate (tmp2[i]);
    }

  return *this;
}

template <typename T, typename T_var, size_t MAX>
T **
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::allocbuf (CORBA::ULong)
{
  T ** buf = 0;
  ACE_NEW_RETURN (buf, 
                  T * [MAX], 
                  0);

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      buf[i] = T::_nil ();
    }

  return buf;
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::freebuf (T ** buffer)
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

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::_allocate_buffer (
    CORBA::ULong length
  )
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::allocbuf (length);
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  T ** tmp = ACE_reinterpret_cast (T **, 
                                   this->buffer_);
  TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template <typename T, typename T_var, size_t MAX>
void
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::_shrink_buffer (CORBA::ULong nl,
                                                           CORBA::ULong ol)
{
  T ** tmp = ACE_reinterpret_cast (T **, 
                                   this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::release (tmp[i]);
      tmp[i] = T::_nil ();
    }
}

// *************************************************************
// Operations for class TAO_Unbounded_Array_Sequence
// *************************************************************

template <typename T, typename T_life> 
T *
TAO_Unbounded_Array_Sequence<T,T_life>::allocbuf (CORBA::ULong nelems)
{
  T * buf = 0;
  ACE_NEW_RETURN (buf,
                  T [nelems],
                  0);

  return buf;
}

template <typename T, typename T_life> 
TAO_Unbounded_Array_Sequence<T,T_life>::TAO_Unbounded_Array_Sequence (
    const TAO_Unbounded_Array_Sequence<T,T_life> & rhs
  )
  : TAO_Unbounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T * tmp1 =
        TAO_Unbounded_Array_Sequence<T,T_life>::allocbuf (this->maximum_);

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
      const T * tmp2 = (const T *) rhs.buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
      const T * tmp2 = ACE_reinterpret_cast (const T *,
                                             rhs.buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          T_life::tao_copy (tmp1[i], tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <typename T, typename T_life> 
TAO_Unbounded_Array_Sequence<T,T_life>::~TAO_Unbounded_Array_Sequence (void)
{
  this->_deallocate_buffer ();
}

template <typename T, typename T_life> 
TAO_Unbounded_Array_Sequence<T,T_life> &
TAO_Unbounded_Array_Sequence<T,T_life>::operator= (
    const TAO_Unbounded_Array_Sequence<T,T_life> & rhs
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
          T * tmp = (T *) this->buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
          T * tmp = ACE_reinterpret_cast (T *,
                                          this->buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

          TAO_Unbounded_Array_Sequence<T,T_life>::freebuf (tmp);

          if (rhs.maximum_ == 0)
            {
              this->buffer_ = 0;
            }
          else
            {
              this->buffer_ =
                TAO_Unbounded_Array_Sequence<T,T_life>::allocbuf (
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
            TAO_Unbounded_Array_Sequence<T,T_life>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
      T_life::tao_copy (((T *) this->buffer_)[i],
                        ((const T *) rhs.buffer_)[i]);
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
      T_life::tao_copy (ACE_reinterpret_cast (T *,
                                              this->buffer_)[i],
                   ACE_reinterpret_cast (const T *,
                                         rhs.buffer_)[i]);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
    }

  return *this;
}

template <typename T, typename T_life> 
T *
TAO_Unbounded_Array_Sequence<T,T_life>::get_buffer (CORBA::Boolean orphan)
{
  T * result = 0;

  if (orphan == 0)
    {
      // We retain ownership.

      if (this->buffer_ == 0)
        {
          if (this->length_ > 0)
            {
              result = 
	        TAO_Unbounded_Array_Sequence<T,T_life>::allocbuf (
                    this->length_
                  );
              this->buffer_ = result;
	      this->release_ = 1;
            }
        }
      else
        {
          result =
            ACE_reinterpret_cast (T*, 
	                          this->buffer_);
        }
    }
  else
    {
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          result = ACE_reinterpret_cast (T *,
	                                 this->buffer_);
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }

  return result;
}

template <typename T, typename T_life> 
void
TAO_Unbounded_Array_Sequence<T,T_life>::_allocate_buffer (CORBA::ULong length)
{
  T * tmp = TAO_Unbounded_Array_Sequence<T,T_life>::allocbuf (length);

  if (this->buffer_ != 0)
    {
#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
      T * old = (T *) this->buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
      T * old = ACE_reinterpret_cast (T *,
                                      this->buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          T_life::tao_copy (tmp[i], old[i]);
        }

      if (this->release_)
        {
          delete [] old;
        }
    }

  this->buffer_ = tmp;
}

template <typename T, typename T_life> 
void
TAO_Unbounded_Array_Sequence<T,T_life>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
  T * tmp = (T *) this->buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
  T * tmp = ACE_reinterpret_cast (T *,
                                 this->buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

  TAO_Unbounded_Array_Sequence<T,T_life>::freebuf (tmp);

  this->buffer_ = 0;
}

template <typename T, typename T_life> 
void
TAO_Unbounded_Array_Sequence<T,T_life>::replace (CORBA::ULong max,
				                 CORBA::ULong length,
                                                 T * data,
                                                 CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;

  if (this->buffer_ && this->release_ == 1)
    {
      T * tmp = ACE_reinterpret_cast (T *,
                                      this->buffer_);
      TAO_Unbounded_Array_Sequence<T,T_life>::freebuf (tmp);
    }

  this->buffer_ = data;
  this->release_ = release;
}

// *************************************************************
// Operations for class TAO_Bounded_Array_Sequence
// *************************************************************

template <typename T, typename T_life, size_t MAX> 
T *
TAO_Bounded_Array_Sequence<T,T_life,MAX>::allocbuf (CORBA::ULong)
{
  T * buf = 0;
  ACE_NEW_RETURN (buf,
                  T [MAX],
                  0);

  return buf;
}

template <typename T, typename T_life, size_t MAX> 
TAO_Bounded_Array_Sequence<T,T_life,MAX>::TAO_Bounded_Array_Sequence (
    const TAO_Bounded_Array_Sequence<T,T_life,MAX> &rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      T * tmp1 =
        TAO_Bounded_Array_Sequence<T,T_life,MAX>::allocbuf (MAX);

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
      const T * tmp2 = (const T *) rhs.buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
      const T * tmp2 = ACE_reinterpret_cast (const T *,
                                             rhs.buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          T_life::tao_copy (tmp1[i], tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
  else
    {
      this->buffer_ = 0;
    }
}

template <typename T, typename T_life, size_t MAX> 
TAO_Bounded_Array_Sequence<T,T_life,MAX>::~TAO_Bounded_Array_Sequence (void)
{
  this->_deallocate_buffer ();
}

template <typename T, typename T_life, size_t MAX> 
TAO_Bounded_Array_Sequence<T,T_life,MAX> &
TAO_Bounded_Array_Sequence<T,T_life,MAX>::operator= (
    const TAO_Bounded_Array_Sequence<T,T_life,MAX> & rhs
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
            TAO_Bounded_Array_Sequence<T,T_life,MAX>::allocbuf (rhs.maximum_);
        }
    }

  TAO_Bounded_Base_Sequence::operator= (rhs);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
      T_life::tao_copy (((T *) this->buffer_)[i],
                        ((const T *) rhs.buffer_)[i]);
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
      T_life::tao_copy (ACE_reinterpret_cast (T *,
                                              this->buffer_)[i],
                        ACE_reinterpret_cast (const T *,
                                              rhs.buffer_)[i]);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
    }

  return *this;
}

template <typename T, typename T_life, size_t MAX> 
T *
TAO_Bounded_Array_Sequence<T,T_life,MAX>::get_buffer (CORBA::Boolean orphan)
{
  T * result = 0;

  if (orphan == 0)
    {
      // We retain ownership.

      if (this->buffer_ == 0)
        {
          result = 
	    TAO_Bounded_Array_Sequence<T,T_life,MAX>::allocbuf (this->maximum_);
          this->buffer_ = result;
	  this->release_ = 1;
        }
      else
        {
          result =
            ACE_reinterpret_cast (T *, this->buffer_);
        }
    }
  else
    {
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish ownership.
          result = ACE_reinterpret_cast(T *,this->buffer_);
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }

  return result;
}

template <typename T, typename T_life, size_t MAX> 
void
TAO_Bounded_Array_Sequence<T,T_life,MAX>::freebuf (T * buffer)
{
  if (buffer == 0)
    {
      return;
    }

  delete [] buffer;
}

template <typename T, typename T_life, size_t MAX> 
void
TAO_Bounded_Array_Sequence<T,T_life,MAX>::_allocate_buffer (
    CORBA::ULong length
  )
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_Array_Sequence<T,T_life,MAX>::allocbuf (length);
}

template <typename T, typename T_life, size_t MAX> 
void
TAO_Bounded_Array_Sequence<T,T_life,MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    {
      return;
    }

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
  T * tmp = (T *) this->buffer_;
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
  T * tmp = ACE_reinterpret_cast (T *,
                                  this->buffer_);
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */

  TAO_Bounded_Array_Sequence<T,T_life,MAX>::freebuf (tmp);

  this->buffer_ = 0;
}

template <typename T, typename T_life, size_t MAX> 
void
TAO_Bounded_Array_Sequence<T,T_life,MAX>::replace (CORBA::ULong max,
                                                   CORBA::ULong length,
                                                   T * data,
                                                   CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;

  if (this->buffer_ && this->release_ == 1)
    {
      T * tmp = ACE_reinterpret_cast (T *, 
                                      this->buffer_);
      TAO_Bounded_Array_Sequence<T,T_life,MAX>::freebuf (tmp);
    }

  this->buffer_ = data;
  this->release_ = release;
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
    const TAO_Bounded_String_Sequence<MAX> & rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      char ** tmp1 =
        TAO_Bounded_String_Sequence<MAX>::allocbuf (this->maximum_);

      char ** const tmp2 =
        ACE_reinterpret_cast (char ** ACE_CAST_CONST,
                              rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
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

template<size_t MAX> 
TAO_Bounded_String_Sequence<MAX> &
TAO_Bounded_String_Sequence<MAX>::operator= (
    const TAO_Bounded_String_Sequence<MAX> & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      char ** tmp = ACE_reinterpret_cast (char **, 
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

  char ** tmp1 = ACE_reinterpret_cast  (char **, 
                                      this->buffer_);
  char ** const tmp2 = ACE_reinterpret_cast (char ** ACE_CAST_CONST,
                                             rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    {
      tmp1[i] = CORBA::string_dup (tmp2[i]);
    }

  return *this;
}

template<size_t MAX> 
TAO_SeqElem_String_Manager
TAO_Bounded_String_Sequence<MAX>::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  char ** const tmp = ACE_reinterpret_cast  (char ** ACE_CAST_CONST,
                                           this->buffer_);
  return TAO_SeqElem_String_Manager (tmp + slot,
                                     this->release_);
}

template<size_t MAX> 
char **
TAO_Bounded_String_Sequence<MAX>::allocbuf (CORBA::ULong)
{
  char **buf = 0;
  ACE_NEW_RETURN (buf, 
                  char * [MAX], 
                  0);

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      buf[i] = 0;
    }

  return buf;
}

template<size_t MAX> 
void
TAO_Bounded_String_Sequence<MAX>::freebuf (char ** buffer)
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

template<size_t MAX> 
void
TAO_Bounded_String_Sequence<MAX>::_allocate_buffer (CORBA::ULong /* length */)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_String_Sequence<MAX>::allocbuf (MAX);
}

template<size_t MAX> 
void
TAO_Bounded_String_Sequence<MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  char ** tmp = ACE_reinterpret_cast (char **, 
                                      this->buffer_);
  TAO_Bounded_String_Sequence<MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template<size_t MAX> 
void
TAO_Bounded_String_Sequence<MAX>::_shrink_buffer (CORBA::ULong nl,
                                                  CORBA::ULong ol)
{
  char ** tmp = ACE_reinterpret_cast (char **,
                                      this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::string_free (tmp[i]);
      tmp[i] = 0;
    }
}

template<size_t MAX> 
void
TAO_Bounded_String_Sequence<MAX>::replace (CORBA::ULong length,
                                           char ** data,
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
    const TAO_Bounded_WString_Sequence<MAX> & rhs
  )
  : TAO_Bounded_Base_Sequence (rhs)
{
  if (rhs.buffer_ != 0)
    {
      CORBA::WChar ** tmp1 =
        TAO_Bounded_WString_Sequence<MAX>::allocbuf (this->maximum_);

      CORBA::WChar ** const tmp2 =
        ACE_reinterpret_cast (CORBA::WChar ** ACE_CAST_CONST,
                              rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
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

template<size_t MAX> 
TAO_Bounded_WString_Sequence<MAX> &
TAO_Bounded_WString_Sequence<MAX>::operator= (
    const TAO_Bounded_WString_Sequence<MAX> & rhs
  )
{
  if (this == &rhs)
    {
      return *this;
    }

  if (this->release_)
    {
      CORBA::WChar ** tmp = ACE_reinterpret_cast (CORBA::WChar **,
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

  CORBA::WChar ** tmp1 = ACE_reinterpret_cast (CORBA::WChar **,
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

template<size_t MAX> 
TAO_SeqElem_WString_Manager
TAO_Bounded_WString_Sequence<MAX>::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  CORBA::WChar ** const tmp =
    ACE_reinterpret_cast (CORBA::WChar ** ACE_CAST_CONST,
                          this->buffer_);
  return TAO_SeqElem_WString_Manager (tmp + slot,
                                      this->release_);
}

template<size_t MAX> 
CORBA::WChar **
TAO_Bounded_WString_Sequence<MAX>::allocbuf (CORBA::ULong)
{
  CORBA::WChar ** buf = 0;
  ACE_NEW_RETURN (buf, 
                  CORBA::WChar * [MAX], 
                  0);

  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      buf[i] = 0;
    }

  return buf;
}

template<size_t MAX> 
void
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

template<size_t MAX> 
void
TAO_Bounded_WString_Sequence<MAX>::_allocate_buffer (
    CORBA::ULong /* length */
  )
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_WString_Sequence<MAX>::allocbuf (MAX);
}

template<size_t MAX> 
void
TAO_Bounded_WString_Sequence<MAX>::_deallocate_buffer (void)
{
  if (this->release_ == 0)
    {
      return;
    }

  CORBA::WChar ** tmp = ACE_reinterpret_cast (CORBA::WChar **,
                                              this->buffer_);
  TAO_Bounded_WString_Sequence<MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template<size_t MAX> 
void
TAO_Bounded_WString_Sequence<MAX>::_shrink_buffer (CORBA::ULong nl,
                                                   CORBA::ULong ol)
{
  CORBA::WChar ** tmp = ACE_reinterpret_cast (CORBA::WChar **,
                                              this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::wstring_free (tmp[i]);
      tmp[i] = 0;
    }
}

template<size_t MAX> 
void
TAO_Bounded_WString_Sequence<MAX>::replace (CORBA::ULong length,
                                            CORBA::WChar ** data,
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
