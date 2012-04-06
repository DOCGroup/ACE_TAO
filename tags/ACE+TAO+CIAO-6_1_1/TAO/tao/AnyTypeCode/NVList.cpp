// $Id$


// Implementation of Named Value List and NamedValue classes

#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/BoundsC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Impl.h"

#include "tao/CORBA_String.h"
#include "tao/CDR.h"
#include "tao/debug.h"
#include "tao/SystemException.h"

#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/CORBA_macros.h"

#if !defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/NVList.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Reference counting for DII Request object

CORBA::ULong
CORBA::NamedValue::_incr_refcount (void)
{
  return ++this->refcount_;
}

CORBA::ULong
CORBA::NamedValue::_decr_refcount (void)
{
  CORBA::ULong const new_count = --this->refcount_;

  if (new_count == 0)
    delete this;

  return new_count;
}

CORBA::NamedValue::~NamedValue (void)
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
CORBA::NVList::_incr_refcount (void)
{
  return ++this->refcount_;
}

CORBA::ULong
CORBA::NVList::_decr_refcount (void)
{
  CORBA::ULong const new_count = --this->refcount_;

  if (new_count == 0)
    delete this;

  return new_count;
}

CORBA::NVList::~NVList (void)
{
  // initialize an iterator and delete each NamedValue
  ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> iter (this->values_);

  for (iter.first (); !iter.done (); iter.advance ())
    {
      CORBA::NamedValue_ptr *nv = 0;
      (void) iter.next (nv);
      delete *nv;
    }

  this->max_ = 0;

  // Remove the CDR stream if it is present.
  delete this->incoming_;
}

// add an element and just initialize its flags
CORBA::NamedValue_ptr
CORBA::NVList::add (CORBA::Flags flags)
{
  // call the helper to allocate a NamedValue element (if necessary)
  return this->add_element (flags);
}

// add an element and just initialize its flags and name
CORBA::NamedValue_ptr
CORBA::NVList::add_item (const char *name, CORBA::Flags flags)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags);

  if (nv)
    {
      // now initialize the fields
      nv->name_ = CORBA::string_dup (name);
      return nv;
    }
  else
    {
      return 0;
    }
}

// add a value. If necessary, increment the list
CORBA::NamedValue_ptr
CORBA::NVList::add_value (const char *name,
                          const CORBA::Any &value,
                          CORBA::Flags flags)
{
  // Call the helper to allocate a NamedValue element.
  CORBA::NamedValue_ptr nv = this->add_element (flags);

  if (nv)
    {
      nv->name_ = CORBA::string_dup (name);

      // With the original Any implementation, we had alternate
      // paths for the assignment based on the IN_COPY_VALUE flag.
      // Now that the Any's contained Any_Impl is refcounted, the
      // distinction between the ORB "copying" or "borrowing" the
      // memory is irrelevant. The IN_COPY_VALUE flag was not
      // checked anywhere else in the ORB anyway.
      nv->any_ = value;
      return nv;
    }
  else
    {
      return 0;
    }
}

// add an element and just initialize its flags and name
CORBA::NamedValue_ptr
CORBA::NVList::add_item_consume (char *name, CORBA::Flags flags)
{

  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags);

  if (nv)
    {
      // now initialize the fields

      // consume the name
      nv->name_ = name;
      return nv;
    }
  else
    {
      return 0;
    }
}

// add a value. If necessary, increment the list
CORBA::NamedValue_ptr
CORBA::NVList::add_value_consume (char * name,
                                  CORBA::Any * value,
                                  CORBA::Flags flags)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags);

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
    {
      return 0;
    }
}

//CORBA::Status
void
CORBA::NVList::remove (CORBA::ULong /* n */)
{
  // not implemented
  // @@ (ASG) - TODO
}

// Helper method
CORBA::NamedValue_ptr
CORBA::NVList::add_element (CORBA::Flags flags)
{
  this->evaluate ();

  if (ACE_BIT_DISABLED (flags,
                        CORBA::ARG_IN | CORBA::ARG_OUT | CORBA::ARG_INOUT))
    {
      throw ::CORBA::BAD_PARAM ();
    }

  CORBA::NamedValue_ptr nv;

  // allocate a new NamedValue
  ACE_NEW_THROW_EX (nv,
                    CORBA::NamedValue,
                    CORBA::NO_MEMORY ());

  // set the flags and enqueue in the queue
  nv->flags_ = flags;

  if (this->values_.enqueue_tail (nv) == -1)
    {
      delete nv;
      return 0;
    }

  ++this->max_;
  return nv; // success
}

// return the item at location n
CORBA::NamedValue_ptr
CORBA::NVList::item (CORBA::ULong n)
{
  this->evaluate ();

  if (n >= this->max_)
    {
      throw ::CORBA::Bounds ();
    }

  CORBA::NamedValue_ptr *nv = 0;

  this->values_.get (nv, n);
  return *nv;
}

void
CORBA::NVList::_tao_incoming_cdr (TAO_InputCDR &cdr,
                                  int flag,
                                  bool &lazy_evaluation)
{
  // If the list is empty then using lazy evaluation is the only
  // choice.
  // @@ There are other cases where we can use lazy evaluation, for
  //    example if the list is not empty but the anys own all their
  //    objects.
  if (lazy_evaluation == false && this->max_ == 0)
    {
      lazy_evaluation = true;
    }

  if (lazy_evaluation == false)
    {
      this->_tao_decode (cdr, flag);
      return;
    }

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->incoming_ != 0)
    {
      delete this->incoming_;
      this->incoming_ = 0;
    }

  ACE_NEW (this->incoming_, TAO_InputCDR (cdr));
  this->incoming_flag_ = flag;
}

void
CORBA::NVList::_tao_encode (TAO_OutputCDR &cdr, int flag)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             ace_mon,
             this->lock_);

  if (this->incoming_ != 0)
    {
      if (this->max_ == 0)
        {
          // The list is empty aggressively reduce copies and just send
          // the CDR stream, we assume that
          // TAO_Server_Request::init_reply
          // has inserted appropriated padding already to make this
          // operation correct
          cdr.write_octet_array_mb (this->incoming_->start ());
          return;
        }

      // Then unmarshal each "in" and "inout" parameter.
      ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> i (this->values_);

      for (i.first (); !i.done (); i.advance ())
        {
          CORBA::NamedValue_ptr *item = 0;
          (void) i.next (item);

          CORBA::NamedValue_ptr nv = *item;

          if (ACE_BIT_DISABLED (nv->flags (), flag))
            {
              continue;
            }

          if (TAO_debug_level > 3)
            {
              const char* arg = nv->name ();

              if (arg == 0)
                {
                  arg = "(nil)";
                }

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("NVList::_tao_encode - parameter <%C>\n"),
                          arg));
            }
          CORBA::TypeCode_ptr tc = nv->value ()->_tao_get_typecode ();
          (void) TAO_Marshal_Object::perform_append (tc,
                                                     this->incoming_,
                                                     &cdr);
        }

      delete this->incoming_;
      this->incoming_ = 0;
      return;
    }

  // The list is already evaluated, we cannot optimize the copies, go
  // ahead with the slow way to do things.

  // Then marshal each "in" and "inout" parameter.
  ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> i (this->values_);

  for (i.first (); !i.done (); i.advance ())
    {
      CORBA::NamedValue_ptr *item = 0;
      (void) i.next (item);

      CORBA::NamedValue_ptr nv = *item;

      if (ACE_BIT_DISABLED (nv->flags (), flag))
        {
          continue;
        }

      nv->value ()->impl ()->marshal_value (cdr);
    }
}

void
CORBA::NVList::_tao_decode (TAO_InputCDR &incoming, int flag)
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) : NVList::_tao_decode\n")));
    }

  // Then unmarshal each "in" and "inout" parameter.
  ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> i (this->values_);

  for (i.first (); !i.done (); i.advance ())
    {
      CORBA::NamedValue_ptr *item = 0;
      (void) i.next (item);

      CORBA::NamedValue_ptr nv = *item;

      // check if it is an in or inout parameter
      // @@ this is where we assume that the NVList is coming from
      //    a Server-side request, we could probably handle both
      //    cases with a flag, but there is no clear need for that.
      if (ACE_BIT_DISABLED (nv->flags (), flag))
        {
          continue;
        }

      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) : NVList::_tao_decode - %C\n"),
                      nv->name ()? nv->name () : "(no name given)" ));
        }

      CORBA::Any_ptr any = nv->value ();
      any->impl ()->_tao_decode (incoming
                                );
    }
}

ptrdiff_t
CORBA::NVList::_tao_target_alignment (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    ACE_CDR::MAX_ALIGNMENT);

  if (this->incoming_ == 0)
    {
      return ACE_CDR::MAX_ALIGNMENT;
    }

  const char* rd = this->incoming_->start ()->rd_ptr ();
  ptrdiff_t t = ptrdiff_t (rd) % ACE_CDR::MAX_ALIGNMENT;

  if (t < 0)
    {
      t += ACE_CDR::MAX_ALIGNMENT;
    }

  return t;
}

void
CORBA::NVList::evaluate (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->incoming_ != 0)
    {
      auto_ptr<TAO_InputCDR> incoming (this->incoming_);
      this->incoming_ = 0;

      this->_tao_decode (*(incoming.get ()), this->incoming_flag_);
    }
}

CORBA::Boolean
CORBA::NVList::_lazy_has_arguments (void) const
{
  if (this->incoming_ != 0)
    {
      return this->incoming_->length () <= 1 ? false : true;
    }
  else
    {
      return this->count () <= 1 ? false : true;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
