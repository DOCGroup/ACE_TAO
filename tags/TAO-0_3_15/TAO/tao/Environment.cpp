// $Id$

#include "tao/Environment.h"
#include "tao/ORB_Core.h"
#include "tao/Exception.h"

#if !defined (__ACE_INLINE__)
# include "tao/Environment.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Environment, "$Id$")

#if 0
CORBA_Environment::CORBA_Environment (void)
  : exception_ (0),
    previous_ (TAO_ORB_Core_instance ()->default_environment ())
{
  TAO_ORB_Core_instance ()->default_environment (this);
}

CORBA_Environment::CORBA_Environment (const CORBA_Environment& rhs)
  : exception_ (rhs.exception_),
    previous_ (TAO_ORB_Core_instance ()->default_environment ())
{
  TAO_ORB_Core_instance ()->default_environment (this);
  exception_->_incr_refcnt ();
}

CORBA_Environment::CORBA_Environment (TAO_ORB_Core* orb_core)
  : exception_ (0),
    previous_ (orb_core->default_environment ())
{
  orb_core->default_environment (this);
}

#else

CORBA_Environment::CORBA_Environment (void)
  : exception_ (0),
    previous_ (0)
{
  //  TAO_ORB_Core_instance ()->default_environment (this);
}

CORBA_Environment::CORBA_Environment (const CORBA_Environment& rhs)
  : exception_ (rhs.exception_),
    previous_ (0)
{
  //  TAO_ORB_Core_instance ()->default_environment (this);
  exception_->_incr_refcnt ();
}

CORBA_Environment::CORBA_Environment (TAO_ORB_Core* orb_core)
  : exception_ (0),
    previous_ (orb_core->default_environment ())
{
  orb_core->default_environment (this);
}
#endif

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

  // If previous is 0 then this is the first Environment, allocated
  // with the ORB, it shouldn't try to pop because the ORB is beign
  // destroyed also.
  if (this->previous_ != 0)
    TAO_ORB_Core_instance ()->default_environment (this->previous_);
}

void
CORBA_Environment::exception (CORBA_Exception *ex)
{
  if (ex != this->exception_)
    {
      this->clear ();
      this->exception_ = ex;
    }
  if (this->exception_ != 0)
    {
      this->exception_->_incr_refcnt ();
#if defined (TAO_HAS_EXCEPTIONS)
      this->exception_->_raise ();
#endif /* TAO_HAS_EXCEPTIONS */
    }
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
#if defined (TAO_HAS_EXCEPTIONS)
  //
  // If we are using native C++ exceptions the user is *not* supposed
  // to clear the environment every time she calls into TAO, in fact
  // the user is not supposed to use the environment at all!
  //
  // But TAO is using the default environment internally, thus
  // somebody has to clear it. Since TAO passes the environment around
  // this function should only be called when going from the user code
  // into TAO's code.
  //
  // This is not an issue when using the alternative C++ mapping (with
  // the Environment argument) because then the user is supposed to
  // clear the environment before calling into the ORB.
  //
  TAO_ORB_Core_instance ()->default_environment ()->clear ();
#endif /* TAO_HAS_EXCEPTIONS */

  return *TAO_ORB_Core_instance ()->default_environment ();
}

CORBA_Environment&
CORBA::default_environment ()
{
  return CORBA_Environment::default_environment ();
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

const char*
CORBA_Environment::exception_id (void) const
{
  if (this->exception_ == 0)
    return 0;

  return this->exception_->_id ();
}

// Diagnostic utility routine: describe the exception onto the
// standard I/O stream passed as a parameter.

void
CORBA::Environment::print_exception (const char *info,
                                     FILE *) const
{
  if (this->exception_)
    {
      const char *id = this->exception_->_id ();

      ACE_DEBUG ((LM_ERROR, "(%P|%t) EXCEPTION, %s\n", info));

      CORBA::SystemException *x2 =
        CORBA_SystemException::_narrow (this->exception_);

      if (x2 != 0)
        {

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
                      (x2->completed () == CORBA::COMPLETED_YES) ? "YES" :
                      (x2->completed () == CORBA::COMPLETED_NO) ? "NO" :
                      (x2->completed () == CORBA::COMPLETED_MAYBE) ? "MAYBE" :
                      "garbage"));
        }
      else
        // @@ we can use the exception's typecode to dump all the data
        // held within it ...

        ACE_DEBUG ((LM_ERROR,
                    "(%P|%t) user exception, ID '%s'\n",
                    id));
    }
  else
    ACE_DEBUG ((LM_ERROR,
                "(%P|%t) no exception\n"));
}
