// $Id$

#include "tao/DynamicInterface/ExceptionList.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/SystemException.h"

#if !defined (__ACE_INLINE__)
# include "tao/DynamicInterface/ExceptionList.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::ExceptionList::ExceptionList (CORBA::ULong len,
                                     CORBA::TypeCode_ptr *tc_list)
  : ref_count_ (1)
{
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      this->add (tc_list [i]);
    }
}

CORBA::ExceptionList::~ExceptionList (void)
{
  for (CORBA::ULong i = 0; i < this->count (); ++i)
    {
      CORBA::TypeCode_ptr *tc = 0;

      if (this->tc_list_.get (tc, i) == -1)
        {
          return;
        }

      ::CORBA::release (*tc);
    }
}

void
CORBA::ExceptionList::add (CORBA::TypeCode_ptr tc)
{
  this->tc_list_.enqueue_tail (CORBA::TypeCode::_duplicate (tc));
}

void
CORBA::ExceptionList::add_consume (CORBA::TypeCode_ptr tc)
{
  this->tc_list_.enqueue_tail (tc);
}

CORBA::TypeCode_ptr
CORBA::ExceptionList::item (CORBA::ULong slot)
{
  CORBA::TypeCode_ptr *tc = 0;

  if (this->tc_list_.get (tc, slot) == -1)
    {
      throw ::CORBA::TypeCode::Bounds ();
    }
  else
    {
      return CORBA::TypeCode::_duplicate (*tc);
    }
}

void
CORBA::ExceptionList::remove (CORBA::ULong)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

CORBA::ExceptionList_ptr
CORBA::ExceptionList::_duplicate (void)
{
  this->_incr_refcount ();
  return this;
}

void
CORBA::ExceptionList::_destroy (void)
{
  this->_decr_refcount ();
}

void
CORBA::ExceptionList::_incr_refcount (void)
{
  ++this->ref_count_;
}

void
CORBA::ExceptionList::_decr_refcount (void)
{
  CORBA::ULong const refcount = --this->ref_count_;

  if (refcount == 0)
    {
      delete this;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
