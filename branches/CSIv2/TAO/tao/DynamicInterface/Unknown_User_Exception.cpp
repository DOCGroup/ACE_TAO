// $Id$

#include "Unknown_User_Exception.h"
#include "tao/Any.h"

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
    ((ACE_OS_String::strcmp (interface_id,
                             "IDL:omg.org/CORBA/UnknownUserException:1.0")
        == 0)
      || UserException::_is_a (interface_id));
}

CORBA::UnknownUserException *
CORBA::UnknownUserException::_downcast (CORBA::Exception *ex)
{
  if (ex->_is_a ("IDL:omg.org/CORBA/UnknownUserException:1.0"))
    {
      return ACE_dynamic_cast (CORBA::UnknownUserException *,
                               ex);
    }

  return 0;
}

void
CORBA::UnknownUserException::_raise (void)
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
