// $Id$


// Portions of this file are:
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//

#include "tao/Any.h"
#include "tao/Typecode.h"
#include "tao/Marshal.h"
#include "tao/ORB_Core.h"
#include "tao/Object.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Any.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Any, "$Id$")


CORBA::TypeCode_ptr
CORBA_Any::type (void) const
{
  return CORBA::TypeCode::_duplicate (this->type_.in ());
}

// Will replace if the typecode arg is an alias for the existing one -
// otherwise raises an exception.

void
CORBA_Any::type (CORBA::TypeCode_ptr tc
                 ACE_ENV_ARG_DECL)
{
  CORBA::Boolean equiv = this->type_->equivalent (tc
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (equiv)
    {
      this->type_ = CORBA::TypeCode::_duplicate (tc);
    }
  else
    {
      ACE_THROW (CORBA::BAD_TYPECODE ());
    }
}

// This method is deprecated and will eventually be removed from the
// spec. It is included here for backward compatibility and its return
// value may NOT be cast to anything useful. It will only tell whether the
// Any contains a value or not. Use of >>= is recommended for anything else.

const void *
CORBA_Any::value (void) const
{
  if (this->any_owns_data_)
    {
      return this->value_;
    }
  else
    {
      return this->cdr_;
    }
}

// Default "Any" constructor -- initializes to nulls per the
// OMG C++ mapping.

// NOTE: null (zero) typecode pointers are also treated as the null
// typecode ...

CORBA_Any::CORBA_Any (void)
  : type_ (CORBA::TypeCode::_duplicate (CORBA::_tc_null)),
    byte_order_ (TAO_ENCAP_BYTE_ORDER),
    cdr_ (0),
    any_owns_data_ (0),
    contains_local_ (0),
    value_ (0),
    destructor_ (0)
{
}

CORBA_Any::CORBA_Any (CORBA::TypeCode_ptr tc)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    byte_order_ (TAO_ENCAP_BYTE_ORDER),
    cdr_ (0),
    any_owns_data_ (0),
    contains_local_ (0),
    value_ (0),
    destructor_ (0)
{
}

// Constructor using a message block.
CORBA_Any::CORBA_Any (CORBA::TypeCode_ptr type,
                      CORBA::UShort,
                      int byte_order,
                      const ACE_Message_Block* mb)
  : type_ (CORBA::TypeCode::_duplicate (type)),
    byte_order_ (byte_order),
    any_owns_data_ (0),
    contains_local_ (0),
    value_ (0),
    destructor_ (0)
{
  ACE_NEW (this->cdr_, ACE_Message_Block);
  ACE_CDR::consolidate (this->cdr_, mb);
}

// Copy constructor for "Any".
CORBA_Any::CORBA_Any (const CORBA_Any &src)
  : cdr_ (0),
    any_owns_data_ (0),
    contains_local_ (src.contains_local_),
    value_ (0),
    destructor_ (0)
{
  if (!CORBA::is_nil (src.type_.in ()))
    this->type_ =
      CORBA::TypeCode::_duplicate (src.type_.in ());
  else
    this->type_ =
      CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  // CDR stream always contains encoded object, if any holds anything
  // at all.
  this->byte_order_ = src.byte_order_;
  ACE_NEW (this->cdr_, ACE_Message_Block);
  ACE_CDR::consolidate (this->cdr_, src.cdr_);

  // No need to copy src's value_.  We can always get that from cdr.
}

// assignment operator

CORBA_Any &
CORBA_Any::operator= (const CORBA_Any &src)
{
  // Check if it is a self assignment
  if (this == &src)
    {
      return *this;
    }

  // Decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated.
  ACE_Message_Block::release ((ACE_Message_Block *) this->cdr_);
  this->cdr_ = 0;

  // If we own any previous data, deallocate it.
  this->free_value ();

  // Now copy the contents of the source to ourselves.
  if (!CORBA::is_nil (src.type_.in ()))
    {
      this->type_ =
        CORBA::TypeCode::_duplicate (src.type_.in ());
    }
  else
    {
      this->type_ =
        CORBA::TypeCode::_duplicate (CORBA::_tc_null);
    }

  this->byte_order_ = src.byte_order_;

  ACE_NEW_RETURN (this->cdr_,
                  ACE_Message_Block,
                  *this);

  ACE_CDR::consolidate (this->cdr_,
                        src.cdr_);

  return *this;
}

// Destructor for an "Any" deep-frees memory if needed.
CORBA_Any::~CORBA_Any (void)
{
  // Decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated.

  ACE_Message_Block::release (this->cdr_);
  this->cdr_ = 0;

  this->free_value ();
}

// TAO proprietary methods, used in the implementation of the >>= and
// <<= operators.

void
CORBA_Any::_tao_replace (CORBA::TypeCode_ptr tc,
                         int byte_order,
                         const ACE_Message_Block *mb)
{
  // Decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated).
  ACE_Message_Block::release (this->cdr_);
  this->cdr_ = 0;

  this->free_value ();

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  this->byte_order_ = byte_order;

  ACE_NEW (this->cdr_,
           ACE_Message_Block);

  ACE_CDR::consolidate (this->cdr_,
                        mb);
  // We can save the decode operation if there's no need to extract
  // the object.
}

void
CORBA_Any::_tao_replace (CORBA::TypeCode_ptr tc,
                         int byte_order,
                         const ACE_Message_Block *mb,
                         CORBA::Boolean any_owns_data,
                         void* value,
                         CORBA::Any::_tao_destructor destructor)
{
  // Decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated).
  ACE_Message_Block::release (this->cdr_);
  this->cdr_ = 0;

  this->free_value ();

  this->any_owns_data_ = any_owns_data;
  this->value_ = value;

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  this->byte_order_ = byte_order;
  ACE_NEW (this->cdr_, ACE_Message_Block);
  ACE_CDR::consolidate (this->cdr_, mb);
  // We can save the decode operation if there's no need to extract
  // the object.

  this->destructor_ = destructor;
}

void
CORBA_Any::_tao_replace (CORBA::TypeCode_ptr tc,
                         CORBA::Boolean any_owns_data,
                         void* value,
                         CORBA::Any::_tao_destructor destructor)
{
  this->free_value ();

  this->any_owns_data_ = any_owns_data;
  this->value_ = value;

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  this->destructor_ = destructor;
}

// Free internal data.
void
CORBA_Any::free_value (void)
{
  if (this->any_owns_data_
      && this->value_ != 0
      && this->destructor_ != 0)
    {
      (*this->destructor_) (this->value_);
    }
  this->any_owns_data_ = 0;
  this->value_ = 0;
  this->destructor_ = 0;
}

void
CORBA_Any::_tao_encode (TAO_OutputCDR &cdr,
                        TAO_ORB_Core *orb_core
                        ACE_ENV_ARG_DECL)
{
  // Always append the CDR stream, even when the value_.
  if (this->cdr_ == 0)
    ACE_THROW (CORBA::NO_IMPLEMENT ());

  TAO_InputCDR in (this->cdr_,
                   this->byte_order_,
                   TAO_DEF_GIOP_MAJOR,
                   TAO_DEF_GIOP_MINOR,
                   orb_core);
  TAO_Marshal_Object::perform_append (this->type_.in (),
                                      &in,
                                      &cdr
                                      ACE_ENV_ARG_PARAMETER);
}

void
CORBA_Any::_tao_decode (TAO_InputCDR &cdr
                        ACE_ENV_ARG_DECL)
{
  // Just read into the CDR stream...

  // @@ (JP) The following code depends on the fact that
  //         TAO_InputCDR does not contain chained message blocks,
  //         otherwise <begin> and <end> could be part of
  //         different buffers!

  // This will be the start of a new message block.
  char *begin = cdr.rd_ptr ();

  // Skip over the next aregument.
  CORBA::TypeCode::traverse_status status =
    TAO_Marshal_Object::perform_skip (this->type_.in (),
                                      &cdr
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      ACE_THROW (CORBA::MARSHAL ());
    }

  // This will be the end of the new message block.
  char *end = cdr.rd_ptr ();

  // The ACE_CDR::mb_align() call can shift the rd_ptr by up to
  // ACE_CDR::MAX_ALIGNMENT-1 bytes. Similarly, the offset adjustment
  // can move the rd_ptr by up to the same amount. We accommodate
  // this by including 2 * ACE_CDR::MAX_ALIGNMENT bytes of additional
  // space in the message block.
  size_t size = end - begin;
  ACE_Message_Block mb (size + 2 * ACE_CDR::MAX_ALIGNMENT);
  ACE_CDR::mb_align (&mb);
  ptr_arith_t offset = ptr_arith_t (begin) % ACE_CDR::MAX_ALIGNMENT;
  mb.rd_ptr (offset);
  mb.wr_ptr (offset + size);

  ACE_OS::memcpy (mb.rd_ptr (),
                  begin,
                  size);

  // Stick it into the Any. It gets duplicated there.
  this->_tao_replace (this->type_.in (),
                      cdr.byte_order (),
                      &mb);
}

// Insertion operators.

void
CORBA_Any::operator<<= (CORBA::Short s)
{
  TAO_OutputCDR stream;
  stream << s;
  this->_tao_replace (CORBA::_tc_short,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (CORBA::UShort s)
{
  TAO_OutputCDR stream;
  stream << s;
  this->_tao_replace (CORBA::_tc_ushort,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (CORBA::Long l)
{
  TAO_OutputCDR stream;
  stream << l;
  this->_tao_replace (CORBA::_tc_long,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (CORBA::ULong l)
{
  TAO_OutputCDR stream;
  stream << l;
  this->_tao_replace (CORBA::_tc_ulong,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (CORBA::LongLong l)
{
  TAO_OutputCDR stream;
  stream << l;
  this->_tao_replace (CORBA::_tc_longlong,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (CORBA::ULongLong l)
{
  TAO_OutputCDR stream;
  stream << l;
  this->_tao_replace (CORBA::_tc_ulonglong,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (CORBA::Float f)
{
  TAO_OutputCDR stream;
  stream << f;
  this->_tao_replace (CORBA::_tc_float,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (CORBA::Double d)
{
  TAO_OutputCDR stream;
  stream << d;
  this->_tao_replace (CORBA::_tc_double,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (CORBA::LongDouble d)
{
  TAO_OutputCDR stream;
  stream << d;
  this->_tao_replace (CORBA::_tc_longdouble,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

// Insertion of Any - copying.
void
CORBA_Any::operator<<= (const CORBA_Any &a)
{
  TAO_OutputCDR stream;
  stream << a;
  this->_tao_replace (CORBA::_tc_any,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

// Insertion of Any - non-copying.
void
CORBA::Any::_tao_any_destructor (void *x)
{
  CORBA::Any *tmp = ACE_static_cast (CORBA::Any *, x);
  delete tmp;
}

void
CORBA::Any::operator<<= (CORBA::Any *a)
{
  TAO_OutputCDR stream;
  stream << *a;
  this->_tao_replace (CORBA::_tc_any,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin (),
                      1,
                      a,
                      CORBA::Any::_tao_any_destructor);
}

// implementing the special types

void
CORBA_Any::operator<<= (from_boolean b)
{
  TAO_OutputCDR stream;
  stream << b;
  this->_tao_replace (CORBA::_tc_boolean,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (from_octet o)
{
  TAO_OutputCDR stream;
  stream << o;
  this->_tao_replace (CORBA::_tc_octet,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (from_char c)
{
  TAO_OutputCDR stream;
  stream << c;
  this->_tao_replace (CORBA::_tc_char,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (from_wchar wc)
{
  TAO_OutputCDR stream;
  stream << wc;
  this->_tao_replace (CORBA::_tc_wchar,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

void
CORBA_Any::operator<<= (CORBA::TypeCode_ptr tc)
{
  TAO_OutputCDR stream;
  stream << tc;
  this->_tao_replace (CORBA::_tc_TypeCode,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

// Insertion of CORBA::Exception - copying.
void
CORBA_Any::operator<<= (const CORBA_Exception &exception)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      TAO_OutputCDR stream;
      exception._tao_encode (stream 
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->_tao_replace (exception._type (),
                          TAO_ENCAP_BYTE_ORDER,
                          stream.begin ());
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Exception in CORBA::Exception insertion\n")));
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Insertion of CORBA::Exception - non-copying.
void
CORBA_Any::operator<<= (CORBA_Exception *exception)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      TAO_OutputCDR stream;
      exception->_tao_encode (stream 
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->_tao_replace (exception->_type (),
                          TAO_ENCAP_BYTE_ORDER,
                          stream.begin (),
                          1,
                          exception,
                          CORBA_Exception::_tao_any_destructor);
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Exception in CORBA::Exception insertion\n")));
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Insertion of CORBA object - copying.
void
CORBA::Any::operator<<= (const CORBA::Object_ptr obj)
{
  CORBA::Object_ptr objptr =
    CORBA::Object::_duplicate (obj);

  (*this) <<= &objptr;
}

// Insertion of CORBA object - non-copying.
void
CORBA::Any::operator<<= (CORBA::Object_ptr *objptr)
{
  if (objptr == 0)
    {
      return; // @@ Should we raise an exception?
    }

  TAO_OutputCDR stream;
  stream << *objptr;
  this->_tao_replace (CORBA::_tc_Object,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

// Insertion of from_string.
void
CORBA_Any::operator<<= (from_string s)
{
  TAO_OutputCDR stream;
  stream << s;

  char *tmp;
  // Non-copying.
  if (s.nocopy_)
    {
      tmp = s.val_;
    }
  else
    {
      tmp = CORBA::string_dup (s.val_);
    }

  if (s.bound_ > 0)
    {
      // If the inserted string is bounded, we create a typecode.
      static CORBA::Long _oc_string [] =
      {
        TAO_ENCAP_BYTE_ORDER,   // native endian + padding; "tricky"
        ACE_static_cast (CORBA::Long, s.bound_)
      };

      CORBA::TypeCode_ptr tc = 0;

      ACE_NEW (tc,
               CORBA::TypeCode (CORBA::tk_string,
                                sizeof _oc_string,
                                (char *) &_oc_string,
                                1,
                                sizeof (CORBA::String_var)));

      this->_tao_replace (tc,
                          TAO_ENCAP_BYTE_ORDER,
                          stream.begin (),
                          1,
                          tmp,
                          CORBA::Any::_tao_any_string_destructor);

      CORBA::release (tc);
    }
  else
    {
      // Unbounded.
      this->_tao_replace (CORBA::_tc_string,
                          TAO_ENCAP_BYTE_ORDER,
                          stream.begin (),
                          1,
                          tmp,
                          CORBA::Any::_tao_any_string_destructor);
    }
}

void
CORBA_Any::operator<<= (from_wstring ws)
{
  TAO_OutputCDR stream;
  stream << ws;

  CORBA::WChar *tmp;
  // Non-copying.
  if (ws.nocopy_)
    {
      tmp = ws.val_;
    }
  else
    {
      tmp = CORBA::wstring_dup (ws.val_);
    }

  if (ws.bound_ > 0)
    {
      // If the inserted string is bounded, we create a typecode.
      static CORBA::Long _oc_wstring [] =
      {
        TAO_ENCAP_BYTE_ORDER,   // native endian + padding; "tricky"
        ACE_static_cast (CORBA::Long, ws.bound_)
      };

      CORBA::TypeCode_ptr tc = 0;
      ACE_NEW (tc,
               CORBA::TypeCode (CORBA::tk_wstring,
                                sizeof _oc_wstring,
                                (char *) &_oc_wstring,
                                1,
                                sizeof (CORBA::WString_var)));

      this->_tao_replace (tc,
                          TAO_ENCAP_BYTE_ORDER,
                          stream.begin (),
                          1,
                          tmp,
                          CORBA::Any::_tao_any_wstring_destructor);

      CORBA::release (tc);
    }
  else
    {
      this->_tao_replace (CORBA::_tc_wstring,
                          TAO_ENCAP_BYTE_ORDER,
                          stream.begin (),
                          1,
                          tmp,
                          CORBA::Any::_tao_any_wstring_destructor);
    }
}

// Extraction: these are safe and hence we have to check that the
// typecode of the Any is equal to the one we are trying to extract
// into.

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Short &s) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_short
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        return 0;

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);
      return stream.read_short (s);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::Short extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::UShort &s) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_ushort
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);

      return stream.read_ushort (s);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::UShort extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Long &l) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_long
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);

      return stream.read_long (l);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::Long extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::ULong &l) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_ulong
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);

      return stream.read_ulong (l);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::ULong extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::LongLong &l) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_longlong
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);

      return stream.read_longlong (l);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::LongLong extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::ULongLong &l) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_ulonglong
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result)
        {
          if (this->any_owns_data_ && this->value_)
            {
              l = *(CORBA::ULongLong *) this->value_;
              return 1;
            }
          else
            {
              TAO_InputCDR stream (this->cdr_,
                                   this->byte_order_);
              return stream.read_ulonglong (l);
            }
        }
      else
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::ULongLong extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Float &f) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_float
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);

      return stream.read_float (f);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::Float extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Double &d) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_double
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);

      return stream.read_double (d);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::Double extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::LongDouble &ld) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_longdouble
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);

      return stream.read_longdouble (ld);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::LongDouble extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Any &a) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_any
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);
      if (stream >> a)
        {
          return 1;
        }
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::Any extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (const CORBA::Any *&a) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      a = 0;
      CORBA::Boolean equivalent =
        this->type_->equivalent (CORBA::_tc_any
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!equivalent)
        {
          return 0;
        }

      if (this->any_owns_data_ && this->value_)
        {
          a = (CORBA::Any *) this->value_;
          return 1;
        }
      else
        {
          CORBA::Any *x;
          ACE_NEW_RETURN (x, CORBA::Any, 0);
          CORBA::Any_var tmp = x;
          TAO_InputCDR stream (this->cdr_,
                               this->byte_order_);

          if (!(stream >> tmp.inout ()))
            {
              return 0;
            }

          ACE_const_cast(CORBA::Any*,
                         this)->_tao_replace (CORBA::_tc_any,
                                              1,
                                              ACE_static_cast(CORBA::Any*,tmp),
                                              CORBA::Any::_tao_any_destructor);

          ACE_const_cast (CORBA::Any *&, a) = tmp._retn ();
          return 1;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Exception in CORBA::Any extraction\n")));
        }
    }
  ACE_ENDTRY;

  a = 0;
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (const char *&s) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      s = 0;

      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_string
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      if (this->any_owns_data_ && this->value_)
        {
          s = ACE_static_cast (char *,this->value_);
          return 1;
        }


      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);
      CORBA::String_var tmp;

      if (!stream.read_string (tmp.out ()))
        {
          return 0;
        }

      ACE_const_cast(CORBA::Any*,
                     this)->_tao_replace (CORBA::_tc_string,
                                          1,
                                          tmp.inout (),
                                          CORBA::Any::_tao_any_string_destructor);

      s = tmp._retn ();
      return 1;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  s = 0;
  return 0;
}

void
CORBA_Any::_tao_any_string_destructor (void *x)
{
  char *tmp = ACE_static_cast (char*,x);
  CORBA::string_free (tmp);
}

void
CORBA::Any::_tao_any_wstring_destructor (void *x)
{
  CORBA::WChar *tmp = ACE_static_cast (CORBA::WChar*,x);
  CORBA::wstring_free (tmp);
}

CORBA::Boolean
CORBA_Any::operator>>= (const CORBA::WChar *&s) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      s = 0;

      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_wstring
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      if (this->any_owns_data_ && this->value_)
        {
          s = ACE_static_cast (CORBA::WChar *,this->value_);
          return 1;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_);
      CORBA::WString_var tmp;

      if (!stream.read_wstring (tmp.out ()))
        {
          return 0;
        }

      ACE_const_cast(CORBA::Any*,
                     this)->_tao_replace (CORBA::_tc_wstring,
                                          1,
                                          tmp.inout (),
                                          CORBA::Any::_tao_any_wstring_destructor);

      s = tmp._retn ();
      return 1;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in unbounded wstring extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

void
CORBA::Any::_tao_any_tc_destructor (void *x)
{
  CORBA::TypeCode_ptr tmp = ACE_static_cast (CORBA::TypeCode_ptr,x);
  CORBA::release (tmp);
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::TypeCode_ptr &tc) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_TypeCode
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      if (this->any_owns_data_ && this->value_)
        {
          tc = ACE_static_cast (CORBA::TypeCode_ptr, this->value_);
          return 1;
        }

      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_,
                           this->byte_order_);
      CORBA::TypeCode_var tmp;

      if (!(stream >> tmp.inout ()))
        {
          return 0;
        }

      ACE_const_cast(CORBA::Any*,
                     this)->_tao_replace (CORBA::_tc_TypeCode,
                                          1,
                                          tmp.in (),
                                          CORBA::Any::_tao_any_tc_destructor);

      tc = tmp._retn ();
      return 1;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::TypeCode_ptr extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

// = extraction into the special types

CORBA::Boolean
CORBA_Any::operator>>= (to_boolean b) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_boolean
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_,
                           this->byte_order_);

      return stream.read_boolean (b.ref_);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::Boolean extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_octet o) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_octet
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_,
                           this->byte_order_);

      return stream.read_octet (o.ref_);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::Octet extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_char c) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_char
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_,
                           this->byte_order_);

      return stream.read_char (c.ref_);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::Char extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_wchar wc) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_wchar
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          return 0;
        }

      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_,
                           this->byte_order_);

      return stream.read_wchar (wc.ref_);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::WChar extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_string s) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ULong kind =
        this->type_->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_.in ());

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_string)
        {
          return 0;
        }

      CORBA::ULong bound = tcvar->length (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (s.bound_ != bound)
        {
          return 0;
        }

      if (this->any_owns_data_ && this->value_)
        {
          s.val_ = ACE_static_cast (char*,this->value_);
          return 1;
        }

      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_,
                           this->byte_order_);
      CORBA::String_var tmp;

      if (!stream.read_string (tmp.out ()))
        {
          return 0;
        }

      CORBA::Any *non_const_this = ACE_const_cast (CORBA::Any*, 
                                                   this);
      non_const_this->_tao_replace (CORBA::_tc_string,
                                    1,
                                    ACE_static_cast(char*,tmp),
                                    CORBA::Any::_tao_any_string_destructor);

      s.val_ = tmp._retn ();
      return 1;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in bounded string extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_wstring ws) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ULong kind =
        this->type_->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_.in ());

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_wstring)
        {
          return 0;
        }

      CORBA::ULong bound = tcvar->length (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ws.bound_ != bound)
        {
          return 0;
        }

      if (this->any_owns_data_ && this->value_)
        {
          ws.val_ = ACE_static_cast(CORBA::WChar *, this->value_);
          return 1;
        }

      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_,
                           this->byte_order_);
      CORBA::WString_var tmp;

      if (!stream.read_wstring (tmp.out ()))
        {
          return 0;
        }

      CORBA::Any *non_const_this = ACE_const_cast (CORBA::Any*, 
                                                   this);
      non_const_this->_tao_replace (CORBA::_tc_string,
                                    1,
                                    ACE_static_cast(CORBA::WChar*,tmp),
                                    CORBA::Any::_tao_any_wstring_destructor);

      ws.val_ = tmp._retn ();
      return 1;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in bounded wstring extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_object obj) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ULong kind =
        this->type_->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_.in ());

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_objref)
        {
          return 0;
        }

      // It is hard to apply this optimization, because value_ points
      // to a derived class from CORBA::Object, and with multiple
      // virtual inheritance.  One posibility is to use a helper
      // function to upcast from void* to CORBA::Object_ptr, but this
      // is only useful to implement the >>=(to_object) operator,
      // which is very rarely used.
      // It is better to just demarshal the object everytime,
      // specially because the caller is owns the returned object.
      //
      // if (this->any_owns_data_ && this->value_)
      //  {
      //    // CORBA 2.3 has changed the behavior of this operator. Caller
      //    // is now responsible for release.
      //    obj.ref_ =
      //      CORBA::Object::_duplicate (
      //          (*this->upcast_) (this->value_)
      //        );
      //    return 1;
      //  }

      // @@ This uses ORB_Core instance because we need one to
      // demarshal objects (to create the right profiles for that
      // object), but the Any does not belong to any ORB.
      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_,
                           TAO_DEF_GIOP_MAJOR,
                           TAO_DEF_GIOP_MINOR,
                           TAO_ORB_Core_instance ());

      if (stream >> obj.ref_)
        {
          return 1;
        }
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Exception in CORBA::Object_ptr extraction\n")));
    }
  ACE_ENDTRY;

  return 0;
}

// this is a copying version for unbounded strings Not inline, to
// avoid use in Any.i before definition in ORB.i.

void
CORBA_Any::operator<<= (const char* s)
{
  TAO_OutputCDR stream;
  stream << s;
  this->_tao_replace (CORBA::_tc_string,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

// And the version for unbounded wide string.
void
CORBA_Any::operator<<= (const CORBA::WChar* s)
{
  TAO_OutputCDR stream;
  stream << s;
  this->_tao_replace (CORBA::_tc_wstring,
                      TAO_ENCAP_BYTE_ORDER,
                      stream.begin ());
}

CORBA::Boolean
operator<< (TAO_OutputCDR& cdr,
            const CORBA::Any &x)
{
  // If a locality-constrained interface is contained at
  // any level, the Any cannot be marshaled.
  if (x.contains_local ())
    {
      return 0;
    }

  CORBA::TypeCode_var tc = x.type ();

  if (!(cdr << tc.in ()))
    {
      return 0;
    }

  ACE_TRY_NEW_ENV
    {
      TAO_InputCDR input (x._tao_get_cdr (),
                          x._tao_byte_order ());
      TAO_Marshal_Object::perform_append (tc.in (),
                                          &input,
                                          &cdr
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA_Exception, ex)
    {
      return 0;
    }
  ACE_ENDTRY;
  return 1;
}

CORBA::Boolean
operator>> (TAO_InputCDR &cdr,
            CORBA::Any &x)
{
  CORBA::TypeCode_var tc;

  if (!(cdr >> tc.out ()))
    {
      return 0;
    }

  ACE_TRY_NEW_ENV
    {
      // @@ (JP) The following code depends on the fact that
      //         TAO_InputCDR does not contain chained message blocks,
      //         otherwise <begin> and <end> could be part of
      //         different buffers!

      // This will be the start of a new message block.
      char *begin = cdr.rd_ptr ();

      // Skip over the next aregument.
      CORBA::TypeCode::traverse_status status =
        TAO_Marshal_Object::perform_skip (tc.in (), &cdr ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          return 0;
        }

      // This will be the end of the new message block.
      char *end = cdr.rd_ptr ();

      // The ACE_CDR::mb_align() call can shift the rd_ptr by up to
      // ACE_CDR::MAX_ALIGNMENT-1 bytes. Similarly, the offset adjustment
      // can move the rd_ptr by up to the same amount. We accommodate
      // this by including 2 * ACE_CDR::MAX_ALIGNMENT bytes of additional
      // space in the message block.
      size_t size = end - begin;
      ACE_Message_Block mb (size + 2 * ACE_CDR::MAX_ALIGNMENT);
      ACE_CDR::mb_align (&mb);
      ptr_arith_t offset = ptr_arith_t (begin) % ACE_CDR::MAX_ALIGNMENT;
      mb.rd_ptr (offset);
      mb.wr_ptr (offset + size);
      ACE_OS::memcpy (mb.rd_ptr (), begin, size);

      // Stick it into the Any. It gets duplicated there.
      x._tao_replace (tc.in (),
                      cdr.byte_order (),
                      &mb);
    }
  ACE_CATCH (CORBA_Exception, ex)
    {
      return 0;
    }
  ACE_ENDTRY;

  return 1;
}

// ****************************************************************

CORBA::Any_var &
CORBA_Any_var::operator= (CORBA::Any *p)
{
  if (this->ptr_ != p)
    {
      delete this->ptr_;

      this->ptr_ = p;
    }

  return *this;
}

CORBA::Any_var &
CORBA_Any_var::operator= (const CORBA::Any_var& r)
{
  CORBA_Any_ptr tmp;

  ACE_NEW_RETURN (tmp,
                  CORBA::Any (*r.ptr_),
                  *this);

  delete this->ptr_;

  this->ptr_ = tmp;

  return *this;
}
