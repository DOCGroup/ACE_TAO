// $Id$

#include "tao/Any_T.h"

#if !defined (__ACE_INLINE__)
# include "tao/Any.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           Any,
           "$Id$")

CORBA::Any::Any (void)
  : impl_ (0)
{
}

CORBA::Any::Any (const CORBA::Any &rhs)
  : impl_ (rhs.impl_)
{
  if (this->impl_ != 0)
    {
      this->impl_->_add_ref ();
    }
}

CORBA::Any::~Any (void)
{
  if (this->impl_ != 0)
    {
      this->impl_->free_value ();
    }
}

CORBA::Any &
CORBA::Any::operator= (const CORBA::Any &rhs)
{
  if (this->impl_ != rhs.impl_)
    {
      if (this->impl_ != 0)
        {
          this->impl_->free_value ();
        }

      this->impl_ = rhs.impl_;

      if (this->impl_ != 0)
        {
          this->impl_->_add_ref ();
        }
    }

  return *this;
}

void 
CORBA::Any::replace (TAO::Any_Impl *new_impl)
{
  if (this->impl_ != 0)
    {
      this->impl_->_remove_ref ();
    }

  this->impl_ = new_impl;
}

CORBA::TypeCode_ptr
CORBA::Any::type (void) const
{
  if (this->impl_ != 0)
    {
      return this->impl_->type ();
    }

  return CORBA::TypeCode::_duplicate (CORBA::_tc_null);
}

CORBA::TypeCode_ptr
CORBA::Any::_tao_get_typecode (void) const
{
  if (this->impl_ != 0)
    {
      return this->impl_->_tao_get_typecode ();
    }

  return CORBA::_tc_null;
}

ACE_Message_Block *
CORBA::Any::_tao_get_cdr (void) const
{
  if (this->impl_ != 0)
    {
      return this->impl_->_tao_get_cdr ();
    }

  return 0;
}

int
CORBA::Any::_tao_byte_order (void) const
{
  if (this->impl_ != 0)
    {
      return this->impl_->_tao_byte_order ();
    }

  return TAO_ENCAP_BYTE_ORDER;
}

void
CORBA::Any::type (CORBA::TypeCode_ptr tc
                  ACE_ENV_ARG_DECL)
{
  CORBA::Boolean equiv = 0;

  if (this->impl_ != 0)
    {
      equiv = 
        this->impl_->_tao_get_typecode ()->equivalent (tc
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (equiv)
    {
      this->impl_->type (tc);
    }
  else
    {
      ACE_THROW (CORBA::BAD_TYPECODE ());
    }
}

void
CORBA::Any::_tao_any_destructor (void *x)
{
  CORBA::Any *tmp = ACE_static_cast (CORBA::Any *, x);
  delete tmp;
}

const void *
CORBA::Any::value (void) const
{
  if (this->impl_ != 0)
    {
      return this->impl_->value ();
    }

  return 0;
}

CORBA::Any::to_object::to_object (CORBA::Object_out obj)
  : ref_ (obj.ptr ())
{
}

CORBA::Any::to_abstract_base::to_abstract_base (CORBA::AbstractBase_ptr &obj)
  : ref_ (obj)
{
}

CORBA::Any::to_value::to_value (CORBA::ValueBase *& obj)
  : ref_ (obj)
{
}

// =======================================================================

TAO::Any_Impl::Any_Impl (_tao_destructor destructor,
                         CORBA::TypeCode_ptr tc)
  : value_destructor_ (destructor),
    type_ (tc),
    refcount_ (1),
    refcount_lock_ (0)
{
  ACE_NEW (this->refcount_lock_,
           TAO_SYNCH_MUTEX);
}

TAO::Any_Impl::~Any_Impl (void)
{
  delete this->refcount_lock_;
}

CORBA::Boolean
TAO::Any_Impl::marshal (TAO_OutputCDR &cdr)
{
  if ((cdr << this->type_) == 0)
    {
      return 0;
    }

  return this->marshal_value (cdr);
}

void
TAO::Any_Impl::_tao_any_string_destructor (void *x)
{
  char *tmp = ACE_static_cast (char *, x);
  CORBA::string_free (tmp);
}

void
TAO::Any_Impl::_tao_any_wstring_destructor (void *x)
{
  CORBA::WChar *tmp = ACE_static_cast (CORBA::WChar *, x);
  CORBA::wstring_free (tmp);
}

void
TAO::Any_Impl::_add_ref (void)
{
  if (this->refcount_lock_ != 0)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, 
                 mon, 
                 *this->refcount_lock_);

      this->refcount_++;
    }
}

void
TAO::Any_Impl::_remove_ref (void)
{
  if (this->refcount_lock_ != 0)
    {
      {
        ACE_GUARD (TAO_SYNCH_MUTEX, 
                   mon, 
                   *this->refcount_lock_);

        this->refcount_--;

        if (this->refcount_ != 0)
          {
            return;
          }
      }

      this->free_value ();
      delete this;
    }
}

// =======================================================================

TAO::Unknown_IDL_Type::Unknown_IDL_Type (CORBA::TypeCode_ptr tc,
                                         ACE_Message_Block *mb,
                                         int byte_order)
  : TAO::Any_Impl (0,
                   tc),
    cdr_ (mb),
    byte_order_ (byte_order)
{
}

TAO::Unknown_IDL_Type::~Unknown_IDL_Type (void)
{
}

CORBA::Boolean
TAO::Unknown_IDL_Type::marshal_value (TAO_OutputCDR &cdr)
{
  ACE_TRY_NEW_ENV
    {
      TAO_InputCDR input (this->cdr_,
                          this->byte_order_);

      CORBA::TypeCode::traverse_status status =
        TAO_Marshal_Object::perform_append (this->type_,
                                            &input,
                                            &cdr
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          return 0;
        }
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      return 0;
    }
  ACE_ENDTRY;

  return 1;
}

void
TAO::Unknown_IDL_Type::free_value (void)
{
//  CORBA::release (this->type_);
//  this->type_ = CORBA::TypeCode::_nil ();
  ACE_Message_Block::release (this->cdr_);
}

// ****************************************************************

CORBA::Any_var &
CORBA::Any_var::operator= (CORBA::Any *p)
{
  if (this->ptr_ != p)
    {
      delete this->ptr_;
      this->ptr_ = p;
    }

  return *this;
}

CORBA::Any_var &
CORBA::Any_var::operator= (const CORBA::Any_var &r)
{
  CORBA::Any_ptr tmp = 0;

  ACE_NEW_RETURN (tmp,
                  CORBA::Any (*r.ptr_),
                  *this);

  delete this->ptr_;
  this->ptr_ = tmp;
  return *this;
}

// ***********************************************************************

CORBA::Boolean 
operator<< (TAO_OutputCDR &cdr, const CORBA::Any &any)
{
  TAO::Any_Impl *impl = any.impl ();

  if (impl != 0)
    {
      return impl->marshal (cdr);
    }

  return (cdr << CORBA::_tc_null);
}

CORBA::Boolean
operator>> (TAO_InputCDR &cdr, CORBA::Any &any)
{
  CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();

  if ((cdr >> tc) == 0)
    {
      return 0;
    }

  ACE_TRY_NEW_ENV
    {
      CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (kind == CORBA::tk_any)
        {
          tc->_decr_refcnt ();
          CORBA::Any nested_any;

          TAO::Any_Any_Impl *any_impl = 0;
          ACE_NEW_RETURN (any_impl,
                          TAO::Any_Any_Impl (nested_any),
                          0);

          any.replace (any_impl);
          return any_impl->demarshal_value (cdr);
        }

      // @@ (JP) The following code depends on the fact that
      //         TAO_InputCDR does not contain chained message blocks,
      //         otherwise <begin> and <end> could be part of
      //         different buffers!

      // This will be the start of a new message block.
      char *begin = cdr.rd_ptr ();

      // Skip over the next aregument.
      CORBA::TypeCode::traverse_status status =
        TAO_Marshal_Object::perform_skip (tc,
                                          &cdr
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          return 0;
        }

      // This will be the end of the new message block.
      char *end = cdr.rd_ptr ();

      // The ACE_CDR::mb_align() call can shift the rd_ptr by up to
      // ACE_CDR::MAX_ALIGNMENT - 1 bytes. Similarly, the offset adjustment
      // can move the rd_ptr by up to the same amount. We accommodate
      // this by including 2 * ACE_CDR::MAX_ALIGNMENT bytes of additional
      // space in the message block.
      size_t size = end - begin;

      ACE_Message_Block *mb = 0;
      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (size + 2 * ACE_CDR::MAX_ALIGNMENT),
                      0);

      ACE_CDR::mb_align (mb);
      ptr_arith_t offset = ptr_arith_t (begin) % ACE_CDR::MAX_ALIGNMENT;
      mb->rd_ptr (offset);
      mb->wr_ptr (offset + size);
      ACE_OS::memcpy (mb->rd_ptr (), begin, size);

      TAO::Unknown_IDL_Type *impl = 0;
      ACE_NEW_RETURN (impl,
                      TAO::Unknown_IDL_Type (tc,
                                             mb,
                                             cdr.byte_order ()),
                      0);

      any.replace (impl);
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      return 0;
    }
  ACE_ENDTRY;

  return 1;
}

// =======================================================================

// Insertion of the special basic types.

void
CORBA::Any::operator<<= (CORBA::Any::from_boolean b)
{
  TAO::Any_Special_Basic_Impl_T<
      CORBA::Boolean, 
      CORBA::Any::from_boolean,
      CORBA::Any::to_boolean
    >::insert (*this,
               CORBA::_tc_boolean,
               b.val_);
}

void
CORBA::Any::operator<<= (CORBA::Any::from_octet o)
{
  TAO::Any_Special_Basic_Impl_T<
      CORBA::Octet, 
      CORBA::Any::from_octet,
      CORBA::Any::to_octet
    >::insert (*this,
               CORBA::_tc_octet,
               o.val_);
}

void
CORBA::Any::operator<<= (CORBA::Any::from_char c)
{
  TAO::Any_Special_Basic_Impl_T<
      char, 
      CORBA::Any::from_char,
      CORBA::Any::to_char
    >::insert (*this,
               CORBA::_tc_char,
               c.val_);
}

void
CORBA::Any::operator<<= (CORBA::Any::from_wchar wc)
{
  TAO::Any_Special_Basic_Impl_T<
      CORBA::WChar, 
      CORBA::Any::from_wchar,
      CORBA::Any::to_wchar
    >::insert (*this,
               CORBA::_tc_wchar,
               wc.val_);
}

void
CORBA::Any::operator<<= (CORBA::Any::from_string s)
{
  TAO::Any_Special_Impl_T<
      char, 
      CORBA::Any::from_string,
      CORBA::Any::to_string
    >::insert (*this,
               TAO::Any_Impl::_tao_any_string_destructor,
               CORBA::tk_string,
               s.nocopy_ ? s.val_ : CORBA::string_dup (s.val_),
               s.bound_);
}

void
CORBA::Any::operator<<= (CORBA::Any::from_wstring ws)
{
  TAO::Any_Special_Impl_T<
      CORBA::WChar, 
      CORBA::Any::from_wstring,
      CORBA::Any::to_wstring
    >::insert (*this,
               TAO::Any_Impl::_tao_any_wstring_destructor,
               CORBA::tk_wstring,
               ws.nocopy_ ? ws.val_ : CORBA::wstring_dup (ws.val_),
               ws.bound_);
}

// Insertion of the other basic types.

void
operator<<= (CORBA::Any &any, CORBA::Short s)
{
  TAO::Any_Basic_Impl_T<CORBA::Short>::insert (any,
                                               CORBA::_tc_short,
                                               s);
}

void
operator<<= (CORBA::Any &any, CORBA::UShort us)
{
  TAO::Any_Basic_Impl_T<CORBA::UShort>::insert (any,
                                                CORBA::_tc_ushort,
                                                us);
}

void
operator<<= (CORBA::Any &any, CORBA::Long l)
{
  TAO::Any_Basic_Impl_T<CORBA::Long>::insert (any,
                                              CORBA::_tc_long,
                                              l);
}

void
operator<<= (CORBA::Any &any, CORBA::ULong ul)
{
  TAO::Any_Basic_Impl_T<CORBA::ULong>::insert (any,
                                               CORBA::_tc_ulong,
                                               ul);
}

void
operator<<= (CORBA::Any &any, CORBA::LongLong ll)
{
  TAO::Any_Basic_Impl_T<CORBA::LongLong>::insert (any,
                                                  CORBA::_tc_longlong,
                                                  ll);
}

void
operator<<= (CORBA::Any &any, CORBA::ULongLong ull)
{
  TAO::Any_Basic_Impl_T<CORBA::ULongLong>::insert (any,
                                                   CORBA::_tc_ulonglong,
                                                   ull);
}

void
operator<<= (CORBA::Any &any, CORBA::Float f)
{
  TAO::Any_Basic_Impl_T<CORBA::Float>::insert (any,
                                               CORBA::_tc_float,
                                               f);
}

void
operator<<= (CORBA::Any &any, CORBA::Double d)
{
  TAO::Any_Basic_Impl_T<CORBA::Double>::insert (any,
                                                CORBA::_tc_double,
                                                d);
}

void
operator<<= (CORBA::Any &any, CORBA::LongDouble ld)
{
  TAO::Any_Basic_Impl_T<CORBA::LongDouble>::insert (any,
                                                    CORBA::_tc_longdouble,
                                                    ld);
}

// Insertion of Any - copying.
void
operator<<= (CORBA::Any &any, const CORBA::Any &a)
{
  TAO::Any_Any_Impl::insert_copy (any,
                                  a);
}

// Insertion of Any - non-copying.
void
operator<<= (CORBA::Any &any, CORBA::Any *a)
{
  TAO::Any_Dual_Impl_T<CORBA::Any>::insert (any,
                                            CORBA::Any::_tao_any_destructor,
                                            CORBA::_tc_any,
                                            a);
}

void
operator<<= (CORBA::Any &any, CORBA::TypeCode_ptr tc)
{
  TAO::Any_Impl_T<CORBA::TypeCode>::insert (
      any,
      CORBA::TypeCode::_tao_any_destructor,
      CORBA::_tc_TypeCode,
      tc
    );
}

// Insertion of CORBA::Exception - copying.
void
operator<<= (CORBA::Any &any, const CORBA::Exception &exception)
{
  TAO::Any_Dual_Impl_T<CORBA::Exception>::insert_copy (any,
                                                       exception._type (),
                                                       exception);
}

// Insertion of CORBA::Exception - non-copying.
void
operator<<= (CORBA::Any &any, CORBA::Exception *exception)
{
  TAO::Any_Dual_Impl_T<CORBA::Exception>::insert (
      any,
      CORBA::Exception::_tao_any_destructor,
      exception->_type (),
      exception
    );
}

// Insertion of CORBA object - copying.
void
operator<<= (CORBA::Any &any, const CORBA::Object_ptr obj)
{
  CORBA::Object_ptr dup = CORBA::Object::_duplicate (obj);
  any <<= &dup;
}

// Insertion of CORBA object - non-copying.
void
operator<<= (CORBA::Any &any, CORBA::Object_ptr *objptr)
{
  TAO::Any_Impl_T<CORBA::Object>::insert (any,
                                          CORBA::Object::_tao_any_destructor,
                                          CORBA::_tc_Object,
                                          *objptr);
}

// These are copying versions for unbounded (w)strings. They are not inlined,
// to avoid use in Any.i before definition in ORB.i.

void
operator<<= (CORBA::Any &any, const char* s)
{
  TAO::Any_Impl_T<char>::insert (any,
                                 TAO::Any_Impl::_tao_any_string_destructor,
                                 CORBA::_tc_string,
                                 CORBA::string_dup (s));
}

void
operator<<= (CORBA::Any &any, const CORBA::WChar* ws)
{
  TAO::Any_Impl_T<CORBA::WChar>::insert (
      any,
      TAO::Any_Impl::_tao_any_wstring_destructor,
      CORBA::_tc_wstring,
      CORBA::wstring_dup (ws)
    );
}

// Extraction: these are safe and hence we have to check that the
// typecode of the Any is equal to the one we are trying to extract
// into.

// Extraction into the special basic types.

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_boolean b) const
{
  return TAO::Any_Special_Basic_Impl_T<
      CORBA::Boolean, 
      CORBA::Any::from_boolean,
      CORBA::Any::to_boolean
    >::extract (*this,
                CORBA::_tc_boolean,
                b.ref_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_octet o) const
{
  return TAO::Any_Special_Basic_Impl_T<
      CORBA::Octet, 
      CORBA::Any::from_octet,
      CORBA::Any::to_octet
    >::extract (*this,
                CORBA::_tc_octet,
                o.ref_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_char c) const
{
  return TAO::Any_Special_Basic_Impl_T<
      char, 
      CORBA::Any::from_char,
      CORBA::Any::to_char
    >::extract (*this,
                CORBA::_tc_char,
                c.ref_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_wchar wc) const
{
  return TAO::Any_Special_Basic_Impl_T<
      CORBA::WChar, 
      CORBA::Any::from_wchar,
      CORBA::Any::to_wchar
    >::extract (*this,
                CORBA::_tc_wchar,
                wc.ref_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_string s) const
{
  return 
    TAO::Any_Special_Impl_T<
        char, 
        CORBA::Any::from_string,
        CORBA::Any::to_string
      >::extract (*this,
                  TAO::Any_Impl::_tao_any_string_destructor,
                  CORBA::tk_string,
                  s.bound_,
                  s.val_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_wstring ws) const
{
  return 
    TAO::Any_Special_Impl_T<
        CORBA::WChar, 
        CORBA::Any::from_wstring,
        CORBA::Any::to_wstring
      >::extract (*this,
                  TAO::Any_Impl::_tao_any_wstring_destructor,
                  CORBA::tk_wstring,
                  ws.bound_,
                  ws.val_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_object obj) const
{
  return TAO::Any_Impl_T<CORBA::Object>::widen (
      *this,
      CORBA::Object::_tao_any_destructor,
      CORBA::tk_objref,
      obj.ref_
    );
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_abstract_base obj) const
{
  return TAO::Any_Impl_T<CORBA::AbstractBase>::widen (
      *this,
      CORBA::AbstractBase::_tao_any_destructor,
      CORBA::tk_abstract_interface,
      obj.ref_
    );
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_value obj) const
{
  return TAO::Any_Impl_T<CORBA::ValueBase>::widen (
      *this,
      CORBA::ValueBase::_tao_any_destructor,
      CORBA::tk_value,
      obj.ref_
    );
}

// Extraction into the other basic types.

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::Short &s)
{
  return TAO::Any_Basic_Impl_T<CORBA::Short>::extract (any,
                                                       CORBA::_tc_short,
                                                       s);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::UShort &us)
{
  return TAO::Any_Basic_Impl_T<CORBA::UShort>::extract (any,
                                                        CORBA::_tc_ushort,
                                                        us);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::Long &l)
{
  return TAO::Any_Basic_Impl_T<CORBA::Long>::extract (any,
                                                      CORBA::_tc_long,
                                                      l);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::ULong &ul)
{
  return TAO::Any_Basic_Impl_T<CORBA::ULong>::extract (any,
                                                       CORBA::_tc_ulong,
                                                       ul);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::LongLong &ll)
{
  return 
    TAO::Any_Basic_Impl_T<CORBA::LongLong>::extract (any,
                                                     CORBA::_tc_longlong,
                                                     ll);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::ULongLong &ull)
{
  return 
    TAO::Any_Basic_Impl_T<CORBA::ULongLong>::extract (any,
                                                      CORBA::_tc_ulonglong,
                                                      ull);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::Float &f)
{
  return TAO::Any_Basic_Impl_T<CORBA::Float>::extract (any,
                                                       CORBA::_tc_float,
                                                       f);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::Double &d)
{
  return TAO::Any_Basic_Impl_T<CORBA::Double>::extract (any,
                                                        CORBA::_tc_double,
                                                        d);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::LongDouble &ld)
{
  return 
    TAO::Any_Basic_Impl_T<CORBA::LongDouble>::extract (any,
                                                       CORBA::_tc_longdouble,
                                                       ld);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, const CORBA::Any &a)
{
  return TAO::Any_Any_Impl::extract_ref (
      any,
      ACE_const_cast (CORBA::Any &, a)
    );
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, const CORBA::Any *&a)
{
  return TAO::Any_Dual_Impl_T<CORBA::Any>::extract (
      any,
      CORBA::Any::_tao_any_destructor,
      CORBA::_tc_any,
      a
    );
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, const char *&s)
{
  return TAO::Any_Impl_T<char>::extract (
      any,
      TAO::Any_Impl::_tao_any_string_destructor,
      CORBA::_tc_string,
      s
    );
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, const CORBA::WChar *&ws)
{
  return TAO::Any_Impl_T<CORBA::WChar>::extract (
      any,
      TAO::Any_Impl::_tao_any_wstring_destructor,
      CORBA::_tc_wstring,
      ws
    );
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::TypeCode_ptr &tc)
{
  return TAO::Any_Impl_T<CORBA::TypeCode>::extract (
      any,
      CORBA_TypeCode::_tao_any_destructor,
      CORBA::_tc_TypeCode,
      tc
    );
}

// =======================================================================

// Specializations for CORBA::Any

CORBA::Boolean
TAO::Any_Impl_T<CORBA::Any>::marshal_value (TAO_OutputCDR &cdr)
{
  return 0;
}

CORBA::Boolean
TAO::Any_Impl_T<CORBA::Any>::demarshal_value (TAO_InputCDR &cdr)
{
  return 0;
}

// =======================================================================

// Specializations of the create_empty method for long long and long double.

TAO::Any_Basic_Impl_T<CORBA::LongLong> *
TAO::Any_Basic_Impl_T<CORBA::LongLong>::create_empty (
    CORBA::TypeCode_ptr tc
  )
{
  CORBA::LongLong zero = ACE_CDR_LONGLONG_INITIALIZER;
  TAO::Any_Basic_Impl_T<CORBA::LongLong> * retval = 0;
  ACE_NEW_RETURN (retval,
                  TAO::Any_Basic_Impl_T<CORBA::LongLong> (tc,
                                                          zero),
                  0);
  return retval;
}

TAO::Any_Basic_Impl_T<CORBA::LongDouble> *
TAO::Any_Basic_Impl_T<CORBA::LongDouble>::create_empty (
    CORBA::TypeCode_ptr tc
  )
{
  CORBA::LongDouble zero = ACE_CDR_LONG_DOUBLE_INITIALIZER;
  TAO::Any_Basic_Impl_T<CORBA::LongDouble> * retval = 0;
  ACE_NEW_RETURN (retval,
                  TAO::Any_Basic_Impl_T<CORBA::LongDouble> (tc,
                                                            zero),
                  0);
  return retval;
}

// =======================================================================

// Specializations for CORBA::Exception

CORBA::Boolean
TAO::Any_Dual_Impl_T<CORBA::Exception>::marshal_value (TAO_OutputCDR &cdr)
{
  ACE_TRY_NEW_ENV
    {
      this->value_->_tao_encode (cdr
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 1;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  
  return 0;
}

CORBA::Boolean
TAO::Any_Dual_Impl_T<CORBA::Exception>::demarshal_value (TAO_InputCDR &cdr)
{
  ACE_TRY_NEW_ENV
    {
      this->value_->_tao_decode (cdr
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 1;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  
  return 0;
}

// =======================================================================

TAO::Any_Any_Impl::Any_Any_Impl (void)
  : TAO::Any_Dual_Impl_T<CORBA::Any> (CORBA::_tc_any),
    any_holder_ (CORBA::Any ())
{
  this->value_ = &this->any_holder_;
}

TAO::Any_Any_Impl::Any_Any_Impl (const CORBA::Any &val)
  : TAO::Any_Dual_Impl_T<CORBA::Any> (CORBA::_tc_any),
    any_holder_ (val)
{
  this->value_ = &this->any_holder_;
}

TAO::Any_Any_Impl::~Any_Any_Impl (void)
{
}

void
TAO::Any_Any_Impl::insert_copy (CORBA::Any & any,
                                const CORBA::Any & value)
{
  TAO::Any_Any_Impl *new_impl = 0;
  ACE_NEW (new_impl,
           TAO::Any_Any_Impl (value));
  any.replace (new_impl);
}

CORBA::Boolean
TAO::Any_Any_Impl::extract_ref (const CORBA::Any & any,
                                CORBA::Any & _tao_elem)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::TypeCode_ptr any_tc = any._tao_get_typecode ();
      CORBA::Boolean _tao_equiv = any_tc->equivalent (CORBA::_tc_any
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (_tao_equiv == 0)
        {
          return 0;
        }

      TAO::Any_Impl *impl = any.impl ();

      TAO::Any_Any_Impl *narrow_impl =
        dynamic_cast <TAO::Any_Any_Impl *> (impl);

      if (narrow_impl != 0)
        {
          _tao_elem = *narrow_impl->value_;
          return 1;
        }

      TAO::Any_Any_Impl *replacement = 0;
      ACE_NEW_RETURN (replacement,
                      TAO::Any_Any_Impl,
                      0);
                      
      auto_ptr<TAO::Any_Any_Impl> replacement_safety (replacement);

      TAO_InputCDR cdr (impl->_tao_get_cdr (),
                        impl->_tao_byte_order ());

      CORBA::Boolean result = replacement->demarshal_value (cdr);

      if (result == 1)
        {
          _tao_elem = *replacement->value_;
          ACE_const_cast (CORBA::Any &, any).replace (replacement);
          replacement_safety.release ();
          return result;
        }
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  
  return 0;
}

void
TAO::Any_Any_Impl::free_value (void)
{
  this->any_holder_.impl ()->free_value ();
}


