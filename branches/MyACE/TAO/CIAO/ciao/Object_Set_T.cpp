// $Id$

#ifndef CIAO_OBJECT_SET_T_C
#define CIAO_OBJECT_SET_T_C

#include "Object_Set_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "Object_Set_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ciao, Object_Set_T, "$Id$")

template <class T, class T_var>
CIAO::Object_Set<T, T_var>::Object_Set (CORBA::ULong init_capacity,
                                    CORBA::ULong step)
  : capacity_ (init_capacity),
    size_ (0),
    step_ (step)
{
  this->buffer_ = new T_var [this->capacity_];
}

template <class T, class T_var>
CIAO::Object_Set<T, T_var>::~Object_Set ()
{
  delete[] this->buffer_;
}

template <class T, class T_var> void
CIAO::Object_Set<T, T_var>::release ()
{
  CORBA::ULong i = 0;

  for (; i < this->size_; ++i)
    {
      this->buffer_[i] = T::_nil ();
    }
}

template <class T, class T_var> CORBA::Long
CIAO::Object_Set<T, T_var>::add (T *objref)
{
  if (this->size_ == this->capacity_)
    this->grow ();

  this->buffer_[this->size_] = T::_duplicate (objref);
  return this->size_++;
}

template<class T, class T_var> CORBA::Long
CIAO::Object_Set<T, T_var>::remove (T *objref)
{
  if (CORBA::is_nil (objref))
    return -1;

  CORBA::ULong i = 0;

  for (; i < this->size_; ++i)
    if (objref->_is_equivalent (this->buffer_[i].in ())) // _is_equivalent could be unreliable?
      {
        --this->size_;
        if (i != this->size_)
          this->buffer_[i] = this->buffer_[this->size_];

        this->buffer_[this->size_] = T::_nil ();
        return 0;
      }
  return -1;                    // not found.
}

template <class T, class T_var> void
CIAO::Object_Set<T, T_var>::grow (void)
{
  this->capacity_ += this->step_;

  T_var *newbuf = new T_var [this->capacity_];

  CORBA::ULong i = 0;
  for (; i < this->size_; ++i)
    newbuf[i] = this->buffer_[i]; // this will hijack the object ownership

  delete[] this->buffer_;

  this->buffer_ = newbuf;
}

template <class T, class T_var> CORBA::ULong
CIAO::Object_Set<T, T_var>::copy (CORBA::ULong len,
                                  T **buf)
{
  if (buf == 0)
    return 0;

  if (len > this->size_)
    len = this->size_;

  CORBA::ULong i = 0;
  for (; i < len; ++i)
    buf[i] = T::_duplicate (this->buffer_[i].in ());

  return len;
}

template <class T, class T_var> int
CIAO::Object_Set<T, T_var>::object_in_set (T *objref)
{
  if (CORBA::is_nil (objref))  // Don't count nil objref
    return 0;

  CORBA::ULong i = 0;

  for (; i < this->size_; ++i)
    if (objref->_is_equivalent (this->buffer_[i].in ())) // _is_equivalent could be unreliable?
      return -1;

  return 0;
}
#endif /* CIAO_OBJECT_SET_T_C */
