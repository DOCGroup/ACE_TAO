// $Id$

#include "tao/DynamicInterface/Unknown_User_Exception.h"

#include "tao/AnyTypeCode/Any.h"
#include "tao/SystemException.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/Null_RefCount_Policy.h"
#include "tao/AnyTypeCode/TypeCode_Struct_Field.h"
#include "tao/AnyTypeCode/Struct_TypeCode_Static.h"

#include "ace/OS_NS_string.h"
#include "ace/OS_Memory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::UnknownUserException::UnknownUserException (void)
  : CORBA::UserException ("IDL:omg.org/CORBA/UnknownUserException:1.0",
                          "UnknownUserException"),
    exception_ (0)
{
}

CORBA::UnknownUserException::UnknownUserException (CORBA::Any &ex)
  : CORBA::UserException ("IDL:omg.org/CORBA/UnknownUserException:1.0",
                         "UnknownUserException")
{
  ACE_NEW (this->exception_,
           CORBA::Any (ex));
}

CORBA::UnknownUserException::UnknownUserException (
    const CORBA::UnknownUserException& e
  )
  : CORBA::UserException (e._rep_id (),
                         e._name ())
{
  ACE_NEW (this->exception_,
           CORBA::Any (*e.exception_));
}

CORBA::UnknownUserException::~UnknownUserException (void)
{
  delete this->exception_;
}

CORBA::Any &
CORBA::UnknownUserException::exception (void)
{
  return *this->exception_;
}

CORBA::UnknownUserException *
CORBA::UnknownUserException::_downcast (CORBA::Exception *ex)
{
  return dynamic_cast<CORBA::UnknownUserException *> (ex);
}

CORBA::UnknownUserException const *
CORBA::UnknownUserException::_downcast (CORBA::Exception const * ex)
{
  return dynamic_cast<CORBA::UnknownUserException const *> (ex);
}

void
CORBA::UnknownUserException::_raise (void) const
{
  throw *this;
}

CORBA::Exception *
CORBA::UnknownUserException::_tao_duplicate (void) const
{
  CORBA::Exception *result;
  ACE_NEW_RETURN (
      result,
      CORBA::UnknownUserException (*this),
      0
    );
  return result;
}

void
CORBA::UnknownUserException::_tao_encode (TAO_OutputCDR &) const
{
  throw ::CORBA::MARSHAL ();
}

void
CORBA::UnknownUserException::_tao_decode (TAO_InputCDR &)
{
  throw ::CORBA::MARSHAL ();
}

const char *
CORBA::UnknownUserException::_rep_id (void) const
{
  return this->exception_->_tao_get_typecode ()->id ();
}

CORBA::TypeCode_ptr
CORBA::UnknownUserException::_tao_type (void) const
{
  //return CORBA::_tc_UnknownUserException;
  return this->exception_->_tao_get_typecode ();
}

namespace TAO
{
  namespace TypeCode
  {
    Struct_Field<char const *, CORBA::TypeCode_ptr const *> const
    fields_CORBA_UnknownUserException[] =
      {
        { "exception", &CORBA::_tc_any }
      };

    Struct<char const *,
           CORBA::TypeCode_ptr const *,
           Struct_Field<char const *, CORBA::TypeCode_ptr const *> const *,
           TAO::Null_RefCount_Policy> tc_UnknownUserException (
             CORBA::tk_except,
             "IDL:omg.org/CORBA/UnknownUserException:1.0",
             "UnknownUserException",
             TAO::TypeCode::fields_CORBA_UnknownUserException,
             1 /* # of fields */);
  }
}

namespace CORBA
{
  CORBA::TypeCode_ptr const _tc_UnknownUserException =
    &TAO::TypeCode::tc_UnknownUserException;
}

TAO_END_VERSIONED_NAMESPACE_DECL
