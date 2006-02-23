// $Id$

#include "Any_Impl.h"
#include "TypeCode.h"
#include "Marshal.h"

#include "tao/CORBA_String.h"
#include "tao/SystemException.h"

#include "ace/Guard_T.h"

ACE_RCSID (tao,
           Any_Impl,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Any_Impl::Any_Impl (_tao_destructor destructor,
                         CORBA::TypeCode_ptr tc,
                         bool encoded)
  : value_destructor_ (destructor)
  , type_ (CORBA::TypeCode::_duplicate (tc))
  , encoded_ (encoded)
  , refcount_ (1)
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
TAO::Any_Impl::free_value (void)
{
  // We always have to do this.
  ::CORBA::release (this->type_);
}

CORBA::TypeCode_ptr
TAO::Any_Impl::type (void) const
{
  return CORBA::TypeCode::_duplicate (this->type_);
}

CORBA::TypeCode_ptr
TAO::Any_Impl::_tao_get_typecode (void) const
{
  return this->type_;
}

void
TAO::Any_Impl::type (CORBA::TypeCode_ptr tc)
{
  ::CORBA::release (this->type_);
  this->type_ = CORBA::TypeCode::_duplicate (tc);
}

int
TAO::Any_Impl::_tao_byte_order (void) const
{
  return TAO_ENCAP_BYTE_ORDER;
}

void
TAO::Any_Impl::_tao_any_string_destructor (void *x)
{
  char *tmp = static_cast<char *> (x);
  CORBA::string_free (tmp);
}

void
TAO::Any_Impl::_tao_any_wstring_destructor (void *x)
{
  CORBA::WChar *tmp = static_cast<CORBA::WChar *> (x);
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
  const CORBA::ULong new_count = --this->refcount_;

  if (new_count != 0)
    return;

  this->free_value ();

  delete this;
}

void
TAO::Any_Impl::_tao_decode (TAO_InputCDR &
                            ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA::Boolean
TAO::Any_Impl::to_object (CORBA::Object_ptr &) const
{
  return 0;
}

CORBA::Boolean
TAO::Any_Impl::to_value (CORBA::ValueBase *&) const
{
  return 0;
}

CORBA::Boolean
TAO::Any_Impl::to_abstract_base (CORBA::AbstractBase_ptr &) const
{
  return 0;
}

bool
TAO::Any_Impl::encoded (void) const
{
  return this->encoded_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
