// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/Any_Basic_Impl.h"
#include "tao/AnyTypeCode/Any_Impl_T.h"
#include "tao/AnyTypeCode/Any_Special_Impl_T.h"
#include "tao/AnyTypeCode/Any_Dual_Impl_T.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_wchar.h"
#include "ace/OS_Memory.h"

#if !defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/Any.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

using namespace TAO;

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
CORBA::Any::replace (TAO::Any_Impl *new_impl)
{
  ACE_ASSERT (new_impl != 0);

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

void
CORBA::Any::_tao_set_typecode (const CORBA::TypeCode_ptr tc)
{
  if (this->impl_ == 0)
    {
      ACE_NEW (this->impl_,
               TAO::Unknown_IDL_Type (tc));
    }
  else
    {
      this->impl_->type (tc);
    }
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
CORBA::Any::type (CORBA::TypeCode_ptr tc)
{
  CORBA::Boolean equiv = false;

  if (this->impl_ != 0)
    {
      equiv = this->impl_->_tao_get_typecode ()->equivalent (tc);
    }

  if (equiv)
    {
      this->impl_->type (tc);
    }
  else
    {
      throw ::CORBA::BAD_TYPECODE ();
    }
}

void
CORBA::Any::_tao_any_destructor (void *x)
{
  CORBA::Any *tmp = static_cast<CORBA::Any *> (x);
  delete tmp;
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
      return false;
    }

  return this->impl_->to_value (_tao_elem);
}

CORBA::Boolean
CORBA::Any::checked_to_abstract_base (CORBA::AbstractBase_ptr &_tao_elem) const
{
  if (this->impl_ == 0)
    {
      return false;
    }

  return this->impl_->to_abstract_base (_tao_elem);
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
  CORBA::TypeCode_var tc;

  if ((cdr >> tc.out ()) == 0)
    {
      return false;
    }

  try
    {
      TAO::Unknown_IDL_Type *impl = 0;
      ACE_NEW_RETURN (impl,
                      TAO::Unknown_IDL_Type (tc.in ()),
                      false);

      any.replace (impl);
      impl->_tao_decode (cdr);
    }
  catch (const ::CORBA::Exception&)
    {
      return false;
    }

  return true;
}

#if defined (GEN_OSTREAM_OPS)

std::ostream &
operator<< (std::ostream &strm, const CORBA::Any &)
{
  return strm << "CORBA::Any";
}

#endif /* GEN_OSTREAM_OPS */

// =======================================================================

// Insertion of the special basic types.

void
CORBA::Any::operator<<= (CORBA::Any::from_boolean b)
{
  TAO::Any_Basic_Impl::insert (*this, CORBA::_tc_boolean, &b.val_);
}

void
CORBA::Any::operator<<= (CORBA::Any::from_octet o)
{
  TAO::Any_Basic_Impl::insert (*this, CORBA::_tc_octet, &o.val_);
}

void
CORBA::Any::operator<<= (CORBA::Any::from_char c)
{
  TAO::Any_Basic_Impl::insert (*this, CORBA::_tc_char, &c.val_);
}

void
CORBA::Any::operator<<= (CORBA::Any::from_wchar wc)
{
  TAO::Any_Basic_Impl::insert (*this, CORBA::_tc_wchar, &wc.val_);
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

#ifdef ACE_ANY_OPS_USE_NAMESPACE
namespace CORBA
{
#endif

void
operator<<= (CORBA::Any &any, CORBA::Short s)
{
  TAO::Any_Basic_Impl::insert (any, CORBA::_tc_short, &s);
}

void
operator<<= (CORBA::Any &any, CORBA::UShort us)
{
  TAO::Any_Basic_Impl::insert (any, CORBA::_tc_ushort, &us);
}

void
operator<<= (CORBA::Any &any, CORBA::Long l)
{
  TAO::Any_Basic_Impl::insert (any, CORBA::_tc_long, &l);
}

void
operator<<= (CORBA::Any &any, CORBA::ULong ul)
{
  TAO::Any_Basic_Impl::insert (any, CORBA::_tc_ulong, &ul);
}

void
operator<<= (CORBA::Any &any, CORBA::LongLong ll)
{
  TAO::Any_Basic_Impl::insert (any, CORBA::_tc_longlong, &ll);
}

void
operator<<= (CORBA::Any &any, CORBA::ULongLong ull)
{
  TAO::Any_Basic_Impl::insert (any, CORBA::_tc_ulonglong, &ull);
}

void
operator<<= (CORBA::Any &any, CORBA::Float f)
{
  TAO::Any_Basic_Impl::insert (any, CORBA::_tc_float, &f);
}

void
operator<<= (CORBA::Any &any, CORBA::Double d)
{
  TAO::Any_Basic_Impl::insert (any, CORBA::_tc_double, &d);
}

void
operator<<= (CORBA::Any &any, CORBA::LongDouble ld)
{
  TAO::Any_Basic_Impl::insert (any, CORBA::_tc_longdouble, &ld);
}

// Insertion of Any - copying.
void
operator<<= (CORBA::Any &any, const CORBA::Any &a)
{
  if (0 == &a) // Trying to de-reference NULL Any
    any <<= static_cast<CORBA::Any *>( 0 ); // Use non-copying insertion of a NULL
  else
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
// to avoid use in Any.inl before definition in ORB.inl.

void
operator<<= (CORBA::Any &any, const char *s)
{
  TAO::Any_Special_Impl_T<
      char,
      CORBA::Any::from_string,
      CORBA::Any::to_string
    >::insert (any,
               TAO::Any_Impl::_tao_any_string_destructor,
               CORBA::_tc_string,
               CORBA::string_dup (s),
               0);
}

void
operator<<= (CORBA::Any &any, const CORBA::WChar *ws)
{
  TAO::Any_Special_Impl_T<
      CORBA::WChar,
      CORBA::Any::from_wstring,
      CORBA::Any::to_wstring
    >::insert (any,
               TAO::Any_Impl::_tao_any_wstring_destructor,
               CORBA::_tc_wstring,
               CORBA::wstring_dup (ws),
               0);
}

void
operator <<= (CORBA::Any &any, const std::string & str)
{
  any <<= str.c_str ();
}

void
operator <<= (CORBA::Any &, std::string *)
{
  // @todo
}

#if !defined(ACE_LACKS_STD_WSTRING)
void
operator <<= (CORBA::Any &any, const std::wstring & str)
{
  any <<= str.c_str ();
}

void
operator <<= (CORBA::Any &, std::wstring *)
{
  // @todo
}
#endif

// Extraction: these are safe and hence we have to check that the
// typecode of the Any is equal to the one we are trying to extract
// into.

// Extraction into the special basic types.

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_boolean b) const
{
  return TAO::Any_Basic_Impl::extract (*this, CORBA::_tc_boolean, &b.ref_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_octet o) const
{
  return TAO::Any_Basic_Impl::extract (*this, CORBA::_tc_octet, &o.ref_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_char c) const
{
  return TAO::Any_Basic_Impl::extract (*this, CORBA::_tc_char, &c.ref_);
}

CORBA::Boolean
CORBA::Any::operator>>= (CORBA::Any::to_wchar wc) const
{
  return TAO::Any_Basic_Impl::extract (*this, CORBA::_tc_wchar, &wc.ref_);
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
  return TAO::Any_Basic_Impl::extract (any, CORBA::_tc_short, &s);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::UShort &us)
{
  return TAO::Any_Basic_Impl::extract (any,
                                       CORBA::_tc_ushort,
                                       &us);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::Long &l)
{
  return TAO::Any_Basic_Impl::extract (any,
                                       CORBA::_tc_long,
                                       &l);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::ULong &ul)
{
  return TAO::Any_Basic_Impl::extract (any,
                                       CORBA::_tc_ulong,
                                       &ul);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::LongLong &ll)
{
  return
    TAO::Any_Basic_Impl::extract (any,
                                  CORBA::_tc_longlong,
                                  &ll);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::ULongLong &ull)
{
  return
    TAO::Any_Basic_Impl::extract (any,
                                  CORBA::_tc_ulonglong,
                                  &ull);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::Float &f)
{
  return TAO::Any_Basic_Impl::extract (any,
                                       CORBA::_tc_float,
                                       &f);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::Double &d)
{
  return TAO::Any_Basic_Impl::extract (any,
                                       CORBA::_tc_double,
                                       &d);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, CORBA::LongDouble &ld)
{
  return
    TAO::Any_Basic_Impl::extract (any,
                                  CORBA::_tc_longdouble,
                                  &ld);
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
operator>>= (const CORBA::Any &any, const CORBA::Char *&s)
{
  return
    TAO::Any_Special_Impl_T<
        char,
        CORBA::Any::from_string,
        CORBA::Any::to_string
      >::extract (any,
                  TAO::Any_Impl::_tao_any_string_destructor,
                  CORBA::_tc_string,
                  s,
                  0);
}

CORBA::Boolean
operator>>= (const CORBA::Any &any, const CORBA::WChar *&ws)
{
  return
    TAO::Any_Special_Impl_T<
        CORBA::WChar,
        CORBA::Any::from_wstring,
        CORBA::Any::to_wstring
      >::extract (any,
                  TAO::Any_Impl::_tao_any_wstring_destructor,
                  CORBA::_tc_wstring,
                  ws,
                  0);
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

CORBA::Boolean
operator >>= (const CORBA::Any &any, std::string &str)
{
  const char *buf = 0;
  CORBA::Boolean flag = any >>= buf;
  str.assign (buf);
  ACE::strdelete (const_cast <char *> (buf));
  return flag;
}

#if !defined(ACE_LACKS_STD_WSTRING)
CORBA::Boolean
operator >>= (const CORBA::Any &any, std::wstring &str)
{
  const wchar_t *buf = 0;
  CORBA::Boolean const flag = any >>= buf;
  str.assign (buf);
  ACE::strdelete (const_cast <wchar_t *> (buf));
  return flag;
}
#endif

#ifdef ACE_ANY_OPS_USE_NAMESPACE
}
#endif

// ================================================================
// Any_Impl_T template specializations.

namespace TAO
{
  template<>
  CORBA::Boolean
  Any_Impl_T<CORBA::Object>::to_object (CORBA::Object_ptr &_tao_elem) const
  {
    _tao_elem = CORBA::Object::_duplicate (this->value_);
    return true;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
