#include "tao/DynamicInterface/ExceptionList.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/SystemException.h"

#include <new>

#if !defined (__ACE_INLINE__)
# include "tao/DynamicInterface/ExceptionList.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::ExceptionList::ExceptionList (CORBA::ULong len,
                                     CORBA::TypeCode_ptr *tc_list)
  : refcount_ (1)
{
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      this->add (tc_list [i]);
    }
}

CORBA::ExceptionList::~ExceptionList ()
{
  for (CORBA::TypeCode_ptr tc : this->tc_list_)
    {
      ::CORBA::release (tc);
    }
}

void
CORBA::ExceptionList::add (CORBA::TypeCode_ptr tc)
{
  CORBA::TypeCode_ptr duplicate = CORBA::TypeCode::_duplicate (tc);
  try
    {
      this->tc_list_.push_back (duplicate);
    }
  catch (std::bad_alloc const&)
    {
      ::CORBA::release (duplicate);
      throw CORBA::NO_MEMORY ();
    }
}

void
CORBA::ExceptionList::add_consume (CORBA::TypeCode_ptr tc)
{
  try
    {
      this->tc_list_.push_back (tc);
    }
  catch (std::bad_alloc const&)
    {
      ::CORBA::release (tc);
      throw CORBA::NO_MEMORY ();
    }
}

CORBA::TypeCode_ptr
CORBA::ExceptionList::item (CORBA::ULong slot)
{
  if (slot >= this->tc_list_.size ())
    {
      throw ::CORBA::TypeCode::Bounds ();
    }

  return CORBA::TypeCode::_duplicate (this->tc_list_[slot]);
}

void
CORBA::ExceptionList::remove (CORBA::ULong)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

CORBA::ExceptionList_ptr
CORBA::ExceptionList::_duplicate ()
{
  this->_incr_refcount ();
  return this;
}

void
CORBA::ExceptionList::_destroy ()
{
  this->_decr_refcount ();
}

void
CORBA::ExceptionList::_incr_refcount ()
{
  ++this->refcount_;
}

void
CORBA::ExceptionList::_decr_refcount ()
{
  CORBA::ULong const refcount = --this->refcount_;

  if (refcount == 0)
    {
      delete this;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
