// $Id$

#include "tao/Any_Impl_T.h"
#include "tao/Any_Basic_Impl_T.h"
#include "tao/Any_Special_Impl_T.h"
#include "tao/Any_Special_Basic_Impl_T.h"
#include "tao/Any_Dual_Impl_T.h"

#include "tao/ORB_Core.h"
#include "tao/Valuetype_Adapter.h"

#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
# include "tao/Any.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           Any,
           "$Id$")

using namespace TAO;

// Specializations of the create_empty method for long long and long double.

template<>
Any_Basic_Impl_T<CORBA::LongLong> *
Any_Basic_Impl_T<CORBA::LongLong>::create_empty (
    CORBA::TypeCode_ptr tc
  )
{
  const CORBA::LongLong zero = ACE_CDR_LONGLONG_INITIALIZER;
  Any_Basic_Impl_T<CORBA::LongLong> * retval = 0;
  ACE_NEW_RETURN (retval,
                  Any_Basic_Impl_T<CORBA::LongLong> (tc,
                                                     zero),
                  0);
  return retval;
}

template<>
Any_Basic_Impl_T<CORBA::LongDouble> *
Any_Basic_Impl_T<CORBA::LongDouble>::create_empty (
    CORBA::TypeCode_ptr tc
  )
{
  const CORBA::LongDouble zero = ACE_CDR_LONG_DOUBLE_INITIALIZER;
  Any_Basic_Impl_T<CORBA::LongDouble> * retval = 0;
  ACE_NEW_RETURN (retval,
                  Any_Basic_Impl_T<CORBA::LongDouble> (tc,
                                                       zero),
                  0);
  return retval;
}

// =======================================================================

// Specializations for CORBA::Exception

template<>
Any_Dual_Impl_T<CORBA::Exception>::Any_Dual_Impl_T (
    _tao_destructor destructor,
    CORBA::TypeCode_ptr tc,
    const CORBA::Exception & val
  )
  : Any_Impl (destructor,
              tc)
{
  this->value_ = val._tao_duplicate ();
}

template<>
CORBA::Boolean
Any_Dual_Impl_T<CORBA::Exception>::marshal_value (TAO_OutputCDR &cdr)
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

template<>
CORBA::Boolean
Any_Dual_Impl_T<CORBA::Exception>::demarshal_value (TAO_InputCDR &cdr)
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

// This should never get called since we don't have extraction operators
// for CORBA::Exception, but it is here to sidestep the constructor call
// in the unspecialized version that causes a problem with compilers that
// require explicit instantiation
template<>
CORBA::Boolean
Any_Dual_Impl_T<CORBA::Exception>::extract (const CORBA::Any &,
                                            _tao_destructor,
                                            CORBA::TypeCode_ptr,
                                            const CORBA::Exception *&)
{
  return 0;
}

// =======================================================================

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

CORBA::Any::Any (CORBA::TypeCode_ptr tc,
                 void *value,
                 CORBA::Boolean release)
{
  if (value != 0)
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_THROW (CORBA::NO_IMPLEMENT ());
      return;
    }

  ACE_NEW (this->impl_,
           TAO::Unknown_IDL_Type (tc,
                                  0,
                                  ACE_CDR_BYTE_ORDER,
                                  release));
}

CORBA::Any::~Any (void)
{
  if (this->impl_ != 0)
    {
      this->impl_->_remove_ref ();
    }
}

CORBA::Any &
CORBA::Any::operator= (const CORBA::Any &rhs)
{
  if (this->impl_ != rhs.impl_)
    {
      if (this->impl_ != 0)
        {
          this->impl_->_remove_ref ();
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
CORBA::Any::replace (CORBA::TypeCode_ptr,
                     void *,
                     CORBA::Boolean)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
CORBA::Any::replace (TAO::Any_Impl *new_impl)
{
  if (this->impl_ != 0)
    {
      this->impl_->_remove_ref ();
    }

  this->impl_ = new_impl;

  ACE_ASSERT (this->impl_ != 0);
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

CORBA::Boolean
CORBA::Any::checked_to_object (CORBA::Object_ptr &_tao_elem) const
{
  if (this->impl_ == 0)
    {
      return 0;
    }

  return this->impl_->to_object (_tao_elem);
}

CORBA::Boolean
CORBA::Any::checked_to_value (CORBA::ValueBase *&_tao_elem) const
{
  if (this->impl_ == 0)
    {
      return 0;
    }

  return this->impl_->to_value (_tao_elem);
}

CORBA::Boolean
CORBA::Any::checked_to_abstract_base (
    CORBA::AbstractBase_ptr &_tao_elem
  ) const
{
  if (this->impl_ == 0)
    {
      return 0;
    }

  return this->impl_->to_abstract_base (_tao_elem);
}

// =======================================================================

TAO::Any_Impl::Any_Impl (_tao_destructor destructor,
                         CORBA::TypeCode_ptr tc)
  : value_destructor_ (destructor),
    type_ (tc),
    refcount_ (1)
{
}

TAO::Any_Impl::~Any_Impl (void)
{
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
  ++this->refcount_;
}

void
TAO::Any_Impl::_remove_ref (void)
{
  if (--this->refcount_ != 0)
    {
      return;
    }

  this->free_value ();
  delete this;
}

void
TAO::Any_Impl::_tao_decode (TAO_InputCDR &
                            ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO::Any_Impl::assign_translator (CORBA::TCKind,
                                  TAO_InputCDR *)
{
}

// =======================================================================

TAO::Unknown_IDL_Type::Unknown_IDL_Type (
    CORBA::TypeCode_ptr tc,
    const ACE_Message_Block *mb,
    int byte_order,
    CORBA::Boolean release_tc,
    ACE_Char_Codeset_Translator *ctrans,
    ACE_WChar_Codeset_Translator *wtrans
  )
  : TAO::Any_Impl (0,
                   tc),
    cdr_ (ACE_Message_Block::duplicate (mb)),
    byte_order_ (byte_order),
    release_tc_ (release_tc),
    char_translator_ (ctrans),
    wchar_translator_ (wtrans)
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
  if (this->release_tc_ == 1)
    {
      CORBA::release (this->type_);
    }

  ACE_Message_Block::release (this->cdr_);
}

void
TAO::Unknown_IDL_Type::_tao_decode (TAO_InputCDR &cdr
                                    ACE_ENV_ARG_DECL)
{
  // @@ (JP) The following code depends on the fact that
  //         TAO_InputCDR does not contain chained message blocks,
  //         otherwise <begin> and <end> could be part of
  //         different buffers!

  // This will be the start of a new message block.
  char *begin = cdr.rd_ptr ();

  // Skip over the next argument.
  CORBA::TypeCode::traverse_status status =
    TAO_Marshal_Object::perform_skip (this->type_,
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
  // ACE_CDR::MAX_ALIGNMENT - 1 bytes. Similarly, the offset adjustment
  // can move the rd_ptr by up to the same amount. We accommodate
  // this by including 2 * ACE_CDR::MAX_ALIGNMENT bytes of additional
  // space in the message block.
  size_t size = end - begin;

  ACE_Message_Block::release (this->cdr_);
  ACE_NEW (this->cdr_,
           ACE_Message_Block (size + 2 * ACE_CDR::MAX_ALIGNMENT));

  ACE_CDR::mb_align (this->cdr_);
  ptrdiff_t offset = ptrdiff_t (begin) % ACE_CDR::MAX_ALIGNMENT;

  if (offset < 0)
    offset += ACE_CDR::MAX_ALIGNMENT;

  this->cdr_->rd_ptr (offset);
  this->cdr_->wr_ptr (offset + size);

  ACE_OS::memcpy (this->cdr_->rd_ptr (),
                  begin,
                  size);

  // Get character translators.
  this->char_translator_ = cdr.char_translator();
  this->wchar_translator_ = cdr.wchar_translator();
}

void
TAO::Unknown_IDL_Type::assign_translator (CORBA::TCKind kind,
                                          TAO_InputCDR *cdr)
{
  switch (kind)
    {
      case CORBA::tk_string:
      case CORBA::tk_char:
        cdr->char_translator (this->char_translator_);
        break;
      case CORBA::tk_wstring:
      case CORBA::tk_wchar:
        cdr->wchar_translator(this->wchar_translator_);
        break;
      default:
        break;
    }
}

CORBA::Boolean
TAO::Unknown_IDL_Type::to_object (CORBA::Object_ptr &obj) const
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ULong kind =
        this->type_->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_);

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_objref)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_,
                           TAO_DEF_GIOP_MAJOR,
                           TAO_DEF_GIOP_MINOR,
                           TAO_ORB_Core_instance ());

      return stream >> obj;
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
TAO::Unknown_IDL_Type::to_value (CORBA::ValueBase *&val) const
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ULong kind =
        this->type_->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_);

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_value)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_,
                           TAO_DEF_GIOP_MAJOR,
                           TAO_DEF_GIOP_MINOR,
                           TAO_ORB_Core_instance ());

      TAO_Valuetype_Adapter *adapter =
        ACE_Dynamic_Service<TAO_Valuetype_Adapter>::instance (
            TAO_ORB_Core::valuetype_adapter_name ()
          );

      if (adapter == 0)
        {
          ACE_THROW_RETURN (CORBA::INTERNAL (),
                            0);
        }

      return adapter->stream_to_value (stream,
                                       val);
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::Boolean
TAO::Unknown_IDL_Type::to_abstract_base (CORBA::AbstractBase_ptr &obj) const
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ULong kind =
        this->type_->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (this->type_);

      while (kind == CORBA::tk_alias)
        {
          tcvar = tcvar->content_type (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          kind = tcvar->kind (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (kind != CORBA::tk_value)
        {
          return 0;
        }

      TAO_InputCDR stream (this->cdr_,
                           this->byte_order_,
                           TAO_DEF_GIOP_MAJOR,
                           TAO_DEF_GIOP_MINOR,
                           TAO_ORB_Core_instance ());

      TAO_Valuetype_Adapter *adapter =
        ACE_Dynamic_Service<TAO_Valuetype_Adapter>::instance (
            TAO_ORB_Core::valuetype_adapter_name ()
          );

      if (adapter == 0)
        {
          ACE_THROW_RETURN (CORBA::INTERNAL (),
                            0);
        }

      return adapter->stream_to_abstract_base (stream,
                                               obj);
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
    }
  ACE_ENDTRY;

  return 0;
}

// ****************************************************************

CORBA::Any_var::Any_var (const CORBA::Any_var &r)
  : ptr_ (0)
{
  if (r.ptr_ != 0)
    {
      ACE_NEW (this->ptr_,
               CORBA::Any (*r.ptr_));
    }
}

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
  delete this->ptr_;
  this->ptr_ = 0;

  if (r.ptr_ != 0)
    {
      ACE_NEW_RETURN (this->ptr_,
                      CORBA::Any (*r.ptr_),
                      *this);
    }

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
      TAO::Unknown_IDL_Type *impl = 0;
      ACE_NEW_RETURN (impl,
                      TAO::Unknown_IDL_Type (tc,
                                             0,
                                             cdr.byte_order ()),
                      0);

      any.replace (impl);
      impl->_tao_decode (cdr
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
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
  if (s.bound_ > 0 && s.val_ != 0 && ACE_OS::strlen (s.val_) > s.bound_)
    {
      return;
    }

  TAO::Any_Special_Impl_T<
      char,
      CORBA::Any::from_string,
      CORBA::Any::to_string
    >::insert (*this,
               TAO::Any_Impl::_tao_any_string_destructor,
               CORBA::_tc_string,
               s.nocopy_ ? s.val_ : CORBA::string_dup (s.val_),
               s.bound_);
}

void
CORBA::Any::operator<<= (CORBA::Any::from_wstring ws)
{
  if (ws.bound_ > 0 && ws.val_ != 0 && ACE_OS::wslen (ws.val_) > ws.bound_)
    {
      return;
    }

  TAO::Any_Special_Impl_T<
      CORBA::WChar,
      CORBA::Any::from_wstring,
      CORBA::Any::to_wstring
    >::insert (*this,
               TAO::Any_Impl::_tao_any_wstring_destructor,
               CORBA::_tc_wstring,
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
  TAO::Any_Dual_Impl_T<CORBA::Any>::insert_copy (
      any,
      CORBA::Any::_tao_any_destructor,
      CORBA::_tc_any,
      a
    );
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

// Insertion of TypeCode - copying.
void
operator<<= (CORBA::Any &any, CORBA::TypeCode_ptr tc)
{
  CORBA::TypeCode_ptr dup = CORBA::TypeCode::_duplicate (tc);
  any <<= &dup;
}

// Insertion of TypeCode - non-copying.
void
operator<<= (CORBA::Any &any, CORBA::TypeCode_ptr * tc)
{
  TAO::Any_Impl_T<CORBA::TypeCode>::insert (
      any,
      CORBA::TypeCode::_tao_any_destructor,
      CORBA::_tc_TypeCode,
      *tc
    );
}

// Insertion of CORBA::Exception - copying.
void
operator<<= (CORBA::Any &any, const CORBA::Exception &exception)
{
  TAO::Any_Dual_Impl_T<CORBA::Exception>::insert_copy (
      any,
      CORBA::Exception::_tao_any_destructor,
      exception._type (),
      exception
    );
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
                  CORBA::_tc_string,
                  s.val_,
                  s.bound_);
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
                  CORBA::_tc_wstring,
                  ws.val_,
                  ws.bound_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_object obj) const
{
  return this->checked_to_object (obj.ref_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_abstract_base obj) const
{
  return this->checked_to_abstract_base (obj.ref_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_value obj) const
{
  return this->checked_to_value (obj.ref_);
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
      ACE_const_cast (char *&, s)
    );
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, const CORBA::WChar *&ws)
{
  return TAO::Any_Impl_T<CORBA::WChar>::extract (
      any,
      TAO::Any_Impl::_tao_any_wstring_destructor,
      CORBA::_tc_wstring,
      ACE_const_cast (CORBA::WChar *&, ws)
    );
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::TypeCode_ptr &tc)
{
  return TAO::Any_Impl_T<CORBA::TypeCode>::extract (
      any,
      CORBA::TypeCode::_tao_any_destructor,
      CORBA::_tc_TypeCode,
      tc
    );
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::Any_Special_Basic_Impl_T<
      CORBA::Boolean,
      CORBA::Any::from_boolean,
      CORBA::Any::to_boolean
    >;

template class TAO::Any_Special_Basic_Impl_T<
      CORBA::Octet,
      CORBA::Any::from_octet,
      CORBA::Any::to_octet
    >;

template class TAO::Any_Special_Basic_Impl_T<
      char,
      CORBA::Any::from_char,
      CORBA::Any::to_char
    >;

template class TAO::Any_Special_Basic_Impl_T<
      CORBA::WChar,
      CORBA::Any::from_wchar,
      CORBA::Any::to_wchar
    >;

template class TAO::Any_Special_Impl_T<
      char,
      CORBA::Any::from_string,
      CORBA::Any::to_string
    >;

template class TAO::Any_Special_Impl_T<
      CORBA::WChar,
      CORBA::Any::from_wstring,
      CORBA::Any::to_wstring
    >;

template class TAO::Any_Basic_Impl_T<CORBA::Short>;
template class TAO::Any_Basic_Impl_T<CORBA::UShort>;
template class TAO::Any_Basic_Impl_T<CORBA::Long>;
template class TAO::Any_Basic_Impl_T<CORBA::ULong>;
template class TAO::Any_Basic_Impl_T<CORBA::LongLong>;
template class TAO::Any_Basic_Impl_T<CORBA::ULongLong>;
template class TAO::Any_Basic_Impl_T<CORBA::Float>;
template class TAO::Any_Basic_Impl_T<CORBA::Double>;
template class TAO::Any_Basic_Impl_T<CORBA::LongDouble>;
template class TAO::Any_Dual_Impl_T<CORBA::Any>;
template class TAO::Any_Impl_T<CORBA::TypeCode>;
template class TAO::Any_Dual_Impl_T<CORBA::Exception>;
template class TAO::Any_Impl_T<char>;
template class TAO::Any_Impl_T<CORBA::WChar>;
template class TAO::Any_Impl_T<CORBA::Object>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO::Any_Special_Basic_Impl_T<
      CORBA::Boolean,
      CORBA::Any::from_boolean,
      CORBA::Any::to_boolean
    >

#pragma instantiate TAO::Any_Special_Basic_Impl_T<
      CORBA::Octet,
      CORBA::Any::from_octet,
      CORBA::Any::to_octet
    >

#pragma instantiate TAO::Any_Special_Basic_Impl_T<
      char,
      CORBA::Any::from_char,
      CORBA::Any::to_char
    >

#pragma instantiate TAO::Any_Special_Basic_Impl_T<
      CORBA::WChar,
      CORBA::Any::from_wchar,
      CORBA::Any::to_wchar
    >

#pragma instantiate TAO::Any_Special_Impl_T<
      char,
      CORBA::Any::from_string,
      CORBA::Any::to_string
    >

#pragma instantiate TAO::Any_Special_Impl_T<
      CORBA::WChar,
      CORBA::Any::from_wstring,
      CORBA::Any::to_wstring
    >

#pragma instantiate TAO::Any_Basic_Impl_T<CORBA::Short>
#pragma instantiate TAO::Any_Basic_Impl_T<CORBA::UShort>
#pragma instantiate TAO::Any_Basic_Impl_T<CORBA::Long>
#pragma instantiate TAO::Any_Basic_Impl_T<CORBA::ULong>
#pragma instantiate TAO::Any_Basic_Impl_T<CORBA::LongLong>
#pragma instantiate TAO::Any_Basic_Impl_T<CORBA::ULongLong>
#pragma instantiate TAO::Any_Basic_Impl_T<CORBA::Float>
#pragma instantiate TAO::Any_Basic_Impl_T<CORBA::Double>
#pragma instantiate TAO::Any_Basic_Impl_T<CORBA::LongDouble>
#pragma instantiate TAO::Any_Dual_Impl_T<CORBA::Any>
#pragma instantiate TAO::Any_Impl_T<CORBA::TypeCode>
#pragma instantiate TAO::Any_Dual_Impl_T<CORBA::Exception>
#pragma instantiate TAO::Any_Impl_T<char>
#pragma instantiate TAO::Any_Impl_T<CORBA::WChar>
#pragma instantiate TAO::Any_Impl_T<CORBA::Object>
#pragma instantiate TAO::Any_Impl_T<CORBA::AbstractBase>
#pragma instantiate TAO::Any_Impl_T<CORBA::ValueBase>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
