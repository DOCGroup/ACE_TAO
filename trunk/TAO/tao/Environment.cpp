// $Id$

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
# include "tao/Environment.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Environment, "$Id$")

CORBA_Environment::CORBA_Environment (void)
  : exception_ (0),
    previous_ (TAO_ORB_Core_instance ()->default_environment ())
{
  TAO_ORB_Core_instance ()->default_environment (this);
}

CORBA_Environment::CORBA_Environment (TAO_ORB_Core* orb_core)
  : exception_ (0),
    previous_ (orb_core->default_environment ())
{
  orb_core->default_environment (this);
}

CORBA_Environment::CORBA_Environment (const CORBA_Environment& rhs)
  : exception_ (rhs.exception_),
    previous_ (TAO_ORB_Core_instance ()->default_environment ())
{
  TAO_ORB_Core_instance ()->default_environment (this);
  exception_->_incr_refcnt ();
}

CORBA_Environment&
CORBA_Environment::operator= (const CORBA_Environment& rhs)
{
  if (this != &rhs)
    {
      this->clear ();
      this->exception (rhs.exception_);
    }
  return *this;
}

CORBA_Environment::~CORBA_Environment (void)
{
  this->clear ();
  TAO_ORB_Core_instance ()->default_environment (this->previous_);
}

void
CORBA_Environment::exception (CORBA_Exception *ex)
{
  if (ex != this->exception_)
    {
      this->clear ();
      this->exception_ = ex;
      this->exception_->_incr_refcnt ();
    }
#if defined (TAO_USE_EXCEPTIONS)
  ex->_raise ();
#endif /* TAO_USE_EXCEPTIONS */
}

void
CORBA_Environment::clear (void)
{
  if (this->exception_)
    this->exception_->_decr_refcnt ();

  this->exception_ = 0;
}

CORBA_Environment&
CORBA_Environment::default_environment ()
{
  return *TAO_ORB_Core_instance ()->default_environment ();
}

// Convenience -- say if the exception is a system exception or not.

int
CORBA::Environment::exception_type (void) const
{
  static char sysex_prefix [] = "IDL:omg.org/CORBA/";
  static char typecode_extra [] = "TypeCode/";
  static char poa_prefix [] = "IDL:PortableServer/";

  if (!this->exception_)
    return CORBA::NO_EXCEPTION;

  // All exceptions currently (CORBA 2.0) defined in the CORBA
  // scope are system exceptions ... except for a couple that
  // are related to TypeCodes.

  const char *id = this->exception_->_id ();

  if ((ACE_OS::strncmp (id, sysex_prefix, sizeof sysex_prefix - 1) == 0
       && ACE_OS::strncmp (id + sizeof sysex_prefix - 1,
                           typecode_extra, sizeof typecode_extra - 1) != 0)
      || ACE_OS::strncmp (id, poa_prefix, sizeof poa_prefix - 1) == 0)
    return CORBA::SYSTEM_EXCEPTION;
  
  return CORBA::USER_EXCEPTION;
}

// Diagnostic utility routine: describe the exception onto the
// standard I/O stream passed as a parameter.

void
CORBA::Environment::print_exception (const char *info,
                                     FILE *) const
{
  const char *id = this->exception_->_id ();

  ACE_DEBUG ((LM_ERROR, "(%P|%t) EXCEPTION, %s\n", info));

  // @@ get rid of this logic, and rely on some member function on
  // Exception to say if it's user or system exception.

  if (this->exception_type () == CORBA::SYSTEM_EXCEPTION)
    {
      CORBA::SystemException *x2 =
	CORBA_SystemException::_narrow (this->exception_);

      // @@ there are a other few "user exceptions" in the CORBA
      // scope, they're not all standard/system exceptions ... really
      // need to either compare exhaustively against all those IDs
      // (yeech) or (preferably) to represent the exception type
      // directly in the exception value so it can be queried.

      ACE_DEBUG ((LM_ERROR,
                  "(%P|%t) system exception, ID '%s'\n",
                  id));
      ACE_DEBUG ((LM_ERROR,
                  "(%P|%t) minor code = %x, completed = %s\n",
                  x2->minor (),
                  (x2->completion () == CORBA::COMPLETED_YES) ? "YES" :
                  (x2->completion () == CORBA::COMPLETED_NO) ? "NO" :
                  (x2->completion () == CORBA::COMPLETED_MAYBE) ? "MAYBE" :
                  "garbage"));
    }
  else
    // @@ we can use the exception's typecode to dump all the data
    // held within it ...

    ACE_DEBUG ((LM_ERROR,
                "(%P|%t) user exception, ID '%s'\n",
                id));
}
