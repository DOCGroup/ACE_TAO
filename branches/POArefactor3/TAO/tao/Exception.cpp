// $Id$

#include "Exception.h"
#include "SystemException.h"
#include "Environment.h"
#include "Any_SystemException.h"
#include "Any_Dual_Impl_T.h"
#include "Typecode.h"
#include "ORB_Constants.h"
#include "TC_Constants_Forward.h"
#include "CORBA_String.h"
#include "CDR.h"
#include "debug.h"

#include "ace/Malloc.h"
#include "ace/SString.h"
#include "ace/OS_NS_string.h"


#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
// Needed for ostream& operator<< (ostream &os, const CORBA::Exception &e)
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"
#endif /* (ACE_LACKS_IOSTREAM_TOTALLY) */

#if !defined (__ACE_INLINE__)
# include "tao/Exception.i"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_stdio.h"


ACE_RCSID (tao,
           Exception,
           "$Id$")


// ****************************************************************

CORBA::Exception::Exception (const char * repository_id,
                             const char * local_name)
  : id_ (CORBA::string_dup (repository_id)),
    name_ (CORBA::string_dup (local_name))
{
  ACE_ASSERT (this->id_.in () != 0 && this->name_.in () != 0);
}

CORBA::Exception::Exception (const CORBA::Exception &src)
  : id_ (CORBA::string_dup (src.id_)),
    name_ (CORBA::string_dup (src.name_))
{
  ACE_ASSERT (this->id_.in () != 0 && this->name_.in () != 0);
}

// NOTE: It's this code, not anything defined in a subclass, which is
// responsible for releasing any storage owned by the exception.  It
// can do this because it's got the local name and the id.

CORBA::Exception::Exception (void)
  : id_ (),
    name_ ()
{
}

CORBA::Exception::~Exception (void)
{
}

CORBA::Exception &
CORBA::Exception::operator= (const CORBA::Exception &src)
{
  this->id_ = CORBA::string_dup (src.id_);
  ACE_ASSERT (this->id_.in () != 0);

  this->name_ = CORBA::string_dup (src.name_);
  ACE_ASSERT (this->name_.in () != 0);

  return *this;
}

const char *
CORBA::Exception::_rep_id (void) const
{
  return this->id_.in ();
}

const char *
CORBA::Exception::_name (void) const
{
  return this->name_.in ();
}

CORBA::TypeCode_ptr
CORBA::Exception::_type (void) const
{
  return CORBA::TypeCode::_nil ();
}

int
CORBA::Exception::_is_a (const char* repository_id) const
{
  return ACE_OS::strcmp (repository_id,
                         "IDL:omg.org/CORBA/Exception:1.0") == 0;
}



void
CORBA::Exception::_tao_print_exception (const char *user_provided_info,
                                        FILE *) const
{
  ACE_DEBUG ((LM_ERROR,
              ACE_TEXT ("(%P|%t) EXCEPTION, %s\n")
              ACE_TEXT ("%s\n"),
              ACE_TEXT_CHAR_TO_TCHAR (user_provided_info),
              ACE_TEXT_CHAR_TO_TCHAR (this->_info ().c_str ())));
}

#if defined (ACE_USES_WCHAR)
void
CORBA::Exception::_tao_print_exception (const ACE_WCHAR_T *info,
                                        FILE *f) const
{
  // Even though this call causes additional type conversions, this is
  // better for the maintenance.  Plus, this will occur only on
  // exception anyway.
  this->_tao_print_exception (ACE_TEXT_ALWAYS_CHAR (info), f);
}
#endif  // ACE_USES_WCHAR

void
CORBA::Exception::_tao_any_destructor (void *x)
{
  CORBA::Exception *tmp = static_cast<CORBA::Exception *> (x);
  delete tmp;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

namespace CORBA
{
  ostream& operator<< (ostream &os,
                       const CORBA::Exception &e)
  {
    os << e._name () << " (" << e._rep_id () << ')';

    return os;
  }

  ostream& operator<< (ostream &os,
                       const CORBA::Exception *e)
  {
    os << e->_name () << " (" << e->_rep_id () << ')';

    return os;
  }
}

#endif /* (ACE_LACKS_IOSTREAM_TOTALLY) */


#if defined (TAO_DONT_CATCH_DOT_DOT_DOT)
TAO_DONT_CATCH::TAO_DONT_CATCH (void)
{}
#endif /* TAO_DONT_CATCH_DOT_DOT_DOT */

// Specializations for CORBA::Exception Any operators.

ACE_TEMPLATE_SPECIALIZATION
void
TAO::Any_Dual_Impl_T<CORBA::Exception>::value (
    const CORBA::Exception & val
  )
{
  this->value_ = val._tao_duplicate ();
}

ACE_TEMPLATE_SPECIALIZATION
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

ACE_TEMPLATE_SPECIALIZATION
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

// This should never get called since we don't have extraction operators
// for CORBA::Exception, but it is here to sidestep the constructor call
// in the unspecialized version that causes a problem with compilers that
// require explicit instantiation
ACE_TEMPLATE_SPECIALIZATION
CORBA::Boolean
TAO::Any_Dual_Impl_T<CORBA::Exception>::extract (
    const CORBA::Any &,
    _tao_destructor,
    CORBA::TypeCode_ptr,
    const CORBA::Exception *&
  )
{
  return 0;
}

// =======================================================================

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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::Any_Dual_Impl_T<CORBA::Exception>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO::Any_Dual_Impl_T<CORBA::Exception>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
