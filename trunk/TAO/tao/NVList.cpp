// $Id$

// Implementation of Named Value List and NamedValue classes

#include "tao/NVList.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/Exception.h"
#include "tao/Environment.h"
#include "tao/ORB.h"
#include "tao/debug.h"

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

  // Remove the CDR stream if it is present.
  delete this->incoming_;
}

// add an element and just initialize its flags
CORBA::NamedValue_ptr
CORBA_NVList::add (CORBA::Flags flags,
                   CORBA::Environment &ACE_TRY_ENV)
{
  // call the helper to allocate a NamedValue element (if necessary)
  return this->add_element (flags, ACE_TRY_ENV);
}

// add an element and just initialize its flags and name
CORBA::NamedValue_ptr
CORBA_NVList::add_item (const char *name,
                        CORBA::Flags flags,
                        CORBA::Environment &ACE_TRY_ENV)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

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
                         CORBA::Environment &ACE_TRY_ENV)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
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
                                ACE_TRY_ENV);
            }
          else
            {
              nv->any_._tao_replace (value.type_,
                                     value.cdr_,
                                     ACE_TRY_ENV);
            }
          ACE_CHECK_RETURN (0);
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
                                CORBA::Environment &ACE_TRY_ENV)
{

  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

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
                                 CORBA::Environment & ACE_TRY_ENV)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

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
CORBA_NVList::add_element (CORBA::Flags flags,
                           CORBA::Environment &ACE_TRY_ENV)
{
  this->compute_list (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::NamedValue::_nil ());

  if (ACE_BIT_DISABLED (flags,
                        CORBA::ARG_IN | CORBA::ARG_OUT | CORBA::ARG_INOUT))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

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
CORBA_NVList::item (CORBA::ULong n, CORBA::Environment &ACE_TRY_ENV)
{
  this->compute_list (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::NamedValue::_nil ());

  if (n >= this->max_) // 0 based indexing
    ACE_THROW_RETURN (CORBA::TypeCode::Bounds (), 0);

  CORBA::NamedValue_ptr *nv;

  this->values_.get (nv, n);
  return *nv;
}

void
CORBA_NVList::_tao_incoming_cdr (const TAO_InputCDR &cdr,
                                 int flag)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_);
  if (this->incoming_ != 0)
    {
      delete this->incoming_;
      this->incoming_ = 0;
    }

  ACE_NEW (this->incoming_, TAO_InputCDR (cdr));
  this->incoming_flag_ = flag;
}

void
CORBA_NVList::_tao_encode (TAO_OutputCDR &cdr,
                           TAO_ORB_Core *orb_core,
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_);
  if (this->incoming_ != 0)
    {
      // Then unmarshal each "in" and "inout" parameter.
      ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> i (this->values_);

      for (i.first (); !i.done (); i.advance ())
        {
          CORBA::NamedValue_ptr *item;
          (void) i.next (item);

          CORBA::NamedValue_ptr nv = *item;

          if (ACE_BIT_DISABLED (nv->flags (), this->incoming_flag_))
            continue;

          if (TAO_debug_level > 3)
            {
              const char* arg = nv->name ();
              if (arg == 0)
                arg = "(nil)";

              ACE_DEBUG ((LM_DEBUG,
                          "NVList::_tao_encode - parameter <%s>\n",
                          arg));
            }
          cdr.append (nv->value ()->type_,
                      this->incoming_,
                      ACE_TRY_ENV);
          ACE_CHECK;
        }

      delete this->incoming_;
      this->incoming_ = 0;
      return;
    }

  // The list is already evaluated, we cannot optimize the copies, go
  // ahead with the slow way to do things.

  // Then unmarshal each "in" and "inout" parameter.
  ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> i (this->values_);

  for (i.first (); !i.done (); i.advance ())
    {
      CORBA::NamedValue_ptr *item;
      (void) i.next (item);

      CORBA::NamedValue_ptr nv = *item;

      if (ACE_BIT_DISABLED (nv->flags (), this->incoming_flag_))
        continue;

      // If the Any owns the data, then we have allocated space.
      if (nv->value ()->any_owns_data_)
        {
          (void) cdr.encode (nv->value ()->type_,
                             nv->value ()->value_, 0,
                             ACE_TRY_ENV);
          ACE_CHECK;
        }
      else
        {
          TAO_InputCDR in (nv->value ()->cdr_,
                           TAO_ENCAP_BYTE_ORDER,
                           orb_core);
          cdr.append (nv->value ()->type_, &in, ACE_TRY_ENV);
          ACE_CHECK;
        }
    }
}

void
CORBA_NVList::compute_list (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_);
  if (this->incoming_ == 0)
    return;

  if (TAO_debug_level > 3)
    ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) : NVList::compute_list\n"));

  // Then unmarshal each "in" and "inout" parameter.
  ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> i (this->values_);

  for (i.first (); !i.done (); i.advance ())
    {
      CORBA::NamedValue_ptr *item;
      (void) i.next (item);

      CORBA::NamedValue_ptr nv = *item;

      // check if it is an in or inout parameter
      // @@ this is where we assume that the NVList is coming from
      //    a Server-side request, we could probably handle both
      //    cases with a flag, but there is no clear need for that.
      if (ACE_BIT_DISABLED (nv->flags (), this->incoming_flag_))
        continue;

      if (TAO_debug_level > 3)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) : NVList::compute_list - %s\n",
                    nv->name ()?nv->name ():"(no name given)" ));

      CORBA::Any_ptr any = nv->value ();
      CORBA::TypeCode_var tc = any->type ();

      // @@ (JP) The following code depends on the fact that
      //         TO_InputCDR does not contain chained message blocks,
      //         otherwise <begin> and <end> could be part of
      //         different buffers!

      // This will be the start of a new message block.
      char *begin = this->incoming_->rd_ptr ();

      // Skip over the next aregument.
      CORBA::TypeCode::traverse_status status =
        this->incoming_->skip (tc.in (), ACE_TRY_ENV);
      ACE_CHECK;

      if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          if (TAO_debug_level > 0)
            {
              const char* param_name = nv->name ();
              if (param_name == 0)
                param_name = "(no name given)";

              ACE_ERROR ((LM_ERROR,
                          "CORBA_NVList::compute_list - problem while"
                          " decoding parameter <%s>\n", param_name));
            }
          return;
        }

      // This will be the end of the new message block.
      char *end = this->incoming_->rd_ptr ();

      // Allocate the new message block and set its endpoints.
      ACE_Message_Block cdr (end - begin);

      cdr.rd_ptr (begin);

      cdr.wr_ptr (end);

      // Stick it into the Any. It gets duplicated there.
      any->_tao_replace (tc.in (),
                         &cdr,
                         ACE_TRY_ENV);
      ACE_CHECK;
    }

  delete this->incoming_;
  this->incoming_ = 0;
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
