// $Id$

// Implementation of Named Value List and NamedValue classes

#include "tao/NVList.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/Exception.h"
#include "tao/Environment.h"
#include "tao/ORB.h"

#if !defined (__ACE_INLINE__)
# include "tao/NVList.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, NVList, "$Id$")

// Reference counting for DII Request object

CORBA::ULong
CORBA_NamedValue::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
CORBA_NamedValue::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

CORBA_NamedValue::~CORBA_NamedValue (void)
{
  if (this->name_)
    {
      CORBA::string_free (this->name_);
      this->name_ = 0;
    }
  // the any will be destroyed by itself
}

// ****************************************************************

CORBA::ULong
CORBA_NVList::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
CORBA_NVList::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

CORBA_NVList::~CORBA_NVList (void)
{
  // initialize an iterator and delete each NamedValue
  ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> iter (this->values_);

  for (iter.first (); !iter.done (); iter.advance ())
    {
      CORBA::NamedValue_ptr *nv;
      (void) iter.next (nv);
      delete *nv;
    }

  this->max_ = 0;
}

// add an element and just initialize its flags
CORBA::NamedValue_ptr
CORBA_NVList::add (CORBA::Flags flags,
                   CORBA::Environment &env)
{
  // call the helper to allocate a NamedValue element (if necessary)
  return this->add_element (flags, env);
}

// add an element and just initialize its flags and name
CORBA::NamedValue_ptr
CORBA_NVList::add_item (const char *name,
                        CORBA::Flags flags,
                        CORBA::Environment &env)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, env);

  if (nv)
    {
      // now initialize the fields
      nv->name_ = CORBA::string_dup (name);
      return nv;
    }
  else
    return 0;
}

// add a value. If necessary, increment the list
CORBA::NamedValue_ptr
CORBA_NVList::add_value (const char *name,
                         const CORBA::Any &value,
                         CORBA::Flags flags,
                         CORBA::Environment &env)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, env);

  if (nv)
    {
      // now initialize the fields
      nv->name_ = CORBA::string_dup (name);
      if (ACE_BIT_ENABLED (flags, CORBA::IN_COPY_VALUE))
        // IN_COPY_VALUE means that the parameter is not "borrowed" by
        // the ORB, but rather that the ORB copies its value.
        //
        // Initialize the newly allocated memory using a copy
        // constructor that places the new "Any" value at just the right
        // place, and makes a "deep copy" of the data.
        nv->any_ = value;
      else
        {
          // The normal behaviour for parameters is that the ORB "borrows"
          // their memory for the duration of calls.
          //
          if (value.value_)
            {
              nv->any_.replace (value.type_,
                                value.value_,
                                0,
                                env);
            }
          else
            {
              nv->any_._tao_replace (value.type_,
                                     value.cdr_,
                                     0,
                                     env);
            }
        }
      return nv;
    }
  else
    return 0;
}

// add an element and just initialize its flags and name
CORBA::NamedValue_ptr
CORBA_NVList::add_item_consume (char *name,
                                CORBA::Flags flags,
                                CORBA::Environment &env)
{

  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, env);

  if (nv)
    {
      // now initialize the fields

      // consume the name
      nv->name_ = name;
      return nv;
    }
  else
    return 0;
}

// add a value. If necessary, increment the list
CORBA::NamedValue_ptr
CORBA_NVList::add_value_consume (char * name,
                                 CORBA::Any * value,
                                 CORBA::Flags flags,
                                 CORBA::Environment & env)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, env);

  if (nv)
    {
      // now initialize the fields

      // consume name
      nv->name_ = name;

      // consume the value @@ (ASG) have we? we may need to destroy
      // the in parameter
      nv->any_ = *value;
      return nv;
    }
  else
    return 0;
}

//CORBA::Status
void
CORBA_NVList::remove (CORBA::ULong /*n*/, CORBA::Environment &/*env*/)
{
  // not implemented
  // @@ (ASG) - TODO
}

// Helper method
CORBA::NamedValue_ptr
CORBA_NVList::add_element (CORBA::Flags flags, CORBA::Environment &env)
{
  if (ACE_BIT_DISABLED (flags,
                        CORBA::ARG_IN | CORBA::ARG_OUT | CORBA::ARG_INOUT))
    {
      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
      return 0;
    }

  CORBA::NamedValue_ptr nv;

  // allocate a new NamedValue
  ACE_NEW_RETURN (nv, CORBA::NamedValue, 0);

  // set the flags and enqueue in the queue
  nv->flags_ = flags;
  if (this->values_.enqueue_tail (nv) == -1)
    {
      delete nv;
      return 0;
    }

  this->max_++;
  return nv; // success
}

// return the item at location n
CORBA::NamedValue_ptr
CORBA_NVList::item (CORBA::ULong n, CORBA::Environment &env)
{
  if (n >= this->max_) // 0 based indexing
    {
      env.exception (new CORBA::TypeCode::Bounds ());
      return 0;
    }
  else
    {
      CORBA::NamedValue_ptr *nv;

      this->values_.get (nv, n);
      return *nv;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<CORBA::NamedValue_ptr>;
template class ACE_Unbounded_Queue<CORBA::NamedValue_ptr>;
template class ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<CORBA::NamedValue_ptr>
#pragma instantiate ACE_Unbounded_Queue<CORBA::NamedValue_ptr>
#pragma instantiate ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_MINIMUM_CORBA */
