// $Id$

#include "ExceptionList.h"

ACE_RCSID(DynamicInterface, ExceptionList, "$Id$")

#include "tao/Typecode.h"
#include "tao/Environment.h"

#if !defined (__ACE_INLINE__)
# include "ExceptionList.inl"
#endif /* __ACE_INLINE__ */

CORBA_ExceptionList::CORBA_ExceptionList (CORBA::ULong len,
                                          CORBA::TypeCode_ptr *tc_list)
  : ref_count_ (1)
{
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      this->add (tc_list [i]);
    }
}

CORBA_ExceptionList::~CORBA_ExceptionList (void)
{
  for (CORBA::ULong i = 0; i < this->count (); ++i)
    {
      CORBA::TypeCode_ptr *tc = 0;

      if (this->tc_list_.get (tc, i) == -1)
        {
          return;
        }

      CORBA::release (*tc);
    }
}

void
CORBA_ExceptionList::add (CORBA::TypeCode_ptr tc)
{
  this->tc_list_.enqueue_tail (CORBA::TypeCode::_duplicate (tc));
}

void
CORBA_ExceptionList::add_consume (CORBA::TypeCode_ptr tc)
{
  this->tc_list_.enqueue_tail (tc);
}

CORBA::TypeCode_ptr
CORBA_ExceptionList::item (CORBA::ULong slot
                           TAO_ENV_ARG_DECL)
{
  CORBA::TypeCode_ptr *tc = 0;

  if (this->tc_list_.get (tc, slot) == -1)
    {
      ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                        CORBA::TypeCode::_nil ());
    }
  else
    {
      return CORBA::TypeCode::_duplicate (*tc);
    }
}

void
CORBA_ExceptionList::remove (CORBA::ULong
                             TAO_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA_ExceptionList_ptr
CORBA_ExceptionList::_duplicate (void)
{
  this->_incr_refcnt ();
  return this;
}

void
CORBA_ExceptionList::_destroy (void)
{
  this->_decr_refcnt ();
}

void
CORBA_ExceptionList::_incr_refcnt (void)
{
  this->ref_count_++;
}

void
CORBA_ExceptionList::_decr_refcnt (void)
{
  this->ref_count_--;

  if (this->ref_count_ == 0)
    {
      delete this;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong>;
template class ACE_Atomic_Op_Ex<TAO_SYNCH_MUTEX, CORBA::ULong>;
template class ACE_Node<CORBA::TypeCode_ptr>;
template class ACE_Unbounded_Queue<CORBA::TypeCode_ptr>;
template class ACE_Unbounded_Queue_Iterator<CORBA::TypeCode_ptr>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong>
#pragma instantiate ACE_Atomic_Op_Ex<TAO_SYNCH_MUTEX, CORBA::ULong>
#pragma instantiate ACE_Node<CORBA::TypeCode_ptr>
#pragma instantiate ACE_Unbounded_Queue<CORBA::TypeCode_ptr>
#pragma instantiate ACE_Unbounded_Queue_Iterator<CORBA::TypeCode_ptr>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
