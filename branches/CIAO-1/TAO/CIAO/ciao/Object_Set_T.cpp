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

template <class COBJ>
CIAO::Object_Set<COBJ>::Object_Set (CORBA::ULong init_capacity,
                                    CORBA::ULong step)
  : capacity_ (init_capacity),
    size_ (0),
    step_ (step)
{
  this->buffer_ = new COBJ::_var_type [this->capacity_];
}

template <class COBJ>
CIAO::Object_Set<COBJ>::~Object_Set ()
{
  delete[] this->buffer_;
}

template <class COBJ> void
CIAO::Object_Set<COBJ>::release ()
{
  CORBA::ULong i = 0;

  for (; i < this->size_; ++i)
    {
      this->buffer_[i] = COBJ::_nil ();
    }
}

template <class COBJ> CORBA::Long
CIAO::Object_Set<COBJ>::add (COBJ::_ptr_type objref)
{
  if (this->size_ == this->capacity_)
    this->grow ();

  this->buffer_[this->size_] = COBJ::_duplicate (objref);
  return this->size_++;
}

template<class COBJ> CORBA::Long
CIAO::Object_Set<COBJ>::remove (COBJ::_ptr_type objref)
{
  if (CORBA::_is_nil (objref))
    return -1;

  CORBA::ULong i = 0;

  for (; i < this->size_; ++i)
    if (objref->_is_equivalent (this->buffer_[i])) // _is_equivalent could be unreliable?
      {
        --this->size_;
        if (i != this->size_)
          this->buffer_[i] = this->buffer_[this->size_];

        this->buffer_[this->size_] = COBJ::_nil ();
        return 0;
      }
  return -1;                    // not found.
}

template <class COBJ> void
CIAO::Object_Set<COBJ>::grow (void)
{
  this->capacity_ += this->step_;

  COBJ::_var_type *newbuf = new CORBA::_var_type [this->capacity_];

  CORBA::ULong i = 0;
  for (; i < this->size_; ++i)
    newbuf[i] = this->buffer_[i]; // this will hijack the object ownership

  delete[] this->buffer_;

  this->buffer_ = newbuf;
}

template <class COBJ> CORBA::ULong
CIAO::Object_Set<COBJ>::copy (CORBA::ULong len,
                              COBJ **buf)
{
  if (buf == 0)
    return 0;

  if (len > this->size_)
    len = this->size_;

  CORBA::ULong i = 0;
  for (; i < len; ++i)
    buf[i] = COBJ::_duplicate (this->buffer_[i].in ());

  return len;
}
#endif /* CIAO_OBJECT_SET_T_C */
