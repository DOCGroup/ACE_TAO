// $Id$

#include "Unknown_User_Exception.h"
#include "tao/Any.h"

ACE_RCSID(DynamicInterface, Unknown_User_Exception, "$Id$")

CORBA_UnknownUserException::CORBA_UnknownUserException (void)
  : CORBA_UserException ("IDL:omg.org/CORBA/UnknownUserException:1.0"),
    exception_ (0)
{
}

CORBA_UnknownUserException::CORBA_UnknownUserException (CORBA_Any &ex)
  : CORBA_UserException ("IDL:omg.org/CORBA/UnknownUserException:1.0")
{
  ACE_NEW (this->exception_,
           CORBA_Any (ex));
}

CORBA_UnknownUserException::CORBA_UnknownUserException (
    const CORBA_UnknownUserException& e
  )
  : CORBA_UserException (e._id ())
{
  ACE_NEW (this->exception_,
           CORBA_Any (*e.exception_));
}

CORBA_UnknownUserException::~CORBA_UnknownUserException (void)
{
  delete this->exception_;
}

CORBA_Any &
CORBA_UnknownUserException::exception (void)
{
  return *this->exception_;
}

int
CORBA_UnknownUserException::_is_a (const char *interface_id) const
{
  return
    ((ACE_OS_String::strcmp (interface_id,
                             "IDL:omg.org/CORBA/UnknownUserException:1.0")
        == 0)
      || CORBA_UserException::_is_a (interface_id));
}

CORBA_UnknownUserException *
CORBA_UnknownUserException::_downcast (CORBA_Exception *ex)
{
  if (ex->_is_a ("IDL:omg.org/CORBA/UnknownUserException:1.0"))
    {
      return ACE_dynamic_cast (CORBA_UnknownUserException *,
                               ex);
    }

  return 0;
}

void
CORBA_UnknownUserException::_raise (void)
{
  TAO_RAISE (*this);
}

void
CORBA_UnknownUserException::_tao_encode (
    TAO_OutputCDR &
    TAO_ENV_ARG_DECL
  ) const
{
  ACE_THROW (CORBA::MARSHAL ());
}

void
CORBA_UnknownUserException::_tao_decode (TAO_InputCDR &
                                         TAO_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::MARSHAL ());
}

CORBA::TypeCode_ptr
CORBA_UnknownUserException::_type (void) const
{
  return CORBA::_tc_UnknownUserException;
}

