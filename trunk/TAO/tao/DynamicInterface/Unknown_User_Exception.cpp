// $Id$

#include "Unknown_User_Exception.h"

#include "tao/Any.h"
#include "tao/Environment.h"
#include "tao/TC_Constants_Forward.h"
#include "tao/SystemException.h"

#include "ace/OS_NS_string.h"
#include "ace/OS_Memory.h"


ACE_RCSID (DynamicInterface,
           Unknown_User_Exception,
           "$Id$")


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

int
CORBA::UnknownUserException::_is_a (const char *interface_id) const
{
  return
    ((ACE_OS::strcmp (interface_id,
                      "IDL:omg.org/CORBA/UnknownUserException:1.0")
        == 0)
      || UserException::_is_a (interface_id));
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
  TAO_RAISE (*this);
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
CORBA::UnknownUserException::_tao_encode (
    TAO_OutputCDR &
    ACE_ENV_ARG_DECL
  ) const
{
  ACE_THROW (CORBA::MARSHAL ());
}

void
CORBA::UnknownUserException::_tao_decode (TAO_InputCDR &
                                         ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::MARSHAL ());
}

CORBA::TypeCode_ptr
CORBA::UnknownUserException::_type (void) const
{
  return CORBA::_tc_UnknownUserException;
}

namespace TAO
{
  namespace TypeCode
  {
    Struct_Field<char const *> const fields_CORBA_UnknownUserException[] =
      {
        { "exception", &CORBA::_tc_any }
      };

    Struct<char const *,
           Struct_Field<char const *> const *,
           CORBA::tk_except,
           TAO::Null_RefCount_Policy> tc_UnknownUserException (
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
